#ifndef SCREEN1VIEW_HPP
#define SCREEN1VIEW_HPP

#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>

class Screen1View : public Screen1ViewBase
{
public:
    Screen1View();
    virtual ~Screen1View() {}
    virtual void setupScreen() override;
    virtual void tearDownScreen() override;

    // Presenter와 동일한 이름의 함수를 선언하여 일관성을 유지합니다.
    // Designer에서 각 버튼을 이 함수들과 연결해주세요.
    void button1Clicked(); // 장애인
    void button2Clicked(); // 전기차
    void button3Clicked(); // 소형차
    void button4Clicked(); // 일반

protected:
private:
};

#endif // SCREEN1VIEW_HPP
