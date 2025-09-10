// in gui/src/common/FrontendApplication.cpp

#include <gui/common/FrontendApplication.hpp>
#include <gui/model/Model.hpp> // Model 클래스 정의를 위해 include 합니다.

// ★ 1. Model 객체를 가리킬 전역 포인터의 "실체"를 여기서 정의합니다.
Model* globalModelPtr = nullptr;

FrontendApplication::FrontendApplication(Model& m, FrontendHeap& heap)
    : FrontendApplicationBase(m, heap)
{
    // ★ 2. 생성자에서 TouchGFX가 전달해준 진짜 model(m)의 주소를 전역 포인터에 저장합니다.
    globalModelPtr = &m;
}
