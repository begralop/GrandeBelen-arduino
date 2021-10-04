// -*- mode: c++ -*-

#ifndef LED_H_INCLUIDO
#define LED_H_INCLUIDO

// ----------------------------------------------------------
// Belén Grande López
// 2021-10-4
// ----------------------------------------------------------

// ----------------------------------------------------------
 /*
     * Método para esperar
     *
     * L  tiempo  -  Le pasamos un long con el tiempo de espera
     *
     * No devuelve nada
 */
// ----------------------------------------------------------
void esperar (long tiempo) {
  delay (tiempo);
}

// ----------------------------------------------------------
// ----------------------------------------------------------
class LED {
private:
  int numeroLED;  // Z
  bool encendido;  // VoF
public:

  // .........................................................
  /*
     * Constructor de la clase
     *
     * Z  numero  -  Le pasamos un int con el numero 
     *
     * No devuelve nada
     */
  // .........................................................
  LED (int numero)
	: numeroLED (numero), encendido(false)
  {
	pinMode(numeroLED, OUTPUT);
	apagar ();
  }

  // .........................................................
  /*
     * Método para encender el led
     *
     * No le pasamos nada
     *
     * No devuelve nada
  */
  // .........................................................
  void encender () {
	digitalWrite(numeroLED, HIGH); 
	encendido = true;
  }

  // .........................................................
    /*
     * Método para apagar el led
     *
     * No le pasamos nada
     *
     * No devuelve nada
  */
  // .........................................................
  void apagar () {
	  digitalWrite(numeroLED, LOW);
	  encendido = false;
  }

  // .........................................................
    /*
     * Método para alternar el led
     *
     * No le pasamos nada
     *
     * No devuelve nada
  */
  // .........................................................
  void alternar () {
	if (encendido) {
	  apagar();
	} else {
	  encender ();
	}
  } // ()

  // .........................................................
    /*
     * Método para hacer brillar el led
     *
     * L  tiempo  -  Le pasamos un long con el tiempo que debe esperar para brillar 
     *
     * No devuelve nada
  */
  // .........................................................
  void brillar (long tiempo) {
	encender ();
	esperar(tiempo); 
	apagar ();
  }
}; // class

// ----------------------------------------------------------
// ----------------------------------------------------------
// ----------------------------------------------------------
// ----------------------------------------------------------
#endif
