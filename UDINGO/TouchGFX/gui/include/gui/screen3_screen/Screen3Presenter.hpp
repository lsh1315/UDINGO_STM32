#ifndef SCREEN3PRESENTER_HPP
#define SCREEN3PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Screen3View;
class Model;

class Screen3Presenter : public Presenter, public ModelListener
{
public:
    Screen3Presenter(Screen3View& v);
    virtual ~Screen3Presenter() {}

    // ★ FrontendApplicationBase에서 자동으로 호출됨
    void bind(Model* m) { model = m; }

    virtual void activate() override;
    virtual void deactivate() override;

    // Model → Presenter → View 전달
    virtual void recommendedParkingChanged(int x, int y) override;
    virtual void pathUpdated() override;


private:
    Screen3View& view;
    Model*       model;  // ★ 명시적으로 멤버 추가
};

#endif // SCREEN3PRESENTER_HPP
