// opc_number.cpp ----------------------------------
// Copyright 2025 Tomat7 (star0413@gmail.com)

#include "opc_client.h"

#ifdef _UA_TYPE
#undef _UA_TYPE
#endif
#define _UA_TYPE(_XNUM) ua_types_map[type_index(typeid(_XNUM))]


namespace OPC
{

// ======= Definition of __READ__ TEMPLATE =========

Number_c OpcClient_c::ReadNumber(string varname)
{
  muxClient->lock();
//  bool rc = false;
  int _type = 0;
  Number_c Numx;
  Numx.status = false;

  if (_connect()) {
    /* Read the value attribute of the node. UA_Client_readValueAttribute is a
       wrapper for the raw read service available as UA_Client_Service_read. */
    /* Variants can hold scalar values and arrays of any type */
    /* NodeId of the variable holding the current time */
    _variant_init();

    const UA_NodeId nodeId = UA_NODEID_STRING(1, const_cast<char*>(varname.c_str()));
    scRead = UA_Client_readValueAttribute(uaClient, nodeId, uaVariant);

    if (scRead == UA_STATUSCODE_GOOD) {
      _type = _variant_get_uatype(uaVariant);
      Numx.set(_type, uaVariant->data);
    } else
      LOGE("OPC_cli::%s: %s %s", __func__, varname.c_str(), UA_StatusCode_name(scRead));

    _variant_clean();
  }

  muxClient->unlock();

  return Numx;
}

/*
  template <typename T>
  bool OpcClient_c::Write(string varname, T &x)
  {
  muxClient->lock();
  bool rc = false;

  if (_connect()) {
  //  Init & Write & Clean
    _variant_init();
    T value = x;
    const UA_NodeId nodeId = UA_NODEID_STRING(1, const_cast<char*>(varname.c_str()));
    scWrite = UA_Variant_setScalarCopy(uaVariant, &value, &UA_TYPES[_UA_TYPE(x)]);
  //  int uaType = ua_types[type_index(typeid(x))];
  //  scWrite = UA_Variant_setScalarCopy(uaVariant, &value, &UA_TYPES[uaType]);

    if (scWrite != UA_STATUSCODE_GOOD)
      LOGE("Variant/Scalar: %s", UA_StatusCode_name(scWrite));
    else {
      scWrite = UA_Client_writeValueAttribute(uaClient, nodeId, uaVariant);
      if (scWrite != UA_STATUSCODE_GOOD)
        LOGE("Writing: %s", UA_StatusCode_name(scWrite));
      else
        rc = true;
    }

    _variant_clean();
  }

  muxClient->unlock();

  return rc;
  }
*/


} // namespace OPC

//  bool isScalar = UA_Variant_isScalar(uaVariant);
//  bool isTypeOk = (uaVariant->type == &UA_TYPES[types[type_index(typeid(x))]]);

//    x = *(T*)uaVariant->data;
//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Reading the value failed: %s", UA_StatusCode_name(scRead));

//  int uaType = ua_types[type_index(typeid(x))];
//  bool hasScalar = UA_Variant_hasScalarType(uaVariant, &UA_TYPES[uaType]);


/*  UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               "Variant/Scalar: %s", UA_StatusCode_name(scWrite)); */
/*    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                 "Writing: %s", UA_StatusCode_name(scWrite)); */


// eof
