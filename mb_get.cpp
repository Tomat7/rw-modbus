// ----------------------------------------------------------------------------
#include <vector>

#include "config.h"
#include "libs.h"

using namespace std;
using namespace libconfig;

void mb_fill_plcs_full();
int mb_fill_regs_full(int i);

void mb_get_regs_byname();

// int mb_set_response_timeout(int i);

/*
void mb_print_reg_details(int i, int j);
void mb_show_regs_name();
void mb_fill_reg_name(string devname, string regname, uint16_t *val);
*/

int mb_get() {
  cout << endl << "======= mb_get =======" << endl;
  // ===== Print ALL PLCs detailed.
  mb_fill_plcs_full();

  // ===== Print ALL REGs names.
  mb_get_regs_byname();

  return 0;
}


void mb_fill_plcs_full() {
  int nb_plcs = static_cast<int>(PLCset.size());
  cout << "Total PLCs: " << nb_plcs << endl;
  // ===== Cycle to fill PLCs REGs =====
  for (int i = 0; i < nb_plcs; ++i) {
    //    mb_set_response_timeout(i);
    mb_fill_regs_full(i);
  }

  return;
}


int mb_fill_regs_full(int i) {

  const int nb_regs = static_cast<int>(PLCset[i].regs.size());
  uint16_t mbregs[nb_regs + 1];

  const char *_ip = PLCset[i].ip_addr;
  int _port = PLCset[i].tcp_port;

  modbus_t *mb;
  int rc = 0;

  // ===== Try/check to open MB

  mb = modbus_new_tcp(_ip, _port);
  rc = modbus_connect(mb);
  if (rc == -1) {
    fprintf(stderr, "MB connection failed: %s\n", modbus_strerror(errno));
    modbus_free(mb);
    return rc;
  }
  /*
    rc = modbus_set_response_timeout(mb, 0, PLCset[i].err_timeout);
    if ( rc == -1)
      {
          fprintf(stderr, "MB set timeout failed: %s\n",
    modbus_strerror(errno)); modbus_free(mb); return rc;
      }
  */
  //  mb_set_response_timeout(i);

  // ===== Try to get ALL HOLDING registers by ONE request */
  rc = modbus_read_registers(mb, 0, nb_regs, mbregs);
  if (rc == -1) {
    fprintf(stderr, "MB multi-read error: %s \n", modbus_strerror(errno));
    return rc;
  }

  // ===== Cycle to get MB-registers =====
  for (int j = 0; j < nb_regs; ++j) {
    //    reg_t &reg = PLCset[i].regs[j];
    //    int &raddr = reg.raddr;
    PLCset[i].regs[j].rvalue = mbregs[PLCset[i].regs[j].raddr];
  }

  modbus_close(mb);
  modbus_free(mb);

  return 0;
}


/*
int mb_set_respose_timeout() {
    uint32_t response_sec;
    uint32_t response_us;
    modbus_get_response_timeout(mb, &response_sec, &response_us);
    printf("Current timeouts: %d sec %d usec. \n", response_sec, response_us);
    rc = modbus_set_response_timeout(mb, response_sec, response_us);
  return rc;
}
*/


void mb_get_regs_byname() {
  cout << endl << "======= mb_get_regs_byname =======" << endl;
  for (const auto &[rname, rval] : MBreg)
    cout << "  " << setw(12) << left << rname << setw(7) << right << *rval
         << endl;
  return;
}

/*
void mb_fill_reg_name(string devname, string regname, uint16_t *val) {
  MBreg[devname + "." + regname] = val;
  *MBreg[devname + "." + regname] = 5757;
  return;
}
*/

/*
void mb_print_plc_details(int i) {

  cout << setw(5) << left << PLCset[i].dev_name << "  " << setw(5) << left
       << PLCset[i].nb_regs << "  " << setw(15) << left << PLCset[i].ip_addr
       << "  " << PLCset[i].poll_interval << "  " << PLCset[i].err_timeout
       << endl;

  return;
}
*/

// eof
