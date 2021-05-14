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
    inline void e_St_delStation    (Station *St) { delete St;                                                        }
    // ## Получение информации о станции ## //
    inline const char *e_St_getName(Station *St) { return (St->getName()).c_str(); }
    inline int e_St_getNumber      (Station *St) { return St->getNumber();         }
    inline int e_St_getTraffic     (Station *St) { return St->getTraffic();        }
    inline int e_St_isCrossing     (Station *St) { return St->isCrossing();}
    // ## Изменение информации о станции ## //
    inline void e_St_setNumber  (Station *St, int number)  { St->setNumber(number);    }
    inline void e_St_setTraffic (Station *St, int traffic) { St->setTraffic(traffic);  }
    inline void e_St_setName    (Station *St, char * name) { St->setName(name);        }
    // ## Печать информации в терминал (бесполезно для GUI) ## //
    inline void e_St_consolePrint_Full  (Station *St) { St->printFullInfo();  }
    inline void e_St_consolePrint_Short (Station *St) { St->printShortInfo(); }

    // ## SPAN ## //
    const char * e_Sp_getName_r (Span* Sp) { return Sp->getRight().c_str(); }
    const char * e_Sp_getName_l (Span* Sp) { return Sp->getLeft().c_str() ; }

    double e_Sp_getTime_max (Span *Sp) { return Sp->getTime_max(); }
    double e_Sp_getTime_min (Span *Sp) { return Sp->getTime_min(); }

    void e_Sp_setTime_max (Span *Sp, double time) { Sp->setTime_max(time); }
    void e_Sp_setTime_min (Span *Sp, double time) { Sp->setTime_min(time); }

    // ## Menu ## //
    char * e_drawMainMenu(){

    }

    // ## RAILWAY_SYSTEM ## //
    railway_system* e_RS_init(char *filename, int mode)
    {
        auto RS = new railway_system;
        RS->lineInit(filename, mode);
        return RS;
    }
    void e_RS_finish(railway_system *RS) { delete RS; }

}
