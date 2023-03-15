#include "raylib.h"
#include "raymath.h"

class Heal{
private:
    Texture2D texture{};
    Vector2 worldPos{};
    float scale{4.0f};
public:
    Heal(Vector2 pos,Texture2D tex);
    void Render(Vector2 charaPos);
    Rectangle GetCollisionRec(Vector2 charaPos);


};