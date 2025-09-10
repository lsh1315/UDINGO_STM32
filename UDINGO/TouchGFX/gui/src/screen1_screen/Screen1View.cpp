#include <gui/screen1_screen/Screen1View.hpp>

Screen1View::Screen1View()
{
}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::button1Clicked() // 장애인
{
    presenter->button1Clicked();
    application().gotoScreen2ScreenNoTransition();
}

void Screen1View::button2Clicked() // 전기차
{
    presenter->button2Clicked();
    application().gotoScreen2ScreenNoTransition();
}

void Screen1View::button3Clicked() // 소형차
{
    presenter->button3Clicked();
    application().gotoScreen2ScreenNoTransition();
}

void Screen1View::button4Clicked() // 일반
{
    presenter->button4Clicked();
    application().gotoScreen2ScreenNoTransition();
}
