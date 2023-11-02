#include "Camera.h"

#include <chrono>
#include <iostream>

namespace fl
{

Camera::Camera(const Vector2& windowRes, const float& frameTime) :
    res(windowRes), deltaTime(frameTime), pos({ 160000, 160000 }), moveDirs(), speed(0.3), zoom(2), moved(true), zoomThread(nullptr)
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

    rect.x = float(pos.x - ((res.x / 2) / zoom)) / 32;
    rect.y = float(pos.y - ((res.y / 2) / zoom)) / 32;

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
    auto zoomLambda = [this, dir]()
    {
    std::cout << zoom << std::endl;
        if (dir)
        {
            for (int i = 0; i < 10; i++)
            {
                if (this->zoom > 4)
                {
                    this->zoom = 4; break;
                }
                this->zoom += 0.1;
                moved = true;
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
        else
        {
            for (int i = 0; i < 10; i++)
            {
                if (this->zoom < 1)
                {
                    this->zoom = 1; break;
                }
                this->zoom -= 0.1;
                moved = true;
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
        this->zoom = round(this->zoom);
        moved = true;
    };


    if (zoom == 1 && !dir)
    {
        return;
    }
    if (zoom == 4 && dir)
    {
        return;
    }

    zoomThread = new std::thread(zoomLambda);
}

void Camera::Update()
{
    moved = false;

    pos.x += ((moveDirs[0] * -1) + (moveDirs[1])) * speed * -deltaTime;
    pos.y += ((moveDirs[2] * -1) + (moveDirs[3])) * speed * -deltaTime;

    if (pos.x < res.x / 2) pos.x = res.x / 2;
    if (pos.y < res.y / 2) pos.y = res.y / 2;

    if (pos.x + res.x / 2 > 320000) pos.x = 320000 - (res.x / 2);
    if (pos.y + res.y / 2 > 320000) pos.y = 320000 - (res.y / 2);

    if (moveDirs[0] || moveDirs[1] || moveDirs[2] || moveDirs[3]) moved = true;
}

void Camera::Sprint(const bool& sprint)
{
    if (sprint) speed = 0.6;
    else speed = 0.3;
}

}