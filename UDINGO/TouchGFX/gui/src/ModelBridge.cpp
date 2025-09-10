#include "gui_bridge.h"
#include <gui/model/Model.hpp>


// FrontendApplication.cpp에 정의된 전역 포인터를 사용하겠다고 선언합니다.
extern Model* globalModelPtr;

// --- 기존 함수는 그대로 둡니다 ---
void gui_update_recommended_parking(int x, int y)
{
    if (globalModelPtr)
    {
        globalModelPtr->recommendedParkingX = x;
        globalModelPtr->recommendedParkingY = y;
        globalModelPtr->hasNewGoal = true;
    }
}

void gui_update_path_points(const uint8_t* points, int count)
{
    // globalModelPtr이 유효할 때만 실행 (안전장치)
    if (globalModelPtr)
    {
        // 2. 전달받은 점의 개수가 Model이 저장할 수 있는 최대치(40개)를 넘지 않도록 합니다.
        int numToCopy = (count < Model::MAX_PATH_POINTS) ? count : Model::MAX_PATH_POINTS;
        globalModelPtr->numPathPoints = numToCopy;

        // 3. C에서 넘어온 배열의 데이터를 Model의 pathPoints 배열로 복사합니다.
        for (int i = 0; i < numToCopy; ++i)
        {
            // C의 path[i][0]는 y, path[i][1]는 x 입니다.
            // C 배열은 메모리상에서 [y0, x0, y1, x1, y2, x2, ...] 형태로 연속되어 있습니다.
            globalModelPtr->pathPoints[i].y = points[i * 2 + 0]; // 짝수 인덱스 = y
            globalModelPtr->pathPoints[i].x = points[i * 2 + 1]; // 홀수 인덱스 = x
        }

        // 4. Model의 tick() 함수가 변화를 감지할 수 있도록 플래그를 true로 켭니다.
        globalModelPtr->pathUpdated = true;
    }
}
