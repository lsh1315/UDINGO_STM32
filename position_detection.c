#include <stdio.h>
#include <math.h>
#include <stdint.h>

// Function to calculate the position using trilateration
// Returns 1 on success, 0 on failure
int trilaterate(double distances[4], uint8_t position[2]) {
    // Anchor coordinates
    double x1 = 0, y1 = 0;
    double x2 = 600, y2 = 0;
    double x3 = 0, y3 = 1200;
    double x4 = 600, y4 = 1200;

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

int update_current_position(uint8_t* position) {
    double distances[4];

    distances[0] = 63.16;
    distances[1] = 542.58;
    distances[2] = 1176.43;
    distances[3] = 1293.98;

    trilaterate(distances, position);

    return 0;
}