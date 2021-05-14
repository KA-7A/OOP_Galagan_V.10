//
// Created by Администратор on 30.04.2021.
//

#ifndef OOP_2_RAILWAY_SYSTEM_H
#define OOP_2_RAILWAY_SYSTEM_H

#include <vector>

#include "line.h"

#define SKIP_LIST 1
#define LNKD_LIST 0

class railway_system {
private:
    std::vector<Line*> m_Lines;
public:
    int lineInit(const char *filename, int mode) ;
    std::vector<Line*> getLines() { return m_Lines; }
    ~railway_system() { for (int i = 0; i < m_Lines.size(); delete m_Lines[i], i++); }
};


#endif //OOP_2_RAILWAY_SYSTEM_H
