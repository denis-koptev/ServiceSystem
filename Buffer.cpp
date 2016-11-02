#include "Buffer.hpp"

#include <algorithm>
#include <limits>

Buffer::Buffer()
    : size(0), declined(0)
{}

Buffer::Buffer(uint size_)
    : size(size_), declined(0)
{}

bool Buffer::push(const Request & request)
{
    if (requests.size() < size)
    {
        requests.push_back(request);
        return true;
    }
    return false;
}

uint Buffer::pop() // return value - number of popped request's source
{
    Request pop_rq = requests.front();
    // oldest rq is first in buffer
    requests.erase(requests.begin());
    return pop_rq.get_src_num();
}

Request Buffer::get()
{
    if (requests.size() != 0)
    {
        Request rq = requests.front();
        requests.erase(requests.begin());
        return rq;
    }
    throw std::runtime_error("Buffer is empty!");
}

uint Buffer::get_declined() const
{
    return declined;
}

void Buffer::clear()
{
    requests.clear();
    declined = 0;
}

uint Buffer::count_remains() const
{
    return (uint)requests.size();
}

uint Buffer::get_current_size() const
{
    return (uint)requests.size();
}

uint Buffer::get_max_size() const
{
    return size;
}

Request Buffer::at(uint pos) const
{
    if (pos >= requests.size() || pos < 0)
    {
        throw std::invalid_argument("Wrong buffer position!");
    }
    return requests.at(pos);
}
