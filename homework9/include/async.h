#pragma once

#include <cstddef>

namespace async {

void* connect(std::size_t bulk);
void receive(void* handle, const char *data, std::size_t size);
void disconnect(void* handle);

}
