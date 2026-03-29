#include <iostream>
#include <fstream>
#include <string>
#include <sys/statvfs.h>
#include <mosquitto.h>
#include <unistd.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

struct MemoryInfo {
    long total;
    long available;
};

struct DiskInfo {
    unsigned long total;
    unsigned long free;
};

struct WirelessInfo {
    float link;
    float signal;
    float noise;
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
    MemoryInfo memory_info = {0,0};
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
    float uptime = -1.0;
    ifstream file("/proc/uptime");
    if (!file.is_open()) return uptime;

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

WirelessInfo get_wireless_data() {
    WirelessInfo data = {-1, -1, -1};
    std::ifstream file("/proc/net/wireless");
    std::string line;

    if (!file.is_open()) return data;

    while (std::getline(file, line)) {
        if (line.find("wlan0:") != std::string::npos) {
            float link, signal;
            int noise;

            if (sscanf(
                line.c_str(),
                "%*[^:]: %*x %f %f %d",
                &link,
                &signal,
                &noise
            ) == 3) {
                data.link   = (int)link;
                data.signal = (int)signal;
                data.noise  = (int)noise;
            }
            break;
        }
    }
    return data;
}

class MQTTService {
    private:
        struct mosquitto *m_mosq;
        string m_topic;

    public:
        MQTTService(const string& topic) : m_topic(topic), m_mosq(nullptr) {
            mosquitto_lib_init();
        }

        ~MQTTService() {
            if (m_mosq) {
                mosquitto_disconnect(m_mosq);
                mosquitto_destroy(m_mosq);
            }
            mosquitto_lib_cleanup();
        }

        bool connect() {
            m_mosq = mosquitto_new(m_topic.c_str(), true, NULL);
            if (!m_mosq) {
                std::cerr << "Error while initializing MQTT client!" << endl;
                return false;
            }
            return (mosquitto_connect(m_mosq, "localhost", 1883, 60) == MOSQ_ERR_SUCCESS);
        }

        bool publish(const string& payload) {
            if (mosquitto_publish(m_mosq, NULL, m_topic.c_str(), payload.length(), payload.c_str(), 0, false) != MOSQ_ERR_SUCCESS) {
                std::cerr << "Error while publishing MQTT message!" << endl;
                return false;
            }
            return true;
        }
};

int main() {
    MQTTService mqtt("telemetry");
    mqtt.connect();

    while (true) {
        float cpu_temp = get_cpu_temp();
        MemoryInfo ram = get_ram_usage();
        float system_uptime = get_system_uptime();
        DiskInfo disk = get_disk_usage();
        WirelessInfo wlan = get_wireless_data();
        json payload;
        payload["cpu_temp"]=cpu_temp;
        payload["ram"]["total"]=ram.total;
        payload["ram"]["available"]=ram.available;
        payload["uptime"]=system_uptime;
        payload["disk"]["total"]=disk.total;
        payload["disk"]["free"]=disk.free;
        payload["wlan"]["link"]=wlan.link;
        payload["wlan"]["signal"]=wlan.signal;
        payload["wlan"]["noise"]=wlan.noise;

        // payload = {
        //     "cpu_temp": cpu_temp,
        //     "ram_total": ram.total,
        //     "ram_available": ram.available,
        //     "uptime": system_uptime,
        //     "disk_total": disk.total,
        //     "disk_free": disk.free,
        //     "wlan_link": wlan.link,
        //     "wlan_signal": wlan.signal,
        //     "wlan_noise": wlan.noise
        // };
        cout << payload << endl;
        mqtt.publish(payload.dump().c_str());
        sleep(5);
    }
}