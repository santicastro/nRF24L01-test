#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>



#include <Wire.h>
#include <LiquidCrystal.h>

#define LED 2

LiquidCrystal lcd(0x0);

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  // put your setup code here, to run once:
  lcd.begin(20, 4);
  lcd.setCursor(0,0);
  lcd.print("Setuping...");

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

  Mirf.setRADDR((byte *)"echo1");

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
  Serial.println("setup finish ... ");
  lcd.setCursor(0,0);
  lcd.print("Listening...");

}
int count = 0;
void loop() {
  /*
   * A buffer to store the data.
   */

  byte data[Mirf.payload];
  data[0]=0;
  /*
   * If a packet has been recived.
   *
   * isSending also restores listening mode when it 
   * transitions from true to false.
   */

  if(!Mirf.isSending() && Mirf.dataReady()){
    lcd.setCursor(0,1);
    lcd.print("Got packet");

    Serial.println("Got packet");

    /*
     * Get load the packet into the buffer.
     */

    Mirf.getData(data);

    /*
     * Set the send address.
     */

    Mirf.setTADDR((byte *)"clie1");

    /*
     * Send the data back to the client.
     */
    //    lcd.setCursor(0,2);
    //    for(int i=0; i < Mirf.payload; i++){
    //      if(data[i]=0)
    //        break;
    //      Serial.print( (char)data[i] );
    //      lcd.print( (char)data[i] );
    //    }
    Mirf.send(data);

    /*
     * Wait untill sending has finished
     *
     * NB: isSending returns the chip to receving after returning true.
     */

    Serial.println("Reply sent.");
  }
  else{
    if(count ==0){
      count = 8000;

      digitalWrite(LED, HIGH);
      delay(10);
      digitalWrite(LED, LOW);
    }
    count--;
  }
}








