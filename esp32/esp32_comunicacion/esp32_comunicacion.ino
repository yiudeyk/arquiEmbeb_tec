
#define luzLed 8    //luz led que trae consigo la placa

// ----------  Señales digitales
#define numeroDeX 16
byte X[numeroDeX];

int DI_00 = 21;
int DI_01 = 20;
int DI_02 = 10;
int DI_03 = 9;

#define numeroDeY 16
byte Y[numeroDeY];

int DO_00 = 7;
int DO_01 = 6;
int DO_02 = 5;
int DO_03 = 4;
int DO_04 = 3;
int DO_05 = 2;    

// ----------  Temporizadores
#define  numeroDeTON 16

struct temporizador {
  byte entrada;
  byte salida;
  unsigned long tiempo;
  unsigned long tiempoActual;
} TON[numeroDeTON];

struct temporizadorAux {
  byte bandera;
  unsigned long tiempo_Aux1;
  unsigned long tiempo_Aux2;
} TON_Aux[numeroDeTON];

void actualizarTON (int);

// ----------  Comunicación
/*
    00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20
   |CI| I  N V | TI  | NUM | LON | DATOS ...........        |V |CF| 
    CI  - Caracter de inicio
    INV - Para invernadero
    TI  - Tipo de instrucción
    NUM - Numero de instrucción
    LON - Longitud
    V   - Verificación
    CF  - Caracter de final
 */

#define bufferIndiceMaximo  40

byte bufferLectura[bufferIndiceMaximo];
int bufferIndice = 0;

byte bufferUltimaInstruccion[bufferIndiceMaximo];
int tamanioBufferUltimaInstruccion = 0;

char caracterDeInicio = '@';
char caracterDeFin = '$';

//prototipos de función
void colocarDatosEnBufferSerial(void);
void leerInstruccionesDeBufferSerial(byte *, int *, byte *, int *);
//void imprimirTrama(byte *, int, int);
void leerInstruccion(int);

int contador = 0;
String infoSerial = "";   //cadena que contendra la información extraida del serial
bool imprimirInfo = false;  //indica si se extrajo información del serial para ser impresa

//---------------------------------------------------------------------------------------------------------------------------------------------

void setup() 
{  
  pinMode(DO_00, OUTPUT);
  pinMode(DO_01, OUTPUT);
  pinMode(DO_02, OUTPUT);
  pinMode(DO_03, OUTPUT);
  pinMode(DO_04, OUTPUT);
  pinMode(DO_05, OUTPUT);

  pinMode(DI_00, INPUT);
  pinMode(DI_01, INPUT);
  pinMode(DI_02, INPUT);
  pinMode(DI_03, INPUT);

  TON[0].tiempo = (unsigned long)1 * 1000;     // 1 segundo
  TON[1].tiempo = (unsigned long)1 * 400;      // 0.4 segundos
  TON[2].tiempo = (unsigned long)1 * 800;      // 0.8 segundos

  Serial.begin(9600);
  delay(1000);

  Serial.print("\nPrograma iniciado \n");   //mensaje de prueba solo para indicar que se ejecuto todo el setUp
}

void loop() 
{
  colocarDatosEnBufferSerial();   //si se encuentra información en el serial se extrae

  imprimirTrama2(bufferLectura, bufferIndice, &imprimirInfo);

  //leerInstruccionesDeBufferSerial(bufferLectura, &bufferIndice, bufferUltimaInstruccion, &tamanioBufferUltimaInstruccion);
  
  // TON[0].entrada = !TON[0].salida;         //la salida es lo contrario de la entrada
  // actualizarTON(0);

  // if (TON[0].salida) 
  // {
  //   contador += 1;
  // }

  // // Parpadeo indicando que el programa esta funcionando
  // TON[1].entrada = !TON[2].salida;
  // actualizarTON(1);

  // TON[2].entrada = TON[1].salida;
  // actualizarTON(2);

  // Y[5] = TON[1].salida & !TON[2].salida;

  // X[0] = digitalRead(DI_00);
  // X[1] = digitalRead(DI_01);
  // X[2] = digitalRead(DI_02);
  // X[3] = digitalRead(DI_03);

  // digitalWrite(DO_00, Y[0]);
  // digitalWrite(DO_01, Y[1]);
  // digitalWrite(DO_02, Y[2]);
  // digitalWrite(DO_03, Y[3]);
  // digitalWrite(DO_04, Y[4]);
  // digitalWrite(DO_05, Y[5]);

  //Serial.print("contador: ");
  //Serial.write(contador);
  //Serial.print("\n");

  //Serial.print ("TON0");
  //Serial.print("\t");
  //Serial.print(TON[0].tiempoActual);
  //Serial.print("\t");
  //Serial.print(TON[0].salida);
  //Serial.print("\n");
  delay(1000);
}

//----------------------------------------------------------------------------------------------------------------------------------------------

// ----------- extrae valores que hayan sido colocados en el serial hacia un arreglo 
void colocarDatosEnBufferSerial()
{
  byte caracter;            //servira para extraer caracter a caracter
  int aux = 0;              //
  imprimirInfo = false;     //se reinicializa la variable

  while (Serial.available())    //mientras hay información en el serial se extrae
  {
    imprimirInfo = true;    //si hay información se marca que se imprima lo que se extraiga del serial

    caracter = Serial.read();   //se lee caracter a caracter la información en el serial
    
    //infoSerial += (char)caracter;
    bufferLectura[bufferIndice++] = caracter;   //se va guardando la información en un 

    if (bufferIndice >= bufferIndiceMaximo)  //si ya llenaron todos los lugares del arreglo
    {      
      aux = bufferIndiceMaximo >> 1;    //tomamos la mitad del valor de 'bufferIndiceMaximo' 
      //infoSerial = "";        //guardamos el último caracter para que no se pierda al hacer el corte

      //mediante el for se pasan los valores de la segunda mitad del arreglo hacia la primer mitad 
      //'i' necesita alcanzar el valor de 'bufferIndiceMaximo' ya que 'aux' representa su mitad
      //y al ser restados se debe poder obtener el valor 20 y así no perder el valor de esa posición
      for (int i = aux; i <= bufferIndiceMaximo; i++)  
      {
        bufferIndice = i - aux;                           //bufferIndice toma el valor de 0 en un principio
        bufferLectura[bufferIndice] = bufferLectura[i];   //se guardan los valores de la segunda mitad del arreglo
        //infoSerial += (char)bufferLectura[i];
      }
    }
  }
}

// se imprime un arreglo de bytes desde la posición '0' hasta la posición 'tamanio'
void imprimirTrama2(byte *trama, int tamanio, bool *verify)
{
  //'verify' útil cuando la impresión de un valor se basa en el valor de un 'bool'
  //en este caso sirve como apoyo ya que solo se quiere imprimir el mensaje una vez
  if(*verify)
  {
    Serial.print("\n>> ");    // indicador que la información viene del serial

    // Iterar sobre el búfer de bytes y agregar cada byte a la cadena
    for (int i = 0; i < tamanio; i++) 
    {
        // Convertir cada byte a un carácter y agregarlo a la cadena
        Serial.write( (char)trama[i] );
        delay(30);
    }

    *verify = false;   //para que no se imprima constantemente 
  }
}

//funcion del maestro
void imprimirTrama(byte *ptrTrama, int inicio, int tamanio) 
{
  Serial.print("\n>>");

  for ( int k = inicio; k < inicio + tamanio; k++ ) 
  {
    Serial.write( ptrTrama[ k ] );
  }
}

// función del maestro limpiada y documentada
void leerInstruccionesDeBufferSerial(byte *ptrBufferLectura, int *ptrBufferIndice, 
                                      byte *ptrBufferUltimaInstruccion, int *ptrTamanioBufferUltimaInstruccion)
{
  int k = 0;
  int encontrado = *ptrBufferIndice - 1;

  //se requiere que el 'caracterDeFin' se encuentre al final de la instrucción para que se active este bloque
  if ( ptrBufferLectura[ encontrado ] == caracterDeFin )
  {
    for ( k = *ptrBufferIndice; k >= 0; --k ) //se recorre el arreglo en reversa
    {
      if ( ptrBufferLectura[ k ] == (byte)caracterDeInicio )  //en busca del 'caracterDeInicio'
      {
        encontrado = k;         //cuando se encuentra el 'caracterDeInicio' se guarda la posición 
        if ( encontrado >= 0 )    
        {
          *ptrTamanioBufferUltimaInstruccion = 0;       //

          //se recorre ahora desde donde se encontro el inicio de la instruccion hasta el final
          for ( int j = k; j < *ptrBufferIndice; j++ )    
          {
            //se guarda toda la instrucción recibida en el buffer de última instrucción 
            ptrBufferUltimaInstruccion[ *ptrTamanioBufferUltimaInstruccion ] = ptrBufferLectura[j];
            (*ptrTamanioBufferUltimaInstruccion)++;
          }

          // se imprime la instrucción encontrada 
          imprimirTrama(ptrBufferUltimaInstruccion, 0, *ptrTamanioBufferUltimaInstruccion);

          //con la siguiente instrucción se impide que el mensaje se imprima continuamente ya que volverá a ingresar
          //a esta función pero con el valor de la posición donde se encontró el 'caracterDeInicio' por lo que al 
          //recorrerse en reversa a partir de esta posición no se encontrará con el 'caracterDeFin' 
          *ptrBufferIndice = k; 
        }
      }
    }
  }
}

//
int obtenerListaDeInstrucciones()
{
  int *tamanio = 0;
  byte *cadena;
  int salida = 0;
  int tipoDeInstruccion = 0;
  int numeroDeInstruccion = 0;

  tamanio = &tamanioBufferUltimaInstruccion;
  cadena = bufferUltimaInstruccion;
}

//
void leerInstruccion(int caracter) {
      switch (caracter) {
        case 1:
          Serial.print("Instruccion 1");
          break;

        case 2:
          Serial.print("Instruccion 2");
          break;

        case 3:
          Serial.print("Instruccion 3");
          break;

         
          
    }
}


void actualizarTON (int i)
{
  if (TON [i].entrada)
  {
    if (!TON_Aux[i].bandera) 
    {
      TON_Aux[i].bandera = true;
      TON_Aux[i].tiempo_Aux1 = millis ();   //se toma la referencia de tiempo
    }

    TON_Aux[i].tiempo_Aux2 = millis ();
    TON [i].tiempoActual = TON_Aux[i].tiempo_Aux2 - TON_Aux[i].tiempo_Aux1;

    if (TON [i].tiempoActual > TON [i].tiempo) 
    {
      TON [i].salida = true;
    }
  } 
  else 
  {
    TON [i].salida = false;
    TON_Aux[i].bandera = false;
  }
}