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

- console, file attribute 를 다르게 가져갈 수 있다. 
- 각 sink에 등록한 포멧으로 출력된다. 

## 다음 질문

- sink마다 다른 포맷과 필터를 줄 수 있는가?
할 수 있습니다.

## 궁금한 점
- add_file_log에 옵션들에 대한 정리

`add_file_log`는 file sink를 만들면서 옵션을 함께 넘기는 helper 함수다. 현재 Phase 03에서 먼저 이해하면 좋은 옵션은 아래 네 가지다.

- `file_name`
  - 로그 파일 이름이나 패턴을 정한다.
  - 현재 예제의 `"logs/phase3_sample.log"`는 현재 작업 디렉터리 기준 상대 경로다.
- `open_mode`
  - 파일을 어떤 방식으로 열지 정한다.
  - 현재 예제의 `std::ios_base::app`는 기존 파일 뒤에 이어 쓰기(append)다.
- `auto_flush`
  - 로그 한 줄을 쓸 때마다 바로 flush할지 정한다.
  - `true`면 실행 중간에 프로그램이 끝나도 파일에 바로 반영되는 편이라 학습용으로 확인하기 쉽다.
- `format`
  - file sink 전용 formatter를 정한다.
  - console sink와 다른 formatter를 넣으면 파일 출력 모양을 따로 가져갈 수 있다.

추가로 자주 보게 되는 옵션은 아래와 같다.

- `filter`
  - 이 sink에 들어올 로그만 따로 거른다.
- `rotation_size`
  - 파일 크기가 일정 크기를 넘으면 다음 파일로 넘긴다.
- `target`
  - 로그 파일이 저장될 폴더를 따로 정한다.
- `max_size`, `max_files`, `min_free_space`
  - 오래된 로그 파일 정리 정책이나 디스크 사용량 제한과 연결된다.

예를 들어 아래처럼 바꾸면 파일 이름 패턴과 rotation 옵션을 함께 실험할 수 있다.

```cpp
boost::log::add_file_log(
    keywords::file_name = "logs/phase3_%N.log",
    keywords::open_mode = std::ios_base::app,
    keywords::rotation_size = 10 * 1024,
    keywords::auto_flush = true,
    keywords::format = file_format);
```

위 예제에서 `%N`은 회전 파일 번호가 들어갈 자리로 이해하면 된다.

## 채점 결과

- 총평: 7/10
- 잘한 점:
  - 하나의 로그 이벤트가 console sink와 file sink로 동시에 갈 수 있다는 핵심을 잘 짚었다.
  - sink마다 다른 formatter를 가질 수 있다는 점을 짧게라도 정리한 것은 좋다.
- 보완할 점:
  - `console, file attribute를 다르게 가져갈 수 있다`는 표현은 현재 코드와 조금 다르다. 지금 예제에서 다른 것은 attribute 자체보다 sink별 formatter와 출력 대상이다.
  - `각 sink에 등록한 포멧으로 출력된다`는 방향은 맞지만, `포멧`보다는 `포맷`, 그리고 `각 sink는 자기 formatter 규칙으로 출력된다` 정도로 쓰면 더 자연스럽다.
  - `sink마다 다른 포맷과 필터를 줄 수 있는가?`에 대한 답은 맞지만 너무 짧다. `keywords::format`, `keywords::filter` 같은 sink별 옵션으로 각각 다르게 줄 수 있다고 한 줄 더 적어 두면 학습 메모로 더 좋아진다.
