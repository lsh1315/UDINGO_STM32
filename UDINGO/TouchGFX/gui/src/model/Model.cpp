#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <stdio.h>
#include <stdint.h>

// ★★★ 이 부분을 꼭 확인해주세요! ★★★
// C++ 컴파일러에게 gui_bridge.h 안의 함수들이 C언어 함수임을 알려줍니다.
extern "C" {
#include "gui_bridge.h"
}

#define ARRAY_CAPACITY 100

extern uint8_t user_preference[2];
extern uint8_t position[2];
extern uint8_t path[ARRAY_CAPACITY][2];
extern uint8_t path_length;
extern uint8_t goal[2];

Model::Model()
    // ★ 아래 초기화 순서를 Model.hpp의 변수 선언 순서와 일치하도록 수정합니다.
    : preferredType(-1),
      preferExit(-1),
      recommendedParkingX(-1),
      recommendedParkingY(-1),
      hasNewGoal(false),
      // pathPoints 배열은 이 리스트에서 초기화할 수 없으므로 건너뜁니다.
      numPathPoints(0),
      pathUpdated(false),
      modelListener(nullptr)
{
}

void Model::tick()
{
    // --- 기존 추천 주차 공간 업데이트 로직은 그대로 둡니다 ---
    if (hasNewGoal)
    {
        hasNewGoal = false;
        if (modelListener)
        {
            modelListener->recommendedParkingChanged(recommendedParkingX, recommendedParkingY);
        }
    }

    // ★ 2. 경로점이 업데이트되었는지 확인하는 로직을 추가합니다.
    // pathUpdated 플래그는 C 코드로부터 데이터가 들어오면 true가 됩니다.
    if (pathUpdated)
    {
        pathUpdated = false; // 플래그를 다시 false로 내려서 반복 실행을 방지합니다.
        if (modelListener)
        {
            // Presenter에게 "경로 데이터가 바뀌었으니 화면을 업데이트해!" 라고 신호를 보냅니다.
            modelListener->pathUpdated();
        }
    }
}

void Model::sendPreferencesToMCU()
{
    if (preferredType == -1 || preferExit == -1) {
        return;
    }

    user_preference[0] = this->preferredType;
    user_preference[1] = this->preferExit;


}
