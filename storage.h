#ifndef STORAGE_H
#define STORAGE_H
#include <QString>
#include <QJsonObject>

class Storage
{
public:
    Storage();
    bool validLocalJson();
    void saveJsonFile();
    void readJsonFile();
    void setFaOnInit(bool);
    void setFaOnHidden(bool);
    void setIsIdleOnFaOnly(bool);
    bool getFaOnInit();
    bool getFaOnHidden();
    bool getIsIdleOnFaOnly();
    ~Storage();
private:
    QString fileName;
    QString filePath;
    bool faOnInit;
    bool faOnHidden;
    bool isIdleOnFaOnly;
};

#endif // STORAGE_H
