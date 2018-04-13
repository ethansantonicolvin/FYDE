/*
  SerialPassthrough from ESP to Mega
*/

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial1.begin(9600);
  //Serial.println("Hello from Arduino - Press reset to start up the WiFi");
  //Serial1.println("Hi Blynky\n Help me");
}

void loop() {
  if (Serial.available()) {      // If anything comes in Serial (USB),
    Serial1.write(Serial.read());   // read it and send it out Serial1 (pins 0 & 1)
  }

  if (Serial1.available()) {     // If anything comes in Serial1 (pins 0 & 1)
    //Serial.write("5");
    Serial.write(Serial1.read());   // read it and send it out Serial (USB)
    
  }
}

/*
 * Parser code #1
 * 
char inData[80];
byte index = 0;

void loop()
{
  while(Serial.available() > 0)
  {
     char aChar = Serial.read();
     if(aChar == '\n')
     {
        // End of record detected. Time to parse

        index = 0;
        inData[index] = NULL;
     }
     else
     {
        inData[index] = aChar;
        index++;
        inData[inex] = '\0'; // Keep the string NULL terminated
     }
  }
}



 */

 /*
  * Parser #2
  * 
#include <string.h>

char sz[] = "3500,270,890,70,4";
void setup()
{
char *p = sz;
char *str;
Serial.begin(115200);
while ((str = strtok_r(p, ",", &p)) != "\n") // delimiter is the semicolon
  Serial.println(str);
}

void loop(){}
  */

  /*
   * 
   * 
  int id1, id2, id3;
  int pos1, pos2, pos3;
  char* buf = "1:90&2:80&3:180";
  int n = sscanf(buf, "%d:%d&%d:%d&%d:%d", &id1, &pos1, &id2, &pos2, &id3, &pos3);
  Serial.print(F("n="));
  Serial.println(n);
  Serial.print(F("id1="));
  Serial.print(id1);
  Serial.print(F(", pos1="));
  Serial.println(pos1);
  Serial.print(F("id2="));
  Serial.print(id2);
  Serial.print(F(", pos2="));
  Serial.println(pos2);
  Serial.print(F("id3="));
  Serial.print(id3);
  Serial.print(F(", pos3="));
  Serial.println(pos3);

   */
   /*
    * 
 
char str[] = "1:90&2:80&3:180";     // test sample serial input from servo
int servoId;
int position;

char* p = str;
while (sscanf(p, "%d:%d", &servoId, &position) == 2)
{
    // process servoId, position here
    //
    while (*p && *p++ != '&');   // to next id/pos pair
}

    */
