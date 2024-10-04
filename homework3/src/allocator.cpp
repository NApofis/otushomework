//
// Created by ntikhonov on 02.08.24.
//

#include "allocator.h"


OneChunk::OneChunk(void* ptr, const size_t& n)
{
    ch = ptr;
    *ptr_2_next_chunk() = nullptr;
    *ch_ptr_2_size() = CommandBlock::inf_size(n);
    CommandBlock(ch_ptr_2_data(), n);
}

bool OneChunk::seek()
{
    const auto next = ptr_2_next_chunk();
    if(*next)
    {
        ch = *next;
        return true;
    }
    return false;
}

void OneChunk::push(const OneChunk& other)
{
    while(seek()) {}
    const auto old_val = *ptr_2_next_chunk();
    *ptr_2_next_chunk() = other.ch;
    *other.ptr_2_next_chunk() = old_val;
}

void OneChunk::pop(const OneChunk& other)
{
    do{
        if(*ptr_2_next_chunk() == other.ch)
        {
            *ptr_2_next_chunk() = *other.ptr_2_next_chunk();
            break;
        }
    }
    while(seek());
}

void* OneChunk::get_suitable_block(const size_t& n)
{
    CommandBlock best;
    const size_t block_size = CommandBlock::inf_size(n) + 1; // 1 Байт на создание минимального блока
    do
    {
        auto iter = ch_ptr_2_data();
        while(iter < (ch_ptr_2_data() + *ch_ptr_2_size()))
        {
            CommandBlock b(iter);
            iter += CommandBlock::inf_size(*b.bl_ptr_2_size());
            if(*b.ptr_2_used())
            {
                continue;
            }

            if (*b.bl_ptr_2_size() == n)
            {
                // Если нашли блок такого же размера который нам нужен
                return b.bl_ptr_2_data();
            }

            if(*b.bl_ptr_2_size() > block_size && (!best || *b.bl_ptr_2_size() < *best.bl_ptr_2_size()))
            {
                best = b;
            }
        }
    }
    while (seek());
    if(best)
    {
        // Наиболее подходящий блок (который будет больше того который нам нужен) разделяем на 2
        best.separate(n);
        *best.ptr_2_used() = true;
        return best.bl_ptr_2_data();
    }
    return nullptr;
}

void OneChunk::remove_block(const void* p, const size_t& s)
{
    CommandBlock block;
    do
    {
        auto iter = ch_ptr_2_data();
        if(p < iter || p > (iter + *ch_ptr_2_size()))
        {
            continue;
        }
        while(iter <= (ch_ptr_2_data() + *ch_ptr_2_size()))
        {
            CommandBlock previous = block;
            block = CommandBlock(iter);

            if(block.bl_ptr_2_data() != p)
            {
                iter += CommandBlock::inf_size(*block.bl_ptr_2_size());
                continue;
            }

            if(*block.bl_ptr_2_size() != s)
            {
                throw std::bad_alloc();
            }

            *block.ptr_2_used() = false;
            if(previous && !*previous.ptr_2_used())
            {
                // Соеденим с предыдущим блоком
                previous.join(block);
                block = previous;
            }

            if(block.bl_ptr_2_data() + *block.bl_ptr_2_size() >= (ch_ptr_2_data() + *ch_ptr_2_size()))
            {
                break;
            }

            CommandBlock next(block.bl_ptr_2_data() + *block.bl_ptr_2_size());
            if(!*next.ptr_2_used())
            {
                // Соеденим со следующим блоком
                block.join(next);
            }
            break;
        }
    }
    while (seek());
}

void* OneChunk::find_empty_chunk()
{
    OneChunk previous;
    do
    {
        auto iter = ch_ptr_2_data();
        bool remove = true;

        while(iter != (ch_ptr_2_data() + *ch_ptr_2_size()))
        {
            CommandBlock block(iter);
            iter += CommandBlock::inf_size(*block.bl_ptr_2_size());
            if(*block.ptr_2_used())
            {
                remove = false;
                break;
            }
        }
        if(remove && previous)
        {
            // Если это не первый кусок в цепочке тогда его можно удалить
            previous.pop(*this);
            return ch;
        }
        previous = *this;
    }
    while (seek());
    return nullptr;
}


void CommandBlock::separate(const size_t& n) const
{
    size_t old_size = *bl_ptr_2_size();
    *bl_ptr_2_size() = n;
    CommandBlock(bl_ptr_2_data() + n, old_size - inf_size(n));

}

void CommandBlock::join(const CommandBlock& other) const
{
    *bl_ptr_2_size() += inf_size(*other.bl_ptr_2_size());
}
