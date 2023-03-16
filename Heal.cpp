#include "Heal.h"

Heal::Heal(Vector2 pos,Texture2D tex,int si):
    texture(tex),worldPos(pos),size(si)
{

}
Heal::~Heal(){

}
Heal::Heal(){
    
}

void Heal::Render(Vector2 knightPos){
    if(isUsed) return;

    scale=0.5f;
    Vector2 screenPos{Vector2Subtract(worldPos,knightPos)};
    DrawTextureEx(texture,screenPos,0.f,scale,WHITE);
    //draw hitbox
    //DrawRectangleLinesEx(GetCollisionRec(knightPos),1.f,RED);
}

Rectangle Heal::GetCollisionRec(Vector2 knightPos){
    scale=0.5f;
    Vector2 screenPos{Vector2Subtract(worldPos,knightPos)};
    if(!isUsed) return Rectangle{
        screenPos.x,screenPos.y,
        texture.width*scale,texture.height*scale
    };
    else return Rectangle{};
}
