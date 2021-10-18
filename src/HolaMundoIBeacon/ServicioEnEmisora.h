// -*- mode: c++ -*-


// --------------------------------------------------------------
//
// ServicioEnEmisora
// Belén Grande López
// 2021-10-4
// Clase del servicio en emisora
//
// --------------------------------------------------------------

#ifndef SERVICIO_EMISORA_H_INCLUIDO
#define SERVICIO_EMISORA_H_INCLUIDO

// ----------------------------------------------------
// ----------------------------------------------------
#include <vector>

// ----------------------------------------------------
// alReves() utilidad
// pone al revés el contenido de una array en el mismo array
// ----------------------------------------------------
template< typename T >
T *  alReves( T * p, int n ) {
  T aux;

  for( int i=0; i < n/2; i++ ) {
	aux = p[i];
	p[i] = p[n-i-1];
	p[n-i-1] = aux;
  }
  return p;
} // ()

// ----------------------------------------------------
// ----------------------------------------------------
uint8_t * stringAUint8AlReves( const char * pString, uint8_t * pUint, int tamMax ) {

	int longitudString =  strlen( pString );
	int longitudCopiar = ( longitudString > tamMax ? tamMax : longitudString );
	// copio nombreServicio -> uuidServicio pero al revés
	for( int i=0; i<=longitudCopiar-1; i++ ) {
	  pUint[ tamMax-i-1 ] = pString[ i ];
	} // for

	return pUint;
} // ()

// ----------------------------------------------------------
// ----------------------------------------------------------
class ServicioEnEmisora {

public:


  // --------------------------------------------------------
  // --------------------------------------------------------

  // .........................................................
  // .........................................................
  using CallbackCaracteristicaEscrita = void ( uint16_t conn_handle,
											   BLECharacteristic * chr,
											   uint8_t * data, uint16_t len); 
  // .........................................................
  // .........................................................
  class Caracteristica {
  private:
	uint8_t uuidCaracteristica[16] = { // el uuid se copia aquí (al revés) a partir de un string-c
	  // least signficant byte, el primero
	  '0', '1', '2', '3', 
	  '4', '5', '6', '7', 
	  '8', '9', 'A', 'B', 
	  'C', 'D', 'E', 'F'
	};

	// 
	// 
	// 
	BLECharacteristic laCaracteristica;

  public:

	// .........................................................
   /*
     * Constructor de la clase
     * 
     * @param [char]  nombreCaracteristica_  -  Le pasamos un char con el nombre de la caracteristica
     *
     * @return No devuelve nada
    */
	// .........................................................
	Caracteristica( const char * nombreCaracteristica_ )
	  :
	  laCaracteristica( stringAUint8AlReves( nombreCaracteristica_, &uuidCaracteristica[0], 16 ) )
	{
	  
	} // ()

	// .........................................................
   /*
     * Constructor 2 de la clase
     * 
     * @param [char]  nombreCaracteristica_  -  Le pasamos un char con el nombre de la caracteristica
     * @param Z  propos  -  Le pasamos un uint8_t
     * @param SecureMode_t  permisoRead  -  Para que tenga permiso para leer
     * @param SecureMode_t  permisoWrite  -  Para que tenga permiso para escribir
     *
     * @return No devuelve nada
    */
	// .........................................................
	Caracteristica( const char * nombreCaracteristica_ ,
					uint8_t props,
					SecureMode_t permisoRead,
					SecureMode_t  permisoWrite, 
					uint8_t tam ) 
	  :
	  Caracteristica( nombreCaracteristica_ ) // llamada al otro constructor
	{
	  (*this).asignarPropiedadesPermisosYTamanyoDatos( props, permisoRead, permisoWrite, tam );
	} // ()

  private:
	// .........................................................
	// CHR_PROPS_WRITE , CHR_PROPS_READ ,  CHR_PROPS_NOTIFY 
   /*
     * Método para asignar propiedades
     * 
     * @param N  props  -  Le pasamos un uint8_t
     *
     * @return No devuelve nada
    */
	// .........................................................
	void asignarPropiedades ( uint8_t props ) {
	  // no puedo escribir AUN si el constructor llama a esto: Serial.println( " laCaracteristica.setProperties( props ); ");
	  (*this).laCaracteristica.setProperties( props );
	} // ()

	// .........................................................
	// BleSecurityMode::SECMODE_OPEN  , BleSecurityMode::SECMODE_NO_ACCESS
   /*
     * Método para asignar permisos
     * 
     * @param SecureMode_t  permisoRead  -  Para que tenga permiso para leer
     * @param SecureMode_t  permisoWrite  -  Para que tenga permiso para escribir
     *
     * @return No devuelve nada
    */
	// .........................................................
	void asignarPermisos( SecureMode_t permisoRead, SecureMode_t permisoWrite ) {
	  // no puedo escribir AUN si el constructor llama a esto: Serial.println( "laCaracteristica.setPermission( permisoRead, permisoWrite ); " );
	  (*this).laCaracteristica.setPermission( permisoRead, permisoWrite );
	} // ()

	// .........................................................
   /*
     * Método para asignar tamaños
     * 
     * @param N  tam  -  Le pasamos un uint8_t con el tamaño
     *
     * @return No devuelve nada
    */
	// .........................................................
	void asignarTamanyoDatos( uint8_t tam ) {
	  // no puedo escribir AUN si el constructor llama a esto: Serial.print( " (*this).laCaracteristica.setFixedLen( tam = " );
	  // no puedo escribir AUN si el constructor llama a esto: Serial.println( tam );
	  // (*this).laCaracteristica.setFixedLen( tam );
	  (*this).laCaracteristica.setMaxLen( tam );
	} // ()

  public:
	// .........................................................
   /*
     * Método para asignar propiedades, permisos y tamaño de datos
     * 
     * @param N  propos  -  Le pasamos un uint8_t
     * @param SecureMode_t  permisoRead  -  Para que tenga permiso para leer
     * @param SecureMode_t  permisoWrite  -  Para que tenga permiso para escribir
     * @param Z  propos  -  Le pasamos un uint8_t
     *
     * @return No devuelve nada
    */
	// .........................................................
	void asignarPropiedadesPermisosYTamanyoDatos( uint8_t props,
												 SecureMode_t permisoRead,
												 SecureMode_t permisoWrite, 
												 uint8_t tam ) {
	  asignarPropiedades( props );
	  asignarPermisos( permisoRead, permisoWrite );
	  asignarTamanyoDatos( tam );
	} // ()
												 

	// .........................................................
   /*
     * Método para escribir datos
     * 
     * @param [char]  str  -  Le pasamos un char 
     *
     * @return No devuelve nada
    */
	// .........................................................
	uint16_t escribirDatos( const char * str ) {
	  // Serial.print( " return (*this).laCaracteristica.write( str  = " );
	  // Serial.println( str );

	  uint16_t r = (*this).laCaracteristica.write( str );

	  // Serial.print( ">>>Escritos " ); Serial.print( r ); Serial.println( " bytes con write() " );

	  return r;
	} // ()

	// .........................................................
   /*
     * Método para notificar datos
     * 
     * @param [char]  str  -  Le pasamos un char 
     *
     * @return Z  -  Devuelve un uint16_t
    */
	// .........................................................
	uint16_t notificarDatos( const char * str ) {
	  
	  uint16_t r = laCaracteristica.notify( &str[0] );

	  return r;
	} //  ()

	// .........................................................
   /*
     * Método para instalar callback de caracteristica escrita
     * 
     * @param CallbackCaracteristicaEscrita  cb  -  Le pasamos un pbjeto CallbackCaracteristicaEscrita 
     *
     * @return No devuelve nada
    */
	// .........................................................
	void instalarCallbackCaracteristicaEscrita( CallbackCaracteristicaEscrita cb ) {
	  (*this).laCaracteristica.setWriteCallback( cb );
	} // ()

	// .........................................................
  /*
     * Método para activar
     * 
     * @param No le pasamos nada
     *
     * @return No devuelve nada
    */
	// .........................................................
	void activar() {
	  err_t error = (*this).laCaracteristica.begin();
	  Globales::elPuerto.escribir(  " (*this).laCaracteristica.begin(); error = " );
	  Globales::elPuerto.escribir(  error );
	} // ()

  }; // class Caracteristica
  
  // --------------------------------------------------------
  // --------------------------------------------------------
private:
  
  uint8_t uuidServicio[16] = { // el uuid se copia aquí (al revés) a partir de un string-c
	// least signficant byte, el primero
	'0', '1', '2', '3', 
	'4', '5', '6', '7', 
	'8', '9', 'A', 'B', 
	'C', 'D', 'E', 'F'
  };

  //
  //
  //
  BLEService elServicio;

  //
  //
  //
  std::vector< Caracteristica * > lasCaracteristicas;

public:
  
  // .........................................................
  /*
     * Constructor de la clase
     * 
     * @param [char]  nombreServicio_  -  Le pasamos un char con el nombre del servicio
     *
     * @return No devuelve nada
    */
  // .........................................................
  ServicioEnEmisora( const char * nombreServicio_ )
	:
	elServicio( stringAUint8AlReves( nombreServicio_, &uuidServicio[0], 16 ) )
  {
	
  } // ()
  
  // .........................................................
   /*
     * Método para escribir un uuid
     * 
     * @param No le pasamos nada
     *
     * @return No devuelve nada
    */
  // .........................................................
  void escribeUUID() {
	Serial.println ( "**********" );
	for (int i=0; i<= 15; i++) {
	  Serial.print( (char) uuidServicio[i] );
	}
	Serial.println ( "\n**********" );
  } // ()

  // .........................................................
   /*
     * Método para añadir una caracteristica
     * 
     * @param Caracteristica  car -  Le pasamos una caracteristica
     *
     * @return No devuelve nada
    */
  // .........................................................
  void anyadirCaracteristica( Caracteristica & car ) {
	(*this).lasCaracteristicas.push_back( & car );
  } // ()

  // .........................................................
   /*
     * Método para activar el servicio
     * 
     * @param No le pasamos nada
     *
     * @return No devuelve nada
    */
  // .........................................................
  void activarServicio( ) {
	// entiendo que al llegar aquí ya ha sido configurado
	// todo: características y servicio

	err_t error = (*this).elServicio.begin();
	Serial.print( " (*this).elServicio.begin(); error = " );
	Serial.println( error );

	for( auto pCar : (*this).lasCaracteristicas ) {
	  (*pCar).activar();
	} // for

  } // ()

  // .........................................................
  // .........................................................
  operator BLEService&() {
	// "conversión de tipo": si pongo esta clase en un sitio donde necesitan un BLEService
	return elServicio;
  } // ()
	
}; // class

#endif

// ----------------------------------------------------------
// ----------------------------------------------------------
// ----------------------------------------------------------
// ----------------------------------------------------------
