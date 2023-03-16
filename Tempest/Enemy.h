#include "raylib.h"
#include "BaseChara.h"
#include "Character.h"

class Enemy : public BaseChara
{
public:
    Enemy();
    ~Enemy();
    Enemy(Texture2D idle_tex, Texture2D move_tex, int mapW, int mapH,float dmg,float hp,float spd);
    void tick(float);
    void setTarget(Character *chara) { target = chara; }
    virtual Vector2 getScreenPos() override;
    int getHealth() { return health; }
    void takeDmg(Vector2 kbVector, float kbMagnitude);
    bool getErase() { return erased;}
    void setErase(bool er) { erased = er; }

private:
    Character *target;
    Vector2 wander{};
    int enemyDmg{};
    int hit_interval;
    int mode; // 0 = chase, 1 = wander
    bool checkWander = false;
    int randWander{2};
    int randWanderCounter{1};
    int wander_interval{};
    int wander_cd{};
    int health{};
    bool isAlive{true};
    bool erased{false};
    float ChaseRange = 475.f;
    float OutofChaseRange = 575.f;
    float rad = 25.f;
    int OOR_count = 0;
};