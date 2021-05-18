//
// Created by Администратор on 30.04.2021.
//
#include "station.h"
#include "span.h"
#include "railway_system.h"

extern "C"
{

// Префикс e_ означает, что функция написана для внешней библиотеки [так ведь можно?)) ]
// префикс St_ означает, что речь в функции пойдёт о станции, а не каком-либо другом объекте


// ## Создание и удаление объекта STATION  ## //
    Station *   e_St_newStation (int number, int traffic, char* name)  { return new Station(number, traffic, name);  }
    void e_St_delStation    (Station *St) { delete St;                                                        }
    // ## Получение информации о станции ## //
    Station * e_get_St_byNumber(Line * line, int number)
    {
        return line->findStationByNumber(number);
    }
    const char *e_get_St_Name(Station *St) { return (St->getName()).c_str(); }
    int e_get_St_Number      (Station *St) { return St->getNumber();         }
    double e_get_St_Traffic  (Station *St) { return St->getTraffic();        }
    int e_get_St_isCrossing  (Station *St) { return St->isCrossing();        }
    // ## Изменение информации о станции ## //
    int e_set_St_Number  (Station *St, int new_number)  {
        if (St && St->isNumFree(new_number))
        {
            St->setNumber(new_number);
            return 0;
        }
        else
            return 1;
    }
    int e_set_St_Traffic (Station *St, int traffic) {
        if (St) {
            St->setTraffic(traffic);
            return 0;
        } else return 1;
    }
    int e_set_St_Name    (Station *St, char * name) {
        if (St) {
            St->setName(name);
            return 0;
        } else return 1;
    }
    // ## Печать информации в терминал (бесполезно для GUI) ## //
    int e_St_consolePrint_Full  (Station *St) { St->printFullInfo();  }
    int e_St_consolePrint_Short (Station *St) { St->printShortInfo(); }

    // ## SPAN ## //
    const char * e_get_Sp_Name_r (Span* Sp) { return Sp->getRight().c_str(); }
    const char * e_get_Sp_Name_l (Span* Sp) { return Sp->getLeft().c_str() ; }

    double e_get_Sp_Time_max (Span *Sp) { return Sp->getTime_max(); }
    double e_get_Sp_Time_min (Span *Sp) { return Sp->getTime_min(); }

    int e_set_Sp_Time_max (Span *Sp, double time) { Sp->setTime_max(time); }
    int e_set_Sp_Time_min (Span *Sp, double time) { Sp->setTime_min(time); }
    // ## RAILWAY_SYSTEM ## //
    railway_system* e_RS_init(char *filename, int mode)
    {
        auto RS = new railway_system;
        RS->lineInit(filename, mode);
        return RS;
    }
    void e_RS_finish(railway_system *RS) { delete RS; }
}
