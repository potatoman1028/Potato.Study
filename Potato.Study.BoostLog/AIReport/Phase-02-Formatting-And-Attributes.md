# Phase 02. Formatting And Attributes

## 목표

로그 포맷 문자열과 공통 attribute가 어떤 역할을 하는지 이해한다.

## 이번 단계에서 볼 것

- `add_common_attributes()`의 역할
- `LineID`, `TimeStamp`, `ThreadID`가 어디에서 오는지
- 포맷 표현식이 로그 문자열을 어떻게 만드는지

## 코드 포인트

- 공통 attribute 등록: [Phase02Formatting.cpp](../phases/Phase02Formatting.cpp)
- 포맷 정의 시작: [Phase02Formatting.cpp](../phases/Phase02Formatting.cpp)
- 날짜/시간 지원 헤더: [Phase02Formatting.cpp](../phases/Phase02Formatting.cpp)

## 핵심 이해

- `add_common_attributes()`가 없으면 포맷에서 참조한 기본 attribute가 비거나 실패할 수 있다.
- `boost::posix_time::ptime`를 로그 포맷에 쓰려면 날짜/시간 지원 헤더가 필요하다.
- 포맷은 문자열 템플릿이 아니라 expression 조합이다.

## 직접 해볼 것

- `ThreadID` 출력을 잠시 제거하고 결과를 비교한다.
- 로그 앞의 `[#LineID]` 구간을 다른 순서로 바꿔 본다.
- `Severity`를 메시지 뒤로 옮겨 가독성을 비교한다.

## 배운 점

- attribute를 설정하고, 출력 format를 만들어 sink를 코어에 등록하여 로그를 출력하는 예제이다. 
- add_common_attributes() 함수는 LineID, TimeStamp, ThreadID의 attribute를 등록하게 해준다. 
- Severity는 직접 등록한 attribute 이다. 
- add_console_log는 console sink에 cout을 출력 스트림으로 설정하고, format 형식을 따라 출력하게 지정한다. 
- logger는 심각도 기반 record를 등록할 수 있게 해준다. 
## 관찰 메모

- 같은 프로세스에서 phase를 연속 실행하면 `LineID`는 이어질 수 있다.
- 이는 `add_common_attributes()`가 process-wide `core`에 공통 attribute를 등록하는 구조와 연결해서 이해하는 편이 좋다.

## 다음 질문

- custom attribute를 추가하면 같은 방식으로 포맷에 넣을 수 있는가?
할 수 있습니다.

## 채점 결과

- 총평: 7/10
- 잘한 점:
  - `add_common_attributes()`, `add_console_log`, `logger`처럼 Phase 02에서 중요한 구성요소를 직접 짚었다.
  - 이 phase가 attribute를 준비하고 formatter를 만들어 sink에 연결하는 흐름이라는 점을 잘 잡았다.
  - custom attribute도 같은 방식으로 formatter에 넣을 수 있다는 방향은 맞다.
- 보완할 점:
  - `Severity`는 `add_common_attributes()`처럼 직접 등록한 공통 attribute가 아니라, `severity_logger`와 `BOOST_LOG_SEV`가 record에 붙여 주는 source-specific attribute다.
  - `logger는 심각도 기반 record를 등록할 수 있게 해준다`보다는 `심각도 값을 포함한 로그 record를 만들 수 있게 해준다`가 더 정확하다.
  - `custom attribute를 추가하면 같은 방식으로 포맷에 넣을 수 있는가?`에 대한 답은 맞지만 너무 짧다. 예를 들어 global custom attribute를 직접 등록하려면 아래처럼 테스트해 볼 수 있다.

```cpp
#include <string>
#include <boost/log/core.hpp>
#include <boost/log/attributes/constant.hpp>

boost::log::core::get()->add_global_attribute(
    "Tag",
    boost::log::attributes::constant<std::string>("Phase02"));

const auto format =
    expr::stream
    << "[" << expr::attr<std::string>("Tag") << "] "
    << "[#" << expr::attr<unsigned int>("LineID") << "] "
    << "[" << expr::attr<boost::posix_time::ptime>("TimeStamp") << "] "
    << "[" << expr::attr<Severity>("Severity") << "] "
    << "[" << expr::attr<boost::log::attributes::current_thread_id::value_type>("ThreadID") << "] "
    << expr::smessage;
```

  - 위 예제를 `Phase02Formatting.cpp`에서 `add_common_attributes()` 바로 아래와 formatter 정의 부분에 적용하면, 출력 앞에 `[Phase02]` 같은 custom attribute가 함께 보인다.
