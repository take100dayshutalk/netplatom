#include "Logindlg.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication app(argc,argv);
    Logindlg mywidget;
    mywidget.exec();
    qDebug()<<g_token;
    return app.exec();

}
