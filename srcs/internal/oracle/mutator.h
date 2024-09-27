// mutator.h
#ifndef __MUTATOR_H__
#define __MUTATOR_H__

#include <string>

class Mutator {
 public:
  Mutator() {}  // 생성자
  ~Mutator() {} // 소멸자

  // 쿼리를 변이시키는 가상의 함수 선언
  void mutate_query(const std::string &query) {
    // 쿼리를 변이시키는 로직을 여기에 작성
  }
};

#endif  // __MUTATOR_H__

