#include "Source.hpp"

#include <cstdlib>
#include <stdexcept>
#include <iostream>

Source::Source(uint src_num_, double lambda_)
    : src_num(src_num_), accepted_num(0),
      declined_num(0), generated(1),
      curr_rq(src_num, 0, 0), lambda(lambda_)
{}

void Source::accept()
{
	accepted_num++;
}

void Source::decline()
{
	declined_num++;
}

void Source::generate()
{
    int step = 0;
	double exp = std::exp(-lambda);
	double x = (double)rand() / RAND_MAX;
	while (x > exp)
	{
		x -= exp;
		step++;
		exp *= lambda/step;
	}

    uint new_rq_num = curr_rq.get_rq_num() + 1;
    double new_rq_time = curr_rq.get_time_in() + step;
    curr_rq = Request(src_num, new_rq_num, new_rq_time);
	generated++;
}

Request Source::get_request() const
{
    return curr_rq;
}

uint Source::get_generated() const
{
	return generated;
}

uint Source::get_accepted() const
{
	return accepted_num;
}

uint Source::get_declined() const
{
	return declined_num;
}
