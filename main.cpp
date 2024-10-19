#include <QApplication>
#include <QWidget>
#include "server.h"
int main(int argc,char *argv[]){
    QApplication app(argc,argv);
    QWidget w;
    w.show();
    server se("127.0.0.1",8079);

    return app.exec();
}
