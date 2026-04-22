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
	- 출력 값이 false로 변한다. 
- `features` 배열에 값을 하나 더 추가해 본다.
	- `"test"`를 추가하면 마지막 serialize 결과 JSON에도 함께 포함된다.
- `phase`를 문자열로 바꾸고 어떤 차이가 생기는지 확인한다.
	- parse 자체는 성공할 수 있다.
	- 다만 현재 코드는 `phase`를 숫자로 기대하고 `as_int64()`를 호출하므로, 값 접근 단계에서 타입 mismatch 예외가 발생한다.

## 배운 점

- `Boost.JSON`은 문자열을 곧바로 DOM으로 파싱할 수 있다.
- `boost::json::value` 하나로 object/array/string/number/bool/null을 담을 수 있다.
- JSON serializer 학습의 첫 단계는 "파일"보다 "문자열과 DOM의 왕복"을 먼저 보는 편이 이해가 쉽다.

## 다음 질문

- 배열 요소는 어떤 식으로 읽는가?
문자열 배열이라면 아래처럼 읽는 편이 가장 직관적이다.

```cpp
const json::array& features = root.at("features").as_array();

for (const json::value& item : features)
{
    std::cout << item.as_string() << '\n';
}
```

배열은 `as_array()`로 꺼내고, 각 요소는 다시 `json::value`로 받아 필요한 타입의 accessor를 호출한다.


- `null`과 `bool`은 어떤 API로 구분하는가?
타입 검사는 `is_null()`과 `is_bool()`로 하고, bool 값 접근은 `as_bool()`로 한다.

- parse 실패는 어떻게 처리하는가?
가장 단순하게는 예외 처리로 확인할 수 있다.

```cpp
try
{
    json::value value = json::parse(text);
}
catch (const std::exception& ex)
{
    std::cout << ex.what() << '\n';
}
```

예외 대신 `error_code` 기반으로 처리할 수도 있다.

또 정리해 두면, 문자열 형식이 잘못되면 `parse` 단계에서 실패하고, parse는 성공했지만 기대 타입이 다르면 이후 `as_int64()`, `as_bool()` 같은 값 접근 단계에서 실패한다.


## 궁금한 점
- DOM이 뭔지 궁금함
DOM은 Document Object Model의 줄임말이다.
JSON 문자열을 메모리 안의 트리 구조로 올려 둔 표현이라고 생각하면 된다.
이 예제에서는 `json::parse(source)`가 만든 `boost::json::value`가 DOM 전체이고, 그 안에서 object, array, string, number, bool, null을 따라가며 읽는다.

## 채점 결과

- 총평: 8/10
- 잘한 점:
  - `Boost.JSON`이 문자열을 DOM으로 파싱하고 다시 serialize 한다는 핵심 흐름을 잘 잡았다.
  - `boost::json::value`가 여러 JSON 타입을 담는 공통 컨테이너라는 점을 잘 이해했다.
  - 파일 입출력보다 문자열과 DOM 왕복부터 보는 학습 순서도 이 프로젝트 목적과 잘 맞는다.
- 보완할 점:
  - `enabled`를 `false`로 바꿔 본다는 메모는 현재 예제 원본도 이미 `false`라서, 학습 기록으로는 `true`와 `false`를 비교해 적는 편이 더 좋다.
  - `phase`를 문자열로 바꿨을 때를 "parse 실패"라고 적은 부분은 `parse 성공 후 타입 mismatch`로 구분해서 적는 편이 더 정확하다.
  - `null`과 `bool`을 구분할 때는 `is_null()`, `is_bool()`를 쓰고, `as_bool()`는 값 접근용이라고 구분해 두는 편이 좋다.
  - 배열 읽기 예시는 `std::cout << value`까지 같이 쓰기보다, 학습용으로는 `as_array()`와 `as_string()` 흐름만 남기는 쪽이 더 읽기 쉽다.

### 정답 정리

- `enabled` 값을 바꿨을 때:
  - 현재 예제 원본은 이미 `false`다.
  - 학습 메모에는 `true`로 바꿨을 때와 다시 `false`로 바꿨을 때 출력이 어떻게 달라지는지 비교해서 적는 편이 더 좋다.

- `features` 배열에 `"test"`를 추가했을 때:
  - 현재 코드는 배열 요소를 한 줄씩 출력하지 않는다.
  - 대신 마지막 `Serialized JSON text` 출력에 `"test"`가 포함된 JSON 문자열이 보이게 된다.

- `phase`를 문자열로 바꿨을 때:
  - `json::parse`는 정상 동작할 수 있다.
  - 다만 현재 코드는 `phase`를 숫자로 기대하고 `as_int64()`를 호출하므로, 값 접근 단계에서 타입 mismatch 예외가 발생한다.

- 배열 요소는 어떤 식으로 읽는가?

```cpp
const json::array& features = root.at("features").as_array();

for (const json::value& item : features)
{
    std::cout << item.as_string() << '\n';
}
```

배열은 `as_array()`로 바꾸고, 각 요소는 `json::value`로 순회한 뒤 필요한 타입의 accessor를 호출한다.

- `null`과 `bool`은 어떤 API로 구분하는가?
  - 타입 검사는 `is_null()`, `is_bool()`로 한다.
  - 값 접근은 `as_bool()`로 한다.

- parse 실패는 어떻게 처리하는가?
  - 가장 단순한 방법은 예외 처리다.

```cpp
try
{
    json::value value = json::parse(text);
}
catch (const std::exception& ex)
{
    std::cout << ex.what() << '\n';
}
```

예외 대신 `error_code` 기반으로 처리할 수도 있다.
또, 문자열 형식이 잘못되면 `parse` 단계에서 실패하고, parse는 성공했지만 기대 타입이 다르면 이후 값 접근 단계에서 실패한다.

- DOM이 뭔지?
  - DOM은 Document Object Model의 줄임말이다.
  - JSON 문자열을 메모리 안의 트리 구조로 올려 둔 표현이라고 생각하면 된다.
  - 이 예제에서는 `json::parse(source)`가 만든 `boost::json::value`가 DOM 전체이고, 그 안에서 object, array, string, number, bool, null을 따라가며 읽는다.
