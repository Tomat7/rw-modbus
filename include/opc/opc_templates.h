#pragma once
// opc_templates.h
//

#include "opc_class.h"

// ======== Definition of TEMPLATEs =========

template <typename T>
T OpcServer_c::getValue(string s)
{
  T x = bad_value.dbl;
  if (!getNumericValue(s, x))
    if (isVariable(s))
      x = *static_cast<T*>((void*)&vars[s].value);
//      x = vars[s].value;
  return x;
}

template <typename T>
bool OpcServer_c::getValue(string s, T &x)
{
  bool ret = false;
  if (!getNumericValue(s, x))
    if (isVariable(s)) {
      x = *static_cast<T*>((void*)&vars[s].value);
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
  if (isVariable(s))         // check for existing
    x = *static_cast<T*>((void*)&vars[s].value);
//    x = vars[s].value.ui16;  // if var exist - return last good value
  return x;                  // else - the "BAD" value will return
}

template <typename T>
bool OpcServer_c::readValue(string s, T &x)
{
  bool ret = false;
  if (isVariable(s)) {  // check for existing
    x = *static_cast<T*>((void*)&vars[s].value);
    if (isGood(s))
      ret = true;
  }
  return ret;           // else - the "BAD" value will return
}

// ======== Definition of TEMPLATEs =========

template <typename T>
T OpcServer_c::updateVar(std::string s, T Value_set, bool isOK)
{
  T Value_get = Value_set;
  getNumericValue(s, Value_get);
  setNumericValue(s, Value_set, isOK);
  return Value_get;
}

template <typename T>
int OpcServer_c::addVar(std::string s, T Value, int rmode)
{
  //LOGD("%s - 1", __func__);
  rc = addVar_Names(s, types[type_index(typeid(Value))], rmode);
  if (rc == 0)
    return 0;

  //LOGD("%s - 2", __func__);
  addVar_NodeId(vars[s]);
  vars[s].ptr_value = &Value;
  addVariable(vars[s]);
  setNumericValue(s, Value, true);

  //LOGD("%s - 3", __func__);
  return 1;
}

template <typename T>
void OpcServer_c::setNumericValue(std::string s, T Value_set, bool isOK)
{
  uaDataMux->lock();

  if (vars.count(s)) {
    vars[s].ptr_value = static_cast<T*>(&Value_set);
    vars[s].value = *static_cast<value_u*>(vars[s].ptr_value);
    writeVariable(vars[s], isOK);
  } else
    LOGA("Set: Ignore non-existing variable: %s", s.c_str());

  uaDataMux->unlock();
  return;
}

template <typename T>
bool OpcServer_c::getNumericValue(std::string s, T &Value_get)
{
  uaDataMux->lock();
  bool isFresh = refreshRawValue(s);  // Update .value

  if (isFresh)
    Value_get = *static_cast<T*>((void*)&vars[s].value);

  uaDataMux->unlock();
  return isFresh;
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
