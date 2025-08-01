#ifndef PATH_PLANNING_H
#define PATH_PLANNING_H

/**
 * @brief 주어진 조건으로 최적의 주차 공간을 찾습니다.
 *
 * @param user_preference   [입력] 사용자 선호도 배열 [preferred_type, prefer_exit]
 * @param map_matrix        [입력] 주차장 지도 (2차원 배열)
 * @param map_rows          [입력] 지도의 세로 크기
 * @param map_cols          [입력] 지도의 가로 크기
 * @param parking_spot      [입력] 주차 구역 좌표들
 * @param num_spot          [입력] 주차 구역 개수
 * @param empty             [입력] 빈 주차 공간 목록 (현재 사용되지 않음)
 * @param goal              [출력] 최종 목적지 좌표가 저장될 배열 [y, x]
 */
void find_preferred_parking(
    const uint8_t* user_preference,
    uint8_t** map_matrix,
    uint8_t map_rows,
    uint8_t map_cols,
    uint8_t* goal
);

/**
 * @brief A* 알고리즘을 사용하여 최단 경로를 계산합니다.
 *
 * @param start_pos         [입력] 시작 위치 배열 [y, x]
 * @param target_pos        [입력] 목적지 위치 배열 [y, x]
 * @param map_matrix        [입력] 주차장 지도 (2차원 배열)
 * @param rows              [입력] 지도의 세로 크기
 * @param cols              [입력] 지도의 가로 크기
 * @param path              [출력] 계산된 경로 좌표가 저장될 배열 (int path[ARRAY_CAPACITY][2])
 * @return                  계산된 경로의 길이 (좌표 쌍의 개수). 경로를 찾지 못하면 0을 반환.
 */
int astar(
    const uint8_t* start_pos,
    const uint8_t* target_pos,
    uint8_t** map_matrix,
    uint8_t rows,
    uint8_t cols,
    uint8_t path[][2]
);

#endif // PATH_PLANNING_H