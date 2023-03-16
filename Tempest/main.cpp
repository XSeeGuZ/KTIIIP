//How to i make fade transition from start menu of the game to the game when i press the start button here is the code

#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Prop.h"
#include "Enemy.h"
#include <string>
#include <vector>
#include <list>
#include "Heal.h"
using namespace std;

static const int Windowwidth = 1280, Windowheight = 720;
static bool Init = true;

void AddEnemy(vector<Enemy *> *ptr, Texture2D idle, Texture2D move, Texture2D map)
{
    ptr->push_back(new Enemy{idle,
                             move,
                             map.width,
                             map.height, 10.f, 100.f, 2.5f});
}

int main()
{

    int wave = 0;
    int EnemyAmount = 10;
    bool isEnemyDead[EnemyAmount]{false};
    bool inStartMenu = true;
    
    InitWindow(Windowwidth, Windowheight, "Game");
    InitAudioDevice();

    Sound BGM = LoadSound("song.mp3");         // Background Music
    Sound Walk = LoadSound("walk.mp3");        // Code that i will merge
    Sound Sword = LoadSound("swordsound.mp3"); // Code that i will merge

    Texture2D map = LoadTexture("tiles/map.png"); // Load Map
    Vector2 mapPos{0.0, 0.0};                     // Load Map Coords
    const float mapScale = 2.f;
    Texture2D startMenuLogo = LoadTexture("1.png");
    Texture2D startButton = LoadTexture("2.png");
    Character AiPalm{Windowwidth, Windowheight, map.width, map.height};

    Prop props[2]{Prop{Vector2{600.f * mapScale, 600.f * mapScale}, LoadTexture("characters/weapon_sword.png")},
                  Prop{Vector2{400.f * mapScale, 300.f * mapScale}, LoadTexture("characters/weapon_sword.png")}};

    Heal *heal = new Heal[2];
    heal[0] = Heal{Vector2{300.f * mapScale, 200.f * mapScale}, LoadTexture("characters/weapon_sword_1.png"), 1};
    heal[1] = Heal{Vector2{400.f * mapScale, 200.f * mapScale}, LoadTexture("characters/weapon_sword_1.png"), 1};
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

        if (inStartMenu) // Code that i will merge
        {
            DrawTextureEx(map, Vector2{-750, -500}, 0.0, 2.0, WHITE);
            DrawTexture(startMenuLogo,Windowwidth/2-startMenuLogo.width/2,100,WHITE);
            DrawTexture(startButton, Windowwidth / 2 - startButton.width / 2, 300, WHITE);
            Vector2 mousePos = GetMousePosition();
            Rectangle startButtonRec = {Windowwidth/2-startButton.width/2,300,startButton.width,startButton.height};
            if (CheckCollisionPointRec(mousePos, startButtonRec))
            {
                DrawTexture(startButton, Windowwidth / 2 - startButton.width / 2, 300, RED);
            }
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, startButtonRec))
            {
                inStartMenu = false;
            }
        }    //----
        else // Code that i will merge
        {
            mapPos = Vector2Scale(AiPalm.getWorldPos(), -1.f);

            DrawTextureEx(map, mapPos, 0.0, mapScale, WHITE); // Draw map

            if ((IsSoundPlaying(BGM) == false) && inStartMenu == false)
            {
                SetSoundVolume(BGM,0.15);
                PlaySound(BGM);
            }

            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) PlaySound(Sword);
            
            

            for (auto prop : props)
            {
                prop.render(AiPalm.getWorldPos()); // Draw props
            }
            for (unsigned int i = 0; i < 2; i++)
            {
                heal[i].Render(AiPalm.getWorldPos());
            }

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
                            e->takeDmg(AiPalm.getDmg(), AiPalm.getKB_V(), AiPalm.getKB());
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
        }
        EndDrawing();
    }
    CloseAudioDevice();
    UnloadSound(BGM);
    UnloadSound(Sword);
    UnloadSound(Walk);
    CloseWindow();
}
