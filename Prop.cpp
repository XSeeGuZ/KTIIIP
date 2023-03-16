#include "Prop.h"
#include "raymath.h"

Prop::Prop(Vector2 pos, Texture2D tex , int Nprop) : worldPos(pos), texture(tex) , Np(Nprop){
}

void Prop::render(Vector2 charaPos){
    Vector2 screenPos = Vector2Subtract(worldPos, charaPos);
    DrawTextureEx(texture, screenPos, 0.f, scale, WHITE);
}



Rectangle Prop::getCollisionRec(Vector2 charaPos){
    Vector2 screenPos = Vector2Subtract(worldPos, charaPos);
    
    if( Np >=34)
    {
      return Rectangle{
        screenPos.x+50.f , screenPos.y+160.f , texture.width * scale * 0.35f, texture.height * scale * 0.25f
    };
    } else
    {
      return Rectangle{
        screenPos.x, screenPos.y, texture.width * scale, texture.height * scale
    };
    }

    
    
}