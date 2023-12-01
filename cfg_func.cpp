// cfg_func.cpp --------------------------------------------------------------

#include "config.h"
#include "libs.h"

using namespace std;
using namespace libconfig;

Config cfg;
plc_t plcnow;

// This example reads the configuration file and displays
// some of its contents.

int cfg_read_mbset(const char *cfg_file, vector<plc_t> &plcset) {

  // Read the file. If there is an error, report it and exit.
  try {
    cfg.readFile(cfg_file);
    std::cout << "I/O reading file OK: " << cfg_file << std::endl;
  } catch (const FileIOException &fioex) {
    std::cerr << "I/O error while reading file " << cfg_file << std::endl;
    return (EXIT_FAILURE);
  } catch (const ParseException &pex) {
    std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
              << " - " << pex.getError() << std::endl;
    return (EXIT_FAILURE);
  }

  // Get the store name.
  try {
    string name = cfg.lookup("maintitle");
    cout << "Config title: " << name << endl << endl;
  } catch (const SettingNotFoundException &nfex) {
    cerr << "No 'nametitle' setting in configuration file." << endl;
  }

  // Output a list of all PLCs in the inventory.
  try {
    const Setting &PLCs = cfg.lookup("plc");
    int count_PLCs = PLCs.getLength();

    for (int i = 0; i < count_PLCs; ++i) {
      const Setting &plc = PLCs[i];
      const Setting &REGs = plc["regs"];
      int count_REGs = REGs.getLength();

      // Only output the record if all of the expected fields are present.
      const char *ptitle;

      if (!(plc.lookupValue("title", ptitle) &&
            plc.lookupValue("name", plcnow.preffix) &&
            plc.lookupValue("ip", plcnow.ipaddr) &&
            plc.lookupValue("polling", plcnow.polling) &&
            plc.lookupValue("timeout", plcnow.timeout) )) {
        cout << "Warning!! Error reading PLC configuration: " << i << endl;
        continue;
      }

      plcnow.nb_regs = count_REGs;

      cout << setw(10) << left << ptitle << "  " << setw(10) << left
           << plcnow.preffix << "  " << setw(20) << left
           << plcnow.ipaddr << "  " << plcnow.nb_regs << endl;

      // ===== Show registers details =====
      for (int j = 0; j < count_REGs; ++j) {
        const Setting &reg = REGs[j];
        reg_t regnow;

        if (!(reg.lookupValue("rname", regnow.rname) &&
              reg.lookupValue("addr", regnow.raddr) &&
              reg.lookupValue("access", regnow.rmode))) 
        {
          cout << "error reading REG " << j << endl;
          continue;
        }

        regnow.rvalue = 0;
        plcnow.regs.push_back(regnow);

        cout << "       " << setw(9) << left << regnow.rname << "" 
			 << setw(3) << right << regnow.raddr << " " 
			 << setw(5) << left << regnow.rmode << "  " << endl;
      }
      // ===== END registers details =====
      
      cout << endl;
      plcset.push_back(plcnow);
    }
    cout << "Configured PLCs: " << plcset.size() << endl;

  } catch (const SettingNotFoundException &nfex) {
    cout << "Great ERROR! Exiting." << endl;
    // Ignore.
  }

  /*
    // Output a list of all PLCs in the inventory.
    try {
      const Setting &movies = root["inventory"]["movies"];
      int count = movies.getLength();

      cout << setw(30) << left << "TITLE"
           << "  " << setw(10) << left << "MEDIA"
           << "   " << setw(6) << left << "PRICE"
           << "  "
           << "QTY" << endl;

      for (int i = 0; i < count; ++i) {
        const Setting &movie = movies[i];

        // Only output the record if all of the expected fields are present.
        string title, media;
        double price;
        int qty;

        if (!(movie.lookupValue("title", title) &&
              movie.lookupValue("media", media) &&
              movie.lookupValue("price", price) && movie.lookupValue("qty",
    qty))) continue;

        cout << setw(30) << left << title << "  " << setw(10) << left << media
             << "  " << '$' << setw(6) << right << price << "  " << qty << endl;
      }

      cout << endl;
    } catch (const SettingNotFoundException &nfex) {
      // Ignore.
    }
  */
  return (EXIT_SUCCESS);
}

// eof
