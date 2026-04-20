# Phase 03. Sinks And Output

## 목표

하나의 로그 이벤트가 콘솔과 파일로 동시에 기록되는 구조를 이해한다.

## 이번 단계에서 볼 것

- console sink 추가
- file sink 추가
- 상대 경로 파일 출력의 기준

## 코드 포인트

- console sink: [Phase03Sinks.cpp](../phases/Phase03Sinks.cpp)
- file sink: [Phase03Sinks.cpp](../phases/Phase03Sinks.cpp)
- 실행 결과 예시 파일: `logs/phase3_sample.log`

## 핵심 이해

- sink는 로그가 실제로 흘러가는 목적지다.
- 같은 포맷을 여러 sink에 공유할 수 있다.
- `logs/sample.log` 같은 상대 경로는 실행 파일 위치가 아니라 현재 작업 디렉터리를 기준으로 해석된다.

## 직접 해볼 것

- file sink만 남기고 콘솔 sink를 제거해 본다.
- `logs/sample_%N.log` 같은 패턴으로 파일명을 바꿔 본다.
- `auto_flush`를 끄고 동작 차이를 관찰한다.

## 배운 점

- 아직 비워 둠

## 다음 질문

- sink마다 다른 포맷과 필터를 줄 수 있는가?
