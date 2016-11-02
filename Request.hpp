#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Configuration.hpp"

class Request
{
	public:

		Request();
		Request(uint src_num_, uint rq_num_, double time_in_);

		uint get_src_num() const;
		uint get_rq_num() const;
		double get_time_in() const;

	private:

		uint src_num, rq_num;
		double time_in;

};

#endif
