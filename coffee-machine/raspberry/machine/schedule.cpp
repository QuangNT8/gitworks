#include "comm.machine.pb.h"
#include "schedule.h"


void bj::Schedule::start()
{
    DLOG(INFO) << "Start Scheduler";
    auto manager = bj::Manager::instance();
    left_.motor = manager->leftMotor();
    left_.other = &right_;
    right_.motor = manager->rightMotor();
    right_.other = &left_;

    for (auto i=0;i<STEP_COUNT;i++)
    {
        left_.steps[i].back = manager->getTank(i);
        left_.steps[i].front = manager->getTank(i+STEP_COUNT);
        right_.steps[i].back = manager->getTank(STEP_COUNT-1-i);
        right_.steps[i].front = manager->getTank(2*STEP_COUNT-1-i);
        left_.steps[i].distance = DLEFT + DVALVE*i;
        right_.steps[i].distance = DRIGHT + DVALVE*i;
    }

    this->reset();
}

void bj::Schedule::reset()
{
    boost::mutex::scoped_lock lock(mutex_);
    left_.reset(); right_.reset();
}

bj::Schedule::Result bj::Schedule::make(uint64_t sessionid, uint16_t door, const std::vector<std::pair<uint64_t, double>>& ingredients, bool loop)
{
    boost::mutex::scoped_lock lock(mutex_);
    auto &robot = (door == bj::LEFT?left_:right_);
    robot.sessionid = sessionid;
    robot.make_loop = loop;
    return robot.plan(ingredients);
}

bj::Schedule::Result bj::Schedule::check(uint16_t door, const std::vector<std::pair<uint64_t, double>>& ingredients)
{
    auto robot = (door == bj::LEFT?left_:right_);
    if (robot.state != FREE) return BUSY;

    uint16_t count = ingredients.size();
    for (auto& it: ingredients)
    {
        for (int i=0;i<STEP_COUNT; i++)
        {
            auto& step = robot.steps[i];
            if (step.back->check(it.first, it.second) || step.front->check(it.first, it.second)) count--;
        }
    }
    if (count == 0)
    {
        return OK;
    }
    return OUT_OF_LIQUID;
}

void bj::Schedule::valveClosed(uint16_t tank)
{
    boost::mutex::scoped_lock lock(mutex_);
    Tank::pointer t = nullptr;
    for(auto i = 0; i < TANK_COUNT; i++)
    {
        t = bj::Manager::instance()->getTank(i);
        if(t->port() == tank) break;
        else t = nullptr;
    }
    if(t != nullptr)
    {
        if(left_.state != FREE) left_.valveClosed(t);
        if(right_.state != FREE) right_.valveClosed(t);
    }
}

void bj::Schedule::motorStopped(uint16_t door, uint32_t position)
{
    boost::mutex::scoped_lock lock(mutex_);
    auto &robot = (door == bj::LEFT?left_:right_);
    if(robot.state != FREE) robot.motorStopped();
}

void bj::Schedule::Step::openValves()
{
    auto manager = bj::Manager::instance();
    if (back_reserved > 0.0)
    {
        auto &tank = back;
        auto net = bj::Manager::instance()->network(tank->liquid());
        vector<double> input;
        input.push_back(tank->volume());
        input.push_back(back_reserved);
        uint16_t open_valve = static_cast<uint16_t>(net->predict(input)[0]);
        tank->open(open_valve);

        comm::machine::StateChanged state;
        auto add = state.add_tanks();
        add->set_index(tank->port());
        add->set_volume(tank->volume());
        add->set_liquidid(tank->liquid());
        add->set_pouring(true);
        manager->pushToConsole(comm::machine::StateChanged::kEXTFieldNumber, state);
    }
    if (front_reserved > 0.0)
    {
        auto &tank = front;
        auto net = bj::Manager::instance()->network(tank->liquid());
        vector<double> input;
        input.push_back(tank->volume());
        input.push_back(front_reserved);
        uint16_t open_valve = static_cast<uint16_t>(net->predict(input)[0]);
        tank->open(open_valve);

        comm::machine::StateChanged state;
        auto add = state.add_tanks();
        add->set_index(tank->port());
        add->set_volume(tank->volume());
        add->set_liquidid(tank->liquid());
        add->set_pouring(true);
        manager->pushToConsole(comm::machine::StateChanged::kEXTFieldNumber, state);
    }
}

bj::Schedule::Result bj::Schedule::Robot::plan(const std::vector<std::pair<uint64_t, double>>& ingredients)
{
    if (state != FREE) return BUSY;
    uint16_t count = ingredients.size();
    for (auto& it: ingredients)
    {
        for (int i=0;i<STEP_COUNT; i++)
        {
            auto& step = steps[i];
            if (step.back->reserve(it.first, it.second))
            {
                step.back_reserved = it.second;
                count--;
            }
            else if (step.front->reserve(it.first, it.second))
            {
                step.front_reserved = it.second;
                count--;
            }
        }
    }
    if (count == 0)
    {
        state = WAITING;
        if(other->state == FREE) next();
        return OK;
    }
    this->reset();
    return OUT_OF_LIQUID;
}

void bj::Schedule::Robot::next()
{
    if (state != WAITING) return;

    for (int i=STEP_COUNT-1; i >= 0; i--)
    {
        auto& step = steps[i];
        if (!step.done() && !step.locked)
        {
            if(safeMove(step.distance))
            {
                other->lockForward(DLINE - step.distance);
                position = step.distance;
                state = MOVING;
                motor->moveTo(position);
                break;
            }
        }
    }

    if(done())
    {
        position = 0;
        state = MOVING;
        motor->moveTo(position);
    }
}

void bj::Schedule::Robot::valveClosed(Tank::pointer tank)
{
    for (auto i = 0; i<STEP_COUNT; i++)
    {
        auto& step = steps[i];
        if (step.distance == position)
        {
            if (step.back == tank)
            {
                tank->updateVolume(tank->volume() - step.back_reserved);
                volumeChanged(tank);
                step.back_reserved = 0.0;
            }
            else if (step.front == tank)
            {
                tank->updateVolume(tank->volume() - step.front_reserved);
                volumeChanged(tank);
                step.front_reserved = 0.0;
            }
            if (step.done())
            {
                state = WAITING;
                next();
            }
            break;
        }
    }
}

void bj::Schedule::Robot::motorStopped()
{
    if (position == 0)
    {
        if (done())
        {
            state = FREE;
            other->cleanLock();
            drinkMade();
        }
    }
    else
    {
        state = STOPPED;
        for (auto i = 0; i< STEP_COUNT;i++)
        {
            auto &step = steps[i];
            if (step.distance == position)
            {
                step.openValves();
                break;
            }
        }
    }
    other->next();
}

void bj::Schedule::Robot::volumeChanged(Tank::pointer tank)
{
    auto manager = bj::Manager::instance();

    comm::machine::StateChanged state;
    auto add = state.add_tanks();
    add->set_index(tank->port());
    add->set_volume(tank->volume());
    add->set_liquidid(tank->liquid());
    add->set_pouring(false);
    manager->pushToConsole(comm::machine::StateChanged::kEXTFieldNumber, state);
}

void bj::Schedule::Robot::drinkMade()
{
    auto manager = bj::Manager::instance();
    uint32_t door = (manager->leftMotor() == motor ? bj::LEFT:bj::RIGHT);
    comm::machine::StateChanged state;
    state.set_drinkmadeondoor(door);
    manager->pushToConsole(comm::machine::StateChanged::kEXTFieldNumber, state);
    if(sessionid == 0)
    {
        if(make_loop)
        {
            if(door == bj::LEFT) plan(manager->ingredientsLeft);
            else plan(manager->ingredientsRight);
        }
    }
    else
    {
        comm::machine::OrderStateChanged event;
        event.set_state(0);
        manager->pushToOrder(sessionid, comm::machine::OrderStateChanged::kEXTFieldNumber, event);
    }
}

bool bj::Schedule::Robot::safeMove(uint32_t position)
{
    return !(!safe(DLINE - other->position) && !other->safe(DLINE - position));
}

void bj::Schedule::Robot::lockForward(uint32_t position)
{
    for (auto i = 0; i < STEP_COUNT; i++)
    {
        auto &step = steps[i];
        if (step.distance >= position) step.locked = true;
        else step.locked = false;
    }
}

void bj::Schedule::Robot::cleanLock()
{
    for (auto i = 0; i < STEP_COUNT; i++) steps[i].locked = false;
}

bool bj::Schedule::Robot::safe(uint32_t position)
{
    auto other_step = other->nearestStep();
    if(other_step != nullptr)
    {
        for (auto i = STEP_COUNT - 1; i >= 0; i--)
        {
            auto &step = steps[i];
            if (!step.done() && step.distance >= position && step.distance + other_step->distance >= DLINE) return false;
        }
    }
    return true;
}

bj::Schedule::Step* bj::Schedule::Robot::nearestStep()
{
    for (auto i = 0; i < STEP_COUNT; i++)
    {
        if (!steps[i].done()) return &steps[i];
    }
    return nullptr;
}

bool bj::Schedule::Robot::done()
{
    for (int i=0;i<STEP_COUNT; i++)
    {
        if(!steps[i].done()) return false;
    }
    return true;
}

void bj::Schedule::Robot::reset()
{
    for (int i=0;i<STEP_COUNT; i++)
    {
        auto& step = steps[i];
        step.back_reserved = 0.0;
        step.front_reserved = 0.0;
        step.locked = false;
    }
    state = FREE;
    position = 0;
    sessionid = 0;
    make_loop = false;

}
