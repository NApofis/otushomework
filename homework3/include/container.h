//
// Created by ntikhonov on 20.07.24.
//

#ifndef CONTAINER_H
#define CONTAINER_H
#include <cstring>
#include <stdexcept>

template <typename T, typename _Alloc = std::allocator<T>>
class Deque;

// Структура для хранения указателей на данные
template <typename T>
struct __DeqInfo
{
    T* begin = nullptr; // Указатель на начало пачки
    T* end = nullptr; // Указатель на элемент после последнего
    __DeqInfo* next = nullptr; // Следующая пачка
    __DeqInfo* previous = nullptr; // Предыдущая пачка

    __DeqInfo() = default;
    __DeqInfo(const __DeqInfo* other)
    {
        begin = other->begin;
        end = other->end;
        previous = other->previous;
        next = other->next;
    }

    bool operator == (const __DeqInfo& other) const {return other.begin == begin && other.end == end && other.next == next && other.previous == previous; }
    bool operator != (const __DeqInfo& other) const {return other.begin != begin || other.end != end || other.next != next || other.previous != previous; }

    // Переместить текущй объект на следующую пачку
    bool front()
    {
        if(next)
        {
            *this = __DeqInfo(next);
            return true;
        }
        return false;
    }

    // Переместить текущй объект на предыдущую пачку
    bool back()
    {
        if(previous)
        {
            *this = __DeqInfo(previous);
            return true;
        }
        return false;
    }
};

// Итератор для переещения по контейнеру
template <typename T>
class __DeqIterator
{
    T* item;
    __DeqInfo<T> pack;
    using Base = Deque<T>;

    // Проверка нужно ли менять пачку на следующую
    T* check_plus(T* p)
    {
        if(p == pack.end && pack.front())
        {
            item = pack.begin;
            return item;
        }
        return p;
    }

    // Проверка нужно ли менять пачку на предыдущую
    T* check_minus(T* p)
    {
        if(p == pack.begin && pack.back())
        {
            item = pack.end - sizeof(T);
            return item;
        }
        return p;
    }

public:
    __DeqIterator(T* p, __DeqInfo<T>* i) : item(p), pack(i) {}
    __DeqIterator(T* p, __DeqInfo<T> i) : item(p), pack(i) {}


    T& operator+ (int n)
    {
        item = check_plus(item + n);
        return *item;
    }
    T& operator- (int n)
    {
        item = check_plus(item - n);
        return *check_minus(item);
    }

    T& operator++ (int)
    {
        auto result = *item;
        item = check_plus(item + 1);
        return *result;
    }
    T& operator++ ()
    {
        return operator+(1);
    }

    T& operator-- (int)
    {
        auto result = *item;
        item = check_minus(item - 1);
        return *result;
    }
    T& operator-- ()
    {
        return operator-(1);
    }

    bool operator == (const __DeqIterator& it) const { return it.item == item && it.pack == pack; }
    bool operator != (const __DeqIterator& it) const { return it.item != item || it.pack != pack; }

    T& operator* () { return *item; }
};


// Конетейнер который работает с пачками
template <typename T, typename _Alloc>
class Deque
{
    using __DeqInfAlloc = typename _Alloc::rebind<__DeqInfo<T>>::other;
    using __DI = __DeqInfo<T>;
    using __Iter = __DeqIterator<T>;
    const size_t count_t_in_pack = 10;

    _Alloc allocator;
    __DI* first_pack = nullptr;
    __DI* last_pack = nullptr;

    // Запрашивает память на count_t_in_pack объектов.
    // При создании или удалении объектов контейнер работает с запрошенной памятью
    // Способ получения памяти не зависит от типа аллокатора
    __DI* new_pack()
    {
        void* p = allocator.allocate(count_t_in_pack);

        __DeqInfAlloc i_alloc(allocator);
        auto pi = i_alloc.allocate(1);
        i_alloc.construct(pi);

        pi->begin = static_cast<T*>(p);
        pi->end = pi->begin;
        pi->next = nullptr;
        if(last_pack)
        {
            pi->previous = last_pack;
            last_pack->next = pi;
        }
        last_pack = pi;
        return pi;
    }

    // Получить пачку и позицию объекта с порядковым номером n
    std::pair<__DI, size_t> position(const size_t& n)
    {
        if(n <= 0)
        {
            throw std::runtime_error("Порядковый номер должен быть больше 0");
        }

        __DI it(first_pack);
        auto item_number = (n - 1) % count_t_in_pack;
        auto pack_number = (n - 1) / count_t_in_pack;

        for(auto i = 0; i < pack_number; i++)
        {
            if(!it.front())
            {
                throw std::out_of_range("Порядковый номер превышает размер контейнера");
            }
        }
        if((it.end - it.begin) < item_number)
        {
            throw std::out_of_range("Порядковый номер превышает размер контейнера");
        }
        return {it, item_number};
    }

public:
    Deque()
    {
        first_pack = new_pack();
    }

    Deque(std::initializer_list<T> data)
    {
        first_pack = new_pack();
        for(const auto& i : data)
        {
            if((last_pack->end - last_pack->begin) > count_t_in_pack)
            {
                new_pack();
            }
            allocator.construct(last_pack->end, i);
            ++last_pack->end;
        }
    }

    ~Deque()
    {
        // Очистить вместе с первой пачкой
        clear(true);
    };

    Deque(const Deque& other)
    {
        if(first_pack != nullptr || last_pack != nullptr)
        {
            clear();
        }

        first_pack = new_pack();
        if(other.empty())
        {
            return;
        }

        auto memory_for = [](const size_t& n) { return n * sizeof(T); };

        __DI it(other.first_pack);
        do
        {
            new_pack();
            if(!first_pack)
            {
                first_pack = last_pack;
            }

            memcpy(last_pack->begin, it.begin, memory_for(it.end - it.begin));
            last_pack->end += it.end - it.begin;
        }
        while (it.front());
    }

    Deque(Deque&& other) noexcept
    {
        first_pack = other.first_pack;
        last_pack = other.last_pack;
        other.first_pack = nullptr;
        other.last_pack = nullptr;
        allocator = other.allocator;
    }

    T& operator [](const size_t& n)
    {
        auto [it, pos] = position(n);
        return *(it.begin + pos);
    }

    Deque& operator*(Deque* p) {return *p;}

    size_t size()
    {
        size_t result = 0;
        __DI it(first_pack);
        do
        {
            result += it.end - it.begin;
        }
        while (it.front());
        return result;
    }

    bool empty() const
    {
        for (const auto& i : *this)
        {
            return false;
        }
        return true;
    }

    __Iter insert(const size_t& n, const T& value)
    {
        try
        {
            auto [it, pos] = position(n);
            allocator.construct(it.begin + pos, value);
            auto ptr = it.begin + pos;
            return __Iter(ptr, it);
        }
        catch (std::out_of_range& e)
        {
            resize(n);
            return insert(n, value);
        }
    }

    void resize(const size_t& new_size)
    {
        auto current_size = size();
        if(new_size < current_size)
        {
            return;
        }
        auto it = last_pack;
        for(auto i = current_size; i <= new_size; ++i)
        {
            if((it->end - it->begin) < count_t_in_pack)
            {
                allocator.construct(it->end, T());
                ++it->end;
            }
            else
            {
                it = new_pack();
            }
        }
    }

    template<typename ...Args>
    void emplace(Args &&...args)
    {
        if((last_pack->end - last_pack->begin) < count_t_in_pack)
        {
            allocator.construct(last_pack->end, args...);
            ++last_pack->end;
        }
        else
        {
            new_pack();
            return emplace(args...);
        }
    }

    __Iter begin() const { return {first_pack->begin, first_pack}; }
    __Iter end() const { return {last_pack->end, last_pack}; }

    void remove(const size_t& n)
    {
        auto [it, pos] = position(n);

        __Iter prev(it.begin + pos, it);
        for(auto i = __Iter(it.begin + pos + 1, it); i != end(); ++i)
        {
            *prev = std::move(*i);
            prev = i;
        }
        allocator.destroy(last_pack->end - 1);
        --last_pack->end;

        __DeqInfAlloc i_alloc(allocator);
        __DI* ptr = first_pack->next;

        while(ptr)
        {
            auto del = ptr;
            ptr = ptr->next;

            if(del->begin == del->end)
            {
                del->previous->next = del->next;
                allocator.deallocate(del->begin, count_t_in_pack);
                i_alloc.destroy(del);
                i_alloc.deallocate(del, 1);
            }
        }

        ptr = first_pack;
        do
        {
            last_pack = ptr;
            ptr = ptr->next;
        } while (ptr);
    }

    // Очистить контейнер кроме первой пачки
    void clear(bool full = false)
    {
        __DI* it = nullptr;
        bool step;
        __DeqInfAlloc i_alloc(allocator);

        do
        {
            step = false;
            auto _item = first_pack->begin;
            while (_item < first_pack->end)
            {
                auto p = static_cast<T*>(_item);
                allocator.destroy(p);
                ++_item;
            }
            allocator.deallocate(first_pack->begin, count_t_in_pack);
            it = first_pack;
            if(first_pack->next)
            {
                first_pack = first_pack->next;
                step = true;
            }
            i_alloc.destroy(it);
            i_alloc.deallocate(it, 1);
        }
        while (step);

        if (!full)
        {
            first_pack = new_pack();
        }
    }
};

#endif //CONTAINER_H
