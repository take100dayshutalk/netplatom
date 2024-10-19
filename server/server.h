#ifndef SERVER_CUSTOM
#define SERVER_CUSTOM

#include <QHttpServer>
#include <QHostAddress>
#include <QHttpServerRequest>
#include <QHttpServerResponder>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QByteArray>
#include <QIODevice>
#include <QFile>
#include <QDir>
#include "../databasepool/connectionpool.h"
class server{
public:
    server(QString ip,quint16 port);
    void setkey(QString newkey);
private:
    QHttpServer m_server;
    QString ip;
    quint16 port;
    QString key;

};
class cookie{
public:
    cookie(QString str,QString key);
    QByteArray& get();
    void OP();
private:
    QByteArray data;

};

#define checkjson_valid \
QJsonParseError error;\
auto rawdata=QJsonDocument::fromJson(request.body(),&error);\
if(error.error!=QJsonParseError::NoError)\
return resultcode::submiterror.tojson();\

#define checkjson_valid_and_account \
QJsonParseError error;\
auto rawdata=QJsonDocument::fromJson(request.body(),&error);\
if(error.error!=QJsonParseError::NoError)\
return resultcode::submiterror.tojson();\
auto obj=rawdata.object();\
if(!obj.contains("user_account")||!obj.contains("user_password")){\
    return resultcode::paramjsonInvalid.tojson();\
    qDebug()<<obj;}\



 \


#endif
