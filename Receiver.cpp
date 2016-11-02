#include "Receiver.hpp"

#include <cstdlib>
#include <stdexcept>
#include <cmath>

#define E 2.718282

Receiver::Receiver(uint rcv_num_, double lambda_)
    : rcv_num(rcv_num_), busy_until(0),
      accepted(0), downtime(0), worktime(0), lambda(lambda_)
{}

void Receiver::push(double time)
{
    if (time < 0)
    {
        throw std::invalid_argument("Wrong time value!");
    }
    if (busy_until > time)
    {
        throw std::runtime_error("Receiver is busy");
    }

    int step = 0;
    double exp = std::exp(-lambda);
    double x = (double)rand() / RAND_MAX;
    while (x > exp)
    {
        x -= exp;
        step++;
        exp *= lambda / step;
    }

    busy_until = time + step;
    worktime += step;

    accepted++;
}

bool Receiver::is_busy(double at_time) const
{
    if (at_time < 0)
    {
        throw std::invalid_argument("Wrong time value!");
    }
    return (at_time < busy_until);
}

uint Receiver::get_accepted() const
{
    return accepted;
}

double Receiver::get_busy_until() const
{
    return busy_until;
}

double Receiver::get_downtime() const
{
    return downtime;
}

double Receiver::get_worktime() const
{
    return worktime;
}

void Receiver::upd_downtime(double till)
{
    if (till < 0)
    {
        throw std::invalid_argument("Wrong time value!");
    }
    if (till > busy_until)
    {
        downtime += till - busy_until;
        busy_until = till;
    }
}
