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
static bool gameOver = false;

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
                                 map.height, 5.f, 5.f, RandomFloat(2.f, 2.4f)});
    }
    else if (type == "knight")
    {
        ptr->push_back(new Enemy{idle,
                                 move,
                                 map.width,
                                 map.height, RandomFloat(8.f, 10.f), 11.f, RandomFloat(2.2f, 2.6f)});
    }
}

int main()
{
    srand(time(0));
    int wave = 0;
    int EnemyAmount = 10;
    //bool isEnemyDead[EnemyAmount]{false};
    bool inStartMenu = true;
    bool win = false;
    bool lose = false;
    InitWindow(Windowwidth, Windowheight, "Game");
    InitAudioDevice();

    Sound BGM = LoadSound("sounds/BGM.mp3");          // Background Music
    Sound Walk = LoadSound("walk.mp3");               // Code that i will merge
    Sound Sword = LoadSound("sounds/swordsound.mp3"); // Code that i will merge
    Sound Win = LoadSound("sounds/winsound.mp3");

    SetSoundVolume(Sword, 0.5f);
    SetSoundVolume(Win, 0.7f);

    Texture2D map = LoadTexture("tiles/finalmap.png"); // Load Map
    Vector2 mapPos{0.0, 0.0};                          // Load Map Coordsa
    const float mapScale = 2.f;
    Texture2D startMenuLogo = LoadTexture("tiles/1.png");
    Texture2D startButton = LoadTexture("tiles/2.png");

    Character AiPalm{Windowwidth, Windowheight, map.width, map.height}, forProp{Windowwidth, Windowheight, map.width, map.height};

    vector<Prop> props;

    for (int i = 0; i < 12; i++)
    {
        props.push_back(Prop{Vector2{(rand() % 1300 + 450) * mapScale, (rand() % 1550 + 280) * mapScale}, LoadTexture("tiles/log.png"), i});
    }

    for (int i = 13; i < 33; i++)
    {
        props.push_back(Prop{Vector2{(rand() % 1300 + 450) * mapScale, (rand() % 1510 + 280) * mapScale}, LoadTexture("tiles/rock.png"), i});
    }

    for (int i = 34; i <= 70; i++)
    {
        props.push_back(Prop{Vector2{(rand() % 1300 + 450) * mapScale, (rand() % 1510 + 280) * mapScale}, LoadTexture("tiles/tree.png"), i});
    }

    Heal *heal = new Heal[7];
    heal[0] = Heal{Vector2{(rand() % 1300 + 400) * mapScale, (rand() % 1610 + 230) * mapScale}, LoadTexture("HealthPotion/small.png"), 1};
    heal[1] = Heal{Vector2{(rand() % 1300 + 400) * mapScale, (rand() % 1610 + 230) * mapScale}, LoadTexture("HealthPotion/small.png"), 1};
    heal[2] = Heal{Vector2{(rand() % 1300 + 400) * mapScale, (rand() % 1610 + 230) * mapScale}, LoadTexture("HealthPotion/small.png"), 1};
    heal[3] = Heal{Vector2{(rand() % 1300 + 400) * mapScale, (rand() % 1610 + 230) * mapScale}, LoadTexture("HealthPotion/small.png"), 1};
    heal[4] = Heal{Vector2{(rand() % 1300 + 400) * mapScale, (rand() % 1610 + 230) * mapScale}, LoadTexture("HealthPotion/medium.png"), 2};
    heal[5] = Heal{Vector2{(rand() % 1300 + 400) * mapScale, (rand() % 1610 + 230) * mapScale}, LoadTexture("HealthPotion/medium.png"), 2};
    heal[6] = Heal{Vector2{(rand() % 1300 + 400) * mapScale, (rand() % 1610 + 230) * mapScale}, LoadTexture("HealthPotion/large.png"), 3};

    vector<Enemy *> goblins{};
    vector<Enemy *> knights{};
    vector<vector<Enemy *>> enemies{};

    SetTargetFPS(60);
    while (!WindowShouldClose()) // Update
    {
        BeginDrawing();
        ClearBackground(WHITE);

        if (gameOver==true)
        {
            if(win)
            {
                DrawRectangle(0.f, 0.f, Windowwidth, Windowheight, DARKGREEN);
                DrawText("You Win", Windowwidth * 0.2f, Windowheight * 0.35f, 200, WHITE);
                EndDrawing();
                continue;
            }
            else if(lose)
            {
                DrawRectangle(0.f, 0.f, Windowwidth, Windowheight, RED);
                DrawText("Game Over", Windowwidth * 0.1f, Windowheight * 0.4f, 200, WHITE);
                EndDrawing();
                continue;
            }
        }
 
        if (inStartMenu) // Code that i will merge
        {
            DrawTextureEx(map, Vector2{-750, -500}, 0.0, 2.0, WHITE);
            Rectangle source{0.f, 0.f, startMenuLogo.width, startMenuLogo.height};
            Rectangle dest{Windowwidth / 2 - startMenuLogo.width, -50.f
            , startMenuLogo.width * 2.f, startMenuLogo.height * 2.f};
            DrawTexturePro(startMenuLogo, source, dest, Vector2{}, 0.f, WHITE);
            //DrawTexture(startMenuLogo, Windowwidth / 2 - startMenuLogo.width / 2, 100, WHITE);
            DrawTexture(startButton, Windowwidth / 2 - startButton.width / 2, 300, WHITE);
            Vector2 mousePos = GetMousePosition();
            Rectangle startButtonRec = {Windowwidth / 2 - startButton.width / 2, 300, startButton.width, startButton.height};
            if (CheckCollisionPointRec(mousePos, startButtonRec))
            {
                DrawTexture(startButton, Windowwidth / 2 - startButton.width / 2, 300, RED);
            }
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, startButtonRec))
            {
                inStartMenu = false;
            }
        } //----
        else
        {
            mapPos = Vector2Scale(AiPalm.getWorldPos(), -1.f);

            DrawTextureEx(map, mapPos, 0.0, mapScale, WHITE); // Draw map

            if ((IsSoundPlaying(BGM) == false) && inStartMenu == false)
            {
                SetSoundVolume(BGM, 0.035f);
                PlaySound(BGM);
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && AiPalm.getAttacking() == false)
                PlaySound(Sword);

            if (Init == true) // Initialize wave
            {
                Init = false;
                wave++;
                goblins.clear();
                knights.clear();
                if (wave == 1)
                {
                    EnemyAmount = 8;
                    //EnemyAmount = 1;
                }
                else if (wave == 2)
                {
                    EnemyAmount = 12;
                    //EnemyAmount = 0;
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
                else if (wave >= 3)
                {
                    gameOver = true;
                    win = true;
                    PlaySound(Win);
                    continue;
                }
                for (auto enemy : enemies)
                {
                    for (auto e : enemy)
                    {
                        e->setTarget(&AiPalm); // Set Target to Player
                    }
                }
            }

            AiPalm.tick(GetFrameTime());// Render character

            for (auto prop : props)
            {
                prop.render(AiPalm.getWorldPosLF()); // Draw props
            }

            for (unsigned int i = 0; i < 7; i++) // Draw heal
            {
                heal[i].Render(AiPalm.getWorldPosLF());
            }

            AiPalm.DrawHB(); //Draw healthbar

            string eAmount = "Enemy Left: ";
            eAmount.append(to_string(EnemyAmount));
            DrawText(eAmount.c_str(), 50, 100, 50, WHITE);

            string waveAmount = "Current Wave: ";
            waveAmount.append(to_string(wave));
            DrawText(waveAmount.c_str(), 50, 175, 50, WHITE);

            for (auto enemy : enemies)
            {
                for (auto e : enemy)
                {
                    e->tick(GetFrameTime());
                }
            }

            for (unsigned int i = 0; i < 7; i++)
            {
                if ((CheckCollisionRecs(heal[i].GetCollisionRec(AiPalm.getWorldPos()), AiPalm.getCollisionRec())) && (heal[i].getIsUsed() == false))
                {
                    AiPalm.heal(heal[i].getSize());
                    heal[i].Used();
                }
            }

            if (!AiPalm.getAlive()) // Character is dead
            {
                gameOver = true;
                lose = true;
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

            if (AiPalm.getWorldPos().x < 0 || AiPalm.getWorldPos().y - 20 < 0 || // check out of bounds for character
                (AiPalm.getWorldPos().x + Windowwidth + 40 > map.width * mapScale) ||
                (AiPalm.getWorldPos().y + Windowheight + 40 > map.height * mapScale))
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
                            //e->undoMovement();
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
        }
        
        EndDrawing();
    }
    CloseAudioDevice();
    UnloadSound(BGM);
    UnloadSound(Sword);
    UnloadSound(Walk);
    CloseWindow();
}
