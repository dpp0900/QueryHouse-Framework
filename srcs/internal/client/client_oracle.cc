#include "client_oracle.h"

#include <oci.h>
#include <iostream>
#include <string>
#include <optional>
#include <vector>

using namespace std;

namespace {
bool is_crash_response(sword status) {
  return status == OCI_ERROR || status == OCI_INVALID_HANDLE;
}
};  // namespace

namespace client {

std::string get_error_message(OCIError* err) {
    OraText err_msg[512];
    sb4 err_code;

    // SQLSTATE를 사용하지 않으므로 nullptr로 설정
    OraText sqlstate[6]; // SQLSTATE는 5 문자로 구성되므로 배열을 선언합니다.

    // Record number를 1로 설정하여 가장 최근 오류를 가져옵니다.
    OCIErrorGet(err, 1, sqlstate, &err_code, err_msg, sizeof(err_msg), OCI_DEFAULT);

    return std::string((char*)err_msg);
}




void OracleClient::initialize(YAML::Node config) {
  host_ = config["host"].as<std::string>();
  user_name_ = config["user_name"].as<std::string>();
  passwd_ = config["passwd"].as<std::string>();
  db_prefix_ = config["db_prefix"].as<std::string>();
}

void OracleClient::prepare_env() {
  ++database_id_;
  std::string database_name = db_prefix_ + std::to_string(database_id_);
  if (!create_schema(database_name)) {
    std::cerr << "Failed to create schema." << std::endl;
  }
}

ExecutionStatus OracleClient::execute(const char* query, size_t size, std::vector<std::vector<std::string>>& result) {
  // Create a connection for executing the query
  std::optional<OCIEnv*> env = create_env();
  if (!env.has_value()) {
    std::cerr << "Cannot create OCI environment." << std::endl;
    return kServerCrash;
  }

  std::optional<OCISvcCtx*> svc_ctx = create_connection(env.value());
  if (!svc_ctx.has_value()) {
    std::cerr << "Cannot create OCI connection." << std::endl;
    OCIHandleFree(env.value(), OCI_HTYPE_ENV);
    return kServerCrash;
  }

  OCIStmt* stmt = nullptr;
  OCIError* err = nullptr;

  // Allocate memory for the error handle
  OCIHandleAlloc(env.value(), (void**)&err, OCI_HTYPE_ERROR, 0, nullptr);

  // Prepare the statement
  if (OCIStmtPrepare2(svc_ctx.value(), &stmt, err, (const OraText*)query, size, nullptr, 0, OCI_NTV_SYNTAX, OCI_DEFAULT) != OCI_SUCCESS) {
    std::cerr << "OCI statement preparation failed." << std::endl;
    OCIStmtRelease(stmt, err, nullptr, 0, OCI_DEFAULT);
    OCIHandleFree(err, OCI_HTYPE_ERROR);  // Free error handle after use
    return kSyntaxError;
  }

  // Execute the statement
  if (OCIStmtExecute(svc_ctx.value(), stmt, err, 1, 0, nullptr, nullptr, OCI_DEFAULT) != OCI_SUCCESS) {
    std::cerr << "OCI statement execution failed." << std::endl;
    OCIStmtRelease(stmt, err, nullptr, 0, OCI_DEFAULT);
    OCIHandleFree(err, OCI_HTYPE_ERROR);  // Free error handle after use
    return kServerCrash;
  }

  // Fetch result set
  OCIParam* param = nullptr;
  OCIHandleAlloc(env.value(), (void**)&param, OCI_HTYPE_DESCRIBE, 0, nullptr);
  ub4 num_cols = 0;
  OCIAttrGet(stmt, OCI_HTYPE_STMT, &num_cols, nullptr, OCI_ATTR_PARAM_COUNT, err);

  for (ub4 i = 1; i <= num_cols; i++) {
    OCIParam* col_param = nullptr;
    OCIParamGet(stmt, OCI_HTYPE_STMT, err, (void**)&col_param, i);
    
    OraText* col_name;
    ub4 col_name_len;
    OCIAttrGet(col_param, OCI_DTYPE_PARAM, &col_name, &col_name_len, OCI_ATTR_NAME, err);

    std::vector<std::string> row_result;
    row_result.push_back(std::string((char*)col_name, col_name_len));

    result.push_back(row_result);
  }

  // Release the statement and free the handles
  OCIStmtRelease(stmt, err, nullptr, 0, OCI_DEFAULT);
  OCIHandleFree(err, OCI_HTYPE_ERROR);  // Free error handle after use
  OCIHandleFree(env.value(), OCI_HTYPE_ENV);
  
  return kNormal;
}


void OracleClient::clean_up_env() {
  std::optional<OCIEnv*> env = create_env();
  if (!env.has_value()) {
    std::cerr << "Cannot create OCI environment for cleanup." << std::endl;
    return;
  }

  std::optional<OCISvcCtx*> svc_ctx = create_connection(env.value());
  if (!svc_ctx.has_value()) {
    std::cerr << "Cannot create OCI connection for cleanup." << std::endl;
    OCIHandleFree(env.value(), OCI_HTYPE_ENV);
    return;
  }

  OCIStmt* stmt = nullptr;
  OCIError* err = nullptr;

  // Allocate memory for the error handle
  OCIHandleAlloc(env.value(), (void**)&err, OCI_HTYPE_ERROR, 0, nullptr);

  std::string database_name = db_prefix_ + std::to_string(database_id_);
  std::string reset_query = "DROP USER " + database_name + " CASCADE";

  // Prepare the statement
  if (OCIStmtPrepare2(svc_ctx.value(), &stmt, err, (const OraText*)reset_query.c_str(), reset_query.size(), nullptr, 0, OCI_NTV_SYNTAX, OCI_DEFAULT) != OCI_SUCCESS) {
    std::cerr << "OCI statement preparation failed during cleanup." << std::endl;
    OCIStmtRelease(stmt, err, nullptr, 0, OCI_DEFAULT);
    OCIHandleFree(err, OCI_HTYPE_ERROR);  // Free error handle after use
    OCIHandleFree(env.value(), OCI_HTYPE_ENV);
    return;
  }

  // Execute the statement
  if (OCIStmtExecute(svc_ctx.value(), stmt, err, 1, 0, nullptr, nullptr, OCI_DEFAULT) != OCI_SUCCESS) {
    std::cerr << "OCI statement execution failed during cleanup." << std::endl;
    OCIStmtRelease(stmt, err, nullptr, 0, OCI_DEFAULT);
    OCIHandleFree(err, OCI_HTYPE_ERROR);  // Free error handle after use
    OCIHandleFree(env.value(), OCI_HTYPE_ENV);
    return;
  }

  // Release the statement and free the handles
  OCIStmtRelease(stmt, err, nullptr, 0, OCI_DEFAULT);
  OCIHandleFree(err, OCI_HTYPE_ERROR);  // Free error handle after use
  OCIHandleFree(env.value(), OCI_HTYPE_ENV);
}


std::optional<OCIEnv*> OracleClient::create_env() {
  OCIEnv* env = nullptr;
  if (OCIEnvCreate(&env, OCI_DEFAULT, nullptr, nullptr, nullptr, nullptr, 0, nullptr) != OCI_SUCCESS) {
    std::cerr << "Failed to create OCI environment." << std::endl;
    return std::nullopt;
  }
  return env;
}

std::optional<OCISvcCtx*> OracleClient::create_connection(OCIEnv* env) {
  OCISvcCtx* svc_ctx = nullptr;
  OCIHandleAlloc(env, (void**)&svc_ctx, OCI_HTYPE_SVCCTX, 0, nullptr);
  
  OCIError* err = nullptr;
  OCIHandleAlloc(env, (void**)&err, OCI_HTYPE_ERROR, 0, nullptr);

  if (OCILogon(env, err, &svc_ctx, (OraText*)user_name_.c_str(), user_name_.length(),
               (OraText*)passwd_.c_str(), passwd_.length(),
               (OraText*)host_.c_str(), host_.length()) != OCI_SUCCESS) {
    std::cerr << "Failed to connect to Oracle." << std::endl;
    OCIHandleFree(err, OCI_HTYPE_ERROR);
    OCIHandleFree(svc_ctx, OCI_HTYPE_SVCCTX);
    return std::nullopt;
  }

  OCIHandleFree(err, OCI_HTYPE_ERROR);
  return svc_ctx;
}

bool OracleClient::check_alive() {
  std::optional<OCIEnv*> env = create_env();
  if (!env.has_value()) {
    return false;
  }

  std::optional<OCISvcCtx*> svc_ctx = create_connection(env.value());
  if (!svc_ctx.has_value()) {
    return false;
  }

  OCIHandleFree(env.value(), OCI_HTYPE_ENV);
  return true;
}

bool OracleClient::create_schema(const std::string &schema_name) {
    std::optional<OCIEnv*> env = create_env();
    if (!env.has_value()) {
        std::cerr << "Cannot create OCI environment." << std::endl;
        return false;
    }

    std::optional<OCISvcCtx*> svc_ctx = create_connection(env.value());
    if (!svc_ctx.has_value()) {
        std::cerr << "Cannot create OCI connection." << std::endl;
        OCIHandleFree(env.value(), OCI_HTYPE_ENV);
        return false;
    }

    OCIStmt* stmt = nullptr;
    OCIError* err = nullptr;

    // Allocate memory for the error handle
    OCIHandleAlloc(env.value(), (void**)&err, OCI_HTYPE_ERROR, 0, nullptr);

    std::string cmd = "CREATE USER " + schema_name + " IDENTIFIED BY password";
    if (OCIStmtPrepare2(svc_ctx.value(), &stmt, err, (const OraText*)cmd.c_str(), cmd.size(), nullptr, 0, OCI_NTV_SYNTAX, OCI_DEFAULT) != OCI_SUCCESS) {
        std::cerr << "OCI statement preparation failed: " << get_error_message(err) << std::endl;
        OCIHandleFree(err, OCI_HTYPE_ERROR);  // Free error handle
        OCIHandleFree(env.value(), OCI_HTYPE_ENV);
        return false;
    }

    // Execute the statement
    if (OCIStmtExecute(svc_ctx.value(), stmt, err, 1, 0, nullptr, nullptr, OCI_DEFAULT) != OCI_SUCCESS) {
        std::cerr << "OCI statement execution failed: " << get_error_message(err) << std::endl;
        OCIStmtRelease(stmt, err, nullptr, 0, OCI_DEFAULT);
        OCIHandleFree(err, OCI_HTYPE_ERROR);  // Free error handle
        OCIHandleFree(env.value(), OCI_HTYPE_ENV);
        return false;
    }

    // Clean up
    OCIStmtRelease(stmt, err, nullptr, 0, OCI_DEFAULT);
    OCIHandleFree(err, OCI_HTYPE_ERROR);  // Free error handle
    OCIHandleFree(env.value(), OCI_HTYPE_ENV);
    return true;
}



};  // namespace client
