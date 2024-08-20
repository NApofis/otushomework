//
// Created by ntikhonov on 15.08.24.
//

#pragma once

#include "models/document.h"

class DocumentHandler
{
    std::shared_ptr<models::document> _document;
    void add_shape(std::shared_ptr<models::IShape> shape)
    {
        _document->shapes.push_back(shape);
        std::cout << "Save shape" << std::endl;
    }

public:
    DocumentHandler() = default;

    void create_document(std::string background, unsigned int width, unsigned int height)
    {
        _document = std::make_shared<models::document>(background, width, height);
        std::cout << "Creating new document" << std::endl;
    }

    void set_document(std::shared_ptr<models::document> document)
    {
        _document = document;
        std::cout << "Set document" << std::endl;
    }

    std::weak_ptr<models::document> get_document()
    {
        std::cout << "Get document" << std::endl;
        return {_document};
    }

    std::shared_ptr<models::Square> add_square(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
    {
        auto shape = std::make_shared<models::Square>(x, y, width, height);
        std::cout << "Save shape Square" << std::endl;
        add_shape(shape);
        return shape;
    }

    std::shared_ptr<models::Triangle> add_triangle(unsigned int x, unsigned int y, unsigned int leg1, unsigned int leg2)
    {
        auto shape = std::make_shared<models::Triangle>(x, y, leg1, leg2);
        add_shape(shape);
        std::cout << "Save shape Triangle" << std::endl;
        return shape;
    }

    void remove_shape(std::shared_ptr<models::IShape> shape)
    {
        std::cout << "Remove shape" << std::endl;
        _document->shapes.remove(shape);
    }

};
