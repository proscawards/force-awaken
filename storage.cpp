#include "storage.h"
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QTextStream>
#include <QDir>

struct Json{
    bool faOnInit; //Force awake on initialization (Autostart)
    bool faOnHidden; //Force awake on initialization (Hidden)
    bool isIdleOnFaOnly; //Force awake ONLY during idling
    Json(
            bool _faOnInit,
            bool _faOnHidden,
            bool _isIdleOnFaOnly
    ){
        faOnInit = _faOnInit;
        faOnHidden = _faOnHidden;
        isIdleOnFaOnly = _isIdleOnFaOnly;
    }
};

Storage::Storage(){
    //Init
    this->faOnInit = false;
    this->faOnHidden = false;
    this->isIdleOnFaOnly = false;
    this->filePath = "settings/";
    this->fileName = "force_awaken.json";
    if (!validLocalJson()){saveJsonFile();}
    else{readJsonFile();}
}

//Check if local path has force_awaken.json
bool Storage::validLocalJson(){
    QDir dir;
    if (!dir.exists(this->filePath)){dir.mkpath(this->filePath);}

    if (QFile::exists(this->filePath + this->fileName)){
        return true;
    }
    return false;
}

//Create a local json file to store data
void Storage::saveJsonFile(){
    Json json(faOnInit, faOnHidden, isIdleOnFaOnly);
    QJsonObject obj, jsonObj;
    obj.insert("faOnInit", json.faOnInit);
    obj.insert("faOnHidden", json.faOnHidden);
    obj.insert("isIdleOnFaOnly", json.isIdleOnFaOnly);
    jsonObj.insert("config", obj);
    QJsonDocument jsonDoc;
    jsonDoc.setObject(jsonObj);
    QFile file (this->filePath + this->fileName);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        file.write(jsonDoc.toJson(QJsonDocument::Indented));
        file.close();
    }
}

//Read json file and save it into a json object
void Storage::readJsonFile(){
    QFile file(this->filePath + this->fileName);
    if(file.open(QIODevice::ReadOnly ))
    {
        QJsonParseError jsonError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll(), &jsonError);
        if(jsonError.error != QJsonParseError::NoError){saveJsonFile();}
        if(jsonDoc.isObject()){
            QJsonObject jsonObj = jsonDoc.object();
            QJsonObject obj = jsonObj.value("config").toObject();
            QJsonValue faOnInitJV = obj.value("faOnInit");
            QJsonValue faOnHiddenJV = obj.value("faOnHidden");
            QJsonValue isIdleOnFaOnlyJV = obj.value("isIdleOnFaOnly");
            this->faOnInit = faOnInitJV.toBool();
            this->faOnHidden = faOnHiddenJV.toBool();
            this->isIdleOnFaOnly = isIdleOnFaOnlyJV.toBool();
        }
    }
}

bool Storage::getFaOnInit(){return this->faOnInit;}
bool Storage::getFaOnHidden(){return this->faOnHidden;}
bool Storage::getIsIdleOnFaOnly(){return this->isIdleOnFaOnly;}

void Storage::setFaOnInit(bool _faOnInit){
    this->faOnInit = _faOnInit;
    saveJsonFile();
}

void Storage::setFaOnHidden(bool _faOnHidden){
    this->faOnHidden = _faOnHidden;
    saveJsonFile();
}

void Storage::setIsIdleOnFaOnly(bool _isIdleOnFaOnly){
    this->isIdleOnFaOnly = _isIdleOnFaOnly;
    saveJsonFile();
}
