#include <stdio.h>
#include "parking_lot.h" 
#include "path_planning.h"
#include "position_detection.h"

#define ARRAY_CAPACITY 100 // 경로 및 빈자리 배열의 최대 크기

// 전역 변수 선언
int lot_number;                     // 주차장 번호
int **map_matrix = NULL;            // 주차장 지도 (동적 할당)
int map_rows, map_cols;             // 지도의 행과 열 크기
int empty[ARRAY_CAPACITY][2];       // 빈 주차 공간 목록 (현재 사용되지 않음)
int user_preference[2];             // 사용자 선호도 [주차 구역 유형, 기준]
int position[2];                    // 현재 위치 [행, 열]
int path[ARRAY_CAPACITY][2];        // 계산된 경로
int goal[2];                        // 목적지 주차 공간 [행, 열]


int main() {
    // 1. 주차장 식별
    lot_number = 1; // 예시로 1번 주차장으로 설정
    printf("\n------------ 주차장이 감지되었습니다. UWB 기반 실내 내비게이션을 동작합니다. --------------\n\n");
    
    // 2. 주차장 지도 정보 로드
    // lot_number에 해당하는 주차장 지도를 동적으로 생성하고 map_matrix에 할당
    map_matrix = create_map_copy(lot_number, &map_rows, &map_cols);

    // 3. 사용자 선호도 입력 (향후 GUI로 대체될 부분)
    printf("주차 구역 유형을 선택하세요. (2: 경차, 3: 장애인, 4: 일반, 5: 전기차): ");
    scanf("%d", &user_preference[0]);
    printf("어떤 기준으로 선택할까요? (1: 입구 근처, 2: 출구 근처, 3: 마트 출입구 근처): ");
    scanf("%d", &user_preference[1]);

    // 4. 서버와 통신해 비어있는 주차 구역 정보 받아오기

    // 5. 메인 루프 (실제 시스템에서는 RTOS의 태스크로 각 모듈이 실행됨)
    int count = 0; // 루프 실행 횟수 (디버깅용)
    while(1){
        printf("\n\n<< %d 번째 수행 >>\n", ++count);

        // 5-1. 현재 위치 업데이트
        // position[0] = 58, position[1] = 25; // 입구 (디버깅용)
        update_current_position(position);

        // 5-2. 최적의 주차 공간 탐색 및 경로 계획 (goal, path 전역변수에 저장 후 경로의 길이를 반환)
        int path_length = path_planning(position, user_preference, map_matrix, map_rows, map_cols, goal, path);

        // 5-3. 목적지 출력 (GUI로 대체)
        printf("추천 목적지: (%d, %d)\n", goal[0], goal[1]);

        // 5-4. 경로 출력 (GUI로 대체)
        // printf("path_length: %d\n", path_length); // 디버깅용
        printf("경로 안내:\n");
        for(int i = 0; i < path_length; i++) {
            printf("(%d, %d) -> ", path[i][0], path[i][1]);
        }
        printf("도착!\n");

        // 디버깅을 위한 루프 탈출 기능
        char quit;
        printf("\n\nMain Loop 를 종료하시겠습니까?(y/n): ");
        scanf(" %c", &quit); // 공백을 추가하여 이전 입력의 개행 문자를 무시
        if(quit == 'y') break;
    }

    // 6. 동적 할당된 메모리 해제
    if (map_matrix != NULL) {
        printf("\n프로그램 종료 전, 마지막으로 사용한 맵의 메모리를 해제합니다.\n");
        free_map_copy(map_matrix, map_rows);
    }
    
    return 0;
}