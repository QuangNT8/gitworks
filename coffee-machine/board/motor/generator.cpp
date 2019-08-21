#include "generator.h"
#include "uart/controller.h"

bool motor::Generator::start(uint32_t A, uint32_t V, int32_t S)
{
    if (state_ != NONE) return false;
    if (S==0) return true;

    if (S<0)
    {
        forward_ = false;
        S = -S;
    }
    else forward_ = true;

    double tmp = (double)S/V - (double)V/A + 1;

    if (tmp < 1 )
    {
        /*TODO: case S < a*/
        uint32_t s = 0;
        n2_ = 0;
        v_ = 0;
        n1_ = 0;
        a_ = A;

        while (s < S)
        {
            v_ += a_;
            s += 2*v_;
            n1_++;
        }

        s -= S;
        r_ = s/n1_;
        count_ = s % n1_;
    }
    else
    {
        n2_ = tmp + 1;
        v_ = V;
        a_ = A;
        n1_ = v_/a_;

        int32_t s = n1_*(n1_+1)*a_ + n2_*v_ - S;
        r_ = s/(n1_+1);
        count_ = s % (n1_+1);
    }

    v = 0;
    n = 0;
    state_ = STAGE1;
    //uart::CONTROLLER.printfMessage("n1=%d, n2=%d, N=%d", n1_, n2_, n2_+n1_*2);
    return true;
}

int32_t motor::Generator::generate()
{
    uint32_t ret;
    switch (state_)
    {
    case STAGE1:
        if (n==0) v = a_ - r_;
        else v += a_;
        ret = v;
        if (n<count_) ret--;
        n++;
        if (n == n1_)
        {
            if (n2_ == 0) state_ = STAGE3;
            else state_ = STAGE2;
            n = 0;
        }
        break;

    case STAGE2:
        if (n == 0) ret = v_ - r_;
        else ret = v_;

        n++;
        if (n == n2_)
        {
            state_ = STAGE3;
            n = 0;
        }
        break;

    case STAGE3:
        if (n==0) v = n1_*a_;
        else v -= a_;

        n++;
        ret = v;
        if (n == n1_) state_ = NONE;
        break;
    case ZERO:
        ret = zeroSpeed_;
        break;
    default:
        ret = 0;
        break;
    }
    if (forward_) return ret;
    else return -ret;
}
