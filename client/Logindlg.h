#ifndef LOGINDLG_H
#define LOGINDLG_H
#include <QWidget>
#include <QKeyEvent>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QCloseEvent>
#include <QPushButton>
#include <QPainter>
#include <QFile>
#include <QStaticText>
#include <QPen>
#include <QGraphicsDropShadowEffect>
#include <QImage>
#include <QStackedWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSettings>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDialog>
extern QByteArray g_token;
class Logindlg:public QDialog{
     Q_OBJECT
public:
    Logindlg(QWidget *parent=nullptr);
    ~Logindlg();
    bool eventFilter(QObject *watch,QEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;
    void paintEvent(QPaintEvent *e) override;
    void init_childwidget();
private:
    bool is_trag;
    QRect m_leftrect,m_rightrect,m_middlerect;
    QPushButton *bu_close,*bu_confirm,*bu_confirm1,*bu_towidget,*backlogin;
    QLineEdit *account,*password,*account1,*password1,*password2;
    QStaticText *lefttext;
    QStackedWidget *stackwidget;
    QComboBox *jobchose;
    QWidget *login_widget,*sign_up_widget;
    QNetworkAccessManager *networkmanager;
    QByteArray token;

};

#endif // LOGINDLG_H
