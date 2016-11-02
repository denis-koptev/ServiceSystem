#include "Request.hpp"

Request::Request()
    : src_num(0), rq_num(0), time_in(0)
{}

Request::Request(unsigned int src_num_, unsigned int rq_num_, double time_in_)
    : src_num(src_num_), rq_num(rq_num_), time_in(time_in_)
{}

uint Request::get_src_num() const
{
    return src_num;
}

uint Request::get_rq_num() const
{
    return rq_num;
}

double Request::get_time_in() const
{
	return time_in;
}
