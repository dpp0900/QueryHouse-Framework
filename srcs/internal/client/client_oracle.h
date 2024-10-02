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
  // Initialize client with YAML configuration
  virtual void initialize(YAML::Node config);
  
  // Set up a clean environment for execution
  virtual void prepare_env();
  
  // Execute a query and store results
  virtual ExecutionStatus execute(const char* query, size_t size, std::vector<std::vector<std::string>>& result);
  
  // Clean up the environment after execution
  virtual void clean_up_env();
  
  // Check if the Oracle server is alive
  virtual bool check_alive();

 private:
  ExecutionStatus clean_up_connection(OCIEnv* env);
  
  // Create schema (equivalent to creating database in MySQL)
  bool create_schema(const std::string& schema);

  // Create a new OCI environment
  std::optional<OCIEnv*> create_env();

  // Create a connection to the Oracle server
  std::optional<OCISvcCtx*> create_connection(OCIEnv* env);

  unsigned int database_id_ = 0;
  std::string host_;
  std::string user_name_;
  std::string passwd_;
  std::string db_prefix_;
};

};  // namespace client

#endif
