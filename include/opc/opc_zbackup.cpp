
/* var_union OpcServer_c::getVarUnion(string s)
  {
  void* VarData = getVarData(s);
  if (VarData != nullptr)
      vars[s].value = *static_cast<var_union*>(VarData);
  else
    LOGA("ValueUnion: Wrong variable data: %s", s.c_str());
  return vars[s].value;
  }
*/
/*
   if (Vrnt.data != nullptr)
     Value_get = *(static_cast<T*>(Vrnt.data));
   //else
   //  Vrnt.data = &Value_get;
   UA_Variant_clear(&Vrnt);
   vars[s].ptr_value = static_cast<T*>(&Value_get);
   vars[s].value = *static_cast<value_u*>(vars[s].ptr_value);
*/
/*
  void OpcServer_c::readVariable(var_t &v)
  {
  if (v.ptr_value == nullptr) {
    LOGA("Wrong ptr: %s", v.name);
    return;
  }

  // UA_NodeId varNodeId = UA_NODEID_STRING(1, v.qf_name);

  UA_Variant myVariant;
  UA_Variant_init(&myVariant);

  UA_Variant_setScalar(&myVariant, v.ptr_value, &UA_TYPES[v.type]);
  UA_Server_writeValue(uaServer, v.node_id.var, myVariant);

  // Set the status code of the value to an error code. The function
  //   UA_Server_write provides access to the raw service. The above
  //   UA_Server_writeValue is syntactic sugar for writing a specific node
  //   attribute with the write service.
  UA_WriteValue wv;
  UA_WriteValue_init(&wv);
  wv.nodeId = v.node_id.var;
  wv.attributeId = UA_ATTRIBUTEID_VALUE;
  wv.value.status = UA_STATUSCODE_BADNOTCONNECTED;
  wv.value.hasStatus = true;
  UA_Server_write(uaServer, &wv);

  // Reset the variable to a good statuscode with a value
  wv.value.status = UA_STATUSCODE_GOOD;  // UA_STATUSCODE_BADNOTCONNECTED;
  // wv.value.status = UA_STATUSCODE_BADCOMMUNICATIONERROR;
  wv.value.hasStatus = true;
  wv.value.value = myVariant;
  wv.value.hasValue = true;
  // wv.value.serverTimestamp = true;
  UA_Server_write(uaServer, &wv);
  }
*/

/*   if (!ValueIsOK) {
    wv.value.status = UA_STATUSCODE_BADNOTCONNECTED; //UA_STATUSCODE_BAD;
    UA_Server_writeDataValue(uaServer, v.node_id.var, wv.value);
  }
*/
/* {
  wv.nodeId = v.node_id.var;
  wv.attributeId = UA_ATTRIBUTEID_VALUE;

  wv.value.hasStatus = true;
  //wv.value.status = UA_STATUSCODE_GOOD;
  //wv.value.status = UA_STATUSCODE_BADNODEIDUNKNOWN; //UA_STATUSCODE_BAD;
  wv.value.status = UA_STATUSCODE_BAD;

  wv.value.value = Attr.value;
  wv.value.hasValue = true;

  UA_DateTime currentTime = UA_DateTime_now();
  wv.value.hasServerTimestamp = true;
  wv.value.serverTimestamp = currentTime;
  wv.value.hasSourceTimestamp = true;
  wv.value.sourceTimestamp = currentTime - 1800 * UA_DATETIME_SEC;

  UA_Server_writeDataValue(uaServer, v.node_id.var, wv.value);
  }
*/

// eof
