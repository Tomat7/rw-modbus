#pragma once
// plc_templates.h ----------------------------------
// Copyright 2025 Tomat7 (star0413@gmail.com)

#include "opc_class.h"


// ======= Definition of add TEMPLATEs =========

template <typename T>
int OpcServer_c::addVar(std::string s, T Numeric, int rmode)
{
  // LOGD("%s - 1", __func__);
  rc = addVar_Names(s, types[type_index(typeid(Numeric))], rmode);
  if (rc == 0)
    return 0;

  // LOGD("%s - 2", __func__);
  addVar_NodeId(vars[s]);
  vars[s].ptr_value = &Numeric;
  addVariable(vars[s]);
  WriteNumericValue(s, Numeric, true);

  // LOGD("%s - 3", __func__);
  return 1;
}

// ======== Definition of read/update TEMPLATEs =========

template <typename T>
T OpcServer_c::updateVar(std::string s, T Numeric_set, bool isOK)
{
  T Numeric_get = Numeric_set;
  getNumericValue(s, Numeric_get);
  WriteNumericValue(s, Numeric_set, isOK);
  return Numeric_get;
}

template <typename T>
bool OpcServer_c::getNumericValue(std::string s, T &Numeric_get)
{
  uaDataMux->lock();
  bool isFresh = refreshRawValue(s);  // Update .value

  if (isFresh)
    Numeric_get = *static_cast<T*>((void*)&vars[s].raw_value);

  uaDataMux->unlock();
  return isFresh;
}

template <typename T>
T OpcServer_c::getNumber(string s)
{
  T x = bad_value.dbl;
  if (!getNumericValue(s, x))
    if (isVariable(s))
      x = *static_cast<T*>((void*)&vars[s].raw_value);
  //      x = vars[s].value;
  return x;
}

template <typename T>
bool OpcServer_c::ReadNumber(string s, T &x)
{
  bool ret = false;
  if (!getNumericValue(s, x))
    if (isVariable(s)) {
      x = *static_cast<T*>((void*)&vars[s].raw_value);
      if (isGood(s))
        ret = true;
    }
  return x;
}

// =======================================

template <typename T>
T OpcServer_c::readValue(string s)
{
  T x = bad_value.ui16;
  if (isVariable(s))  // check for existing
    x = *static_cast<T*>((void*)&vars[s].raw_value);
  //    x = vars[s].value.ui16;  // if var exist - return last good value
  return x;  // else - the "BAD" value will return
}

template <typename T>
bool OpcServer_c::readValue(string s, T &x)
{
  bool ret = false;
  if (isVariable(s)) {  // check for existing
    x = *static_cast<T*>((void*)&vars[s].raw_value);
    if (isGood(s))
      ret = true;
  }
  return ret;  // else - the "BAD" value will return
}

// ======= Definition of write TEMPLATEs =======


template <typename T>
void OpcServer_c::WriteNumericValue(std::string s, T Numeric_set, bool isOK)
{
  uaDataMux->lock();

  if (vars.count(s)) {
    vars[s].ptr_value = static_cast<T*>(&Numeric_set);
    vars[s].raw_value = *static_cast<value_u*>(vars[s].ptr_value);
    writeVariable(vars[s], isOK);
  } else
    LOGA("Set: Ignore non-existing variable: %s", s.c_str());

  uaDataMux->unlock();
  return;
}


/*
  template <typename T>
  bool OpcServer_c::getNumericValue(std::string s, T &Value_get)
  {
  uaDataMux->lock();
  bool ret = false;
  void* VarData = getVariantDataPtr(s);

  if (VarData != nullptr) {
    Value_get = *(static_cast<T*>(VarData));
    vars[s].ptr_value = static_cast<T*>(&Value_get);
    vars[s].value = *static_cast<value_u*>(VarData);
    ret = true;
  }
  uaDataMux->unlock();
  return ret;
  }
*/

// eof
