# Phase 07. Multi-Thread Logging

## 목표

여러 스레드가 같은 logger와 sink로 동시에 로그를 남길 때 `ThreadID`와 출력 순서가 어떻게 보이는지 이해한다.

## 이번 단계에서 볼 것

- `severity_logger_mt`
- `std::thread`
- `ThreadID`
- 여러 스레드 로그의 interleaving

## 코드 포인트

- phase 실행 함수: [Phase07MultiThread.cpp](../phases/Phase07MultiThread.cpp)
- worker thread 함수: [Phase07MultiThread.cpp](../phases/Phase07MultiThread.cpp)
- thread-safe logger: [Phase07MultiThread.cpp](../phases/Phase07MultiThread.cpp)
- thread 생성과 `join`: [Phase07MultiThread.cpp](../phases/Phase07MultiThread.cpp)

## 핵심 이해

- `severity_logger_mt`는 여러 스레드에서 동시에 사용할 수 있는 logger source다.
- 각 로그 레코드에는 현재 스레드의 `ThreadID`가 같이 찍힌다.
- 멀티스레드 로그는 worker별로 깔끔하게 묶이지 않고 실행 타이밍에 따라 섞여 보일 수 있다.
- 이 단계의 목표는 로그 순서를 고정하는 것이 아니라, 멀티스레드 환경에서 로그가 어떻게 보이는지 관찰하는 것이다.

## 직접 해볼 것

- worker 개수를 `3`에서 `5`로 바꿔 본다.
- 각 worker의 반복 횟수를 늘려 출력이 더 많이 섞이는지 본다.
- `sleep_for` 시간을 줄이거나 제거해 출력 순서가 어떻게 달라지는지 본다.

## 배운 점

- 각 스레드에서 공유 자원인 logger을 이용해 로깅을 진행하는 예제이다. 
- severity_logger_mt는 멀티스레드 환경에서 설계된 심각도 기반의 로깅 객체이다. 
- main 스레드와 생성한 워커 스레드의 ID를 같이 확인 할 수 있다. 
- 워커를 생성하며 실행 후 join을 통해 대기한다. 
- 대기 만료 후 메인 스레드에서 로그를 출력한다.

## 관찰 메모

- 서로 다른 `ThreadID`가 보여야 한다.
- `Worker 1`, `Worker 2`, `Worker 3` 로그가 섞여 나올 수 있다.
- 마지막에 `All worker threads joined` 로그가 보여야 한다.

## 다음 질문

- 멀티스레드 환경에서는 언제 `severity_logger_mt`만으로 충분하고, 언제 `async sink`까지 고려해야 하는가?
지금 짐작되는 것은 여러 스레드가 하나의 sink에 붙는 경우가 있을 것 같다. 예를 들면 add_file_log를 두번 하는데(두번 할 수 있나?)
같은 타겟을 여러개 만든다던가 하는 실수..? 같은게 있을 것 같다.

## 궁금한 점

severity_logger_mt의 원리를 알고싶어 mt라고 한 만큼 로거의 레코드 안전은 보장할거라고 보이는데,
mutex로 데이터 안전을 보장하는지 아니면 다른 방법을 통해 안정성을 보장하는지 궁금해
lock을 통해서 방어처리를 한다면 mutex인지 srw lock인지도 궁금해 이걸 찾은 결과를 어떤 경로를 통해 얻은
근거인지 함께 기술해줘.

현재 로컬에 설치된 Boost 헤더를 따라가 보면 아래 순서로 확인할 수 있다.

- `severity_logger_mt` 정의:
  - `boost/log/sources/severity_logger.hpp`
  - `severity_logger_mt`는 `basic_composite_logger< ..., multi_thread_model< boost::log::aux::light_rw_mutex >, ... >`를 상속한다.
- `multi_thread_model` 정의:
  - `boost/log/sources/threading_models.hpp`
  - 내부에 `mutable MutexT m_Mutex;`를 가지고 `lock_shared`, `lock`, `unlock_shared`, `unlock` 같은 호출을 그대로 위임한다.
- `light_rw_mutex` 정의:
  - `boost/log/detail/light_rw_mutex.hpp`
  - Windows 분기에서는 `boost::winapi::SRWLOCK_`를 멤버로 가지고 `AcquireSRWLockShared`, `AcquireSRWLockExclusive`를 호출한다.

즉, 현재 Windows 환경 기준으로는 `severity_logger_mt`가 일반 `std::mutex` 하나로 막는 구조라기보다,
`multi_thread_model< light_rw_mutex >`를 통해 read/write lock 계열의 보호를 받는 구조로 보는 것이 맞다.

조금 더 구체적으로 보면:

- attribute 추가/제거/설정은 `exclusive_lock_guard`를 사용한다.
- `open_record()`는 `shared_lock_guard`를 사용한다.
- `push_record()`는 logger 내부 락을 추가로 잡지 않고 core로 record를 넘긴다.

그래서 이 logger가 보장하려는 것은 "logger 객체가 들고 있는 source-specific state와 record 생성 과정"의
스레드 안전성에 가깝고, sink의 출력 지연이나 비동기 처리까지 해결해 주는 것은 아니다.

정리하면:

- `severity_logger_mt`는 멀티스레드용 logger다.
- 현재 Windows 환경에서는 내부 동기화 primitive로 `SRWLOCK` 계열을 사용한다.
- 따라서 단순 `mutex` 하나로 막는 구조라고 보기보다, shared/exclusive lock 모델을 사용하는 구현이라고 이해하면 된다.

근거 경로:

- 로컬 설치 헤더:
  - `D:\vcpkg\installed\x64-windows\include\boost\log\sources\severity_logger.hpp`
  - `D:\vcpkg\installed\x64-windows\include\boost\log\sources\threading_models.hpp`
  - `D:\vcpkg\installed\x64-windows\include\boost\log\detail\light_rw_mutex.hpp`
  - `D:\vcpkg\installed\x64-windows\include\boost\log\sources\basic_logger.hpp`
- 공식 문서:
  - `severity_logger_mt`는 thread-safe logger로 설명된다.
  - `basic_logger`는 threading model policy를 사용한다고 설명한다.

## 채점 결과

- 총평: 7/10
- 잘한 점:
  - `severity_logger_mt`가 멀티스레드용 logger라는 핵심은 잘 잡았다.
  - main thread와 worker thread의 `ThreadID`를 같이 관찰했다는 점이 이번 phase 목표와 잘 맞는다.
  - `join()` 뒤에 main thread 로그가 다시 나온다는 흐름을 본 점도 좋다.
- 보완할 점:
  - 현재 코드에서는 worker를 `3`개가 아니라 `5`개 생성하고 있다. 그래서 `배운 점`과 `관찰 메모`를 쓸 때는 현재 코드 기준으로 맞춰 적는 편이 더 정확하다.
  - `대기 만료 후`라는 표현은 이번 코드와는 조금 다르다. timeout이 있는 대기가 아니라 `join()`으로 worker 종료를 끝까지 기다린 뒤 main thread가 마지막 로그를 남긴다.
  - `다음 질문`에 대한 현재 답변은 "멀티스레드에서 실수할 수 있는 sink 구성" 쪽으로 가 있어서, 질문의 핵심인 `severity_logger_mt`와 `async sink`의 역할 차이를 직접 설명하지는 못했다.
  이 부분은 "thread-safe logger만으로 충분한 경우"와 "sink 지연 때문에 async sink가 필요한 경우"로 나눠 적으면 더 좋아진다.
  정석적인 답변 예시:
  `severity_logger_mt`만으로 충분한 경우는 여러 스레드가 같은 logger를 안전하게 공유해야 하지만, sink 출력 비용이 아직 큰 병목이 아닌 경우다.
  반대로 `async sink`를 고려해야 하는 경우는 파일 출력이나 콘솔 출력 때문에 작업 스레드가 자주 대기하게 되거나, 로그 처리 지연을 작업 스레드에서 분리하고 싶을 때다.
  즉, `severity_logger_mt`는 logger 객체의 스레드 안전성에 초점이 있고, `async sink`는 sink 처리 비용을 비동기로 넘겨 호출 스레드의 부담을 줄이는 데 초점이 있다.
