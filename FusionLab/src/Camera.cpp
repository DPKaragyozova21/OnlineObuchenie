#include "Camera.h"

namespace fl
{

Camera::Camera(const Vector2& windowRes, float frameTime) :
    res(windowRes), deltaTime(frameTime), pos({ 700, 600 }), moveDirs(), speed(3), zoom(2), moved(true)
{
    moveDirs[0] = false; moveDirs[1] = false; moveDirs[2] = false; moveDirs[3] = false;
}

const Vector2F& Camera::GetPosition() const
{
    return pos;
}

const Bounds2F& Camera::GetView() const
{
    static Bounds2F rect;

    rect.x = float(pos.x - (res.x / 2)) / 32;
    rect.y = float(pos.y - (res.y / 2)) / 32;

    rect.w = ceil(float(res.x) / (zoom * 32));
    rect.h = ceil(float(res.y) / (zoom * 32));

    if (!moved) rect.x = -1;

    return rect;
}

void Camera::Move(const Direction& dir, const bool& moving)
{
    moveDirs[(int)dir] = moving;
}

void Camera::ChangeZoom(const bool& dir)
{
    if (zoom == 1 && !dir) return;
    if (zoom == 4 && dir) return;

    if (dir)
    {
        zoom++;
        /*pos.x += res.x / (2 + zoom);
        pos.y += res.y / (2 + zoom);*/
    }
    else
    {
        zoom--;
        /*pos.x -= res.x / (2 + zoom);
        pos.y -= res.y / (2 + zoom);*/
    }


    moved = true;
}

void Camera::Update()
{
    moved = false;

    pos.x += (((moveDirs[0] * -1) + (moveDirs[1])) * speed) / zoom;
    pos.y += (((moveDirs[2] * -1) + (moveDirs[3])) * speed) / zoom;

    if (pos.x < res.x / 2) pos.x = res.x / 2;
    if (pos.y < res.y / 2) pos.y = res.y / 2;

    if (moveDirs[0] || moveDirs[1] || moveDirs[2] || moveDirs[3]) moved = true;
}

}