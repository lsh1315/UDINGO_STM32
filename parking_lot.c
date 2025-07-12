#include "parking_lot.h"

#include <stdio.h>
#include <stdlib.h> // malloc, free를 위해 필수

// --- 내부 데이터 구조 (외부에 노출되지 않음) ---
typedef struct {
    int rows;
    int cols;
    const int* map_data_ptr;
} InternalMapInfo;

// --- 원본 데이터 (static으로 완벽히 숨김) ---
static const int parking_lot_1_map[5][7] = {
    {1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1}
};
static const int parking_lot_2_map[8][4] = {
    {1, 1, 1, 1},
    {1, 0, 0, 1},
    {1, 0, 0, 1},
    {1, 1, 0, 1},
    {1, 0, 0, 1},
    {1, 0, 0, 1},
    {1, 0, 0, 1},
    {1, 1, 1, 1}
};

static const InternalMapInfo all_maps[] = {
    {5, 7, (const int*)parking_lot_1_map},
    {8, 4, (const int*)parking_lot_2_map}
};

// --- 외부에 공개되는 기능 함수들 ---

int** create_map_copy(int lot_number, int* out_rows, int* out_cols) {
    int num_lots = sizeof(all_maps) / sizeof(all_maps[0]);
    if (lot_number < 0 || lot_number >= num_lots) {
        return NULL; // 유효하지 않은 lot 번호
    }

    const InternalMapInfo* info = &all_maps[lot_number];
    int rows = info->rows;
    int cols = info->cols;

    // 1. 세로 (row) 포인터들을 담을 공간 할당
    int** new_map = (int**)malloc(rows * sizeof(int*));
    if (new_map == NULL) return NULL; // 메모리 할당 실패

    // 2. 각 가로 (col) 배열을 위한 공간을 할당하고 데이터 복사
    for (int r = 0; r < rows; r++) {
        new_map[r] = (int*)malloc(cols * sizeof(int));
        if (new_map[r] == NULL) {
            // 할당 실패 시, 그 전까지 할당된 모든 메모리를 해제해야 함
            for (int i = 0; i < r; i++) {
                free(new_map[i]);
            }
            free(new_map);
            return NULL;
        }
        // 데이터 복사
        for (int c = 0; c < cols; c++) {
            new_map[r][c] = info->map_data_ptr[r * cols + c];
        }
    }

    *out_rows = rows;
    *out_cols = cols;
    return new_map;
}

void free_map_copy(int** map, int rows) {
    if (map == NULL) return;

    // 각 row에 할당된 메모리를 먼저 해제
    for (int r = 0; r < rows; r++) {
        free(map[r]);
    }
    // row 포인터 배열 자체를 해제
    free(map);
}
