#ifndef __ORACLE_H__
#define __ORACLE_H__

#include <memory>
#include <stack>
#include <string>
#include <vector>

#include "db.h"   // DataBase 클래스가 포함된 헤더
#include "mutator.h" // Mutator 클래스가 정의된 헤더

class IR; // IR 클래스의 전방 선언

class OracleDB : public DataBase {
 public:
  OracleDB();
  virtual bool initialize(YAML::Node config); // YAML::Node로부터 초기화
  virtual size_t mutate(const std::string &query); // 쿼리 변이를 처리
  virtual bool save_interesting_query(const std::string &query); // 흥미로운 쿼리 저장
  virtual std::string get_next_mutated_query(); // 변이된 쿼리 반환
  virtual bool has_mutated_test_cases() { return !validated_test_cases_.empty(); } // 변이된 테스트 케이스가 있는지 확인
  virtual bool clean_up() { return true; } // 정리 작업 (필요 시 구현 가능)

 private:
  size_t validate_all(std::vector<IR *> &ir_set); // IR 집합에 대한 유효성 검사를 수행
  std::unique_ptr<Mutator> mutator_; // Mutator의 유일한 인스턴스를 관리
  std::stack<std::string> validated_test_cases_; // 유효성 검사된 테스트 케이스를 저장하는 스택
};

// OracleDB 생성 함수를 정의
OracleDB *create_oracle();

#endif

