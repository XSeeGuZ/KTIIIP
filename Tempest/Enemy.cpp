#include "Enemy.h"
#include "raylib.h"
#include "raymath.h"
#include <string>
Enemy::Enemy()
{
}
Enemy::~Enemy()
{
}

Enemy::Enemy(Texture2D idle_tex, Texture2D move_tex, int mapW, int mapH, float dmg, float hp, float spd)
{
    worldPos = {static_cast<float>(GetRandomValue(170.f * enemyScale, 840.f * enemyScale)), static_cast<float>(GetRandomValue(100.f * enemyScale, 920.f * enemyScale))};
    // worldPos = {500.f * enemyScale, 600.f * enemyScale};
    texture = idle_tex;
    idle = idle_tex;
    move = move_tex;

    width = texture.width / max_frames;
    height = texture.height;
    hit_interval = 0;
    mode = 1;
    isEnemy = true;
    health = hp;
    maxHealth = health;
    enemyDmg = dmg;
    enemySpeed = spd;
}

void Enemy::tick(float deltaTime)
{
    if (!getAlive())
        return; // Check dead

    BaseChara::tick(deltaTime); // Animation & Draw Sprite

    if (mode == 0 && target->getAlive() == true) // Mode chase
    {
        if (hit_interval <= 0)
        {
            velocity = {};
            velocity = Vector2Scale(Vector2Normalize(Vector2Subtract(target->getScreenPos(), getScreenPos())), enemySpeed); // set enemy's target vector
            // DrawText(std::to_string(Vector2Length(velocity)).c_str(), 50, 200, 50, WHITE);
            if (Vector2Length(Vector2Subtract(target->getScreenPos(), getScreenPos())) < rad)
                velocity = {};
            worldPos = Vector2Add(worldPos, velocity); // move enemy
            getScreenPos() = Vector2Subtract(worldPos, target->getWorldPos());
        }
        else
        {
            velocity = {};
        }
        if (Vector2Length(Vector2Subtract(target->getScreenPos(), getScreenPos())) > OutofChaseRange && OOR_count >= 10) // if player is out of range, reset to wander mode
        {
            OOR_count = 0;
            mode = 1;
        }
        else if (OOR_count < 10)
        {
            OOR_count++;
        }
    }

    //DrawText(std::to_string(enemySpeed).c_str(), getScreenPos().x, 30.f + getScreenPos().y, 25, WHITE);
    if (mode == 1) // Mode wander
    {
        if (Vector2Length(Vector2Subtract(target->getScreenPos(), getScreenPos())) < ChaseRange) // if player is withing range, set to chase mode
        {
            mode = 0;
        }
        if (wander_interval <= 0)
        {
            velocity = {};
            if (wander_cd <= 0)
            {
                if (checkWander == false)
                {
                    checkWander = true;
                    randWanderCounter = 1;
                    wander_cd = GetRandomValue(150, 210);
                    randWander = GetRandomValue(120, wander_cd);
                    wander = {static_cast<float>(GetRandomValue(-5, 5)), static_cast<float>(GetRandomValue(-5, 5))}; // random pos
                    wander = Vector2Normalize(wander);
                }
                else
                {
                    checkWander = false;
                    wander_interval = GetRandomValue(100, 240);
                }
            }
            else
            {
                wander_cd--;
            }

            if (randWanderCounter <= randWander)
            {
                velocity = Vector2Scale(wander, enemySpeed);
                randWanderCounter++;
            }
            else
                velocity = {};
        }
        else
            wander_interval--;
    }

    if (Vector2Length(velocity) != 0) // moving
    {
        texture = move;
        worldPos = Vector2Add(worldPos, Vector2Scale(Vector2Normalize(velocity), speed));

        velocity.x < 0.f ? rightLeft = -1.f : rightLeft = 1.f;
    }
    else // not moving
    {
        texture = idle;
    }

    if (!CheckCollisionRecs(getCollisionRec(), target->getCollisionRec())) // KB calculation
    {
        KB_V = Vector2Scale(Vector2Normalize(Vector2Subtract(target->getScreenPos(), getScreenPos())), 100.f);
    }

    if (CheckCollisionRecs(getCollisionRec(), target->getCollisionRec()) && hit_interval <= 0) // Check hitting player with interval
    {
        target->takeDmg(enemyDmg, KB_V, enemyKB); // deal damage and knockback to player
        hit_interval = 40;                        // Set interval
    }

    // DrawText(std::to_string(health).c_str(), 50, 200, 50, WHITE);

    if (hit_interval > 0)
        hit_interval--;

    Rectangle healthBarLine{getScreenPos().x, getScreenPos().y - 20.f, width * scale * 2, height};
    Rectangle healthBar{getScreenPos().x, getScreenPos().y - 20.f, health / maxHealth * width * scale * 2, height};
    DrawRectangleRec(healthBar, RED);
    DrawRectangleLinesEx(healthBarLine, 5.f, BLACK);
}

Vector2 Enemy::getScreenPos()
{
    return Vector2Subtract(worldPos, target->getWorldPos());
}

void Enemy::takeDmg(Vector2 kbDirection, float kbMagnitude)
{
    if (target->get_eDmg_int() == false)
    {
        time_count = 0.f;
        target->set_eDmg_int(true);
        health -= damage;
        // Apply knockback
        Vector2 kbVector = Vector2Normalize(kbDirection);
        kbVector = Vector2Scale(kbVector, kbMagnitude);
        worldPos = Vector2Add(worldPos, kbVector);
    }

    if (health <= 0)
    {
        setAlive(false);
        return;
    }
}