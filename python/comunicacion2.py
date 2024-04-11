
import serial
import time

class Comunicacion:
    def __init__(self, nombre, baudrate):
        # --------------------------
        self.puertoSerie = serial.Serial()
        self.puertoSerie.port = nombre #"COM3"
        self.puertoSerie.baudrate = baudrate #9600
        self.puertoSerie.parity = serial.PARITY_NONE
        self.puertoSerie.timeout = 1
        self.puertoSerie.stopbits = serial.STOPBITS_ONE
        self.puertoSerie.bytesize = serial.EIGHTBITS

    def abrirPuerto(self):
        self.puertoSerie.open()

    def cerrarPuerto(self):
        self.puertoSerie.close()

    def leerPuerto(self):
        pass

    def escribir(self):
        pass

    def enviar_mensaje(self, instruccion):
        cadena = bytearray(1)
        r = None
        if self.puertoSerie.is_open:
            cadena[0] = instruccion

            # enviando al microntrontrolador
            self.puertoSerie.write(cadena)
            time.sleep(0.10)
            # leyendo del microcontrolador
            r = self.puertoSerie.read(8)
            print(r, int.from_bytes(r))
        
        #return r    

    def probarPuerto(self):
        cadena = bytearray(1)   #en este caso va vacia la cadena pero puede llevar información

        if self.puertoSerie.is_open:
            print('\nPuerto abierto. Leyendo... \n')

            # ENVIANDO al microntrontrolador
            self.puertoSerie.write(cadena)   #función necesaria para establecer la comunicación y poder leer el serial
            time.sleep(1.0)             #tiempo para lea (reciba) la información del serial

            # LEYENDO del microcontrolador
            r = self.puertoSerie.read(40)
            print(r)


def main():
    comu = Comunicacion("COM3", 9600)
    comu.abrirPuerto()
    comu.probarPuerto()
    comu.cerrarPuerto()

if __name__ == "__main__":
    main()