#pragma once
// plc_templates.h ----------------------------------
// Copyright 2025 Tomat7 (star0413@gmail.com)

#include "opc_client.h"


// ======= Definition of __READ__ TEMPLATE =========

template <typename T>
bool OpcClient_c::ReadNumber(string varname, T &x)
{
  muxClient->lock();
  if (!connect())
    return false;

  bool rc = false;
  /* Read the value attribute of the node. UA_Client_readValueAttribute is a
     wrapper for the raw read service available as UA_Client_Service_read. */
  /* Variants can hold scalar values and arrays of any type */
  UA_Variant *uaVariant = UA_Variant_new();
  UA_Variant_clear(uaVariant);
  UA_Variant_init(uaVariant);

  /* NodeId of the variable holding the current time */
  const UA_NodeId nodeId = UA_NODEID_STRING(1, const_cast<char*>(varname.c_str()));
  scRead = UA_Client_readValueAttribute(uaClient, nodeId, uaVariant);

  int uaType = types[type_index(typeid(x))];
  bool hasScalar = UA_Variant_hasScalarType(uaVariant, &UA_TYPES[uaType]);

//  bool isScalar = UA_Variant_isScalar(uaVariant);
//  bool isTypeOk = (uaVariant->type == &UA_TYPES[types[type_index(typeid(x))]]);

  if (scRead == UA_STATUSCODE_GOOD && hasScalar) {
    x = *(T*)uaVariant->data;
    rc = true;
  } else {
    LOGE("%s reading: %s", varname.c_str(), UA_StatusCode_name(scRead));
//    x = *(T*)uaVariant->data;
//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Reading the value failed: %s", UA_StatusCode_name(scRead));
  }

  UA_Variant_clear(uaVariant);
  UA_Variant_delete(uaVariant);
  muxClient->unlock();

  return rc;
}

template <typename T>
bool OpcClient_c::WriteNumber(string varname, T &x)
{
  muxClient->lock();
  if (!connect())
    return false;

  bool rc = false;
  T value = x;
  UA_Variant *uaVariant = UA_Variant_new();
  UA_Variant_clear(uaVariant);
  UA_Variant_init(uaVariant);

  int uaType = types[type_index(typeid(x))];
  const UA_NodeId nodeId = UA_NODEID_STRING(1, const_cast<char*>(varname.c_str()));

  scWrite = UA_Variant_setScalarCopy(uaVariant, &value, &UA_TYPES[uaType]);
  if (scWrite != UA_STATUSCODE_GOOD) {
    LOGE("Variant/Scalar: %s", UA_StatusCode_name(scWrite));
    /*  UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                    "Variant/Scalar: %s", UA_StatusCode_name(scWrite)); */
  } else {
    scWrite = UA_Client_writeValueAttribute(uaClient, nodeId, uaVariant);
    if (scWrite != UA_STATUSCODE_GOOD) {
      LOGE("Writing: %s", UA_StatusCode_name(scWrite));
      /*    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                        "Writing: %s", UA_StatusCode_name(scWrite)); */
    } else
      rc = true;
  }

  UA_Variant_clear(uaVariant);
  UA_Variant_delete(uaVariant);
  muxClient->unlock();

  return rc;
}

// eof
