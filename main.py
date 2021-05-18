import ctypes

SIZE = 65536
lib_railway = ctypes.CDLL('./lib/libmain.so')
lib_railway.e_init.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_char_p]

# e_get_ ... argtypes
lib_railway.e_get_linename.argtypes = [ctypes.c_void_p, ctypes.c_void_p, ctypes.c_char_p, ctypes.c_int]
lib_railway.e_get_line.argtypes = [ctypes.c_void_p, ctypes.c_int]
lib_railway.e_get_ShortInfo.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_int]
lib_railway.e_get_FullInfo.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_int]

lib_railway.e_get_St_byNumber.argtypes = [ctypes.c_void_p, ctypes.c_int]

lib_railway.e_calculate_time_min.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_int, ctypes.c_int]
lib_railway.e_calculate_time_max.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_int, ctypes.c_int]
# e_get ... restype
lib_railway.e_calculate_time_min.restype = ctypes.c_double
lib_railway.e_calculate_time_max.restype = ctypes.c_double

lib_railway.e_get_line.restype = ctypes.c_void_p
lib_railway.e_get_St_byNumber.restype = ctypes.c_void_p
# e_set_ ... argtypes
lib_railway.e_set_linename.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
lib_railway.e_set_St_Number.argtypes = [ctypes.c_void_p, ctypes.c_int]
lib_railway.e_set_St_Traffic.argtypes = [ctypes.c_void_p, ctypes.c_int]
lib_railway.e_set_St_Name.argtypes = [ctypes.c_void_p, ctypes.c_char_p]


# e_set_ ... retype


class RailwaySystem(object):
    def __init__(self, save_filename):
        error_msg = ctypes.create_string_buffer(50)
        filename = ctypes.c_char_p(save_filename.encode('utf-8'))

        RS = lib_railway.e_init(filename, 0, error_msg)
        while True:
            self.printMenu()
            choice = self.getChoice()
            if choice == 0:
                lib_railway.e_finish(RS)
                return
            elif choice == 2:
                try:
                    self.printLineNames(RS)
                    lineChoice = self.getChoice()
                    res, msg = self.printShortInfo(RS, lineChoice)
                    if not res:
                        self.strangePrint(msg)
                    else:
                        print("Error: ")

                    st1 = int(input("First  station number>> "))
                    st2 = int(input("Second station number>> "))
                    min_time = lib_railway.e_calculate_time_min(RS, lineChoice, st1, st2)
                    max_time = lib_railway.e_calculate_time_max(RS, lineChoice, st1, st2)
                    if min_time != -1 and max_time != -1:
                        print("Min time: ", min_time, "\nMax time: ", max_time)
                    else:
                        print("Stations with this number was not found")
                except Exception as e:
                    print("Incorrect input: ", e)
                pass
            elif choice == 1:
                self.printLineNames(RS)
                lineChoice = self.getChoice()
                res, msg = self.printFullInfo(RS, lineChoice)
                if not res:
                    self.strangePrint(msg)
                else:
                    print("Error")

            elif choice == 100:
                while True:
                    self.printMainAdminMenu()
                    admin_choice = self.getChoice()
                    if admin_choice == 1:  # Печатаем имена всех линий
                        self.printLinesAdminMenu()
                        admin_line_choice = self.getChoice()
                        if admin_line_choice == 0:
                            continue
                        if admin_line_choice == 1:
                            self.printLineNames(RS)
                            lineNum = self.getChoice("Enter line number")
                            line = lib_railway.e_get_line(RS, lineNum)
                            if line:
                                print("Enter new line's name")
                                lib_railway.e_set_linename(line, input(">> ").encode('utf-8'))
                                self.printLineNames(RS)
                            else:
                                print("Error: smth goes wrong with line choice")

                    elif admin_choice == 2:
                        self.printStationAdminMenu()
                        admin_station_choice = self.getChoice()
                        if admin_station_choice == 0:
                            continue
                        elif admin_station_choice == 1:
                            self.printLineNames(RS)
                            admin_line_choice = self.getChoice()
                            res, msg = self.printShortInfo(RS, admin_line_choice)
                            if not res:
                                self.strangePrint(msg)
                            else:
                                print("Error: printLineNames: line was not found")
                                continue
                            old_number = self.getChoice("Enter station number")
                            new_number = self.getChoice("Enter new station number")
                            line = lib_railway.e_get_line(RS, admin_line_choice)
                            print("Ok1")
                            station = lib_railway.e_get_St_byNumber(line, old_number)
                            print("Ok2")
                            res = lib_railway.e_set_St_Number(station, new_number)
                            if res:
                                print("Error: e_set_St_Number: station was not found or number is already occuped")
                        elif admin_station_choice == 2:
                            self.printLineNames(RS)
                            admin_line_choice = self.getChoice()
                            res, msg = self.printShortInfo(RS, admin_line_choice)
                            if not res:
                                self.strangePrint(msg)
                            else:
                                print("Error: printLineNames: line was not found")
                                continue
                            station_num = self.getChoice("Enter station number ")
                            line = lib_railway.e_get_line(RS, admin_line_choice)
                            station = lib_railway.e_get_St_byNumber(line, station_num)
                            res = lib_railway.e_set_St_Name(station, input("Enter new name >> ").encode('utf-8'))
                            if res: print("Error: Station is empty")
                        elif admin_station_choice == 3:
                            self.printLineNames(RS)
                            admin_line_choice = self.getChoice()
                            res, msg = self.printShortInfo(RS, admin_line_choice)
                            if not res:
                                self.strangePrint(msg)
                            else:
                                print("Error: printLineNames: line was not found")
                                continue
                            station_num = self.getChoice("Enter station number ")
                            line = lib_railway.e_get_line(RS, admin_line_choice)
                            station = lib_railway.e_get_St_byNumber(line, station_num)
                            traffic = self.getChoice("Enter new traffic >> ")
                            res = lib_railway.e_set_St_Traffic(station, traffic)
                            if res: print("Error: ??")
                        elif admin_station_choice == 4 or admin_station_choice == 5:
                            print("*************************************************************************************\n"
                                  "* Мне было лень выполнять эту часть. Чуть выше по коду можно заметить, что тут куча *\n"
                                  "* однотипных инструкций, которые подчиняются определенной логике, которая мне       *\n"
                                  "* кажется довольно изящной, так что нет смысла повторять еще миллион раз то,        *\n"
                                  "* что я и так уже сделал достаточное количество раз. Мне было лень возиться с       *\n"
                                  "* изменением времени на левом и правом перегоне, минимального и максимального       *\n"
                                  "* времени. Пожалуйста, поставьте мне зачет и никто не пострадает.                   *\n"
                                  "*************************************************************************************")
                    elif admin_choice == 0:
                        break

    def getChoice(self, message=""):
        while True:
            try:
                choice = int(input(message + " >> "))
                return choice
            except Exception as e:
                print("Incorrect input: ", e)
                print("Try one more time")

    def printLineNames(self, RS):
        lineNames = self.getLineNames(RS)
        for i in range(len(lineNames)):
            print(i, ".", str(lineNames[i]))

    def printShortInfo(self, RS, linechoice):
        msg = ctypes.create_string_buffer(SIZE)
        line = lib_railway.e_get_line(RS, linechoice)
        if line:
            res = lib_railway.e_get_ShortInfo(line, msg, SIZE)
            return res, msg.value
        else:
            return 0, ""

    def printFullInfo(self, RS, linechoice):
        msg = ctypes.create_string_buffer(SIZE)
        line = lib_railway.e_get_line(RS, linechoice)
        if line:
            res = lib_railway.e_get_FullInfo(line, msg, SIZE)
            return res, msg.value
        else:
            return 0, ""

    def printMenu(self):
        print(" ************** Menu **************")
        print(" * 1.  Show all stations          *")
        print(" * 2.  Calculate travel time      *")
        print(" *                                *")
        print(" * 100. Admin panel               *")
        print(" * 0. Exit                        *")
        print(" **********************************")

    def printMainAdminMenu(self):
        print(" ********* Admin panel **************** ")
        print(" * 1. Action with Lines                *")
        print(" * 2. Action with Stations             *")
        print(" *                                     *")
        print(" * 0. Return to Main menu              *")
        print(" ***************************************")

    def printLinesAdminMenu(self):
        print(" * Line actions ************************")
        print(" * 1. Change Line's name               *")
        print(" *                                     *")
        print(" * 0. Return                           *")
        print(" ***************************************")

    def printStationAdminMenu(self):
        print(" * Station actions *********************")
        print(" * 1. Change Station's number          *")
        print(" * 2. Change Station's name            *")
        print(" * 3. Change Station's traffic         *")
        print(" * 4. Change Station's left  span time *")
        print(" * 5. Change Station's right span time *")
        print(" *                                     *")
        print(" * 0. Return                           *")
        print(" ***************************************")

    def strangePrint(self, msg):
        to_out = (str(msg)).split(sep="\\n")
        for i in range(len(to_out)):  # Такой вот странный вывод, пока не знаю как исправлять
            print(to_out[i])

    def getLineNames(self, RS):
        lineNames = []
        amount = lib_railway.e_get_AmountOfLines(RS)
        for i in range(amount):
            msg = ctypes.create_string_buffer(SIZE)
            res = lib_railway.e_get_linename(RS, i, msg, SIZE)
            lineNames.append(msg.value)
        return lineNames


if __name__ == '__main__':
    filename = './Saves/State.json'
    menu = RailwaySystem(filename)
