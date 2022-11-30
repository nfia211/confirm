#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "input.h"
#include <vector>
#include <string.h>
#include <QVector>
#include <QString>
using std::string;
using std::vector;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void getFiles();
    void getCodePairs();
    void showCodes();
    void updateEqual();
    void updateInEqual();
    void updateUnknown();
    bool next();
    void outFile();

private slots:
    void on_equal_clicked();

    void on_inequal_clicked();

    void on_unknown_clicked();

private:
    Ui::MainWindow *ui;

    string fatherPath="/home/cjh/input";
    Input* input;
    int curPairIdx;

    vector<vector<string>> equalpairs;
    vector<vector<string>> inequalpairs;
    vector<vector<string>> unknownpairs;
    string equalfilePath="/home/cjh/confirm_equal.csv";
    string inequalfilePath="/home/cjh/confirm_inequal.csv";
    string unknownfilePath="/home/cjh/confirm_unknown.csv";

};



#endif // MAINWINDOW_H
