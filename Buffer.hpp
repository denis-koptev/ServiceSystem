#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "Request.hpp"
#include "Configuration.hpp"

#include <vector>

class Buffer
{
public:

	typedef std::vector<Request>::iterator iterator;

    Buffer();
    Buffer(uint size_);
	
    bool push(const Request & request);
    uint pop();

    Request get(); // get request erasing it from buffer

    uint get_declined() const;
    uint get_current_size() const;
    uint get_max_size() const;

    Request at(uint pos) const;

	void clear();

    uint count_remains() const;

private:
	
	std::vector<Request> requests;
    uint size;
    uint declined;
};

#endif
