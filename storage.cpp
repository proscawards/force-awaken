#include "storage.h"
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QTextStream>

struct Json{
    bool faOnInit; //Force awake on initialization
    bool isIdleOnFaOnly; //Force awake ONLY during idling
    Json(
            bool _faOnInit,
            bool _isIdleOnFaOnly
    ){
        faOnInit = _faOnInit;
        isIdleOnFaOnly = _isIdleOnFaOnly;
    }
};

Storage::Storage(){
    //Init
    faOnInit = false;
    isIdleOnFaOnly = false;
    fileName = "./force_awaken.json";
    if (!validLocalJson()){saveJsonFile();}
    else{readJsonFile();}
}

//Check if local path has force_awaken.json
bool Storage::validLocalJson(){
    if (QFile::exists(fileName)){
        return true;
    }
    return false;
}

//Create a local json file to store data
void Storage::saveJsonFile(){
    Json json(faOnInit, isIdleOnFaOnly);
    QJsonObject obj, jsonObj;
    obj.insert("faOnInit", json.faOnInit);
    obj.insert("isIdleOnFaOnly", json.isIdleOnFaOnly);
    jsonObj.insert("config", obj);
    QJsonDocument jsonDoc;
    jsonDoc.setObject(jsonObj);
    QFile file (fileName);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        file.write(jsonDoc.toJson(QJsonDocument::Indented));
        file.close();
    }
}

//Read json file and save it into a json object
void Storage::readJsonFile(){
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly ))
    {
        QJsonParseError jsonError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll(), &jsonError);
        if(jsonError.error != QJsonParseError::NoError){saveJsonFile();}
        if(jsonDoc.isObject()){
            QJsonObject jsonObj = jsonDoc.object();
            QJsonObject obj = jsonObj.value("config").toObject();
            QJsonValue faOnInitJV = obj.value("faOnInit");
            QJsonValue isIdleOnFaOnlyJV = obj.value("isIdleOnFaOnly");
            faOnInit = faOnInitJV.toBool();
            isIdleOnFaOnly = isIdleOnFaOnlyJV.toBool();
        }
    }
}

bool Storage::getFaOnInit(){return faOnInit;}
bool Storage::getIsIdleOnFaOnly(){return isIdleOnFaOnly;}

void Storage::setFaOnInit(bool _faOnInit){
    faOnInit = _faOnInit;
    saveJsonFile();
}

void Storage::setIsIdleOnFaOnly(bool _isIdleOnFaOnly){
    isIdleOnFaOnly = _isIdleOnFaOnly;
    saveJsonFile();
}
