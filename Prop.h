#include "raylib.h"

class Prop{
public:
    Prop(Vector2 pos, Texture2D tex , int Nprop);
    void render(Vector2 charaPos);
    Rectangle getCollisionRec(Vector2 charaPos);
    
private:
    Vector2 worldPos{};
    Texture2D texture{};
    int Np{} ;
    float scale = 4.f;
};