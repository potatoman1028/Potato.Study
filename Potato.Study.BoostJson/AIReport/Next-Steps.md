## TODO And Backlog

## 목표

`Boost.JSON`을 사용해 JSON serializer 자체를 어떤 순서로 학습할지 TODO 목록으로 관리한다.

현재 범위는 `config load`가 아니라 아래 주제에 집중한다.

- JSON text와 DOM 사이 변환
- C++ 타입과 JSON 사이 변환
- 잘못된 입력 처리
- 성능 기초

## 현재 작업 phase

- [Phase-05-Json-To-Custom-Struct.md](./Phase-05-Json-To-Custom-Struct.md)

## TODO List

- [x] Phase 01 - Parse and Serialize
  - JSON 문자열을 `boost::json::value`로 파싱하고 다시 문자열로 serialize 한다.
- [x] Phase 02 - Read JSON Types
  - object, array, string, number, bool, null을 읽는 가장 작은 예제를 만든다.
- [x] Phase 03 - Build JSON Programmatically
  - C++ 코드로 object와 array를 직접 조립해 serialize 한다.
- [x] Phase 04 - Custom Struct to JSON
  - 사용자 정의 타입을 `value_from` 기반으로 JSON으로 바꾼다.
- [x] Phase 05 - JSON to Custom Struct
  - JSON object를 `value_to<T>` 기반으로 다시 사용자 정의 타입으로 복원한다.
- [ ] Phase 06 - Validation and Error Handling
  - 잘못된 JSON, 필드 누락, 타입 mismatch를 어떻게 다룰지 확인한다.
- [ ] Phase 07 - Performance Basics
  - `stream_parser`, `monotonic_resource`, parser 재사용이 어떤 차이를 만드는지 본다.

## 왜 이 순서인가

- 먼저 JSON DOM 자체를 이해한 뒤 사용자 정의 타입 변환으로 넘어갈 수 있다.
- parse / serialize와 타입 접근을 먼저 익혀야 이후 phase의 코드가 읽기 쉬워진다.
- validation과 성능은 기본 흐름이 잡힌 뒤 붙이는 편이 학습용으로 더 명확하다.
- `config load` 같은 실전 예제는 serializer 기초를 익힌 뒤 별도 주제로 확장하기 좋다.

## 다음 실험 아이디어

- 파일에서 JSON 읽기와 파일로 JSON 저장하기
- enum을 문자열로 직렬화하기
- optional 필드와 기본값 처리하기
- config 파일 예제로 확장하기
- `RapidJSON`과 작은 성능 비교하기

## 언제 이 접근이 유용한가

- 새 JSON 라이브러리를 처음 익힐 때
- config format을 도입하기 전에 serializer 기초를 먼저 이해하고 싶을 때
- 팀에 공유할 최소 예제를 단계별로 만들고 싶을 때

## 언제 피해야 하는가

- 이미 JSON serializer를 잘 알고 있고 바로 운영용 config loader를 만들어야 할 때
- 파일 감시, hot reload, schema validation까지 한 번에 다뤄야 할 때
- 성능 측정만이 목적이라 학습용 DOM 예제가 오히려 방해가 될 때
