#ifndef SCREEN3VIEW_HPP
#define SCREEN3VIEW_HPP

#include <gui_generated/screen3_screen/Screen3ViewBase.hpp>
#include <gui/screen3_screen/Screen3Presenter.hpp>

// ★ 1. 필요한 헤더 파일들을 추가합니다.
#include <touchgfx/widgets/Image.hpp> // Designer에서 추가한 Image 위젯을 제어하기 위해
#include <gui/model/Model.hpp>      // Model.hpp에 정의한 PathPoint 구조체를 사용하기 위해

class Screen3View : public Screen3ViewBase
{
public:
    Screen3View();
    virtual ~Screen3View() {}
    virtual void setupScreen() override;
    virtual void tearDownScreen() override;

    void updateRecommendedParking(int x, int y);

    // ★ 2. Presenter가 호출할 경로점 업데이트 함수를 새로 선언합니다.
    void updatePathPoints(const PathPoint* points, int count);

protected:
    // ★ 3. Designer에서 추가한 40개의 Image 위젯을 가리킬 포인터 배열을 선언합니다.
    // 이 배열을 통해 코드에서 pathPoint_0, pathPoint_1, ... 위젯들을 쉽게 제어할 수 있습니다.
    touchgfx::Image* pathPointWidgets[Model::MAX_PATH_POINTS];
    // ★ 이전 차량 위치를 저장할 변수
    int prev_car_x;
    int prev_car_y;
    bool is_first_update;  // 첫 업데이트인지 확인

    // ★ 차량 방향 판단 및 이미지 설정 함수
    void updateCarDirection(int current_x, int current_y);
};


#endif // SCREEN3VIEW_HPP
