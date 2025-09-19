//
// ======= SCADA registers definitions: =======
//
// rsource && rfolder && rname- is mandatory parameter for SCADA registers 
//
// rfolder - possible parameter for Modbus registers
// rname - is mandatory parameter for Modbus registers
//
// rsource = "-" - mean register not linked with Modbus register
// rsource = "DEVICE.regname" - SCADA register linked with PLC register named "DEVICE.regname"
// rsource - is prohibited for Modbus register
//
// raddr - prohibited parameter for SCADA registers (??)
//

// ======= Test regs
//
  { rfolder = "";  rname = "Millis"; rsource = "-"; rmode = "rw"; rtype = "u"; },
  { rfolder = "";  rname = "Double"; rsource = "-"; rmode = "rw"; rtype = "Float"; },
  { rfolder = "Temp";  rname = "T49_100";    rsource = "GATE49.Temp1";  rmode = "r"; rtype = "f100"; },

// ======= Mapping of Modbus regs to SCADA (with type translation)
//
  { rfolder = "Temp";  rname = "Tkub1";    rsource = "Kub.Temp1";     rmode = "r"; rtype = "f100"; },
  { rfolder = "Temp";  rname = "Tkub2";  rsource = "Buf.Temp3"; rmode = "r"; rtype = "f100"; },
  { rfolder = "Temp";  rname = "Tbuf";   rsource = "Buf.Temp2"; rmode = "r"; rtype = "f100"; },
  { rfolder = "Temp";  rname = "Tdef";   rsource = "Def.Temp1"; rmode = "r"; rtype = "f100"; },
  { rfolder = "Temp";  rname = "Ttsa";   rsource = "Def.Temp3"; rmode = "r"; rtype = "f100"; },
  { rfolder = "";  rname = "Pset";      rsource = "Kub.Pset"; rmode = "rw"; rtype = "u"; },
  { rfolder = "";  rname = "Pnow";      rsource = "Kub.Pnow";  rmode = "r"; rtype = "u"; },
  { rfolder = "";  rname = "OverPress"; rsource = "Buf.Press"; rmode = "r"; rtype = "u"; },
  { rfolder = "";  rname = "AbsPress";  rsource = "Def.Press"; rmode = "r"; rtype = "u"; },
//  { rfolder = ".";  rname = "WaterPump"  rsource = "Buf.Press"; rmode = "r"; rtype = "*"; },

// ======= SCADA regs (calculated within program)
//
  { rfolder = "";  rname = "Process"; rsource = "-"; rmode = "rw"; rtype = "u"; },
  { rfolder = "";  rname = "Mode";    rsource = "-"; rmode = "rw"; rtype = "u"; },
  { rfolder = "Temp";  rname = "Tkub0";   rsource = "-"; rmode = "rw"; rtype = "f"; },
  { rfolder = "Temp";  rname = "T_boil";  rsource = "-"; rmode = "rw"; rtype = "float"; },
  { rfolder = "Temp";  rname = "T_heat";  rsource = "-"; rmode = "rw"; rtype = "float"; },

/*
// ======= CONFIGs regs  // MOVED to other file!
//
  { rfolder = "cfg";  rname = "P_MAX";   rsource = "-"; rmode = "rw"; rtype = "u"; },
  { rfolder = "cfg";  rname = "P_WARM";  rsource = "-"; rmode = "rw"; rtype = "u"; },
  { rfolder = "cfg";  rname = "P_HEAD";  rsource = "-"; rmode = "rw"; rtype = "u"; },
  { rfolder = "cfg";  rname = "P_BODY";  rsource = "-"; rmode = "rw"; rtype = "u"; },
  { rfolder = "cfg";  rname = "P_TAIL";  rsource = "-"; rmode = "rw"; rtype = "u"; },
  { rfolder = "cfg";  rname = "P_DIST";  rsource = "-"; rmode = "rw"; rtype = "u"; },
  { rfolder = "cfg";  rname = "P_DIST2"; rsource = "-"; rmode = "rw"; rtype = "u"; },
  { rfolder = "cfg";  rname = "P_DIST3"; rsource = "-"; rmode = "rw"; rtype = "u"; },
  { rfolder = "cfg";  rname = "P_BOIL";  rsource = "-"; rmode = "rw"; rtype = "u"; },
*/

// ======= TEMPLATE (with wrong type)
//
  { rfolder = ".";  rname = "empty"; rsource = "-"; rmode = "rw"; rtype = "main$"; }

