#include "station.h"
#include "line.h"
#include "Menu.h"

#define SKIP_LIST 1
#define LNKD_LIST 0

std::vector<Line *> getLines(const char *filename, std::vector<Line*> Lines, int mode);
// Эта функция будет доставать из файла информацию о линии. Надо запилить еще миллион проверок

int main() {
    std::vector<Line*> Lines;
    Lines = getLines("./Saves/State.json", Lines, SKIP_LIST);
    Menu Menu(Lines);
    for(unsigned int i = 0; i < Lines.size(); i++)
        delete Lines[i];
    return 0;
}
