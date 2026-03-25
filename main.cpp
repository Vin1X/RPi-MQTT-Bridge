#include <iostream>
#include <fstream>
#include <string>
using namespace std;

float getCpuTemp() {
    string cpu_temp_raw;
    ifstream file("/sys/class/thermal/thermal_zone0/temp");
    file >> cpu_temp_raw;
    file.close();
    return stof(cpu_temp_raw) / 1000.0;
}

float getRamUsage() {
    return 0.0;
}

float getSystemUptime() {
    return 0.0;
}

float getDiskUsage() {
    return 0.0;
}

float getCurrentDataTransfer() {
    return 0.0;
}

int main() {
    cout << "CPU Temperature: " << getCpuTemp() << " °C" << endl;
}