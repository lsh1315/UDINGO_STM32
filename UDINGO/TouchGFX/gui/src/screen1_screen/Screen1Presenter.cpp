#include <gui/screen1_screen/Screen1Presenter.hpp>
#include <gui/screen1_screen/Screen1View.hpp>
#include <gui/model/Model.hpp>
#include <stdio.h>

Screen1Presenter::Screen1Presenter(Screen1View& v)
    : Presenter(),
      view(v),
      model(nullptr)
{
}

void Screen1Presenter::button1Clicked()
{
    if (model) {
        model->setPreferredType(3);        // 장애인 선택
        printf("PreferredType set to %d\r\n", 3);
        fflush(stdout);
    }
}

void Screen1Presenter::button2Clicked()
{
    if (model) {
        model->setPreferredType(5);        // 전기차 선택
        printf("PreferredType set to %d\r\n", 5);
        fflush(stdout);
    }
}

void Screen1Presenter::button3Clicked()
{
    if (model) {
        model->setPreferredType(2);        // 소형차 선택
        printf("PreferredType set to %d\r\n", 2);
        fflush(stdout);
    }
}

void Screen1Presenter::button4Clicked()
{
    if (model) {
        model->setPreferredType(4);        // 일반 선택
        printf("PreferredType set to %d\r\n", 4);
        fflush(stdout);
    }
}
