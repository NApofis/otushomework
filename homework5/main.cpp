//
// Created by ntikhonov on 20.07.24.
// MVC
//

#include "controllers/document_handler.h"
#include "controllers/export_import.h"
#include "views/viewer.h"

void create_document(DocumentHandler& d)
{
    std::cout << "User create document" << std::endl;
    d.create_document("black", 100, 100);
}

std::shared_ptr<models::Square> add_shape(DocumentHandler& d)
{
    std::cout << "User create square" << std::endl;
    return d.add_square(0, 0, 70, 70);
}

void remove_shape(DocumentHandler& d, std::shared_ptr<models::IShape> s)
{
    std::cout << "User remove square" << std::endl;
    d.remove_shape(s);
}

void export_document(DocumentHandler& d)
{
    std::cout << "User export document to file" << std::endl;
    auto exporter = get_exporter(ExportImportTypes::file);
    exporter->save(d.get_document());
}
void import_document(DocumentHandler& d)
{
    std::cout << "User import document from file" << std::endl;
    auto importer = get_importer(ExportImportTypes::file);
    d.set_document(importer->load());
}


int main()
{
    DocumentHandler h_document;
    WindowsViewer viewer;
    create_document(h_document);
    viewer.show_document(h_document.get_document());

    auto shape = add_shape(h_document);
    viewer.show_shape(h_document.get_document());

    remove_shape(h_document, shape);
    viewer.show_shape(h_document.get_document());

    export_document(h_document);
    import_document(h_document);
    viewer.show_document(h_document.get_document());
    return 0;
}