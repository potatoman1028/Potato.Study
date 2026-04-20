# Phase 01. Basic Setup

## 목표

`Boost.Log` 예제가 어떤 최소 구성으로 시작되는지 이해한다.

## 이번 단계에서 볼 것

- 왜 `ConfigureLogging()`를 먼저 호출하는지
- 왜 `severity_logger`를 사용하는지
- 로그 한 줄이 어디에서 만들어지는지

## 코드 포인트

- phase 실행 함수: [Phase01Basic.cpp](D:/Potato.Study/Potato.Study.BoostLog/phases/Phase01Basic.cpp:10)
- logger 선언: [Phase01Basic.cpp](D:/Potato.Study/Potato.Study.BoostLog/phases/Phase01Basic.cpp:15)
- 첫 로그 기록: [Phase01Basic.cpp](D:/Potato.Study/Potato.Study.BoostLog/phases/Phase01Basic.cpp:17)

## 핵심 이해

- 로깅 시스템은 먼저 설정한 뒤 사용해야 한다.
- `severity_logger<Severity>`는 로그 레벨을 코드에서 명시적으로 다루게 해준다.
- `BOOST_LOG_SEV` 매크로는 logger와 severity를 함께 받아 로그 레코드를 만든다.

## 직접 해볼 것

- `Severity::Trace`를 `Severity::Info`로 바꿔 보며 출력이 어떻게 달라지는지 확인한다.
- 로그 메시지 문구를 바꾸고 실행 결과를 본다.

## 배운 점

- 아직 비워 둠

## 다음 질문

- severity enum을 더 늘리면 포맷과 출력은 그대로 유지되는가?
