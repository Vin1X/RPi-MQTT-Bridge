#include <iostream>
#include <fstream>
#include <string>
#include <sys/statvfs.h>
using namespace std;

struct MemoryInfo {
    long total;
    long available;
};

struct DiskInfo {
    unsigned long total;
    unsigned long free;
};

float get_cpu_temp() {
    string cpu_temp_raw;
    ifstream file("/sys/class/thermal/thermal_zone0/temp");

    if (!file.is_open()) return -1.0;

    file >> cpu_temp_raw;
    file.close();
    return stof(cpu_temp_raw) / 1000.0;
}

MemoryInfo get_ram_usage() {
    MemoryInfo memory_info;
    string line;
    int found_count = 0;
    ifstream file("/proc/meminfo");

    if (!file.is_open()) return memory_info;

    while (getline(file, line) && found_count < 2) {
        if (line.compare(0, 8, "MemTotal") == 0) {
            sscanf(line.c_str(), "MemTotal: %ld", &memory_info.total);
            found_count++;
        }
        else if ( line.compare(0, 12, "MemAvailable") == 0) {
            sscanf(line.c_str(), "MemAvailable: %ld", &memory_info.available);
            found_count++;
        }
    }
    return memory_info;
}

float get_system_uptime() {
    float uptime;
    ifstream file("/proc/uptime");
    file >> uptime;

    return uptime;
}

DiskInfo get_disk_usage() {
    struct statvfs disk_data;
    DiskInfo disk_info;
    if (statvfs("/", &disk_data)) {
        return disk_info;
    } else {
        disk_info.total = disk_data.f_blocks * disk_data.f_frsize;
        disk_info.free = disk_data.f_bavail * disk_data.f_frsize;
        return disk_info;
    }
}

float get_current_data_transfer() {
    return 0.0;
}

int main() {
    cout << "CPU Temperature: " << get_cpu_temp() << " °C" << endl;
    MemoryInfo ram = get_ram_usage();

    cout << "RAM Total: " << ram.total << " kB" << endl;
    cout << "RAM Available: " << ram.available << " kB" << endl;

    cout << "Uptime: " << get_system_uptime() << " seconds" << endl;

    DiskInfo disk = get_disk_usage();
    cout << "Disk Free: " << disk.free << " kB" << endl;
    cout << "Disk Total: " << disk.total << " kB" << endl;
}