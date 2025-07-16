#include <stdio.h>
#include <stdlib.h> // for malloc, free, atoi
#include <string.h> // for strcpy, strtok, strlen
#include <stdint.h> // uint8_t 정의
#include "main.h"
#define MAX_RESPONSE_SIZE 50
#define MAX_PARKING_SPOT 50
#define BUF_SIZE 50
#define WIFI_NSS_GPIO_Port GPIOA
#define WIFI_NSS_Pin GPIO_PIN_11

// 정적 전역 변수 선언
static char response[MAX_RESPONSE_SIZE];
static uint8_t non_empty_spot[MAX_PARKING_SPOT][2];
static char spi_rx_buf[MAX_RESPONSE_SIZE];

extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart1;

void wifi_send_at(const char *cmd, uint32_t delay_ms)
{
    memset(spi_rx_buf, 0, BUF_SIZE);
    HAL_SPI_Transmit(&hspi2, (uint8_t *)cmd, strlen(cmd), 1000);
    HAL_Delay(delay_ms);
    HAL_SPI_Receive(&hspi2, (uint8_t *)spi_rx_buf, BUF_SIZE - 1, 5000);
}

/**
 * @brief 서버에 주차 공간 점유 정보를 요청하고 응답을 수신합니다.
 * @note 현재는 실제 서버 통신 대신 디버깅용 문자열을 사용합니다.
 * @param response 서버 응답을 저장할 버퍼에 대한 포인터입니다.
 * @return 성공 시 1, 실패 시 0을 반환합니다.
 */
static int Non_empty_spot_download(char *response) { // 여기 수정

	// NSS (CS) LOW
	HAL_GPIO_WritePin(WIFI_NSS_GPIO_Port, WIFI_NSS_Pin, GPIO_PIN_RESET);
	memset(response, 0, BUF_SIZE);

	    // 1️ AT 통신 확인
	    wifi_send_at("AT\r\n", 100);
	    HAL_UART_Transmit(&huart1, (uint8_t*)spi_rx_buf, strlen(spi_rx_buf),100);
	    if (!strstr(spi_rx_buf, "OK")) return 0;

	    // 2️ Wi-Fi 연결 (SSID, PASSWORD 변경 필요)
	    wifi_send_at("AT+CWJAP=\"Desktopdy\",\"12345678\"\r\n", 3000);
	    HAL_UART_Transmit(&huart1, (uint8_t*)"2", 2,100);
	    if (!strstr(spi_rx_buf, "WIFI GOT IP")) return 0;

	    // 3️⃣ TCP 연결 시작 (IP, PORT)
	    wifi_send_at("AT+CIPSTART=\"TCP\",\"3.39.40.177\",5000\r\n", 300);
	    HAL_UART_Transmit(&huart1, (uint8_t*)"3", 2,100);
	    if (!strstr(spi_rx_buf, "OK")) return 0;

	    // 4️⃣ HTTP GET 요청 길이 송신
	    char http_request[] = "GET /occupied_string HTTP/1.1\r\nHost: 3.39.40.177\r\n\r\n";
	    char at_cipsend[32];
	    sprintf(at_cipsend, "AT+CIPSEND=%d\r\n", (int)strlen(http_request));
	    wifi_send_at(at_cipsend, 100);
	    HAL_UART_Transmit(&huart1, (uint8_t*)"4", 2,100);
	    if (!strstr(spi_rx_buf, ">")) return 0;  // > 나오면 준비 완료

	    // 5️⃣ HTTP GET 송신
	    wifi_send_at(http_request, 500);
	    // 6️⃣ 응답 수신 (spi_rx_buf 안에 +IPD 포함될 것)
	    HAL_SPI_Receive(&hspi2, (uint8_t *)spi_rx_buf, BUF_SIZE - 1, 3000);

	    // 7️⃣ +IPD 응답 파싱해서 response[] 저장
	    char *start = strstr(spi_rx_buf, "+IPD,");
	    if (start)
	    {
	        start = strchr(start, ':');
	        if (start && *(start + 1) != '\0')
	        {
	            start++;
	            strcpy(response, start);
	        }
	    }

	    // 8️⃣ TCP 연결 종료
	    wifi_send_at("AT+CIPCLOSE\r\n", 100);
	    return 1;
}


/**
 * @brief "x1,y1;x2,y2;" 형식의 좌표 문자열을 2차원 정수 배열로 파싱합니다.
 * @param response 파싱할 좌표 문자열입니다.
 * @param array 파싱된 좌표를 저장할 2차원 배열입니다.
 * @return 파싱된 좌표 쌍의 개수를 반환합니다. 메모리 할당 실패 시 -1을 반환합니다.
 */
static int Response_to_Coordinates(char* response, uint8_t array[MAX_PARKING_SPOT][2]) {
    uint8_t i = 0;
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
static void replace_1_non_empty_spot(uint8_t** map, uint8_t spot_array[MAX_PARKING_SPOT][2], uint8_t num) {
    for (int i = 0; i < num; i++) {
        uint8_t row = spot_array[i][0];
        uint8_t colum = spot_array[i][1];
        
        map[row][colum] = 1;
    }
}

/**
 * @brief 서버로부터 주차장 점유 정보를 받아와 전체 맵을 업데이트하는 메인 함수.
 * @param map 업데이트할 주차장 맵 (2차원 배열).
 * @return 성공 시 1, 실패 시 0을 반환합니다.
 */
int update_parking_occupancy(uint8_t **map) {
    if (Non_empty_spot_download(response) != 1) {
        return 0; // 서버로부터 데이터 다운로드 실패
    }

    uint8_t num_non_empty = Response_to_Coordinates(response, non_empty_spot);
    if (num_non_empty < 0) {
        return 0; // 좌표 파싱 실패
    }

    replace_1_non_empty_spot(map, non_empty_spot, num_non_empty);

    return 1; // 성공적으로 맵 업데이트 완료
}
