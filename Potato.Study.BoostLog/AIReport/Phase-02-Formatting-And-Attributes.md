# Phase 02. Formatting And Attributes

## 목표

로그 포맷 문자열과 공통 attribute가 어떤 역할을 하는지 이해한다.

## 이번 단계에서 볼 것

- `add_common_attributes()`의 역할
- `LineID`, `TimeStamp`, `ThreadID`가 어디에서 오는지
- 포맷 표현식이 로그 문자열을 어떻게 만드는지

## 코드 포인트

- 공통 attribute 등록: [Phase02Formatting.cpp](D:/Potato.Study/Potato.Study.BoostLog/phases/Phase02Formatting.cpp:18)
- 포맷 정의 시작: [Phase02Formatting.cpp](D:/Potato.Study/Potato.Study.BoostLog/phases/Phase02Formatting.cpp:20)
- 날짜/시간 지원 헤더: [Phase02Formatting.cpp](D:/Potato.Study/Potato.Study.BoostLog/phases/Phase02Formatting.cpp:8)

## 핵심 이해

- `add_common_attributes()`가 없으면 포맷에서 참조한 기본 attribute가 비거나 실패할 수 있다.
- `boost::posix_time::ptime`를 로그 포맷에 쓰려면 날짜/시간 지원 헤더가 필요하다.
- 포맷은 문자열 템플릿이 아니라 expression 조합이다.

## 직접 해볼 것

- `ThreadID` 출력을 잠시 제거하고 결과를 비교한다.
- 로그 앞의 `[#LineID]` 구간을 다른 순서로 바꿔 본다.
- `Severity`를 메시지 뒤로 옮겨 가독성을 비교한다.

## 배운 점

- 아직 비워 둠

## 다음 질문

- custom attribute를 추가하면 같은 방식으로 포맷에 넣을 수 있는가?
