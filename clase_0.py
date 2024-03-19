
import sys
from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QApplication, QMainWindow, QPushButton, QWidget, QLineEdit, QVBoxLayout, QLabel, QHBoxLayout
import python.comunicacion2 as comunic

class Ventana(QMainWindow):
    def __init__(self) -> None:
        super().__init__()

        boton = QPushButton("Escribir")
        self.texto = QLineEdit()
        self.etiquetaString = QLabel()
        etiquetaHex = QLabel()

        layoutVertical1 = QVBoxLayout()
        layoutHorizontal1 = QHBoxLayout()
        layoutHorizontal2 = QHBoxLayout()

        layoutVertical1.addLayout(layoutHorizontal1)
        layoutVertical1.addLayout(layoutHorizontal2)

        layoutHorizontal1.addWidget(self.texto)
        layoutHorizontal1.addWidget(boton)

        layoutHorizontal2.addWidget(self.etiquetaString)
        layoutHorizontal2.addWidget(etiquetaHex) 

        self.etiquetaString.setStyleSheet(f'background-color: white')
        etiquetaHex.setStyleSheet(f'background-color: white')

        widget = QWidget()
        widget.setLayout(layoutVertical1)

        self.setCentralWidget(widget)

        boton.clicked.connect(self.accionBoton1)

    def accionBoton1(self):
        print("boton clickeado")

        comu = comunic.Comunicacion('COM3', 9600)
        comu.abrirPuerto()

        respuesta = comu.enviar_mensaje(int(self.texto.text()))
        #self.etiquetaString.text(respuesta)
        print('La respuesta recibida es ', respuesta)

        comu.cerrarPuerto()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = Ventana()
    window.show();
    sys.exit(app.exec())
       
