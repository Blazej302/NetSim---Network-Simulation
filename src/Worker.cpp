//
// Created by blazej on 7/24/26.
//

#include "Worker.hpp"

void Worker::do_work(Time t)
{
    if (!processing_buffer_.has_value() && !q_->empty())
    {
        processing_buffer_ = q_->pop();
        processing_start_time_ = t;
    }

    if (processing_buffer_.has_value())
    {
        if (t - processing_start_time_ + 1 >= processing_duration_)
        {
            push_package(std::move(processing_buffer_.value()));
            processing_buffer_.reset();
        }
    }
}
