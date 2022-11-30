#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <dirent.h>
#include <vector>
#include <string.h>
#include <QVector>
#include <QString>
#include <sstream>
#include <QWidget>
#include <QLabel>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QStringList>
#include <QMessageBox>
using std::string;
using std::vector;
using std::ifstream;
using std::istringstream;



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    input=new Input(fatherPath,"/home/cjh/equal.csv","/home/cjh/inequal.csv");
    curPairIdx=0;

    for(int i=0;i<input->inequalpairs.size();++i){
        //std::cout<<input->inequalpairs[i][0]<<input->inequalpairs[i][1]<<"\n";
        inequalpairs.push_back(input->inequalpairs[i]);
    }
    showCodes();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::showCodes(){

    string code1Path=fatherPath+input->equalpairs[curPairIdx][0];
    string code2Path=fatherPath+input->equalpairs[curPairIdx][1];

    //std::cout<<code1<<"  "<<code2<<"\n";

    vector<string> code1,code2;//数组code1,code2分别按行保存两个源文件中的内容
    ifstream file1(code1Path);//按行读入code1中的代码
    if(!file1.good())exit(0);
    string line;
    while(getline(file1,line))
        code1.push_back(line);
    file1.close();

    ifstream file2(code2Path);//按行读入code2中的代码
    if(!file2.good())exit(0);
    while(getline(file2,line))
        code2.push_back(line);
    file2.close();

    vector<vector<string>> finalcode;
    vector<int>index1,index2;//数组index1,index2分别为code1和code2中相同的每行代码在数组中的下标
    int i1=0,i2=0,l1=code1.size(),l2=code2.size();
    while(i1<l1&&i2<l2){
        int index=-1;
        for(int j=i1;j<l1&&index==-1;++j){
            if(code1[j]==code2[i2]){
                index=j;
            }
        }
        if(index!=-1){
            i1=index;
            while(i1<l1&&i2<l2&&code1[i1]==code2[i2]){
                index1.push_back(i1);
                index2.push_back(i2);
                i1++;i2++;
            }
        }
        else{
            i2++;
        }
    }
    i1=0;i2=0;
    for(int j=0;j<index1.size();++j){
        while(i1<index1[j]&&i2<index2[j]){
            string temp1=code1[i1],temp2=code2[i2];
            //std::cout<<temp1<<","<<temp2<<"\n";
            vector<string> temp;
            temp.push_back(temp1);
            temp.push_back(temp2);
            finalcode.push_back(temp);
            ++i1;++i2;
        }
        while(i1<index1[j]){
            string temp1=code1[i1],temp2="";
            //std::cout<<temp1<<","<<temp2<<"\n";
            vector<string> temp;
            temp.push_back(temp1);
            temp.push_back(temp2);
            finalcode.push_back(temp);
            ++i1;
        }
        while(i2<index2[j]){
            string temp1="",temp2=code2[i2];
            //std::cout<<temp1<<","<<temp2<<"\n";

            vector<string> temp;
            temp.push_back(temp1);
            temp.push_back(temp2);
            finalcode.push_back(temp);
            ++i2;
        }
        string temp1=code1[i1],temp2=code2[i2];
        //std::cout<<temp1<<","<<temp2<<"\n";

        vector<string> temp;
        temp.push_back(temp1);
        temp.push_back(temp2);
        finalcode.push_back(temp);
        ++i1;++i2;

    }
    while(i1<l1&&i2<l2){
        string temp1=code1[i1],temp2=code2[i2];
        //std::cout<<temp1<<","<<temp2<<"\n";
        vector<string> temp;
        temp.push_back(temp1);
        temp.push_back(temp2);
        finalcode.push_back(temp);
        ++i1;++i2;
    }
    while(i1<l1){
        string temp1=code1[i1],temp2=" ";
        //std::cout<<temp1<<","<<temp2<<"\n";

        vector<string> temp;
        temp.push_back(temp1);
        temp.push_back(temp2);
        finalcode.push_back(temp);
        ++i1;
    }
    while(i2<l2){
        string temp1=" ",temp2=code2[i2];
        //std::cout<<temp1<<","<<temp2<<"\n";

        vector<string> temp;
        temp.push_back(temp1);
        temp.push_back(temp2);
        finalcode.push_back(temp);
        ++i2;
    }

    ui->code->clear();

    QStringList header;
    header<<QString::fromStdString(input->equalpairs[curPairIdx][0])<<QString::fromStdString(input->equalpairs[curPairIdx][1]);
    ui->code->setHorizontalHeaderLabels(header);
    ui->code->setColumnCount(header.size());                        // 设置表格的列数
    ui->code->setHorizontalHeaderLabels(header);                    // 设置水平头
    ui->code->setRowCount(finalcode.size());                                       // 设置总行数
    ui->code->setEditTriggers(QAbstractItemView::NoEditTriggers);   // 设置表结构默认不可编辑
    ui->code->setSelectionMode(QAbstractItemView::NoSelection);		//设置不可选
    ui->code->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->code->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);

    int l=finalcode.size();
    for(int i=0;i<l;++i){
        QTableWidgetItem *item0 = new QTableWidgetItem(QString::fromStdString(finalcode[i][0]));
        QTableWidgetItem *item1 = new QTableWidgetItem(QString::fromStdString(finalcode[i][1]));
        ui->code->setItem(i,0,item0);
        ui->code->setItem(i,1,item1);
        if(finalcode[i][0]==finalcode[i][1]){
            ui->code->item(i,0)->setBackground(Qt::red);
            ui->code->item(i,1)->setBackground(Qt::red);
        }
    }
    ui->code->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::updateEqual(){
    for(int i=0;i<equalpairs.size();++i){//遍历等价程序对<a,b>
        string a=equalpairs[i][0], b=equalpairs[i][1];

            //std::cout<<"equal "<<a<<","<<b<<"\n";

        for(int j=0;j<equalpairs.size();++j){//遍历等价程序对数组
            string c="";
            if(a==equalpairs[j][0]&&b!=equalpairs[j][1])
                c=equalpairs[j][1];
            if(a==equalpairs[j][1]&&b!=equalpairs[j][0])
                c=equalpairs[j][0];
            if(c!=""){//若存在c与a等价，则c与b等价
                bool flag=true;
                for(int k=0;k<equalpairs.size()&&flag;++k){//判断<b,c>是否已在等价对数组中
                    if((c==equalpairs[k][0]&&b==equalpairs[k][1])||(b==equalpairs[k][0]&&c==equalpairs[k][1]))
                        flag=false;
                }
                if(flag)//不存在则将<b,c>加入到等价对数组中
                {
                    std::cout<<"add equal "<<b<<","<<c<<"\n";

                    vector<string> temp;
                    temp.push_back(b);
                    temp.push_back(c);
                    equalpairs.push_back(temp);
                }
            }
        }

        for(int j=0;j<equalpairs.size();++j){//遍历等价程序对数组
            string c="";
            if(b==equalpairs[j][0]&&a!=equalpairs[j][1])
                c=equalpairs[j][1];
            if(b==equalpairs[j][1]&&a!=equalpairs[j][0])
                c=equalpairs[j][0];
            if(c!=""){//若存在c与b等价，则c与a等价
                bool flag=true;
                for(int k=0;k<equalpairs.size()&&flag;++k){//判断<a,c>是否已在等价对数组中
                    if((c==equalpairs[k][0]&&a==equalpairs[k][1])||(a==equalpairs[k][0]&&c==equalpairs[k][1]))
                        flag=false;
                }
                if(flag)//不存在则将<a,c>加入到等价对数组中
                {
                    std::cout<<"add equal "<<a<<","<<c<<"\n";

                    vector<string> temp;
                    temp.push_back(a);
                    temp.push_back(c);
                    equalpairs.push_back(temp);
                }
            }
        }
    }

    for(int i=0;i<equalpairs.size();++i){//遍历等价程序对<a,b>
        string a=equalpairs[i][0], b=equalpairs[i][1];

        //std::cout<<"equal "<<a<<","<<b<<"\n";


        for(int j=0;j<inequalpairs.size();++j){//遍历不等价程序对数组
            string c="";
            if(a==inequalpairs[j][0]&&b!=inequalpairs[j][1])
                c=inequalpairs[j][1];
            if(a==inequalpairs[j][1]&&b!=inequalpairs[j][0])
                c=inequalpairs[j][0];
            if(c!=""){//若存在c与a不等价，则c与b不等价
                bool flag=true;
                for(int k=0;k<inequalpairs.size()&&flag;++k){//判断<b,c>是否已在不等价对数组中
                    if((c==inequalpairs[k][0]&&b==inequalpairs[k][1])||(b==inequalpairs[k][0]&&c==inequalpairs[k][1]))
                        flag=false;
                }
                if(flag)//不存在则将<b,c>加入到不等价对数组中
                {

                    std::cout<<"add inequal "<<b<<","<<c<<"\n";

                    vector<string> temp;
                    temp.push_back(b);
                    temp.push_back(c);
                    inequalpairs.push_back(temp);
                }
            }
        }

        for(int j=0;j<inequalpairs.size();++j){//遍历不等价程序对数组
            string c="";
            if(b==inequalpairs[j][0]&&a!=inequalpairs[j][1])
                c=inequalpairs[j][1];
            if(b==inequalpairs[j][1]&&a!=inequalpairs[j][0])
                c=inequalpairs[j][0];
            if(c!=""){//若存在c与b不等价，则c与a不等价
                bool flag=true;
                for(int k=0;k<inequalpairs.size()&&flag;++k){//判断<a,c>是否已在不等价对数组中
                    if((c==inequalpairs[k][0]&&a==inequalpairs[k][1])||(a==inequalpairs[k][0]&&c==inequalpairs[k][1]))
                        flag=false;
                }
                if(flag)//不存在则将<a,c>加入到不等价对数组中
                {

                    std::cout<<"add inequal "<<a<<","<<c<<"\n";

                    vector<string> temp;
                    temp.push_back(a);
                    temp.push_back(c);
                    inequalpairs.push_back(temp);
                }
            }
        }
    }

}

void MainWindow::updateInEqual(){
    for(int i=0;i<inequalpairs.size();++i){//遍历不等价程序对<a,b>
        string a=inequalpairs[i][0], b=inequalpairs[i][1];

        //std::cout<<"inequal "<<a<<","<<b<<"\n";

        for(int j=0;j<equalpairs.size();++j){//遍历等价程序对数组
            string c="";
            if(a==equalpairs[j][0]&&b!=equalpairs[j][1])
                c=equalpairs[j][1];
            if(a==equalpairs[j][1]&&b!=equalpairs[j][0])
                c=equalpairs[j][0];
            if(c!=""){//若存在c与a等价，则c与b不等价
                bool flag=true;
                for(int k=0;k<inequalpairs.size()&&flag;++k){//判断<b,c>是否已在不等价对数组中
                    if((c==inequalpairs[k][0]&&b==inequalpairs[k][1])||(b==inequalpairs[k][0]&&c==inequalpairs[k][1]))
                        flag=false;
                }
                if(flag)//不存在则将<b,c>加入到不等价对数组中
                {
                    std::cout<<"add inequal "<<b<<","<<c<<"\n";

                    vector<string> temp;
                    temp.push_back(b);
                    temp.push_back(c);
                    inequalpairs.push_back(temp);
                }
            }
        }

        for(int j=0;j<equalpairs.size();++j){//遍历等价程序对数组
            string c="";
            if(b==equalpairs[j][0]&&a!=equalpairs[j][1])
                c=equalpairs[j][1];
            if(b==equalpairs[j][1]&&a!=equalpairs[j][0])
                c=equalpairs[j][0];
            if(c!=""){//若存在c与b等价，则c与a不等价
                bool flag=true;
                for(int k=0;k<inequalpairs.size()&&flag;++k){//判断<a,c>是否已在不等价对数组中
                    if((c==inequalpairs[k][0]&&a==inequalpairs[k][1])||(a==inequalpairs[k][0]&&c==inequalpairs[k][1]))
                        flag=false;
                }
                if(flag)//不存在则将<a,c>加入到不等价对数组中
                {
                    std::cout<<"add inequal "<<a<<","<<c<<"\n";

                    vector<string> temp;
                    temp.push_back(a);
                    temp.push_back(c);
                    inequalpairs.push_back(temp);
                }
            }
        }
    }
}

void MainWindow::updateUnknown(){
    for(auto it=unknownpairs.begin();it!=unknownpairs.end();){//遍历所有存疑程序对<a,b>
        string a=(*it)[0], b=(*it)[1];
        bool flag=true;
        for(int i=0;i<equalpairs.size()&&flag;++i){//判断<a,b>是否已在等价对数组中
            if((a==equalpairs[i][0]&&b==equalpairs[i][1])||(b==equalpairs[i][0]&&a==equalpairs[i][1]))
                flag=false;
        }
        for(int i=0;i<inequalpairs.size()&&flag;++i){//判断<a,b>是否已在不等价对数组中
            if((a==inequalpairs[i][0]&&b==inequalpairs[i][1])||(b==inequalpairs[i][0]&&a==inequalpairs[i][1]))
                flag=false;
        }
        if(!flag)//若<a,b>已被判定为等价或不等价，则删除<a,b>
            it=unknownpairs.erase(it);
        else
            it++;
    }
}

bool MainWindow::next(){
    if(curPairIdx>=input->equalpairs.size()-1)
        return false;
    for(int i=curPairIdx+1;i<input->equalpairs.size();++i){
        string a=input->equalpairs[i][0],b=input->equalpairs[i][1];
        bool flag=true;
        for(int j=0;j<equalpairs.size()&&flag;++j){//判断<a,b>是否已在等价对数组中
            if((a==equalpairs[j][0]&&b==equalpairs[j][1])||(b==equalpairs[j][0]&&a==equalpairs[j][1]))
                flag=false;
        }
        for(int j=0;j<inequalpairs.size()&&flag;++j){//判断<a,b>是否已在不等价对数组中
            if((a==inequalpairs[j][0]&&b==inequalpairs[j][1])||(b==inequalpairs[j][0]&&a==inequalpairs[j][1]))
                flag=false;
        }
        for(int j=0;j<unknownpairs.size()&&flag;++j){//判断<a,b>是否已在不等价对数组中
            if((a==unknownpairs[j][0]&&b==unknownpairs[j][1])||(b==unknownpairs[j][0]&&a==unknownpairs[j][1]))
                flag=false;
        }
        if(flag){
            curPairIdx=i;
            return true;
        }
    }
    return false;
}

void MainWindow::outFile(){
    std::cout<<"out"<<"\n";
    ofstream equalofs(equalfilePath),
            inequalofs(inequalfilePath),
            unknownofs(unknownfilePath);

    for(int i=0;i<equalpairs.size();++i)
        equalofs<<equalpairs[i][0]<<','<<equalpairs[i][1]<<endl;
    equalofs.close();

    for(int i=0;i<inequalpairs.size();++i)
        inequalofs<<inequalpairs[i][0]<<','<<inequalpairs[i][1]<<endl;
    inequalofs.close();

    for(int i=0;i<unknownpairs.size();++i)
        unknownofs<<unknownpairs[i][0]<<','<<unknownpairs[i][1]<<endl;
    unknownofs.close();
}

void MainWindow::on_equal_clicked()
{
    std::cout<<"equal"<<"\n";
    std::cout<<input->equalpairs[curPairIdx][0]<<","<<input->equalpairs[curPairIdx][1]<<"\n";
    equalpairs.push_back(input->equalpairs[curPairIdx]);
    updateEqual();
    updateInEqual();
    updateUnknown();
    if(next())
        showCodes();
    else
    {
        outFile();
        QMessageBox::about(NULL, "Confirm", "Confirmation is completed!");
        this->close();
    }
}

void MainWindow::on_inequal_clicked()
{
    std::cout<<"inequal"<<"\n";
    inequalpairs.push_back(input->equalpairs[curPairIdx]);
    updateEqual();
    updateInEqual();
    updateUnknown();
    if(next())
        showCodes();
    else
    {
        outFile();
        QMessageBox::about(NULL, "Confirm", "Confirmation is completed!");
        this->close();
    }
}

void MainWindow::on_unknown_clicked()
{
    std::cout<<"unknown"<<"\n";
    unknownpairs.push_back(input->equalpairs[curPairIdx]);
    updateEqual();
    updateInEqual();
    updateUnknown();
    if(next())
        showCodes();
    else
    {
        outFile();
        QMessageBox::about(NULL, "Confirm", "Confirmation is completed!");
        this->close();
    }
}

