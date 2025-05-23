// Реализация многопоточных алгоритмов, включая работу с примитивами синхронизации потоков из STL.

#include <iostream>
#include <thread>
#include <algorithm>

#include "async.h"

void create()
{
  auto h = async::connect(2);
  std::string commands = "t1-1\nt1-2\nt1-3";
  async::receive(h, commands.c_str(), commands.size());
  commands = "\n{\nt1-4";
  async::receive(h, commands.c_str(), commands.size());
  commands = "\n{\nt1-5";
  async::receive(h, commands.c_str(), commands.size());
  commands = "\nt1-6\nt1-7\nt1-8\n}\n}";
  async::receive(h, commands.c_str(), commands.size());
  commands = "t1-9";
  async::receive(h, commands.c_str(), commands.size());
  commands = "10\n";
  async::receive(h, commands.c_str(), commands.size());
  async::disconnect(h);
}

void update(void*& h)
{
  std::string commands = "t2-1\nt2-2\nt2-3\n";
  async::receive(h, commands.c_str(), commands.size());
  commands = "{\nt2-4\n";
  async::receive(h, commands.c_str(), commands.size());
  commands = "t2-5\n";
  async::receive(h, commands.c_str(), commands.size());
  commands = "}\n";
  async::receive(h, commands.c_str(), commands.size());
  async::disconnect(h);
}


int main(int, char *[]) {
  std::size_t bulk = 5;

  void* h = async::connect(bulk);
  std::thread t1(create);
  std::string commands = "main-1\n";
  async::receive(h, commands.c_str(), commands.size());
  std::thread t2([&h] { update(std::ref(h)); });
  try
  {

    commands = "main-2\nmain-3\nmain-4\nmain-5\nmain-6\n{\nmain-7\n";
    async::receive(h, commands.c_str(), commands.size());
    commands = "{\nmain-8\nmain-9\nmain-10\n}\n}\n";
    async::receive(h, commands.c_str(), commands.size());
    t1.join();
    t2.join();
    async::disconnect(h);
  }
  catch (std::exception& e)
  {
    std::cout << e.what() << std::endl;
  }


  return 0;
}