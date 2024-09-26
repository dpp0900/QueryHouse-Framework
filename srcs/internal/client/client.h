#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <cstddef>
#include <string>
#include <vector>

#include "yaml-cpp/yaml.h"

namespace client {


inline std::vector<std::string> split_query(const char *buf, size_t len) {
  std::vector<std::string> queries;
  std::string query;
  for (size_t i = 0; i < len; i++) {
    if (buf[i] == ';') {
      query.push_back(buf[i]);
      queries.push_back(query);
      query.clear();
    } else {
      query.push_back(buf[i]);
    }
  }
  return queries;
}
enum ExecutionStatus {
  kConnectFailed,
  kExecuteError,
  kServerCrash,
  kNormal,
  kTimeout,
  kSyntaxError,
  kSemanticError
};

class DBClient {
 public:
  virtual void initialize(YAML::Node) = 0;
  virtual bool check_alive() = 0;
  // Set up a clean environment for execution.
  virtual void prepare_env() = 0;
  virtual ExecutionStatus execute(const char *query, size_t size,std::vector<std::vector<std::string>> &result) = 0;
  virtual void clean_up_env() {}
};

DBClient *create_client(const std::string &db_name, const YAML::Node &config);
};  // namespace client

#endif
