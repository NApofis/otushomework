//
// Created by ntikhonov on 15.08.24.
//

#ifndef VIEWER_H
#define VIEWER_H

#include "models/document.h"

class IViewer
{
public:
    virtual ~IViewer() = default;
    virtual void show_document(std::weak_ptr<models::document> docum) = 0;
    virtual void show_shape(std::weak_ptr<models::document> docum) = 0;
};

class WindowsViewer : public IViewer
{
    public:
    WindowsViewer() = default;

    void show_document(const std::weak_ptr<models::document> docum) override
    {
        std::cout << "WindowsViewer draw document [" << std::string(*docum.lock()) << "]" << std::endl << std::endl;
    }

    void show_shape(const std::weak_ptr<models::document> docum) override
    {
        for(const auto& i: docum.lock()->shapes)
        {
            std::cout << "WindowsViewer draw shape points:" << i->get_points() ;
        }
        if(docum.lock()->shapes.empty())
        {
            std::cout << "WindowsViewer not found shapes for draw";
        }
        std::cout << std::endl << std::endl;
    }
};
#endif //VIEWER_H
