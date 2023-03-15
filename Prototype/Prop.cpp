#include "Prop.h"
#include "raymath.h"

Prop::Prop(Vector2 pos, Texture2D tex) : worldPos(pos), texture(tex){
}

void Prop::render(Vector2 charaPos){
    Vector2 screenPos = Vector2Subtract(worldPos, charaPos);
    DrawTextureEx(texture, screenPos, 0.f, scale, WHITE);
}

Rectangle Prop::getCollisionRec(Vector2 charaPos){
    Vector2 screenPos = Vector2Subtract(worldPos, charaPos);
    return Rectangle{
        screenPos.x, screenPos.y, texture.width * scale, texture.height * scale
    };
}