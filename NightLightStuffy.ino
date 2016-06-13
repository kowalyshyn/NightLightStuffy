
/**************************************************************************************
   ISTE 2016 Workshop: The new WWW -- Wonderful World of Wearables
   Night Light Stuffy

   How it works -:
   
   Read value from light sensor conntected to A2, serial monitor displays live data
   Returns values 0 to 1023 (1024 steps or 2^10 - 10 bit resolution)
   Value is scaled to V units with the formula: sensorValue * (5V /1023)
   Note digital pins provide up to: 40mA and 5V (0.2W power)

   RGB LED is connected to digital pins 9, 10, 11
   Depending on light intensity, a PWM value is written back to pins 9, 10, 11 (analogWrite command)

   
   Alternative method for (+) and (-) power connections on digital pins
   
   // define digital power and ground pins for example on 2 and 3
   int componentPower = 2;
   int componentGround = 3;

   // put into output mode
   pinMode(componentPower,OUTPUT);
   pinMode(componentGround,OUTPUT);

   // power up component
   digitalWrite (componentPower, HIGH);  //Supply 5V
   digitalWrite (componentGround, LOW);  //Ground 0V

   TROUBLESHOOTING THE ARDUINO ENVIRONMENT
   -delete preferences file ~/library/preferences/com.arduino.
   -restart Arduino application

   DISABLE ROOTLESS MODE OSX 10.11 (El Capitan)
   1. Reboot into recovery mode (cmd-R)
   2. Log into terminal and enter: csrutil disable or csrutil enable --without kext (to only disable kernal extensions like serial drivers)
   3. Reboot
   4. Check using csrutil status
   
   http://tzapu.com/2015/09/24/making-ch340-ch341-serial-adapters-work-under-el-capitan-os-x/

   
 **************************************************************************************/
 
//Define constants - values do not change in program from starting state

#define sensorPin A5       //sensor pin, reads voltage in 10-bit resolution (0-1023)
#define redPin 11          //R petal on RGB LED module connected to digital pin 11
#define greenPin 9         //G petal on RGB LED module connected to digital pin 9
#define bluePin 10         //B petal on RGB LED module connected to digital pin 10
#define time_delay 0       //set delay that LED will display the color, used a test variable, normal set to 0

//Define variables

int sensorValue = 0;        //variable that returns value from sensorPin
float sensorVoltage = 0.0;  //float variable to hold converted voltages
float threshold1 = 0.08;    //RED, turn on when room is dark
float threshold2 = 0.50;    //GREEN, turn on normal light conditions
float threshold3 = 1.20;    //BLUE, turn on when light is bright
float thresholdMax = 2.00;  //Very bright room, turn off LED

void setup() {
  // initialize serial communication at 9600 baud
  Serial.begin(9600);

  // set up pins connected to RGB module
  pinMode(redPin, OUTPUT);    // sets the redPin to be an output
  pinMode(greenPin, OUTPUT);  // sets the greenPin to be an output
  pinMode(bluePin, OUTPUT);   // sets the bluePin to be an input

}

// define color function to set colors in RGB values from 0-255
void color (unsigned char red, unsigned char green, unsigned char blue)
{
  analogWrite(redPin, 255 - red);
  analogWrite(bluePin, 255 - blue);
  analogWrite(greenPin, 255 - green);
}


// the main loop routine runs over and over again forever:
void loop() {

  // STEP 1 - read the input on analog pin 0:
  sensorValue = analogRead(sensorPin);

  // STEP 2 - convert to equivalent voltage value from value read between 0-1023
  sensorVoltage = sensorValue * (5.0 / 1023.0);


  // STEP 3 - activate LED depending on light intensity using an elif ladder
  if (sensorVoltage < threshold1) {
    color(255, 0, 0);   //turn the RGB LED red
    delay(time_delay);  //delay to hold light before changing
  }

  else if ((sensorVoltage > threshold1) && (sensorVoltage < threshold2)) {
    color(0, 255, 0);   //turn the RGB LED green
    delay(time_delay);
  }

  else if ((sensorVoltage > threshold2) && (sensorVoltage < threshold3)) {
    color(0, 0, 255);   //turn the RGB LED blue
    delay(time_delay);
  }

  else if ((sensorVoltage > threshold3) && (sensorVoltage < thresholdMax)) {
    color(0, 0, 0); //turn off LED, set to no color.
  }

  // output to serial monitor for review as needed
  Serial.print("Light sensor value is: ");
  Serial.print(sensorVoltage);
  Serial.println(" Volts");
  delay(200);   // delay in between reads for viewing
}


