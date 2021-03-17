#include "station.h"
#include "line.h"
#include "Menu.h"
#include "getlines.cpp"

std::vector<Line*> getLines(const char *filename, std::vector<Line*> Lines); // Эта функция будет доставать из файла информацию о линии. Надо запилить еще миллион проверок

int main() {
    std::vector<Line*> Lines;
    Lines = getLines("./Saves/State.json", Lines);
    Menu Menu(Lines);  // Возможно можно обойтись 1 переменной, мб - двумя. Просто хз как работает вектор
    for( int i = 0; i < Lines.size(); i++)
        delete Lines[i];
    return 0;
}
