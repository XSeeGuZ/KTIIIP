#include "Heal.h"

Heal::Heal(Vector2 pos,Texture2D tex):
    worldPos(pos),
    texture(tex)
{

}

void Heal::Render(Vector2 charaPos){
    Vector2 screenPos{Vector2Subtract(worldPos,charaPos)};
    DrawTextureEx(texture,screenPos,0.f,scale,WHITE);
}

Rectangle Heal::GetCollisionRec(Vector2 charaPos){
    Vector2 screenPos{Vector2Subtract(worldPos,charaPos)};
    return Rectangle{
        screenPos.x,screenPos.y,
        texture.width*scale,texture.height*scale
    };
}