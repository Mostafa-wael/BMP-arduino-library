#include <BMP.h>
BMP * Pbmp= new BMP(174);
void setup() 
{
 Serial.begin (9600);
 Pbmp->initialize ();

}

void loop() 
{
  Pbmp->Serial_print_data();
}
