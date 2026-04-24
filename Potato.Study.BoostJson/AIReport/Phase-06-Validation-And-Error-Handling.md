# Phase 06 - Validation and Error Handling

## 목표

잘못된 JSON text, 필드 누락, 타입 mismatch를 각각 구분해서 처리하는 가장 작은 예제를 만든다.

## 이번 단계에서 볼 것

- `json::parse(source, ec)`
- `boost::system::error_code`
- `object.find()`
- `is_string()`, `is_int64()`, `is_bool()`, `is_array()`
- `std::optional`
- 기본값 처리

## 코드 포인트

- 메뉴 연결: [main.cpp](../main.cpp)
- phase 실행 함수: [Phase06ValidationAndErrorHandling.cpp](../phases/Phase06ValidationAndErrorHandling.cpp)
- validation 함수: [Phase06ValidationAndErrorHandling.cpp](../phases/Phase06ValidationAndErrorHandling.cpp)

## 핵심 이해

- parse error와 validation error는 서로 다른 단계의 실패다.
- JSON text 자체가 깨졌으면 `parse` 단계에서 실패한다.
- parse는 성공했지만 필드가 없거나 타입이 다르면 validation 단계에서 실패한다.
- `at()`으로 바로 읽는 대신 `find()`와 `is_*()`를 쓰면 어떤 실패인지 직접 설명할 수 있다.
- 선택 필드는 없을 때 기본값을 넣을지, 에러로 볼지 validation 함수 안에서 결정할 수 있다.

## 직접 해볼 것

- `parse failure` 예제의 JSON을 고쳐서 validation 단계까지 진행되는지 확인한다.
- `missing field` 예제에 `title`을 추가하고 성공하는지 확인한다.
- `type mismatch` 예제에서 `phase`를 숫자로 되돌리고 결과가 어떻게 바뀌는지 본다.
- `default enabled` 예제에 `"enabled": false`를 추가하고 출력이 어떻게 바뀌는지 본다.
- `tags` 배열 안에 숫자를 하나 넣고 어떤 validation error가 나오는지 확인한다.

## 배운 점

## 다음 질문

- validation error를 하나만 반환하는 방식과 여러 개 모으는 방식은 어떤 차이가 있는가?
- `find()` 기반 validation과 예외 기반 validation은 각각 언제 더 자연스러운가?
- optional 필드와 required 필드는 코드에서 어떻게 구분하면 읽기 좋은가?
- validation 로직은 `tag_invoke(value_to_tag<T>, ...)` 안에 넣는 편이 좋은가, 별도 함수로 분리하는 편이 좋은가?
