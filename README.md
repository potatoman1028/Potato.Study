# Potato.Study
개발에 필요한 것들을 학습하는 레포지토리입니다.

## 공통 개발 환경

이 저장소의 C++ 학습 예제는 앞으로 `vcpkg` 환경을 기준으로 설명한다.

- 운영 환경: Windows
- IDE: Visual Studio 2022
- 빌드 도구: MSBuild
- 패키지 관리자: Visual Studio 번들 `vcpkg`
- 의존성 관리 방식: `vcpkg` manifest mode

현재 로컬의 Visual Studio 번들 `vcpkg`는 manifest를 기준으로 동작한다. 따라서 이 저장소의 문서는 `BOOST_ROOT` 수동 설정 대신, 솔루션 루트 `vcpkg.json`으로 의존성을 관리하는 방향을 기준으로 정리한다.

현재 상태:

- 루트 `vcpkg.json`은 아직 없다.
- 따라서 CLI 패키지 복원 절차는 manifest 추가와 함께 확정해야 한다.
- 솔루션 전체 의존성 기준으로는 최소 `boost-log`, `boost-json`이 manifest에 포함되어야 한다.

의존성 복원이 끝난 뒤 공통 빌드 명령 예시는 아래와 같다.

```powershell
& "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" .\Potato.Study.sln /p:Configuration=Debug /p:Platform=x64
```

현재 학습 예제:

- [Potato.Study.BoostLog](Potato.Study.BoostLog/README.md)
- [Potato.Study.BoostJson](Potato.Study.BoostJson/README.md)
