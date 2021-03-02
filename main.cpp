#include <iostream>
#include <cassert>
#include "station.h"
#include "line.h"
#include "span.h"
#include "crossing_station.h"


std::vector<Line> getLines(const char *filename, std::vector<Line> Lines)  // Эта функция будет доставать из файла информацию о линии. Надо запилить еще миллион проверок
{
    // -- Тут идет блок с открытием файла и парсингом его в объект, с которым дальше нужно будет работать -- //
//--- Parsing from file to object ---//
    FILE *source = fopen(filename, "r");
    assert(source);
    char readBuffer[65536];
    FileReadStream is(source, readBuffer, sizeof (readBuffer));

    Document linesObj;
    linesObj.ParseStream(is);

    fclose(source);
//--- The end of Parsing ---//

    // -- Закрыли файл, теперь у нас есть объект linesObj, в котором, по идее, должен лежать корректный набор данных -- //
    // -- Дальше нужно заполнить список
    // -- Но сначала сделаем пару проверок
    assert(linesObj.IsObject());
    assert(linesObj.HasMember("lines") && linesObj["lines"].Size());

    for (unsigned int i = 0; i < linesObj["lines"].Size() ; i++)       // Идем по линиям
    {
        assert(linesObj["lines"][i].IsString());    // Проверили, строка ли тут у нас
        std::string lineName = linesObj["lines"][i].GetString();

        assert(linesObj.HasMember(lineName.c_str())); // Проверяем, что у нас есть такой блок
        assert(linesObj[linesObj["lines"][i]].HasMember("stations") && linesObj[linesObj["lines"][i]].HasMember("spans"));    // Проверяем, что в нашем блоке-линии есть необходимые элементы
        // Вот это потенциально сложный для осознания момент:
        const Value& line  = linesObj[linesObj["lines"][i]]["stations"];    // Делаем объект, состоящий из станций
        const Value& spans = linesObj[linesObj["lines"][i]]["spans"];       // Делаем объект, состоящий из перегонов
        // std::cout << "line.Size = " << line.Size() << "; spans.Size - 1 = " << spans.Size() - 1 << std::endl;
        assert(line.Size() == (spans.Size() - 1));    // Проверяем, что перегонов на 1 больше, чем станци//
                                                        // (2 мнимых перегона с нулевым временем по краям ветки)
        /*
         * Суть в том, что у нас есть JSON следующего вида:
         * "lines" : [ lineName1, lineName2, ...],
         * "lineName1": { "stations": [ "name" : ... , ... ], "spans": [...]},
         * "lineName2": { ... }...,
         * ...
         * Теперь нам надо добраться до каждого lineName. Для этого говорим, что типа element -- это маленький JSON
         * из большого JSON'a. И так теперь проверять его будет проще и быстре... По идее)))
         * крч element -- это JSON одной станции следующего вида:
         * { "name" : ... , ... }
         */
        Line tmpLine;
// ---------- Here is Spans filling block ------------------ //
        //std::cout << "lineNum == "<< i <<"; lineName is "<< lineName << ";" << std::endl;
        std::vector<Span> span_vector;

        for (unsigned int j = 0; j < spans.Size(); j++)  // Тут мы заполнили вектор из
        {
            const Value& Sp = spans[j];
            assert(Sp.IsObject()                                                                // Проверяем, что это у нас объект
            && Sp.HasMember("left")  && Sp["left"].IsString()                                   // Что в нем есть строковое поле "left"
            && Sp.HasMember("right") && Sp["right"].IsString()                                  // Что в нём есть строковое поле "right"
            && Sp.HasMember("time")  && (Sp["time"].IsDouble() || Sp["time"].IsInt()) && Sp["time"].GetDouble() >= 0    // Что там есть поле время с неотрицательным значением
            );
            Span tmpSpan(Sp["left"].GetString(), Sp["right"].GetString(), Sp["time"].GetDouble());
            tmpLine.spanPushBack(tmpSpan);
        }
// ---------- Here is Lines filling and connection block -- //
        // -- Вот до этого момента мы загнали линию в отдельный объект. -- //

        tmpLine.writeName(lineName);
        for (unsigned int j = 0; j < line.Size(); j++)     // Идем по станциям в этой линии
        {
            const Value& St = line[j];
            // Document st = linesObj[lineName.c_str()][j];
            // Проверяем, что объект, который мы тут читаем, вообще подходит под формат станции (под нужный нам формат)
            // Защищаемся вообще от всего. Вот вообще.
            assert (St.IsObject()
            && St.HasMember("name")         && St["name"].IsString()                                        // Проверяем, что у нас есть имя, и что это строка
            && St.HasMember("number")       && St["number"].IsInt()         && St["number"].GetInt() >= 0   // Проверяем, что у нас есть корректный номер станции
            && St.HasMember("neighbours")   && St["neighbours"].IsArray()   && St["neighbours"].Size() == 2 // Проверяем, что у станции есть ровно 2 соседа по линии
            && St.HasMember("is_crossing")  && St["is_crossing"].IsBool()                                   // Проверяем, что есть инфа о том, пересадочная ли станция
            && St.HasMember("cross_to")
            && ((       St["is_crossing"].GetBool() && St["cross_to"].Size())
                   || (!St["is_crossing"].GetBool() && !St["cross_to"].Size())) // Проверяем инфу о соседстве
            && St.HasMember("traffic")      && St["traffic"].IsInt()        && St["traffic"].GetInt() >= 0  // Проверяем инфу о траффике
            );
            // Station(int number, int av_traffic, std::string name)
            int tmpNum = St["number"].GetInt();
            if (tmpNum < 1000) tmpNum += 1000 * (i+1);  // Обеспечиваем уникальность номера станции таким вот прикольным способом
            if (St["is_crossing"].GetBool())  // Если у нас станция переходная
            {   // То создаем станцию
                //std::cout << "\n============================\n num: " << St["number"].GetInt() << "\ntraffic: " << St["traffic"].GetInt() << "\nname: " << St["name"].GetString() << "\n============================\n" << std::endl;

                CrossingStation tmpStation( tmpNum,
                                            St["traffic"].GetInt(),
                                            St["name"].GetString());

                for (unsigned int k = 0; k < St["cross_to"].Size(); k++) // И запихиваем в неё все станции пересадки.
                {
                    assert(St["cross_to"][k].IsString());
                    tmpStation.addCrossingStation(St["cross_to"][k].GetString());
                }
                tmpLine.addStationToLine(tmpStation);
                // tmpStation.printInfo();
                // tmpStation.printCrossStations();
            }
            else    // Иначе станция обычная, и с ней головной боли должно быть меньше
                {
                //std::cout << "\n============================\n num: " << St["number"].GetInt() << "\ntraffic: " << St["traffic"].GetInt() << "\nname: " << St["name"].GetString() << "\n============================\n" << std::endl;

                Station tmpStation(tmpNum,
                                   St["traffic"].GetInt(),
                                   St["name"].GetString());
                tmpLine.addStationToLine(tmpStation);       // Тут добавляем станцию в вектор
                // tmpStation.printInfo();
            }


        }
        tmpLine.connectLine();
        Lines.push_back(tmpLine);
    }
    return Lines;
}

int main() {
    std::vector<Line> Lines = {};
    Lines = getLines("/home/ka_7a/Desktop/MIPT/OOP_2/Saves/State.json", Lines);  // Возможно можно обойтись 1 переменной, мб - двумя. Просто хз как работает вектор
    /*
    for (int i = 0; i < Lines.size(); i++)
        Lines[i].printAllStationsInfo();
        */
    return 0;
}
