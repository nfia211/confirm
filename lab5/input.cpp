#include "input.h"
#include <iostream>
#include <fstream>
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
using std::string;
using std::vector;
using std::ifstream;
using std::istringstream;
Input::Input()
{

}
Input::Input(string _fatherPath,string _equalfilePath,string _inequalfilePath):
    fatherPath(_fatherPath),equalfilePath(_equalfilePath),inequalfilePath(_inequalfilePath)
{
    getCodePairs();
}

void Input::getCodePairs(){
    ifstream file1(equalfilePath);
    if(!file1.good())exit(0);
    string line;
    while(getline(file1,line)){
        vector<string> s;
        string s1,s2;
        istringstream ss(line);
        getline(ss,s1,',');
        getline(ss,s2);
        s.push_back(s1);
        s.push_back(s2);
        equalpairs.push_back(s);
    }

    file1.close();

    ifstream file2(inequalfilePath);
    if(!file2.good())exit(0);
    while(getline(file2,line)){
        vector<string> s;
        string s1,s2;
        istringstream ss(line);
        getline(ss,s1,',');
        getline(ss,s2);
        s.push_back(s1);
        s.push_back(s2);
        inequalpairs.push_back(s);
    }

    file2.close();


}
