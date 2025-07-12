#include "path_planning.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h> // abs()를 위해 포함

#define INF 99999

typedef struct {
    int y, x;
    int g, f;
    int parent_y, parent_x;
} Node;

// --- 헬퍼 함수 선언 (static 제거) ---
static int heuristic(int y1, int x1, int y2, int x2);
static int reconstruct_path(Node** nodes, int current_y, int current_x, int path[][2], int map_cols);

// find_preferred_parking 함수 정의 (static 제거 및 인자 추가)
void find_preferred_parking(
    const int* user_preference,
    int** map_matrix,
    int map_rows,
    int map_cols,
    int** parking_spot,
    int num_spot,
    int* empty,
    int* goal
) {
    int preferred_type = user_preference[0];
    int prefer_criteria = user_preference[1]; // 1: 입구 근처, 2: 출구 근처, 3: 마트 출입구 근처
    
    int min_dist = INF;
    goal[0] = -1;
    goal[1] = -1;

    int exit_pos[2] = {-1, -1}, entry_pos[2] = {-1, -1}, mall_pos[2] = {-1, -1};

    // 출구, 입구, 마트 출입구 위치 찾기
    for (int r = 0; r < map_rows; r++) {
        for (int c = 0; c < map_cols; c++) {
            if (map_matrix[r][c] == 7) exit_pos[0] = r, exit_pos[1] = c; // 출구
            if (map_matrix[r][c] == 6) entry_pos[0] = r, entry_pos[1] = c; // 입구
            if (map_matrix[r][c] == 8) mall_pos[0] = r, mall_pos[1] = c; // 마트 출입구
        }
    }

    // parking_spot 배열을 순회하며 최적의 주차 공간 찾기
    for (int i = 0; i < num_spot; i++) {
        int spot_y = parking_spot[i][0];
        int spot_x = parking_spot[i][1];
        int spot_type = parking_spot[i][2]; // parking_spot 배열의 세 번째 요소가 주차 공간 유형이라고 가정

        // 비어있는지 확인 (empty 배열은 현재 사용되지 않으므로, 이 부분은 향후 구현 필요)
        // if (empty[i] == 0) continue; // 비어있지 않으면 건너뛰기

        if (spot_type == preferred_type) {
            int dist = INF;
            if (prefer_criteria == 1 && entry_pos[0] != -1) {
                dist = heuristic(spot_y, spot_x, entry_pos[0], entry_pos[1]);
            } else if (prefer_criteria == 2 && exit_pos[0] != -1) {
                dist = heuristic(spot_y, spot_x, exit_pos[0], exit_pos[1]);
            } else if (prefer_criteria == 3 && mall_pos[0] != -1) {
                dist = heuristic(spot_y, spot_x, mall_pos[0], mall_pos[1]);
            }

            if (dist < min_dist) {
                min_dist = dist;
                goal[0] = spot_y;
                goal[1] = spot_x;
            }
        }
    }
}

// astar 함수 정의 (static 제거 및 인자 변경)
int astar(const int* start_pos, const int* target_pos, int** map_matrix, int rows, int cols, int path[][2]) {
    int sy = start_pos[0];
    int sx = start_pos[1];
    int ty = target_pos[0];
    int tx = target_pos[1];

    Node** nodes = (Node**)malloc(rows * sizeof(Node*));
    for(int i = 0; i < rows; i++) {
        nodes[i] = (Node*)malloc(cols * sizeof(Node));
    }

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            nodes[r][c] = (Node){.y = r, .x = c, .g = INF, .f = INF, .parent_y = -1, .parent_x = -1};
        }
    }

    Node start_node = {.y = sy, .x = sx, .g = 0, .f = heuristic(sy, sx, ty, tx), .parent_y = -1, .parent_x = -1};
    nodes[sy][sx] = start_node;

    Node* open_list = (Node*)malloc(rows * cols * sizeof(Node));
    int open_size = 0;
    open_list[open_size++] = start_node;

    int* in_open_list = (int*)calloc(rows * cols, sizeof(int));
    in_open_list[sy * cols + sx] = 1;

    int path_len = 0;

    while (open_size > 0) {
        int current_idx = 0;
        for (int i = 1; i < open_size; i++) {
            if (open_list[i].f < open_list[current_idx].f) {
                current_idx = i;
            }
        }
        Node current = open_list[current_idx];
        open_list[current_idx] = open_list[open_size - 1];
        open_size--;

        if (current.y == ty && current.x == tx) {
            path_len = reconstruct_path(nodes, ty, tx, path, cols);
            break; // 경로 찾았으므로 루프 종료
        }

        int dy[] = {-1, 1, 0, 0};
        int dx[] = {0, 0, -1, 1};

        for (int i = 0; i < 4; i++) {
            int ny = current.y + dy[i];
            int nx = current.x + dx[i];

            if (ny < 0 || ny >= rows || nx < 0 || nx >= cols || map_matrix[ny][nx] == 1) {
                continue;
            }

            int tentative_g_score = current.g + 1;

            if (tentative_g_score < nodes[ny][nx].g) {
                nodes[ny][nx].parent_y = current.y;
                nodes[ny][nx].parent_x = current.x;
                nodes[ny][nx].g = tentative_g_score;
                nodes[ny][nx].f = tentative_g_score + heuristic(ny, nx, ty, tx);
                
                if (!in_open_list[ny * cols + nx]) {
                    open_list[open_size++] = nodes[ny][nx];
                    in_open_list[ny * cols + nx] = 1;
                }
            }
        }
    }
    
    for(int i = 0; i < rows; i++) free(nodes[i]);
    free(nodes);
    free(open_list);
    free(in_open_list);

    return path_len;
}

static int heuristic(int y1, int x1, int y2, int x2) {
    return abs(y1 - y2) + abs(x1 - x2);
}

static int reconstruct_path(Node** nodes, int current_y, int current_x, int path[][2], int map_cols) {
    int temp_path[ARRAY_CAPACITY][2]; // ARRAY_CAPACITY 사용
    int len = 0;

    int y = current_y;
    int x = current_x;

    while (y != -1 && x != -1) {
        if (len >= ARRAY_CAPACITY) return 0; // 버퍼 오버플로우 방지
        temp_path[len][0] = y;
        temp_path[len][1] = x;
        len++;
        Node current_node = nodes[y][x];
        y = current_node.parent_y;
        x = current_node.parent_x;
    }

    for (int i = 0; i < len; i++) {
        path[i][0] = temp_path[len - i - 1][0];
        path[i][1] = temp_path[len - i - 1][1];
    }
    return len;
}