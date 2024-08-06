//
// Created by ntikhonov on 20.07.24.
//

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <bits/shared_ptr_base.h>
#include <functional>
#include "iostream"

// Класс для работы с "куском" памяти.
// Структура куска | указатель на следующий кусок [void*] | размер куска [size_t] | данные кускка == разные Block [void] |
class OneChunk
{
    void* ch = nullptr;
    using next_chunck_type = void*;
    using size_type = size_t;

    inline next_chunck_type* ptr_2_next_chunk() const
    {
        return static_cast<next_chunck_type*>(ch );
    }
    inline size_type* ch_ptr_2_size() const
    {
        return static_cast<size_type*>(ch + sizeof(next_chunck_type));
    }
    inline void* ch_ptr_2_data() const
    {
        return ch + sizeof(next_chunck_type) + sizeof(size_type);
    }
    // Переместить укзатель на следующий кусок
    bool seek();

public:

    OneChunk() = default;
    explicit OneChunk(void* ptr) : ch(ptr) {}
    explicit OneChunk(void* ptr, const size_t& n);

    // Присоеденить кусок в конец текущей цепочки
    void push(const OneChunk& other);
    // Удалить кусок из цепочки
    void pop(const OneChunk& other);

    explicit operator bool() const
    {
        return ch;
    }

    // Размер памяти который необходим для созсдания "куска"
    static size_t inf_size(const size_t& n)
    {
        return n + sizeof(next_chunck_type) + sizeof(size_type);
    }


    // Найти блок (во всей цепочке) который максимально приближен по размеру к n
    void* get_suitable_block(const size_t& n);
    // Пометить что блок не используется и при необходимости объеденить его с соседними. Что бы уменьшить колличество маленьких блоков
    void remove_block(const void* p, const size_t& s);
    // Найти кусок у которого все блоки не используются. Если это не первый косок тогда его память можно будет освободить
    void* find_empty_chunk();

};

// Класс для работы с блоками который лежат в "куске" памяти.
// Структура блока | размер блока [size_t] | флаг используется блок или нет [bool] | данные блока [void] | следующий блок | ... |
class Block
{

    friend OneChunk;
    void* block = nullptr;
    using size_type = size_t;
    using used_type = bool;

    Block() = default;
    explicit Block(void* ptr) : block(ptr){};
    explicit Block(void* ptr, const size_t& n) : block(ptr)
    {
        *bl_ptr_2_size() = n;
        *ptr_2_used() = false;
    };

    inline size_type* bl_ptr_2_size() const
    {
        return static_cast<size_type*>(block);
    };
    inline used_type* ptr_2_used() const
    {
        return static_cast<used_type*>(block + sizeof(size_type));
    }
    inline void* bl_ptr_2_data() const
    {
        return block + sizeof(size_type) + sizeof(used_type);
    }

    explicit operator bool() const { return block; }

    // Соеденить два соседних блока
    void join(const Block& other) const;
    // Разделить блок на два. Один из которых будет n байт
    void separate (const size_t& n) const;


public:
    // Размер необходимый для одного блока с даннмыми размера n
    static inline size_t inf_size(const size_t& n)
    {
        return n + sizeof(size_type) + sizeof(used_type);
    }
};


template <typename T>
struct update_allocator {

    using value_type = T;

    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;

    const unsigned int count_obj_in_memory = 10;
    const unsigned int size_memery = count_obj_in_memory * sizeof(T);

    template <typename U>
    struct rebind {
        using other = update_allocator<U>;
    };

    template <typename U>
    bool operator == (const update_allocator<U> &t)
    {
        return start == t.start;
    }

    update_allocator()
    {
        start = malloc(OneChunk::inf_size(Block::inf_size(size_memery)));
        void* pp = malloc(sizeof(size_t));

        if(!start || !pp)
        {
            throw std::bad_alloc();
        }
        counter = static_cast<size_t*>(pp);
        *counter = 1;
        OneChunk(start, size_memery);
    };
    ~update_allocator()
    {
        *this->counter -= 1;
        if(*this->counter == 0)
        {
            // Если это последний диструктор объекта который работает с памятью start тогда унитожаем start.
            free(start);
        }
    };

    template <typename U>
    explicit update_allocator(const update_allocator<U> &t)
    {
        this->start = t.start;
        this->counter = t.counter;
        *t.counter += 1;
    }


    // Запросить новый кусок памяти на n байт
    void get_memory(const size_t& n) const
    {
        void* p = malloc(OneChunk::inf_size(Block::inf_size(n)));
        if(!p)
        {
            throw std::bad_alloc();
        }
        const OneChunk new_chunk(p, n);
        OneChunk(start).push(new_chunk);
    }

    pointer allocate(const std::size_t& n)
    {
        auto need_memory_size = n * sizeof(T);
        if(n > count_obj_in_memory)
        {
            // Если запроашиваемое место больше чем размер стандартного куска, тогда создадим "особый" кусок
            get_memory(need_memory_size);
        }

        auto ptr= OneChunk(start).get_suitable_block(need_memory_size);

        if(!ptr)
        {
            // Если в текущих кусках нет блока который уместил бы n объектов, тогда создаем новый кусок
            get_memory(size_memery);
            return allocate(n);
        }
        return static_cast<pointer>(ptr);
    }

    void deallocate(pointer p, const std::size_t& n)
    {
        if(!start)
        {
            throw std::bad_alloc();
        }

        OneChunk(start).remove_block(static_cast<void*>(p), n * sizeof(T));
        const auto ptr = OneChunk(start).find_empty_chunk();
        if(ptr)
        {
            // Если нашли кусок который не используется
            free(ptr);
        }
    }

    template <typename U, typename... Args>
    void construct(U *p, Args &&...args)
    {
        new (p) U(std::forward<Args>(args)...);
    }
    template <typename U>
    static void destroy(U *p)
    {
        p->~U();
    }

    void* start;
    size_t* counter;

};


#endif //ALLOCATOR_H
