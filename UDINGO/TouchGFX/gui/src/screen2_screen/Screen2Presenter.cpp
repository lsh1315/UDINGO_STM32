#include <gui/screen2_screen/Screen2Presenter.hpp>
#include <gui/screen2_screen/Screen2View.hpp>
#include <gui/model/Model.hpp>
#include <stdio.h>

Screen2Presenter::Screen2Presenter(Screen2View& v)
    : Presenter(),
      view(v),
      model(nullptr)
{
}

void Screen2Presenter::button2_1Clicked()
{
    if (model) {
        model->setPreferExit(1); // 입구
        printf("PreferredExit set to %d\r\n", 1);
        fflush(stdout);
    }
}

void Screen2Presenter::button2_2Clicked()
{
    if (model) {
        model->setPreferExit(2); // 출구
        printf("PreferredExit set to %d\r\n", 2);
        fflush(stdout);
    }
}

void Screen2Presenter::button2_3Clicked()
{
    if (model) {
        model->setPreferExit(3); // 마트
        printf("PreferredExit set to %d\r\n", 3);
        fflush(stdout);
    }
}
