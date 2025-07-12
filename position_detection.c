#include "position_detection.h"

#include <stdio.h>
#include <math.h>

// Function to calculate the position using trilateration
// Returns 1 on success, 0 on failure
int trilaterate(double distances[4], int position[2]) {
    // Anchor coordinates
    double x1 = 0, y1 = 0;
    double x2 = 1200, y2 = 0;
    double x3 = 0, y3 = 600;
    double x4 = 1200, y4 = 600;

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
    position[0] = round(AtA_inv[0][0] * Atb[0] + AtA_inv[0][1] * Atb[1]);
    position[1] = round(AtA_inv[1][0] * Atb[0] + AtA_inv[1][1] * Atb[1]);

    return 1;
}

int update_current_position(int* position) {
    double distances[4];

    printf("\n4개의 앵커로부터 태거까지의 거리 값을 입력하세요.\n");

    printf("앵커 1 (0, 0)까지의 거리: ");
    if (scanf("%lf", &distances[0]) != 1) {
        printf("\n잘못된 입력입니다. 숫자 값을 입력해야 합니다.\n");
        return 1;
    }

    printf("앵커 2 (1200, 0)까지의 거리: ");
    if (scanf("%lf", &distances[1]) != 1) {
        printf("\n잘못된 입력입니다. 숫자 값을 입력해야 합니다.\n");
        return 1;
    }

    printf("앵커 3 (0, 600)까지의 거리: ");
    if (scanf("%lf", &distances[2]) != 1) {
        printf("\n잘못된 입력입니다. 숫자 값을 입력해야 합니다.\n");
        return 1;
    }

    printf("앵커 4 (1200, 600)까지의 거리: ");
    if (scanf("%lf", &distances[3]) != 1) {
        printf("\n잘못된 입력입니다. 숫자 값을 입력해야 합니다.\n");
        return 1;
    }

    if (trilaterate(distances, position)) {
        printf("\n계산된 태거의 위치: (x=%d, y=%d)\n", position[0], position[1]);
    } else {
        printf("\n위치를 계산할 수 없습니다. 입력 값을 확인해주세요.\n");
    }

    return 0;
}