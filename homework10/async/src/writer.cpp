//
// Created by ntikhonov on 07.09.24.
//

#include "writer.h"

#include <block.h>
#include <iostream>
#include <fstream>

DCL_Actor_Writer* DCL_Actor_Writer::get_instance()
{
    static auto instance = new DCL_Actor_Writer();
    return instance;
}


void DCL_Actor_Writer::stop()
{
    stop_flag = true;
    file_pool_condition.notify_all();
    console_condition.notify_all();

    for(auto& thread: file_writer_pool)
    {
        if(thread.joinable())
        {
            thread.join();
        }
    }
    if(console_writer_thread.joinable())
    {
        console_writer_thread.join();
    }
}

void DCL_Actor_Writer::run_threads()
{
    file_writer_pool.emplace_back([this, name=std::to_string(file_writer_pool.size() + 1)] { write_to_file(name); });
    file_writer_pool.emplace_back([this, name=std::to_string(file_writer_pool.size() + 1)] { write_to_file(name);  });
    console_writer_thread = std::thread([this] { write_to_console(); });
}


void DCL_Actor_Writer::write(const std::string& data, const std::string& filename)
{
    std::call_once(flag_run_thread, [this]{ run_threads(); });
    {
        std::lock_guard guard(file_pool_mutex);
        file_pool_queue.emplace(filename, data);
    }
    file_pool_condition.notify_one();

    {
        std::lock_guard guard(console_mutex);
        console_queue.emplace(data);
    }
    console_condition.notify_all();
}



void DCL_Actor_Writer::write_to_console()
{
    while(!stop_flag)
    {
        std::unique_lock lock(console_mutex);
        while (!stop_flag && console_queue.empty())
        {
            console_condition.wait(lock);
        }
        if(!console_queue.empty())
        {
            std::cout << console_queue.front();
            console_queue.pop();
            std::cout.flush();
        }
    }
}


void DCL_Actor_Writer::write_to_file(const std::string& thread_number)
{
    while(!stop_flag)
    {
        std::unique_lock lock(file_pool_mutex);
        while (!stop_flag && file_pool_queue.empty())
        {
            file_pool_condition.wait(lock);
        }
        if(!file_pool_queue.empty())
        {
            std::ofstream file(thread_number + file_pool_queue.front().first);
            file << file_pool_queue.front().second;
            file_pool_queue.pop();
            file.close();
        }
    }
}
