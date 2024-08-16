//
// Created by ntikhonov on 15.08.24.
//

#ifndef EXPORTIIMPORT_H
#define EXPORTIIMPORT_H

enum class ExportImportTypes : uint8_t
{
    file, db, other
};


class IImport
{
public:
    virtual ~IImport() = default;
    virtual std::shared_ptr<models::document> load() = 0;
};

class ImportFromFile : public IImport
{
public:
    void check()
    {
        std::cout << "Check file to import" << std::endl;
    }
    std::shared_ptr<models::document> load() override
    {
        check();
        std::cout << "Import from file" << std::endl;
        return std::make_shared<models::document>("White", 200, 200);
    }
};

class IExport
{
public:
    IExport() = default;
    virtual ~IExport() = default;
    virtual void save(std::weak_ptr<models::document>) = 0;
};

class ExportToFile : public IExport
{
public:
    void prepare()
    {
        std::cout << "Prepare file to export" << std::endl;
    }
    void save(std::weak_ptr<models::document>) override
    {
        prepare();
        std::cout << "Export to file" << std::endl;
    }
};

std::shared_ptr<IExport> get_exporter(ExportImportTypes t)
{
    switch (t)
    {
    case ExportImportTypes::file:
        {
            return std::make_shared<ExportToFile>();
        }

    case ExportImportTypes::db:
        {
            break;
        }
    default:
        {
            break;
        }
    }
    return nullptr;
}

std::shared_ptr<IImport> get_importer(ExportImportTypes t)
{
    switch (t)
    {
    case ExportImportTypes::file:
        {
            return std::make_shared<ImportFromFile>();
        }
    case ExportImportTypes::db:
        {
            break;
        }
    default:
        {
            break;
        }
    }
    return nullptr;
}

#endif //EXPORTIIMPORT_H
