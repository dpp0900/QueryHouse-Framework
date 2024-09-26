#include "client_sqlite.h"
#include <iostream>
#include <string>
#include <vector>

namespace client {

void SQLiteClient::initialize(YAML::Node config) {
  db_prefix_ = config["db_prefix"].as<std::string>();
  db_path_ = config["db_path"].as<std::string>();
}

void SQLiteClient::prepare_env() {
  ++database_id_;
  std::string db_name = db_prefix_ + std::to_string(database_id_) + ".db";
  db_path_ = db_name;
  std::cout << "[SQLite] Prepared environment with DB: " << db_path_ << std::endl;
}

ExecutionStatus SQLiteClient::execute(const char *query, size_t size) {
  std::vector<std::string> queries = split_query(query, size);
  sqlite3 *db = nullptr;
  if (auto conn = create_connection(); conn) {
    db = *conn;
  } else {
    std::cerr << "Failed to connect to SQLite database" << std::endl;
    return kServerCrash;
  }
  for (const auto &q : queries) {
    char *err_msg = nullptr;
    std::cout << "[SQLite] Execute query: " << q << std::endl;
    int result = sqlite3_exec(db, q.c_str(), nullptr, nullptr, &err_msg);
    if (result != SQLITE_OK) {
      std::cerr << "SQLite error: " << err_msg << std::endl;
      sqlite3_free(err_msg);
      close_connection(db);
      return kSyntaxError;
    }
  }
  close_connection(db);
  return kNormal;
}

void SQLiteClient::clean_up_env() {
  std::string db_name = db_prefix_ + std::to_string(database_id_) + ".db";
  if (remove(db_name.c_str()) != 0) {
    std::cerr << "Error deleting database file: " << db_name << std::endl;
  } else {
    std::cout << "[SQLite] Deleted database: " << db_name << std::endl;
  }
}

bool SQLiteClient::check_alive() {
  if (auto conn = create_connection(); conn) {
    close_connection(*conn);
    return true;
  }
  return false;
}

std::optional<sqlite3*> SQLiteClient::create_connection() {
  sqlite3 *db = nullptr;
  if (sqlite3_open(db_path_.c_str(), &db) != SQLITE_OK) {
    std::cerr << "Failed to open SQLite database: " << sqlite3_errmsg(db) << std::endl;
    return std::nullopt;
  }
  return db;
}

void SQLiteClient::close_connection(sqlite3* db) {
  sqlite3_close(db);
}

}  // namespace client
