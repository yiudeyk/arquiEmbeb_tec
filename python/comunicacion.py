import serial
import time

# --------- Opcional -------
# Para listar la lista de puertos disponibles 
import serial.tools.list_ports

ports = list(serial.tools.list_ports.comports())
for port in ports:
    print(port)
# --------------------------

puertoSerie = serial.Serial()

puertoSerie.port ="COM3"
puertoSerie.baudrate = 9600
puertoSerie.parity = serial.PARITY_NONE
puertoSerie.timeout = 1
puertoSerie.stopbits = serial.STOPBITS_ONE
puertoSerie.bytesize = serial.EIGHTBITS

cadena = bytearray(1)

puertoSerie.open()
time.sleep(2)

if puertoSerie.is_open:
    print('\nPuerto abierto. Leyendo...   \n')

    # ENVIANDO al microntrontrolador
    puertoSerie.write(cadena)   #función necesaria para establecer la comunicación y poder leer el serial
    time.sleep(1.0)             #tiempo para lea (reciba) la información del serial

    # LEYENDO del microcontrolador
    r = puertoSerie.read(40)
    print(r)

    puertoSerie.close()









