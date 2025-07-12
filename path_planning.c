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

// --- 내부 헬퍼 함수 선언 ---
static int heuristic(int y1, int x1, int y2, int x2);
static void find_preferred_parking(const int* user_preference, int** map_matrix, int map_rows, int map_cols, int* goal_y, int* goal_x);
static int reconstruct_path(Node** nodes, int current_y, int current_x, int path[][2], int map_cols);
static int astar(int sy, int sx, int ty, int tx, int** map_matrix, int rows, int cols, int path[][2]);


int path_planning(
    const int* position,
    const int* user_preference,
    int** map_matrix,
    int map_rows,
    int map_cols,
    int* goal,
    int path[][2]
) {
    if (!position || !user_preference || !map_matrix || !goal || !path) {
        return 0; // 유효성 검사
    }

    // 1. 최적의 주차 공간(목표 지점) 찾기
    find_preferred_parking(user_preference, map_matrix, map_rows, map_cols, &goal[0], &goal[1]);

    if (goal[0] == -1) {
        return 0; // 적절한 주차 공간을 찾지 못함
    }

    // 2. A* 알고리즘으로 경로 탐색 및 경로 길이 반환
    return astar(position[0], position[1], goal[0], goal[1], map_matrix, map_rows, map_cols, path);
}


// --- 내부 헬퍼 함수 구현 ---

static int astar(int sy, int sx, int ty, int tx, int** map_matrix, int rows, int cols, int path[][2]) {
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

static void find_preferred_parking(const int* user_preference, int** map_matrix, int map_rows, int map_cols, int* goal_y, int* goal_x) {
    int preferred_type = user_preference[0];
    int prefer_exit = user_preference[1];
    
    int min_dist = INF;
    *goal_y = -1;
    *goal_x = -1;

    int exit_pos[2] = {-1, -1}, entry_pos[2] = {-1, -1}, mall_pos[2] = {-1, -1};

    for (int r = 0; r < map_rows; r++) {
        for (int c = 0; c < map_cols; c++) {
            if (map_matrix[r][c] == 7) exit_pos[0] = r, exit_pos[1] = c;
            if (map_matrix[r][c] == 6) entry_pos[0] = r, entry_pos[1] = c;
            if (map_matrix[r][c] == 8) mall_pos[0] = r, mall_pos[1] = c;
        }
    }

    for (int r = 0; r < map_rows; r++) {
        for (int c = 0; c < map_cols; c++) {
            if (map_matrix[r][c] == preferred_type) {
                int dist = INF;
                if (prefer_exit == 1 && entry_pos[0] != -1) {
                    dist = heuristic(r, c, entry_pos[0], entry_pos[1]);
                } else if (prefer_exit == 2 && exit_pos[0] != -1) {
                    dist = heuristic(r, c, exit_pos[0], exit_pos[1]);
                } else if (prefer_exit == 3 && mall_pos[0] != -1) {
                    dist = heuristic(r, c, mall_pos[0], mall_pos[1]);
                }

                if (dist < min_dist) {
                    min_dist = dist;
                    *goal_y = r;
                    *goal_x = c;
                }
            }
        }
    }
}

static int reconstruct_path(Node** nodes, int current_y, int current_x, int path[][2], int map_cols) {
    int temp_path[100][2];
    int len = 0;

    int y = current_y;
    int x = current_x;

    while (y != -1 && x != -1) {
        if (len >= 100) return 0; // 버퍼 오버플로우 방지
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
