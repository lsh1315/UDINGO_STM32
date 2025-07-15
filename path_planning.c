#include <stdio.h>
#include <stdlib.h>
#include <math.h> // abs()를 위해 포함
#include <stdint.h> // uint8_t 정의

#define ARRAY_CAPACITY 100 // 경로 및 빈자리 배열의 최대 크기 정의
#define INF 99999

// ---------- 구조체 ----------
typedef struct {
    int y, x;
    int g, f;
    int parent_y, parent_x;
} Node;

// ---------- 휴리스틱 함수 ----------
static int heuristic(int y1, int x1, int y2, int x2) {
    return abs(y1 - y2) + abs(x1 - x2);
}

// ---------- 경로 재구성 ----------
static int reconstruct_path(Node** nodes, int current_y, int current_x, int path[][2], int map_cols) {
    int temp_path[ARRAY_CAPACITY][2];
    int len = 0;
    int y = current_y;
    int x = current_x;

    while (y != -1 && x != -1) {
        if (len >= ARRAY_CAPACITY) return 0;
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

// ---------- A* 알고리즘 ----------
int astar(const int* start_pos, const int* target_pos, uint8_t** map_matrix, int rows, int cols, int path[][2]) {
    int sy = start_pos[0];
    int sx = start_pos[1];
    int ty = target_pos[0];
    int tx = target_pos[1];

    Node** nodes = (Node**)malloc(rows * sizeof(Node*));
    for (int i = 0; i < rows; i++) {
        nodes[i] = (Node*)malloc(cols * sizeof(Node));
        for (int j = 0; j < cols; j++) {
            nodes[i][j] = (Node){ .y = i, .x = j, .g = INF, .f = INF, .parent_y = -1, .parent_x = -1 };
        }
    }

    Node start_node = { .y = sy, .x = sx, .g = 0, .f = heuristic(sy, sx, ty, tx), .parent_y = -1, .parent_x = -1 };
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
        open_list[current_idx] = open_list[--open_size];

        if (current.y == ty && current.x == tx) {
            path_len = reconstruct_path(nodes, ty, tx, path, cols);
            break;
        }

        int dy[4] = { -1, 1, 0, 0 };
        int dx[4] = { 0, 0, -1, 1 };

        for (int i = 0; i < 4; i++) {
            int ny = current.y + dy[i];
            int nx = current.x + dx[i];

            if (ny < 0 || ny >= rows || nx < 0 || nx >= cols || map_matrix[ny][nx] == 1)
                continue;

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

    for (int i = 0; i < rows; i++) free(nodes[i]);
    free(nodes);
    free(open_list);
    free(in_open_list);

    return path_len;
}

// ---------- 주차 자리 선택 ----------
void find_preferred_parking(
    const int* user_preference,
    uint8_t** map_matrix,
    int map_rows,
    int map_cols,
    int* goal
) {
    int preferred_type = user_preference[0];
    int prefer_criteria = user_preference[1];

    int min_dist = INF;
    goal[0] = goal[1] = -1;

    int entry_pos[2] = { -1, -1 }, exit_pos[2] = { -1, -1 }, mall_pos[2] = { -1, -1 };

    for (int r = 0; r < map_rows; r++) {
        for (int c = 0; c < map_cols; c++) {
            if (map_matrix[r][c] == 6) entry_pos[0] = r, entry_pos[1] = c;
            if (map_matrix[r][c] == 7) exit_pos[0] = r, exit_pos[1] = c;
            if (map_matrix[r][c] == 8 && mall_pos[0] == -1) mall_pos[0] = r, mall_pos[1] = c;
        }
    }
    int found_preferred = 0; // 경차/장애인 자리 다 찼을때 일반자리에서 찾게하는 로직위한 변수
    // 주차 공간을 map_matrix에서 직접 찾습니다.
    for (int r = 0; r < map_rows; r++) {
        for (int c = 0; c < map_cols; c++) {
            int type = map_matrix[r][c];
            // 주차 공간 타입 (예: 2, 3, 4, 5)에 따라 처리
            if (type >= 2 && type <= 5) { // 가정: 2~5는 주차 공간 타입
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
                        found_preferred = 1; // 경차/장애인 자리 다 찼을때 일반자리에서 찾게하는 로직위한 변수
                    }
                }
            }
        }
    }
    // 선호 주차 타입이 다 찼다면 일반 타입(4번) 중 추천
    if (!found_preferred) {
        min_dist = INF;
        for (int r = 0; r < map_rows; r++) {
            for (int c = 0; c < map_cols; c++) {
                if (map_matrix[r][c] == 4) {
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
