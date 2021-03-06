/*
 * Станция пересадки: описывается порядковым номером на линии, именем, средним грузопотоком и массивом названий
 * станций других линий (не учитываемых данным приложением), на которые можно осуществить пересадку.
 * Created by ka_7a on 19.02.2021.
*/

#ifndef RAILWAY_CROSSING_STATION_H
#define RAILWAY_CROSSING_STATION_H
#include "station.h"
#include <utility>
#include <vector>

class CrossingStation : public Station {
    private:
        std::vector<std::string> m_CrossList;
    public:
        CrossingStation()
        {
            fill(0, 0, "Unnamed");
            changeToType(false);
        }
        CrossingStation(int number, int av_traffic, std::string name){
            fill(number, av_traffic, std::move(name));
            changeToType(false);
        }
        std::vector<std::string> getCrossingStations() const    { return m_CrossList; }
        inline void addCrossingStation(const std::string& name) { m_CrossList.push_back(name); }

        void printFullInfo() const override {
            std::cout << "_________________________"        << std::endl <<
                         "| name: "       << getName()      << std::endl <<
                         "| num : "       << getNumber()    << std::endl <<
                         "| av_traffic: " << getTraffic()   << std::endl <<
                         "| Type: crossing"                 << std::endl <<
                         "| neighbours: "                   << std::endl;
            if (getLeftAddr() ) std::cout << "| l:" << getLeftName()  << std::endl;
            if (getRightAddr()) std::cout << "| r:" << getRightName() << std::endl;
            std::cout << "| | Cross to: " << std::endl;
            for (int i = 0; i < m_CrossList.size(); i++)
                std::cout << "| | "<< i << ". " << m_CrossList[i] <<";" << std::endl;
            std::cout  << "|________________________"  << std::endl;
        }

// ## Вообще не факт, что это когда-либо пригодится, но пусть будет
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
        inline void printCrossStations() const    {
            std::cout << "Cross to: \n";
            for (long unsigned int i = 0; i < m_CrossList.size(); std::cout << "--\"" << m_CrossList[i] << "\"" << std::endl, i++);
            std::cout << "-------------------" << std::endl;
        }
        ~CrossingStation ()
        {
            m_CrossList.clear();
            delete(&m_CrossList);
        }
};


#endif //RAILWAY_CROSSING_STATION_H
