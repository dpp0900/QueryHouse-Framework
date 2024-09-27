#ifndef __CLIENT_ORACLE_H__
#define __CLIENT_ORACLE_H__

#include <string>
#include "client.h"
#include "yaml-cpp/yaml.h"

namespace client {

class OracleClient : public DBClient {
 public:
  virtual void initialize(YAML::Node);
  virtual void prepare_env();
  virtual ExecutionStatus execute(const char *query, size_t size);
  virtual void clean_up_env();
  virtual bool check_alive();

 private:
  std::string user_name_;
  std::string passwd_;
  std::string db_name_;
};

};  // namespace client

#endif
