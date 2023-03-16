#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Prop.h"
#include "Enemy.h"
#include <string>
#include <vector>
#include <list>
#include "Heal.h"
#include <cstdlib>
#include <ctime>
using namespace std;

static const int Windowwidth = 1280, Windowheight = 720;
static bool Init = true;

float RandomFloat(float min, float max)
{
    float ran = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    float diff = max - min;
    float r = min + (diff * ran);
    return r;
}

void AddEnemy(vector<Enemy *> *ptr, Texture2D idle, Texture2D move, Texture2D map, string type)
{
    if (type == "goblin")
    {
        ptr->push_back(new Enemy{idle,
                                 move,
                                 map.width,
                                 map.height, 5.f,5.f, RandomFloat(2.2f, 2.7f)});
    }
    else if (type == "knight")
    {
        ptr->push_back(new Enemy{idle,
                                 move,
                                 map.width,
                                 map.height, RandomFloat(8.f, 10.f), 11.f, RandomFloat(2.6f, 3.1f)});
    }
}

int main()
{
    srand(time(0));
    int wave = 0;
    int EnemyAmount = 10;
    bool isEnemyDead[EnemyAmount]{false};
    InitWindow(Windowwidth, Windowheight, "Game");

    Texture2D map = LoadTexture("tiles/finalmap.png"); // Load Map
    Vector2 mapPos{0.0, 0.0};                     // Load Map Coords
    const float mapScale = 2.f;

    Character AiPalm{Windowwidth, Windowheight, map.width, map.height};

    vector<Prop> props{Prop{Vector2{600.f * mapScale, 600.f * mapScale}, LoadTexture("characters/weapon_sword.png")},
                       Prop{Vector2{400.f * mapScale, 300.f * mapScale}, LoadTexture("characters/weapon_sword.png")}};

    Heal *heal = new Heal[2];
    heal[0] = Heal{Vector2{300.f * mapScale, 200.f * mapScale}, LoadTexture("characters/weapon_sword_1.png"), 1};
    heal[1] = Heal{Vector2{400.f * mapScale, 200.f * mapScale}, LoadTexture("characters/weapon_sword_1.png"), 1};

    vector<Enemy *> goblins{};
    vector<Enemy *> knights{};
    vector<vector<Enemy *>> enemies{};

    SetTargetFPS(60);
    while (!WindowShouldClose()) // Update
    {
        BeginDrawing();
        ClearBackground(WHITE);

        mapPos = Vector2Scale(AiPalm.getWorldPos(), -1.f);

        DrawTextureEx(map, mapPos, 0.0, mapScale, WHITE); // Draw map

        if (Init == true) // Initialize wave
        {
            Init = false;
            wave++;
            goblins.clear();
            knights.clear();
            if (wave == 1)
            {
                EnemyAmount = 8;
            }
            else if (wave == 2)
            {
                EnemyAmount = 12;
            }

            for (int i = 0; i < EnemyAmount; i++) // Initialize Enemy
            {
                if (wave == 1)
                {
                    AddEnemy(&goblins, // Add goblin
                             LoadTexture("characters/goblin_idle_spritesheet.png"),
                             LoadTexture("characters/goblin_run_spritesheet.png"),
                             map,
                             "goblin");
                }
                else if (wave == 2)
                {
                    AddEnemy(&knights, // Add goblin
                             LoadTexture("characters/knight_idle_spritesheet.png"),
                             LoadTexture("characters/knight_run_spritesheet.png"),
                             map,
                             "knight");
                }
            }
            if (wave == 1)
                enemies.push_back(goblins);
            else if (wave == 2)
                enemies.push_back(knights);

            for (auto enemy : enemies)
            {
                for (auto e : enemy)
                {
                    e->setTarget(&AiPalm); // Set Target to Player
                }
            }
        }

        for (auto prop : props)
        {
            prop.render(AiPalm.getWorldPos()); // Draw props
        }
        for (unsigned int i = 0; i < 2; i++)
        {
            heal[i].Render(AiPalm.getWorldPos());
        }

        DrawText(to_string(EnemyAmount).c_str(), 50, 200, 50, WHITE);

        AiPalm.tick(GetFrameTime()); // Render character
        for (auto enemy : enemies)
        {
            for (auto e : enemy)
            {
                e->tick(GetFrameTime());
            }
        }

        for (unsigned int i = 0; i < 2; i++)
        {
            if ((CheckCollisionRecs(heal[i].GetCollisionRec(AiPalm.getWorldPos()), AiPalm.getCollisionRec())) && (heal[i].getIsUsed() == false))
            {
                AiPalm.heal(heal[i].getSize());
                heal[i].Used();
            }
        }

        if (!AiPalm.getAlive()) // Character is dead
        {
            DrawRectangle(0.f, 0.f, Windowwidth, Windowheight, RED);
            DrawText("Game Over", Windowwidth * 0.1f, Windowheight * 0.4f, 200, WHITE);
            EndDrawing();
            continue;
        }
        else
        {
            std::string knightHealth{""};
            knightHealth.append(std::to_string(AiPalm.getHealth()), 0, 5);
            DrawText(knightHealth.c_str(), Windowwidth * 0.42f, Windowheight * 0.015f, 50, WHITE);
            std::string maxHp{""};
            maxHp.append("/");
            maxHp.append(std::to_string(AiPalm.getMaxHp()), 0, 3);
            DrawText(maxHp.c_str(), Windowwidth * 0.48f, Windowheight * 0.015f, 50, WHITE);
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
                        e->takeDmg(AiPalm.getKB_V(), AiPalm.getKB());
                        if (e->getAlive() == false && e->getErase() == false)
                        {
                            e->setErase(true);
                            EnemyAmount--;
                        }
                    }
                }
            }
        }
        // delete enemy when they're dead *****bug with vector<Enemy*>***** only work with normal pointer dynamic allocation
        /*for (unsigned int i = 0; i < EnemyAmount ; i++){
            if(!enemies->at(i)->getAlive()&&!isEnemyDead[i]){
                isEnemyDead[i]=true;
                delete &enemies[i];
            }
        }*/

        if (EnemyAmount <= 0)
        {
            Init = true;
        }

        EndDrawing();
    }
    CloseWindow();
}
