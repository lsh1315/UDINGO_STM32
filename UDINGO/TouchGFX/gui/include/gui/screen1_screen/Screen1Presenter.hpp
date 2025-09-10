#ifndef SCREEN1PRESENTER_HPP
#define SCREEN1PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Screen1View;
class Model;

class Screen1Presenter : public Presenter, public ModelListener
{
public:
    // ★ Model& 제거: View만 받도록
    Screen1Presenter(Screen1View& v);

    // ★ 프레임워크 호출과 일치: 포인터 시그니처
    void bind(Model* m) { model = m; }

    virtual ~Screen1Presenter() {}
    virtual void activate()   override {}
    virtual void deactivate() override {}

    void button1Clicked();  // 장애인
    void button2Clicked();  // 전기차
    void button3Clicked();  // 소형차
    void button4Clicked();  // 일반

private:
    Screen1View& view;
    Model*       model;   // bind(Model*)로 세팅됨
};

#endif // SCREEN1PRESENTER_HPP
