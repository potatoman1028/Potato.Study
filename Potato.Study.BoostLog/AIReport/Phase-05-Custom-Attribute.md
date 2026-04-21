# Phase 05. Custom Attribute

## 목표

요청 ID 같은 custom attribute를 로그 record에 붙이고 formatter에서 읽는 구조를 이해한다.

## 이번 단계에서 볼 것

- custom attribute 추가
- scoped thread attribute
- attribute가 없을 때 fallback 값 사용

## 코드 포인트

- phase 실행 함수: [Phase05CustomAttribute.cpp](../phases/Phase05CustomAttribute.cpp)
- scoped custom attribute: [Phase05CustomAttribute.cpp](../phases/Phase05CustomAttribute.cpp)
- formatter의 fallback 값: [Phase05CustomAttribute.cpp](../phases/Phase05CustomAttribute.cpp)

## 핵심 이해

- custom attribute도 기본 attribute와 같은 방식으로 formatter에서 읽을 수 있다.
- scoped thread attribute를 쓰면 현재 스코프 안의 로그에만 값을 붙일 수 있다.
- formatter에서 fallback 값을 주면 attribute가 없을 때도 출력 형식을 유지할 수 있다.

## 직접 해볼 것

- `REQ-1001` 값을 다른 문자열로 바꿔 본다.
- scoped block 밖에서도 `RequestID`가 유지되도록 global attribute로 바꿔 본다.
- `or_default("none")` 대신 다른 기본 문자열을 넣어 본다.

## 배운 점
global custom attribute를 등록 할 수 있다. 
scoped thread attribute를 등록 하여 해당 스코프만 특수한 값을 적용 할 수 있다. 

## 관찰 메모

- scoped block 안의 로그만 `RequestID=REQ-1001`로 보여야 한다.
- scoped block 밖의 로그는 `none`으로 보여야 한다.

## 다음 질문

- global custom attribute와 scoped thread attribute는 언제 각각 쓰는가?
global custom attribute는 로그 전체에 해당하는 default 값에 적용한다. 
scoped thread attribute는 해당 스코프 내에만 적용할 작업을 사용한다. 

## 궁금한 점
BOOST_LOG_SCOPED_THREAD_ATTR에 대한 자세한 설명과 다른 매크로도 있으면 알려줘. 
또한 어떤 경우에 사용하는지도 알려줘.

`BOOST_LOG_SCOPED_THREAD_ATTR(attr_name, attr)`는 현재 scope가 시작될 때 thread-specific attribute를 등록하고, scope가 끝날 때 자동으로 제거하는 매크로다.

즉, 현재 예제에서는 아래 코드 블록 안에서만 `RequestID`가 붙는다.

```cpp
{
    BOOST_LOG_SCOPED_THREAD_ATTR(
        "RequestID",
        attrs::constant<std::string>("REQ-1001"));

    BOOST_LOG_SEV(logger, Severity::Info) << "Request context is active";
}
```

이 매크로를 쓰면 아래 특징이 있다.

- 현재 thread에만 적용된다.
- 현재 scope 안에서만 유지된다.
- logger 객체를 직접 수정하지 않아도 된다.
- scope를 빠져나오면 자동으로 제거된다.

그래서 아래 같은 경우에 특히 유용하다.

- 요청 처리 함수 안에서 여러 로그에 같은 `RequestID`를 붙이고 싶을 때
- 작업 단위 하나가 끝나면 attribute도 같이 사라지게 하고 싶을 때
- logger 자체 설정은 건드리지 않고 현재 실행 문맥만 잠깐 표시하고 싶을 때

비슷한 방법으로는 아래가 있다.

- `BOOST_LOG_SCOPED_LOGGER_ATTR(logger, attr_name, attr)`
  - 특정 logger 객체에 scoped attribute를 붙인다.
  - thread 전체보다 "이 logger를 쓰는 범위"에 더 초점을 둘 때 쓴다.
- `boost::log::add_value("RequestID", "REQ-1001")`
  - 한 번의 로그 record에만 값을 붙일 때 쓴다.
  - 여러 줄 로그에 같은 값을 반복해서 붙일 필요가 없고, 딱 한 줄만 꾸미고 싶을 때 적합하다.
- `core->add_global_attribute(...)`
  - 애플리케이션 전체 로그에 공통으로 붙일 정보를 등록할 때 쓴다.
  - 예를 들면 `Tag`, `ProcessID`, 공통 환경 이름 같은 값이 더 잘 맞는다.
- `core->add_thread_attribute(...)`
  - 현재 thread 전체에 attribute를 붙이고, 필요할 때 직접 제거하는 방식이다.
  - scope 자동 해제보다 명시적인 등록/제거 제어가 필요할 때 쓴다.

정리하면 아래처럼 보면 된다.

- 한 줄 로그에만 잠깐 붙이기: `add_value`
- 현재 scope와 thread 안의 여러 로그에 붙이기: `BOOST_LOG_SCOPED_THREAD_ATTR`
- 특정 logger 범위에 붙이기: `BOOST_LOG_SCOPED_LOGGER_ATTR`
- 프로그램 전체 공통 정보로 쓰기: `add_global_attribute`

주의할 점도 하나 있다.

- 같은 이름의 attribute가 이미 등록되어 있으면 scoped 매크로가 기대한 대로 새 값을 덮어쓰지 못할 수 있다.
- 그래서 이름 충돌이 없는지 보는 습관이 중요하다.

## 채점 결과

- 총평: 8/10
- 잘한 점:
  - `scoped thread attribute`가 특정 범위에서만 특별한 값을 적용한다는 점을 잘 잡았다.
  - `global custom attribute`와 `scoped thread attribute`를 쓰는 상황을 나누려는 방향이 좋다.
- 보완할 점:
  - `global custom attribute를 등록 할 수 있다`는 말 자체는 맞지만, 이번 `Phase 05` 코드가 직접 보여주는 것은 global custom attribute보다 `scoped thread attribute` 쪽이다. 그래서 배운 점에는 `이번 예제에서는 scoped thread attribute로 RequestID를 붙였다`고 쓰는 편이 더 정확하다.
  - `scoped thread attribute`는 단순히 "해당 스코프"뿐 아니라 현재 thread에도 묶여 있다는 점을 함께 적어 두면 더 좋다.
  - `global custom attribute는 로그 전체에 해당하는 default 값`이라는 설명은 방향은 맞지만 조금 좁다. 실제로는 애플리케이션 전체 로그에 공통으로 붙일 정보나 모든 logger에서 공통으로 볼 메타데이터에 더 가깝다고 적으면 더 정확하다.
