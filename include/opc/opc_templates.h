#pragma once
// plc_templates.h ----------------------------------
// Copyright 2025 Tomat7 (star0413@gmail.com)

#include "opc_class.h"


// ======= Definition of add TEMPLATEs =========

template <typename T>
int OpcServer_c::AddVar(std::string s, T Numeric, int rmode)
{
  // LOGD("%s - 1", __func__);
  rc = add_VarName(s, types[type_index(typeid(Numeric))], rmode);
  if (rc == 0)
    return 0;

  // LOGD("%s - 2", __func__);
  add_VarNodeId(vars[s]);
  vars[s].ptr_value = &Numeric;
  add_Variable(vars[s]);
  set_NumericValue(s, Numeric, true);

  // LOGD("%s - 3", __func__);
  return 1;
}

// ======== Definition of read/update TEMPLATEs =========

template <typename T>
T OpcServer_c::UpdateVar(std::string s, T Numeric_set, bool &isOK)
{
  T Numeric_get = Numeric_set;
  bool is_read_ok = ReadNumber(s, Numeric_get);
// get_NumericValue(s, Numeric_get);
  bool is_write_ok = WriteNumber(s, Numeric_set, isOK);
// set_NumericValue(s, Numeric_set, isOK);
  isOK = (is_read_ok && is_write_ok);
  return Numeric_get;
}

template <typename T>
bool OpcServer_c::ReadNumber(string s, T &x)
{
  bool is_ok = false;
  if (isVariable(s)) {
    if (!get_NumericValue(s, x)) {
      x = *static_cast<T*>((void*)&vars[s].raw_value);
      is_ok = isGood(s);
    }
  } else
    LOGW("%s: Ignore non-existing variable: %s", __func__, s.c_str());

  return is_ok;
}


template <typename T>
bool OpcServer_c::WriteNumber(string s, T &x, bool isOK)
{
  bool is_ok = false;
  if (isVariable(s))
    is_ok = set_NumericValue(s, x, isOK);
  else
    LOGW("%s: Ignore non-existing variable: %s", __func__, s.c_str());

  return is_ok;
}


template <typename T>
bool OpcServer_c::get_NumericValue(std::string s, T &Numeric_get)
{
  uaDataMux->lock();
  bool isFresh = refresh_RawValue(s);  // Update .value

  if (isFresh)
    Numeric_get = *static_cast<T*>((void*)&vars[s].raw_value);

  uaDataMux->unlock();
  return isFresh;
}



template <typename T>
bool OpcServer_c::set_NumericValue(std::string s, T Numeric_set, bool isOK)
{
  uaDataMux->lock();
  bool is_write_ok = false;

  if (vars.count(s)) {
    vars[s].ptr_value = static_cast<T*>(&Numeric_set);
    vars[s].raw_value = *static_cast<value_u*>(vars[s].ptr_value);
    is_write_ok = write_Variable(vars[s], isOK);
  } else
    LOGA("Set: Ignore non-existing variable: %s", s.c_str());

  uaDataMux->unlock();
  return is_write_ok;
}


/*
  template <typename T>
  T OpcServer_c::GetNumber(string s)
  {
  T x = bad_value.dbl;
  if (!get_NumericValue(s, x))
    if (isVariable(s))
      x = *static_cast<T*>((void*)&vars[s].raw_value);
  //      x = vars[s].value;
  return x;
  }
*/

// =======================================
/*
  template <typename T>
  T OpcServer_c::ReadValue(string s)
  {
  T x = bad_value.ui16;
  if (isVariable(s))  // check for existing
    x = *static_cast<T*>((void*)&vars[s].raw_value);
  //    x = vars[s].value.ui16;  // if var exist - return last good value
  return x;  // else - the "BAD" value will return
  }
*/

/*
  template <typename T>
  bool OpcServer_c::ReadValue(string s, T &x)
  {
  bool ret = false;
  if (isVariable(s)) {  // check for existing
    x = *static_cast<T*>((void*)&vars[s].raw_value);
    if (isGood(s))
      ret = true;
  }
  return ret;  // else - the "BAD" value will return
  }
*/

// ======= Definition of write TEMPLATEs =======

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
