#include <gui/screen3_screen/Screen3View.hpp>
#include <touchgfx/Unicode.hpp>
#include <stdio.h>
#include <cstdlib>  // ★ abs() 함수를 위해 추가

Screen3View::Screen3View()
: prev_car_x(0), prev_car_y(0), is_first_update(true)
{
    // pathPoint 0~99
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
    pathPointWidgets[100] = &pathPoint_100;
       pathPointWidgets[101] = &pathPoint_101;
       pathPointWidgets[102] = &pathPoint_102;
       pathPointWidgets[103] = &pathPoint_103;
       pathPointWidgets[104] = &pathPoint_104;
       pathPointWidgets[105] = &pathPoint_105;
       pathPointWidgets[106] = &pathPoint_106;
       pathPointWidgets[107] = &pathPoint_107;
       pathPointWidgets[108] = &pathPoint_108;
       pathPointWidgets[109] = &pathPoint_109;
       pathPointWidgets[110] = &pathPoint_110;
       pathPointWidgets[111] = &pathPoint_111;
       pathPointWidgets[112] = &pathPoint_112;
       pathPointWidgets[113] = &pathPoint_113;
       pathPointWidgets[114] = &pathPoint_114;
       pathPointWidgets[115] = &pathPoint_115;
       pathPointWidgets[116] = &pathPoint_116;
       pathPointWidgets[117] = &pathPoint_117;
       pathPointWidgets[118] = &pathPoint_118;
       pathPointWidgets[119] = &pathPoint_119;
       pathPointWidgets[120] = &pathPoint_120;
           pathPointWidgets[121] = &pathPoint_121;
           pathPointWidgets[122] = &pathPoint_122;
           pathPointWidgets[123] = &pathPoint_123;
           pathPointWidgets[124] = &pathPoint_124;
           pathPointWidgets[125] = &pathPoint_125;
           pathPointWidgets[126] = &pathPoint_126;
           pathPointWidgets[127] = &pathPoint_127;
           pathPointWidgets[128] = &pathPoint_128;
           pathPointWidgets[129] = &pathPoint_129;
}

void Screen3View::setupScreen()
{
    Screen3ViewBase::setupScreen();

    // 모든 경로점 숨김
    for (int i = 0; i < Model::MAX_PATH_POINTS; ++i)
    {
        pathPointWidgets[i]->setVisible(false);
    }

    // ★ 모든 차량 이미지 숨김 (처음 시작 시)
    car0.setVisible(false);
    car90.setVisible(false);
    car180.setVisible(false);
    car270.setVisible(false);

    // ★ 이전 위치 초기화
    is_first_update = true;
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

// ★ 차량 방향 판단 및 이미지 설정 함수
void Screen3View::updateCarDirection(int current_x, int current_y)
{
    // 첫 업데이트면 기본 이미지(car0) 사용
    if (is_first_update)
    {
        car0.setVisible(true);
        car90.setVisible(false);
        car180.setVisible(false);
        car270.setVisible(false);

        prev_car_x = current_x;
        prev_car_y = current_y;
        is_first_update = false;
        return;
    }

    // 이동 방향 계산
    int dx = current_x - prev_car_x;
    int dy = current_y - prev_car_y;

    // 이동이 없으면 현재 방향 유지
    if (dx == 0 && dy == 0)
    {
        return;
    }

    // 모든 차량 이미지 먼저 숨김
    car0.setVisible(false);
    car90.setVisible(false);
    car180.setVisible(false);
    car270.setVisible(false);

    // 방향 판단 및 해당 이미지 표시
    if (dx > 0 && dy == 0)
    {
        // 오른쪽으로 이동 (동쪽)
        car90.setVisible(true);
    }
    else if (dx < 0 && dy == 0)
    {
        // 왼쪽으로 이동 (서쪽)
        car270.setVisible(true);
    }
    else if (dx == 0 && dy < 0)
    {
        // 위로 이동 (북쪽)
        car0.setVisible(true);
    }
    else if (dx == 0 && dy > 0)
    {
        // 아래로 이동 (남쪽)
        car180.setVisible(true);
    }
    else
    {
        // 대각선 이동: 더 큰 변화량 방향 선택
        if (abs(dx) > abs(dy))
        {
            if (dx > 0)
            {
                car90.setVisible(true);
            }
            else
            {
                car270.setVisible(true);
            }
        }
        else
        {
            if (dy > 0)
            {
                car180.setVisible(true);
            }
            else
            {
                car0.setVisible(true);
            }
        }
    }

    // 이전 위치 업데이트
    prev_car_x = current_x;
    prev_car_y = current_y;
}

void Screen3View::updatePathPoints(const PathPoint* points, int count)
{
    if (count <= 0)
    {
        // 경로점이 없으면 모든 차량 이미지 숨김
        car0.setVisible(false);
        car90.setVisible(false);
        car180.setVisible(false);
        car270.setVisible(false);
        return;
    }

    // ★ A. 첫 번째 점(points[0])은 차량 이미지로 표시
    {
        int map_x = points[0].x;
        int map_y = points[0].y;

        // 차량 이미지 크기 (4개 모두 같은 크기라고 가정)
        int car_width = car0.getWidth();
        int car_height = car0.getHeight();

        int display_x = map_x * 3.97 + 4.68 - (car_width / 2);
        int display_y = map_y * 4.11 + 13 - (car_height / 2);

        // ★ 차량 방향 업데이트 (어떤 이미지를 표시할지 결정)
        updateCarDirection(map_x, map_y);

        // 모든 차량 이미지를 같은 위치로 설정
        car0.setXY(display_x, display_y);
        car90.setXY(display_x, display_y);
        car180.setXY(display_x, display_y);
        car270.setXY(display_x, display_y);

        // invalidate 호출
        car0.invalidate();
        car90.invalidate();
        car180.invalidate();
        car270.invalidate();

        // 첫 번째 pathPoint 위젯은 숨김
        pathPointWidgets[0]->setVisible(false);

        printf("[Screen3View] Car position: map(%d,%d) -> pixel(%d,%d)\r\n",
               map_x, map_y, display_x, display_y);
    }

    // ★ B. 나머지 점들(points[1] ~ points[count-1])은 경로점으로 표시
    for (int i = 1; i < count; ++i)
    {
        int map_x = points[i].x;
        int map_y = points[i].y;

        int display_x = map_x * 3.97 + 4.68 - 8;
        int display_y = map_y * 4.11 + 13 - 8;

        pathPointWidgets[i]->setXY(display_x, display_y);
        pathPointWidgets[i]->setVisible(true);
        pathPointWidgets[i]->invalidate();
    }

    // C. 사용하지 않는 나머지 점들은 숨김
    for (int i = count; i < Model::MAX_PATH_POINTS; ++i)
    {
        pathPointWidgets[i]->setVisible(false);
        pathPointWidgets[i]->invalidate();
    }
}
