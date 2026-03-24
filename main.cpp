#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    string mytext;
    ifstream file("/sys/class/thermal/thermal_zone0/temp");
    while(getline (file, mytext)){
        cout<<mytext;
    }
    file.close();
}