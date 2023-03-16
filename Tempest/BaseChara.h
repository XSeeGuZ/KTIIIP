
#ifndef BASECHARA
#define BASECHARA
#include "raylib.h"
class BaseChara
{
public:
    ~BaseChara();
    BaseChara();
    Vector2 getWorldPos() { return worldPos; }
    void undoMovement();
    Rectangle getCollisionRec();
    virtual void tick(float deltaTime);
    virtual Vector2 getScreenPos() = 0;
    bool getAlive() { return alive;}
    void setAlive(bool isAlive) { alive = isAlive; }
    bool getAttacking() { return Attacking;}
    float getMaxHp(){return maxHealth;}

protected:
    Texture2D texture = LoadTexture("characters/chara_idle.png");
    Texture2D idle = LoadTexture("characters/chara_idle.png"); // Load Character
    Texture2D move = LoadTexture("characters/chara_run.png");  // Load Character moving
    Texture2D attack = LoadTexture("characters/chara_atk.png");
    Texture2D dash = LoadTexture("characters/chara_dash.png");
    Texture2D lose = LoadTexture("characters/chara_lose.png");
    Vector2 worldPos{};
    Vector2 worldPosLastframe{};
    Vector2 Dash_dir{};
    Vector2 velocity{};
    Vector2 KB_V{};
    float rightLeft = 1.f;         // Facing direction
    int max_frames = 6;            // total frames
    int max_atk_frames = 2;        // atk total
    float runningTime = 0;         // current time
    int frame = 0;                 // current character frame
    int atk_frame = 0;
    float updateTime = 1.f / 12.f; // time in secs to next frame          
    int Dash_Ani_holder = 0;
    bool Dashed = false;
    int dash_interval = 1;
    int atk_ani_holder = 10;
    int atk_interval = 0;
    float width{};
    float wp_width{};
    float height{};
    float scale = 2.f;
    float enemyScale = 4.f;
    int mapWidth;
    int mapHeight;
    float speed; //Speed of character
    float enemySpeed; //Speed of enemy
    float enemyKB = 15.f;
    float KB = 50.f;
    bool Attacking = false;
    bool isEnemy;
    float maxHealth{};
    int atk_duration = 5;
    int atk_interval_max = 8;
    bool temp_immune{false};
    float time_count{0.f};
    bool enemy_dmg_interval = false;
    float Ulti{0.f};
    float maxUlti{100.f};
    float Ulticharge{};
    float damage{5};
    float Dash_speed{5.f};
    bool charging = false;
    bool useUlti = false;
private:
    bool alive{true};
};

#endif