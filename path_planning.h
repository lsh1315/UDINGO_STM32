#ifndef PATH_PLANNING_H
#define PATH_PLANNING_H

/**
 * @brief 주어진 조건으로 최적의 주차 공간을 찾고 경로를 계산합니다.
 *
 * @param position          [입력] 시작 위치 배열 [y, x]
 * @param user_preference   [입력] 사용자 선호도 배열 [preferred_type, prefer_exit]
 * @param map_matrix        [입력] 주차장 지도 (2차원 배열)
 * @param map_rows          [입력] 지도의 세로 크기
 * @param map_cols          [입력] 지도의 가로 크기
 * @param goal              [출력] 최종 목적지 좌표가 저장될 배열 [y, x]
 * @param path              [출력] 계산된 경로 좌표가 저장될 배열 (int path[100][2])
 * @return                  계산된 경로의 길이 (좌표 쌍의 개수). 경로를 찾지 못하면 0을 반환.
 */
int path_planning(
    const int* position,
    const int* user_preference,
    int** map_matrix,
    int map_rows,
    int map_cols,
    int* goal,
    int path[][2]
);

#endif // PATH_PLANNING_H