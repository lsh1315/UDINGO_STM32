#ifndef SERVER_COMMUNICATION_H
#define SERVER_COMMUNICATION_H

/**
 * @brief 서버로부터 주차장 점유 정보를 받아와 전체 맵을 업데이트하는 메인 함수.
 *
 * 이 함수는 서버에 점유 정보를 요청하고, 응답받은 좌표 문자열을 파싱하여
 * 내부 주차장 맵에 점유 상태(1)를 반영합니다.
 *
 * @param map 업데이트할 주차장 맵 (동적으로 할당된 2차원 배열).
 * @return 성공 시 1, 실패(서버 통신 오류, 메모리 할당 실패 등) 시 0을 반환합니다.
 */
int update_parking_occupancy(int **map);

#endif // SERVER_COMMUNICATION_H
