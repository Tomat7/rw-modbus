
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>

UA_StatusCode readCurrentTime(UA_Server* server,
                              const UA_NodeId* sessionId, void* sessionContext,
                              const UA_NodeId* nodeId, void* nodeContext,
                              UA_Boolean sourceTimeStamp, const UA_NumericRange* range,
                              UA_DataValue* dataValue)
{
  UA_DateTime now = UA_DateTime_now();
  UA_Variant_setScalarCopy(&dataValue->value, &now,
                           &UA_TYPES[UA_TYPES_DATETIME]);
  dataValue->hasValue = true;

  // UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
  //            "readCurrentTime(func): variable was updated");

  return UA_STATUSCODE_GOOD;
}

UA_StatusCode writeCurrentTime(UA_Server* server,
                               const UA_NodeId* sessionId, void* sessionContext,
                               const UA_NodeId* nodeId, void* nodeContext,
                               const UA_NumericRange* range, const UA_DataValue* data)
{
  UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
              "writeCurrentTime(func): Changing the system time is not implemented");
  return UA_STATUSCODE_BADINTERNALERROR;
}

void addCurrentTimeDataSourceVariable(UA_Server* server)
{
  UA_VariableAttributes attr = UA_VariableAttributes_default;
  attr.displayName = UA_LOCALIZEDTEXT("en-US", "Current time - data source");
  attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

  UA_NodeId currentNodeId = UA_NODEID_STRING(1, "current-time-datasource");
  UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "current-time-datasource");

  UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
  UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
  UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

  UA_DataSource timeDataSource;
  timeDataSource.read = readCurrentTime;
  timeDataSource.write = writeCurrentTime;
  UA_Server_addDataSourceVariableNode(server, currentNodeId, parentNodeId,
                                      parentReferenceNodeId, currentName,
                                      variableTypeNodeId, attr,
                                      timeDataSource, NULL, NULL);
}

static UA_DataValue* externalValue;

void addCurrentTimeExternalDataSource(UA_Server* server)
{
  UA_NodeId currentNodeId = UA_NODEID_STRING(1, "current-time-external-source");

  UA_ValueBackend valueBackend;
  valueBackend.backendType = UA_VALUEBACKENDTYPE_EXTERNAL;
  valueBackend.backend.external.value = &externalValue;

  UA_Server_setVariableNode_valueBackend(server, currentNodeId, valueBackend);
}
