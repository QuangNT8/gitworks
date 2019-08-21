#ifndef BJ_SCHEDULE_H
#define BJ_SCHEDULE_H

#define STEP_COUNT      4
#define DLEFT           ((740+50)*1024)
#define DRIGHT          ((740+50)*1024)
#define DVALVE          (420*1024)
#define DLINE           DLEFT+DRIGHT+(STEP_COUNT-1)*DVALVE
#include "manager.h"
#include <chrono>
#include "config.h"

COMPONENT(bj, Schedule)

public:
    enum Result{OK=0, BUSY, OUT_OF_LIQUID};
    void start();
    Result check(uint16_t door, const std::vector<std::pair<uint64_t, double>>& ingredients);
    Result make(uint64_t sessionid, uint16_t door, const std::vector<std::pair<uint64_t, double>>& ingredients, bool loop = false);
    void valveClosed(uint16_t tank);
    void motorStopped(uint16_t door, uint32_t position);
    void reset();
private:
    enum State{FREE = 0, MOVING, STOPPED, WAITING};
    struct Step
    {
        Tank::pointer back;
        Tank::pointer front;
        uint32_t distance;
        double back_reserved;
        double front_reserved;
        bool locked;
        bool done(){return (back_reserved + front_reserved == 0.0);}
        void openValves();
    };

    struct Robot
    {
        Motor::pointer motor;
        Step steps[STEP_COUNT];
        State state;
        Robot* other;
        uint32_t position;
        uint64_t sessionid;
        bool make_loop;

        Result plan(const std::vector<std::pair<uint64_t, double>>& ingredients);
        void next();
        void valveClosed(Tank::pointer tank);
        void motorStopped();
        void volumeChanged(Tank::pointer tank);
        void drinkMade();
        bool safeMove(uint32_t position);
        void lockForward(uint32_t position);
        void cleanLock();
        bool safe(uint32_t position);
        bool done();
        void checkState();
        Step* nearestStep();
        void reset();
    };
    Robot left_, right_;
    boost::mutex mutex_;

COMPONENT_END

#endif // BJ_SCHEDULE_H
