#include "client_oracle.h"

#include <iostream>
#include <oci.h>
#include <string>
#include <vector>

using namespace std;

namespace client {

void OracleClient::initialize(YAML::Node config) {
  host_ = config["host"].as<std::string>();
  port_ = config["port"].as<std::string>();
  service_ = config["service"].as<std::string>();
  user_name_ = config["user_name"].as<std::string>();
  passwd_ = config["passwd"].as<std::string>();
  db_prefix_ = config["db_prefix"].as<std::string>();
}

void OracleClient::prepare_env() {
  ++database_id_;
  std::string schema_name = db_prefix_ + std::to_string(database_id_);
  
  // 스키마 생성
  if (!create_schema(schema_name)) {
    std::cerr << "Failed to create schema." << std::endl;
  } else {
    // 새로 만든 스키마 이름 저장
    default_schema_ = "C##" + schema_name;
  }
}

ExecutionStatus OracleClient::execute(const char *query, size_t size, std::vector<std::vector<std::string>> &result) {
    std::vector<std::string> queries = split_query(query, size);
    for (auto &q : queries) {
        if (q.back() == ';') {
            q.pop_back();
        }
    }
    
    OCIEnv *env = nullptr;
    OCIError *err = nullptr;
    OCISvcCtx *svc = nullptr;
    OCIDefine *defn = nullptr;

    char result_string[1024];

    if (!create_connection(env, err, svc)) {
        std::cerr << "Cannot create connection at execute" << std::endl;
        return kServerCrash;
    }

    for (const auto &q : queries) {
        std::cout << "[Oracle] Execute query: " << q << std::endl;
        OCIStmt *stmt = nullptr;
        OCIHandleAlloc(env, (void **)&stmt, OCI_HTYPE_STMT, 0, nullptr);

        if (OCIStmtPrepare(stmt, err, (text *)q.c_str(), (ub4)q.size(), OCI_NTV_SYNTAX, OCI_DEFAULT) != OCI_SUCCESS) {
            std::cerr << "Failed to prepare statement" << std::endl;

            // Retrieve detailed error message
            text errbuf[512];
            sb4 errcode;
            OCIErrorGet(err, 1, NULL, &errcode, errbuf, sizeof(errbuf), OCI_HTYPE_ERROR);
            std::cerr << "Error - " << errbuf << std::endl;

            clean_up_connection(env, err, svc);
            return kSyntaxError;
        }

        // Check if it's a SELECT statement
        if (q.find("SELECT") != std::string::npos) {
            // Define the output variable (before execution)
            if (OCIDefineByPos(stmt, &defn, err, 1, (dvoid*)result_string, 1024, SQLT_STR, NULL, NULL, NULL, OCI_DEFAULT) != OCI_SUCCESS) {
                std::cerr << "Failed to define result" << std::endl;

                // Retrieve detailed error message
                text errbuf[512];
                sb4 errcode;
                OCIErrorGet(err, 1, NULL, &errcode, errbuf, sizeof(errbuf), OCI_HTYPE_ERROR);
                std::cerr << "Error - " << errbuf << std::endl;

                clean_up_connection(env, err, svc);
                return kSemanticError;
            }

            // Execute the query
            if (OCIStmtExecute(svc, stmt, err, 0, 0, nullptr, nullptr, OCI_DEFAULT) != OCI_SUCCESS) {
                std::cerr << "Failed to execute SELECT query" << std::endl;

                // Retrieve detailed error message
                text errbuf[512];
                sb4 errcode;
                OCIErrorGet(err, 1, NULL, &errcode, errbuf, sizeof(errbuf), OCI_HTYPE_ERROR);
                std::cerr << "Error - " << errbuf << std::endl;

                clean_up_connection(env, err, svc);
                return kSemanticError;
            }

            // Fetch the result
            while (OCIStmtFetch(stmt, err, 1, OCI_FETCH_NEXT, OCI_DEFAULT) == OCI_SUCCESS) {
                std::cout << "Result: " << result_string << std::endl;
                result.push_back({result_string});
            }
        } else {
            // For non-SELECT statements, simply execute
            if (OCIStmtExecute(svc, stmt, err, 1, 0, nullptr, nullptr, OCI_DEFAULT) != OCI_SUCCESS) {
                std::cerr << "Failed to execute non-SELECT query" << std::endl;

                // Retrieve detailed error message
                text errbuf[512];
                sb4 errcode;
                OCIErrorGet(err, 1, NULL, &errcode, errbuf, sizeof(errbuf), OCI_HTYPE_ERROR);
                std::cerr << "Error - " << errbuf << std::endl;

                clean_up_connection(env, err, svc);
                return kSemanticError;
            }
        }

        OCIHandleFree(stmt, OCI_HTYPE_STMT);
    }

    clean_up_connection(env, err, svc);
    return kNormal;
}



void OracleClient::clean_up_env() {
  std::string schema_name = db_prefix_ + std::to_string(database_id_);
  std::string prefix = "C##";
  std::string common_user_name = prefix + schema_name;
  std::string drop_query = "DROP USER " + common_user_name + " CASCADE";

  std::cout << "Dropping schema: " << drop_query << std::endl;
  
  OCIEnv *env = nullptr;
  OCIError *err = nullptr;
  OCISvcCtx *svc = nullptr;

  if (create_connection(env, err, svc)) {
    OCIStmt *stmt = nullptr;
    OCIHandleAlloc(env, (void **)&stmt, OCI_HTYPE_STMT, 0, nullptr);
    OCIStmtPrepare(stmt, err, (text *)drop_query.c_str(), (ub4)drop_query.size(), OCI_NTV_SYNTAX, OCI_DEFAULT);
    OCIStmtExecute(svc, stmt, err, 1, 0, nullptr, nullptr, OCI_DEFAULT);
    OCIHandleFree(stmt, OCI_HTYPE_STMT);
    clean_up_connection(env, err, svc);
  }
}

bool OracleClient::check_alive() {
  OCIEnv *env = nullptr;
  OCIError *err = nullptr;
  OCISvcCtx *svc = nullptr;

  bool alive = create_connection(env, err, svc);
  if (alive) {
    clean_up_connection(env, err, svc);
  }
  return alive;
}

bool OracleClient::create_schema(const std::string &schema_name) {
  std::string common_user_name = "C##" + schema_name;
  std::string create_user_query = "CREATE USER " + common_user_name + " IDENTIFIED BY password";
  std::string grant_query = "GRANT ALL PRIVILEGES TO " + common_user_name;

  OCIEnv *env = nullptr;
  OCIError *err = nullptr;
  OCISvcCtx *svc = nullptr;

  if (!create_connection(env, err, svc)) {
    return false;
  }

  OCIStmt *stmt = nullptr;
  OCIHandleAlloc(env, (void **)&stmt, OCI_HTYPE_STMT, 0, nullptr);

  // Try to execute the "CREATE USER" query
  if (OCIStmtPrepare(stmt, err, (text *)create_user_query.c_str(), (ub4)create_user_query.size(), OCI_NTV_SYNTAX, OCI_DEFAULT) != OCI_SUCCESS) {
    std::cerr << "Failed to prepare CREATE USER statement" << std::endl;
    clean_up_connection(env, err, svc);
    return false;
  }

  if (OCIStmtExecute(svc, stmt, err, 1, 0, nullptr, nullptr, OCI_DEFAULT) != OCI_SUCCESS) {
    std::cerr << "Failed to create schema" << std::endl;
    
    // Retrieve detailed error message
    text errbuf[512];
    sb4 errcode;
    OCIErrorGet(err, 1, NULL, &errcode, errbuf, sizeof(errbuf), OCI_HTYPE_ERROR);
    std::cerr << "Error - " << errbuf << std::endl;
    
    clean_up_connection(env, err, svc);
    return false;
  }

  // Grant privileges
  if (OCIStmtPrepare(stmt, err, (text *)grant_query.c_str(), (ub4)grant_query.size(), OCI_NTV_SYNTAX, OCI_DEFAULT) != OCI_SUCCESS) {
    std::cerr << "Failed to prepare GRANT statement" << std::endl;
    clean_up_connection(env, err, svc);
    return false;
  }

  OCIStmtExecute(svc, stmt, err, 1, 0, nullptr, nullptr, OCI_DEFAULT);
  OCIHandleFree(stmt, OCI_HTYPE_STMT);
  
  clean_up_connection(env, err, svc);
  return true;
}

bool OracleClient::create_connection(OCIEnv *&env, OCIError *&err, OCISvcCtx *&svc) {
  // Initialize OCI environment
  if (OCIEnvCreate(&env, OCI_DEFAULT, nullptr, nullptr, nullptr, nullptr, 0, nullptr) != OCI_SUCCESS) {
    std::cerr << "Failed to create OCI environment" << std::endl;
    return false;
  }

  // Allocate OCI error handle
  OCIHandleAlloc(env, (void **)&err, OCI_HTYPE_ERROR, 0, nullptr);

  // Create connection string: "//host:port/service"
  std::string conn_string = "//" + host_ + ":" + port_ + "/" + service_;

  // Log in to the database
  if (OCILogon(env, err, &svc, (text *)user_name_.c_str(), (ub4)user_name_.size(),
               (text *)passwd_.c_str(), (ub4)passwd_.size(), (text *)conn_string.c_str(), (ub4)conn_string.size()) != OCI_SUCCESS) {
    std::cerr << "Failed to connect to Oracle database" << std::endl;
    OCIHandleFree(err, OCI_HTYPE_ERROR);
    OCIHandleFree(env, OCI_HTYPE_ENV);
    return false;
  }

  // Change schema to the newly created schema
  if (!default_schema_.empty()) {
    std::string alter_schema_query = "ALTER SESSION SET CURRENT_SCHEMA = " + default_schema_;
    OCIStmt *stmt = nullptr;
    OCIHandleAlloc(env, (void **)&stmt, OCI_HTYPE_STMT, 0, nullptr);
    if (OCIStmtPrepare(stmt, err, (text *)alter_schema_query.c_str(), (ub4)alter_schema_query.size(), OCI_NTV_SYNTAX, OCI_DEFAULT) != OCI_SUCCESS) {
      std::cerr << "Failed to change schema after connection" << std::endl;
      OCIHandleFree(stmt, OCI_HTYPE_STMT);
      clean_up_connection(env, err, svc);
      return false;
    }
    OCIStmtExecute(svc, stmt, err, 1, 0, nullptr, nullptr, OCI_DEFAULT);
    OCIHandleFree(stmt, OCI_HTYPE_STMT);
  }

  return true;
}

ExecutionStatus OracleClient::clean_up_connection(OCIEnv *env, OCIError *err, OCISvcCtx *svc) {
  if (svc) {
    OCILogoff(svc, err);
  }
  if (err) {
    OCIHandleFree(err, OCI_HTYPE_ERROR);
  }
  if (env) {
    OCIHandleFree(env, OCI_HTYPE_ENV);
  }
  return kNormal;
}

};  // namespace client
