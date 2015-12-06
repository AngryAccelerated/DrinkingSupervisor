#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "windows.h"
#include "winable.h"
#include "QTimer"
#include "QTime"
#include "QTableWidget"
#include "QString"
#include "QDebug"
#include "QTimeEdit"
#include "QLineEdit"
#include "QMessageBox"
#define table ui->twMain
#define Cups ui->spxCups
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Drinking Superisor");
    timeString << "时间" << "是否喝了水";//初始化提示
}
void MainWindow::InitializeTable()
{
    cups = Cups->value();                //确定喝水次数
    table->setColumnCount(2);
    table->setRowCount(cups);
    table->setHorizontalHeaderLabels(timeString);
    for(int j=0;j<cups;j++)
    {
        QTimeEdit * widgets = new QTimeEdit();//创建时间编辑器类
        QLineEdit * lineEdit = new QLineEdit();//创建行编辑器类
        lineEdit->setEnabled(false);lineEdit->setText("否");//设置行编辑器属性
        lineEdit->setAlignment(Qt::AlignCenter);//设置文本对齐
        table->setCellWidget(j,0,widgets);//把时间编辑器类加入到表格
        table->setCellWidget(j,1,lineEdit);//把航编辑器类加入到控件
    }
}
bool MainWindow::CheckTime()
{
    int Nums = 0;
    for(int j=0;j<cups;j++)
    {
        QTimeEdit * timeEdit = (QTimeEdit *)table->cellWidget(j,0);
        if(timeEdit->text() != "0:00")Nums++;
    }
    return Nums == cups ? true : false;
}
void MainWindow::StartSupervise()
{
    ui->btnOK->text() == "开始计划！" ? BeginPlan() : EndPlan();
    /*if(ui->btnOK->text() == "开始计划！")
    else if(ui->btnOK->text() == "停止计划！")*/
}
void MainWindow::CheckingTime()
{
    for(int i=0;i<cups;i++)
    {
        QLineEdit * lineEdit = (QLineEdit *)table->cellWidget(i,1);//转换控件
        QTimeEdit * timeEdit = (QTimeEdit *)table->cellWidget(i,0);//转换控件.得到设置时间
        QString tString = QTime::currentTime().toString();//得到现在时间字符串
        QString SString = timeEdit->time().toString();//得到设置时间字符串
        tString = tString.left(tString.lastIndexOf(":"));//把时间调整为 时:分
        SString = SString.left(SString.lastIndexOf(":"));//把时间调整为 时:分
        CheckFinished();
        if(QString::compare(tString,SString) == 0 && lineEdit->text() == "否")//检测时间是否相等并且没有喝水的时段
        {
            int lockTime = ui->spTime->value();
            QMessageBox::information(this,"Title","第"+QString::number(i + 1)+"阶段喝水时间到,为了保证喝水和休息,我们将会锁定鼠标和键盘"+QString::number(lockTime / 1000)+"秒");
            locking = new QTimer();
            connect(locking,SIGNAL(timeout()),this,SLOT(LockedMK()));//链接信号槽,这个是时间到的话才执行的
            lineEdit->setText("是");
            LockMK();//锁上键盘鼠标,并且改变ComboBox的状态
            locking->start(lockTime);//开始计时器,运行间隔为n秒一次
        }
    }
}
void MainWindow::EndPlan()
{
    qDebug() << "停止计划！";
    ui->btnOK->setText("开始计划！");
    ui->btnInit->setEnabled(true);
    ui->twMain->setEnabled(true);
    start->stop();
}
void MainWindow::BeginPlan()
{
    if(CheckTime())
    {
        qDebug() << "符合计划";
        start = new QTimer();
        connect(start,SIGNAL(timeout()),this,SLOT(CheckingTime()));
        start->start(800);
        ui->btnOK->setText("停止计划！");
        ui->btnInit->setEnabled(false);
        ui->twMain->setEnabled(false);
    }
    else
    {
        qDebug() << "不符合计划！请把所有的时间填写完整！";
        QMessageBox::information(this,"Title","不符合计划！请把所有的时间填写完整！");
    }
}
void MainWindow::LockedMK()
{
    qDebug() << "解锁！";
    UnLockMK();//解锁
    CheckFinished();
    drinkcup++;
    locking->stop();
    QMessageBox::information(this,"Title","解锁时间到,可以自行操作");
}
void MainWindow::LockMK()
{
    this->hide();
    label = new QLabel();
    label->setText("喝水有益健康哦");
    label->setFont(QFont("微软雅黑",60));
    label->setAlignment(Qt::AlignCenter);
    label->showFullScreen();
    qDebug() << "锁定！";
    BlockInput(true);//锁定！
}
void MainWindow::CheckFinished()
{
    if(drinkcup == cups)//如果喝的杯数达到上限杯数,则
    {
        QMessageBox::information(this,"Title","今天的喝水完成了！(*^__^*) 嘻嘻……祝您身体健康！点击确定软件自动关闭！");
        this->close();
    }
}
void MainWindow::UnLockMK()
{
    label->close();
    delete label;
    this->show();
    BlockInput(false);//解锁！
}
MainWindow::~MainWindow()
{
    delete ui;
}


