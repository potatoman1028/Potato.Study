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
	- `author.active`가 `false`가 된다.
- `tags` 배열에 값을 하나 더 추가하고 `tags size` 출력이 어떻게 바뀌는지 본다.
	-  `tag size`가 1 늘어났다.
- `phase`를 문자열로 바꾸고 어느 지점에서 에러가 나는지 확인한다.
	- plan을 `value_to`하는 중에 `value is not a std::int64_t`가 발생한다.
- `title` key를 `lesson_title`로 바꾸고 현재 복원 코드가 어떤 예외를 내는지 본다.
	- `out of range`가 발생한다.

## 배운 점
 - 커스텀 struct를 변환하는 함수를 등록해야 할 경우에도 `tag_invoke`를 사용한다 이 때 사용되는 태그는 `value_to_tag`이다.

## 다음 질문

- `value_to<T>()`는 내부에서 어떤 순서로 변환 함수를 찾는가?
	- `value_to_impl` 내에서 ADL을 이용해 `tag_invoke`를 찾아 실행한다.
- `tag_invoke(value_to_tag<T>, ...)`는 왜 값을 반환하는 형태인가?
	- `json::value`를 원래 자료형으로 복원하는 기능이기 때문이다.
- 없는 key를 `at()`으로 읽을 때와 타입이 다른 값을 읽을 때 에러는 어떻게 구분되는가?
	- value_to를 호출해서 타입이 다른 값을 읽을 때는 out of range가 발생한다. 잘못된 메모리 영역에 접근했기 때문이다.
	- 없는 key를 `at()`으로 호출할 때도 마찬가지로 out of range가 발생한다.
- 복원 과정에서 기본값을 주고 싶다면 `tag_invoke` 안에서 어떤 식으로 처리하는가?
	- if문을 통해 값이 없으면 기본값을 추가하고 적용하면 된다.

## 채점 결과

- 총평: 7/10
- 잘한 점:
  - `author.active`를 `false`로 바꾸면 복원된 struct 값도 `false`가 된다고 정리한 점은 맞다.
  - `tags` 배열에 값을 하나 더 넣으면 `tags size`가 1 늘어난다는 답도 맞다.
  - `tag_invoke`에서 `value_to_tag`를 사용한다는 배운 점은 정확하다.
  - 복원 과정에서 기본값을 주고 싶을 때 `if`로 key 존재 여부를 확인한다는 방향도 좋다.
- 보완할 점:
  - `phase`를 문자열로 바꿨을 때는 단순히 `value_to` 전체에서 에러가 난다기보다, `LessonPlan`용 `tag_invoke` 내부의 `obj.at("phase").as_int64()` 지점에서 타입 mismatch가 발생한다고 보는 편이 더 정확하다.
  - 없는 key를 `at()`으로 읽으면 `std::out_of_range`가 발생한다.
  - 타입이 다른 값을 읽을 때는 `out_of_range`가 아니라 타입 mismatch 예외가 발생한다.
  - "잘못된 메모리 영역에 접근했기 때문"이라는 설명은 틀리다. `at()`과 `as_*()`는 안전하게 검사하고 예외를 던지는 API다.
  - `value_to<T>()`는 custom struct에서는 ADL로 사용자 정의 `tag_invoke`를 찾는 것이 핵심이지만, 전체 변환 시스템은 기본 변환과 사용자 정의 변환 규칙을 함께 사용한다고 이해하는 편이 좋다.

### 정답 정리

- `author.active`를 `false`로 바꿨을 때:
  - JSON object의 `author.active` 값이 false가 되면, 복원된 `plan.author.active`도 `false`가 된다.

- `tags` 배열에 값을 하나 더 추가했을 때:
  - `std::vector<std::string>`으로 복원되는 요소 수가 하나 늘어난다.
  - 따라서 `tags size` 출력도 1 증가한다.

- `phase`를 문자열로 바꿨을 때:
  - JSON parse 자체는 성공할 수 있다.
  - `json::value_to<LessonPlan>(parsed)`가 `LessonPlan`용 `tag_invoke`를 호출한다.
  - 그 안에서 `obj.at("phase").as_int64()`를 호출할 때, 실제 값이 string이면 타입 mismatch 예외가 발생한다.

```cpp
static_cast<int>(obj.at("phase").as_int64())
```

- `title` key를 `lesson_title`로 바꿨을 때:
  - 현재 복원 코드는 여전히 `obj.at("title")`을 읽는다.
  - 따라서 `"title"` key가 없으면 `std::out_of_range` 예외가 발생한다.

```cpp
json::value_to<std::string>(obj.at("title"))
```

- `value_to<T>()`는 내부에서 어떤 순서로 변환 함수를 찾는가?
  - 기본 타입과 표준 컨테이너처럼 Boost.JSON이 지원하는 변환은 라이브러리의 기본 변환 규칙을 사용한다.
  - 사용자 정의 타입은 타입과 연관된 namespace에서 ADL로 `tag_invoke(value_to_tag<T>, ...)`를 찾는 흐름이 핵심이다.

- `tag_invoke(value_to_tag<T>, ...)`는 왜 값을 반환하는 형태인가?
  - `value_to<T>()`는 JSON value에서 C++ 객체 `T`를 새로 만들어 돌려주는 방향이기 때문이다.
  - 반대로 `value_from()`은 이미 받은 `json::value&`에 값을 채워 넣는 방향이라 `void tag_invoke(...)` 형태를 쓴다.

- 없는 key를 `at()`으로 읽을 때와 타입이 다른 값을 읽을 때 에러는 어떻게 구분되는가?
  - 없는 key를 `at()`으로 읽으면 `std::out_of_range`가 발생한다.
  - key는 있지만 타입이 다르면 `as_int64()`, `as_bool()`, `value_to<T>()` 같은 값 접근 또는 변환 단계에서 타입 mismatch 예외가 발생한다.

```cpp
obj.at("lesson_title");      // key가 없으면 std::out_of_range
obj.at("phase").as_int64();  // phase가 string이면 타입 mismatch
```

- 복원 과정에서 기본값을 주고 싶다면 `tag_invoke` 안에서 어떤 식으로 처리하는가?
  - `find()`로 key 존재 여부를 확인하고, 없으면 기본값을 넣어 struct를 만든다.

```cpp
bool active = true;

if (auto it = obj.find("active"); it != obj.end())
{
    active = it->value().as_bool();
}
```

이 방식은 missing key를 예외로 볼지, 기본값으로 보정할지 `tag_invoke` 안에서 직접 결정할 수 있게 해준다.
