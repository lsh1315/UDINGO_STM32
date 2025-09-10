#include <stdio.h>
#include <stdlib.h> // for malloc, free, atoi
#include <string.h> // for strcpy, strtok, strlen
#include <stdint.h> // uint8_t 정의
#include "main.h"
#define MAX_RESPONSE_SIZE 512
#define MAX_PARKING_SPOT 50
#define BUF_SIZE 50


// 정적 전역 변수 선언
static char response[MAX_RESPONSE_SIZE];
static uint8_t non_empty_spot[MAX_PARKING_SPOT][2];

extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart1;

void wifi_nss_low(void)
{
    HAL_GPIO_WritePin(SPI2_NSS_GPIO_Port, SPI2_NSS_Pin, GPIO_PIN_RESET);
}

void wifi_nss_high(void)
{
    HAL_GPIO_WritePin(SPI2_NSS_GPIO_Port, SPI2_NSS_Pin, GPIO_PIN_SET);
}

void wifi_send_cmd_16bit(const char* cmd)
{
    uint8_t len = strlen(cmd);
    uint8_t i = 0;

    wifi_nss_low();

    while (i < len)
    {
        uint8_t ch1 = cmd[i++];
        uint8_t ch2 = (i < len) ? cmd[i++] : 0x15;  // 홀수 패딩
        uint16_t word = (ch2 << 8) | ch1;
        uint16_t resp = 0;
        HAL_SPI_TransmitReceive(&hspi2, (uint8_t*)&word, (uint8_t*)&resp, 1, HAL_MAX_DELAY);
    }

    wifi_nss_high();
    HAL_Delay(10);
}
void wifi_receive_response_16bit(char* buf, uint16_t buf_len)
{
    uint8_t i = 0;
    uint32_t t_start = HAL_GetTick();

    wifi_nss_low();
    while (i < buf_len - 1) {
        // ⏰ 타임아웃: 1초 이상 응답 없으면 탈출
        if (HAL_GetTick() - t_start > 1000) break;

        uint16_t dummy = 0x0000;
        uint16_t resp = 0;
        HAL_SPI_TransmitReceive(&hspi2, (uint8_t*)&dummy, (uint8_t*)&resp, 1, HAL_MAX_DELAY);

        // Low byte
        buf[i++] = resp & 0xFF;
        if ((resp & 0xFF) == '>') break;

        // High byte
        if (i < buf_len - 1) {
            buf[i++] = (resp >> 8) & 0xFF;
            if (((resp >> 8) & 0xFF) == '>') break;
        }

        HAL_Delay(1);  // 너무 빠른 루프 방지
    }

    buf[i] = '\0';
    wifi_nss_high();
    HAL_Delay(5);
}



void wifi_wait_ready(void)
{
    while (HAL_GPIO_ReadPin(WIFI_DATRDY_GPIO_Port, WIFI_DATRDY_Pin) == GPIO_PIN_RESET);
}

/**
 * @brief 서버에 주차 공간 점유 정보를 요청하고 응답을 수신합니다.
 */
static int Non_empty_spot_download(char* response)
{
    //sprintf(response, "30,17;42,15;17,102;");
    HAL_GPIO_WritePin(WIFI_RST_GPIO_Port, WIFI_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(WIFI_RST_GPIO_Port, WIFI_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(50);

    HAL_GPIO_WritePin(WIFI_WKUP_GPIO_Port, WIFI_WKUP_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(WIFI_BOOT_GPIO_Port, WIFI_BOOT_Pin, GPIO_PIN_RESET);
    HAL_Delay(100);

    wifi_wait_ready();
    wifi_send_cmd_16bit("$$$\r");
    wifi_wait_ready();
    wifi_receive_response_16bit(response, 128);
    HAL_UART_Transmit(&huart1, (uint8_t*)response, strlen(response), 100);

    wifi_wait_ready();
    wifi_send_cmd_16bit("S1=10\r");
    wifi_wait_ready();
    wifi_receive_response_16bit(response, 128);
    HAL_UART_Transmit(&huart1, (uint8_t*)response, strlen(response), 100);

    wifi_wait_ready();
    wifi_send_cmd_16bit("C1=Dayeon\r\n");
    wifi_wait_ready();
    wifi_receive_response_16bit(response, 128);
    HAL_UART_Transmit(&huart1, (uint8_t*)response, strlen(response), 100);

    wifi_wait_ready();
    wifi_send_cmd_16bit("C2=12791279\r\n");
    wifi_wait_ready();
    wifi_receive_response_16bit(response, 128);
    HAL_UART_Transmit(&huart1, (uint8_t*)response, strlen(response), 100);

    wifi_wait_ready();
    wifi_send_cmd_16bit("C3=4\r\n");
    wifi_wait_ready();
    wifi_receive_response_16bit(response, 128);
    HAL_UART_Transmit(&huart1, (uint8_t*)response, strlen(response), 100);

    wifi_wait_ready();
    wifi_send_cmd_16bit("C4=1\r\n");
    wifi_wait_ready();
    wifi_receive_response_16bit(response, 128);
    HAL_UART_Transmit(&huart1, (uint8_t*)response, strlen(response), 100);

    wifi_wait_ready();
    wifi_send_cmd_16bit("C0\r\n");
    wifi_wait_ready();
    wifi_receive_response_16bit(response, 128);
    HAL_UART_Transmit(&huart1, (uint8_t*)response, strlen(response), 100);

    wifi_send_cmd_16bit("P0=0\r");  // 소켓 0 사용
    wifi_wait_ready();
    wifi_receive_response_16bit(response, 128);
    HAL_UART_Transmit(&huart1, (uint8_t*)response, strlen(response), 100);


    wifi_send_cmd_16bit("P1=0\r");  // TCP
    wifi_wait_ready();
    wifi_receive_response_16bit(response, MAX_RESPONSE_SIZE);
    HAL_UART_Transmit(&huart1, (uint8_t*)response, strlen(response), 100);


    wifi_send_cmd_16bit("P3=3.39.40.177\r");  // Remote IP
    wifi_wait_ready();
    wifi_receive_response_16bit(response, 128);
    HAL_UART_Transmit(&huart1, (uint8_t*)response, strlen(response), 100);


    wifi_send_cmd_16bit("P4=5000\r");  // Remote port
    wifi_wait_ready();
    wifi_receive_response_16bit(response, 128);
    HAL_UART_Transmit(&huart1, (uint8_t*)response, strlen(response), 100);



    //wifi_send_cmd_16bit("P?\r");
    //wifi_wait_ready();
    //wifi_receive_response_16bit(response, MAX_RESPONSE_SIZE);
    //HAL_UART_Transmit(&huart1, (uint8_t*)response, strlen(response), 100);


    // 3. TCP 연결 시작
    wifi_send_cmd_16bit("P6=1\r");
    wifi_wait_ready();
    wifi_receive_response_16bit(response, MAX_RESPONSE_SIZE);
    HAL_UART_Transmit(&huart1, (uint8_t*)response, strlen(response), 100);

    HAL_Delay(200);

    wifi_send_cmd_16bit("R1=30\r");
    wifi_wait_ready();
    //wifi_receive_response_16bit(response, MAX_RESPONSE_SIZE);
    //HAL_UART_Transmit(&huart1, (uint8_t*)response, strlen(response), 100);

    wifi_send_cmd_16bit("R1=300\r");
    wifi_wait_ready();
    wifi_receive_response_16bit(response, MAX_RESPONSE_SIZE);
    HAL_UART_Transmit(&huart1, (uint8_t*)response, strlen(response), 100);

    wifi_send_cmd_16bit("R?\r");
    wifi_wait_ready();
    wifi_receive_response_16bit(response, MAX_RESPONSE_SIZE);
    HAL_UART_Transmit(&huart1, (uint8_t*)response, strlen(response), 100);

    // --- 단발 수신으로 변경 ---
    // 서버가 계속 푸시 중이어도 여기서는 'R0' 한 번만 요청하고,
    // 도착한 버퍼를 response에 저장한 뒤 TCP를 닫는다.
    wifi_wait_ready();
    wifi_send_cmd_16bit("R0\r");                 // 수신 요청 1회
    wifi_wait_ready();

    memset(response, 0, MAX_RESPONSE_SIZE);
    wifi_receive_response_16bit(response, MAX_RESPONSE_SIZE);  // 타임아웃 포함
    if (strlen(response) > 0) {
        HAL_UART_Transmit(&huart1, (uint8_t*)response, strlen(response), 100);
    }

    // --- TCP 연결 종료 (P6=0) ---
    wifi_wait_ready();
    wifi_send_cmd_16bit("P6=0\r");               // TCP Close
    wifi_wait_ready();
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
