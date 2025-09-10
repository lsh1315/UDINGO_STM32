#ifndef SCREEN2PRESENTER_HPP
#define SCREEN2PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Screen2View;
class Model;

class Screen2Presenter : public Presenter, public ModelListener
{
public:
    // ★ Model 제거, View만 받음
    Screen2Presenter(Screen2View& v);

    // ★ 프레임워크 호출과 일치
    void bind(Model* m) { model = m; }

    virtual ~Screen2Presenter() {}
    virtual void activate()   override {}
    virtual void deactivate() override {}

    // 버튼 콜백
    void button2_1Clicked();   // 입구 근처
    void button2_2Clicked();   // 출구 근처
    void button2_3Clicked();   // 마트 근처

protected:
    Screen2View& view;
    Model*       model;
};

#endif // SCREEN2PRESENTER_HPP
