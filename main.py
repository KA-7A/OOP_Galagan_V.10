import ctypes
lib_railway = ctypes.CDLL('./lib/libmain.so')

lib_railway.e_init.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_char_p]
# lib_railway.e_init.argtypes = [ctypes.c_char_p, ctypes.c_int]
lib_railway.e_consoleMenu.argtypes = [ctypes.c_void_p]

error_msg = ctypes.create_string_buffer(50)
filename = ctypes.c_char_p('./Saves/State.json'.encode('utf-8'))

RS = lib_railway.e_init(filename, 0, error_msg)
if len(error_msg.value) == 0:

else:
    lib_railway.e_finish(RS)


