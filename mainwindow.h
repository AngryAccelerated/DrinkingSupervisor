#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QTableWidget>
#include <QComboBox>
#include <QLabel>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool CheckTime();
    void UnLockMK();
    void LockMK();
    void EndPlan();
    void BeginPlan();
    void CheckFinished();
public slots:
    void InitializeTable();
    void StartSupervise();
    void CheckingTime();
    void LockedMK();
private:
    Ui::MainWindow *ui;
    QStringList timeString;
    QTimer *start;
    QTimer *locking;
    QLabel *label;
    int cups = 0;
    int drinkcup = 0;
};

#endif // MAINWINDOW_H
