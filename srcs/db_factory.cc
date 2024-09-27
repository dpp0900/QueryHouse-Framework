#include <cassert>

#ifdef __SQUIRREL_SQLITE__
#include "sqlite.h"
#endif

#ifdef __SQUIRREL_MYSQL__
#include "mysql.h"
#endif

#ifdef __SQUIRREL_POSTGRESQL__
#include "postgresql.h"
#endif

#ifdef __SQUIRREL_ORACLE__
#include "oracle.h" // OracleDB 관련 헤더 파일 추가
#endif

//#include "db.h"

DataBase* create_database(YAML::Node config) {
  const std::string db_name = config["db"].as<std::string>();
  DataBase* result = nullptr;

  if (db_name == "sqlite") {
#ifdef __SQUIRREL_SQLITE__
    result = create_sqlite();
#endif
  } else if (db_name == "mysql") {
#ifdef __SQUIRREL_MYSQL__
    result = create_mysql();
#endif
  } else if (db_name == "postgresql") {
#ifdef __SQUIRREL_POSTGRESQL__
    result = create_postgresql();
#endif
  } else if (db_name == "oracle") {  // OracleDB 처리 추가
#ifdef __SQUIRREL_ORACLE__
    result = create_oracle();
#endif
  } else {
    assert(false && "Unreachable");
  }

  assert(result && "Unreachable");
  result->initialize(config);
  return result;
}

