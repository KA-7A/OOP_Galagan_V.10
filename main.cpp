#include "station.h"
#include "line.h"
#include "railway_system.h"
#include "Menu.h"
#include <cstring>

#define SKIP_LIST 1
#define LNKD_LIST 0

#define NO_SUCH_FILE -1
#define NOT_A_JSON -2
#define INCORRECT_CONTENT -3

int main() {
    railway_system RS;
    RS.lineInit("./Saves/State.json", LNKD_LIST);
    Menu Menu(RS.getLines());
    return 0;
}

extern "C"
{
    railway_system * e_init(char *filename, int mode, char *error_msg) {
        auto *RS  = new railway_system;
        int error = RS->lineInit(filename, mode);
        //char error_msg[100];
        switch (error) {
            case NO_SUCH_FILE:
            {
                strcpy(error_msg, "No such file or directory!\n");
                break;
            }
            case NOT_A_JSON:
            {
                strcpy(error_msg, "JSON structure have not found in file!\n");
                break;
            }
            case INCORRECT_CONTENT:
            {
                strcpy(error_msg, "Incorrect data in file!\n");
                break;
            }
            case 0:
            {
                strcpy(error_msg, "\0");
                break;
            }
            default:
            {
                strcpy(error_msg, "???\n");
                break;
            }
        }
        return RS;
    }

    void e_cpp_consoleMenu( railway_system *RS) { Menu(RS->getLines()); }
    void e_finish (railway_system *RS) { delete RS; }
    int e_get_AmountOfLines(railway_system *RS) { return RS->get_Amount_of_lines(); }
    int e_get_linename(railway_system *RS, int number, char *msg, int size)
    {
        std::string s = RS->get_line(number)->getName();
        //std::cout << s;
        if (s.size() > size)
        {
            strcpy(msg, "\0");
            return -1;
        }
        else
            strncpy(msg, s.c_str(), s.size());
        return 0;
    }
    Line* e_get_line(railway_system *RS, int number)
    {
        return RS->get_line(number);
    }
    // Вывод всей линии (короткая и полная инфа соответственно)
    int e_get_ShortInfo(Line *line, char *msg, int size)
    {
        return line->get_ShortAllStationsInfo_list(msg, size);
    }
    int e_get_FullInfo(Line *line, char *msg, int size)
    {
        return line->get_FullAllStationsInfo_list(msg, size);
    }

}
