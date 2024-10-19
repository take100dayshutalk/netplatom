#include "Logindlg.h"
QByteArray g_token;
Logindlg::Logindlg(QWidget *parent):QDialog(parent){
    setWindowFlags(windowFlags()|Qt::WindowType::FramelessWindowHint);
    networkmanager=new QNetworkAccessManager(this);
    is_trag=false;
    setMouseTracking(true);
    installEventFilter(this);
    resize(740,340);
    setAttribute(Qt::WidgetAttribute::WA_TranslucentBackground);
    m_rightrect={5,25,730,290};
    m_leftrect={5,5,200,330};
    m_middlerect={0,5,400,330};
    QFile file(":/style.css");
    file.open(QIODevice::ReadOnly);
    setStyleSheet(file.readAll());
    file.close();
    bu_close=new QPushButton(this);
    connect(bu_close,&QPushButton::clicked,this,[this]{close();});
    bu_close->resize(32,32);
    bu_close->move(700,25);
    bu_close->setObjectName("bu_close");
    bu_close->setFocusPolicy(Qt::FocusPolicy::NoFocus);

    lefttext=new QStaticText("Welcome");
    auto effect=new QGraphicsDropShadowEffect(this);
    effect->setOffset(0);
    effect->setColor(qRgb(126,112,150));
    effect->setBlurRadius(10);
    setGraphicsEffect(effect);
    init_childwidget();

}
Logindlg::~Logindlg(){
    delete lefttext;
    networkmanager->deleteLater();

}
void Logindlg::init_childwidget(){
    stackwidget=new QStackedWidget(this);
    stackwidget->setGeometry(410,60,320,240);
    login_widget=new QWidget;
    sign_up_widget=new QWidget;
    QVBoxLayout *login_vbox=new QVBoxLayout(login_widget);
    QVBoxLayout *signup_vbox=new QVBoxLayout(sign_up_widget);
    QHBoxLayout *login_hbox=new QHBoxLayout;
    QCheckBox *automatic_login=new QCheckBox("Automatic login");
    QCheckBox *remembered=new QCheckBox("Remember password");
    automatic_login->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    remembered->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    account=new QLineEdit;
    account->setPlaceholderText("Account");
    password=new QLineEdit;
    password->setEchoMode(QLineEdit::EchoMode::Password);
    password->setPlaceholderText("Password");

    account1=new QLineEdit;
    account1->setPlaceholderText("Account Register");
    password1=new QLineEdit;
    password1->setPlaceholderText("Password");
    password1->setEchoMode(QLineEdit::EchoMode::Password);
    password2=new QLineEdit;
    password2->setPlaceholderText("Repeat Password");
    password2->setEchoMode(QLineEdit::EchoMode::Password);

    bu_confirm=new QPushButton("Login");
    bu_confirm->setObjectName("Login");
    bu_confirm1=new QPushButton("Register");
    bu_confirm1->setObjectName("Register");
    bu_towidget=new QPushButton("No account, come and register");
    bu_towidget->setObjectName("Towidget");
    backlogin=new QPushButton("Back");
    backlogin->setObjectName("Backlogin");
    jobchose=new QComboBox;
    jobchose->addItem(QIcon(":/teacher.png"),"Teacher");
    jobchose->addItem(QIcon(":/student.png"),"Student");


    jobchose->setCurrentIndex(1);
    connect(bu_towidget,&QPushButton::clicked,[this]{this->stackwidget->setCurrentIndex(1);});
    connect(backlogin,&QPushButton::clicked,[this]{this->stackwidget->setCurrentIndex(0);});
    connect(bu_confirm,&QPushButton::clicked,[=]{
        if(password->text().isEmpty()||account->text().isEmpty()){
            QMessageBox::information(this,"Tips","请完整填写登陆账户和密码");
            return ;
        }
        QNetworkRequest request(QUrl("http://127.0.0.1:8079/api/login"));
        QJsonObject obj;
        obj.insert("user_account",account->text());
        obj.insert("user_password",password->text());
        request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
        auto reply=networkmanager->post(request,QJsonDocument(obj).toJson(QJsonDocument::Compact));
        connect(reply,&QNetworkReply::errorOccurred,[=]{
            qDebug()<<"reply error";
            qDebug()<<reply->errorString();
        });
        connect(reply,&QNetworkReply::finished,[=]{
            QByteArray data=reply->readAll();
            auto acc= QJsonDocument::fromJson(data);
            auto accept_data=acc.object();
            if(accept_data["code"].toInt()!=2000){
                QMessageBox::information(this,"Tips",accept_data["message"].toString());
                return ;
            }
            this->token=accept_data.value("token").toVariant().toByteArray();
            close();

        });

    });
    connect(bu_confirm1,&QPushButton::clicked,[this]{
        if(password1->text().isEmpty()||password2->text().isEmpty()||account1->text().isEmpty()){
            QMessageBox::information(this,"Tips","请完整填写注册账户和注册密码");
            return ;
        }
        if(password1->text()!=password2->text()){
            QMessageBox::information(this,"Tips","两次设置的密码不一致");
            return ;
        }
        QNetworkRequest request(QUrl("http://127.0.0.1:8079/api/register"));
        QJsonObject obj;
        obj.insert("user_account",account1->text());
        obj.insert("user_password",password1->text());
        request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
        auto reply= networkmanager->post(request,QJsonDocument(obj).toJson(QJsonDocument::Compact));
        connect(reply,&QNetworkReply::errorOccurred,[=]{
            qDebug()<<"reply error";
            qDebug()<<reply->errorString();
        });
        connect(reply,&QNetworkReply::finished,[=]{
            auto data=reply->readAll();
            auto acc= QJsonDocument::fromJson(data);
            auto accept_data=acc.object();
            if(accept_data["code"].toInt()!=2000){
                QMessageBox::information(this,"Tips",accept_data["message"].toString());
                return ;
            }
            account1->setText(nullptr);
            password1->setText(nullptr);
            password2->setText(nullptr);
            this->stackwidget->setCurrentIndex(0);
        });


    });


    login_vbox->addWidget(account);
    login_vbox->addWidget(password);
    login_hbox->addWidget(automatic_login);
    login_hbox->addWidget(remembered);
    login_vbox->addLayout(login_hbox);
    login_vbox->addWidget(bu_confirm);
    login_vbox->addWidget(bu_towidget);

    signup_vbox->addWidget(account1);
    signup_vbox->addWidget(password1);
    signup_vbox->addWidget(password2);
    QHBoxLayout *temp=new QHBoxLayout(sign_up_widget);
    temp->addWidget(bu_confirm1);
    temp->addWidget(backlogin);
    temp->setSpacing(0);
    signup_vbox->addWidget(jobchose);
     signup_vbox->addLayout(temp);




    stackwidget->addWidget(login_widget);
    stackwidget->addWidget(sign_up_widget);
    stackwidget->setCurrentIndex(0);

}


void Logindlg::paintEvent(QPaintEvent *e){
    QPainter pan(this);

    pan.setRenderHint(QPainter::RenderHint::Antialiasing);
    pan.fillRect(m_rightrect,qRgb(163,172,218));
    pan.setBrush(QBrush(qRgb(255,255,255)));
    QPen penori=pan.pen();
    pan.setPen(Qt::NoPen);
    pan.drawRect(m_leftrect);
    pan.drawChord(m_middlerect,270*16,180*16);
    pan.setPen(penori);
    pan.setFont(QFont("微软雅黑",40,20,false));
    pan.drawStaticText(150,100,*lefttext);
    pan.setFont(QFont("微软雅黑",20,10,true));
    lefttext->setText("LOGIN");
    pan.drawStaticText(150,160,*lefttext);
    pan.drawLine(150,191,400,191);
    pan.drawPixmap(QPoint(10,20),QPixmap(":/back.png"));

    pan.end();


}

void Logindlg::keyPressEvent(QKeyEvent *e){

    if(e->key()==Qt::Key::Key_Escape){
        close();
    }


}
bool Logindlg::eventFilter(QObject *watch,QEvent *e){
    static QPoint POS;
    switch(e->type()){
        case QEvent::Type::MouseButtonPress:{
            QMouseEvent *mouse= dynamic_cast<QMouseEvent*>(e);
                if(mouse->button()==Qt::MouseButton::LeftButton){
                is_trag=true;
                POS=mouse->pos();
                 }
        return true;
        break;
        }
        case QEvent::Type::MouseButtonRelease:{
            QMouseEvent *mouse= dynamic_cast<QMouseEvent*>(e);
            if(mouse->button()==Qt::MouseButton::LeftButton)  is_trag=false;
        return true;
        break;
        }
        case QEvent::Type::MouseMove:{
        QMouseEvent *mouse= dynamic_cast<QMouseEvent*>(e);
        if(is_trag) move(pos()+mouse->pos()-POS);
        break;
        }
        case QEvent::Type::Close:{
            g_token=this->token;
            break;
             }
        }

    return  false;
}
