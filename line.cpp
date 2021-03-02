//
// Created by ka_7a on 19.02.2021.
//

#include "line.h"
/*
void Line::connectLine() // Супер наивный алгоритм. Наверняка, если хорошо подумать, то можно найти что-то нормальное
{
    std::cout << "Ok, lets connect this shit" << std::endl;
    std::vector<Span> Sp = m_spansList;
    std::vector<Station> St = m_line;
    for (int i = 0; i < St.size(); i++)
    {
        Station curSt = St[i];
        std::string st_name = curSt.getName();
        std::cout << "### Name - " << st_name << " - ###" << std::endl;
        for (int j = 0; j < Sp.size(); j++) {
            Span curSp = Sp[j];
            std::string left = curSp.getLeft(), right = curSp.getRight();
            std::cout << " L : " << left << " ### R : " << right << std::endl;
            if (left == st_name)
                for (int k = 0; k < St.size(); k++)
                    if (St[k].getName() == right)
                    {
                        St[i].rightConnect(&St[k], Sp[j]);
                        std::cout <<  st_name << " -> " << right  << std::endl;
                        break;
                    }

            if (right == st_name)
                for (int k = 0; k < St.size(); k++)
                    if (St[k].getName() == left)
                    {
                        St[i].leftConnect(&St[k], Sp[j]);
                        std::cout << left << " <- " << st_name << std::endl;
                        break;
                    }
        }
    }
}
 */

void Line::printLine() const {

}
