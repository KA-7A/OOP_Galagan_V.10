#include "station.h"
#include "line.h"
#include "railway_system.h"
#include "Menu.h"

#define SKIP_LIST 1
#define LNKD_LIST 0


int main() {
    railway_system RS;
    RS.lineInit("./Saves/State.json", LNKD_LIST);
    Menu Menu(RS.getLines());
    return 0;
}

extern "C"
{
    railway_system * e_init(char *filename, int mode) {
        railway_system *RS;
        RS->lineInit(filename, mode);
        return RS;
    }
    void e_consoleMenu( railway_system *RS)
    {
        Menu(RS->getLines());
        return;
    }
    void finish (railway_system *RS)
    {
        delete RS;
    }
}
