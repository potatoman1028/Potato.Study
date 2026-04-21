## TODO And Backlog

## 목표

현재 최소 예제 다음에 어떤 순서로 학습을 확장할지 TODO 목록으로 관리한다.

## TODO List

- [x] severity 필터 추가
  - 현재 작업 phase: [Phase-04-Severity-Filter.md](./Phase-04-Severity-Filter.md)
- [x] custom attribute 추가
  - 현재 작업 phase: [Phase-05-Custom-Attribute.md](./Phase-05-Custom-Attribute.md)
- [x] 회전 파일 설정
  - 현재 작업 phase: [Phase-06-Rotation-File.md](./Phase-06-Rotation-File.md)
- [x] 멀티스레드 로그 실험
  - 현재 작업 phase: [Phase-07-Multi-Thread.md](./Phase-07-Multi-Thread.md)
- [x] 비동기 sink 검토
  - 현재 작업 phase: [Phase-08-Async-Sink.md](./Phase-08-Async-Sink.md)

## 왜 이 순서인가

- 현재 예제 위에 가장 작은 차이로 붙일 수 있다.
- 각 단계가 이전 단계의 개념을 그대로 재사용한다.
- 한 번에 하나의 개념만 추가하므로 학습용으로 읽기 쉽다.

## 다음 실험 아이디어

- `Error` 이상만 파일에 남기기
- 요청 ID 같은 custom attribute 추가하기
- 파일 크기 기준 회전 로그 만들기
- 여러 스레드에서 동시에 로그 남기기

## 언제 이 접근이 유용한가

- 새 라이브러리를 처음 익힐 때
- 팀에 공유할 최소 예제를 만들 때
- 문제를 작은 단계로 나눠 검증할 때

## 언제 피해야 하는가

- 운영 환경 설정을 바로 만들 때
- 예외 처리, 성능, 보존 정책까지 한 번에 다뤄야 할 때
- 실전 로깅 정책을 곧바로 확정하려고 할 때
