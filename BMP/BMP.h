#pragma once

#include <Wire.h>
#include "Arduino.h"


class BMP //for temperature ,Pressure and altitude

{
 private : short BMP_AC1 ,BMP_AC2 ,BMP_AC3,BMP_B1,BMP_B2,BMP_MB,BMP_MC,BMP_MD;
           long UT,UP,X1,X2,X3,B3,B5,B6,Pressure;
           float temperature,altitude,to_calibrate_altitude;
           unsigned char  OSS;
           unsigned short BMP_AC4,BMP_AC5,BMP_AC6;
           unsigned long B4,B7;
           short READ_REG (unsigned char regs);
           long READ_REGR (char regrs);
		   /////////////////////////////////////
		   char BMP_adress;
		   bool is_intilaized;

		   int OSS_value;
		   double get_real_altitude(float);



 public :
          BMP(double to_calibrate_altitude=0, int OSS=0);
          void initialize(); // should be written after the serial.begin command

		  void set_temperature(float);
		  void set_Pressure(long);
		  void set_altitude(double);
		  void calibrate_altitude (double);


		  float get_temperature();
		  long get_Pressure();
		  long get_sealevel_pressure(float altitude_meters = 0);
		  float get_altitude();


		  void Read_data(); //should be used before getting any data
          void Serial_print_data (bool read=1);
          void Serial_print_data_LabView(bool read=1);


		  ~BMP();
};
//MACROS:
//#define BMP_adress 0x77
#define AC1 0xAA
#define AC2 0xAC
#define AC3 0xAE
#define AC4 0xB0
#define AC5 0xB2
#define AC6 0xB4
#define B1 0xB6
#define B2 0xB8
#define MB 0xBA
#define Temperature 0x2E
#define CONTROL 0xF4
#define PRESSURE 0x34
#define MC 0xBC
#define MD 0xBE


