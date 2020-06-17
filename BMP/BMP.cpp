#include "BMP.h"

/**************************-**********************************
*															 *
				   Utility functions
*															 *
***************************-*********************************/
short BMP:: READ_REG (unsigned char regs){
  short regs_combine,regs1,regs2;
  Wire.beginTransmission(BMP_adress);
  Wire.write(regs);
  Wire.endTransmission();
  Wire.requestFrom(BMP_adress,2);
  if (Wire.available()==2)
  {
    regs1 = Wire.read();
    regs2 = Wire.read();
  }
  regs_combine=((regs1<<8) | regs2);
  return regs_combine;
}
long BMP::READ_REGR (char regrs){
  unsigned long regrs_combine,regrs1,regrs2,regrs3;
  Wire.beginTransmission(BMP_adress);
  Wire.write(regrs);
  Wire.endTransmission();
  Wire.requestFrom(BMP_adress,3);
  if (Wire.available()==3)
  {
    regrs1 = Wire.read();
    regrs2 = Wire.read();
    regrs3 = Wire.read();
  }
  return ((regrs1<<16) | (regrs2<<8) | regrs3)>>(8-OSS_value);

}
/**************************-**********************************
*															 *
					Initialization
*															 *
***************************-*********************************/
// A constructor which takes the altitude of the ground from the sea level and calibrates it and the OSS's value , the initial value of OSS is 0 (medium accuracy)
BMP::BMP(double altitude_factor, int OSS)
{
	BMP_adress=0x77;
	OSS_value=OSS;

    to_calibrate_altitude=altitude_factor;
	//initialize();
}
//initializing the sensor and reading data , N.B : check the datasheet for more details
void BMP::initialize ()
{
 //Serial.begin(9600);
 Wire.begin();
 // datasheet BMP page 15
//1st block
BMP_AC1 = READ_REG( AC1 );
BMP_AC2 = READ_REG( AC2 );
BMP_AC3 = READ_REG( AC3 );
BMP_AC4 = READ_REG( AC4 );
BMP_AC5 = READ_REG( AC5 );
BMP_AC6 = READ_REG( AC6 );
BMP_B1 = READ_REG( B1 );
BMP_B2 = READ_REG( B2 );
BMP_MB = READ_REG( MB );
BMP_MC = READ_REG( MC );
BMP_MD = READ_REG( MD );
////////////////////////////
Serial.print("AC1: ");
Serial.println(BMP_AC1);
Serial.print("AC2: ");
Serial.println(BMP_AC2);
Serial.print("AC3: ");
Serial.println(BMP_AC3);
Serial.print("AC4: ");
Serial.println(BMP_AC4);
Serial.print("AC5: ");
Serial.println(BMP_AC5);
Serial.print("AC6: ");
Serial.println(BMP_AC6);
Serial.print("B1: ");
Serial.println(BMP_B1);
Serial.print("B2: ");
Serial.println(BMP_B2);
Serial.print("MB: ");
Serial.println(BMP_MB);
Serial.print("MC: ");
Serial.println(BMP_MC);
Serial.print("MD: ");
Serial.println(BMP_MD);

}
void BMP::Read_data()
{

//2nd block in datasheet
Wire.beginTransmission(BMP_adress);
Wire.write(CONTROL);
Wire.write(Temperature);
Wire.endTransmission();
delay(5);
UT=(long)READ_REG(0xF6);


//3rd block in datasheet
Wire.beginTransmission(BMP_adress);
Wire.write(CONTROL);
Wire.write(PRESSURE | (OSS_value<<6));
Wire.endTransmission();
delay(5);
UP = READ_REGR(0xF6);


//4th block -> calculating temperature
X1 = ((UT- BMP_AC6)*BMP_AC5)/(pow(2,15));
X2 = (BMP_MC*(pow(2,11)))/(X1+BMP_MD);
B5 = X1+X2;
temperature = (B5+8) /16;

//5th block -> calculating Pressure
B6 = B5-4000;
X1 = (BMP_B2 * ((B6 * B6)/4096))/2048;
X2 = (BMP_AC2 * B6)/2048;
X3 = X1 + X2;
B3 = ((((long)BMP_AC1*4 + X3)<<OSS_value) + 2)/(4);  // long casting
X1 = (BMP_AC3 * B6)/8192;
X2 = (BMP_B1 * ((B6 * B6)/4096))/65536;
X3 = ((X1 + X2) + 2)/4;
B4 = (BMP_AC4 * (unsigned long)(X3 + 32768))/32768;
B7 = (((unsigned long)UP- B3) * (50000>>OSS_value));
if (B7 < 0x80000000) {Pressure = (B7*2)/B4;}
else { Pressure = (B7/B4)*2;}
X1 = (Pressure/256) * (Pressure/256);
X1= (X1 * 3038)/65536;
X2 = (-7357 * Pressure)/65536;
Pressure += (X1 + X2 + 3791)/16;

//calculating altitude
altitude=  44330*(1-(pow((Pressure/101325.0),(1/5.255)))); // where sealevel_Pressure = 101325
calibrate_altitude(to_calibrate_altitude);

}
/**************************-**********************************
*															 *
						Setters
*															 *
***************************-*********************************/
void BMP::set_temperature(float t)
{
	temperature=t;
}
void BMP::set_Pressure(long p)
{
	Pressure=p;
}
void BMP::set_altitude(double a)
{
	altitude=a;
}
void BMP::calibrate_altitude (double a)
{
    altitude-=a;
}
/**************************-**********************************
*															 *
						Getters
*															 *
***************************-*********************************/
long BMP::get_sealevel_pressure(float altitude_meters )
{
  return (get_Pressure() / pow(1.0-altitude_meters/44330, 5.255));
}
float BMP::get_temperature()
{
return temperature;
}
long BMP::get_Pressure()
{
return Pressure;
}
float BMP::get_altitude()
{
	return altitude;
}


//print the data on the serial monitor
void BMP::Serial_print_data (bool Read_Data=1)
{
if(Read_Data)
    {Read_data ();}
Serial.print("BMP: ");
Serial.print("Pressure= ");
Serial.print(Pressure);
Serial.print (", ");
Serial.print("Altitude= ");
Serial.print(altitude);
Serial.print (", ");
Serial.print("Temperature= ");
Serial.println(temperature/10.0);
Serial.println("   ");

}
void BMP::Serial_print_data_LabView(bool Read_Data=1)
{
    if(Read_Data)
    {Read_data ();}
    Serial.print("BMP,");
    Serial.print(Pressure);
    Serial.print(",");
    Serial.print(altitude);
    Serial.print(",");
    Serial.println(temperature/10.0);

}

BMP::~BMP(void)
{

}
