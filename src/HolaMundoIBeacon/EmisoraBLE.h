// -*- mode: c++ -*-

// --------------------------------------------------------------
//
// EmisoraBLE.h
// Belén Grande López
// 2021-10-4
// Clase de la emisora 
//
// --------------------------------------------------------------
#ifndef EMISORA_H_INCLUIDO
#define EMISORA_H_INCLUIDO

// Buena introducción: https://learn.adafruit.com/introduction-to-bluetooth-low-energy/gap
// https://os.mbed.com/blog/entry/BLE-Beacons-URIBeacon-AltBeacons-iBeacon/

// fuente: https://www.instructables.com/id/Beaconeddystone-and-Adafruit-NRF52-Advertise-Your-/
// https://github.com/nkolban/ESP32_BLE_Arduino/blob/master/src/BLEBeacon.h

// https://os.mbed.com/blog/entry/BLE-Beacons-URIBeacon-AltBeacons-iBeacon/
// https://learn.adafruit.com/bluefruit-nrf52-feather-learning-guide/bleadvertising

// ----------------------------------------------------------
// ----------------------------------------------------------
#include "ServicioEnEmisora.h"

// ----------------------------------------------------------
// ----------------------------------------------------------
class EmisoraBLE {
private:

  const char * nombreEmisora;   // [char]
  const uint16_t fabricanteID;  // N
  const int8_t txPower;         // Z

public:

  // .........................................................
  // .........................................................
  using CallbackConexionEstablecida = void ( uint16_t connHandle );
  using CallbackConexionTerminada = void ( uint16_t connHandle, uint8_t reason);

  // .........................................................
     /*
     * Constructor de la clase
     *
     * @param [char]  nombreEmisora_  -  Le pasamos un char con el nombre de la emisora
     * @param N  fabricanteID_  -  Le pasamos un uint16_t con el id del fabricante
     * @param Z  txPower_  -  Le pasamos un int8_t con el txpower
     *
     * @return No devuelve nada
     */
  // .........................................................
  EmisoraBLE( const char * nombreEmisora_, const uint16_t fabricanteID_,
        const int8_t txPower_ ) 
  :
  nombreEmisora( nombreEmisora_ ) ,
  fabricanteID( fabricanteID_ ) ,
  txPower( txPower_ )
  {
  // no encender ahora la emisora, tal vez sea por el println()
  // que hace que todo falle si lo llamo en el contructor
  // ( = antes que configuremos Serial )
  // No parece que sea por el println,
  // por tanto NO_encenderEmisora();
  } // ()

  // .........................................................
  // .........................................................
  /* creo que no me sirve esta versión porque parece
   que no se instalen los callbacks si la emisora no está encendida,
   pero no la puedo encender en el constructor 
  EmisoraBLE( const char * nombreEmisora_, const uint16_t fabricanteID_,
        const int8_t txPower_,
        CallbackConexionEstablecida cbce,
        CallbackConexionTerminada cbct
        ) 
  :
  EmisoraBLE ( nombreEmisora_, fabricanteID_, txPower_ )
  {
  instalarCallbackConexionEstablecida( cbce );
  instalarCallbackConexionTerminada( cbct );
  } // ()
  */
  
  // .........................................................
     /*
     * Método para encender la emisora 
     *
     * @param No le pasamos nada
     *
     * @return No devuelve nada
     */
  // .........................................................
  void encenderEmisora() {
  // Serial.println ( "Bluefruit.begin() " );
   Bluefruit.begin(); 

   // por si acaso:
   (*this).detenerAnuncio();
  } // ()

  // .........................................................
     /*
     * Método para subir datos fake para así comprobar que funciona correctamente la subida de datos a la base de datos
     *
     * @param CallbackConexionEstablecida cbce - Le pasamos un objeto CallbackConexionEstablecida que hemos definido arriba
     * @param CallbackConexionTerminada cbct - Le pasamos un objeto CallbackConexionTerminada que hemos definido arriba
     *
     * @return No devuelve nada
     */
  // .........................................................
  void encenderEmisora( CallbackConexionEstablecida cbce,
            CallbackConexionTerminada cbct ) {

  encenderEmisora();

  instalarCallbackConexionEstablecida( cbce );
  instalarCallbackConexionTerminada( cbct );

  } // ()

  /// .........................................................
     /*
     * Método para detener anuncio
     *
     * @param No le pasamos nada
     *
     * @return No devuelve nada
     */
  // .........................................................
  void detenerAnuncio() {

  if ( (*this).estaAnunciando() ) {
    // Serial.println ( "Bluefruit.Advertising.stop() " );
    Bluefruit.Advertising.stop(); 
  }

  }  // ()
  
  // .........................................................
     /*
     * Método para comprobar si está anunciando mediante un booleano
     *
     * @param No le pasamos nada
     *
     * @return bool: ToF
     */
  // .........................................................
  bool estaAnunciando() {
  return Bluefruit.Advertising.isRunning();
  } // ()

  // .........................................................
   /*
     * Método para amitir anuncio de un beacon
     *
     * @param [N] beaconUUID  -  Le pasamos un puntero con un uint8_t con el valor del uuid del beacon
     * @param Z   major  -  Le pasamos un int16_t con el valor del major
     * @param Z   minor  -  Le pasamos un int16_t con el valor del minor
     * @param N   rssi  -  Le pasamos un uint8_t con el valor del rssi
     *
     * @return No devuelve nada
     */
  // .........................................................
  void emitirAnuncioIBeacon( uint8_t * beaconUUID, int16_t major, int16_t minor, uint8_t rssi ) {

  //
  //
  //
  (*this).detenerAnuncio();
  
  //
  // creo el beacon 
  //
  BLEBeacon elBeacon( beaconUUID, major, minor, rssi );
  elBeacon.setManufacturer( (*this).fabricanteID );

  //
  // parece que esto debe ponerse todo aquí
  //

  Bluefruit.setTxPower( (*this).txPower );
  Bluefruit.setName( (*this).nombreEmisora );
  Bluefruit.ScanResponse.addName(); // para que envíe el nombre de emisora (?!)

  //
  // pongo el beacon
  //
  Bluefruit.Advertising.setBeacon( elBeacon );

  //
  // ? qué valorers poner aquí
  //
  Bluefruit.Advertising.restartOnDisconnect(true); // no hace falta, pero lo pongo
  Bluefruit.Advertising.setInterval(100, 100);    // in unit of 0.625 ms

  //
  // empieza el anuncio, 0 = tiempo indefinido (ya lo pararán)
  //
  Bluefruit.Advertising.start( 0 ); 
  
  } // ()

  // .........................................................
  //
  // Ejemplo de Beacon (31 bytes)
  //
  // https://os.mbed.com/blog/entry/BLE-Beacons-URIBeacon-AltBeacons-iBeacon/
  //
  // The iBeacon Prefix contains the hex data : 0x0201061AFF004C0215. This breaks down as follows:
  //
  // 0x020106 defines the advertising packet as BLE General Discoverable and BR/EDR high-speed incompatible.
  // Effectively it says this is only broadcasting, not connecting.
  //
  // 0x1AFF says the following data is 26 bytes long and is Manufacturer Specific Data.
  //
  // 0x004C is Apple’s Bluetooth Sig ID and is the part of this spec that makes it Apple-dependent.
  //
  // 0x02 is a secondary ID that denotes a proximity beacon, which is used by all iBeacons.
  //
  // 0x15 defines the remaining length to be 21 bytes (16+2+2+1).
  //
  // Por ejemmplo:
  //
  // 1. prefijo: 9bytes
  //       0x02, 0x01, 0x06,       // advFlags 3bytes
  //       0x1a, 0xff,             // advHeader 2 (0x1a = 26 = 25(lenght de 0x4c a 0xca)+1)   0xFF -> BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA
  //       0x4c, 0x00,             // companyID 2bytes
  //       0x02,                   // ibeacon type 1 byte
  //       0x15,                   // ibeacon length 1 byte (dec=21 lo que va a continuación: desde  la 'f' hasta 0x01)
  // 
  // 2. uuid: 16bytes
  // 'f', 'i', 's', 't', 'r', 'o', 'f', 'i', 's', 't', 'r', 'o', 0xa7, 0x10, 0x96, 0xe0
  // 
  // 2 major: 2bytes
  // 0x04, 0xd2,
  // 
  // minor: 2bytes
  // 0x10, 0xe1,
  // 
  // 0xca, // tx power : 1bytes
  //
  // 0x01, // este es el byte 31 = BLE_GAP_ADV_SET_DATA_SIZE_MAX, parece que sobra
  //
  // .........................................................
  // Para enviar como carga libre los últimos 21 bytes de un iBeacon (lo que normalmente sería uuid-16 major-2 minor-2 txPower-1)
  // .........................................................
  /*
  void emitirAnuncioIBeaconLibre( const char * carga ) {

  const uint8_t tamanyoCarga = strlen( carga );
  */

  // .........................................................
     /*
     * Método para emitir un anuncio de un beacon libre
     *
     * @param [char]  carga  -  Le pasamos un array de chars con la carga
     * @param N  tamanyoCarga  -  Le pasamos un uint8_t con el tamaño de la carga
     *
     * @return No devuelve nada
     */
  // .........................................................  
  void emitirAnuncioIBeaconLibre( const char * carga, const uint8_t tamanyoCarga ) {

  (*this).detenerAnuncio(); 

  Bluefruit.Advertising.clearData();
  Bluefruit.ScanResponse.clearData(); // hace falta?

  // Bluefruit.setTxPower( (*this).txPower ); creo que no lo pongo porque es uno de los bytes de la parte de carga que utilizo
  Bluefruit.setName( (*this).nombreEmisora );
  Bluefruit.ScanResponse.addName();

  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);

  // con este parece que no va  !
  // Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAG_LE_GENERAL_DISC_MODE);

  //
  // hasta ahora habrá, supongo, ya puestos los 5 primeros bytes. Efectivamente.
  // Falta poner 4 bytes fijos (company ID, beacon type, longitud) y 21 de carga
  //
  uint8_t restoPrefijoYCarga[4+21] = {
    0x4c, 0x00, // companyID 2
    0x02, // ibeacon type 1byte
    21, // ibeacon length 1byte (dec=21)  longitud del resto // 0x15 // ibeacon length 1byte (dec=21)  longitud del resto
    '-', '-', '-', '-', 
    '-', '-', '-', '-', 
    '-', '-', '-', '-', 
    '-', '-', '-', '-', 
    '-', '-', '-', '-', 
    '-'
  };

  //
  // addData() hay que usarlo sólo una vez. Por eso copio la carga
  // en el anterior array, donde he dejado 21 sitios libres
  //
  memcpy( &restoPrefijoYCarga[4], &carga[0], ( tamanyoCarga > 21 ? 21 : tamanyoCarga ) ); 

  //
  // copio la carga para emitir
  //
  Bluefruit.Advertising.addData( BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA,
                   &restoPrefijoYCarga[0],
                   4+21 );

  //
  // ? qué valores poner aquí ?
  //
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(100, 100);    // in unit of 0.625 ms

  Bluefruit.Advertising.setFastTimeout( 1 );      // number of seconds in fast mode
  //
  // empieza el anuncio, 0 = tiempo indefinido (ya lo pararán)
  //
  Bluefruit.Advertising.start( 0 ); 

  Globales::elPuerto.escribir( "emitiriBeacon libre  Bluefruit.Advertising.start( 0 );  \n");
  } // ()

  // .........................................................
   /*
     * Método para añadir un servicio
     *
     * @param ervicioEnEmisora  -  Le pasamos un objeto ServicioEnEmisora
     * @param servicio  -  Le pasamos un servicio
     *
     * @return bool: ToF
     */
  // .........................................................
  bool anyadirServicio( ServicioEnEmisora & servicio ) {

  Globales::elPuerto.escribir( " Bluefruit.Advertising.addService( servicio ); \n");

  bool r = Bluefruit.Advertising.addService( servicio );

  if ( ! r ) {
    Serial.println( " SERVICION NO AÑADIDO \n");
  }
  

  return r;
   // nota: uso conversión de tipo de servicio (ServicioEnEmisora) a BLEService
   // para addService()
  } // ()

  
  // .........................................................
    /*
     * Método para añadir un servicio con sus caracteristicas
     *
     * @param ServicioEnEmisora  servicio  -  Le pasamos un objeto ServicioEnEmisora
     *
     * @return bool: ToF
     */
  // .........................................................
  bool anyadirServicioConSusCaracteristicas( ServicioEnEmisora & servicio ) { 
  return (*this).anyadirServicio( servicio );
  } // 

  // .........................................................
   /*
     * Método para añadir un servicio con sus caracteristicas
     *
     * @param ServicioEnEmisora  servicio  -  Le pasamos un objeto ServicioEnEmisora y especificamos que queremos el servicio
     * @param ServicioEnEmisora  Caracteristica  -  Le pasamos un objeto ServicioEnEmisora y especificamos que queremos la caracteristica
     *
     * @return bool: ToF
     */
  // .........................................................
  template <typename ... T>
  bool anyadirServicioConSusCaracteristicas( ServicioEnEmisora & servicio,
                       ServicioEnEmisora::Caracteristica & caracteristica,
                       T& ... restoCaracteristicas) {

  servicio.anyadirCaracteristica( caracteristica );

  return anyadirServicioConSusCaracteristicas( servicio, restoCaracteristicas... );
  
  } // ()

  // .........................................................
    /*
     * Método para añadir un servicio con sus caracteristicas y activarlo
     *
     * @param ServicioEnEmisora servicio  -  Le pasamos un objeto ServicioEnEmisora y especificamos que queremos el servicio
     *
     * @return bool: ToF
     */
  // .........................................................
  
  template <typename ... T>
  bool anyadirServicioConSusCaracteristicasYActivar( ServicioEnEmisora & servicio,
                           // ServicioEnEmisora::Caracteristica & caracteristica,
                           T& ... restoCaracteristicas) {

  bool r = anyadirServicioConSusCaracteristicas( servicio, restoCaracteristicas... );

  servicio.activarServicio();

  return r;
  
  } // ()

  // .........................................................
    /*
     * Método para instalar callback conexion establecida
     *
     * @param CallbackConexionEstablecida  cb  -  Le pasamos un objeto CallbackConexionEstablecida declarado arriba
     *
     * @return No devuelve nada
     */
  // .........................................................
  void instalarCallbackConexionEstablecida( CallbackConexionEstablecida cb ) {
  Bluefruit.Periph.setConnectCallback( cb );
  } // ()

  // .........................................................
  /*
     * Método para instalar callback conexion terminada
     *
     * @param CallbackConexionTerminada  cb  -  Le pasamos un objeto CallbackConexionTerminada declarado arriba
     *
     * @return No devuelve nada
     */
  // .........................................................
  void instalarCallbackConexionTerminada( CallbackConexionTerminada cb ) {
  Bluefruit.Periph.setDisconnectCallback( cb );
  } // ()

  // .........................................................
   /*
     * Método para obtener la conexion
     *
     * @param Z  connHandle  -  Le pasamos un uint16_t con la conexion a realizadas
     *
     * @return No devuelve nada
     */
  // .........................................................
  BLEConnection * getConexion( uint16_t connHandle ) {
    
  return Bluefruit.Connection( connHandle );
  } // ()

}; // class

#endif

// ----------------------------------------------------------
// ----------------------------------------------------------
// ----------------------------------------------------------
// ----------------------------------------------------------
