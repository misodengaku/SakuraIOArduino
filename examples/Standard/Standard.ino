#include <SakuraIO.h>

//SakuraIO_SPI sakuraio(10);
SakuraIO_I2C sakuraio;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);
  Serial.println("Hello");
}

uint32_t c = 0;

uint8_t counter = 0;


void loop() {
  counter++;

  Serial.println("");
  Serial.println(counter);

  uint8_t request[33] = {};
  uint8_t response[33] = {};

  // Echoback
  request[0] = counter;
  sakuraio.echoback(1, request, response);
  Serial.print(request[0]);
  Serial.print(" -> ");
  Serial.println(response[0]);

  // Unixtime
  uint32_t unixtime = (uint32_t)(sakuraio.getUnixtime()/1000UL);
  Serial.print("Unixtime ");
  Serial.println(unixtime);

  // ADC
  uint16_t adc;
  adc = sakuraio.getADC(0);
  Serial.print("ADC0 ");
  Serial.println(adc);
  adc = sakuraio.getADC(1);
  Serial.print("ADC1 ");
  Serial.println(adc);

  // ProductID
  uint16_t pid = sakuraio.getProductID();
  Serial.print("PID ");
  Serial.println(pid);

  // UniqueID
  sakuraio.getUniqueID((char *)response);
  Serial.print("UID ");
  Serial.println((char *)response);

  // Version
  sakuraio.getFirmwareVersion((char *)response);
  Serial.print("Ver ");
  Serial.println((char *)response);

  // Connection Status
  uint8_t connectionStatus = sakuraio.getConnectionStatus();
  Serial.print("Status ");
  Serial.println(connectionStatus);

  //getSignalQuarity
  uint8_t signalQuarity = sakuraio.getSignalQuarity();
  Serial.print("Quality ");
  Serial.println(signalQuarity);


  // Tx Queue
  uint8_t ret;
  ret = sakuraio.enqueueTx((uint8_t)0, (int32_t)counter);
  Serial.print("Enqueue ");
  Serial.println(ret);

  uint8_t avail;
  uint8_t queued;
  sakuraio.getTxQueueLength(&avail, &queued);
  Serial.print("Tx Available=");
  Serial.print(avail);
  Serial.print(" Queued=");
  Serial.println(queued);

  if(queued >= 30){
    ret = sakuraio.clearTx();
    Serial.print("Clear ");
    Serial.println(ret);
  }else if(queued >= 5){
    ret = sakuraio.send();
    Serial.print("Send ");
    Serial.println(ret);
  }

  // Rx Queue
  sakuraio.getRxQueueLength(&avail, &queued);
  Serial.print("Rx Available=");
  Serial.print(avail);
  Serial.print(" Queued=");
  Serial.println(queued);

  for(uint8_t i=0; i<queued; i++){
    uint8_t channel;
    uint8_t type;
    uint8_t values[8];
    int64_t offset;
    ret = sakuraio.dequeueRx(&channel, &type, values, &offset);
    Serial.print("Dequeue ");
    Serial.print(ret);
    if(ret == 0x01){
      Serial.print(" ch="); Serial.print(channel);
      Serial.print(" type="); Serial.print((char)type);
      Serial.print(" values=[");
      for(uint8_t b=0; b<7; b++){
        Serial.print(values[b]);
        Serial.print(" ");
      }
      Serial.print(values[7]);
      Serial.print("] offset="); Serial.println((int32_t)offset);
    }else{
      Serial.println(" ERROR");
    }
  }


  delay(1000);
}
