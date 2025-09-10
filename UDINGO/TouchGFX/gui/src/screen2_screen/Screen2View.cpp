#include <gui/screen2_screen/Screen2View.hpp>

Screen2View::Screen2View()
{
}

void Screen2View::setupScreen()
{
    Screen2ViewBase::setupScreen();
}

void Screen2View::tearDownScreen()
{
    Screen2ViewBase::tearDownScreen();
}

// 입구 근처 버튼 클릭 시
void Screen2View::button2_1Clicked()
{
    presenter->button2_1Clicked();
    application().gotoScreen3ScreenNoTransition(); // 최종 화면으로 전환
}

// 출구 근처 버튼 클릭 시
void Screen2View::button2_2Clicked()
{
    presenter->button2_2Clicked();
    application().gotoScreen3ScreenNoTransition();
}

// 마트 근처 버튼 클릭 시
void Screen2View::button2_3Clicked()
{
    presenter->button2_3Clicked();
    application().gotoScreen3ScreenNoTransition();
}
