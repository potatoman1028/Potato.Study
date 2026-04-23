# Phase 05 - JSON to Custom Struct

## 목표

JSON object를 `boost::json::value_to<T>()`로 사용자 정의 struct로 복원하는 가장 작은 예제를 만든다.

## 이번 단계에서 볼 것

- `value_to<T>()`
- `value_to_tag<T>`
- `tag_invoke`
- JSON object -> custom struct 변환
- nested custom struct 복원

## 코드 포인트

- 메뉴 연결: [main.cpp](../main.cpp)
- phase 실행 함수: [Phase05JsonToCustomStruct.cpp](../phases/Phase05JsonToCustomStruct.cpp)
- custom conversion 정의: [Phase05JsonToCustomStruct.cpp](../phases/Phase05JsonToCustomStruct.cpp)

## 핵심 이해

- `value_to<T>()`는 `json::value`를 원하는 타입 `T`로 바꾼다.
- 사용자 정의 타입은 `tag_invoke(const value_to_tag<T>&, const value&)`를 정의해 복원 규칙을 직접 적을 수 있다.
- nested custom struct도 내부에서 다시 `value_to<InnerType>()`를 호출해 자연스럽게 이어서 복원할 수 있다.
- 표준 컨테이너인 `std::vector<std::string>`은 별도 custom conversion 없이도 array에서 복원된다.

## 직접 해볼 것

- `author.active`를 `false`로 바꾸고 struct 값이 어떻게 달라지는지 확인한다.
- `tags` 배열에 값을 하나 더 추가하고 `tags size` 출력이 어떻게 바뀌는지 본다.
- `phase`를 문자열로 바꾸고 어느 지점에서 에러가 나는지 확인한다.
- `title` key를 `lesson_title`로 바꾸고 현재 복원 코드가 어떤 예외를 내는지 본다.

## 배운 점

## 다음 질문

- `value_to<T>()`는 내부에서 어떤 순서로 변환 함수를 찾는가?
- `tag_invoke(value_to_tag<T>, ...)`는 왜 값을 반환하는 형태인가?
- 없는 key를 `at()`으로 읽을 때와 타입이 다른 값을 읽을 때 에러는 어떻게 구분되는가?
- 복원 과정에서 기본값을 주고 싶다면 `tag_invoke` 안에서 어떤 식으로 처리하는가?
