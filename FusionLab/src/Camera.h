#pragma once
    
#include "SDLHandler.h"
#include <thread>

namespace fl
{

enum class Direction : uint8_t
{
    LEFT,
    UP,
    RIGHT,
    DOWN
};

typedef Direction Side;

class Camera
{
public:

    Camera(const Vector2& windowRes, float& frameTime);

    const Vector2F& GetPosition() const;
    
    const Bounds2F& GetView() const;

    void Move(const Direction& dir, const bool& moving);

    void ChangeZoom(const bool& dir);

    void Update();

    void Sprint(const bool& sprint);

public:

    float zoom;

private:

    void ZoomAnim(const bool& dir);

private:
    
    Vector2 res;
    float& deltaTime;

    Vector2F pos;

    bool moveDirs[4];

    float speed;

    bool moved;

    std::jthread* zoomThread;
    bool currentlyZooming;
};

}

