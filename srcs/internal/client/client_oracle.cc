#include "client_oracle.h"
#include <oci.h>
#include <iostream>
#include <optional>
#include <string>
#include <cstring>

namespace client {

void OracleClient::initialize(YAML::Node config) {
  user_name_ = config["user_name"].as<std::string>();
  passwd_ = config["passwd"].as<std::string>();
  db_name_ = config["db_name"].as<std::string>();
}

void OracleClient::prepare_env() {
  // Oracle 환경 설정 코드
}

ExecutionStatus OracleClient::execute(const char *query, size_t size) {
  std::cerr << "Execute query: " << query << std::endl;

  // Oracle 연결 및 쿼리 실행 코드
  OCIEnv *env = nullptr;
  OCIError *err = nullptr;
  OCISvcCtx *svc = nullptr;
  OCISession *sess = nullptr;

  // 연결 초기화 및 쿼리 실행 후 상태 반환
  if (OCI_SUCCESS != OCIInitialize(OCI_DEFAULT, nullptr, nullptr, nullptr, nullptr)) {
    std::cerr << "Failed to initialize Oracle environment." << std::endl;
    return kServerCrash;
  }
  return kNormal;
}

void OracleClient::clean_up_env() {
  // Oracle 환경 정리 코드
}

bool OracleClient::check_alive() {
  // Oracle 서버의 상태 확인 코드
  return true;
}

}  // namespace client

