
// -*- mode: c++ -*-

// ----------------------------------------------------------
// Belén Grande López
// 2021-10-4
// ----------------------------------------------------------

#ifndef PUERTO_SERIE_H_INCLUIDO
#define PUERTO_SERIE_H_INCLUIDO

// ----------------------------------------------------------
// ----------------------------------------------------------
class PuertoSerie  {

public:
  // .........................................................
    /*
     * Constructor de la clase
     * 
     * @param L  baudios  -  Le pasamos un long con los baudios
     *
     * @return No devuelve nada
    */
  // .........................................................
  PuertoSerie (long baudios) {
	Serial.begin( baudios );
	// mejor no poner esto aquí: while ( !Serial ) delay(10);   
  } // ()

  // .........................................................
  /*
     * Método para esperar si hay disponible algún puerto
     * 
     * @param No le pasamos nada
     *
     * @return No devuelve nada
    */
  // .........................................................
  void esperarDisponible() {

	while ( !Serial ) {
	  delay(10);   
	}

  } // ()

  // .........................................................
    /*
     * Método para escribir
     * 
     * @param T  mensaje  -  Le pasamos un mensaje
     *
     * @return No devuelve nada
    */
  // .........................................................
  template<typename T>
  void escribir (T mensaje) {
	Serial.print( mensaje );
  } // ()
  
}; // class PuertoSerie

// ----------------------------------------------------------
// ----------------------------------------------------------
// ----------------------------------------------------------
// ----------------------------------------------------------
#endif
