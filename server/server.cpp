#include "server.h"
#include "resultcode.h"
#include "analysisdata.h"
server::server(QString ip,quint16 port){
    this->setkey("11223344");
    this->ip=ip;
    this->port=port;
    m_server.setMissingHandler([](const QHttpServerRequest &request,QHttpServerResponder &&responder){
        QString message="错误路径，无操作";
        responder.write(message.toUtf8(),NULL);

    });
    m_server.route("/api/version",[](const QHttpServerRequest& request){
        QJsonObject ver_obj;
        ver_obj.insert("version","1.1");
        ver_obj.insert("name","usermanager-server");
        ver_obj.insert("date",QDateTime::currentDateTime().toString());
        ver_obj.insert("copyright","hutalk write");
        QJsonDocument ver_do(ver_obj);
        return ver_do.toJson(QJsonDocument::Compact);
    });
    m_server.route("/api/login",[=](const QHttpServerRequest &request){
        if(request.method()==QHttpServerRequest::Method::Get) return resultcode::geterror.tojson();

        checkjson_valid_and_account

        auto db=connectionpool::instance()->openconnection();
        QSqlQuery query(db);
        query.prepare("SELECT * FROM user WHERE user_account=? AND user_password=?;");
        query.bindValue(0,obj["user_account"].toString());
        query.bindValue(1,obj["user_password"].toString());
        if(!query.exec()){
            qDebug()<<query.lastError();
            connectionpool::instance()->closeconnect(db.connectionName());
            return resultcode::Server_databaseerror.tojson();
        }
        if(!query.next()){
            connectionpool::instance()->closeconnect(db.connectionName());
            return resultcode::account_or_passworderror.tojson();
        }
        bool is_disable=query.value("is_disable").toBool();
        QString user_account=query.value("user_account").toString();
        connectionpool::instance()->closeconnect(db.connectionName());
        if(!is_disable) return resultcode::disable_login.tojson();
        else{
            cookie token(user_account,this->key);
            return resultcode::login_token(token.get());
        }
    });
    m_server.route("/api/register",[](const QHttpServerRequest& request){
        if(request.method()==QHttpServerRequest::Method::Get) return resultcode::geterror.tojson();

        checkjson_valid_and_account

        QSqlDatabase db=connectionpool::instance()->openconnection();
        QSqlQuery query(db);
        QString order("INSERT INTO user(user_account,user_password) VALUES('%1','%2');");
        order=order.arg(obj["user_account"].toString()).arg(obj["user_password"].toString());
        qDebug()<<order;
        if(!query.exec(order)){
            qDebug()<<query.lastError();
            QString errorstr=query.lastError().databaseText();
            if(errorstr.contains("UNIQUE")){
                connectionpool::instance()->closeconnect(db.connectionName());
                return resultcode::account_exist.tojson();
            }
            connectionpool::instance()->closeconnect(db.connectionName());
            return resultcode::Server_databaseerror.tojson();
        }
        /*
        可以写第二张表插入的数据

         */
        connectionpool::instance()->closeconnect(db.connectionName());
        return resultcode::sucess.tojson();
    });
    m_server.route("/api/userlist",[](const QHttpServerRequest &request){
        if(request.method()==QHttpServerRequest::Method::Post) return resultcode::posterror.tojson();
        QUrlQuery urlquery= request.query();
        int page= urlquery.queryItemValue("page").toInt();
        int pagesize=urlquery.queryItemValue("pagesize").toInt();
        if(page<0||pagesize<=0) return resultcode::urlqueryerror.tojson();
        QSqlDatabase db=connectionpool::instance()->openconnection();
        QSqlQuery query(db);
        if(!query.exec("SELECT * FROM user;")){
            qDebug()<<query.lastError();
            connectionpool::instance()->closeconnect(db.connectionName());
            return resultcode::Server_databaseerror.tojson();
        }
        QSqlRecord record=query.record();
        int count=record.count();
        QJsonArray arry;
        QJsonObject obj;
        while(query.next()){
            QJsonObject temp;
            for(int i=0;i<count;i++)
                temp.insert(record.fieldName(i),query.value(i).toString());
            arry.append(temp);
        }
        obj.insert("code",2000);
        obj.insert("message","正常访问");
        obj.insert("list",arry);
        connectionpool::instance()->closeconnect(db.connectionName());
        return QJsonDocument(obj).toJson(QJsonDocument::Compact);
    });
    m_server.route("/api/userdisable",[=](const QHttpServerRequest &request){
        if(request.method()==QHttpServerRequest::Method::Get) return resultcode::geterror.tojson();
        checkjson_valid
        auto obj=rawdata.object();
        if(!obj.contains("token")) return resultcode::notoken.tojson();
        else{
            auto token_data=obj.value("token").toVariant().toByteArray();
            analysistoken token_m(token_data,this->key);
            if(!token_m.isvalid()) return resultcode::tokenerror.tojson();
        }

        QString order="UPDATE user SET is_disable=0 WHERE user_account='%1';";
        order=order.arg(obj["user_account"].toString());
        QSqlDatabase db=connectionpool::instance()->openconnection();
        QSqlQuery query(db);
        if(!query.exec(order)){
            qDebug()<<query.lastError();
            connectionpool::instance()->closeconnect(db.connectionName());
            return resultcode::Server_databaseerror.tojson();
        }
        connectionpool::instance()->closeconnect(db.connectionName());
        return resultcode::sucess_operation.tojson();

    });
    m_server.route("/api/userable",[](const QHttpServerRequest& request){
        if(request.method()==QHttpServerRequest::Method::Get) return resultcode::geterror.tojson();
        checkjson_valid
            QJsonObject obj=rawdata.object();
        QString order="UPDATE user SET is_disable=1 WHERE user_account='%1';";
        order=order.arg(obj["user_account"].toString());
        QSqlDatabase db=connectionpool::instance()->openconnection();
        QSqlQuery query(db);
        if(!query.exec(order)){
            qDebug()<<query.lastError();
            connectionpool::instance()->closeconnect(db.connectionName());
            return resultcode::Server_databaseerror.tojson();
        }
        connectionpool::instance()->closeconnect(db.connectionName());
        return resultcode::sucess_operation.tojson();
    });
    m_server.route("/api/userdel",[](const QHttpServerRequest &request){
        if(request.method()==QHttpServerRequest::Method::Get) return resultcode::geterror.tojson();
        checkjson_valid
        QJsonObject obj=rawdata.object();
        QString order="DELETE FROM user WHERE user_account='%1';";
        order=order.arg(obj["user_account"].toString());
        QSqlDatabase db=connectionpool::instance()->openconnection();
        QSqlQuery query(db);
        if(!query.exec(order)){
            qDebug()<<query.lastError();
            connectionpool::instance()->closeconnect(db.connectionName());
            return resultcode::Server_databaseerror.tojson();
        }
        connectionpool::instance()->closeconnect(db.connectionName());
        return resultcode::sucess_operation.tojson();
    });
    m_server.route("/api/headpicture",[](const QHttpServerRequest &request,QHttpServerResponder &&responder){
        qDebug()<<"收到请求";
        qDebug()<<request.method();
        if(request.method()==QHttpServerRequest::Method::Post){
            QByteArray rawdata=request.body();
            analysisdata data(rawdata);
            QString path="D:/mixedarea/";
            QDir catalog;
            if(!catalog.exists(path)){
                qDebug()<<"路径不存在，已创建路径";
                catalog.mkpath(path);
             }
              QString filename=path+data.Filename();
              QFile file(filename);
              if(!file.open(QIODevice::WriteOnly)){
              qDebug()<<file.error();
                  responder.write(resultcode::server_loaderror.tojson(),"application/json");
        }
            file.write(data.data());
             file.close();
            responder.write( resultcode::sucess.tojson(),"application/json");
        }
        else{
            QFile file("D:/mixedarea/test.png");
            if(!file.open(QIODevice::ReadOnly)){
                qDebug()<<file.error();
                responder.write(resultcode::server_openfileerror.tojson(),"application/json");
                return ;
            }
            QByteArray data2= file.readAll();
            file.close();
            responder.writeStatusLine();
            responder.writeHeader("Content-Type","image/png");
            responder.writeHeader("Content-Length",QByteArray::number(data2.size()));
            responder.writeBody(data2);
        }


    });



    m_server.listen(QHostAddress(ip),port);
}
cookie::cookie(QString str,QString key){
    QString temp=str+"\r----------\n"+key;
    data=temp.toUtf8();
    OP();

}
void cookie::OP(){
    unsigned char temp,b1,b2;
    for(auto it=data.begin();it!=data.end();it++){
        temp=*it;
        b1=temp<<4;
        b2=temp>>4;
        *it=b1|b2;
    }
}
QByteArray& cookie::get(){
    return data;
}
void server::setkey(QString newkey){
    key=newkey;
}
