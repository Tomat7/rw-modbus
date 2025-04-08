
#define MANUFACTURER_NAME "open62541"
#define PRODUCT_NAME "open62541 OPC UA Server"
#define PRODUCT_URI "http://open62541.org"
#define APPLICATION_NAME "open62541-based OPC UA Application"
#define APPLICATION_URI "urn:unconfigured:application"
#define APPLICATION_URI_SERVER "urn:open62541.server.application"

#define SECURITY_POLICY_SIZE 6

#define STRINGIFY(arg) #arg
#define VERSION(MAJOR, MINOR, PATCH, LABEL) \
    STRINGIFY(MAJOR) "." STRINGIFY(MINOR) "." STRINGIFY(PATCH) LABEL

    endpoint->transportProfileUri =
        UA_STRING_ALLOC("http://opcfoundation.org/UA-Profile/Transport/uatcp-uasc-uabinary");
    endpoint->securityMode = securityMode;

    /* A numeric value that indicates how secure the EndpointDescription is compared to other EndpointDescriptions
     * for the same Server. A value of 0 indicates that the EndpointDescription is not recommended and is only
     * supported for backward compatibility. A higher value indicates better security. */
    UA_String noneuri = UA_STRING("http://opcfoundation.org/UA/SecurityPolicy#None");
    UA_String basic128uri = UA_STRING("http://opcfoundation.org/UA/SecurityPolicy#Basic128Rsa15");
    UA_String basic256uri = UA_STRING("http://opcfoundation.org/UA/SecurityPolicy#Basic256");
    if(UA_String_equal(&securityPolicy->policyUri, &noneuri) ||
       UA_String_equal(&securityPolicy->policyUri, &basic128uri) ||
       UA_String_equal(&securityPolicy->policyUri, &basic256uri)) {

    /* Server Description */
    UA_BuildInfo_clear(&conf->buildInfo);
    conf->buildInfo.productUri = UA_STRING_ALLOC(PRODUCT_URI); // #define PRODUCT_URI "http://open62541.org"
    conf->buildInfo.manufacturerName = UA_STRING_ALLOC(MANUFACTURER_NAME); // #define MANUFACTURER_NAME "open62541"
    conf->buildInfo.productName = UA_STRING_ALLOC(PRODUCT_NAME); // #define PRODUCT_NAME "open62541 OPC UA Server"

    UA_ApplicationDescription_clear(&conf->applicationDescription);
    conf->applicationDescription.applicationUri = UA_STRING_ALLOC(APPLICATION_URI_SERVER); // #define APPLICATION_URI_SERVER "urn:open62541.server.application"
    conf->applicationDescription.productUri = UA_STRING_ALLOC(PRODUCT_URI); // #define APPLICATION_URI "urn:unconfigured:application"
    conf->applicationDescription.applicationName =
        UA_LOCALIZEDTEXT_ALLOC("en", APPLICATION_NAME); // #define APPLICATION_NAME "open62541-based OPC UA Application"
    conf->applicationDescription.applicationType = UA_APPLICATIONTYPE_SERVER;
    /* conf->applicationDescription.gatewayServerUri = UA_STRING_NULL; */
    /* conf->applicationDescription.discoveryProfileUri = UA_STRING_NULL; */
    /* conf->applicationDescription.discoveryUrlsSize = 0; */
    /* conf->applicationDescription.discoveryUrls = NULL; */


    /* Listen on all interfaces (also external). This must be the first
     * entry if this is desired. Otherwise some interfaces may be blocked
     * (already in use) with a hostname that is only locally reachable.*/
    mp_snprintf(serverUrlBuffer[0], sizeof(serverUrlBuffer[0]),
                "opc.tcp://:%u", portNumber);
    serverUrls[serverUrlsSize] = UA_STRING(serverUrlBuffer[0]);
    serverUrlsSize++;

    /* Add to the config */
    UA_StatusCode retval =
        UA_Array_copy(serverUrls, serverUrlsSize,
                      (void**)&conf->serverUrls, &UA_TYPES[UA_TYPES_STRING]);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;
    conf->serverUrlsSize = serverUrlsSize;

    /* Endpoints */
    /* conf->endpoints = {0, NULL}; */

    /* Set Logger for Certificate Verification */
