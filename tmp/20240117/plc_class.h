#pragma once

//#include "config.h"
#include <ctime>
#include <iostream>
#include <modbus/modbus.h>
#include <vector>

using namespace std;

struct reg_t {
    int raddr;
    const char *ch_name;
    const char *ch_mode;
    const char *ch_type;
    uint16_t rvalue;
};

class PLC
{

public:
    PLC()
    {
        cout << "+ New PLC created." << endl;
    }
    ~PLC()
    {
        cout << "- PLC deleted: " << dev_name << endl;
        deinit();
    }

    int init(const char *_ip = "", int _port = 0);
    int set_timeout();
    int mb_connect();
    int read_master();
    void deinit();
    uint64_t millis();

    const char *dev_title;
    const char *dev_desc;
    const char *dev_name;
    const char *ip_addr;
    int tcp_port = 0;
    uint64_t mb_timestamp_ms = 0;     // milliseconds since the Epoch on last read
    uint32_t mb_timeout = 0;  // miCRo seconds (!!) Modbus respose timeout
    uint32_t mb_errors = 0;   // counter of any ERROR
    uint32_t mb_interval = 0; // milliseconds between read request
    int reg_qty = 0;
    vector<reg_t> regs;

private:
    int rc = 0;
    modbus_t *ctx = nullptr;
};
