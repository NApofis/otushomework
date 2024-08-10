//
// Created by ntikhonov on 20.07.24.
//


#include <iostream>
#include <vector>
#include <list>
#include <tuple>
#include <type_traits>

template <typename... Ts>
struct same_types
{
    static const bool value = true;
};

template <typename T, typename U, typename ... Ts>
struct same_types<T, U, Ts...>
{
    static const bool value = std::is_same<T, U>::value && same_types<U, Ts...>::value;
};


template <size_t N, typename... Ts>
struct check_size
{
    static const bool value = N < sizeof...(Ts) - 1;
};


template <
    template <class ... Ts> typename T,
    std::size_t N = 0,
    typename ... Ts,
    std::enable_if_t<!check_size<N, Ts...>::value, bool> = true,
    std::enable_if_t<
        std::is_same<T<Ts ...>, std::tuple<Ts ...>
    >::value && same_types<Ts ...>::value, bool> = true
>
void print_ip(T<Ts ...> ip)
{
    std::cout << std::get<N>(ip) << std::endl;
}

template <
    template <class ... Ts> typename T,
    std::size_t N = 0,
    typename ... Ts,
    std::enable_if_t<check_size<N, Ts...>::value, bool> = true,
    std::enable_if_t<
        std::is_same<T<Ts ...>, std::tuple<Ts ...>
    >::value && same_types<Ts ...>::value, bool> = true
>
void print_ip(T<Ts ...> ip)
{
    std::cout << std::get<N>(ip) << ".";
    print_ip<T, N + 1, Ts...>(ip);
}


template <class T, std::enable_if_t<
    std::is_integral<
        std::remove_reference_t<
            std::remove_const_t<T>
        >>::value, bool> = true>
void print_ip(T ip)
{
    std::vector<unsigned char> result;
    result.push_back(ip);
    for(auto i = 8; i < sizeof(T) * 8; i += 8)
    {
        const auto t = static_cast<unsigned char>(ip >> i);
        result.push_back(t);
    }
    auto it = result.rbegin();
    std::cout << +*(it++);
    while(it != result.rend())
    {
        std::cout << '.' << +*it;
        ++it;
    }
    std::cout << std::endl;
}

template <class T, std::enable_if_t<
    std::is_same<
        std::remove_reference_t<
            std::remove_const_t<T>>
        , std::string
        >::value, bool> = true>
void print_ip(T ip)
{
    std::cout << ip << std::endl;
}

template <
    template <typename, typename> typename T,
	typename Type,
	typename Allocator = std::allocator<Type>,
    std::enable_if_t<
		std::is_same<T<Type, Allocator>, std::vector<Type, Allocator>>::value ||
		    std::is_same<T<Type, Allocator>, std::list<Type, Allocator>>::value, bool> = true>

void print_ip(T<Type, Allocator> ip)
{
    bool first = true;
    for(const auto& i : ip)
    {
        if(!first)
        {
            std::cout << '.';
        }
        first = false;
        std::cout << i;
    }
    std::cout << std::endl;
}

int main()
{
    print_ip( int8_t{-1} ); // 255
    print_ip( int16_t{0} ); // 0.0
    print_ip( int32_t{2130706433} ); // 127.0.0.1
    print_ip( int64_t{8875824491850138409} );// 123.45.67.89.101.112.131.41
    print_ip( std::string{"Hello, World!"} ); // Hello, World!
    print_ip( std::vector<int>{100, 200, 300, 400} ); // 100.200.300.400
    print_ip( std::list<short>{400, 300, 200, 100} ); // 400.300.200.100
    print_ip( std::make_tuple(123, 456, 789, 0) ); // 123.456.789.0

    return 0;
}