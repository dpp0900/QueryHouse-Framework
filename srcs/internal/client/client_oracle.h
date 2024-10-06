#ifndef __CLIENT_ORACLE_H__
#define __CLIENT_ORACLE_H__

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

#include "client.h"
#include "oci.h"
#include "yaml-cpp/yaml.h"

namespace client {

class OracleClient : public DBClient {
 public:
  // Initialize the Oracle client using a YAML configuration node
  virtual void initialize(YAML::Node config) override;

  // Set up a clean environment for execution (e.g., schema creation)
  virtual void prepare_env() override;

  // Execute a query and fetch results
  virtual ExecutionStatus execute(const char *query, size_t size, std::vector<std::vector<std::string>> &result) override;

  // Clean up the environment (e.g., drop schema)
  virtual void clean_up_env() override;

  // Check if the Oracle connection is alive
  virtual bool check_alive() override;

 private:
  // Cleans up the Oracle connection resources
  ExecutionStatus clean_up_connection(OCIEnv *env, OCIError *err, OCISvcCtx *svc);

  // Creates a connection to the Oracle database
  bool create_connection(OCIEnv *&env, OCIError *&err, OCISvcCtx *&svc);

  // Create a schema or user (similar to creating a database in MySQL)
  bool create_schema(const std::string &schema_name);

  // Oracle connection parameters
  std::string host_;      // Oracle host
  std::string port_;      // Oracle port
  std::string service_;   // Oracle service name
  std::string user_name_; // Oracle username
  std::string passwd_;    // Oracle password

  int database_id_ = 0;
  std::string db_prefix_;  // Prefix for schema names (equivalent to DB in MySQL)
  std::string default_schema_;  // Default schema name for the current connection
};

};  // namespace client

#endif  // __CLIENT_ORACLE_H__
