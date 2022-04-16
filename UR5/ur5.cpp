#include "ur5.h"
#include "ui_ur5.h"
#include <QMessageBox>
#include <QString.h>
#include "iostream"
#include"iomanip"
#include <sstream>
UR5::UR5(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UR5)
{
    ui->setupUi(this);

    socket_29999 = new QTcpSocket();
    socket_30001 = new QTcpSocket();
    socket_30003 = new QTcpSocket();
    myTimer= new QTimer(this);

    connect(socket_29999,&QTcpSocket::readyRead, this,&UR5::socket_29999_ReadData);
    connect(socket_30003,&QTcpSocket::readyRead, this,&UR5::socket_30003_ReadData);
    connect(socket_30003,&QTcpSocket::disconnected, this,&UR5::socket_Disconnected);
 // connect(socket_30003,&QTcpSocket::readyRead, this,&UR5::readArmFeedbackMsg);
    connect(myTimer,&QTimer::timeout,this,&UR5::updateDisplayMsg);
    connect(myTimer,&QTimer::timeout,this,&UR5::on_port30003Reconnect);

    connect(ui->explain,SIGNAL(triggered()),this,SLOT(on_openExplainSlot()));
    connect(ui->MoveX_0,&QPushButton::released,this,&UR5::stopl);
    connect(ui->MoveX_1,&QPushButton::released,this,&UR5::stopl);
    connect(ui->MoveY_0,&QPushButton::released,this,&UR5::stopl);
    connect(ui->MoveY_1,&QPushButton::released,this,&UR5::stopl);
    connect(ui->MoveZ_0,&QPushButton::released,this,&UR5::stopl);
    connect(ui->MoveZ_1,&QPushButton::released,this,&UR5::stopl);
    connect(ui->PoseRx_0,&QPushButton::released,this,&UR5::stopl);
    connect(ui->PoseRx_1,&QPushButton::released,this,&UR5::stopl);
    connect(ui->PoseRy_0,&QPushButton::released,this,&UR5::stopl);
    connect(ui->PoseRy_1,&QPushButton::released,this,&UR5::stopl);
    connect(ui->PoseRz_0,&QPushButton::released,this,&UR5::stopl);
    connect(ui->PoseRz_1,&QPushButton::released,this,&UR5::stopl);
    connect(ui->toZeroBtn,&QPushButton::released,this,&UR5::stopl);
    connect(ui->toHomeBtn,&QPushButton::released,this,&UR5::stopl);

    connect(ui->J0_0,&QPushButton::released,this,&UR5::stopj);
    connect(ui->J0_1,&QPushButton::released,this,&UR5::stopj);
    connect(ui->J1_0,&QPushButton::released,this,&UR5::stopj);
    connect(ui->J1_1,&QPushButton::released,this,&UR5::stopj);
    connect(ui->J2_0,&QPushButton::released,this,&UR5::stopj);
    connect(ui->J2_1,&QPushButton::released,this,&UR5::stopj);
    connect(ui->J3_0,&QPushButton::released,this,&UR5::stopj);
    connect(ui->J3_1,&QPushButton::released,this,&UR5::stopj);
    connect(ui->J4_0,&QPushButton::released,this,&UR5::stopj);
    connect(ui->J4_1,&QPushButton::released,this,&UR5::stopj);
    connect(ui->J5_0,&QPushButton::released,this,&UR5::stopj);
    connect(ui->J5_1,&QPushButton::released,this,&UR5::stopj);

    ui->moveSpeedSetBtn->setMaximum(1500);
    ui->moveSpeedSetBtn->setMinimum(1);
    ui->moveSpeedSetBtn->setSingleStep(1);
    ui->moveSpeedSetBtn->setValue(1000);

    ui->moveAccSpeedBtn->setMaximum(1000);
    ui->moveAccSpeedBtn->setMinimum(1);
    ui->moveAccSpeedBtn->setSingleStep(1);
    ui->moveAccSpeedBtn->setValue(400);

    ui->sendBtn->setEnabled(false);
    ui->serverIp->setText("192.168.119.131");
}

UR5::~UR5()
{
    delete ui;
    delete this->socket_29999;
    delete this->socket_30001;
    delete this->socket_30003;

}

void UR5::on_openExplainSlot()
{

    myExplainDlg=new explain(this);
    myExplainDlg->show();;
    qDebug()<<tr("打开说明");
}

void UR5::on_connectBtn_clicked()
{
    if(ui->connectBtn->text() == tr("开始连接"))
       {
           QString IP;
           //获取IP地址
           IP = ui->serverIp->text();

           //取消已有的连接
           socket_29999->abort();
           socket_30001->abort();
           socket_30003->abort();
           //连接服务器
           socket_29999->connectToHost(IP, 29999);
           socket_30001->connectToHost(IP, 30001);
           socket_30003->connectToHost(IP, 30003);

           //等待连接成功
           if(!socket_29999->waitForConnected(1000)&&!socket_30003->waitForConnected(1000))
           {
               qDebug() << "Connection failed!";
               QMessageBox::information(this, tr("警告"),tr("网络连接失败!"));
            // return;
           }
           else
           {
               qDebug() << "Connect successfully!";
               //发送按键使能
               ui->sendBtn->setEnabled(true);
               //修改按键文字
               ui->connectBtn->setText("断开连接");
               ui->connectBtn->setStyleSheet("color:red");
               myTimer->start(200);

           }
       }
       else
       {
           //断开连接
           socket_29999->disconnectFromHost();
           socket_30001->disconnectFromHost();
           socket_30003->disconnectFromHost();

           socket_29999->abort();
           socket_30001->abort();
           socket_30003->abort();

           socket_30003->close();
           //修改按键文字
           ui->connectBtn->setText("开始连接");
           ui->connectBtn->setStyleSheet("color:black");
           ui->sendBtn->setEnabled(false);
           myTimer->stop();
           timeCnt=0;

       }
}

void UR5::on_port30003Reconnect()  //超时1s 30003端口未接收到数据开始重连
{
    QString IP;
    if(timeCnt<5)
    {
       timeCnt++;
       return;
    }
    //获取IP地址
    IP = ui->serverIp->text();
    socket_30003->abort();
    socket_30003->connectToHost(IP, 30003);
    if(!socket_30003->waitForConnected(1000))
    {
        qDebug() << "Part30003 Reconnection failed!";
        ui->connectBtn->setText("开始连接");
        ui->connectBtn->setStyleSheet("color:black");
        return;
    }
    qDebug() << "Part_30003 Reconnection successfully!";
    timeCnt=0;
}

void UR5::socket_Disconnected()
{
    //发送按键失能
    ui->sendBtn->setEnabled(false);
    //修改按键文字
    ui->power_onBtn->setStyleSheet("color:black");
    ui->power_offBtn->setStyleSheet("color:black");
    ui->enableBtn->setStyleSheet("color:black");
    ui->unlockProtectStop->setStyleSheet("color:black");
    ui->teachModBtn->setText("示教模式");
    ui->teachModBtn->setStyleSheet("color:black");

   // QMessageBox::information(this, tr("警告"),tr("网络连接失败!"));
    qDebug() << "Disconnected!";
}

void UR5::on_sendBtn_clicked()
{
    QString str;
    qDebug() << "Send: " << ui->textSenddata->toPlainText();
    str=ui->textSenddata->toPlainText();
    str+='\n';
    //获取文本框内容并以ASCII码形式发送
    bool sendAsDashBoard=ui->dashBoard->isChecked();
    if(!sendAsDashBoard)
     {
        socket_30001->write(str.toLatin1());
        socket_30001->flush();
     }
    else
     {
        socket_29999->write(str.toLatin1());
        socket_29999->flush();
        qDebug()<<str;

     }

}


void UR5::socket_29999_ReadData()
{
    //读取缓冲区数据
    str_29999 = socket_29999->readAll();
    if(!str_29999.isEmpty())
      {
           QString str = ui->textRecvdata->toPlainText();
           str+=str_29999;
           //刷新显示
           ui->textRecvdata->setText(str);
      }
}


void UR5::socket_30003_ReadData()
{
    static int m=0;
    QByteArray buffer;
    bool isDashBoardMod=ui->dashBoard->isChecked();

    //读取缓冲区数据
    buffer = socket_30003->readAll();
    //qDebug()<<buffer.length();
    if(buffer.length()!=1108) return; 
    timeCnt=0;
    if(m<5){m++;return;} m=0;
    memcpy(Recv_buf,buffer,1108);
    readArmFeedbackMsg();
    if(isDashBoardMod) return;
    bool isRecvAsHex=ui->recvAsHex->isChecked();
    if(!isRecvAsHex)
    {
        ui->textRecvdata->clear();
        ui->textRecvdata->setText(str_29999);
    }
    else
    {
        QString str=buffer.toHex();
        ui->textRecvdata->clear();
        ui->textRecvdata->setText(str);
        /*
        //QByteArray temp = mySerial->readAll();
        QDataStream out(&buffer,QIODevice::ReadWrite);    //将字节数组读入
        while(!out.atEnd())
        {
            qint8 outChar = 0;
            out>>outChar;   //每字节填充一次，直到结束
            //十六进制的转换
            QString str = QString("%1").arg(outChar & 0xFF,2,16,QLatin1Char('0'));
            ui->textRecvdata->insertPlainText(str.toUpper());//大写
            ui->textRecvdata->insertPlainText(" ");//每发送两个字符后添加一个空格
            ui->textRecvdata->moveCursor(QTextCursor::End);
        }
        */
    }

}


void UR5::on_clearRecvBtn_clicked()
{
    ui->textRecvdata->clear();
}

void UR5::on_clearSendBtn_clicked()
{
    ui->textSenddata->clear();
}



void UR5::readArmFeedbackMsg()
{
    uint8_t tmp[12][8]={0};
    uint8_t buf1[48]={0};
    uint8_t buf2[48]={0};
    uint8_t buf3[8]={0};
    uint8_t buf4[8]={0};
    uint8_t buf5[8]={0};
    uint8_t buf6[8]={0};

    memcpy(buf1,&Recv_buf[252],48);
    memcpy(buf2,&Recv_buf[444],48);
    memcpy(buf3,&Recv_buf[812],8);
    memcpy(buf4,&Recv_buf[756],8);
    memcpy(buf5,&Recv_buf[980],8);
    memcpy(buf6,&Recv_buf[988],8);

    memcpy(&tmp[0][0],&buf1[0],8);
    memcpy(&tmp[1][0],&buf1[8],8);
    memcpy(&tmp[2][0],&buf1[16],8);
    memcpy(&tmp[3][0],&buf1[24],8);
    memcpy(&tmp[4][0],&buf1[32],8);
    memcpy(&tmp[5][0],&buf1[40],8);

    memcpy(&tmp[6][0],&buf2[0],8);
    memcpy(&tmp[7][0],&buf2[8],8);
    memcpy(&tmp[8][0],&buf2[16],8);
    memcpy(&tmp[9][0],&buf2[24],8);
    memcpy(&tmp[10][0],&buf2[32],8);
    memcpy(&tmp[11][0],&buf2[40],8);

    actJoint_Base=HexToDouble(tmp[0]);
    actJoint_Shoulder=HexToDouble(tmp[1]);
    actJoint_Elbow=HexToDouble(tmp[2]);
    actJoint_Wrist1=HexToDouble(tmp[3]);
    actJoint_Wrist2=HexToDouble(tmp[4]);
    actJoint_Wrist3=HexToDouble(tmp[5]);

    actual_X=HexToDouble(tmp[6]);  //m
    actual_Y=HexToDouble(tmp[7]);
    actual_Z=HexToDouble(tmp[8]);
    actual_Rx=HexToDouble(tmp[9]);
    actual_Ry=HexToDouble(tmp[10]);
    actual_Rz=HexToDouble(tmp[11]);
   // qDebug()<<actual_Rx<<"  "<<actual_Ry<<"  "<<actual_Rz<<endl;

    safe_Mod=HexToDouble(buf3);
    robot_Mod=HexToDouble(buf4);
    robot_voltage=HexToDouble(buf5);
    robot_current=HexToDouble(buf6);
}

void UR5::updateDisplayMsg()
{
    ui->actJoint_Base->setText(QString::number((180.0/3.141592)*actJoint_Base,'g',6));
    ui->actJoint_shoulder->setText(QString::number((180.0/3.141592)*actJoint_Shoulder,'g',6));
    ui->actJoint_Elbow->setText(QString::number((180.0/3.141592)*actJoint_Elbow,'g',6));
    ui->actJoint_Wrist1->setText(QString::number((180.0/3.141592)*actJoint_Wrist1,'g',6));
    ui->actJoint_Wrist2->setText(QString::number((180.0/3.141592)*actJoint_Wrist2,'g',6));
    ui->actJoint_Wrist3->setText(QString::number((180.0/3.141592)*actJoint_Wrist3,'g',6));

    ui->value_X->setText(QString::number(1000*actual_X,'g',6)); //mm
    ui->value_Y->setText(QString::number(1000*actual_Y,'g',6));
    ui->value_Z->setText(QString::number(1000*actual_Z,'g',6));
    ui->value_Rx->setText(QString::number(actual_Rx,'g',6));
    ui->value_Ry->setText(QString::number(actual_Ry,'g',6));
    ui->value_Rz->setText(QString::number(actual_Rz,'g',6));

    ui->voltage->setText(QString::number(robot_voltage,'g',4));
    ui->current->setText(QString::number(robot_current,'g',4));
   // qDebug()<<"数据已经更新--------";
 //安全模式
   if(safe_Mod>0&&safe_Mod<10)
   {
     ui->safetyMod->setCurrentIndex(safe_Mod);
    if(safe_Mod==1)
     {
         ui->safetyMod->setStyleSheet("color:black");
         ui->unlockProtectStop->setStyleSheet("color:black");
     }
    else
     {
        ui->safetyMod->setStyleSheet("color:red");
        ui->unlockProtectStop->setStyleSheet("color:red");

     }
    if(ui->safetyMod->currentIndex()==9)
     {
        qDebug() << "机械臂强制停止，需重启系统";
      //  QMessageBox::information(this, tr("警告"),tr("机械臂强制停止，请重启机械臂系统！"));
     }
   }
   else
     ui->safetyMod->setCurrentIndex(0);
//机器人模式
   if(robot_Mod<9)
   {
       ui->robotMod->setCurrentIndex(robot_Mod);
      if(robot_Mod==7)
       {
           ui->robotMod->setStyleSheet("color:black");
       }
      else
       {
          ui->robotMod->setStyleSheet("color:red");
       }
      if(ui->robotMod->currentIndex()==3)
       {
          ui->power_onBtn->setStyleSheet("color:red");
       }
       else
          ui->power_onBtn->setStyleSheet("color:black");
      if(ui->robotMod->currentIndex()==5)
       {
          ui->enableBtn->setStyleSheet("color:red");
       }
      else
          ui->enableBtn->setStyleSheet("color:black");
   }
   else
       ui->robotMod->setCurrentIndex(9);
}

QString  UR5::DoubleToString(double value)
{
    stringstream ss;
    // 默认浮点数只输出6位有效数字，这里改成小数点后6位
    ss.setf(ios::fixed);
    ss << setprecision(16) << value;
    return QString::fromStdString(ss.str());
}



double UR5::HexToDouble(uint8_t* buf)
{
    union
    {
       uint8_t data[8];
       double value;
    }type_m;
    int8_t tmp[8]={0};
    tmp[0]=*(buf+7);
    tmp[1]=*(buf+6);
    tmp[2]=*(buf+5);
    tmp[3]=*(buf+4);
    tmp[4]=*(buf+3);
    tmp[5]=*(buf+2);
    tmp[6]=*(buf+1);
    tmp[7]=*buf;
    memcpy(type_m.data,tmp,8);
    return type_m.value;

}


void UR5::on_moveSpeedSetBtn_valueChanged(int value)
{
    move_v=float(0.0001f*value);
}

void UR5::on_moveAccSpeedBtn_valueChanged(int value)
{
    move_a=float(0.0001f*value);
}

QString UR5::movel(double x,double y,double z,double Rx,double Ry,double Rz,float a,float v,float t,float r)
{

   // QString str="movel(p[x,y,z,Rx,Ry,Rz],a=a,v=v,t=t,r=r)";
    QString str="movel(p[";
    QString str_x = DoubleToString(x);
    str=str+str_x+",";
    QString str_y = DoubleToString(y);
    str=str+str_y+",";
    QString str_z = DoubleToString(z);
    str=str+str_z+",";
    QString str_Rx = DoubleToString(Rx);
    str=str+str_Rx+",";
    QString str_Ry = DoubleToString(Ry);
    str=str+str_Ry+",";
    QString str_Rz = DoubleToString(Rz);
    str=str+str_Rz+"],a=";
   // QString str_a= QString("%1").arg(a);
    QString str_a= QString::number(a, 'f', 4);  //保留小数点后4位
    str=str+str_a+",v=";
  //  QString str_v= QString("%1").arg(v);
    QString str_v= QString::number(v, 'f', 4);  //保留小数点后4位
    str=str+str_v+",t=";
    QString str_t= QString("%1").arg(t);
    str=str+str_t+",r=";
    QString str_r= QString("%1").arg(r);
    str=str+str_r+")";
    str+="\n";
    return str;
}

QString UR5::movel_pose_add(double x,double y,double z,double Rx,double Ry,double Rz)
{

    QString str="movel(pose_add(p[";
    QString str1=movel(actual_X,actual_Y,actual_Z,actual_Rx,actual_Ry,actual_Rz,move_a,move_v,0,0);
    str1.remove(0,6);
    QString str_p_x = DoubleToString(x);
    str=str+str_p_x+",";
    QString str_p_y = DoubleToString(y);
    str=str+str_p_y+",";
    QString str_p_z = DoubleToString(z);
    str=str+str_p_z+",";
    QString str_p_Rx = DoubleToString(Rx);
    str=str+str_p_Rx+",";
    QString str_p_Ry = DoubleToString(Ry);
    str=str+str_p_Ry+",";
    QString str_p_Rz = DoubleToString(Rz);
    str=str+str_p_Rz+"],";
    str+=str1;
    str.insert(str.length()-28,")");
    return str;
}

QString UR5::movej(double j0,double j1,double j2,double j3,double j4,double j5,float a,float v,float t,float r)
{

    QString str="movej([";
    QString str_j0 = DoubleToString(j0);
    str=str+str_j0+",";
    QString str_j1 = DoubleToString(j1);
    str=str+str_j1+",";
    QString str_j2 = DoubleToString(j2);
    str=str+str_j2+",";
    QString str_j3 = DoubleToString(j3);
    str=str+str_j3+",";
    QString str_j4 = DoubleToString(j4);
    str=str+str_j4+",";
    QString str_j5 = DoubleToString(j5);
    str=str+str_j5+"],a=";
    QString str_a= QString("%1").arg(a);
    str=str+str_a+",v=";
    QString str_v= QString("%1").arg(v);
    str=str+str_v+",t=";
    QString str_t= QString("%1").arg(t);
    str=str+str_t+",r=";
    QString str_r= QString("%1").arg(r);
    str=str+str_r+")";
    str+="\n";
    return str;
}


void UR5::on_toZeroBtn_pressed()
{
   str_movej= movej(0,-1.570796,0,-1.570796,0,0,move_a,move_v,0.0,0.0);
   qDebug()<<str_movej;
   socket_30001->write(str_movej.toLatin1());
   socket_30001->flush();
}


void UR5::on_toHomeBtn_pressed()
{
    str_movej= movej(-0.822733,-0.0554016,-2.8409,-1.82472,-1.58164,-3.11727,move_a,move_v,0.0,0.0);
    qDebug()<<str_movej;
    socket_30001->write(str_movej.toLatin1());
    socket_30001->flush();
}

void UR5::stopl()
{
    str_movel="stopl(1.2)\n";
    qDebug()<<str_movel;
    socket_30001->write(str_movel.toLatin1());
    socket_30001->flush();
}
void UR5::stopj()
{
    str_movej="stopj(1.4)\n";
    qDebug()<<str_movej;
    socket_30001->write(str_movej.toLatin1());
    socket_30001->flush();
}


void UR5::on_power_offBtn_clicked()
{
     str_dashboard="power off\n";
     qDebug()<< str_dashboard;
     socket_29999->write(str_dashboard.toLatin1());
     socket_29999->flush();
}

void UR5::on_power_onBtn_clicked()
{
    str_dashboard="power on\n";
    qDebug()<< str_dashboard;
    socket_29999->write(str_dashboard.toLatin1());
    socket_29999->flush();
}

void UR5::on_shutdownBtn_clicked()
{
    str_dashboard="shutdown\n";
    qDebug()<< str_dashboard;
    socket_29999->write(str_dashboard.toLatin1());
    socket_29999->flush();
}

//暂停当前程序
void UR5::on_pauseBtn_clicked()
{
    str_dashboard="pause\n";
    qDebug()<< str_dashboard;
    socket_29999->write(str_dashboard.toLatin1());
    socket_29999->flush();
}

void UR5::on_playBtn_clicked()
{
    str_dashboard="play\n";
    qDebug()<< str_dashboard;
    socket_29999->write(str_dashboard.toLatin1());
    socket_29999->flush();
}

void UR5::on_stopBtn_clicked()
{
    str_dashboard="stop\n";
    qDebug()<< str_dashboard;
    socket_29999->write(str_dashboard.toLatin1());
    socket_29999->flush();
}
void UR5::on_enableBtn_clicked()
{
    str_dashboard="brake release\n";
    qDebug()<< str_dashboard;
    socket_29999->write(str_dashboard.toLatin1());
    socket_29999->flush();
}

void UR5::on_unlockProtectStop_clicked()
{
    str_dashboard="unlock protective stop\n";
    qDebug()<< str_dashboard;
    socket_29999->write(str_dashboard.toLatin1());
    socket_29999->flush();
    str_dashboard="close popup\n";
    qDebug()<< str_dashboard;
    socket_29999->write(str_dashboard.toLatin1());
    socket_29999->flush();
}

void UR5::on_teachModBtn_clicked()
{

    str_movej="def setFreedrive():\n freedrive_mode()\n while(True):\n sync()\n end\nend\n";
    qDebug()<<str_movej;
    socket_30001->write(str_movej.toLatin1());
    socket_30001->flush();

}

void UR5::on_openFileBtn_clicked()
{
    QString filename=QFileDialog::getOpenFileName(this,tr("打开文件"),"./",tr("program (*.txt *.xml)"));
    if(filename.isEmpty())
     {
         QMessageBox::information(this, tr("提示"),tr("空文件!"));
         return;
     }
    else
    {
       // QFile file(filename);
        file.setFileName(filename);
        if(!file.open(QFile::ReadOnly | QFile::Text))
          {
               QMessageBox msg;
               msg.setText("error !");
               msg.show();
          }
        QTextStream in(&file);
        ui->textSenddata->setText(in.readAll());
    }
}



void UR5::on_writeToFileBtn_clicked()
{
     // QFile file("E:/UR5/test.txt");
      //定义文件内容字符串
      QString content= ui->textSenddata->toPlainText();
       //判断文件是否存在
      if(!file.exists())
        {
            QMessageBox::warning(this,"创建文件","请打开需要写入的文件！");
            return;
        }
      else
        {
            //存在打开，不存在创建
            file.open(QIODevice::ReadWrite | QIODevice::Text);
            //写入内容,这里需要转码，否则报错。
            QByteArray str = content.toUtf8();
            //写入QByteArray格式字符串
            file.write(str);
            //提示成功
            QMessageBox::warning(this,"文件写入","文件创建成功！");
        }
        //关闭文件
        file.close();
}

void UR5::on_addEmptyFunctionBtn_clicked()
{
    QString str="def myProg():\n    sleep(1)\n\n\nend";
    ui->textSenddata->insertPlainText(str);
}

void UR5::on_addCurrentPositionBtn_clicked()
{
    QString str="    "+movej(actJoint_Base,actJoint_Shoulder,actJoint_Elbow,actJoint_Wrist1,actJoint_Wrist2,actJoint_Wrist3,move_a,move_v,0.0,0.0);
    ui->textSenddata->insertPlainText(str);
}

void UR5::on_addStopFunctionBtn_clicked()
{
    QString str="    stopj(1.2)\r\n";
    ui->textSenddata->insertPlainText(str);
}

void UR5::on_addSleepFunctionBtn_clicked()
{
    QString str="    sleep(2)\r\n";
    ui->textSenddata->insertPlainText(str);
}

void UR5::on_addToHomeFunctionBtn_clicked()
{
    QString str="    "+movej(-0.822733,-0.0554016,-2.8409,-1.82472,-1.58164,-3.11727,move_a,move_v,0.0,0.0);
    ui->textSenddata->insertPlainText(str);
}

void UR5::on_addWaitPositionBtn_clicked()
{
    QString str="    "+movej(-0.824076,-0.265129,-2.51147,-1.94419,-1.58259,-3.11613,move_a,move_v,0.0,0.0);
    ui->textSenddata->insertPlainText(str);
}

void UR5::on_addUncoverPositionBtn_clicked()
{
    QString str="    "+movej(-0.823524,-0.12578,-2.7442,-1.851,-1.58198,-3.11659,move_a,move_v,0.0,0.0);
    ui->textSenddata->insertPlainText(str);
}



void UR5::on_MoveX_0_pressed()
{
    str_movel= movel(actual_X-4.00106,actual_Y,actual_Z,actual_Rx,actual_Ry,actual_Rz,move_a,move_v,0.0,0.0);
    qDebug()<<str_movel;
    socket_30001->write(str_movel.toLatin1());
    socket_30001->flush();
}

void UR5::on_MoveX_1_pressed()
{
    str_movel= movel(actual_X+4.00106,actual_Y,actual_Z,actual_Rx,actual_Ry,actual_Rz,move_a,move_v,0.0,0.0);
    qDebug()<<str_movel;
    socket_30001->write(str_movel.toLatin1());
    socket_30001->flush();
}

void UR5::on_MoveY_0_pressed()
{
    str_movel= movel(actual_X,actual_Y-4.00106,actual_Z,actual_Rx,actual_Ry,actual_Rz,move_a,move_v,0.0,0.0);
    qDebug()<<str_movel;
    socket_30001->write(str_movel.toLatin1());
    socket_30001->flush();
}

void UR5::on_MoveY_1_pressed()
{
    str_movel= movel(actual_X,actual_Y+4.00106,actual_Z,actual_Rx,actual_Ry,actual_Rz,move_a,move_v,0.0,0.0);
    qDebug()<<str_movel;
    socket_30001->write(str_movel.toLatin1());
    socket_30001->flush();
}

void UR5::on_MoveZ_0_pressed()
{
    str_movel= movel(actual_X,actual_Y,actual_Z-4.00106,actual_Rx,actual_Ry,actual_Rz,move_a,move_v,0.0,0.0);
    qDebug()<<str_movel;
    socket_30001->write(str_movel.toLatin1());
    socket_30001->flush();
}

void UR5::on_MoveZ_1_pressed()
{
    str_movel= movel(actual_X,actual_Y,actual_Z+4.00106,actual_Rx,actual_Ry,actual_Rz,move_a,move_v,0.0,0.0);
    qDebug()<<str_movel;
    socket_30001->write(str_movel.toLatin1());
    socket_30001->flush();
}

void UR5::on_PoseRx_0_pressed()
{
    str_movel_add= movel_pose_add(0,0,0,-3.141592,0,0);
    qDebug()<<str_movel_add;
    socket_30001->write(str_movel_add.toLatin1());
    socket_30001->flush();
}

void UR5::on_PoseRx_1_pressed()
{
    str_movel_add= movel_pose_add(0,0,0,3.141592,0,0);
    qDebug()<<str_movel_add;
    socket_30001->write(str_movel_add.toLatin1());
    socket_30001->flush();
}

void UR5::on_PoseRy_0_pressed()
{
    str_movel_add= movel_pose_add(0,0,0,0,-3.141592,0);
    qDebug()<<str_movel_add;
    socket_30001->write(str_movel_add.toLatin1());
    socket_30001->flush();
}

void UR5::on_PoseRy_1_pressed()
{
    str_movel_add= movel_pose_add(0,0,0,0,3.141592,0);
    qDebug()<<str_movel_add;
    socket_30001->write(str_movel_add.toLatin1());
    socket_30001->flush();
}

void UR5::on_PoseRz_0_pressed()
{
    str_movel_add= movel_pose_add(0,0,0,0,0,-3.141592);
    qDebug()<<str_movel_add;
    socket_30001->write(str_movel_add.toLatin1());
    socket_30001->flush();
}

void UR5::on_PoseRz_1_pressed()
{
    str_movel_add= movel_pose_add(0,0,0,0,0,3.141592);
    qDebug()<<str_movel_add;
    socket_30001->write(str_movel_add.toLatin1());
    socket_30001->flush();
}

void UR5::on_J0_0_pressed()
{
    str_movej= movej(-6.283185,actJoint_Shoulder,actJoint_Elbow,actJoint_Wrist1,actJoint_Wrist2,actJoint_Wrist3,move_a,move_v,0.0,0.0);
    qDebug()<<str_movej;
    socket_30001->write(str_movej.toLatin1());
    socket_30001->flush();
}

void UR5::on_J0_1_pressed()
{
    str_movej= movej(6.283185,actJoint_Shoulder,actJoint_Elbow,actJoint_Wrist1,actJoint_Wrist2,actJoint_Wrist3,move_a,move_v,0.0,0.0);
    qDebug()<<str_movej;
    socket_30001->write(str_movej.toLatin1());
    socket_30001->flush();
}

void UR5::on_J1_0_pressed()
{
    str_movej= movej(actJoint_Base,-6.283185,actJoint_Elbow,actJoint_Wrist1,actJoint_Wrist2,actJoint_Wrist3,move_a,move_v,0.0,0.0);
    qDebug()<<str_movej;
    socket_30001->write(str_movej.toLatin1());
    socket_30001->flush();
}

void UR5::on_J1_1_pressed()
{
    str_movej= movej(actJoint_Base,6.283185,actJoint_Elbow,actJoint_Wrist1,actJoint_Wrist2,actJoint_Wrist3,move_a,move_v,0.0,0.0);
    qDebug()<<str_movej;
    socket_30001->write(str_movej.toLatin1());
    socket_30001->flush();
}

void UR5::on_J2_0_pressed()
{
    str_movej= movej(actJoint_Base,actJoint_Shoulder,-6.283185,actJoint_Wrist1,actJoint_Wrist2,actJoint_Wrist3,move_a,move_v,0.0,0.0);
    qDebug()<<str_movej;
    socket_30001->write(str_movej.toLatin1());
    socket_30001->flush();
}

void UR5::on_J2_1_pressed()
{
    str_movej= movej(actJoint_Base,actJoint_Shoulder,6.283185,actJoint_Wrist1,actJoint_Wrist2,actJoint_Wrist3,move_a,move_v,0.0,0.0);
    qDebug()<<str_movej;
    socket_30001->write(str_movej.toLatin1());
    socket_30001->flush();
}

void UR5::on_J3_0_pressed()
{
    str_movej= movej(actJoint_Base,actJoint_Shoulder,actJoint_Elbow,-6.283185,actJoint_Wrist2,actJoint_Wrist3,move_a,move_v,0.0,0.0);
    qDebug()<<str_movej;
    socket_30001->write(str_movej.toLatin1());
    socket_30001->flush();
}

void UR5::on_J3_1_pressed()
{
    str_movej= movej(actJoint_Base,actJoint_Shoulder,actJoint_Elbow,6.283185,actJoint_Wrist2,actJoint_Wrist3,move_a,move_v,0.0,0.0);
    qDebug()<<str_movej;
    socket_30001->write(str_movej.toLatin1());
    socket_30001->flush();
}

void UR5::on_J4_0_pressed()
{
    str_movej= movej(actJoint_Base,actJoint_Shoulder,actJoint_Elbow,actJoint_Wrist1,-6.283185,actJoint_Wrist3,move_a,move_v,0.0,0.0);
    qDebug()<<str_movej;
    socket_30001->write(str_movej.toLatin1());
    socket_30001->flush();
}

void UR5::on_J4_1_pressed()
{
    str_movej= movej(actJoint_Base,actJoint_Shoulder,actJoint_Elbow,actJoint_Wrist1,6.283185,actJoint_Wrist3,move_a,move_v,0.0,0.0);
    qDebug()<<str_movej;
    socket_30001->write(str_movej.toLatin1());
    socket_30001->flush();
}

void UR5::on_J5_0_pressed()
{
    str_movej= movej(actJoint_Base,actJoint_Shoulder,actJoint_Elbow,actJoint_Wrist1,actJoint_Wrist2,-6.283185,move_a,move_v,0.0,0.0);
    qDebug()<<str_movej;
    socket_30001->write(str_movej.toLatin1());
    socket_30001->flush();
}

void UR5::on_J5_1_pressed()
{
    str_movej= movej(actJoint_Base,actJoint_Shoulder,actJoint_Elbow,actJoint_Wrist1,actJoint_Wrist2,6.283185,move_a,move_v,0.0,0.0);
    qDebug()<<str_movej;
    socket_30001->write(str_movej.toLatin1());
    socket_30001->flush();
}
