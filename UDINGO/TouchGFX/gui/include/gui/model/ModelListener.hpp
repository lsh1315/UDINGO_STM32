#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

class Model;

class ModelListener
{
public:
    ModelListener() : model(0) {}
    virtual ~ModelListener() {}

    void setModel(Model* m)
    {
        model = m;
    }

    // 기존 함수
    virtual void recommendedParkingChanged(int x, int y) {}

    // ★★★ 이 함수 선언이 누락되었습니다! 꼭 추가해주세요. ★★★
    // Model에서 경로점이 업데이트되었음을 Presenter에게 알리는 신호입니다.
    virtual void pathUpdated() {}

protected:
    Model* model;
};

#endif // MODELLISTENER_HPP // <-- 오타 수정: MODELLER_HPP -> MODELLISTENER_HPP
