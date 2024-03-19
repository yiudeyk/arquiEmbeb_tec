
#define botonActivar_TON 37   //se define el numero de pin donde se conectará el botón físico
#define led_TON 39            //se define el pin del led que indica la activacion de la señal de entrada del TON
#define led_TON_salida 1      //se define el pin del led que indica la activacion de la señal de salida del TON

//aqui definimos el numero de segundos que queremos que cuente nuestro 
const int segundos_TON = 5;

//definimos la estructura de nuestro temporizador
struct TON
{
  byte senial;                     //sera true una vez que se alcance el tiempo establecido
  byte activado;                  //es true desde el momento que establece tiempo_aContar
  unsigned long tiempo_inicio;    //se establece despues de establecer tiempo_aContar
  unsigned long tiempo_aContar;    //se establece por el usuario y activa el funcionamiento del ton
};

TON ton;                          //creamos una instancia de nuestro temporizador

//----------------------------------------------------------------------------------------

void setup() 
{
  Serial.begin(9600);                       //iniciamos el monitor serial

  pinMode(led_TON,OUTPUT);                  //configuramos el pin que enciende el led de la señal de entrada del TON
  pinMode(led_TON_salida,OUTPUT);           //se configura el pin que enciende el led de la señal de salida del TON
  pinMode(botonActivar_TON,INPUT_PULLDOWN); //define el pin donde se escuchara el boton físico cuando sea oprimido

}

void loop() 
{
  //si queremos que se reinicie automaticamente cada vez que el contador alcance su límite y no este en uso
  //if(!ton.activado) iniciar_TON(5);     

  //de esta forma iniciamos el TON al presionar un boton
  if(digitalRead(botonActivar_TON) == HIGH) iniciar_TON(segundos_TON);

  //se verifica el status del TON activado o que se tenga en uso actualmente
  verificar_TON();

}

//---------------------------------------------------------------------------------------

//funcion que inicia el TON y su contador
void iniciar_TON(int tiempo)
{
  //si el led de la señal de salida estaba activada se desactiva
  digitalWrite(led_TON_salida, LOW);

  //se desactiva la señal de salida
  ton.senial = false;

  //se enciende un led como notificacion al usuario que se activo la señal de entrada del TON
  digitalWrite(led_TON,HIGH);

  //se establece el tiempo que debe contar el TON antes de activar la salida
  ton.tiempo_aContar = tiempo * 1000;   

  //se establece el setPoint de referencia para que el TON comience a contar
  ton.tiempo_inicio = millis();

  //indica que el TON ha sido activado y esta en uso 
  ton.activado = true;
}


//esta funcion verifica si el TON esta en activo y si aun no se cumple su tiempo
void verificar_TON()
{
  // si el temporizador esta activado se revisa si ya transcurrio su tiempo
  if(ton.activado)
  {
    //restamos el valor guardado como setPoint al tiempo actual. el valor es en milisegundos
    unsigned long tiempo_transcurrido = (millis() - ton.tiempo_inicio);  

    Serial.println(tiempo_transcurrido);    //se imprime la lectura al usuario

    if( tiempo_transcurrido >= ton.tiempo_aContar )  //si el tiempo establecido ya transcurrió
    {
      //se enciende el led de salida como notificación visual al usuario
      digitalWrite(led_TON_salida,HIGH);            

      ton.senial = true;                            //se activa la señal de salida
      Serial.println("señal de salida activada");   //se le notifica al usuario

      ton.activado = false;   //la señal se queda activada y se desactiva la lectura (uso) del TON      
    }
  }
}

