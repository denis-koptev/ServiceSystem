#ifndef SERVICE_SYSTEM_HPP
#define SERVICE_SYSTEM_HPP

#include "Request.hpp"
#include "Source.hpp"
#include "Buffer.hpp"
#include "Receiver.hpp"
#include "Configuration.hpp"

#include <vector>

class ServiceSystemUI;

class ServiceSystem
{
public:

	struct Results
	{
		uint generated,
				accepted, 
				buf_declined, 
				buf_remained, 
				system_remained;

		double downtime,
		       final_time,
		       worktime;

		Results();
		Results(uint generated_, uint accepted_,
		    uint buf_declined_, uint buf_remained_,
		    uint system_remained_, double downtime_,
		    double worktime_, double final_time_);
	};

	ServiceSystem(uint src_num, uint buff_size, uint rcv_num,
		  double src_lambda, double rcv_lambda);

	void make_step();

	uint get_accepted() const;
	uint get_declined() const;
	Results get_results() const;
	Source get_source(uint src_num) const;

	void clear();

	friend class ServiceSystemUI;

private:

	std::vector<Source> sources;
	Buffer buffer;
	std::vector<Receiver> receivers;
	uint rcv_pos; // Receiver pointer

	double downtime;

	uint next_pos(uint curr_pos, uint size) const;

	typedef std::vector<Source>::iterator src_iterator;
	typedef std::vector<Receiver>::iterator rcv_iterator;
	typedef std::vector<Source>::const_iterator src_c_iterator;
	typedef std::vector<Receiver>::const_iterator rcv_c_iterator;
};

#endif
