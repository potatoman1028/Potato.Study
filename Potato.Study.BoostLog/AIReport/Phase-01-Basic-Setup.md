# Phase 01. Basic Setup

## 목표

`Boost.Log` 예제가 어떤 최소 구성으로 시작되는지 이해한다.

## 이번 단계에서 볼 것

- 왜 `ResetLogging()`과 `add_console_log()`를 먼저 호출하는지
- 왜 `severity_logger`를 사용하는지
- 로그 한 줄이 어디에서 만들어지는지

## 코드 포인트

- phase 실행 함수: [Phase01Basic.cpp](../phases/Phase01Basic.cpp)
- logger 선언: [Phase01Basic.cpp](../phases/Phase01Basic.cpp)
- 첫 로그 기록: [Phase01Basic.cpp](../phases/Phase01Basic.cpp)

## 핵심 이해

- 로깅 시스템은 먼저 설정한 뒤 사용해야 한다.
- `severity_logger<Severity>`는 로그 레벨을 코드에서 명시적으로 다루게 해준다.
- `BOOST_LOG_SEV` 매크로는 logger와 severity를 함께 받아 로그 레코드를 만든다.

## 직접 해볼 것

- `Severity::Trace`를 `Severity::Info`로 바꿔 보며 출력이 어떻게 달라지는지 확인한다.
- 로그 메시지 문구를 바꾸고 실행 결과를 본다.

## 배운 점

- ResetLogging : 메뉴에서 phase를 반복 실행할 수 있으므로, 이전 phase가 core에 등록한 sink를 지워 출력 구성이 누적되지 않게 한다.
- boost::log::add_console_log(std::cout) : console 로그를 위한 헬퍼 함수를 호출한다. 출력 스트림으로는 `std::cout`을 설정한다.
- sources::severity_logger<Severity> logger : 심각도에 기반하는 로그 객체를 만든다. 
- BOOST_LOG_SEV : Boost.Log에서 “severity(로그 레벨)를 포함한 로그 record를 만드는 매크로”

## 다음 질문

- severity enum을 더 늘리면 포맷과 출력은 그대로 유지되는가?
포맷 구조는 유지된다.
다만 새 enum 값을 사람이 읽는 문자열로 출력하려면 `operator<<`도 함께 수정해야 한다.
그렇지 않으면 새 값은 `Unknown`으로 보일 수 있다.

## 채점 결과

- 총평: 8/10
- 잘한 점:
  - `ResetLogging`, `add_console_log`, `severity_logger`, `BOOST_LOG_SEV`처럼 Phase 01의 핵심 구성요소를 잘 짚었다.
  - `BOOST_LOG_SEV`를 "severity를 포함한 log record를 만드는 매크로"라고 정리한 점이 특히 좋다.
- 보완할 점:
  - `ResetLogging`은 "최초 진입 시"만 호출되는 것이 아니라, phase를 실행할 때마다 호출되어 이전에 등록된 sink를 정리한다.
  - sink를 계속 등록하게 되는 직접적인 위치는 `main`이 아니라 각 phase 함수다.
  - `std:cout`는 `std::cout`로 쓰는 편이 맞다.
  - "severity enum을 더 늘리면 포맷과 출력은 그대로 유지된다"는 설명은 절반만 맞다. 포맷 구조는 유지되지만, 새 enum 값을 사람이 읽는 문자열로 출력하려면 `operator<<`도 함께 수정해야 한다. 그렇지 않으면 `Unknown`으로 출력될 수 있다.
