#ifndef SOURCE_HPP
#define SOURCE_HPP

#include "Request.hpp"
#include "Configuration.hpp"

#include <vector>

class Source
{
public:

    Source(uint src_num_, double lambda_);

	void accept();
	void decline();
    void generate();

    Request get_request() const;
    uint get_generated() const;
    uint get_accepted() const;
    uint get_declined() const;

private:

    Request curr_rq;
    uint src_num, accepted_num, declined_num;
    uint generated;
    double lambda;
};

#endif
