//
// Created by ntikhonov on 15.08.24.
//

#ifndef SIMPLE_H
#define SIMPLE_H
#include <vector>

inline std::ostream& operator <<(std::ostream& out, const std::vector<unsigned int>& data)
{
    for (int i = 0; i < data.size(); i += 2)
    {
        out << " [" << data[i] << ", " << data[i + 1] << "]";
    }
    return out;
}

namespace models
{
    class IShape
    {

    protected:
        std::pair<unsigned int, unsigned int> position;

    public:
        virtual ~IShape() = default;

        IShape() = default;
        IShape(unsigned int x, unsigned int y) : position(x, y)
        {}

        virtual std::vector<unsigned int> get_points() = 0;
    };

    class Square : public IShape
    {
        int width = 0;
        int height = 0;
    public:
        Square() = default;
        Square(unsigned int x, unsigned int y, unsigned int width, unsigned int height) : IShape(x, y), width(width), height(height) {};
        std::vector<unsigned int> get_points() override
        {
            return {
                position.first, position.second,
                position.first + width, position.second,
                position.first, position.second + height,
                position.first + width, position.second + height
            };
        }
    };

    class Triangle : public  IShape
    {
        int leg1 = 0;
        int leg2 = 0;

    public:
        Triangle() = default;
        Triangle(unsigned int x, unsigned int y, unsigned int leg1, unsigned int leg2) : IShape(x, y), leg1(leg1), leg2(leg2) {};
        std::vector<unsigned int> get_points() override
        {
            return {
                position.first, position.second,
                position.first + leg1, position.second,
                position.first + leg1, position.second + leg2
            };
        }
    };

    class Line : public IShape
    {
        std::pair<unsigned int, unsigned int> point;

    public:
        Line() = default;
        Line(unsigned int x, unsigned int y, unsigned int x2, unsigned int y2) : IShape(x, y), point(x2, y2) {};
        std::vector<unsigned int> get_points() override
        {
            return {
                position.first, position.second,
                position.first + point.first, position.second + point.second
            };
        }
    };
}

#endif //SIMPLE_H
