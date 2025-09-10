#include <gui/screen3_screen/Screen3View.hpp>
#include <touchgfx/Unicode.hpp>
#include <stdio.h>

Screen3View::Screen3View()
{
    // ★ 1. 생성자에서 Designer에서 만든 위젯과 C++ 포인터 배열을 1:1로 연결합니다.
    // 이 작업은 조금 길지만, 매우 중요합니다.
    // &pathPoint_0 은 Designer에서 만든 위젯의 실제 주소를 의미합니다.
    pathPointWidgets[0] = &pathPoint_0;
    pathPointWidgets[1] = &pathPoint_1;
    pathPointWidgets[2] = &pathPoint_2;
    pathPointWidgets[3] = &pathPoint_3;
    pathPointWidgets[4] = &pathPoint_4;
    pathPointWidgets[5] = &pathPoint_5;
    pathPointWidgets[6] = &pathPoint_6;
    pathPointWidgets[7] = &pathPoint_7;
    pathPointWidgets[8] = &pathPoint_8;
    pathPointWidgets[9] = &pathPoint_9;
    pathPointWidgets[10] = &pathPoint_10;
    pathPointWidgets[11] = &pathPoint_11;
    pathPointWidgets[12] = &pathPoint_12;
    pathPointWidgets[13] = &pathPoint_13;
    pathPointWidgets[14] = &pathPoint_14;
    pathPointWidgets[15] = &pathPoint_15;
    pathPointWidgets[16] = &pathPoint_16;
    pathPointWidgets[17] = &pathPoint_17;
    pathPointWidgets[18] = &pathPoint_18;
    pathPointWidgets[19] = &pathPoint_19;
    pathPointWidgets[20] = &pathPoint_20;
    pathPointWidgets[21] = &pathPoint_21;
    pathPointWidgets[22] = &pathPoint_22;
    pathPointWidgets[23] = &pathPoint_23;
    pathPointWidgets[24] = &pathPoint_24;
    pathPointWidgets[25] = &pathPoint_25;
    pathPointWidgets[26] = &pathPoint_26;
    pathPointWidgets[27] = &pathPoint_27;
    pathPointWidgets[28] = &pathPoint_28;
    pathPointWidgets[29] = &pathPoint_29;
    pathPointWidgets[30] = &pathPoint_30;
    pathPointWidgets[31] = &pathPoint_31;
    pathPointWidgets[32] = &pathPoint_32;
    pathPointWidgets[33] = &pathPoint_33;
    pathPointWidgets[34] = &pathPoint_34;
    pathPointWidgets[35] = &pathPoint_35;
    pathPointWidgets[36] = &pathPoint_36;
    pathPointWidgets[37] = &pathPoint_37;
    pathPointWidgets[38] = &pathPoint_38;
    pathPointWidgets[39] = &pathPoint_39;
    pathPointWidgets[40] = &pathPoint_40;
    pathPointWidgets[41] = &pathPoint_41;
    pathPointWidgets[42] = &pathPoint_42;
    pathPointWidgets[43] = &pathPoint_43;
    pathPointWidgets[44] = &pathPoint_44;
    pathPointWidgets[45] = &pathPoint_45;
    pathPointWidgets[46] = &pathPoint_46;
    pathPointWidgets[47] = &pathPoint_47;
    pathPointWidgets[48] = &pathPoint_48;
    pathPointWidgets[49] = &pathPoint_49;
    pathPointWidgets[50] = &pathPoint_50;
       pathPointWidgets[51] = &pathPoint_51;
       pathPointWidgets[52] = &pathPoint_52;
       pathPointWidgets[53] = &pathPoint_53;
       pathPointWidgets[54] = &pathPoint_54;
       pathPointWidgets[55] = &pathPoint_55;
       pathPointWidgets[56] = &pathPoint_56;
       pathPointWidgets[57] = &pathPoint_57;
       pathPointWidgets[58] = &pathPoint_58;
       pathPointWidgets[59] = &pathPoint_59;
       pathPointWidgets[60] = &pathPoint_60;
       pathPointWidgets[61] = &pathPoint_61;
       pathPointWidgets[62] = &pathPoint_62;
       pathPointWidgets[63] = &pathPoint_63;
       pathPointWidgets[64] = &pathPoint_64;
       pathPointWidgets[65] = &pathPoint_65;
       pathPointWidgets[66] = &pathPoint_66;
       pathPointWidgets[67] = &pathPoint_67;
       pathPointWidgets[68] = &pathPoint_68;
       pathPointWidgets[69] = &pathPoint_69;
       pathPointWidgets[70] = &pathPoint_70;
       pathPointWidgets[71] = &pathPoint_71;
       pathPointWidgets[72] = &pathPoint_72;
       pathPointWidgets[73] = &pathPoint_73;
       pathPointWidgets[74] = &pathPoint_74;
       pathPointWidgets[75] = &pathPoint_75;
       pathPointWidgets[76] = &pathPoint_76;
       pathPointWidgets[77] = &pathPoint_77;
       pathPointWidgets[78] = &pathPoint_78;
       pathPointWidgets[79] = &pathPoint_79;
       pathPointWidgets[80] = &pathPoint_80;
       pathPointWidgets[81] = &pathPoint_81;
       pathPointWidgets[82] = &pathPoint_82;
       pathPointWidgets[83] = &pathPoint_83;
       pathPointWidgets[84] = &pathPoint_84;
       pathPointWidgets[85] = &pathPoint_85;
       pathPointWidgets[86] = &pathPoint_86;
       pathPointWidgets[87] = &pathPoint_87;
       pathPointWidgets[88] = &pathPoint_88;
       pathPointWidgets[89] = &pathPoint_89;
       pathPointWidgets[90] = &pathPoint_90;
       pathPointWidgets[91] = &pathPoint_91;
       pathPointWidgets[92] = &pathPoint_92;
       pathPointWidgets[93] = &pathPoint_93;
       pathPointWidgets[94] = &pathPoint_94;
       pathPointWidgets[95] = &pathPoint_95;
       pathPointWidgets[96] = &pathPoint_96;
       pathPointWidgets[97] = &pathPoint_97;
       pathPointWidgets[98] = &pathPoint_98;
       pathPointWidgets[99] = &pathPoint_99;
}

void Screen3View::setupScreen()
{
    Screen3ViewBase::setupScreen();

    // ★ 2. 화면이 처음 나타날 때, 모든 경로점을 일단 숨깁니다.
    // 이렇게 하면 이전 경로가 화면에 남아있는 것을 방지할 수 있습니다.
    for (int i = 0; i < Model::MAX_PATH_POINTS; ++i)
    {
        pathPointWidgets[i]->setVisible(false);
    }
}

void Screen3View::tearDownScreen()
{
    Screen3ViewBase::tearDownScreen();
}

// --- 기존 추천 주차 공간 업데이트 함수는 그대로 둡니다. ---
void Screen3View::updateRecommendedParking(int map_x, int map_y)
{
    if (map_x != -1 && map_y != -1)
    {
        int marker_width  = recommendedParkingMarker.getWidth();
        int marker_height = recommendedParkingMarker.getHeight();
        int display_x     = map_y * 3.97 + 4.68 - 12; 	// MAP_IMAGE_OFFSET_X + map_x * MAP_GRID_SIZE_PIXELS_X + (MAP_GRID_SIZE_PIXELS_X / 2) - (marker_width / 2);
        int display_y     = map_x * 4.11 + 13 - 12; 	// MAP_IMAGE_OFFSET_Y + map_y * MAP_GRID_SIZE_PIXELS_Y + (MAP_GRID_SIZE_PIXELS_Y / 2) - (marker_height / 2);

        recommendedParkingMarker.setXY(display_x, display_y);
        recommendedParkingMarker.setVisible(true);
        recommendedParkingMarker.invalidate();

        // 부모 컨테이너 invalidate
        if (recommendedParkingMarker.getParent())
            recommendedParkingMarker.getParent()->invalidate();

        // ⭐ 디버깅 로그
        printf("[Screen3View] map(%d,%d) -> pixel(%d,%d), visible=%d, w=%d,h=%d \r\n",
               map_x, map_y, display_x, display_y,
               recommendedParkingMarker.isVisible(),
               marker_width, marker_height);
    }
    else
    {
        recommendedParkingMarker.setVisible(false);
        recommendedParkingMarker.invalidate();

        if (recommendedParkingMarker.getParent())
            recommendedParkingMarker.getParent()->invalidate();

        printf("[Screen3View] map(-1) hide marker\r\n");
    }
}

// ★ 3. 경로점 업데이트 함수의 실제 동작을 구현합니다.
// in gui/src/screen3_screen/Screen3View.cpp

void Screen3View::updatePathPoints(const PathPoint* points, int count)
{
    // A. C 코드에서 전달된 개수(count)만큼 점들을 화면에 보여주고, 위치를 이동시킵니다.
    for (int i = 0; i < count; ++i)
    {
        // PathPoint 구조체에서 x, y를 가져옵니다.
        int map_x = points[i].x;
        int map_y = points[i].y;

        // ★★★★★
        // updateRecommendedParking 함수에서 사용하는 것과 동일한
        // 좌표 변환 공식을 여기에 적용합니다.
        // ★★★★★
        int display_x = map_x * 3.97 + 4.68 -8; // 화면 X좌표는 map_y를 사용
        int display_y = map_y * 4.11 + 13 -8;  // 화면 Y좌표는 map_x를 사용

        // 계산된 픽셀 좌표로 i번째 점 위젯을 이동시킵니다.
        pathPointWidgets[i]->setXY(display_x, display_y);
        // 점을 보이게 설정합니다.
        pathPointWidgets[i]->setVisible(true);
        // 변경된 내용을 화면에 그리라고 요청합니다.
        pathPointWidgets[i]->invalidate();
    }

    // B. 사용하지 않는 나머지 점들(count ~ 49번째)은 확실하게 숨깁니다.
    for (int i = count; i < Model::MAX_PATH_POINTS; ++i)
    {
        pathPointWidgets[i]->setVisible(false);
        pathPointWidgets[i]->invalidate();
    }
}
