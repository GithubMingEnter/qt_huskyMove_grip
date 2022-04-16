#ifndef UR5_H
#define UR5_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QFileDialog>
#include <QTimer>
#include <QDialog>
#include <explain.h>
#include <string.h>
using namespace std;

namespace Ui {
class UR5;
}

class UR5 : public QMainWindow
{
    Q_OBJECT

public:
    explicit UR5(QWidget *parent = 0);
    ~UR5();
   // uint getbitu(const unsigned char *buff, int pos, int len);
    double HexToDouble(uint8_t* buf);
    QString DoubleToString(double value);

private slots:
    void on_openExplainSlot();

    void on_connectBtn_clicked();

    void socket_29999_ReadData();

    void socket_30003_ReadData();

    void on_sendBtn_clicked();

    void on_port30003Reconnect();

    void socket_Disconnected();

    void on_clearRecvBtn_clicked();

    void on_clearSendBtn_clicked();

    void readArmFeedbackMsg();

    void updateDisplayMsg();

    void on_moveSpeedSetBtn_valueChanged(int value);

    void on_moveAccSpeedBtn_valueChanged(int value);

    QString movel(double x,double y,double z,double Rx,double Ry,double Rz,float a,float v,float t,float r);
    QString movel_pose_add(double x,double y,double z,double Rx,double Ry,double Rz);
    QString movej(double j0,double j1,double j2,double j3,double j4,double j5,float a,float v,float t,float r);

    void stopl();

    void stopj();
    void on_toZeroBtn_pressed();

    void on_toHomeBtn_pressed();

    void on_power_offBtn_clicked();

    void on_power_onBtn_clicked();

    void on_shutdownBtn_clicked();

    void on_unlockProtectStop_clicked();

    void on_pauseBtn_clicked();

    void on_openFileBtn_clicked();

    void on_writeToFileBtn_clicked();

    void on_addEmptyFunctionBtn_clicked();

    void on_addCurrentPositionBtn_clicked();

    void on_addStopFunctionBtn_clicked();

    void on_addSleepFunctionBtn_clicked();

    void on_addToHomeFunctionBtn_clicked();

    void on_addWaitPositionBtn_clicked();

    void on_addUncoverPositionBtn_clicked();

    void on_playBtn_clicked();

    void on_stopBtn_clicked();

    void on_enableBtn_clicked();
    void on_teachModBtn_clicked();


    void on_MoveX_0_pressed();

    void on_MoveX_1_pressed();

    void on_MoveY_0_pressed();

    void on_MoveY_1_pressed();

    void on_MoveZ_0_pressed();

    void on_MoveZ_1_pressed();

    void on_PoseRx_0_pressed();

    void on_PoseRx_1_pressed();

    void on_PoseRy_0_pressed();

    void on_PoseRy_1_pressed();

    void on_PoseRz_0_pressed();

    void on_PoseRz_1_pressed();

    void on_J0_0_pressed();

    void on_J0_1_pressed();

    void on_J1_0_pressed();

    void on_J1_1_pressed();

    void on_J2_0_pressed();

    void on_J2_1_pressed();

    void on_J3_0_pressed();

    void on_J3_1_pressed();

    void on_J4_0_pressed();

    void on_J4_1_pressed();

    void on_J5_0_pressed();

    void on_J5_1_pressed();

private:
    Ui::UR5 *ui;
    QFile file;
    explain *myExplainDlg;
    QTcpSocket *socket_29999;
    QTcpSocket *socket_30001;
    QTcpSocket *socket_30003;
    QTimer *myTimer;
    uint8_t Recv_buf[1108];
    double actual_X,actual_Y,actual_Z;
    double actual_Rx,actual_Ry,actual_Rz;

    double actJoint_Base;
    double actJoint_Shoulder;
    double actJoint_Elbow;
    double actJoint_Wrist1;
    double actJoint_Wrist2;
    double actJoint_Wrist3;
    QString str_movej;
    QString str_movel;
    QString str_movel_add;
    QString str_dashboard;
    QString str_29999;
    float move_v;
    float move_a;
    double robot_Mod;
    double safe_Mod;
    double robot_voltage;
    double robot_current;
    int  timeCnt=0;
};

#endif // UR5_H
