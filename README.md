# Distance Sensor I/O (Rectilinear [1D] Displacement Sensor])
## INTRODUCTION:
This project uses a [GEFRAN PY2F10S](https://www.gefran.com/en/in/products/73-py2-touch-version-with-ball-tip) that I had available and connects to an arduino and calibrates the stage.  Total travel range is ~10mm.  An initial attempt to interface directly to a PC powered Arduino using the internal 10-bit ADC resulted in extremely high noise having an RMS variation around 10microns.  It was due to 1) the noise in the PC 5V power rail, 2) the use of the same rail for the ADC reference voltage and 3) the limitations of a 10 bit ADC.  Subsequently, the current version uses:
1) an external power supply (currenlt 12V but anythng >7V is fine)  - this ensures a stable 5V and 3.3V power supply
2) powering the GEFRAN PY2F10S distance sensor using the 3.3V rail rather than the 5V rail.  Since the sensor is basically a variable 1kOhm potentiometer, this results in nominally 3.3mA through the sensors.  the recommended level is 0.1mA but any value less than 10mA is acceptable according to the data sheet.  Since the 3.3V rail is not loaded by any other components on the arduino board (except maybe an LED) this ensures that the power supply od dedicated to run only this sensor and hence should supply a well regulated rail voltage.
3) migration to a 16-bit ADC using the [ADS1115 16 Bit Converter Module](https://www.adafruit.com/product/1085).
4) internal averaging on the measured distance data

## Physical Configuration
The Schematic for the setup is shown below - the EAGLECAD layout is in the repository [TBD].
The assembled box withthe  GEFRAN PY2 distance sensor is shown in Figure 1.  the entire package is fairly small, with all components fitting within the [110x60x30 Aluminium Box](https://www.jaycar.com.au/economy-die-cast-aluminum-boxes-111-x-60-x-30mm/p/HB5062).  However to push the USB connector close to the side panel of the box, it was neccessary to remove the PCB power connector on the arduino (See Figure 2b) 
<img src="file://AssembledClosedBox.png" alt="Figure 1. Closed box showing USB input and GEFRAN PY2 distance sensor" align="left"  width="400"/>  <P></P>
The internals of the box as shown in Figure 2, with the exploded image shown in Figure 2b.  The 2-stacked heatsinks are used (glued together with thermal past) to help disapate th heat from the 5V regulator and to ensure the heatsinks come into intimate contact with the top metal lid of the box to further enhance the heat sinking.  The Arduino PCB power connector is removed to allow the USB connector to be nearee to the edge panel, and 2 flying leads (red & black) are used to connect to a [2.5mm panel mount](https://www.jaycar.com.au/2-5mm-bulkhead-male-dc-power-connector/p/PS0524) connector to the 12V DC external supply.
<img src="file://AssembledOpenedBox.png" alt="Figure 2. Opended Box showing (a) main board, (b) expanded view showing heatsink on 5V regulator, removed power jack, replaced with wiring to the panel mounted 2.5 mm jack, sensor wiring (see Figure 3) and connector to PY2 sensor on the 16Bit ADC" align="left"  width="800"/>  <P></P>
The GEFRAN PY2F10 rectilinear Displacement sensor is shown in Fgiure 3 along with the correspodning color coded wiring.  Interestingly, GEFRAN call this a rectilinear Displacement sensor, which is really just saying it measures  displacement in 1D.  Since displacement is a vector and distance a scalar, and the output of the sensor is a scalar quantity, I prefer to refer to this as a distance sensor for simplicity.  Others might argue over semantics.  The equivalent circuit is also shown in Figure 3, where the sensor acts as simply a continuous variable resistor.  Importantly, the start and end ranges are not valid and over about 1-1.5mm on each end, there is no valid output.  I have accounded for that in the fireware and reported out-of-range values in these ranges.
<img src="file://PY2.png" alt="Figure 3. GEFRAN PY2F10 rectilinear Displacement sensor with color coded wiring to the Arduino  power rails and the ADS1115S ADC." align="left"  width="800"/>  <P></P>

## Circuit Configuration
The Schematic for the setup is shown below - the EAGLECAD layout is in the repository [TBD].
<img src="file://SchematicLayout.PNG" alt="Figure 1. Schematic of the Arduino (Uno) interface to the GEFRAN PY2 distance sensor through the 16Bit ADC" align="left"  width="400"/>  <P></P>
The use of the ADS1115S is important for 2 reasons:
1) the ADS1115S contains an internal, high precision, temperature stabilized voltage reference.  thsi is essential when needing to take accurate measurements. Just as important is a stabile voltage supply for the sensor (as discussed above) when I decided to used the 3.3V reference from the Arduino board.
2) the 16 bit resolution and high 820 samples-per-sec specifications of this ADC make it considerabilty better than using the 10-bit Arduino ADC, or even and oversampled Arduino ADC which has been shown to be capable of an [equivalent of 16-bits, but at a much slower samples-per-seconds](https://www.electricrcaircraftguy.com/2014/05/using-arduino-unos-built-in-16-bit-adc.html).
The Arduino UNO fits within the small metal box (Figure 2) with a squarecut out one side for the USB and a power and lead for the PY2F10S distance sensor.  

## Power Supply
Running froma  12V DC power supply results in significant heating of the onboard 5V regulator.  It is essential the regulator is heat sinked.  During initial testing even a small heatsink reached extremely high temperatures.  Here I decided to stack 2-small heat sinks together, with each glued in place with [heat sink adhesive](https://www.jaycar.com.au/adhesive-heatsink-compound-5g-tube/p/NM2014).  The heatsinks also make contact to the Aluminium metal lid which also helps to disappate the heat.  This ensures good 5V regulation and extends the life of the board.

## Firmaware Configuration
The software was written under the arduino IDE environment. 
The code [distanceSensor.ino](distanceSensor.ino) is a basic interface for testing and calibration.  It provides a fixed number of averages and simply zeros the sensor after reboot or after pressing the rset button.
## Testing
Initial testing indicated the following results

#Repreating zeroing.
The stage was bolted to the bench and the plunger stage lifted repeatedly.  The reported zero values were then summarised in the following statistics.

#Calibration.
A digital caliper was used as a refererence measure and expected to be accurate to [20 microns](https://www.jaycar.com.au/medias/sys_master/images/9318637535262/TD2082-manualMain.pdf) up to 10mm distance.  

