#include <gui/screen3_screen/Screen3Presenter.hpp>
#include <gui/screen3_screen/Screen3View.hpp>
#include <gui/model/Model.hpp>
#include <stdio.h>

Screen3Presenter::Screen3Presenter(Screen3View& v)
    : view(v),
      model(nullptr)   // ★ 초기화
{
}

void Screen3Presenter::activate()
{
    if (model)
    {
        model->bind(this);
    }
    // 화면이 활성화되면 Model에 계산 요청
    if (model)
    {
        model->sendPreferencesToMCU();
    }
}

void Screen3Presenter::deactivate()
{
    // 필요시 해제 처리, 현재는 비워둠
}

void Screen3Presenter::recommendedParkingChanged(int x, int y)
{
    printf("[Presenter] recommendedParkingChanged called (%d,%d)\r\n", x, y); // <— 여기에 찍히는지 확인
    view.updateRecommendedParking(x, y);
}

void Screen3Presenter::pathUpdated()
{
    printf("[Presenter] Event received: pathUpdated\r\n");
    if (model)
    {
        view.updatePathPoints(model->pathPoints, model->numPathPoints);
    }
}
