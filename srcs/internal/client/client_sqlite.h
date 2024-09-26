#ifndef __CLIENT_SQLITE_H__
#define __CLIENT_SQLITE_H__

#include <optional>
#include <string>
#include <vector>
#include "client.h"
#include "sqlite3.h"
#include "yaml-cpp/yaml.h"

namespace client {

class SQLiteClient : public DBClient {
 public:
  virtual void initialize(YAML::Node);
  virtual void prepare_env();
  virtual ExecutionStatus execute(const char *query, size_t size,std::vector<std::vector<std::string>> &result);
  virtual void clean_up_env();
  virtual bool check_alive();

 private:
  std::optional<sqlite3*> create_connection();
  void close_connection(sqlite3* db);

  unsigned int database_id_ = 0;
  std::string db_prefix_;
  std::string db_path_;
};

}  // namespace client

#endif
