#ifndef MAP_DATA_H
#define MAP_DATA_H

/**
 * @brief lot_number에 해당하는 주차장 도면의 복사본을 생성합니다.
 * @param lot_number 복사할 주차장 번호
 * @param out_rows [출력] 생성된 맵의 세로 크기가 저장될 변수의 주소
 * @param out_cols [출력] 생성된 맵의 가로 크기가 저장될 변수의 주소
 * @return 성공 시 동적으로 할당된 2차원 배열(int**), 실패 시 NULL
 */
int** create_map_copy(int lot_number, int* out_rows, int* out_cols);

/**
 * @brief create_map_copy로 생성된 2차원 배열의 메모리를 해제합니다.
 * @param map 해제할 2차원 배열
 * @param rows 해당 배열의 세로 크기 (메모리 해제에 필요)
 */
void free_map_copy(int** map, int rows);

/**
 * @brief lot_number에 해당하는 주차장의 주차구역 복사본을 생성합니다.
 * @param lot_number 복사할 주차장 번호
 * @param num [출력] 생성된 좌표들 개수가 저장될 변수의 주소
 * @return 성공 시 동적으로 할당된 2차원 배열(int**), 실패 시 NULL
 */
int** create_spot_copy(int lot_number, int* num);

/**
 * @brief create_spot_copy로 생성된 2차원 배열의 메모리를 해제합니다.
 * @param spot 해제할 2차원 배열
 * @param num 해당 배열의 세로 크기 (메모리 해제에 필요)
 */
void free_spot_copy(int** spot, int num);

#endif // MAP_DATA_H
