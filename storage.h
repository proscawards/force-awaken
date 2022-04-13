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
    void setIsIdleOnFaOnly(bool);
    bool getFaOnInit();
    bool getIsIdleOnFaOnly();
    ~Storage();
private:
    QString fileName;
    bool faOnInit;
    bool isIdleOnFaOnly;
};

#endif // STORAGE_H
