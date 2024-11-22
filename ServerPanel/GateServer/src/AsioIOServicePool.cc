#include "../include/AsioIOServicePool.h"

AsioIOServicePool::~AsioIOServicePool()
{
    std::cout << R"(AsioIOServicePool has been destructed!)" << std::endl;
}

AsioIOServicePool::AsioIOServicePool(std::size_t size)
    : _nextIoContext(0), _ioContexts(size), _works(size)
{
    for (size_t i = 0; i < size; i++)
    {
        _works[i] = std::unique_ptr<boost::asio::io_context::work>(
            new boost::asio::io_context::work(_ioContexts[i]));
    }

    for (size_t i = 0; i < size; i++)
    {
        _threads.emplace_back([this, i]()
                              { _ioContexts[i].run(); });
    }
}

boost::asio::io_context &AsioIOServicePool::GetIoContext()
{
    auto current = _nextIoContext.fetch_add(1) % _ioContexts.size();
    return _ioContexts[current];
}

void AsioIOServicePool::Stop()
{
    for (auto &work : _works)
    {
        work.reset();
    }

    for (auto &thread : _threads)
    {
        thread.join();
    }
}