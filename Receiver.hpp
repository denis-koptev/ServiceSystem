#ifndef RECEIVER_HPP
#define RECEIVER_HPP

#include "Request.hpp"
#include "Configuration.hpp"

class Receiver
{
public:

    Receiver(uint rcv_num_, double lambda_);

    void push(double time);
    void upd_downtime(double till);
    // upd_downtime must be called after each cycle of extraction

	bool is_busy(double at_time) const;
    double get_busy_until() const;
    uint get_accepted() const;
    double get_downtime() const;
    double get_worktime() const;

private:

    uint rcv_num;
    double lambda;
	double busy_until;

    uint accepted;
    double downtime, worktime;
};

#endif
