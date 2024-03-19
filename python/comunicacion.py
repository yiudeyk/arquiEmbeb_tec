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
    print('El puerto esta abierto')
    # while True:
    #     r = puertoSerie.read(1)
    #     print(r, int.from_bytes(r))
    numero = 2
    cadena[0]= numero

    # enviando al microntrontrolador
    puertoSerie.write(cadena)
    time.sleep(0.01)
    # leyendo del microcontrolador
    r = puertoSerie.read(25)
    print(r, int.from_bytes(r))

puertoSerie.close()