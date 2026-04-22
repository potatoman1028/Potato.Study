# Phase 03 - Build JSON Programmatically

## 목표

문자열을 parse하지 않고, C++ 코드에서 `boost::json::object`와 `boost::json::array`를 직접 조립해 JSON DOM을 만든다.

## 이번 단계에서 볼 것

- `json::object`
- `json::array`
- `operator[]`
- `push_back()`
- 중첩 object / array 조립
- `json::serialize()`

## 코드 포인트

- 메뉴 연결: [main.cpp](../main.cpp)
- phase 실행 함수: [Phase03BuildJsonProgrammatically.cpp](../phases/Phase03BuildJsonProgrammatically.cpp)
- root object 조립: [Phase03BuildJsonProgrammatically.cpp](../phases/Phase03BuildJsonProgrammatically.cpp)

## 핵심 이해

- JSON은 항상 문자열에서만 시작하는 것이 아니라, C++ 코드에서 DOM을 직접 만들 수도 있다.
- object는 `operator[]`로 member를 추가하고, array는 `push_back()`으로 요소를 추가한다.
- nested object와 nested array를 먼저 만든 뒤 root object에 넣는 흐름이 읽기 쉽다.
- 마지막에는 `json::serialize()`로 DOM을 JSON 문자열로 바꿔 확인할 수 있다.

## 직접 해볼 것

- `profile` object에 `nickname` 필드를 하나 더 추가해 본다.
	- profile에 nickname 필드가 추가된다. 
- `features` 배열에 문자열을 하나 더 넣고 출력 크기가 어떻게 바뀌는지 확인한다.
	- features size = 4가 된다.
- `checkpoints` 배열에 object를 하나 더 추가해 본다.
	- Serialized JSON text 결과물에 해당 오브젝트가 출력된다.
- `enabled`를 `false`로 바꾸고 serialize 결과가 어떻게 달라지는지 본다.
	- Serialized JSON text 결과물에 enabled 필드가 false로 출력된다.

## 배운 점

## 다음 질문

- `operator[]`로 값을 넣는 방식과 `emplace_*` 계열 API는 언제 어느 쪽이 더 자연스러운가?
	-  object 내에 emplace 함수를 보면 값을 리턴하고있다. 리턴 value를 통해 검증을 하기 위해서라면 
	emplace를 선택하는 것도 좋아보인다. opeartor[]을 사용하는 경우엔 가독성이 좋아져 확실한 경우에 사용하는게 좋아보인다. 
		
- `null` 값을 코드에서 직접 넣으려면 어떤 식으로 표현하는가?
	- object['key'] = null
	
- array 안에 object를 여러 개 넣을 때 move를 쓰는 이유는 무엇인가?
	- 임시객체(temporary) 를 바로 넣는 경우는 이미 rvalue라서, 보통 직접 std::move를 쓸 필요가 없습니다.
    반면 이름이 있는 지역 변수 obj를 넣을 때는 lvalue라서, 이동 의도를 명시하려면 std::move(obj)가 필요합니다.
- serialize 결과를 사람이 읽기 좋게 pretty format으로 만들려면 어떻게 해야 하는가?
	- 표기법을 명확히 할 필요가 있다. string 라인 하나로 진행하지 말고 각 각 필드 이후엔 들여쓰기나 개행을 통해서 
	  보기 좋은 텍스트를 만들 필요가 있어보인다. 

## 궁금한 점
위에 질문이 나온만큼 lvaue , rvalue, xvalue? 를 한번 정리 해줬으면 좋겠다. 

## 채점 결과

- 총평: 7.5/10
- 잘한 점:
  - `nickname`, `features`, `checkpoints`, `enabled`를 바꿨을 때 serialize 결과나 출력 크기가 어떻게 달라지는지 잘 관찰했다.
  - `std::move`를 쓰는 이유를 "이름 있는 지역 변수는 lvalue이기 때문"이라고 설명한 부분이 특히 좋다.
  - `operator[]`와 `emplace`를 가독성과 반환값 관점에서 나눠 본 시도도 방향이 괜찮다.
- 보완할 점:
  - `null` 값을 넣는 예시는 `null`이 아니라 `nullptr`를 써야 정확하다.
  - `operator[]`와 `emplace_*`의 차이는 "반환값"뿐 아니라, "덮어쓰기", "없을 때만 삽입", "value 자체의 종류 변경"을 구분해서 보는 편이 더 좋다.
  - pretty format 답은 방향은 맞지만, `json::serialize()`는 compact output이고 pretty 출력은 보통 별도 helper 함수로 만든다고 적으면 더 정확하다.
  - `배운 점`은 앞에서 정한 규칙대로 `skip` 대신 비워두는 편이 더 깔끔하다.

### 정답 정리

- `profile` object에 `nickname` 필드를 추가했을 때:
  - `profile["nickname"] = "king";`처럼 넣을 수 있다.
  - serialize 결과의 `profile` object 안에 `nickname` 필드가 추가된다.

- `features` 배열에 문자열을 하나 더 넣었을 때:
  - `push_back()`를 한 번 더 호출하면 된다.
  - 현재 수정 기준으로는 `features size = 4`가 된다.

- `checkpoints` 배열에 object를 하나 더 추가했을 때:
  - 새 object를 만든 뒤 `push_back()`하면 serialize 결과 배열에 object가 하나 더 들어간다.

- `enabled`를 `false`로 바꿨을 때:
  - serialize 결과 JSON에서 `enabled` 값이 `false`로 출력된다.

- `operator[]`와 `emplace_*`는 언제 어느 쪽이 더 자연스러운가?
  - `operator[]`는 값을 단순하게 넣거나 덮어쓸 때 가장 읽기 쉽다.

```cpp
root["enabled"] = false;
root["phase"] = 3;
```

  - `object::emplace(key, value)`는 "이 key가 없을 때만 넣고 싶다"거나, 삽입 성공 여부를 반환값으로 확인하고 싶을 때 더 자연스럽다.
  - `value::emplace_object()`, `value::emplace_array()`, `value::emplace_null()` 같은 API는 `json::value` 자체의 종류를 그 자리에서 바꾸고 싶을 때 더 잘 맞는다.

- `null` 값을 코드에서 직접 넣으려면 어떤 식으로 표현하는가?
  - `nullptr`를 사용한다.

```cpp
json::object obj;
obj["key"] = nullptr;
```

- array 안에 object를 여러 개 넣을 때 move를 쓰는 이유는 무엇인가?
  - 이름 있는 변수 `obj`는 lvalue이므로, 이동 의도를 명시하려면 `std::move(obj)`가 필요하다.
  - 이렇게 하면 복사보다 이동 경로를 타기 쉬워져 불필요한 복사를 줄일 수 있다.
  - 반대로 `json::object{ ... }` 같은 임시객체는 원래 rvalue라서 보통 별도 `std::move`가 필요 없다.

- serialize 결과를 사람이 읽기 좋게 pretty format으로 만들려면 어떻게 해야 하는가?
  - `json::serialize()`는 compact JSON을 만든다.
  - 사람이 읽기 좋은 pretty 출력은 보통 별도 `pretty_print` helper 함수를 만들어 object와 array를 재귀적으로 들여쓰기하는 방식으로 구현한다.

- lvalue / rvalue / xvalue 정리
  - `lvalue`: 이름이 있고 계속 참조할 수 있는 값. 예: `profile`, `features`, `firstCheckpoint`
  - `rvalue`: 곧 사라질 값 전체를 넓게 부르는 말
  - `prvalue`: 순수 임시값. 예: `42`, `"text"`, `json::object{}`
  - `xvalue`: 이동 가능한 expiring value. 예: `std::move(firstCheckpoint)`

실전에서는 이렇게 기억하면 된다.

- 그냥 변수명은 보통 `lvalue`
- `std::move(변수)` 결과는 `xvalue`
- 임시로 만든 값은 `prvalue`
- `prvalue`와 `xvalue`를 묶어서 보통 `rvalue`라고 부른다.
