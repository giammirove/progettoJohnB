#include "weapon.h"
Weapon::Weapon(int danno)
{
    _danno = danno;
}

bool Weapon::hit()
{
    int buff_id = controllaCollisione(ottieniFigura());
}