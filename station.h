/*
 * Обычная станция: описывается порядковым номером на линии, именем и средним грузопотоком. Должна поддерживать
 * регистрацию изменения грузопотока и сравнение по порядковому номеру с помощью операций < и ==.
 * Created by ka_7a on 19.02.2021.
*/

#ifndef RAILWAY_STATION_H
#define RAILWAY_STATION_H
#include <string>
#include <utility>
#include "span.h"

class Station{
private:
    int m_number;           // Номер станции, имеет вид (№ветки * 1000 + №станции)
    int m_av_traffic;       // Значение среднего траффика
    std::string m_name;     // Имя станции
    Station *m_next, *m_prev;   // Указатели на соседние станции. Ну потому что по ТЗ это д.б. связанный список
    Span m_left, m_right; // Указатели на перегоны до соседних станций. Если соседний перегон занимает 0 времени, то станция конечная (оч кривая система, но пусть будет)
    bool m_isRegular;       // true -- станция обычная, false -- пересадочная. Мне не нрав такой момент, но пусть будет
public:
    Station(void)
    {
        m_number = 0;
        m_av_traffic = 0;
        m_name = "Unnamed";
        m_next = m_prev = nullptr;
        m_isRegular = true;     // По дефолту все станции у нас сначала обычные
    }
    Station(int number, int av_traffic, std::string name) {  // Такой вот странный конструктор, который, почему-то, не вызывается. Сцука такая
        m_number = number;
        m_av_traffic = av_traffic;
        m_name = name;
        m_isRegular = true;
        m_next = nullptr;
        m_prev = nullptr;
    }
// #### Производим манипуляции с самой станцией #### //
    void fill(int number, int av_traffic, std::string name){    // Заполняем основные поля станции
        m_number = number;
        m_name = std::move(name);
        m_av_traffic = av_traffic;
        m_next = m_prev = nullptr;
        m_isRegular = true;
    }

    // ## Соединяем станции ## //
    inline void rightConnect(Station *next, Span &right ) { m_next = next ; m_right = right;
    //std::cout << "St." << m_name << "and st." << m_next->getName() << " connected" << std::endl;
    }    // Соединяем станциями справа
    inline void leftConnect (Station *prev,  Span &left)  { m_prev = prev ; m_left  = left;
    //std::cout << "St." << m_name << "and st." << m_prev->getName() << " connected" << std::endl;
    }    // И слева

    // ## Меняем кое-какие показатели ## //
    inline void changeToType(bool type){ m_isRegular = type; }
    inline void TrafficChange(int new_traffic) { m_av_traffic = new_traffic; };
    inline void ChangeTypeToCrossing() { m_isRegular = false; } // Не спрашивай, почему оно вот так

    // ## Выдаем всю информацию о станции на печать ## //
    void printInfo() const {
        std::cout << "\n-----------------------" << std::endl;
        std::cout << "name: " << m_name << "\nnum: " << m_number << "\nav_traffic: " << m_av_traffic << std::endl;
        if (m_isRegular) std::cout << "Type: regular" << std::endl;
        else std::cout << "Type: crossing" << std::endl;
        std::cout << "-----------------------\n" << std::endl;
    }

// #### Получаем информацию о самой станции #### //
    inline int  getNumber() const { return m_number; };
    inline int  getTraffic()const { return m_av_traffic; };
    inline std::string getName()  { return m_name;  };
    inline bool isCrossing() const{ return (!m_isRegular); }

// #### Получаем информацию о соседях станции ### //
    inline std::string getLeftName() const { return m_prev->getName(); }
    inline std::string getRightName() const{ return m_next->getName(); }
    inline Station * getLeftAddr()  const { return m_prev; }
    inline Station * getRightAddr() const { return m_next;}
// #### Перегружаем операторы ##### //
    bool operator== (const Station &S2) const { return (m_number == S2.m_number); }
    bool operator<  (const Station &S2) const { return (m_number < S2.m_number);  }

};
#endif //RAILWAY_STATION_H
