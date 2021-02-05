#include <string.h>
#include "weapon.h"
#include "oggetto.h"
#include "map.h"

class Weapon : public Oggetto{
private:
    int _danno;
    figura _figura;
public:
    Weapon(int danno);
    bool hit();


}