#ifndef MODEL_HPP
#define MODEL_HPP

class ModelListener;

// 1. 경로 좌표를 저장할 간단한 구조체를 새로 정의합니다.
struct PathPoint {
    int x;
    int y;
};

class Model
{
public:
    // 2. 코드 전체에서 사용할 최대 경로점 개수를 상수로 정의합니다.
    static const int MAX_PATH_POINTS = 100;

    Model();

    void bind(ModelListener* listener) { modelListener = listener; }
    void tick();

    // --- 기존 변수들은 그대로 둡니다 ---
    int preferredType;
    int preferExit;
    void setPreferredType(int type)    { preferredType = type; }
    void setPreferExit(int exitOption) { preferExit = exitOption; }
    void sendPreferencesToMCU();
    int  recommendedParkingX;
    int  recommendedParkingY;
    bool hasNewGoal;

    // 3. 경로점 데이터를 저장할 새로운 멤버 변수들을 추가합니다.
    PathPoint pathPoints[MAX_PATH_POINTS]; // 경로점 좌표를 담을 배열
    int       numPathPoints;               // 실제 경로점의 개수
    bool      pathUpdated;                 // 경로가 새로 업데이트되었는지 알려주는 플래그

protected:
    ModelListener* modelListener;
};

#endif // MODEL_HPP
