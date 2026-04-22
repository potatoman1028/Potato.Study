# Potato.Study.BoostJson

## 무엇을 실험하는지

`Boost.JSON`으로 JSON serializer 자체를 단계적으로 학습하는 예제를 만든다.

- JSON text parse / serialize
- JSON type 접근
- object / array 직접 조립
- 사용자 정의 타입과 JSON 변환
- validation / error handling
- 성능 기초(`stream_parser`, `monotonic_resource`)

이번 주제는 `config load`를 바로 만드는 것보다, 먼저 JSON DOM과 serializer의 동작을 명확히 이해하는 데 초점을 둔다.

## 어떻게 실행하는지

현재는 아래 phase까지 구현된 상태다.

- `Phase 01 - Parse and Serialize`
- `Phase 02 - Read JSON Types`
- `Phase 03 - Build JSON Programmatically`

필수 조건:

- Visual Studio 2022 C++ 빌드 도구
- Boost.JSON 헤더와 라이브러리를 찾을 수 있는 환경
  - 현재 로컬에서는 Visual Studio의 vcpkg 통합 환경으로 해결된다.
  - 수동 Boost 환경을 쓸 경우 `BOOST_ROOT`를 Boost 루트로 설정하고 Boost.JSON 라이브러리를 준비한다.

예시:

```powershell
$env:BOOST_ROOT = "C:\local\boost_1_86_0"
```

빌드:

```powershell
& "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" .\Potato.Study.sln /p:Configuration=Debug /p:Platform=x64
```

실행:

```powershell
.\x64\Debug\Potato.Study.BoostJson.exe
```

실행 후 메뉴에서 아래 값을 선택할 수 있다.

- `1`: Phase 01 - Parse and Serialize
- `2`: Phase 02 - Read JSON Types
- `3`: Phase 03 - Build JSON Programmatically

참고:

- 이번 phase는 기본 Boost.JSON 라이브러리 링크 방식을 사용한다.
- 목표는 먼저 `parse` / `serialize` 흐름과 JSON 타입 읽기를 확인하고, 이어서 object / array를 C++ 코드에서 직접 조립하는 것이다.

## 무엇을 배웠는지

- C++20 표준 라이브러리에는 JSON 전용 라이브러리가 포함되어 있지 않다.
- 성능을 우선할 때는 `Boost.JSON`과 `RapidJSON`이 먼저 검토 대상이 된다.
- 현재 저장소는 이미 Boost 기반 예제를 포함하고 있으므로, 새 JSON 학습 주제는 `Boost.JSON`으로 이어가는 편이 자연스럽다.
- `config load`를 바로 다루기보다 JSON serializer 자체를 먼저 익히는 편이 학습 범위를 더 명확하게 나눌 수 있다.
- Phase 01에서는 JSON text -> `boost::json::value` -> JSON text 흐름을 가장 작은 예제로 확인할 수 있다.
- Phase 02에서는 object, array, string, number, bool, null을 각각 어떤 API로 읽는지 확인할 수 있다.
- Phase 03에서는 parse 없이 object와 array를 직접 조립한 뒤 serialize 하는 흐름을 확인할 수 있다.
