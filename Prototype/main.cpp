#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Prop.h"
#include "Enemy.h"
#include <string>
#include <vector>
#include <list>
using namespace std;

static const int Windowwidth = 1280, Windowheight = 720;
static bool Init = true;

void AddEnemy(vector<Enemy *> *ptr, Texture2D idle, Texture2D move, Texture2D map)
{
    ptr->push_back(new Enemy{idle,
                             move,
                             map.width,
                             map.height});
}

int main()
{
    int wave = 0;
    int EnemyAmount = 10;
    InitWindow(Windowwidth, Windowheight, "Game");

    Texture2D map = LoadTexture("tiles/map.png"); // Load Map
    Vector2 mapPos{0.0, 0.0};                     // Load Map Coords
    const float mapScale = 2.f;

    Character AiPalm{Windowwidth, Windowheight, map.width, map.height};

    Prop props[2]{Prop{Vector2{600.f * mapScale, 600.f * mapScale}, LoadTexture("characters/weapon_sword.png")},
                  Prop{Vector2{400.f * mapScale, 300.f * mapScale}, LoadTexture("characters/weapon_sword.png")}};

    /*Enemy goblin{
        LoadTexture("characters/goblin_idle_spritesheet.png"),
        LoadTexture("characters/goblin_run_spritesheet.png"),
        map.width,
        map.height};

    Enemy slime{
        LoadTexture("characters/slime_idle_spritesheet.png"),
        LoadTexture("characters/slime_run_spritesheet.png"),
        map.width,
        map.height};*/

    vector<Enemy *> goblins{};
    for (int i = 0; i < EnemyAmount; i++) // Initialize Enemy
    {
        AddEnemy(&goblins,
                 LoadTexture("characters/goblin_idle_spritesheet.png"),
                 LoadTexture("characters/goblin_run_spritesheet.png"),
                 map);
    }

    vector<Enemy *> enemies[]{goblins};

    for (auto enemy : enemies)
    {
        for (auto e : enemy)
        {
            e->setTarget(&AiPalm); // Set Target to Player
        }
    }

    SetTargetFPS(60);
    while (!WindowShouldClose()) // Update
    {
        BeginDrawing();
        ClearBackground(WHITE);

        mapPos = Vector2Scale(AiPalm.getWorldPos(), -1.f);

        DrawTextureEx(map, mapPos, 0.0, mapScale, WHITE); // Draw map

        for (auto prop : props)
        {
            prop.render(AiPalm.getWorldPos()); // Draw props
        }

        AiPalm.tick(GetFrameTime()); // Render character
        for (auto enemy : enemies)
        {
            for (auto e : enemy)
            {
                e->tick(GetFrameTime());
            }
        }

        if (!AiPalm.getAlive()) // Character is dead
        {
            DrawText("Game Over!", 55.f, 45.f, 50, RED);
            EndDrawing();
            continue;
        }
        else
        {
            string health = "Health: ";
            health.append(to_string(AiPalm.getHealth()));
            DrawText(health.c_str(), 55.f, 45.f, 50, WHITE);
        }

        if (AiPalm.getWorldPos().x < 0 || AiPalm.getWorldPos().y < 0 || // check out of bounds for character
            (AiPalm.getWorldPos().x + Windowwidth > map.width * mapScale) ||
            (AiPalm.getWorldPos().y + Windowheight > map.height * mapScale))
        {
            AiPalm.undoMovement();
        }

        for (auto enemy : enemies)
        {
            for (auto e : enemy)
            {
                if (e->getWorldPos().x < 310 * mapScale || e->getWorldPos().y < 170 * mapScale || // check out of bounds for enemy
                    (e->getWorldPos().x + (340 * mapScale) > map.width * mapScale) ||
                    (e->getWorldPos().y + (185 * mapScale) > map.height * mapScale))
                {
                    e->undoMovement();
                }
            }
        }

        for (auto prop : props)
        { // Prop collision check
            if (CheckCollisionRecs(prop.getCollisionRec(AiPalm.getWorldPos()), AiPalm.getCollisionRec()))
            {
                AiPalm.undoMovement();
            }
            
            for (auto enemy : enemies)
            {
                for (auto e : enemy)
                {
                    if (CheckCollisionRecs(prop.getCollisionRec(AiPalm.getWorldPos()), e->getCollisionRec()))
                    {
                        e->undoMovement();
                    }
                }
            }
        }

        for (auto enemy : enemies)
        {
            for (auto e : enemy)
            {
                if (!CheckCollisionRecs(AiPalm.getCollisionRec(), e->getCollisionRec()))
                {
                    AiPalm.setKB_V(e->getScreenPos());
                }
            }
        }

        if (AiPalm.getAttacking() == true && AiPalm.checkDash() == false) // Attack
        {
            for (auto enemy : enemies)
            {
                for (auto e : enemy)
                {
                    if (CheckCollisionRecs(AiPalm.getweaponColRec(), e->getCollisionRec()))
                {
                    e->takeDmg(AiPalm.getDmg(), AiPalm.getKB_V(), AiPalm.getKB());
                }
                }
            }
        }

        EndDrawing();
    }
    CloseWindow();
}
