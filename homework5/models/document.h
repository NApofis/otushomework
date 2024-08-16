//
// Created by ntikhonov on 15.08.24.
//

#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <iostream>
#include <memory>
#include <list>

#include "simple.h"

namespace models
{

struct document
{
    document(std::string back, unsigned int w, unsigned int h) : background(back), width(w), height(h) {};
    std::list<std::shared_ptr<IShape>> shapes;

    std::string background;
    unsigned int width;
    unsigned int height;

    explicit operator std::string() const
    {
        return "Background: " + background + ", Size: " + std::to_string(width) + "x" + std::to_string(height);
    }

};

}

#endif //DOCUMENT_H
