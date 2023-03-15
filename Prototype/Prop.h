#include "raylib.h"

class Prop{
public:
    Prop(Vector2 pos, Texture2D tex);
    void render(Vector2 charaPos);
    Rectangle getCollisionRec(Vector2 charaPos);
private:
    Texture2D texture{};
    Vector2 worldPos{};
    float scale = 4.f;
};