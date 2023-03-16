#include "raylib.h"
#include "BaseChara.h"

BaseChara::BaseChara()
{
}

BaseChara::~BaseChara() {}

void BaseChara::undoMovement()
{
    worldPos = worldPosLastframe;
}

Rectangle BaseChara::getCollisionRec()
{
    if (isEnemy)
    {
        return Rectangle{
            getScreenPos().x, getScreenPos().y, width * scale, height * scale};
    }
    else
    {
        if (texture.id == attack.id)
        {
            if (rightLeft < 0.f)
                return Rectangle{
                    getScreenPos().x + ((width / 3) * scale), getScreenPos().y + ((height / 10) * scale), static_cast<float>(width / 1.5) * scale, height * scale * 0.9f};
            else
                return Rectangle{
                    getScreenPos().x, getScreenPos().y + ((height / 10) * scale), static_cast<float>(width / 1.5) * scale, height * scale * 0.9f};
        }
        else
            return Rectangle{
                getScreenPos().x, getScreenPos().y, width * scale, height * scale};
    }
}

void BaseChara::tick(float deltaTime)
{
    worldPosLastframe = worldPos;

    runningTime += deltaTime; // Animation

    if (!isEnemy)
    {
        if (texture.id == dash.id)
        {
            width = texture.width;
        }
        if (texture.id == attack.id)
        {
            width = texture.width / max_atk_frames;
        }
        if (texture.id == charge.id)
        {
            width = texture.width;
        }
        if (texture.id != charge.id && texture.id != dash.id && texture.id != attack.id)
        {
            width = texture.width / max_frames;
        }
    }
    else
    {
        width = texture.width / max_frames;
    }

    height = texture.height;
    if (runningTime >= updateTime)
    {
        frame++;
        runningTime = 0.f;
        if (frame > max_frames)
        {
            frame = 0;
        }
    }

    Rectangle source{frame * width, 0.f, rightLeft * width, height}; // Character setup source
    if (Attacking == true)
        source = {atk_frame * width, 0.f, rightLeft * width, height};
    Rectangle dest{getScreenPos().x, getScreenPos().y, scale * width, scale * height}; // Character setup
    if (isEnemy)
        dest = {getScreenPos().x, getScreenPos().y, enemyScale * width, enemyScale * height};
    DrawTexturePro(texture, source, dest, Vector2{}, 0.f, WHITE); // Draw texture
}
