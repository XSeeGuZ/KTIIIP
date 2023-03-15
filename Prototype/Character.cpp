#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include <string>

Character::Character(int winWidth, int winHeight, int mapW, int mapH) : windowWidth(winWidth),
                                                                        windowHeight(winHeight)
{
    width = texture.width / max_frames;
    height = texture.height;
    mapWidth = mapW;
    mapHeight = mapH;
    // worldPos = {static_cast<float>(GetRandomValue(10 * scale, mapWidth - 100)), static_cast<float>(GetRandomValue(6 * scale, mapHeight - 60))};
    worldPos = {800.f, 800.f};
    isEnemy = false;
    maxHealth=health;
}

Vector2 Character::getScreenPos()
{
    return Vector2{
        static_cast<float>(windowWidth) / 2.0f - scale * (0.5f * width),
        static_cast<float>(windowHeight) / 2.0f - scale * (0.5f * height)};
}

void Character::tick(float deltaTime)
{
    if (!getAlive())
    {
        texture = lose;
        width = texture.width;
        height = texture.height;
        Rectangle source{0.f, 0.f, rightLeft * width, height};                             // Character setup source
        Rectangle dest{getScreenPos().x, getScreenPos().y, scale * width, scale * height}; // Character setup
        DrawTexturePro(texture, source, dest, Vector2{}, 0.f, WHITE);                      // Draw texture
    }

    else
    {
        BaseChara::tick(deltaTime);
        velocity = {};

        if (Dash_Ani_holder >= 10)
        {
            Dashed = false;
            Dash_Ani_holder = 0;
            Dash_dir = {0.f, 0.f};
            dash_interval = 60;
        }

        if (Dashed == false && Attacking == false)
        {
            if (IsKeyDown(KEY_A))
                velocity.x -= speed;
            if (IsKeyDown(KEY_D))
                velocity.x += speed;
            if (IsKeyDown(KEY_W))
                velocity.y -= speed;
            if (IsKeyDown(KEY_S))
                velocity.y += speed;
        }
        if (Vector2Length(Vector2Scale(Vector2Normalize(velocity), speed)) != 0)
        {

            if (IsKeyPressed(KEY_SPACE) && Dashed == false && dash_interval == 1 && Attacking == false)
            { // Dash Check
                Dash_Ani_holder = 0;
                Dash_dir = velocity;
                Dashed = true;
                texture = dash;
            }
            else
            {
                if (Dashed == false)
                {
                    texture = move;
                    worldPos = Vector2Add(worldPos, velocity);
                }
            }

            velocity.x < 0.f ? rightLeft = -1.f : rightLeft = 1.f;
        }
        else
        {
            if (Dashed == false)
                texture = idle;
        }
        if (Dashed == true)
        {
            worldPos = Vector2Add(worldPos, Vector2Scale(Dash_dir, 5.f));
        }

        Vector2 origin{}; // Set origin point of weapon
        Vector2 offset{}; // Set offset of weapon to Character
        float rotation{};
        wp_width = weapon.width / max_atk_frames;
        if (rightLeft > 0.f)
        {
            // rotation = 0.f;
            origin = {0.f, 0.f};
            offset = {35.f, 50.f};
            weaponCollisionRec = {getScreenPos().x, getScreenPos().y, wp_width * scale, weapon.height * scale};
            /*weaponCollisionRec = {
                getScreenPos().x + offset.x,
                getScreenPos().y + offset.y - (weapon.height * scale),
                wp_width * scale,
                weapon.height * scale};*/
            // rotation = IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ? 35.f : 0.f;
        }
        else
        {
            // rotation = 180.f;
            origin = {width * scale, 0.f};
            offset = {25.f, 50.f};
            weaponCollisionRec = {getScreenPos().x, getScreenPos().y, wp_width * scale, weapon.height * scale};
            /*weaponCollisionRec = {
                getScreenPos().x + offset.x - (wp_width * scale),
                getScreenPos().y + offset.y - (weapon.height * scale),
                wp_width * scale,
                weapon.height * scale};*/
            // rotation = IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ? -35.f : 0.f;
        }
        DrawRectangleLines(getCollisionRec().x, getCollisionRec().y, getCollisionRec().width, getCollisionRec().height, BLUE);
        // Draw Sword when attacking
        if ((IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || Attacking == true) && Dashed == false && atk_interval <= 0)
        {
            Attacking = true;
            texture = attack;
            if (atk_ani_holder <= 0)
            {
                atk_ani_holder = 3;
                atk_frame++;
                if (atk_frame >= max_atk_frames)
                {
                    atk_frame = 0;
                    Attacking = false;
                    texture = idle;
                    atk_interval = 8;
                }
            }
            else
            {
                atk_ani_holder--;
            }

            if (Attacking == true)
            {
                Rectangle w_source{static_cast<float>(wp_width * atk_frame), 0.f, static_cast<float>(wp_width * rightLeft), static_cast<float>(weapon.height)};
                Rectangle w_dest{getScreenPos().x, getScreenPos().y, wp_width * scale, weapon.height * scale};
                if (rightLeft < 0.f)
                    w_dest = {getScreenPos().x + (wp_width * scale), getScreenPos().y, wp_width * scale, weapon.height * scale};

                DrawTexturePro(weapon, w_source, w_dest, origin, rightLeft, WHITE);
                DrawRectangleLines(weaponCollisionRec.x, weaponCollisionRec.y, weaponCollisionRec.width, weaponCollisionRec.height, RED); // Hitbox of weapon
                DrawRectangleLines(getCollisionRec().x, getCollisionRec().y, getCollisionRec().width, getCollisionRec().height, BLUE);
            }
        }

        if (Dashed == true)
            Dash_Ani_holder++;
        if (dash_interval > 1)
            dash_interval--;
        if (atk_interval > 0)
            atk_interval--;
    }
    Rectangle healthBarLine{0.f, 0.f,static_cast<float>(windowWidth),windowHeight*0.09f};
    Rectangle healthBar{0.f, 0.f,health/maxHealth*windowWidth,windowHeight*0.09f};
    DrawRectangleRec(healthBar,RED);
    DrawRectangleLinesEx(healthBarLine,5.f,BLACK);
}

void Character::takeDmg(int dmg, Vector2 kbDirection, float kbMagnitude)
{
    health -= dmg;
    if (health <= 0)
    {
        setAlive(false);
        return;
    }

    // Apply knockback
    Vector2 kbVector = Vector2Normalize(kbDirection);
    kbVector = Vector2Scale(kbVector, kbMagnitude);
    worldPos = Vector2Add(worldPos, kbVector);
}

void Character::setKB_V(Vector2 enemyScreenPos)
{
    KB_V = Vector2Scale(Vector2Normalize(Vector2Subtract(enemyScreenPos, getScreenPos())), 100.f);
}


void Character::heal(int size){
    float s{0.2f*maxHealth};
    float m{0.5f*maxHealth};
    float l{0.8f*maxHealth};
    switch (size)
    {
    case 1:
        if((health+s)>=maxHealth){ 
            health=maxHealth;
            }
        else{
            health+=s;
        }
        break;
    case 2:
        if((health+m)>=maxHealth)
            health=maxHealth;
        else
            health+=m;
        break;
    case 3:
        if((health+l)>=maxHealth)
            health=maxHealth;
        else
            health+=l;
        break;
    default:
            health+=0;
        break;
    }
}