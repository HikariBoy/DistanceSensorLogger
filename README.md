# Distance Sensor I/O (Rectilinear [1D] Displacement Sensor])
## Introduction

This project uses a [GEFRAN PY2F10S](https://www.gefran.com/en/in/products/73-py2-touch-version-with-ball-tip) that I had available and connects to an Arduino that provides serial I/O at 9600 baud and includes stage calibration.  Total travel range is ~10mm.  An initial attempt to interface directly to a PC powered Arduino using the internal 10-bit ADC resulted in extremely high noise having an RMS variation around 10 &#181;m.  This was due to 1) the noise in the PC 5V power rail, 2) the use of the same rail for the ADC reference voltage and 3) the limitations of a 10 bit ADC.  Subsequently, the current version uses:
1) an external power supply (currently 12V but anything >7V is fine)  - this ensures a stable 5V and 3.3V power supply
2) powering the GEFRAN PY2F10S distance sensor using the 3.3 V rail rather than the 5 V rail.  Since the sensor is basically a variable 1k&#937; potentiometer, this results in nominally 3.3mA through the sensors.  The recommended level is 0.1 mA but any value less than 10 mA is acceptable according to the data sheet.  Since the 3.3V rail is not loaded by any other components on the Arduino board (except maybe an LED) this ensures that the power supply od dedicated to run only this sensor and hence should supply a well regulated rail voltage.
3) migration to a 16-bit ADC using the [ADS1115 16 Bit Converter Module](https://www.adafruit.com/product/1085).
4) internal averaging on the measured distance data

 
## Physical Configuration

The assembled box with the  GEFRAN PY2 distance sensor is shown in Figure 1.  The entire package is fairly small, with all components fitting within the 110&#215;60&#215;30 Aluminium [Box](https://www.jaycar.com.au/economy-die-cast-aluminum-boxes-111-x-60-x-30mm/p/HB5062).  However to push the USB connector close to the side panel of the box, it was necessary to remove the PCB power connector on the Arduino (See Figure 2b) 

<P></P>

<div style="float: left  color: blue font-style: italic">
<img src="AssembledClosedBox.png" alt="Figure 1. Closed box showing USB input and GEFRAN PY2 distance sensor"  align="left" width="400"/></div>  <figcaption > <I><b>Figure 1:</b>Closed box showing USB input and GEFRAN PY2 distance sensor </I> </figcaption> <br>
 
The internals of the box as shown in Figure 2, with the exploded image shown in Figure 2b.  Two (2)-stacked heatsinks are used (glued together with [thermal paste](https://www.jaycar.com.au/adhesive-heatsink-compound-5g-tube/p/NM2014)) to help dissipate the heat from the 5V regulator and to ensure the heatsinks come into intimate contact with the top metal lid of the box to further enhance the heat sinking.  The Arduino PCB power connector is removed to allow the USB connector to be nearer to the edge panel, and 2 flying leads (red & black) are used to connect to a [2.5mm panel mount](https://www.jaycar.com.au/2-5mm-bulkhead-male-dc-power-connector/p/PS0524) connector to the 12V DC external supply.
<div style="clear:both;">  <img src="AssembledOpenedBox.png" width="800"/> </div>  <figcaption > <I><b>Figure 2:</b> Opened Box showing (a) main board, (b) expanded view showing heatsink on 5V regulator, removed power jack, replaced with wiring to the panel mounted 2.5 mm jack, sensor wiring (see Figure 3) and connector to PY2 sensor on the 16Bit ADC</I></figcaption>

The GEFRAN PY2F10 rectilinear Displacement sensor is shown in Figure 3 along with the corresponding color coded wiring.  Interestingly, GEFRAN call this a Rectilinear Displacement sensor, which is really just saying it measures  displacement in 1D.  Since displacement is a vector and distance a scalar, and the output of the sensor is a scalar quantity, I prefer to refer to this as a distance sensor for simplicity.  Others might argue over semantics.  The equivalent circuit is also shown in Figure 3, where the sensor acts as simply a continuous variable resistor.  Importantly, the start and end ranges are not valid and over about 1-1.5mm on each end, there is no valid output.  I have accounted for that in the firmware and reported out-of-range values in these ranges.<P></P>

<div style="float: left"> <img src="./PY2.png" alt="Figure 3. GEFRAN PY2F10 Rectilinear Displacement sensor with color coded wiring to the Arduino  power rails and the ADS1115S ADC."   width="800"/> </div> <figcaption > <I><b>Figure 3:</b> GEFRAN PY2F10 rectilinear Displacement sensor with color coded wiring to the Arduino  power rails and the ADS1115S ADC</I><figcaption><br>

## Circuit Configuration
The Schematic for the setup is shown below - the EAGLECAD layout is in the repository as [GEFRAN_DistanceSensor.sch], created using [EAGLECAD](https://www.autodesk.com/products/eagle/overview).
<div style="float: right"><img src="./SchematicLayout.PNG" alt="Figure 4. Schematic of the Arduino (Uno) interface to the GEFRAN PY2 distance sensor through the 16Bit ADC" align="right"  width="400"/> </div> <figcaption> <I><b>Figure 4:</b> Schematic of the Arduino (Uno) interface to the GEFRAN PY2 distance sensor through the 16Bit ADC </I><figcaption>
  <P></P>
The use of the ADS1115S is important for 2 reasons:
1) the ADS1115S contains an internal, high precision, temperature stabilized voltage reference.  This is essential when needing to take accurate measurements. Just as important is a stable voltage supply for the sensor (as discussed above) when I decided to use the 3.3V reference from the Arduino board.
2) the 16 bit resolution and high 820 samples-per-sec specifications of this ADC make it considerably better than using the 10-bit Arduino ADC, or even and oversampled Arduino ADC which has been shown to be capable of an [ equivalent of 16 bits but at a much slower samples per seconds](https://www.electricrcaircraftguy.com/2014/05/using-Arduino-unos-built-in-16-bit-adc.html) . The Arduino UNO fits within the small metal box (Figure 2) with a square cut out one side for the USB and a power and lead for the PY2F10S distance sensor.  

## Power Supply
Running from a  12V DC power supply (>200mA) results in significant heating of the on-board 5V regulator.  It is essential the regulator has an appropriate sized heat-sink.  When using a 12V supply, (12-5)=7V is dropped across the regulator.  If the board draws 50 mA (TBD) this suggests up to 0.35W is being dissipated in the regulator.  The Arduino uses a NCP117ST50T3G Low-Drop-Out (LDO) voltage regulator in an SOT−223 package.  The [NCP117ST50T3G datasheet] ( https://www.onsemi.com/pub/Collateral/NCP1117-D.PDF) indicates the Thermal Resistance, Junction−to−Ambient with NO HEATSINK is 160 &#176;C/W.  Hence 0.35 W power dissipation on this regulator corresponds to a 56 &#176;C rise in temperature above ambient or a nominal temperature of 81 &#176;C at room temperature (25 &#176;C).  While this is technically below the 125 &#176;C maximum operating temperature, I don’t like to approach temperatures where water can boil! During initial testing even a small 9&#215;9&#215;5mm heatsink reached extremely high temperatures and hence I decided to stack 2-small heat sinks together, with each glued in place with [heat sink adhesive](https://www.jaycar.com.au/adhesive-heatsink-compound-5g-tube/p/NM2014).  The heatsinks also make contact to the Aluminium metal lid which also helps to dissipate the heat and prevent the steady state temperature of the LDO regulator and heatsink from getting to large.  This ensures good 5V regulation and extends the life of the board.
## Firmware Configuration
The software was written under the Arduino IDE environment. The  code uses the [Adafruit ADS1X15 library]( https://github.com/adafruit/Adafruit_ADS1X15) as assigns the PY2 sensor to ADS1115-Port-A0.  The serial connection outputs data at 9600 baud.<P> 
The code [distanceSensor.ino](distanceSensor.ino) is a basic interface for testing and calibration.  It provides a fixed number of averages and simply zeros the sensor after reboot or after pressing the reset button.
## Testing
Initial testing indicated the output was reasonabily linear and a calibration factor was determined to be 0.577260406 &#181;m/ADC_Value giving the ReportedDistance = ADC_Channel_A0*0.577260406 &#181;m/ADC_Value.

The Repeatability and Calibration Check data which follows can be found in the file (TestingData.xlsx).
#Repeating zeroing.
The stage was bolted to the bench and the plunger stage of the PY2 sensor lifted repeatedly. Over this data is shown in Figure 5.
<div style="float: right"><img src="./RepeatabilityCheck.PNG" align="right"  width="400"/> </div> <figcaption> <I><b>Figure 5:</b> Measured data during a repeatibility check of the return to zero value of this sensor </I><figcaption>
  <P></P>
  The reported zero values were then summarised in the following statistics:
When the movement region of PY2 sensor rode was only small (<2 mm) a check of the zero values indicated the average was 2.12 &#181;m and the standard deviaton of the data was 5.26 &#181;m.
When the movement region of PY2 sensor rode was large (~10 mm) a check of the zero values indicated the average was -13.56 &#181;m and the standard deviaton of the data was 9.75 &#181;m.
These results indicate that for small movement of the PY2 rod, the average repeatability was high even though the spread of the data was up to 5.26 &#181;m.  When large movements of the main rod occurs, there is significant drift in the zero and the spread in the data (the standard deviation) almost doubles.  This suggests that perhasp frictional losses due to large movements results in a shift in the absolute measurement value.  Never that less, over 10 mm movement, an average drift in the data of -13.56 &#181;m represents only a drift of 0.0014%.

#Calibration.
A digital calliper was used as a reference measure and expected to be accurate to [20 &#181;m](https://www.jaycar.com.au/medias/sys_master/images/9318637535262/TD2082-manualMain.pdf) up to 10mm distance.  Overall the calibration needs to be repeated as the results are considered ectremely poor.  While the output appears vry linear, the standard deviation of the resisduals between the actual distance and the reported distance is around 180 &#181;m - the expected difference was in the or of 20-50 &#181;m, and hence a reach of thsi data is required to ensure the calibration was performed correctly.
<div style="float: right"><img src="./CalibrationCheck.PNG" align="right"  width="400"/> </div> <figcaption> <I><b>Figure 6:</b> Measured data checking the reported (calibrated) output of the sensor against the digital caliper </I><figcaption>
  <P></P>

