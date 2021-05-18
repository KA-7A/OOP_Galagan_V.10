//
// Created by Администратор on 30.04.2021.
//

#include "railway_system.h"
#define NO_SUCH_FILE -1
#define NOT_A_JSON -2
#define INCORRECT_CONTENT -3


int railway_system::lineInit(const char *filename, int mode) {
    // -- Тут идет блок с открытием файла и парсингом его в объект, с которым дальше нужно будет работать -- //
//--- Parsing from file to object ---//
    FILE *source = fopen(filename, "r");
    if (!source) return NO_SUCH_FILE;
    char readBuffer[65536];
    FileReadStream is(source, readBuffer, sizeof(readBuffer));

    Document linesObj;
    linesObj.ParseStream(is);

    fclose(source);
//--- The end of Parsing ---//

    // -- Закрыли файл, теперь у нас есть объект linesObj, в котором, по идее, должен лежать корректный набор данных -- //
    // -- Дальше нужно заполнить список
    // -- Но сначала сделаем пару проверок
    if (!linesObj.IsObject()) return NOT_A_JSON;        // Проверяем, что то, что у нас есть -- это корректный JSON
    if (!(linesObj.HasMember("lines") &&
          linesObj["lines"].Size()))
    {
        std::cout << "wtf";
        return INCORRECT_CONTENT ; // В котором есть непустой элемент lines (в нем названия линий)
    }


    for (unsigned int i = 0; i < linesObj["lines"].Size(); i++)       // Идем по линиям
    {
        if (!linesObj["lines"][i].IsString()) return INCORRECT_CONTENT;    // Проверили, строка ли тут у нас
        std::string lineName = linesObj["lines"][i].GetString();

        if (! linesObj.HasMember(lineName.c_str())) return INCORRECT_CONTENT; // Проверяем, что у нас есть такой блок
        if (!(linesObj[linesObj["lines"][i]].HasMember("stations") && linesObj[linesObj["lines"][i]].HasMember(
                "spans"))) return INCORRECT_CONTENT;    // Проверяем, что в нашем блоке-линии есть необходимые элементы
        // Вот это потенциально сложный для осознания момент:
        const Value &line = linesObj[linesObj["lines"][i]]["stations"];    // Делаем объект, состоящий из станций
        const Value &spans = linesObj[linesObj["lines"][i]]["spans"];       // Делаем объект, состоящий из перегонов
        // std::cout << "line.Size = " << line.Size() << "; spans.Size - 1 = " << spans.Size() - 1 << std::endl;
        if (line.Size() != (spans.Size() - 1)) return INCORRECT_CONTENT;    // Проверяем, что перегонов на 1 больше, чем станци//
        // (2 мнимых перегона с нулевым временем по краям ветки)
        /*      Запутанное объяснение, можно не читать)
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
        if (mode == LNKD_LIST)
            m_Lines.push_back(new Line);
        else if (mode == SKIP_LIST)
            m_Lines.push_back(new S_Line);
// ---------- Here is Spans filling block ------------------ //
        //std::cout << "lineNum == "<< i <<"; lineName is "<< lineName << ";" << std::endl;
        std::vector<Span> span_vector;

        for (unsigned int j = 0; j < spans.Size(); j++)  // Тут мы заполнили вектор из
        {
            const Value &Sp = spans[j];
            if (!(Sp.IsObject()                                  // Проверяем, что это у нас объект
                   && Sp.HasMember("left") && Sp["left"].IsString()     // Что в нем есть строковое поле "left"
                   && Sp.HasMember("right") && Sp["right"].IsString()    // Что в нём есть строковое поле "right"
                   // Что там есть поле время с неотрицательным значением
                   && Sp.HasMember("min_time") && (Sp["min_time"].IsDouble() || Sp["min_time"].IsInt()) &&
                   Sp["min_time"].GetDouble() >= 0
                   && Sp.HasMember("max_time") && (Sp["max_time"].IsDouble() || Sp["max_time"].IsInt()) &&
                   Sp["max_time"].GetDouble() >= 0
            )) return INCORRECT_CONTENT;
            Span tmpSpan(Sp["left"].GetString(), Sp["right"].GetString(), Sp["min_time"].GetDouble(),
                         Sp["max_time"].GetDouble());
            m_Lines[i]->spanPushBack(tmpSpan);
        }
// ---------- Here is m_Lines filling and connection block -- //
        // -- Вот до этого момента мы загнали линию в отдельный объект. -- //

        m_Lines[i]->setName(lineName);    // Сохранили название ветки
        for (unsigned int j = 0; j < line.Size(); j++) {     // Идем по станциям в этой линии

            const Value &St = line[j];
            // Document st = linesObj[lineName.c_str()][j];
            // Проверяем, что объект, который мы тут читаем, вообще подходит под формат станции (под нужный нам формат)
            // Защищаемся вообще от всего. Вот вообще.
            if (!(St.IsObject()
                    && St.HasMember("name") &&
                    St["name"].IsString()                                        // Проверяем, что у нас есть имя, и что это строка
                    && St.HasMember("number") && St["number"].IsInt() &&
                    St["number"].GetInt() >= 0   // Проверяем, что у нас есть корректный номер станции
                    && St.HasMember("neighbours") && St["neighbours"].IsArray() &&
                    St["neighbours"].Size() == 2 // Проверяем, что у станции есть ровно 2 соседа по линии
                    && St.HasMember("is_crossing") &&
                    St["is_crossing"].IsBool()                                   // Проверяем, что есть инфа о том, пересадочная ли станция
                    && St.HasMember(
                    "cross_to")                                                                     // Проверяем, что есть инфа о пересадках (даже если станция непересадочная)
                    && ((St["is_crossing"].GetBool() &&
                         St["cross_to"].Size())   // Если пересадочная, то кол-во пересадок не ноль
                        || (!St["is_crossing"].GetBool() &&
                            !St["cross_to"].Size())) // Если непересадочная, то кол-во пересадок ноль
                    && St.HasMember("traffic") && St["traffic"].IsInt() &&
                    St["traffic"].GetInt() >= 0  // Проверяем инфу о траффике
            )) return INCORRECT_CONTENT;
            // Station(int number, int av_traffic, std::string name)
            int tmpNum = St["number"].GetInt();
            if (tmpNum < 1000) tmpNum += 1000 * (i + 1);
            /* Для удобства в исходном файле номера станций порядковые
             * Чтобы они были уникальными внутри программы, делаем вот такую вот шутку
             * Работает только если станций в линии не больше тысячи
             * Нет проверки, что в исходном файле номера внутри одной ветки не дублируются
             */
            if (St["is_crossing"].GetBool())  // Если у нас станция пересадочная
            {
                if (mode == LNKD_LIST) {
                    m_Lines[i]->addStationToLine(
                            new CrossingStation(tmpNum, St["traffic"].GetInt(), St["name"].GetString()));
                } else if (mode == SKIP_LIST) {
                    m_Lines[i]->addStationToLine(
                            new S_CrossingStation(tmpNum, St["traffic"].GetInt(), St["name"].GetString()));
                }
                for (unsigned int k = 0;
                     k < St["cross_to"].Size(); k++) // И запихиваем в неё все станции пересадки.
                {
                    if (!St["cross_to"][k].IsString()) return INCORRECT_CONTENT;
                    m_Lines[i]->getEndStation()->addCrossingStation(St["cross_to"][k].GetString());
                }

            } else    // Иначе станция обычная, и с ней головной боли сильно меньше.
            if (mode == LNKD_LIST)
                m_Lines[i]->addStationToLine(new Station(tmpNum, St["traffic"].GetInt(), St["name"].GetString()));
            else if (mode == SKIP_LIST)
                m_Lines[i]->addStationToLine(new S_Station(tmpNum, St["traffic"].GetInt(), St["name"].GetString()));
        }
        m_Lines[i]->connectLine();     // Там происходит сортировка внутри вектора и связывание всех станций в ветку
    }
    return 0;
}

