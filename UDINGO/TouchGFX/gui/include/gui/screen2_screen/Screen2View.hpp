#ifndef SCREEN2VIEW_HPP
#define SCREEN2VIEW_HPP

#include <gui_generated/screen2_screen/Screen2ViewBase.hpp>
#include <gui/screen2_screen/Screen2Presenter.hpp>

class Screen2View : public Screen2ViewBase
{
public:
    Screen2View();
    virtual ~Screen2View() {}
    virtual void setupScreen() override;
    virtual void tearDownScreen() override;

    // Designer의 버튼 Interaction과 연결될 함수들
    void button2_1Clicked(); // 입구 근처
    void button2_2Clicked(); // 출구 근처
    void button2_3Clicked(); // 마트 근처

protected:
private:
};

#endif // SCREEN2VIEW_HPP
