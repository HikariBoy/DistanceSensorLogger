# AccelerometerLockin
# Distance Sensor I/O (Rectilinear [1D] Displacement Sensor])
## INTRODUCTION:
###This project uses a [GEFRAN PY2F10S](https://www.gefran.com/en/in/products/73-py2-touch-version-with-ball-tip) that I had available and connects to an arduino and calibrates the stage.  Total travel range is ~10mm.  An initial attempt to interface directly to a PC powered Arduino using the internal 10-bit ADC resulted in extremely high noise having an RMS variation around 10microns.  It was due to 1) the noise in the PC 5V power rail, 2) the use of the same rail for the ADC reference voltage and 3) the limitations of a 10 bit ADC.  Subsequently, the current version uses:
1) an external power supply (currenlt 12V but anythng >7V is fine)  - this ensures a stable 5V and 3.3V power supply
2) powering the GEFRAN PY2F10S distance sensor using the 3.3V rail rather than the 5V rail.  Since the sensor is basically a variable 1kOhm potentiometer, this results in nominally 3.3mA through the sensors.  the recommended level is 0.1mA but any value less than 10mA is acceptable according to the data sheet.  Since the 3.3V rail is not loaded by any other components on the arduino board (except maybe an LED) this ensures that the power supply od dedicated to run only this sensor and hence should supply a well regulated rail voltage.
3) migration to a 16-bit ADC using the [ADS1115 16 Bit Converter Module](https://www.adafruit.com/product/1085).
4) internal averaging on the measured distance data

## Physical Configuration
###The Schematic for the setup is shown below - the EAGLECAD layout is in the repository [TBD].
The Arduino UNO fit within a small metal box with a squarecut out one side for the USB and a power and lead for the PY2F10S distance sensor, shown below.

[IMAGES TBD]

## Power Supply
Running froma  12V DC power supply results in significant heating of the onboard 5V regulator.  It is essential the regulator is heat sinked.  During initial testing even a small heatsink reached extremely high temperatures.  Hear I decided to use a an M8 nut turned on it side and glued in place with [heat sink adhesive](https://www.jaycar.com.au/adhesive-heatsink-compound-5g-tube/p/NM2014).  The nut also makes contact to the die cast metal lid which also helps to disappate th temperature.  This ensures good 5V regulation and extends the life of the board.

## Firmaware Configuration
The software was written under the arduino IDE environment. 
Version XX is a basic interface for testing and calibration.  It provides a fixed number of averages and simply zeros the sensor after reboot or after pressing the rset button.
## Testing
Initial testing indicated the following results

#Repreating zeroing.
The stage was bolted to the bench and the plunger stage lifted repeatedly.  The reported zero values were then summarised in the following statistics.

#Calibration.
A digital caliper was used as a refererence measure and expected to be accurate to [20 microns](https://www.jaycar.com.au/medias/sys_master/images/9318637535262/TD2082-manualMain.pdf) up to 10mm distance.  

