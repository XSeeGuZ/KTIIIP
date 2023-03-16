#include "raylib.h"
#include "raymath.h"

class Heal{
private:
    Texture2D texture{};
    Vector2 worldPos{};
    float scale{0.5f};
    int size{};
    bool isUsed{false};
public:
    Heal(Vector2 pos,Texture2D tex,int si);
    Heal();
    ~Heal();
    void Render(Vector2 knightPos);
    Rectangle GetCollisionRec(Vector2 knightPos);
    int getSize(){return size;}
    void Used(){isUsed=true;}
    bool getIsUsed() const {return isUsed;}
    
};