#ifndef INPUT_H
#define INPUT_H
#include <vector>
#include <string.h>
#include <iostream>
using namespace std;

class Input
{
private:
    string fatherPath="/home/cjh/input";
    string equalfilePath="/home/cjh/equal.csv";
    string inequalfilePath="/home/cjh/inequal.csv";

public:
    vector<vector<string>> equalpairs;
    vector<vector<string>> inequalpairs;
    Input();
    Input(string _fatherPath,string _equalfilePath,string _inequalfilePath);
    void getCodePairs();
};

#endif // INPUT_H
