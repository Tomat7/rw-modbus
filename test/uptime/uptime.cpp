
#include <iostream>
#include <sys/sysinfo.h>

int main() {
    struct sysinfo info;
    if (sysinfo(&info) != 0) {
        std::cerr << "Failed to get system info." << std::endl;
        return 1;
    }

    // Calculate system uptime in seconds
    unsigned long uptime = info.uptime;

    // Convert uptime to days, hours, minutes, seconds
    unsigned int days = uptime / (24 * 3600);
    uptime %= (24 * 3600);
    unsigned int hours = uptime / 3600;
    uptime %= 3600;
    unsigned int minutes = uptime / 60;
    unsigned int seconds = uptime % 60;

    std::cout << "System Uptime: " << days << " days, " << hours << " hours, "
              << minutes << " minutes, " << seconds << " seconds" << std::endl;

    return 0;
}
