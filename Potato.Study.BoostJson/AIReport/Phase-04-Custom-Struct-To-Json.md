# Phase 04 - Custom Struct to JSON

## 목표

사용자 정의 struct를 `boost::json::value_from()`으로 JSON object로 바꾸는 가장 작은 예제를 만든다.

## 이번 단계에서 볼 것

- `value_from()`
- `tag_invoke`
- 사용자 정의 타입 -> JSON object 변환
- nested custom struct 변환
- `std::vector<std::string>`의 기본 변환

## 코드 포인트

- 메뉴 연결: [main.cpp](../main.cpp)
- phase 실행 함수: [Phase04CustomStructToJson.cpp](../phases/Phase04CustomStructToJson.cpp)
- custom conversion 정의: [Phase04CustomStructToJson.cpp](../phases/Phase04CustomStructToJson.cpp)

## 핵심 이해

- `value_from()`은 기본 타입뿐 아니라 사용자 정의 타입도 `tag_invoke`를 통해 `json::value`로 바꿀 수 있다.
- custom struct 하나를 JSON object로 바꾸는 규칙을 직접 정의하면, 이후 `value_from()` 호출은 일반 타입처럼 읽힌다.
- nested custom struct도 내부에서 다시 `value_from()`을 호출해 자연스럽게 이어서 변환할 수 있다.
- `std::vector<std::string>` 같은 표준 컨테이너는 별도 `tag_invoke` 없이도 array로 변환된다.

## 직접 해볼 것

- `Author`에 `email` 필드를 추가하고 JSON 결과에 같이 들어가는지 본다.
	- 단순히 필드에 추가해서만은 안되고 tag_Invoke에 값을 추가해줘야 한다.
- `LessonPlan`에 `enabled` 같은 bool 필드를 하나 더 추가해 본다.
	- 마찬가지로 bool 필드를 추가하고 tag_Invoke, paln 생성자에 결과를 업데이트 해야줘야 한다._
- `tags`를 하나 더 늘리고 serialize 결과 array가 어떻게 바뀌는지 확인한다.
	- 값이 하나 추가된다.
- field 이름을 `title` 대신 `lesson_title`처럼 다르게 매핑해 본다.
	- out of range 에러가 난다.

## 배운 점
- 커스텀 struct를 등록해야 할 경우 `tag_invoke(...)`로 등록해야 한다. 
	- 기본 변환으로 처리되지 않는 타입은 사용자 제공 `tag_invoke()`로 오버로드 변환 한다.
- `const json::value_from_tag&` 는 이름없는 매개변수다. 
- `const json::value_from_tag&` 는 Boost 문서 기준 빈 태그 타입이며 `value_from`가 tag_invoke 오버로드를 고를 때 쓰는 구분자이다.
- ADL은 Argument-Dependent Lookup이다. 한정되지 않은 함수 이름을 함수 호출식에서 찾을 때, 스코프 탐색에 더해서 인자들이 속한 네임스페이스도 같이 뒤지는 규칙이다. 
- friend를 이용하면 ADL을 찾게 할 수 있다. 

## 다음 질문

- `tag_invoke`는 왜 타입과 같은 namespace에 두는가?
	- ADL을 작동시키기 위해서이다. 
- `value_from()` 안에서 nested custom struct에 다시 `value_from()`을 호출하는 이유는 무엇인가?
	- custom struct 멤버이므로 내부에서 호출해서 값을 얻어와야 하기 때문에 그렇다.
- struct 필드 이름과 JSON key 이름을 다르게 두고 싶을 때는 어디에서 바꾸는가?
	- `tag_invoke(...)` 키 이름을 바꾸면 되겠다.

## 정리 메모

- Boost.JSON의 타입 판단은 보통 말하는 `dynamic_cast` 기반이라고 보기는 어렵다.
- `json::parse()`는 JSON text를 읽어 `boost::json::value`를 만들고, 이 `value`는 내부적으로 `null`, `bool`, `number`, `string`, `array`, `object` 중 하나의 kind를 가진다.
- 즉, 런타임 다형성과 RTTI를 이용하는 `dynamic_cast`보다는, `variant`처럼 "현재 어떤 종류의 값을 들고 있는가"를 관리하는 모델에 더 가깝다.
- `is_object()`, `is_array()`, `is_string()` 같은 함수는 현재 kind를 확인하는 API다.
- `as_object()`, `as_array()` 같은 함수는 kind가 맞으면 내부 값을 참조로 돌려주고, 아니면 예외를 던진다.
- `value_from()` / `value_to()` 쪽은 런타임 캐스팅보다 컴파일 타임 오버로드 해석에 가깝다.
- 특히 custom struct 변환은 `tag_invoke`와 type traits를 통해 결정되며, 이는 `dynamic_cast`가 아니라 "어떤 변환 함수를 선택할 것인가"를 컴파일 타임 규칙으로 정하는 흐름이다.

## 궁금한 점
- `optional` 필드는 어떤 식으로 JSON에 넣거나 생략할 수 있는가? 여기에 대해 자세한 답변을 부탁해.

## 채점 결과

- 총평: 8/10
- 잘한 점:
  - struct에 필드를 추가할 때 선언만 바꾸는 것이 아니라 `tag_invoke`도 같이 수정해야 한다는 점을 잘 짚었다.
  - `tag_invoke`를 타입과 같은 namespace에 두는 이유를 ADL이라고 설명한 부분은 정확하다.
  - struct 필드 이름과 JSON key 이름을 다르게 두고 싶을 때 `tag_invoke(...)` 안에서 바꾸면 된다는 답도 맞다.
  - `value_from_tag`를 변환 오버로드를 고르는 구분자 역할의 태그 타입으로 이해한 것도 방향이 좋다.
- 보완할 점:
  - field 이름을 `title`에서 `lesson_title`로 바꿨을 때는 JSON 생성 자체가 실패하는 것이 아니라, 출력 코드가 여전히 `root.at("title")`를 읽고 있으면 그 시점에 `std::out_of_range`가 날 수 있다고 적는 편이 더 정확하다.
  - nested custom struct에 다시 `value_from()`을 호출하는 이유는 단순히 "값을 얻기 위해서"보다, 해당 타입의 변환 규칙을 재사용해 일관되게 `json::value`로 만들기 위해서라고 정리하면 더 좋다.
  - `friend를 이용하면 ADL을 찾게 할 수 있다`는 문장은 방향은 맞지만, "타입과 연관된 namespace나 friend로 두면 ADL이 찾을 수 있다"라고 적는 편이 더 정확하다.
  - `optional` 질문 답변을 이번 phase에서 함께 정리해 두면 다음 phase와 연결하기 더 좋아진다.

### 정답 정리

- `Author`에 `email` 필드를 추가했을 때:
  - struct 선언에 필드를 추가하는 것만으로는 부족하다.
  - [Phase04CustomStructToJson.cpp](/D:/Potato.Study/Potato.Study.BoostJson/phases/Phase04CustomStructToJson.cpp) 의 `tag_invoke`에서 JSON object에 그 필드를 직접 넣어줘야 serialize 결과에도 나타난다.

- `LessonPlan`에 `enabled` 같은 bool 필드를 추가했을 때:
  - struct에 필드를 추가하고, 예제에서 만드는 `LessonPlan plan` 값도 같이 바꿔야 한다.
  - 그리고 `LessonPlan`용 `tag_invoke`에도 해당 key를 추가해야 JSON 결과에 반영된다.

- `tags`를 하나 더 늘렸을 때:
  - vector 요소가 하나 늘어나고, serialize 결과 array에도 값이 하나 더 추가된다.

- field 이름을 `title` 대신 `lesson_title`처럼 다르게 매핑했을 때:
  - `tag_invoke` 안의 key 문자열을 바꾸면 된다.

```cpp
jv = {
    { "lesson_title", plan.title },
    { "phase", plan.phase },
    { "author", json::value_from(plan.author) },
    { "tags", json::value_from(plan.tags) }
};
```

  - 다만 출력 코드가 여전히 `root.at("title")`를 읽고 있으면 그 시점에 `std::out_of_range`가 발생할 수 있다.

- `tag_invoke`는 왜 타입과 같은 namespace에 두는가?
  - ADL(Argument-Dependent Lookup)이 해당 함수 오버로드를 찾을 수 있게 하기 위해서다.
  - 즉, `value_from()`이 custom 변환 함수를 찾을 때 타입과 연관된 namespace 안의 `tag_invoke`를 발견할 수 있어야 한다.

- `value_from()` 안에서 nested custom struct에 다시 `value_from()`을 호출하는 이유는 무엇인가?
  - nested custom struct도 자기만의 변환 규칙을 가지고 있으므로, 그 규칙을 재사용해서 일관되게 `json::value`로 바꾸기 위해서다.
  - 그래서 `Author` 같은 내부 custom 타입도 다시 `json::value_from(plan.author)`처럼 호출해 주는 편이 자연스럽다.

- struct 필드 이름과 JSON key 이름을 다르게 두고 싶을 때는 어디에서 바꾸는가?
  - `tag_invoke(...)` 안에서 바꾼다.
  - struct 멤버 이름과 JSON key 이름은 반드시 같을 필요가 없다.

- `optional` 필드는 어떤 식으로 JSON에 넣거나 생략할 수 있는가?
  - Boost.JSON 공식 문서 기준으로 `value_from`와 `value_to`는 `std::optional`을 기본 지원한다.
  - 따라서 `std::optional<T>`를 그대로 `json::value_from()`에 넘길 수 있다.

1. 값을 항상 넣고, 없으면 `null`로 표현하기

```cpp
struct Author
{
    std::string name;
    std::optional<std::string> email;
};

void tag_invoke(const json::value_from_tag&, json::value& jv, const Author& author)
{
    jv = {
        { "name", author.name },
        { "email", json::value_from(author.email) }
    };
}
```

  - `email`에 값이 있으면 문자열이 들어간다.
  - 값이 없으면 JSON에서는 `null`로 표현되는 쪽으로 이해하면 된다.

2. 값이 없으면 key 자체를 생략하기

```cpp
void tag_invoke(const json::value_from_tag&, json::value& jv, const Author& author)
{
    json::object obj;
    obj["name"] = author.name;

    if (author.email)
    {
        obj["email"] = *author.email;
    }

    jv = std::move(obj);
}
```

  - 값이 있으면 `"email"` key를 추가한다.
  - 값이 없으면 key 자체를 만들지 않는다.

어느 방식이 더 좋은지는 의미에 따라 다르다.

- `null`이 좋은 경우:
  - "필드는 존재하지만 값이 비어 있다"를 드러내고 싶을 때
- key 생략이 좋은 경우:
  - "이 필드는 선택사항이고 없을 수도 있다"를 표현하고 싶을 때
