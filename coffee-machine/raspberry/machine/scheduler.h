#ifndef BJ_SCHEDULER_H
#define BJ_CHEDULER_H

#define STOP_COUNT  4
#include "manager.h"
#include "config.h"

COMPONENT(bj, Scheduler)

public:
    enum Result{OK=0, BUSY, OUT_OF_LIQUID};
    void start();
    Result makeDrink(uint16_t door, const std::vector<std::pair<uint64_t, double>>& ingredients);
    Result checkOrder(uint16_t door, const std::vector<std::pair<uint64_t, double>>& ingredients);
    void valveClosed(uint16_t tank);
    void motorStopped(uint16_t door, uint32_t position);
    void reset();
private:
    enum Mode{FREE=0, FORWARD, BACKWARD};

    struct Action
    {
        Tank::pointer back;
        Tank::pointer front;
        double vBack;
        double vFront;
        uint32_t distance;
        bool pouring;
        bool done(){return ((vBack + vFront) == 0.0);}
        void openValves();
    };

    struct Robot
    {
        Motor::pointer motor;
        Action actions[STOP_COUNT];
        Mode mode;
        Robot* other;
        int16_t lockIndex;

        Result createPlan(const std::vector<std::pair<uint64_t, double>>& ingredients);
        void moveNext();
        void valveClosed(uint16_t tank);
        void motorStopped(uint32_t position);
        void switchToBackward();
        void reset();
        bool isUnlocked(int16_t index);
    };

    Robot left_, right_;
    boost::mutex mutex_;

COMPONENT_END

#endif // BJ_SCHEDULER_H
