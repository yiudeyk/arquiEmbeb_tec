
#define luzverde1 1          //se define el número de los pines físicos de los leds y los botones
#define luzamarilla1 2  
#define luzroja1 4

#define luzverde2 39
#define luzamarilla2 37
#define luzroja2 35 

#define boton1 10 
#define boton2 16


//definimos la estructura de nuestro temporizador
struct temporizador
{
  byte activo = false;            //indica si el temporizador esta activo y siendo monitoreado en sus cambios de luz
  byte rojo = false;                      //inicia la luz roja
  byte amarillo = false;                  //inicia la luz amarillo
  byte verde = false;                     //inicia luz verde
  byte amarilloActivo = false;            //nos dira si la luz amarilla esta prendida o apagada durante el parpadeo
  unsigned long tiempo_rojo = 3 * 1000;       //se establecen los tiempos de cada luz del semaforo
  unsigned long tiempo_amarillo = 3 * 1000;    
  unsigned long tiempo_verde = 3 * 1000;
  unsigned long tiempo_aContar = 0;         //tiempo que debe contar durante cada uno de los periodos de cada luz
  unsigned long tiempo_referencia = 0;      //el contador se establecera en millis() como referencia de inicio
  unsigned long tiempo_referencia_amarillo = 0;   //cuenta el tiempo que esta prendido el led amarillo antes de parpadear
} semaforo[2];       //creamos un arreglo de 2 semaforos con estructura de temporizador


int boton_semaforo1 = 0;    //variables donde guardaremos la lectura de los botones al ser oprimidos
int boton_semaforo2 = 0;

//---------------------------------------------------------------------------------------------------------------------------------

void setup()
{
  Serial.begin(9600);       //iniciamos el monitor serial

  pinMode(luzroja1, OUTPUT);        //configuramos los pines de los leds como señales de salida
  pinMode(luzverde1, OUTPUT);
  pinMode(luzamarilla1, OUTPUT);
  pinMode(luzroja2, OUTPUT);
  pinMode(luzverde2, OUTPUT);
  pinMode(luzamarilla2, OUTPUT);
  pinMode(boton1, INPUT_PULLDOWN);    //se configuran los botones como señales de entrada
  pinMode(boton2, INPUT_PULLDOWN);

  iniciarSemaforos();               //inicializamos ambos semaforos asignandoles una posición de inicio
}

void loop() 
{
  boton_semaforo2 = digitalRead(boton2);    //registramos si se pulsó alguno de los botónes físicos
  boton_semaforo1 = digitalRead(boton1);

  if(boton_semaforo2 == HIGH && semaforo[1].rojo)   //el semáforo 2 solicita el paso inmediatamente mientras tiene el rojo
  {
    solicitarControl(1);      //se llama a la función que otorga el paso al semaforo que lo solicita
  }
  else if(boton_semaforo1 == HIGH && semaforo[0].rojo)  solicitarControl(0);  //se repite lo anterior con el otro semáforo

  revisarLuz();   //se revisa la secuencia de luces del semáforo activo y las mantiene andando y cambiando según los tiempos asignados
}

//---------------------------------------------------------------------------------------------------------------------------------------
//
void iniciarSemaforos()   //función del setUp
{
  semaforo[0].activo = true;    //al inicio establecemos el semáforo 1 como el que esta activo y siendo monitoreado

  semaforo[0].verde = true;   //establecemos la activacion de las luces de ambos semaforos, uno en verde y otro en rojo
  semaforo[1].rojo = true;

  digitalWrite(luzverde1, HIGH);    //se encienden las luces de los semáforos según su posición asignada
  digitalWrite(luzroja2, HIGH);

  semaforo[0].tiempo_referencia = millis();         //establecemos el contador interno del semáforo que esta activo 
  semaforo[0].tiempo_aContar = semaforo[0].tiempo_verde;                             
}

//
void solicitarControl(int sema)   //función del loop
{
  if(sema == 1)     //revisamos que semaforo esta solicitando el control
  {
    semaforo[0].amarillo = false;   //se apaga el estado de las luces de ambos semáforos
    semaforo[0].verde = false;      
    semaforo[1].rojo = false;
                      
    semaforo[0].rojo = true;     //se marca el encendido de las luces favoreciendo en verde 
    semaforo[1].verde = true;     //al semáforo que solicito el cambio
    

    digitalWrite(luzamarilla1, LOW);  //se realizan "electricamente" las cambios marcados anteriormente
    digitalWrite(luzverde1, LOW);
    digitalWrite(luzroja2, LOW);    

    digitalWrite(luzroja1, HIGH);     //reiniciamos el encendido de leds según el semáforo favorecido con el color verde
    digitalWrite(luzverde2, HIGH);

    semaforo[0].activo = false;     //ya que se hizo el ajuste de las luces trasladamos el control al semáforo que esta en verde 
    semaforo[1].activo = true;      //poniendolo como el semáforo activo para el monitoreo de cambio de luces

    semaforo[1].tiempo_aContar = semaforo[1].tiempo_verde;  //establecemos el contador del semaforo que toma control
    semaforo[1].tiempo_referencia = millis();
  }
  else if(sema == 0)                  //se repite  el mismo proceso anterior, pero con el otro semáforo
  {
    semaforo[1].amarillo = false;
    semaforo[1].verde = false;

    semaforo[1].rojo = true;
    semaforo[0].verde = true;
    semaforo[0].rojo = false;

    digitalWrite(luzamarilla2, LOW);  //apagamos las luces del semáforo contrario
    digitalWrite(luzverde2, LOW);
    digitalWrite(luzroja1, LOW);    //apagamos la luz roja que teniamos prendida para prender la verde

    digitalWrite(luzroja2, HIGH);
    digitalWrite(luzverde1, HIGH);

    semaforo[1].activo = false;     //trasladamos el control de un semaforo a otro
    semaforo[0].activo = true;

    semaforo[0].tiempo_aContar = semaforo[0].tiempo_verde;  //establecemos el contador del semaforo que toma control
    semaforo[0].tiempo_referencia = millis();
  }
}

//
void revisarLuz()           //función del loop
{
  if(semaforo[0].activo)    //revisamos que semaforo esta activo para monitoreo del próximo cambio de luz
  {
    //
    if( (millis() - semaforo[0].tiempo_referencia) >= semaforo[0].tiempo_aContar )  //revisamos el conteo de la luz y si es momento de cambiar
    {
      if(semaforo[0].verde)    //revisamos cual luz esta encendida actualmente y hacemos la transición a la siguiente luz
      {
        semaforo[0].verde = false;      //marcamos la luz que se apaga del semáforo y la que se prende
        semaforo[0].amarillo = true;

        digitalWrite(luzverde1, LOW);       //durante esta estapa solo un semáforo realiza un cambio apagamos la luz actual
        digitalWrite(luzamarilla1, HIGH);   //y encendemos la luz que sigue en la secuencia de encendido de luces del semáforo

        semaforo[0].tiempo_aContar = semaforo[0].tiempo_amarillo;   //reiniciamos el contador de acuerdo con la luz que ahora esta encendida
        semaforo[0].tiempo_referencia = millis();                   //tomamos nuestro tiempo de referencia para poder contar
        semaforo[0].tiempo_referencia_amarillo = semaforo[0].tiempo_referencia;   //asignamos el primer tiempo de referencia para contar el parpadeo
        semaforo[0].amarilloActivo = true;                          //indica si el led esta encendido actualmente o apagado
      }
      else if(semaforo[0].amarillo)
      {
        semaforo[0].amarillo = false;         // se marcan las luces que se encienden y las que se apagan
        semaforo[0].rojo = true;

        semaforo[1].rojo = false;
        semaforo[1].verde = true;

        digitalWrite(luzamarilla1, LOW);       //durante esta estapa ambos semáforos realizan un cambio en su secuencia de luz
        digitalWrite(luzroja2, LOW);

        digitalWrite(luzroja1, HIGH);
        digitalWrite(luzverde2, HIGH);

        semaforo[0].activo = false;         //se transfiere el control al otro semaforo
        semaforo[1].activo = true;

        semaforo[1].tiempo_referencia = millis();                 //reiniciamos el contador del semáforo
        semaforo[1].tiempo_aContar = semaforo[1].tiempo_verde;    //al que se transfirió el control 
        
      }
    }

    //
    if(semaforo[0].amarillo)      //si la luz amarilla está en turno hay que hacerla parpadear
    {
      if( (millis() - semaforo[0].tiempo_referencia_amarillo) >= 1000 ) //si ya pasó 1 segundo que la luz amarilla estuvo encendida
      {
        if(semaforo[0].amarilloActivo)        //apagamos o prendemos la luz según su estado actual
        {
          digitalWrite(luzamarilla1, LOW);                            //la resta hace más corto el tiempo que está apagado el led
          semaforo[0].tiempo_referencia_amarillo = millis() - 800;    //volvemos a tomar una hora como punto de referencia para el led amarillio
          semaforo[0].amarilloActivo = false;         //marcamos si el led esta encendido o apagado
        }
        else
        {
          digitalWrite(luzamarilla1, HIGH);
          semaforo[0].tiempo_referencia_amarillo = millis();
          semaforo[0].amarilloActivo = true;
        }
      }
    }
  }
  else if(semaforo[1].activo)
  {
    //
    if( (millis() - semaforo[1].tiempo_referencia) >= semaforo[1].tiempo_aContar )
    {
      if(semaforo[1].verde)
      {
        //Serial.println("verde 2");
        //
        semaforo[1].verde = false;
        semaforo[1].amarillo = true;

        //asumimos que el semaforo 2 está en rojo

        digitalWrite(luzverde2, LOW);       //durante esta estapa solo un semaforo realiza un cambio
        digitalWrite(luzamarilla2, HIGH);

        semaforo[1].tiempo_aContar = semaforo[1].tiempo_amarillo;   //reiniciamos el contador
        semaforo[1].tiempo_referencia = millis();
        semaforo[1].tiempo_referencia_amarillo = semaforo[1].tiempo_referencia;
        semaforo[1].amarilloActivo = true;
      }
      else if(semaforo[1].amarillo)
      {
        //Serial.println("amarillo 2");
        //
        semaforo[1].amarillo = false;
        semaforo[1].rojo = true;

        semaforo[0].rojo = false;
        semaforo[0].verde = true;

        digitalWrite(luzamarilla2, LOW);       //durante esta estapa ambos semáforos realizan un cambio
        digitalWrite(luzroja1, LOW);

        digitalWrite(luzroja2, HIGH);
        digitalWrite(luzverde1, HIGH);

        

        //se transfiere el control al otro semaforo
        semaforo[1].activo = false;
        semaforo[0].activo = true;

        semaforo[0].tiempo_aContar = semaforo[0].tiempo_verde;   //reiniciamos el contador
        semaforo[0].tiempo_referencia = millis();
      }
    }

    //
    if(semaforo[1].amarillo)      //si la luz amarilla está en turno hay que hacerla parpadear
    {
      if( (millis() - semaforo[1].tiempo_referencia_amarillo) >= 1000 ) //si ya pasó 1 segundo que la luz amarilla estuvo encendida
      {
        if(semaforo[1].amarilloActivo)
        {
          digitalWrite(luzamarilla2, LOW);
          semaforo[1].tiempo_referencia_amarillo = millis() - 800;  //la resta hace más corto el tiempo que está apagado el led
          semaforo[1].amarilloActivo = false;
        }
        else
        {
          digitalWrite(luzamarilla2, HIGH);
          semaforo[1].tiempo_referencia_amarillo = millis();
          semaforo[1].amarilloActivo = true;
        }
      }
    }
  }
}
