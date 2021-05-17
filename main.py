import ctypes

SIZE = 65536
lib_railway = ctypes.CDLL('./lib/libmain.so')
lib_railway.e_init.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_char_p]
lib_railway.e_get_linename.argtypes = [ctypes.c_void_p, ctypes.c_void_p, ctypes.c_char_p, ctypes.c_int]
lib_railway.e_get_line.argtypes = [ctypes.c_void_p, ctypes.c_int]
lib_railway.e_get_ShortInfo.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_int]
lib_railway.e_get_FullInfo.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_int]

lib_railway.e_calculate_time_min.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_int, ctypes.c_int]
lib_railway.e_calculate_time_max.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_int, ctypes.c_int]

lib_railway.e_calculate_time_min.restype = ctypes.c_double
lib_railway.e_calculate_time_max.restype = ctypes.c_double




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
                    lineSize = len(self.getLineNames(RS))
                    lineChoice = int(input(">> "))
                    if lineChoice >= lineSize or lineChoice < 0:
                        print("Incorrect input")
                        continue
                    res, msg = self.printShortInfo(RS, lineChoice)
                    if not res:
                        to_out = (str(msg)).split(sep="\\n")
                        for i in range(len(to_out)):    # Такой вот странный вывод, пока не знаю как исправлять
                            print(to_out[i])
                    else:
                        print("Error")

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
                lineChoice = self.getChoice()
                self.printLineNames(RS)
                lineSize = len(self.getLineNames(RS))
                lineChoice = int(input(">> "))
                if lineChoice >= lineSize or lineChoice < 0:
                    print("Incorrect input")
                    continue
                res, msg = self.printFullInfo(RS, lineChoice)
                if not res:
                    to_out = (str(msg)).split(sep="\\n")
                    for i in range(len(to_out)):    # Такой вот странный вывод, пока не знаю как исправлять
                        print(to_out[i])
                else:
                    print("Error")

            elif choice == 100:
                while True:
                    self.printMainAdminMenu()
                    admin_choice = self.getChoice()
                    if admin_choice == 1:   # Печатаем имена всех линий
                        self.printLineNames(RS)
                        print("Enter line number")
                        lineNum = self.getChoice()
                        lib_railway.e_

                    elif admin_choice == 2:
                        self.printStationAdminMenu()
                    elif admin_choice == 0:
                        break


    def getChoice(self):
        while True:
            try:
                choice = int(input(">> "))
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
        res = lib_railway.e_get_ShortInfo(line, msg, SIZE)
        return res, msg.value

    def printFullInfo(self, RS, linechoice):
        msg = ctypes.create_string_buffer(SIZE)
        line = lib_railway.e_get_line(RS, linechoice)
        res = lib_railway.e_get_FullInfo(line, msg, SIZE)
        return res, msg.value

    def printMenu(self):
        print(" ************** Menu **************")
        print(" * 1.  Show all stations          *")
        print(" * 2.  Calculate travel time      *")
        print(" *                                *")
        print(" * 100. Admin panel               *")
        print(" * 0. Exit                        *")
        print(" **********************************")

    def printMainAdminMenu(self):
        print(" ********* Admin panel *********** ")
        print(" * 1. Action with Lines           *")
        print(" * 2. Action with Stations        *")
        print(" *                                *")
        print(" * 0. Return to Main menu         *")
        print(" **********************************")

    def printLinesAdminMenu(self):
        print(" * Line actions *******************")
        print(" * 1. Change Line's name          *")
        print(" **********************************")

    def printStationAdminMenu(self):
        print(" * Station actions *********************")
        print(" * 1. Change Station's name            *")
        print(" * 2. Change Station's traffic         *")
        print(" * 3. Change Station's left span time  *")
        print(" * 4. Change Station's right span time *")
        print(" ***************************************")


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
