// in Application/User/gui/ModelBridge.cpp

#include "gui_bridge.h"
#include <gui/model/Model.hpp>

// 1. TouchGFX의 핵심인 FrontendHeap 헤더를 포함합니다.
#include <gui/common/FrontendHeap.hpp>

// 'extern Model model;' 선언은 이제 필요 없으므로 삭제합니다.

void gui_update_recommended_parking(int x, int y)
{
    // 2. FrontendHeap 싱글톤을 통해 유일하고 올바른 model 객체의 참조를 가져옵니다.
    Model& model = FrontendHeap::getInstance().model;

    // 3. 이제 이 코드는 GUI가 사용하는 진짜 model 객체를 업데이트합니다.
    model.recommendedParkingX = x;
    model.recommendedParkingY = y;
    model.hasNewGoal = true;  // tick()에서 정상적으로 감지됩니다.
}
