#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#define ARRAY_CAPACITY 100
#define INF 65535 // f, g 값의 최대값 (uint16_t에 맞춤)

// ---------- 상태 정의 ----------
#define STATUS_UNVISITED 0
#define STATUS_OPEN 1
#define STATUS_CLOSED 2

// ---------- 구조체 (최적화) ----------
typedef struct {
    uint16_t g;       // 경로 비용
    uint16_t f;       // 총 예상 비용 (g + h)
    int parent_idx; // 부모 노드의 인덱스
} Node;

// ---------- 휴리스틱 함수 ----------
static int heuristic(int y1, int x1, int y2, int x2) {
    return abs(y1 - y2) + abs(x1 - x2);
}

// ---------- 경로 재구성 (인덱스 기반) ----------
static int reconstruct_path(Node* nodes, int current_idx, uint8_t path[][2], int map_cols) {
    uint8_t temp_path[ARRAY_CAPACITY][2];
    int len = 0;
    int idx = current_idx;

    while (idx != -1) {
        if (len >= ARRAY_CAPACITY) return 0; // 경로 버퍼 오버플로우 방지
        temp_path[len][0] = (uint8_t)(idx / map_cols); // y 좌표
        temp_path[len][1] = (uint8_t)(idx % map_cols); // x 좌표
        len++;
        idx = nodes[idx].parent_idx;
    }

    // 경로를 역순으로 다시 저장
    for (int i = 0; i < len; i++) {
        path[i][0] = temp_path[len - i - 1][0];
        path[i][1] = temp_path[len - i - 1][1];
    }
    return len;
}

// ---------- A* 알고리즘 (메모리 최적화) ----------
int astar(const uint8_t* start_pos, const uint8_t* target_pos, uint8_t** map_matrix, uint8_t rows, uint8_t cols, uint8_t path[][2]) {
    int map_size = rows * cols;
    int sy = start_pos[0], sx = start_pos[1];
    int ty = target_pos[0], tx = target_pos[1];
    int start_idx = sy * cols + sx;
    int target_idx = ty * cols + tx;

    // 1. 노드 정보 배열 (1D)
    Node* nodes = (Node*)malloc(map_size * sizeof(Node));
    if (!nodes) return 0;

    // 2. 상태 관리 배열 (미방문/열림/닫힘)
    uint8_t* status = (uint8_t*)calloc(map_size, sizeof(uint8_t));
    if (!status) {
        free(nodes);
        return 0;
    }

    // 3. Open List (노드 인덱스만 저장)
    int* open_list = (int*)malloc(map_size * sizeof(int));
    if (!open_list) {
        free(nodes);
        free(status);
        return 0;
    }

    for (int i = 0; i < map_size; i++) {
        nodes[i].g = INF;
        nodes[i].f = INF;
        nodes[i].parent_idx = -1;
    }

    nodes[start_idx].g = 0;
    nodes[start_idx].f = heuristic(sy, sx, ty, tx);

    int open_size = 0;
    open_list[open_size++] = start_idx;
    status[start_idx] = STATUS_OPEN;

    int path_len = 0;

    while (open_size > 0) {
        int current_open_idx = 0;
        for (int i = 1; i < open_size; i++) {
            if (nodes[open_list[i]].f < nodes[open_list[current_open_idx]].f) {
                current_open_idx = i;
            }
        }

        int current_idx = open_list[current_open_idx];

        open_list[current_open_idx] = open_list[--open_size];
        status[current_idx] = STATUS_CLOSED;

        if (current_idx == target_idx) {
            path_len = reconstruct_path(nodes, target_idx, path, cols);
            break;
        }

        int cy = current_idx / cols;
        int cx = current_idx % cols;

        int dy[] = {-1, 1, 0, 0};
        int dx[] = {0, 0, -1, 1};

        for (int i = 0; i < 4; i++) {
            int ny = cy + dy[i];
            int nx = cx + dx[i];

            if (ny < 0 || ny >= rows || nx < 0 || nx >= cols || map_matrix[ny][nx] == 1) {
                continue;
            }

            int neighbor_idx = ny * cols + nx;

            if (status[neighbor_idx] == STATUS_CLOSED) {
                continue;
            }

            uint16_t tentative_g_score = nodes[current_idx].g + 1;

            if (tentative_g_score < nodes[neighbor_idx].g) {
                nodes[neighbor_idx].parent_idx = current_idx;
                nodes[neighbor_idx].g = tentative_g_score;
                nodes[neighbor_idx].f = tentative_g_score + heuristic(ny, nx, ty, tx);

                if (status[neighbor_idx] != STATUS_OPEN) {
                    open_list[open_size++] = neighbor_idx;
                    status[neighbor_idx] = STATUS_OPEN;
                }
            }
        }
    }

    free(nodes);
    free(status);
    free(open_list);

    return path_len;
}

// ---------- 주차 자리 선택 (기존 로직 유지) ----------
void find_preferred_parking(
    const uint8_t* user_preference,
    uint8_t** map_matrix,
    uint8_t map_rows,
    uint8_t map_cols,
    uint8_t* goal
) {
    int preferred_type = user_preference[0];
    int prefer_criteria = user_preference[1];

    int min_dist = INF;
    goal[0] = goal[1] = 255; // uint8_t에 맞게 초기화

    int entry_pos[2] = { -1, -1 }, exit_pos[2] = { -1, -1 }, mall_pos[2] = { -1, -1 };

    for (int r = 0; r < map_rows; r++) {
        for (int c = 0; c < map_cols; c++) {
            if (map_matrix[r][c] == 6) { entry_pos[0] = r; entry_pos[1] = c; }
            if (map_matrix[r][c] == 7) { exit_pos[0] = r; exit_pos[1] = c; }
            if (map_matrix[r][c] == 8 && mall_pos[0] == -1) { mall_pos[0] = r; mall_pos[1] = c; }
        }
    }

    int found_preferred = 0;
    for (int r = 0; r < map_rows; r++) {
        for (int c = 0; c < map_cols; c++) {
            int type = map_matrix[r][c];
            if (type == preferred_type) {
                int dist = INF;
                if (prefer_criteria == 1 && entry_pos[0] != -1)
                    dist = heuristic(r, c, entry_pos[0], entry_pos[1]);
                else if (prefer_criteria == 2 && exit_pos[0] != -1)
                    dist = heuristic(r, c, exit_pos[0], exit_pos[1]);
                else if (prefer_criteria == 3 && mall_pos[0] != -1)
                    dist = heuristic(r, c, mall_pos[0], mall_pos[1]);

                if (dist < min_dist) {
                    min_dist = dist;
                    goal[0] = r;
                    goal[1] = c;
                    found_preferred = 1;
                }
            }
        }
    }

    if (!found_preferred) {
        min_dist = INF;
        for (int r = 0; r < map_rows; r++) {
            for (int c = 0; c < map_cols; c++) {
                if (map_matrix[r][c] == 4) { // 일반 주차 공간
                    int dist = INF;
                    if (prefer_criteria == 1 && entry_pos[0] != -1)
                        dist = heuristic(r, c, entry_pos[0], entry_pos[1]);
                    else if (prefer_criteria == 2 && exit_pos[0] != -1)
                        dist = heuristic(r, c, exit_pos[0], exit_pos[1]);
                    else if (prefer_criteria == 3 && mall_pos[0] != -1)
                        dist = heuristic(r, c, mall_pos[0], mall_pos[1]);

                    if (dist < min_dist) {
                        min_dist = dist;
                        goal[0] = r;
                        goal[1] = c;
                    }
                }
            }
        }
    }
}
