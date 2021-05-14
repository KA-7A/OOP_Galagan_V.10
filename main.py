import ctypes

SIZE = 65536
lib_railway = ctypes.CDLL('./lib/libmain.so')
lib_railway.e_init.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_char_p]
lib_railway.e_get_linename.argtypes = [ctypes.c_void_p, ctypes.c_void_p, ctypes.c_char_p, ctypes.c_int]
lib_railway.e_get_line.argtypes = [ctypes.c_void_p, ctypes.c_int]
lib_railway.e_get_ShortInfo.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_int]
lib_railway.e_get_FullInfo.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_int]

class RailwaySystem(object):
    def __init__(self, save_filename):
        error_msg = ctypes.create_string_buffer(50)
        filename = ctypes.c_char_p(save_filename.encode('utf-8'))

        RS = lib_railway.e_init(filename, 0, error_msg)
        while True:
            self.printMenu()
            choice = int(input(">> "))
            try:
                if choice == 0:
                    lib_railway.e_finish(RS)
                    return
                elif choice == 98:
                    pass
                elif choice == 2:
                    pass
                elif choice == 1:
                    try:
                        linenames = self.getLineNames(RS)
                        for i in range(len(linenames)):
                            print(i, ".", str(linenames[i]))
                        linechoice = int(input(">> "))
                        msg = ctypes.create_string_buffer(SIZE)
                        line = lib_railway.e_get_line(RS, linechoice)
                        res = lib_railway.e_get_FullInfo(line, msg, SIZE)
                        if res == 0:
                            to_out = (str(msg.value)).split(sep="\\n")
                            for i in range(len(to_out)):
                                print(to_out[i])
                            # print(msg.value)  # Вот тут внутри msg.value лежит b'string', не знаю как это поправить
                        else:
                            print("Error")
                    except Exception as e:
                        print(e)
            except Exception as e:
                print("Incorrect input!")

    def printMenu(self):
        print(" ************** Menu **************")
        print(" * 1.  Show all stations          *")
        print(" * 2.  Calculate travel time      *")
        print(" *                                *")
        print(" * 98. Do timetable tests!        *")
        print(" *                                *")
        print(" * 0. Exit                        *")
        print(" **********************************")

    def getLineNames(self, RS):
        linenames = []
        amount = lib_railway.e_get_AmountOfLines(RS)
        for i in range(amount):
            msg = ctypes.create_string_buffer(SIZE)
            res = lib_railway.e_get_linename(RS, i, msg, SIZE)
            linenames.append(msg.value)
        return linenames



if __name__ == '__main__':
    filename = './Saves/State.json'
    menu = RailwaySystem(filename)


