
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

    /* Server Description */
#    UA_BuildInfo_clear(&conf->buildInfo);

    conf->buildInfo.productUri = UA_STRING_ALLOC(PRODUCT_URI); // PRODUCT_URI "http://open62541.org"
    conf->buildInfo.manufacturerName = UA_STRING_ALLOC(MANUFACTURER_NAME); // MANUFACTURER_NAME "open62541"
    conf->buildInfo.productName = UA_STRING_ALLOC(PRODUCT_NAME); // PRODUCT_NAME "open62541 OPC UA Server"

#    UA_ApplicationDescription_clear(&conf->applicationDescription);

    conf->applicationDescription.applicationUri = UA_STRING_ALLOC(APPLICATION_URI_SERVER); // APPLICATION_URI_SERVER "urn:open62541.server.application"
    conf->applicationDescription.productUri = UA_STRING_ALLOC(PRODUCT_URI); // PRODUCT_URI "http://open62541.org"
    conf->applicationDescription.applicationName =
        UA_LOCALIZEDTEXT_ALLOC("en", APPLICATION_NAME); // APPLICATION_NAME "open62541-based OPC UA Application"
    conf->applicationDescription.applicationType = UA_APPLICATIONTYPE_SERVER;
