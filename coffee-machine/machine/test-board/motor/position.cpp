#include "controller.h"

#define MAX_PSUM (1024LL<<10)
#define MAX_VSUM (1024LL<<22)
#define MAX_IQREF (1024LL<<20) /*Q15.16*/
#define IQ_SHIFT 11

/*output: iqRef_ (Q15.16)*/
void motor::Controller::updatePosition_(int32_t p) //1kHz
{
    /*position and speed estimation*/
    int16_t delta = p - angle_;
    if (delta > MAX_POSITION/2) delta -= MAX_POSITION;
    else if (delta < -MAX_POSITION/2) delta += MAX_POSITION;
    p_ += delta; //Q14.0
    v_ -= vFilter_[vFilterIndex_]; //Q14.4
    v_ += delta;
    vFilter_[vFilterIndex_] = delta;
    vFilterIndex_++;
    if (vFilterIndex_ >= SPEED_W_SIZE) vFilterIndex_ = 0;
    angle_ = p;

    /*profile generation*/
    int32_t nextVel = generator_.generate();

    aRef_ = nextVel - vRef_;
    vRef_ = nextVel;
    pRef_ += vRef_;

    /*position regulation*/
    int32_t ep = pRef_ - (p_<<POSITION_SHIFT);
    pSum_ += pKI_*ep;
    if (pSum_ > MAX_PSUM) pSum_ = MAX_PSUM;
    else if (pSum_ < -MAX_PSUM) pSum_ = -MAX_PSUM;

    int64_t vCmd = pSum_ + pKP_ * ep + pKD_*(ep - lastE_); //PID
    lastE_ = ep;

    /*speed regulation*/
    int32_t ev = vCmd + KVff_* vRef_ - v_;
    vSum_ += vKI_*ev;
    if (vSum_ > MAX_VSUM) vSum_ = MAX_VSUM;
    else if (vSum_ < -MAX_VSUM) vSum_ = -MAX_VSUM;

    int64_t ret = vSum_ + vKP_*ev + KAff_*aRef_; //PI
    if (ret > MAX_IQREF) iqRef_ = MAX_IQREF;
    else if (ret < -MAX_IQREF) iqRef_ = -MAX_IQREF;
    else   iqRef_ = ret;

    if (cycleCount_ >= 50) //40kHz:(25*16) = 100Hz
    {
        processCommand(PLOT_DATA);
        cycleCount_ = 0;
    }
}
