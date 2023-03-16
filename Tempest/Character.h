#ifndef CHARACTER
#define CHARACTER
#include "raylib.h"
#include "raymath.h"
#include "BaseChara.h"

class Character : public BaseChara
{
public:
    Character(int winWidth, int winHeight, int mapW, int mapH);
    void tick(float);
    virtual Vector2 getScreenPos() override;
    Rectangle getweaponColRec() { return weaponCollisionRec; }
    bool checkDash() { return Dashed; }
    bool checkUltimate() { return Ulti; }
    int getHealth() const { return health; }
    int getDmg() { return dmg; }
    void takeDmg(int dmg, Vector2 kbDirection, float kbMagnitude);
    Vector2 getKB_V() { return KB_V; }
    float getKB() { return KB; }
    void setKB_V(Vector2 enemyScreenPos);
    void heal(int size);

private:
    int windowWidth;
    int windowHeight;
    Texture2D weapon{LoadTexture("characters/chara_weapon.png")};
    Rectangle weaponCollisionRec{};
    int health{100};
    int dmg{5};
};
#endif