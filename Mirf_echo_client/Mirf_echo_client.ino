/**
 * A Mirf example to test the latency between two Ardunio.
 *
 * Pins:
 * Hardware SPI:
 * MISO -> 12
 * MOSI -> 11
 * SCK -> 13
 *
 * Configurable:
 * CE -> 8
 * CSN -> 7
 *
 * Note: To see best case latency comment out all Serial.println
 * statements not displaying the result and load 
 * 'ping_server_interupt' on the server.
 */

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

#define LED 2

void setup(){
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  Serial.begin(9600);
  /*
   * Setup pins / SPI.
   */

  /* To change CE / CSN Pins:
   * 
   * Mirf.csnPin = 9;
   * Mirf.cePin = 7;
   */

  Mirf.cePin = 8;
  Mirf.csnPin = 7;

  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();

  /*
   * Configure reciving address.
   */

  Mirf.setRADDR((byte *)"clie1");

  /*
   * Set the payload length to sizeof(unsigned long) the
   * return type of millis().
   *
   * NB: payload on client and server must be the same.
   */

  Mirf.payload = sizeof(unsigned long);

  /*
   * Write channel and payload config then power up reciver.
   */

  /*
   * To change channel:
   * 
   * Mirf.channel = 10;
   *
   * NB: Make sure channel is legal in your area.
   */
  Mirf.channel = 10;
  Mirf.config();

  Serial.println("Beginning ... "); 
}

void loop(){
  unsigned long time = millis();

  Mirf.setTADDR((byte *)"echo1");

  Mirf.send((byte *)&time);

  Serial.println("trying to send "); 

  while(Mirf.isSending()){
    delay(500);
    Serial.println("waiting");
  }
  digitalWrite(LED, HIGH);

  Serial.println("Finished sending");
  delay(10);
  digitalWrite(LED, LOW);
  
  while(!Mirf.dataReady()){
    //Serial.println("Waiting");
    if ( ( millis() - time ) > 2000 ) {
      Serial.println("Timeout on response from server!");
      return;
    }
  }

  Mirf.getData((byte *) &time);

  Serial.print("Ping: ");
  Serial.println((millis() - time));

  delay(1000);
} 







