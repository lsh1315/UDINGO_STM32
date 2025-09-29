#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#define RX_BUFFER_SIZE 128 // 버퍼 크기는 넉넉하게 설정
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

extern uint8_t rx_buffer[RX_BUFFER_SIZE]; // 수신 데이터를 저장할 버퍼

// Function to calculate the position using trilateration
// Returns 1 on success, 0 on failure
int trilaterate(int distances[4], uint8_t position[2]) {
    // Anchor coordinates
    double x1 = 10, y1 = 10;
    double x2 = 590, y2 = 10;
    double x3 = 10, y3 = 1190;
    double x4 = 590, y4 = 1190;

    double d1 = distances[0];
    double d2 = distances[1];
    double d3 = distances[2];
    double d4 = distances[3];

    // Matrix A (6x2)
    double A[6][2] = {
        {2 * (x2 - x1), 2 * (y2 - y1)},
        {2 * (x3 - x1), 2 * (y3 - y1)},
        {2 * (x4 - x1), 2 * (y4 - y1)},
        {2 * (x3 - x2), 2 * (y3 - y2)},
        {2 * (x4 - x2), 2 * (y4 - y2)},
        {2 * (x4 - x3), 2 * (y4 - y3)}
    };

    // Vector b (6x1)
    double b[6] = {
        pow(d1, 2) - pow(d2, 2) - pow(x1, 2) + pow(x2, 2) - pow(y1, 2) + pow(y2, 2),
        pow(d1, 2) - pow(d3, 2) - pow(x1, 2) + pow(x3, 2) - pow(y1, 2) + pow(y3, 2),
        pow(d1, 2) - pow(d4, 2) - pow(x1, 2) + pow(x4, 2) - pow(y1, 2) + pow(y4, 2),
        pow(d2, 2) - pow(d3, 2) - pow(x2, 2) + pow(x3, 2) - pow(y2, 2) + pow(y3, 2),
        pow(d2, 2) - pow(d4, 2) - pow(x2, 2) + pow(x4, 2) - pow(y2, 2) + pow(y4, 2),
        pow(d3, 2) - pow(d4, 2) - pow(x3, 2) + pow(x4, 2) - pow(y3, 2) + pow(y4, 2)
    };

    // Transpose of A (A_T is 2x6)
    double A_T[2][6];
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 2; j++) {
            A_T[j][i] = A[i][j];
        }
    }

    // A_T * A (AtA is 2x2)
    double AtA[2][2] = {{0, 0}, {0, 0}};
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 6; k++) {
                AtA[i][j] += A_T[i][k] * A[k][j];
            }
        }
    }

    // A_T * b (Atb is 2x1)
    double Atb[2] = {0, 0};
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 6; j++) {
            Atb[i] += A_T[i][j] * b[j];
        }
    }

    // Inverse of AtA
    // For a 2x2 matrix [[a, b], [c, d]], the inverse is (1/det) * [[d, -b], [-c, a]]
    double det = AtA[0][0] * AtA[1][1] - AtA[0][1] * AtA[1][0];

    if (fabs(det) < 1e-6) { // Check if the determinant is close to zero
        return 0; // Cannot compute the position
    }

    double inv_det = 1.0 / det;
    double AtA_inv[2][2];
    AtA_inv[0][0] = AtA[1][1] * inv_det;
    AtA_inv[0][1] = -AtA[0][1] * inv_det;
    AtA_inv[1][0] = -AtA[1][0] * inv_det;
    AtA_inv[1][1] = AtA[0][0] * inv_det;

    // position = AtA_inv * Atb
    position[0] = (uint8_t)round((AtA_inv[0][0] * Atb[0] + AtA_inv[0][1] * Atb[1])/10); // mm to cm
    position[1] = (uint8_t)round((AtA_inv[1][0] * Atb[0] + AtA_inv[1][1] * Atb[1])/10); // mm to cm

    return 1;
}

int receive_dwm1000_distances(int distances[4]){
    
    // It will be replaced with UART Transceive
    // char str[] = "631,250,1114,949";

    char *temp;
    uint8_t i=0;
    temp = strtok(rx_buffer,",");
    while(temp != NULL){
        distances[i] = atoi(temp);
        if (distances[i] < 0) distances[i] = 0;
        temp = strtok(NULL,",");
        i++;
    }

    return 1;
}

void correction(uint8_t original_position[2], uint8_t corrected_position[2]) {
    // 직사각형의 경계 정의
    const uint8_t x_min = 9;
    const uint8_t y_min = 24;
    const uint8_t x_max = 54;
    const uint8_t y_max = 94;

    // 1단계: 점을 직사각형 영역 안으로 제한 (Clamping)
    uint8_t clamped_x = MAX(x_min, MIN(original_position[0], x_max));
    uint8_t clamped_y = MAX(y_min, MIN(original_position[1], y_max));

    // 2단계: 제한된 점에서 네 변까지의 거리를 계산
    uint8_t dist_to_left = clamped_x - x_min;
    uint8_t dist_to_right = x_max - clamped_x;
    uint8_t dist_to_top = clamped_y - y_min;
    uint8_t dist_to_bottom = y_max - clamped_y;

    // 3단계: 가장 짧은 거리를 찾기
    uint8_t min_dist = dist_to_left;
    if (dist_to_right < min_dist) min_dist = dist_to_right;
    if (dist_to_top < min_dist) min_dist = dist_to_top;
    if (dist_to_bottom < min_dist) min_dist = dist_to_bottom;

    // 4단계: 가장 가까운 변으로 투영하여 결과를 output 파라미터에 저장
    if (min_dist == dist_to_left) {
        corrected_position[0] = x_min;
        corrected_position[1] = clamped_y;
    } else if (min_dist == dist_to_right) {
        corrected_position[0] = x_max;
        corrected_position[1] = clamped_y;
    } else if (min_dist == dist_to_top) {
        corrected_position[0] = clamped_x;
        corrected_position[1] = y_min;
    } else { // min_dist == dist_to_bottom
        corrected_position[0] = clamped_x;
        corrected_position[1] = y_max;
    }
}


int update_current_position(uint8_t* position) {
    int distances[4];
    uint8_t original_position[2];

    receive_dwm1000_distances(distances);
    // Debuging
    // printf("%dmm | %dmm | %dmm | %dmm \n",distances[0],distances[1],distances[2],distances[3]);

    trilaterate(distances, original_position);

    correction(original_position, position);

    return 0;
}
