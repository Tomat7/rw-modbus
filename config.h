
#include <string>
#include <vector>

#define CFG_FILE "./conf/modbus.cfg"

using namespace std;

struct reg_t {
    int raddr;
    const char* rname;
    const char* rmode;
    uint16_t rvalue;
};

struct plc_t {
    const char *preffix;
    const char *ipaddr;
    int nb_regs;
    int polling;
    int timeout;
    vector<reg_t> regs;
};

int cfg_read_mbset(const char *cfg_file, vector<plc_t>& plcset);
int mb_show(const vector<plc_t>& plcset);
