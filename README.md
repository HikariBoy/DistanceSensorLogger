# AccelerometerLockin
## INTRODUCTION:
###A synchronous detection approach using a accelerometer and reference frequency
Internet of thinks (IOT) devices power by energy harvesting sources require low power operation.  A mainstay of these IOT system is the microcontroller, a typical one being the [ATMEL 328P](https://www.microchip.com/wwwproducts/en/ATMEGA328P) used extensively in [Arduinos](https://www.arduino.cc/ ) .  The issue with these microcontrollers is their limited storage area, typically 2 kbytes of RAM.  Additional EEPROM space could be added, but EEPROMS are slow, and additional RAM requires additional power to maintain the contents.  My interest is to use these microcontrollers to keep the power requirements low, and add an accelerometer to sense minute vibrations from power transformers. In Australia these transforms run at nominally 50Hz, while in other countries they operation of 60Hz.  The condition monitoring of these transformers is extremely important and requires tracking the vibrations of the fundamental and harmonic components.  Once sense locally on the transformer by an IOT device power by energy harvesting, I want to radio the information back using low power LoRA.  However LoRA is a very low bit rate protocol.  If I want to sense vibrations at 50 Hz over nominally 10 waves, I need to capture data over 10*20ms=200ms.  If I want to capture n-harmonics up to say n=6, I need to sample the data (according to the Nyquist theorem) at 6*50*2=600Hz or equivalengtly a time-series with intervals of 1.67 ms.  So the amount of data I need to collect is  at least 120 data points.  For a 3-axis accelerometer with 16 bit (2-bytes) data on each axis (typical of the MU6050), that’s a minimum of 720 bytes (5760 bits) of data to send every time I want a measurement of the transformer.  At the highest effective bit rate of LoRA (37.5 kbps) that’s 154 ms of data transmission at reasonably high current consumption, almost as long as the time to took to grab the accelerometer data (but at a lot higher current)
## Physical Configuration
###The LORA 433 MHz conection uses:
1) breadboard based LORA +MPU6050 powered via 3.3V and controlled using <code> myMPU6050_LoRa_Tx.ino </code>:
* RFM95_CS 10
* RFM95_RST 9
* RFM95_INT 3
and physical wiring

RFM95LoRa | 328P_Pin |  RFM95LoRa  | 328P_Pin
----------|----------|-------------|---------
ANT       | -        |    GND      | -
GND       |GND       |    DIO5     |  -
DIO3      |-         |   RESET     | 9
DIO4      |  -       |    NSS      | 10
3.3V      |3.3V      |   SCK       | 13
DIO0      |3         |    MOSI     |  11
DIO1      |-         |  MISO       |  12
DIO2      |-         |    GND      | -

2) a Feather32u4 which acts a a LoRA receiver, controlled using <code> myMPU6050_LoRa_Rx.ino </code>::
* RFM95_CS 8
* RFM95_RST 4
* RFM95_INT 7

Ultra Low Power - SLEEP MODES 
<b>ATMEL328P sleep </b>:  The following code is essential to lower 328P current draw down to <b>4.3 microA</b>

    set_sleep_mode(SLEEP_MODE_PWR_DOWN); 
    cli();        
    sleep_enable();   
    sleep_bod_disable(); 
    sei();        
    sleep_cpu();      
    sleep_disable();    
    sei();  


<b>MPU6050 Sleep </b>: The SLEEP and power power modes of the [MPU6050 are detailed here](https://www.i2cdevlib.com/docs/html/class_m_p_u6050.html#a15ec5f8e7daf235f507c1d8b96af051a)

     
Using MPU6050 chips mounted to a PCB board is a convenient alternative to doing it yourself.  However there are several available and they each configure the circuit slightly differently.  see for example the tqo below:
<img src="MPU6050-Circuit_0.png" alt="Figure 1. PCB module for the MPU6050 version #1 (Source: https://components101.com/sensors/mpu6050-module).  Note 4.7kOhm resistor on AD0/SDO" align="left"  width="400"/>  <P></P>
or this alternative

<img src="MPU6050_SchematicV2.PNG" alt="Figure 2. PCB module for the MPU6050 version #2 (Source: Core electronics Website, linked to Schematic :http://image.dfrobot.com/image/data/SEN0142/6%20Dof%20-%20MPU6000%20v1.0%20schematic.pdf).  Note 4.70 Ohm resistor on AD0/SDO" align="left" width="200"/> <P></P>

To achieve low power from these modules, the following action is reuquired:
1) the LDO voltage regulator needs to be removed as it can draw from 50-100 microA  this is typically a 5 terminal device similar to the [BL8555-15474-0 LDO](http://www.chinesechip.com/chipFile/2015-07/BL8555-15474-0.pdf)
2) remove the LED - this is a surface mount device that you can remove using a standrad soldering iron tip by adding more solder to the part- it will probably be destructive to the LED, but this component is not needed in low power systems
3) remove the resistor on the AD0/SDO line - this resistor is not required and if you have a board that used a 470Ohms resistor for this part, it will draw up to an extra 7mA, far more than desired

Once the above is done, the Vin pin which should be power from an external 3,3V source should be connected to the output of the LDO regular.  Check which pin is the ouput by checking with a multimeter to confirm the the pin you believe is the output of the LDO regulator is the one connected to the one pad of the LED you removed (meter should read virtually 0.0 Ohm) .
Within the code  the following is required, assuming an object (code) MPU6050 accelgyro; (/code) has been created after loading in the (code) #include "MPU6050.h"</code) library provided from [Jeff Rowberg](https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050)

    //accelgyro.setSleepEnabled  (true);  //true = sleep
when not reading from the MPU6050 and prior to reading wake using:

    //accelgyro.setSleepEnabled  (false);  //false = wake

With the above checked completed, the current measured from the MPU6050 when in sleep (idle) mode was only <b>5.4 microA</b>.  This agrees very closely to the [MPU6050 Data sheet](https://www.invensense.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf) value of 5 microA.

Note the even pulling the power to th eMPU6050 does not reduce the power consumption becuase the MPU6050 draws microA power from the SCLK and SDA and/or DIO0 pins connected to the Arduino.for explayions, see: 
* [ardiono forum discussion](https://forum.arduino.cc/index.php?topic=509951.0)
* Illustrations and discussion on [stack exchange](https://arduino.stackexchange.com/questions/46300/analog-input-draws-current-when-arduino-is-turned-off)
* excellent video here from[[EEVblog] (https://www.youtube.com/watch?v=2yFh7Vv0Paw)
theref
Therefore, simply using a FET to turn the power off completely to the MPU 6050 would not lower power consumption further.  Concepts to try include :
1) series resistance from AD5/AD4 and DIO2 
2) series diodes from AD5/AD4 and DIO2 
3) remove GND not 3.3V to power down

Adding 20kOhm to the ports lowered the power only by ~1 microA and risks resulting in lost data.  Removing the GND worked very well and resulted in the total power drop to 6microA for the LoRa and 328P boards.  Removing the GND from the LoRa board lower power to only 5.7 mciroA.  
Next 
1) add FETS to remove GND from MPU6050 and LORA
2) investigate lowering clock speed of arduino during sleep to further lower the power of the 328P.
s


<b>LORA sleep </b> is relatively easy to achieve.  In software simply use:
//https://github.com/sandeepmistry/arduino-LoRa/issues/42

    // rf95.sleep();   // to put to cleep
see [github discussion](https://github.com/sandeepmistry/arduino-LoRa/issues/42) 
and wake from sleep using

    // rf95.setModeTx();  // wake from sleep
    
This reduces current consuption to about <b>1.7microA</b> in the RF95 (LoRA board)
