# Phase 04. Severity Filter

## 목표

severity filter를 사용해 같은 로그 이벤트라도 sink마다 다른 기준으로 통과시킬 수 있는 구조를 이해한다.

## 이번 단계에서 볼 것

- sink별 severity filter
- console sink와 file sink의 출력 차이
- formatter와 filter의 역할 차이

## 코드 포인트

- phase 실행 함수: [Phase04SeverityFilter.cpp](../phases/Phase04SeverityFilter.cpp)
- file sink filter: [Phase04SeverityFilter.cpp](../phases/Phase04SeverityFilter.cpp)
- 실행 결과 예시 파일: `logs/phase4_warning_or_higher.log`

## 핵심 이해

- filter는 로그를 "어떻게 보일지"가 아니라 "통과시킬지"를 결정한다.
- sink마다 다른 filter를 주면, 같은 record라도 sink마다 남는 로그가 달라질 수 있다.
- 이번 예제에서는 console sink는 전체 로그를 보고, file sink는 `Warning` 이상만 기록한다.

## 직접 해볼 것

- file sink 기준을 `Severity::Error` 이상으로 바꿔 본다.
- console sink에도 filter를 붙여서 `Info` 이상만 보이게 해 본다.
- file sink formatter에서 `[WarningOrHigher]` 구간을 지우고 출력 차이를 본다.

## 배운 점

- filter는 조건이 맞는 로그만 걸러준다. 

## 관찰 메모

- `Trace`, `Info`는 콘솔에는 보이지만 파일에는 남지 않아야 한다.
- `Warning`, `Error`는 콘솔과 파일 양쪽에 모두 남아야 한다.

## 다음 질문

- global filter와 sink filter를 같이 쓰면 어떤 순서로 적용되는가?
global -> sink 순서로 적용된다.

## 채점 결과

- 총평: 8/10
- 잘한 점:
  - filter의 핵심 역할을 `조건이 맞는 로그만 걸러준다`로 짧고 정확하게 잡았다.
  - `global -> sink` 순서로 적용된다고 정리한 답은 현재 학습 흐름과 잘 맞는다.
- 보완할 점:
  - `배운 점`이 맞는 방향이지만 조금 더 구체적이면 더 좋다. 예를 들어 `console sink는 전체 로그를 받고, file sink는 Warning 이상만 남긴다`까지 적어 두면 이번 phase의 차별점이 분명해진다.
  - `global -> sink`라는 답은 맞지만, `core 입구의 global filter가 먼저 적용되고 그 뒤에 각 sink filter가 적용된다`라고 한 줄 더 풀어 쓰면 나중에 다시 볼 때 덜 헷갈린다.
