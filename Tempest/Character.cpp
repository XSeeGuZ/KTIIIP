
void Character::heal(int size)
{
    float s{0.2f * maxHealth};
    float m{0.5f * maxHealth};
    float l{0.8f * maxHealth};
    switch (size)
    {
    case 1:
        if ((health + s) >= maxHealth)
        {
            health = maxHealth;
        }
        else
        {
            health += s;
        }
        break;
    case 2:
        if ((health + m) >= maxHealth)
            health = maxHealth;
        else
            health += m;
        break;
    case 3:
        if ((health + l) >= maxHealth)
            health = maxHealth;
        else
            health += l;
        break;
    default:
        health += 0;
        break;
    }
}