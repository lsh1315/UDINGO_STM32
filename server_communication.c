#include <stdio.h>
#include <stdlib.h> // for malloc, free, atoi
#include <string.h> // for strcpy, strtok, strlen

#define MAX_RESPONSE_SIZE 100
#define MAX_PARKING_SPOT 100

// 정적 전역 변수 선언
static char response[MAX_RESPONSE_SIZE];
static int non_empty_spot[MAX_PARKING_SPOT][2];

/**
 * @brief 서버에 주차 공간 점유 정보를 요청하고 응답을 수신합니다.
 * @note 현재는 실제 서버 통신 대신 디버깅용 문자열을 사용합니다.
 * @param response 서버 응답을 저장할 버퍼에 대한 포인터입니다.
 * @return 성공 시 1, 실패 시 0을 반환합니다.
 */
static int Non_empty_spot_download(char *response) {
    // TODO: 실제 AWS 서버에 request를 보내고 response를 수신하는 로직 구현 필요
    // 현재는 디버깅을 위해 하드코딩된 문자열을 사용합니다.
    sprintf(response, "30,17;42,15;17,102;");
    return 1; // 성공적으로 응답을 받았다고 가정
}

/**
 * @brief "x1,y1;x2,y2;" 형식의 좌표 문자열을 2차원 정수 배열로 파싱합니다.
 * @param response 파싱할 좌표 문자열입니다.
 * @param array 파싱된 좌표를 저장할 2차원 배열입니다.
 * @return 파싱된 좌표 쌍의 개수를 반환합니다. 메모리 할당 실패 시 -1을 반환합니다.
 */
static int Response_to_Coordinates(char* response, int array[MAX_PARKING_SPOT][2]) {
    int i = 0;
    // strtok_r은 스레드에 안전하며 중첩된 토큰화에 사용됩니다. 원본 문자열 수정을 방지하기 위해 복사본을 사용합니다.
    char* response_copy = malloc(strlen(response) + 1);
    if (response_copy == NULL) {
        // 메모리 할당 실패
        return -1;
    }
    strcpy(response_copy, response);

    char *saveptr1; // 외부 루프의 상태를 저장하기 위한 포인터

    // ";"를 기준으로 좌표 쌍("x,y")을 분리합니다.
    char* pair_token = strtok_r(response_copy, ";", &saveptr1);
    while (pair_token != NULL && i < MAX_PARKING_SPOT) {
        char *saveptr2; // 내부 루프의 상태를 저장하기 위한 포인터

        // ","를 기준으로 x와 y 좌표를 분리합니다.
        char* x_str = strtok_r(pair_token, ",", &saveptr2);
        char* y_str = strtok_r(NULL, ",", &saveptr2);

        if (x_str != NULL && y_str != NULL) {
            // 문자열을 정수로 변환하여 배열에 저장합니다.
            array[i][0] = atoi(x_str);
            array[i][1] = atoi(y_str);
            i++;
        }
        // 다음 좌표 쌍을 찾습니다.
        pair_token = strtok_r(NULL, ";", &saveptr1);
    }

    free(response_copy); // 동적 할당된 메모리를 해제합니다.
    return i; // 파싱된 좌표의 개수를 반환합니다.
}

/**
 * @brief 파싱된 좌표를 기반으로 주차 맵에 점유된 공간(1)을 표시합니다.
 * @param map 업데이트할 주차장 맵 (2차원 배열).
 * @param spot_array 점유된 공간의 좌표 배열.
 * @param num 점유된 공간의 수.
 */
static void replace_1_non_empty_spot(int** map, int spot_array[MAX_PARKING_SPOT][2], int num) {
    for (int i = 0; i < num; i++) {
        int row = spot_array[i][0];
        int colum = spot_array[i][1];
        
        map[row][colum] = 1;
    }
}

/**
 * @brief 서버로부터 주차장 점유 정보를 받아와 전체 맵을 업데이트하는 메인 함수.
 * @param map 업데이트할 주차장 맵 (2차원 배열).
 * @return 성공 시 1, 실패 시 0을 반환합니다.
 */
int update_parking_occupancy(int **map) {
    if (Non_empty_spot_download(response) != 1) {
        return 0; // 서버로부터 데이터 다운로드 실패
    }

    int num_non_empty = Response_to_Coordinates(response, non_empty_spot);
    if (num_non_empty < 0) {
        return 0; // 좌표 파싱 실패
    }

    replace_1_non_empty_spot(map, non_empty_spot, num_non_empty);

    return 1; // 성공적으로 맵 업데이트 완료
}
