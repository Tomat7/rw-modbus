// reg_func.cpp -----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <string.h>

#include <string>
#include <vector>

#include "./config.h"
#include "./libs.h"

#define MB_READ

// using ::std;
// using ::libconfig;

void reg_init();
void reg_print_name();
// void reg_init_name(string devname, string regname, uint16_t *val);

void reg_init()
{
  cout << endl
       << "===== reg_init =====" << endl;

  for (auto &D : PLCset)
    for (auto &R : D.regs)
    {
      string rn = (string)D.dev_name + "." + (string)R.ch_name;
      R.fullname = rn.c_str();
      R.rvalue = 5757; // TODO: remove for production!!

      rmap_t rm;
      rm.p_reg = &R;
      rm.rdata.rvalue = R.rvalue;
      rm.rdata.rstatus = R.rstatus;
      rm.rdata.rmode = R.rmode;
      rm.rdata.rtype = R.rtype;
      //      rm.rdata.rmode = (strcmp(R.ch_mode, "rw") == 0) ? 1 : 0;
      //      rm.rdata.rtype = (strcmp(R.ch_type, "f") == 0) ? 1 : 0;

      rm.fd = create_shm_fd(rn.c_str());
      if (rm.fd != -1)
      {
        rdata_t *addr = (rdata_t *)create_shm_addr(rm.fd, sizeof(rdata_t));
        if (addr != nullptr)
        {
          LOGINFO("SHM: created %s\n", rn.c_str());
          rm.p_shm = addr;
        }
      }

      REGmap[rn] = rm;
    }

  return;
}

void reg_update()
{

  for (auto &[rn, m] : REGmap)
  {
    reg_print(rn, m.p_reg);

    const auto &plc = m.p_reg;
    auto &mem = m.rdata;
    rdata_t shm;
    uint16_t remote_val = plc->rvalue;
    uint16_t mem_val = mem.rvalue;
    uint16_t &shm_val = shm.rvalue;

    if (plc->rmode)
    {
      memcpy(&shm, m.p_shm, sizeof(rdata_t));
      
      if (mem_val != remote_val)
        printf(" >");
      else
        printf("  ");
      
      if (mem_val != shm_val)
      {
        plc->rupdate = 1;
        plc->rvalue = shm_val;
        printf("< %d", shm_val);
      }
      else
        printf("  ");
    }
    else
      printf("    ");

    mem.rvalue = remote_val;
    mem.rstatus = plc->rstatus;

    memcpy(m.p_shm, &m.rdata, sizeof(rdata_t));
    printf("  +\n");
  }

  return;
}

void reg_print(string rn, const reg_t *r)
{
  printf("\n===== regs_print_name =====\n");

  const char *C = KNRM;
  if (r->rstatus < 0)
    C = KRED;

  if (r->rtype)
    printf("%s%-12s %7.2f" NRM, C, rn.c_str(), (int16_t)r->rvalue * 0.01);
  else
    printf("%s%-12s %7d" NRM, C, rn.c_str(), r->rvalue);
}

// eof
