/*
 * Обычная станция: описывается порядковым номером на линии, именем и средним грузопотоком. Должна поддерживать
 * регистрацию изменения грузопотока и сравнение по порядковому номеру с помощью операций < и ==.
 * Created by ka_7a on 19.02.2021.
*/


#ifndef RAILWAY_STATION_H
#define RAILWAY_STATION_H
#include <string>
#include <vector>
#include <list>
#include <ctime>
#include <cmath>
#include <utility>
#include <iostream>
#include "span.h"

class Span;

class Station{
private:
    int m_number;               // Номер станции, имеет вид (№ветки * 1000 + №станции)
    int m_av_traffic;           // Значение среднего траффика
    std::string m_name;         // Имя станции
    Station *m_next, *m_prev;   // Указатели на соседние станции. Ну потому что по ТЗ это д.б. связанный список. Если что-то нулл, то это конечная
    Span m_left, m_right;       // Перегоны до соседних станций. Надо использовать указатели. Исправить.
public:
    Station()   // Дефолтный конструктор
    {
        m_number     = 0;
        m_av_traffic = 0;
        m_name = "Unnamed";
        m_next = m_prev = nullptr;
    }
    Station(int number, int av_traffic, std::string name) {
        m_number = number;
        m_av_traffic = av_traffic;
        m_name = name;
        m_next = nullptr;
        m_prev = nullptr;
    }
    Station(int number, int av_traffic, char* name) {
        m_number = number;
        m_av_traffic = av_traffic;
        m_name = name;
        m_next = nullptr;
        m_prev = nullptr;
    }
// #### Производим манипуляции с самой станцией #### //
    void fill(int number, int av_traffic, std::string name){    // Заполняем основные поля станции
        m_number = number;
        m_name = std::move(name);
        m_av_traffic = av_traffic;
    }
    // ## Соединяем станции ## //
    inline void rightConnect(Station *next, Span &right )  { m_next = next ; m_right = right; }
    inline void leftConnect (Station *prev, Span &left)    { m_prev = prev ; m_left  = left;  }
    inline void rightConnect(Station *next)                { m_next = next; }
    inline void leftConnect (Station *prev)                { m_next = prev; }

    // ## Меняем кое-какие показатели ## //
    inline void setTraffic   (int traffic)      { m_av_traffic = traffic;     }
    inline void setName      (std::string name) { m_name   = std::move(name); }
    inline void setName      (char * name)      { m_name   = name;            }
    inline void setNumber    (int number)       { m_number = number;          }

    // ## Выдаем всю информацию о станции на печать ## //
    virtual void printFullInfo()  const {
        std::cout << "_________________________" << std::endl;
        std::cout << "| name: "         << m_name       << std::endl <<
                     "| num : "         << m_number     << std::endl <<
                     "| av_traffic: "   << m_av_traffic << std::endl <<
                     "| Type: regular"                  << std::endl <<
                     "| neighbours: "                   << std::endl;
        if (getLeftAddr() ) std::cout << "| l:" << getLeftName()  << std::endl;
        if (getRightAddr()) std::cout << "| r:" << getRightName() << std::endl;
        std::cout  << "|________________________"  << std::endl;
    }
    void printShortInfo()         const {
        std::cout << "|---------------------------" << std::endl;
        std::cout << "| name: " << m_name   << std::endl <<
                     "| num : " << m_number << std::endl;
    }

// #### Получаем информацию о самой станции #### //
    inline int getNumber()              const { return m_number;       }
    inline int getTraffic()             const { return m_av_traffic;   }
    inline const std::string& getName() const { return m_name;         }
    inline virtual bool isCrossing()    const { return false;          }

// #### Получаем информацию о соседях станции ### //
    inline const std::string getLeftName()   const { if (m_prev!= nullptr) return m_prev->getName(); else return nullptr; }
    inline const std::string getRightName()  const { if (m_next!= nullptr) return m_next->getName(); else return nullptr; }
    /*
     * Убрали тут возвращение по ссылке потому что компилятор ругается, что в случае else у нас возвращается
     * временный объект. И он прав, оставлю пока вот так.
     */
    inline Station * getLeftAddr()     const { if (m_prev!= nullptr) return m_prev; else return nullptr; }
    inline Station * getRightAddr()    const { if (m_next!= nullptr) return m_next; else return nullptr; }
    inline double getTimeToLeft_min()  const { return m_left.getTime_min() ; }
    inline double getTimeToLeft_max()  const { return m_left.getTime_max() ; }
    inline double getTimeToRight_min() const { return m_right.getTime_min(); }
    inline double getTimeToRight_max() const { return m_right.getTime_max(); }
// И перегонах
    inline Span getLeftSpan()  const { return m_left;  }
    inline Span getRightSpan() const { return m_right; }
// #### Перегружаем операторы ##### //
    bool operator== (const Station &S2) const { return (m_number == S2.m_number); }
    bool operator<  (const Station &S2) const { return (m_number <  S2.m_number); }

    virtual void addCrossingStation(const std::string name) { return; }
    virtual int rightLayerConnect(int layer, Station *St, double minTime, double maxTime) { return 0; }
    virtual int leftLayerConnect (int layer, Station *St, double minTime, double maxTime) { return 0; }
    virtual int getSize() { return 0; }

    virtual ~Station()= default;
};

typedef struct neighbour {
    Station *St;
    double minTime, maxTime;
} neighbour;

class S_Station: public Station{
private:
    std::vector<neighbour*> m_leftDir, m_rightDir;
    int m_size;
    static const int maxSize = 4;
protected:
    void init_size(int seed)
    {
        std::srand(std::time(nullptr)+seed);
        double tmp = (std::rand() % 1000) / 1000.;
        int i;
        for (i = 1; i < maxSize; i++ )
            if (tmp > 1. / pow(2, i) )
            {
                m_size = i;
                return;
            }
    }
    void init_size()
    {
        std::srand(std::time(nullptr));
        double tmp = (std::rand() % 1000) / 1000.;
        int i;
        for (i = 1; i < maxSize; i++ )
            if (tmp > 1. / pow(2, i) )
            {
                m_size = i;
                return;
            }
    }
public:
    S_Station()
    {
        init_size();
    }
    /*explicit (?) */  S_Station(int seed)
    {
        init_size(seed);
    }
    S_Station(int number, int av_traffic, std::string name) {
        fill(number, av_traffic, std::move(name));
        init_size();
        leftConnect(nullptr);
        rightConnect(nullptr);

    }

    int getSize() override { return m_size; }
    int leftLayerConnect(int layer, Station *St, double minTime, double maxTime) override
    {
        if (layer >= m_size)                            return -1;
        if ((unsigned int)layer != m_leftDir.size())  return -2;
        auto tmp = new neighbour;
        tmp->St = St;
        tmp->minTime = minTime;
        tmp->maxTime = maxTime;
        m_leftDir.push_back(tmp);
        return 0;
    }
    int rightLayerConnect(int layer, Station *St, double minTime, double maxTime) override
    {
        if (layer >= m_size)                            return -1;
        if ((unsigned int)layer != m_rightDir.size()) return -2;
        auto tmp = new neighbour;
        tmp->St = St;
        tmp->minTime = minTime;
        tmp->maxTime = maxTime;
        m_rightDir.push_back(tmp);
        return 0;
    }

    //virtual void addCrossingStation(const std::string name) override { return; }

    ~S_Station() override
    {
        for (auto & i : m_leftDir)
            delete i;
        for (unsigned int i = 0; i < m_rightDir.size(); i++)
            delete m_rightDir[i];
        // Оставил две разные конструкции по смыслу одинаковые просто чтобы привыкнуть к ним побольше
    }

};

class S_CrossingStation: public S_Station
{
private:
    std::vector<std::string> m_CrossList;
public:
    S_CrossingStation()   // Дефолтный конструктор
    {
        fill(0, 0, "Unnamed");
    }
    S_CrossingStation(int number, int av_traffic, std::string name){
        fill(number, av_traffic, name);   // Заполняем поля нужным содержимым
    }
    inline bool isCrossing() const override                 { return true; }
    std::vector<std::string> getS_CrossingStations() const  { return m_CrossList;           }
    inline void addCrossingStation(const std::string name) override  { m_CrossList.push_back(name);  }

    void printFullInfo() const override {      // Вывод на печать всей информации о станции
        std::cout << "_________________________"        << std::endl <<
                  "| name: "       << getName()      << std::endl <<
                  "| num : "       << getNumber()    << std::endl <<
                  "| av_traffic: " << getTraffic()   << std::endl <<
                  "| Type: crossing"                 << std::endl <<
                  "| neighbours: "                   << std::endl;
        if (getLeftAddr() ) std::cout << "| l:" << getLeftName()  << std::endl;
        if (getRightAddr()) std::cout << "| r:" << getRightName() << std::endl;
        std::cout << "| | Cross to: " << std::endl;
        for (unsigned int i = 0; i < m_CrossList.size(); i++)
            std::cout << "| | "<< i << ". " << m_CrossList[i] <<";" << std::endl;
        std::cout  << "|________________________"  << std::endl;
    }

// ## Вообще не факт, что это когда-либо пригодится, но пусть будет (ничего не тестил, не факт что вообще работает)
    void rmCrossingStationByName    (const std::string& name) {
        for (long unsigned int i = 0; i != m_CrossList.size(); ++i)  // Trying to find the station in m_CrossList
            if (m_CrossList[i] == name)
            {
                m_CrossList.erase(m_CrossList.begin() + i);   // And erase it
                return;
            }
        std::cout << "No station with this name was found!" << std::endl;
    }
    void rmCrossingStationByPosition(int pos) { m_CrossList.erase(m_CrossList.begin() + pos); }
    inline void rmAllCrossingStations()       { m_CrossList.clear(); }
    void printCrossStations() const {
        std::cout << "Cross to: \n";
        for (long unsigned int i = 0; i < m_CrossList.size(); std::cout << "--\"" << m_CrossList[i] << "\"" << std::endl, i++);
        std::cout << "-------------------" << std::endl;
    }

    ~S_CrossingStation () override {}// Кажется, тут что-то должно быть по другому


};

#endif //RAILWAY_STATION_H
