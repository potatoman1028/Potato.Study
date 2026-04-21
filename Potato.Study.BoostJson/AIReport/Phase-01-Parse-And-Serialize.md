# Phase 01 - Parse and Serialize

## 목표

JSON 문자열을 `boost::json::value`로 파싱하고, 다시 JSON 문자열로 serialize 하는 가장 작은 예제를 만든다.

## 이번 단계에서 볼 것

- `boost::json::parse`
- `boost::json::serialize`
- `boost::json::value`
- `object` 접근 (`as_object`, `at`)

## 코드 포인트

- `main.cpp`
  - Phase 01 메뉴를 선택해 실행한다.
- `phases/Phase01ParseSerialize.cpp`
  - JSON text -> `boost::json::value` -> JSON text 흐름을 보여준다.
- `Potato.Study.BoostJson.vcxproj`
  - Boost.JSON 헤더와 라이브러리를 현재 빌드 환경에서 찾도록 설정한다.

## 핵심 이해

- `parse`는 JSON 문자열을 DOM 값(`boost::json::value`)으로 바꾼다.
- `serialize`는 DOM 값을 다시 JSON 문자열로 바꾼다.
- 이번 phase는 파일 입출력 없이 메모리 안에서 JSON round-trip을 확인하는 최소 예제다.

## 직접 해볼 것

- `enabled` 값을 `false`로 바꿔 본다.
- `features` 배열에 값을 하나 더 추가해 본다.
- `phase`를 문자열로 바꾸고 어떤 차이가 생기는지 확인한다.

## 배운 점

- `Boost.JSON`은 문자열을 곧바로 DOM으로 파싱할 수 있다.
- `boost::json::value` 하나로 object/array/string/number/bool/null을 담을 수 있다.
- JSON serializer 학습의 첫 단계는 "파일"보다 "문자열과 DOM의 왕복"을 먼저 보는 편이 이해가 쉽다.

## 다음 질문

- 배열 요소는 어떤 식으로 읽는가?
- `null`과 `bool`은 어떤 API로 구분하는가?
- parse 실패는 어떻게 처리하는가?