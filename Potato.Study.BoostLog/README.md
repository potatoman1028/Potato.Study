# Potato.Study.BoostLog

## 무엇을 실험하는지

`Boost.Log`로 가장 작은 형태의 구조화된 로그 출력을 만드는 예제를 실험한다.

- 콘솔 sink
- 파일 sink
- severity filter
- custom attribute
- rotation file
- multi-thread logging
- async sink
- severity level
- 공통 attribute(`LineID`, `TimeStamp`, `ThreadID`)

## 어떻게 실행하는지

### 전제 조건

- Visual Studio 2022 C++ 빌드 도구
- Boost 설치 또는 Visual Studio에 전역 통합된 vcpkg Boost 패키지
- 수동 Boost 설치를 사용할 경우 `BOOST_ROOT` 환경 변수가 Boost 루트를 가리켜야 함

`BOOST_ROOT` 예시:

```powershell
$env:BOOST_ROOT = "C:\local\boost_1_86_0"
```

`Boost.Log`는 링크가 필요한 라이브러리이므로, 필요하면 Boost에서 라이브러리를 먼저 빌드해야 한다.

예시:

```powershell
cd $env:BOOST_ROOT
.\bootstrap.bat
.\b2 --with-log --with-thread --with-filesystem --with-date_time stage
```

솔루션 빌드:

```powershell
msbuild .\Potato.Study.sln /p:Configuration=Debug /p:Platform=x64
```

참고:

- 이 저장소는 `vcpkg.json`을 포함하지 않는다.
- Visual Studio에 전역 통합된 vcpkg가 있으면 `BOOST_ROOT` 없이도 빌드가 될 수 있다.
- 전역 vcpkg 통합이 없는 환경에서는 수동 Boost 설치와 `BOOST_ROOT` 설정이 필요하다.

실행 파일:

```powershell
.\x64\Debug\Potato.Study.BoostLog.exe
```

실행하면 콘솔 메뉴가 뜨고, `1`, `2`, `3`, `4`, `5`, `6`, `7`, `8` 중 하나를 입력해서 phase를 선택한다.

- `1`: Phase 01 - Basic severity logger
- `2`: Phase 02 - Formatting and common attributes
- `3`: Phase 03 - Console sink and file sink
- `4`: Phase 04 - Severity filter on file sink
- `5`: Phase 05 - Custom request attribute
- `6`: Phase 06 - Rotation file sink
- `7`: Phase 07 - Multi-thread logging
- `8`: Phase 08 - Async sink
- `q`: 종료

phase 실행이 끝나면 프로그램은 종료되지 않고 다시 메뉴로 돌아간다.

`phase3` 실행 후 콘솔에 로그가 출력되고, 현재 작업 디렉터리 기준 `logs\phase3_sample.log` 파일이 생성된다.

`phase4` 실행 후 콘솔에는 모든 severity가 출력되고, 현재 작업 디렉터리 기준 `logs\phase4_warning_or_higher.log` 파일에는 `Warning` 이상만 기록된다.

`phase5` 실행 후 콘솔에는 `RequestID` custom attribute가 출력된다. scoped request context 안에서는 `REQ-1001`, scope 밖에서는 `none`으로 보인다.

`phase6` 실행 후 `logs\phase6_rotation_%3N.log` 패턴으로 여러 회전 파일이 생성된다.

`phase7` 실행 후 여러 worker thread가 같은 console sink로 로그를 남기고, 출력에 서로 다른 `ThreadID`가 섞여 보인다.

`phase8` 실행 후 async sink가 main thread와 worker thread가 만든 record를 queue에 쌓아 file backend로 넘기고, `logs\phase8_async.log` 파일에서 producer thread별 `ThreadID`를 확인할 수 있다.

주의:

- 같은 프로세스 안에서 phase를 여러 번 실행하면 `LineID` 같은 공통 attribute 값은 이어질 수 있다.
- 이 예제는 sink 구성을 다시 잡는 데 초점을 두고 있고, process-wide attribute 상태 전체를 완전히 초기화하지는 않는다.

## 무엇을 배웠는지

- `add_common_attributes()`를 호출해야 기본 attribute를 포맷에서 사용할 수 있다.
- sink를 추가하면 같은 로그 이벤트를 콘솔과 파일에 동시에 보낼 수 있다.
- sink마다 다른 filter를 주면 같은 로그 이벤트라도 남는 결과가 달라질 수 있다.
- custom attribute를 추가하면 기본 attribute와 함께 formatter에서 읽을 수 있다.
- `rotation_size`와 파일명 패턴을 주면 하나의 file sink가 여러 회전 파일로 나뉠 수 있다.
- `severity_logger_mt`를 사용하면 여러 스레드가 같은 logger source를 공유할 수 있다.
- `asynchronous_sink`를 사용하면 record 생성과 backend 처리를 분리할 수 있다.
- `severity_logger`를 사용하면 문자열 대신 명시적인 심각도 레벨을 다룰 수 있다.
- `Boost.Log`는 헤더만 포함한다고 끝나지 않고 링크 설정까지 필요하다.

## AIReport

학습 내용을 phase 단위로 정리한 문서는 [AIReport/README.md](AIReport/README.md)에서 시작한다.
