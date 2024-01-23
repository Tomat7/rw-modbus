// ----------------------------------------------------------------------------
#include <vector>

#include "config.h"
#include "libs.h"

using namespace std;
using namespace libconfig;

int mb_read()
{
    cout << endl << "===== mb_read =====" << endl;

    for (auto &D : PLCset)
        D.read();

    /*
      int nb_plcs = static_cast<int>(PLCset.size());
      cout << "Total PLCs: " << nb_plcs << endl;
      for (int i = 0; i < nb_plcs; ++i) {
        //    mb_set_response_timeout(i);
        PLCset[i].read();
      }
    */

    return 0;
}

// eof
