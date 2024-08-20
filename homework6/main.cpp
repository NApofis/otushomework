//
// Created by ntikhonov on 20.08.24.
//
#include <assert.h>
#include <stdexcept>
#include <map>
#include <memory>
#include <iostream>

template <typename T>
class AdaptedMatrixIterator;

template <typename T>
class IMatrix
{

public:
    using matrix_type = std::map<std::pair<int, int>, T>;

    virtual ~IMatrix() = default;

    virtual operator T() = 0;
    virtual IMatrix& operator[](unsigned int i) = 0;
    virtual IMatrix& operator=(T t) = 0;
    virtual unsigned int size() = 0;
    virtual void remove() = 0;
    virtual AdaptedMatrixIterator<T> begin() = 0;
    virtual AdaptedMatrixIterator<T> end() = 0;

};

template <typename T>
class AdaptedMatrixIterator : public IMatrix<T>::matrix_type::iterator
{
    using Type = typename IMatrix<T>::matrix_type;
    using Iterator = typename IMatrix<T>::matrix_type::iterator;
    using ResultType = std::tuple<
        std::remove_const_t< typename Type::value_type::first_type::first_type >,
        std::remove_const_t< typename Type::value_type::first_type::second_type >,
        std::remove_const_t< typename Type::value_type::second_type >
    >;


public:
    AdaptedMatrixIterator(const Iterator& it) : Iterator(it) {}


    ResultType operator* ()
    {
        auto value = IMatrix<T>::matrix_type::iterator::operator*();
        return {value.first.first, value.first.second, value.second};
    }

};

template <typename T>
class Matrix : public IMatrix<T>
{
    typename Matrix::matrix_type::key_type get_key()
    {
        auto result = std::make_pair(operator_get_x, operator_get_y);
        return result;
    }
public:
    Matrix() = default;

    void remove() override
    {
        matrix.erase(get_key());
    }

    operator T() override
    {
        T result = matrix.at(get_key());
        return result;
    }

    Matrix& operator =(T t) override
    {
        matrix[get_key()] = t;
        return *this;
    }

    Matrix& operator [](const unsigned int n) override
    {
        if(operator_get_x != -1 && operator_get_y != -1)
        {
            operator_get_x = n;
            operator_get_y = -1;
        }
        else if(operator_get_x == -1)
        {
            operator_get_x = n;
        }
        else if(operator_get_y == -1)
        {
            operator_get_y = n;
        }

        return *this;
    }

    unsigned int size() override
    {
        return matrix.size();
    }

    AdaptedMatrixIterator<T> begin() override
    {
        return {matrix.begin()};
    }

    AdaptedMatrixIterator<T> end() override
    {
        return {matrix.end()};
    }

private:
    typename Matrix::matrix_type matrix;
    long operator_get_x = -1;
    long operator_get_y = -1;
};

template <typename T, T default_value>
class ProxyMatrix : public IMatrix<T>
{
    void remove() override {};
public:
    ProxyMatrix() = default;

    operator T() override
    {
        try
        {
            T result = base;
            return result;
        }
        catch (std::out_of_range& e)
        {
            return default_value;
        }
    }

    ProxyMatrix& operator =(T t) override
    {
        if(t == default_value)
        {
            base.remove();
        }
        else
        {
            base = base.operator=(t);
        }
        return *this;
    }

    ProxyMatrix& operator [](const unsigned int i) override
    {
        base = base.operator[](i);
        return *this;
    }

    unsigned int size() override
    {
        return base.size();
    }

    AdaptedMatrixIterator<T> begin() override
    {
        return base.begin();
    }

    AdaptedMatrixIterator<T> end() override
    {
        return base.end();
    }

private:
    Matrix<T> base;
};

int main()
{
    ProxyMatrix<int, 0> matrix;
    for(int i = 0; i < 10; i++)
    {
        matrix[i][i] = i;
    }
    for(int i = 0; i < 10; i++)
    {
        matrix[i][9 - i] = 9 - i;
    }

    for(int i = 1; i < 9; i++)
    {
        for(int j = 1; j < 9; j++)
        {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl << "Size = " << matrix.size() << std::endl << std::endl;
    for(auto c : matrix)
    {
        auto [x, y, value] = c;
        std::cout << "[ " << x << ", " << y << " ] = " << value << std::endl;
    }

    ((matrix[100][100] = 314) = 0) = 217;
    assert(matrix[100][100] == 217);
    std::cout << std::endl << matrix[100][100] << std::endl;
}