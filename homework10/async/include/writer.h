//
// Created by ntikhonov on 07.09.24.
//

#pragma once
#include <mutex>
#include <vector>
#include <thread>
#include <atomic>
#include <queue>
#include <condition_variable>


class DCL_Actor_Writer
{
    std::once_flag flag_run_thread;

    std::vector<std::thread> file_writer_pool;
    std::thread console_writer_thread;
    std::atomic_bool stop_flag{false};

    std::queue<std::pair<std::string, std::string>> file_pool_queue;
    std::mutex file_pool_mutex;
    std::condition_variable file_pool_condition;
    std::queue<std::string> console_queue;
    std::mutex console_mutex;
    std::condition_variable console_condition;

    static std::once_flag flag_singleton;
    static DCL_Actor_Writer* instance;
    static void create();

    DCL_Actor_Writer() = default;
    DCL_Actor_Writer(const DCL_Actor_Writer& root) = delete;
    DCL_Actor_Writer& operator=(const DCL_Actor_Writer&) = delete;
    DCL_Actor_Writer(DCL_Actor_Writer&& root) = delete;
    DCL_Actor_Writer& operator=(DCL_Actor_Writer&&) = delete;

    void write_to_file(const std::string& thread_number);
    void write_to_console();
    void run_threads();

public:

    static DCL_Actor_Writer* get_instance();
    void write(const std::string& data, const std::string& filename);
    void stop();
    ~DCL_Actor_Writer() = default;

};
