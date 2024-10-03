#include <oci.h>
#include <iostream>
#include <cstring>

void check_err(OCIError* err, sword status) {
    if (status != OCI_SUCCESS) {
        char errbuf[512];
        sb4 errcode;
        OCIErrorGet(err, 1, NULL, &errcode, (OraText *)errbuf, sizeof(errbuf), OCI_HTYPE_ERROR);
        std::cerr << "OCI Error: " << errbuf << std::endl;
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <hostname> <port>" << std::endl;
        return 1;
    }

    const char* hostname = argv[1];
    const char* port = argv[2];
    const char* DB = "(DESCRIPTION=(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)(HOST=%s)(PORT=%s)))(CONNECT_DATA=(SERVICE_NAME=FREE)))";

    OCIEnv* env;
    OCIServer* srv;
    OCIError* err;
    OCISvcCtx* svc;
    OCISession* sess;
    OCIStmt* stmt;
    OCIAuthInfo* auth;
    OCIDefine* defn;
    char result[100];

    // Initialize OCI environment
    std::cout << "Initializing OCI environment..." << std::endl;
    OCIEnvCreate(&env, OCI_DEFAULT, NULL, NULL, NULL, NULL, 0, NULL);

    // Allocate handles
    OCIHandleAlloc(env, (dvoid**)&srv, OCI_HTYPE_SERVER, 0, NULL);
    OCIHandleAlloc(env, (dvoid**)&err, OCI_HTYPE_ERROR, 0, NULL);
    OCIHandleAlloc(env, (dvoid**)&svc, OCI_HTYPE_SVCCTX, 0, NULL);
    OCIHandleAlloc(env, (dvoid**)&auth, OCI_HTYPE_AUTHINFO, 0, NULL);

    // Connect to the Oracle server
    std::cout << "Connecting to Oracle server..." << std::endl;
    char db_connection[512];
    snprintf(db_connection, sizeof(db_connection), DB, hostname, port);
    check_err(err, OCIServerAttach(srv, err, (OraText*)db_connection, strlen(db_connection), 0));
    OCIAttrSet(svc, OCI_HTYPE_SVCCTX, srv, 0, OCI_ATTR_SERVER, err);

    // Start session
    std::cout << "Starting session..." << std::endl;
    OCIHandleAlloc(env, (dvoid**)&sess, OCI_HTYPE_SESSION, 0, NULL);
OCIAttrSet(sess, OCI_HTYPE_SESSION, (void*)"system", strlen("system"), OCI_ATTR_USERNAME, err);
OCIAttrSet(sess, OCI_HTYPE_SESSION, (void*)"password", strlen("password"), OCI_ATTR_PASSWORD, err);


    check_err(err, OCISessionBegin(svc, err, sess, OCI_CRED_RDBMS, OCI_DEFAULT));
    OCIAttrSet(svc, OCI_HTYPE_SVCCTX, sess, 0, OCI_ATTR_SESSION, err);

    // Prepare and execute query
    std::cout << "Connected to Oracle database successfully!" << std::endl;
    const char* query = "SELECT 'Hello, World!' FROM DUAL";
    OCIHandleAlloc(env, (dvoid**)&stmt, OCI_HTYPE_STMT, 0, NULL);
    check_err(err, OCIStmtPrepare(stmt, err, (text*)query, (ub4)strlen(query), OCI_NTV_SYNTAX, OCI_DEFAULT));
    check_err(err, OCIStmtExecute(svc, stmt, err, 0, 0, NULL, NULL, OCI_DEFAULT));

    // Define the output buffer
    check_err(err, OCIDefineByPos(stmt, &defn, err, 1, (dvoid*)result, sizeof(result), SQLT_STR, NULL, NULL, NULL, OCI_DEFAULT));

    // Fetch the result
    std::cout << "Preparing and executing query: " << query << std::endl;
    check_err(err, OCIStmtFetch(stmt, err, 1, OCI_FETCH_NEXT, OCI_DEFAULT));

    // Print the result
    std::cout << "Query result: " << result << std::endl;

    // Clean up
    OCIHandleFree(stmt, OCI_HTYPE_STMT);
    OCISessionEnd(svc, err, sess, OCI_DEFAULT);
    OCIServerDetach(srv, err, OCI_DEFAULT);
    OCIHandleFree(env, OCI_HTYPE_ENV);

    return 0;
}
