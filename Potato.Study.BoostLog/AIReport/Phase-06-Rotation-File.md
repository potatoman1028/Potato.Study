# Phase 06. Rotation File

## 목표

파일 sink에 회전 설정을 붙여 로그 파일이 일정 크기를 넘으면 다음 파일로 넘어가는 구조를 이해한다.

## 이번 단계에서 볼 것

- `rotation_size`
- `%N` 파일명 패턴
- 여러 회전 파일 생성

## 코드 포인트

- phase 실행 함수: [Phase06RotationFile.cpp](../phases/Phase06RotationFile.cpp)
- 회전 파일 설정: [Phase06RotationFile.cpp](../phases/Phase06RotationFile.cpp)
- 실행 결과 예시 패턴: `logs/phase6_rotation_%3N.log`

## 핵심 이해

- `rotation_size`는 파일이 일정 크기에 도달했을 때 다음 파일로 넘어가게 한다.
- `%N`은 회전 파일 번호 자리다.
- formatter는 그대로 유지하면서 출력 파일만 여러 개로 나눌 수 있다.

## 직접 해볼 것

- `rotation_size`를 더 작게 줄여 더 자주 회전하게 해 본다.
- `%3N`을 `%5N`으로 바꿔 파일 번호 폭이 어떻게 달라지는지 본다.
- payload 길이를 줄이거나 늘려 회전 시점을 비교해 본다.

## 배운 점

파일 sink에는 옵션을 둘 수 있다.
파일의 크기가 커지면 로그 파일을 여러개로 쪼갤 수 있다.
rotation_size는 파일이 어느 정도 커지면 다음 번호 파일로 넘길지 정하는 기준이다. 

## 관찰 메모

- `logs` 폴더에 `phase6_rotation_000.log`, `phase6_rotation_001.log` 같은 파일이 둘 이상 생겨야 한다.
- 각 파일 안의 로그 형식은 같고, 파일 개수만 늘어나는지 보는 것이 포인트다.

## 다음 질문

- `target`, `max_files`, `max_size`까지 같이 주면 회전 파일 정리는 어떻게 달라지는가?

`target` : 로그 파일이 저장될 폴더를 따로 정한다.

`max_size` : target 폴더 안에 저장된 로그 파일들의 총합 크기 상한 
`max_files` : target 폴더에 보관할 로그 파일들의 최대 개수
`min_free_space` : 디스크에 최소한 남겨두고 싶은 여유 공간 

각 조건이 상한에 차면 가장 오래 된 로그 부터 지운다.

## 궁금한 점

max_size, max_files, min_free_space 같은 옵션들은 파일을 지우는데, 다른 저장소(target을 옮김)로 
옮겨서 대응하는 방법은 없는지

기본 collector 동작만으로는 없다.

`target`은 회전된 파일을 모아둘 고정된 폴더이고, `max_size`, `max_files`, `min_free_space` 조건을 넘기면
기본 collector는 가장 오래된 회전 파일부터 지우는 방식으로 조건을 맞춘다.

즉, "조건을 넘기면 다른 target으로 자동 이동" 같은 2단계 정책은 기본 옵션만으로는 만들 수 없다.

대신 아래 두 가지 방식은 가능하다.

- `target`만 두고 자동 삭제 조건은 빼서 회전 파일을 한 폴더에 계속 모은다.
- 오래된 파일 이동은 별도 스크립트나 예약 작업으로 처리한다.

더 세밀하게 제어하고 싶으면 custom `collector`를 직접 구현해서 `store_file(...)` 단계에서
"삭제 대신 다른 저장소로 이동"하도록 만들 수 있다.

## 채점 결과

- 총평: 8/10
- 잘한 점:
  - `rotation_size`를 "다음 번호 파일로 넘어가는 기준"으로 이해한 점이 정확하다.
  - `target`, `max_size`, `max_files`, `min_free_space`가 회전 파일 정리와 연결된다는 흐름을 잘 잡았다.
  - 이번 phase의 핵심을 "파일이 여러 개로 나뉜다"는 관찰 포인트로 정리한 점이 좋다.
- 보완할 점:
  - `파일 sink에는 옵션을 둘 수 있다`는 말은 맞지만 너무 넓다. 이번 phase에서는 `rotation_size`, `file_name 패턴`, `target` 같은 회전 관련 옵션이라고 좁혀 쓰면 더 좋다.
  - `max_size`, `max_files`, `min_free_space`는 보통 `target` 폴더의 회전 파일을 collector가 관리할 때 의미가 있다는 점을 함께 적어 두면 더 정확하다.
  - `각 조건이 상한에 차면`이라는 표현은 `min_free_space`와는 조금 결이 다르다. `설정한 제한이나 최소 여유 공간 조건을 만족할 때까지 오래된 파일을 지운다`고 쓰면 더 자연스럽다.
