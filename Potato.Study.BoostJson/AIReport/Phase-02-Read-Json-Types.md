# Phase 02 - Read JSON Types

## 목표

`boost::json::value`에서 object, array, string, number, bool, null을 각각 어떻게 읽는지 확인한다.

## 이번 단계에서 볼 것

- `kind()`
- `as_object()`
- `as_array()`
- `as_string()`
- `as_int64()`
- `as_double()`
- `as_bool()`
- `is_null()`

## 코드 포인트

- 메뉴 연결: [main.cpp](../main.cpp)
- phase 실행 함수: [Phase02ReadJsonTypes.cpp](../phases/Phase02ReadJsonTypes.cpp)
- kind 이름 출력 헬퍼: [Phase02ReadJsonTypes.cpp](../phases/Phase02ReadJsonTypes.cpp)

## 핵심 이해

- `boost::json::value`는 JSON의 모든 타입을 담을 수 있는 공통 타입이다.
- 실제 값을 읽기 전에는 먼저 어떤 타입인지 확인하는 흐름이 중요하다.
- object와 array는 컨테이너처럼 다시 내부 값을 따라 들어가며 읽는다.
- number는 하나처럼 보여도 `Boost.JSON` 내부에서는 `int64`, `uint64`, `double`로 나뉠 수 있다.

## 직접 해볼 것

- `ratio`를 `1`로 바꾸고 `as_double()` 대신 어떤 접근이 더 자연스러운지 비교해 본다.
	- 에러가 발생한다. as_int64 접근이 자연스럽고 double로 접근시 1.0 같은 접근이 나아보인다._
- `nickname`을 `"potato"`로 바꾸고 `is_null()` 결과가 어떻게 달라지는지 확인한다.
	- is_null의 결과값이 false를 리턴하도록 된다.
- `tags` 배열에 값을 하나 더 추가하고 출력이 어떻게 늘어나는지 본다.
	- 배열의 값이 늘어났고, 반복문이 한번 더 반복하여 출력 되었다.
- `enabled`를 문자열 `"true"`로 바꾼 뒤 `as_bool()`를 호출하면 어떤 일이 생기는지 확인한다.
	- 자료형이 맞지 않아 에러가 발생 되었다. 

## 배운 점

- object는 `as_object()`, array는 `as_array()`로 읽은 뒤 내부 값을 다시 `value`로 다루게 된다.
- null은 값으로 꺼내기보다 `is_null()`로 확인하는 쪽이 더 직접적이다.
- 숫자는 항상 같은 종류가 아니므로, `int64`와 `double`을 구분해서 읽는 습관이 필요하다.

## 다음 질문

- `kind()`와 `is_object()` / `is_array()` 중 언제 어느 쪽이 더 읽기 쉬운가?
	- 읽기는 is_object or is_array가 더 편하다. 하지만, 명확한 타입을 명시해야 하는경우엔 kind()가 좋아보인다.

- 없는 key를 `at()`으로 읽으면 어떤 예외가 발생하는가
	- out of range 에러가 발생한다.

- 숫자를 안전하게 읽으려면 `int64`, `uint64`, `double`을 어떤 순서로 확인하는 것이 좋은가?
	  1) 문자열에 '.' 또는 'e'/'E'가 있으면 -> double 시도
	  2) 아니면 정수로 간주
		- '-'로 시작하면 -> int64 시도
		- 그 외에는 -> int64 시도, 실패하면 uint64 시도
	  3) 그래도 안 되면 -> overflow / parse error 
- object 전체를 순회하려면 어떤 식으로 member를 읽는가?
	-object를 이터레이터를 통해 순회하고 value의 kind를 사용하여 값을 판단한 다음 결과를 표현한다.

## 채점 결과

- 총평: 8/10
- 잘한 점:
  - `nickname`을 문자열로 바꿨을 때 `is_null()` 결과가 `false`가 된다고 정리한 점은 정확하다.
  - `tags` 배열 요소를 추가하면 반복문 출력이 한 번 더 늘어난다고 관찰한 점도 맞다.
  - `enabled`를 문자열 `"true"`로 바꾼 뒤 `as_bool()`를 호출하면 타입 mismatch 예외가 난다고 이해한 점도 좋다.
  - `kind()`와 `is_object()` / `is_array()`를 "명시성"과 "가독성" 관점에서 나눠 본 점도 방향이 좋다.
- 보완할 점:
  - `ratio`를 `1`로 바꿨을 때는 단순히 에러라고만 적기보다, 현재 예제에서는 정수로 파싱되므로 `as_int64()`가 더 자연스럽다고 적는 편이 더 정확하다.
  - 없는 key를 `at()`으로 읽을 때는 `out of range`보다 `std::out_of_range` 예외라고 적는 편이 좋다.
  - 숫자를 안전하게 읽는 순서는 "원본 문자열을 다시 본다"보다, 이미 파싱된 `boost::json::value`에서 `is_int64()`, `is_uint64()`, `is_double()` 또는 `kind()`를 확인하는 방향이 이 phase에 더 잘 맞는다.
  - object 순회 답변은 방향은 맞지만, `for (auto const& member : obj)` 같은 짧은 예시가 하나 있으면 더 읽기 쉽다.

### 정답 정리

- `ratio`를 `1`로 바꿨을 때:
  - 현재 예제에서는 정수로 파싱되므로 `as_double()`보다 `as_int64()`가 더 자연스럽다.
  - 다만 필드 의미를 `ratio`처럼 실수로 유지하고 싶다면 입력 자체를 `1.0`처럼 두는 편이 더 일관적이다.

- `nickname`을 `"potato"`로 바꿨을 때:
  - `is_null()` 결과는 `false`가 된다.

- `tags` 배열에 값을 하나 더 추가했을 때:
  - 배열 크기가 늘어나고, 반복문도 한 번 더 돌면서 새 요소가 출력된다.

- `enabled`를 문자열 `"true"`로 바꾼 뒤 `as_bool()`를 호출했을 때:
  - 문자열과 bool은 다른 타입이므로 값 접근 단계에서 타입 mismatch 예외가 발생한다.

- `kind()`와 `is_object()` / `is_array()` 중 언제 어느 쪽이 더 읽기 쉬운가?
  - 단순 분기나 가독성만 보면 `is_object()`, `is_array()`가 더 직관적이다.
  - 여러 타입을 한 번에 구분하거나 로그/디버깅처럼 타입 이름을 명시적으로 다루고 싶을 때는 `kind()`가 더 잘 맞는다.

- 없는 key를 `at()`으로 읽으면 어떤 예외가 발생하는가?
  - `std::out_of_range` 예외가 발생한다.

- 숫자를 안전하게 읽으려면 어떤 순서로 확인하는 것이 좋은가?
  - 이 phase처럼 이미 `boost::json::value`로 파싱된 뒤라면, 원본 문자열을 다시 보기보다 `is_int64()`, `is_uint64()`, `is_double()` 순으로 확인하는 쪽이 자연스럽다.
  - 즉, 지금 단계에서는 "문자열 모양"보다 "파싱된 DOM 타입"을 기준으로 판단하는 편이 좋다.

- object 전체를 순회하려면 어떤 식으로 member를 읽는가?

```cpp
const json::object& obj = root.at("profile").as_object();

for (const auto& member : obj)
{
    std::cout << member.key() << " -> "
              << DescribeKind(member.value()) << '\n';
}
```

각 member는 key와 value를 함께 가지므로, `member.key()`와 `member.value()`를 읽으면서 순회하면 된다.
