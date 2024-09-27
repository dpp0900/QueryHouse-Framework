#include "oracle.h"
#include "mutator.h"  // Mutator 클래스 포함
#include <cassert>
#include <string>
#include <vector>

OracleDB *create_oracle() { return new OracleDB; }

OracleDB::OracleDB() { 
    mutator_ = std::make_unique<Mutator>();  // Mutator 객체 생성
}

bool OracleDB::initialize(YAML::Node config) {
  // Oracle 관련 초기화 코드
  return true;
}

bool OracleDB::save_interesting_query(const std::string &query) {
  // Oracle 쿼리 저장 코드
  return true;
}

size_t OracleDB::mutate(const std::string &query) {
  // Mutator의 변이 기능을 사용
  mutator_->mutate_query(query);
  return 0;
}

std::string OracleDB::get_next_mutated_query() {
  // Oracle 변이된 쿼리 가져오기
  return "";
}

