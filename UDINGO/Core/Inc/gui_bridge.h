#ifndef GUI_BRIDGE_H
#define GUI_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
// GUI 갱신을 RTOS Task(main.c 등)에서 호출할 수 있도록 하는 함수
void gui_update_recommended_parking(int x, int y);
void gui_update_path_points(const uint8_t* points, int count);


#ifdef __cplusplus
}
#endif

#endif // GUI_BRIDGE_H
