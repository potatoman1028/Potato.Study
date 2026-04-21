# Phase 08. Async Sink

## 목표

`asynchronous_sink`를 사용해 log record 생성과 backend 처리를 분리하고, producer thread의 `ThreadID`가 유지되는 구조를 이해한다.

## 이번 단계에서 볼 것

- `asynchronous_sink`
- `text_file_backend`
- dedicated feeding thread
- producer thread의 `ThreadID`
- `flush()`

## 코드 포인트

- phase 실행 함수: [Phase08AsyncSink.cpp](../phases/Phase08AsyncSink.cpp)
- async sink 생성: [Phase08AsyncSink.cpp](../phases/Phase08AsyncSink.cpp)
- file backend 직접 조립: [Phase08AsyncSink.cpp](../phases/Phase08AsyncSink.cpp)
- `flush()` 호출: [Phase08AsyncSink.cpp](../phases/Phase08AsyncSink.cpp)

## 핵심 이해

- `asynchronous_sink`는 log record를 바로 backend에 넘기지 않고 내부 queue에 넣는다.
- 기본적으로 async sink는 dedicated thread를 시작해서 queue에 있는 record를 backend로 전달한다.
- logger가 thread-safe한지와 sink가 async인지은 서로 다른 문제다.
- 로그에 찍히는 `ThreadID`는 보통 backend thread가 아니라 record를 만든 producer thread 기준으로 유지된다.
- `flush()`를 호출하면 queue에 쌓인 record를 backend까지 밀어 넣고 backend flush까지 진행할 수 있다.

## 직접 해볼 것

- `flush()` 호출을 잠시 빼고 결과 파일이 언제 채워지는지 관찰해 본다.
- worker 수를 `2`에서 `4`로 늘려 `ThreadID`가 더 많이 섞이는지 본다.
- 이후 `start_thread = false` 방식으로 바꿔 `feed_records()`를 직접 호출하는 구조를 비교해 본다.

## 배운 점

backend는 asynchronous_sink에 적용할 포멧이다. 
asynchronous_sink는 내부에 queue가 있고 thread safe하다.
sink-flush()을 통해서 쌓여있는 값을 backend에 밀어넣는다. 


## 관찰 메모

- phase 실행 후 `logs/phase8_async.log` 파일이 생성되어야 한다.
- 파일 안에는 `[Phase08][AsyncFile]` 형식의 로그가 여러 줄 들어가야 한다.
- 파일 안에는 main thread와 worker thread의 서로 다른 `ThreadID`가 보여야 한다.
- 콘솔에는 queue와 flush에 대한 안내 문구가 보여야 한다.

## 다음 질문

- `severity_logger_mt`와 `asynchronous_sink`는 각각 어떤 문제를 해결하고, 둘을 언제 같이 써야 하는가?
 
`severity_logger_mt`는 스레드 들이 logger를 안전하게 공유해야 하는 경우에 써야한다.
`async sink`는 파일 출력이나 콘솔 출력 때문에 작업 스레드가 자주 대기하게 되거나, 로그 처리 지연을 작업 스레드에서 분리하고 싶을 때다.

즉, `severity_logger_mt`는 logger 객체의 스레드 안전성에 초점이 있고,
`async sink`는 sink 처리 비용을 비동기로 넘겨 호출 스레드의 부담을 줄이는 데 초점이 있다.


## 궁금한 점

asynchronous_sink 내부의 queue는 스레드 세이프하고, lock-free한 queue한지 궁금함
sink-flush()의 역할

현재 코드에서 `asynchronous_sink`는 queueing strategy를 따로 지정하지 않았기 때문에 기본값인
`unbounded_fifo_queue`를 사용한다.

이 queue는 스레드 세이프하다.

- `asynchronous_sink` 기본 queueing strategy:
  - `boost/log/sinks/async_frontend.hpp`
  - `template< typename SinkBackendT, typename QueueingStrategyT = unbounded_fifo_queue >`
- `unbounded_fifo_queue` 내부:
  - `boost/log/sinks/unbounded_fifo_queue.hpp`
  - 내부에 `threadsafe_queue< record_view > m_queue;`를 사용한다.
- `threadsafe_queue` 설명:
  - `boost/log/detail/threadsafe_queue.hpp`
  - 주석에 `An unbounded thread-safe queue`라고 적혀 있고, `push`와 `try_pop`은 thread-safe라고 설명한다.

다만 "완전히 lock-free한가?"라고 묻는다면 조심해서 답해야 한다.

- `threadsafe_queue` 주석은 Michael-Scott의 non-blocking concurrent queue 알고리즘 기반이라고 설명한다.
- 하지만 `unbounded_fifo_queue` 바깥에는 `event`, `atomic interruption flag` 같은 보조 동기화가 함께 붙어 있다.
- 더 나아가 `asynchronous_sink` 전체는 backend 보호를 위해 `std::recursive_mutex`, flush 대기를 위해
  `condition_variable_any`, frontend mutex 등을 사용한다.

그래서 정리하면:

- 기본 queue 자체는 thread-safe하다.
- 내부 구현은 non-blocking queue 알고리즘 기반이다.
- 하지만 `asynchronous_sink` 전체를 "완전히 lock-free한 구조"라고 부르는 것은 맞지 않다.

`sink->flush()`의 역할은 queue에 쌓여 있는 record를 backend까지 밀어 넣고, 마지막에 backend의
`flush()`까지 호출하는 것이다.

조금 더 구체적으로 보면:

- `flush()` 호출 시 async frontend는 `m_FlushRequested`를 켠다.
- feeding thread가 queue를 비우면서 record를 backend로 전달한다.
- queue를 다 처리한 뒤 `flush_backend(...)`를 호출한다.
- `flush_backend(...)`는 backend가 flushing requirement를 지원하면 실제 `backend.flush()`를 호출한다.

즉, `flush()`는 단순히 "파일 버퍼만 비우는 함수"가 아니라:

- async queue에 남은 record 처리
- backend 전달
- backend flush

까지 포함하는 동작으로 이해하면 된다.

`auto_flush = true`와의 차이도 같이 기억하면 좋다.

- `auto_flush = true`
  - backend가 이미 받은 record를 쓸 때마다 파일 버퍼를 flush한다.
- `sink->flush()`
  - async sink queue에 남아 있는 record까지 backend로 보내고 마지막 backend flush까지 진행한다.

## 추가 궁금점
그렇다면 asynchronous_sink는 그렇다면 새로운 dedicated feeding thread를 만들어서 파일 혹은 콘솔에 적용한다는 것인가? 
그렇다면 console, file 하나씩 sink를 생성할 경우  dedicated feeding thread는 2개가 되는건가?

기본적으로는 그렇다.

`asynchronous_sink`는 생성될 때 기본값으로 dedicated feeding thread를 하나 시작한다.
이 thread가 내부 queue에서 record를 꺼내 backend로 전달한다.

즉:

- async file sink를 하나 만들면 dedicated feeding thread도 보통 하나 생긴다.
- async console sink를 하나 더 만들면 그 sink도 자기 dedicated feeding thread를 하나 더 가질 수 있다.

그래서 console용 async sink 하나, file용 async sink 하나를 따로 만들면 기본적으로 dedicated feeding thread도 2개라고 보면 된다.

다만 예외도 있다.

- `start_thread = false`로 async sink를 만들면 내부 thread를 자동으로 만들지 않는다.
- 이 경우에는 사용자가 직접 `run()`, `feed_records()`, `flush()`를 호출해서 queue를 처리해야 한다.

정리하면:

- async sink 하나 + 기본 설정: dedicated feeding thread 하나
- async sink 두 개 + 기본 설정: dedicated feeding thread 둘
- async sink + `start_thread = false`: 내부 dedicated feeding thread 없음

현재 `Phase 08` 코드는 async file sink 하나만 만들고 있으므로 dedicated feeding thread도 하나라고 이해하면 된다.

## 채점 결과

- 총평: 8/10
- 잘한 점:
  - `asynchronous_sink`가 내부 queue를 두고 backend 처리와 분리된다는 핵심을 잘 잡았다.
  - `flush()`가 "쌓인 값을 backend로 밀어 넣는다"는 방향도 맞다.
  - `severity_logger_mt`와 `async sink`의 역할 차이를 비교한 다음 질문 답변도 이번 phase 목표와 잘 맞는다.
- 보완할 점:
  - `backend는 asynchronous_sink에 적용할 포멧이다`라는 문장은 정확하지 않다. formatter는 sink frontend 쪽 설정이고, backend는 실제 파일 출력 같은 처리를 담당하는 쪽이다.
  - `flush()`는 단순히 queue를 backend로 넘기는 데서 끝나지 않고, backend flush까지 이어질 수 있다는 점을 함께 적어 두면 더 좋다.
  - `queue가 thread-safe하다`는 말은 맞지만, 기본 queue가 `unbounded_fifo_queue`라는 이름과 특성, 그리고 이것이 곧 `asynchronous_sink` 전체가 완전히 lock-free하다는 뜻은 아니라는 점까지 적으면 더 정확하다.
