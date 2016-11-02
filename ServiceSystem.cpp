#include "ServiceSystem.hpp"

#include <algorithm>
#include <stdexcept>

ServiceSystem::ServiceSystem(uint src_num, uint buff_size, uint rcv_num,
                             double src_lambda, double rcv_lambda)
    : buffer(buff_size), rcv_pos(0)
{
    for (uint i = 0; i < src_num; ++i)
    {
        sources.push_back(Source(i, src_lambda));
        sources.at(i).generate();
    }
    for (uint i = 0; i < rcv_num; ++i)
    {
        receivers.push_back(Receiver(i, rcv_lambda));
    }
}

void ServiceSystem::make_step()
{
    Request min_rq = std::min_element(sources.begin(), sources.end(),
                                      [](const Source & lhs, const Source & rhs)
    {
        return (lhs.get_request().get_time_in() < rhs.get_request().get_time_in());
    }
                                     )->get_request();

    uint min_pos = min_rq.get_src_num();
    double min_time = min_rq.get_time_in();

    sources.at(min_pos).generate();

    uint rcvs_busy = 0;
    uint curr_pos = rcv_pos;
    while (buffer.get_current_size() != 0 && rcvs_busy != receivers.size())
    {
        if (!receivers.at(curr_pos).is_busy(min_time))
        {
            Request buff_rq = buffer.get();
            receivers.at(curr_pos).push(min_time);
            sources.at(buff_rq.get_src_num()).accept();
            rcv_pos = next_pos(curr_pos, (uint)receivers.size());
        }
        curr_pos = next_pos(curr_pos, (uint)receivers.size());

        rcvs_busy = 0;
        std::for_each(receivers.begin(), receivers.end(),
                      [&rcvs_busy, min_time](const Receiver & rcv)
        {
            if (rcv.is_busy(min_time))
            {
                ++rcvs_busy;
            }
        });
    }
    rcv_pos = curr_pos;

    for (rcv_iterator it = receivers.begin(); it != receivers.end(); ++it)
    {
        it->upd_downtime(min_time);
    }

    if (!buffer.push(min_rq))
    {
        uint pop_num = buffer.pop();
        sources.at(pop_num).decline();
        buffer.push(min_rq);
    }
}


uint ServiceSystem::get_accepted() const
{
    uint accepted = 0;
    std::for_each(sources.begin(), sources.end(),
                  [&accepted](const Source & src)
    {
        accepted += src.get_accepted();
    });
    return accepted;
}

uint ServiceSystem::get_declined() const
{
    uint declined = 0;
    std::for_each(sources.begin(), sources.end(),
                  [&declined](const Source & src)
    {
        declined += src.get_declined();
    });
    return declined;
}

ServiceSystem::Results ServiceSystem::get_results() const
{
    uint generated = 0;
    for (src_c_iterator it = sources.begin(); it != sources.end(); ++it)
    {
        generated += it->get_generated();
    }

    uint accepted = 0;
    double downtime = 0, worktime = 0;
    double final_time = receivers.at(0).get_busy_until();

    for (rcv_c_iterator it = receivers.begin(); it != receivers.end(); ++it)
    {
        accepted += it->get_accepted();
        downtime += it->get_downtime();
        worktime += it->get_worktime();
        if (it->get_busy_until() > final_time)
        {
            final_time = it->get_busy_until();
        }
    }

    uint system_remains = (uint)sources.size();

    return Results(generated, accepted, buffer.get_declined(),
                   buffer.count_remains(), system_remains, downtime,
                   worktime, final_time);
}

Source ServiceSystem::get_source(uint src_num) const
{
    return sources.at(src_num);
}

void ServiceSystem::clear()
{
    sources.clear();
    buffer.clear();
    receivers.clear();
    rcv_pos = 0;
    downtime = 0;
}

uint ServiceSystem::next_pos(uint curr_pos, uint size) const
{
    return (curr_pos >= size - 1 ? 0 : curr_pos + 1);
}

ServiceSystem::Results::Results()
    : generated(0), accepted(0), buf_declined(0),
      buf_remained(0), system_remained(0), downtime(0),
      final_time(0), worktime(0)
{}

ServiceSystem::Results::Results(uint generated_, uint accepted_,
                                uint buf_declined_, uint buf_remained_,
                                uint system_remained_, double downtime_,
                                double worktime_, double final_time_)
    : generated(generated_), accepted(accepted_),
      buf_declined(buf_declined_), buf_remained(buf_remained_),
      system_remained(system_remained_), downtime(downtime_),
      final_time(final_time_), worktime(worktime_)
{}
