#include <stdio.h>
#include <stdint.h>
#include "parking_lot.h" 
#include "path_planning.h"
#include "position_detection.h"
#include "server_communication.h"

#define ARRAY_CAPACITY 100 // 경로 및 빈자리 배열의 최대 크기 정의

// 전역 변수 선언
uint8_t lot_number;                     // 현재 주차장 번호를 저장
uint8_t **map_matrix = NULL;            // 주차장 지도를 나타내는 2D 배열 (동적 할당)
uint8_t map_rows, map_cols;             // 주차장 지도의 행과 열 크기
uint8_t user_preference[2];             // 사용자 선호도: [주차 구역 유형, 기준]
                                        // 유형: 2(경차), 3(장애인), 4(일반), 5(전기차)
                                        // 기준: 1(입구 근처), 2(출구 근처), 3(마트 출입구 근처)
uint8_t position[2];                    // 현재 차량의 위치 [행, 열]
uint8_t path[ARRAY_CAPACITY][2];        // A* 알고리즘으로 계산된 최적 경로를 저장하는 배열
uint8_t path_length;                    // 최적 경로의 길이
uint8_t goal[2];                        // 최종 목적지 주차 공간의 좌표 [행, 열]


int main() {
    // 1. 주차장 식별
    // 실제 시스템에서는 센서 등을 통해 주차장 번호를 자동으로 감지
    lot_number = 1; // 예시로 1번 주차장으로 설정
    printf("\n------------ 주차장이 감지되었습니다. UWB 기반 실내 내비게이션을 동작합니다. --------------\n\n");
    
    // 2. 주차장 지도 정보 로드
    // lot_number에 해당하는 주차장 지도를 동적으로 생성하고 map_matrix에 할당
    // parking_lot.h에 정의된 함수를 사용하여 지도 및 주차 공간 정보 로드
    map_matrix = create_map_copy(lot_number, &map_rows, &map_cols);

    // 3. 사용자 선호도 입력 (향후 GUI로 대체될 부분)
    // 사용자로부터 주차 구역 유형 및 선호 기준을 입력받음
    printf("주차 구역 유형을 선택하세요. (2: 경차, 3: 장애인, 4: 일반, 5: 전기차): ");
    scanf("%hhu", &user_preference[0]);
    printf("어떤 기준으로 선택할까요? (1: 입구 근처, 2: 출구 근처, 3: 마트 출입구 근처): ");
    scanf("%hhu", &user_preference[1]);

    // 4. 서버와 통신해 비어있는 않은 주차 구역 정보 받아오기 --> 해당 좌표 벽(1)으로 
    if(update_parking_occupancy(map_matrix) != 1){
        printf("서버 통신 실패");
        return 0;
    }

    // 5. 메인 루프 (실제 시스템에서는 RTOS의 태스크로 각 모듈이 주기적으로 실행됨)
    int count = 0; // 루프 실행 횟수 (디버깅 및 테스트용)
    while(1){
        printf("\n\n<< %hhu 번째 수행 >>\n", ++count);

        // 5-1. 현재 위치 업데이트
        // position_detection.h에 정의된 함수를 사용하여 현재 위치를 업데이트
        // position[0] = 58, position[1] = 25;  // 입구 (디버깅용: 특정 위치로 고정)
        update_current_position(position);      // 아직 앵커들과의 거리 고정 --> 입구 (uwb 통신 구현 예정)

        // 5-2. 최적의 주차 공간 탐색 및 경로 계획
        // find_preferred_parking: 사용자 선호도에 따라 최적의 빈 주차 공간(goal)을 탐색
        // astar: 현재 위치(position)에서 목적지(goal)까지의 최적 경로(path)를 A* 알고리즘으로 계산
        // 경로의 길이를 반환하며, goal과 path는 전역변수에 저장됨
        find_preferred_parking(user_preference, map_matrix, map_rows, map_cols, goal);
        path_length = astar(position, goal, map_matrix, map_rows, map_cols, path);

        // 5-3. 결과 출력 (향후 GUI로 대체될 부분)
        printf("현재 위치: (%hhu, %hhu)\n", position[0], position[1]);
        printf("추천 목적지: (%hhu, %hhu)\n", goal[0], goal[1]);
        printf("경로 안내:\n");
        for(int i = 0; i < path_length; i++) {
            printf("(%hhu, %hhu) -> ", path[i][0], path[i][1]);
        }
        printf("도착!\n");

        // 디버깅을 위한 루프 탈출 기능
        char quit;
        printf("\n\nMain Loop 를 계속하시겠습니까?(y/n): ");
        scanf(" %c", &quit); // 공백을 추가하여 이전 입력의 개행 문자를 무시
        if(quit == 'n') break;
    }

    // 6. 동적 할당된 메모리 해제
    // 프로그램 종료 전, 동적으로 할당된 지도 및 주차 공간 메모리를 해제하여 메모리 누수 방지
    if (map_matrix != NULL) {
        printf("\n프로그램 종료 전, 마지막으로 사용한 맵의 메모리를 해제합니다.\n");
        free_map_copy(map_matrix, map_rows);
    }
    
    return 0;
}