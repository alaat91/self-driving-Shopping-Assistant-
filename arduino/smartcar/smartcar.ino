#include <vector>

#include <MQTT.h>
#include <WiFi.h>
#ifdef __SMCE__
#include <OV767X.h>
#endif

#include <Smartcar.h>

MQTTClient mqtt;
WiFiClient net;

//Global variable

// Pins decleration...
const auto triggerPin = 6;
const auto echoPin = 7;
const auto frontIR_PIN = 0;
const auto leftIR_PIN = 1;
const auto rightIR_PIN = 2;
const auto backIR_PIN = 3;
const auto maxDistance = 400;


unsigned int angleToTurn =            1;


bool driveMode = false;

bool pathMode1 = false;

bool pathMode2 = false;

bool pathMode3 = false;

volatile int counter;


unsigned int delayTime =           20UL;




const char ssid[] =                 "***";
const char pass[] =                "****";

const auto carSpeed =                70;
const int SPEED_INCREMENT =          40;



unsigned long previousTransmission = 0UL;
unsigned long currentTime = millis();
const auto oneSecond = 1000UL;



const int fSpeed   =                 70;  // 70% of the full speed forward
const int bSpeed   =                -70; // 70% of the full speed backward
const int lDegrees =                -75; // degrees to turn left
const int rDegrees =                 75;  // degrees to turn right


ArduinoRuntime arduinoRuntime;
BrushedMotor leftMotor(arduinoRuntime, smartcarlib::pins::v2::leftMotorPins);
BrushedMotor rightMotor(arduinoRuntime, smartcarlib::pins::v2::rightMotorPins);
DifferentialControl control(leftMotor, rightMotor);


SimpleCar car(control);


SR04 US_front(arduinoRuntime, triggerPin, echoPin, maxDistance);
GP2Y0A02 FrontIR(arduinoRuntime, frontIR_PIN);
GP2Y0A02 backIR(arduinoRuntime, backIR_PIN );
GP2Y0A02 leftIR(arduinoRuntime, leftIR_PIN );
GP2Y0A02 rightIR(arduinoRuntime, rightIR_PIN);



std::vector<char> frameBuffer;



void setup() {

  Serial.begin(9600);


#ifdef __SMCE__
  Camera.begin(QVGA, RGB888, 15);
  frameBuffer.resize(Camera.width() * Camera.height() * Camera.bytesPerPixel());

#else
  WiFi.begin(ssid, pass);
  mqtt.begin(net);
#endif


  Serial.println("Connecting to WiFi...");
  auto wifiStatus = WiFi.status();
  while (wifiStatus != WL_CONNECTED && wifiStatus != WL_NO_SHIELD) {
    Serial.println(wifiStatus);
    Serial.print(".");
    delay(1000);
    wifiStatus = WiFi.status();
  }


  Serial.println("Connecting to MQTT broker");
  while (!mqtt.connect("arduino", "public", "public")) {
    Serial.print(".");
    delay(1000);
  }


  if (mqtt.connect("arduino", "public", "public")) {
    mqtt.subscribe("/smartcar/control/#", 1);
    mqtt.onMessage([](String topic, String message) {
      if (topic == "/smartcar/control/throttle") {
        car.setSpeed(message.toInt());

      } else if (topic == "/smartcar/control/steering") {

        car.setAngle(message.toInt());
      } else if ( topic == "/smartcar/control/DriveMode") {
        if (message.toInt() == 1) {
          
          driveMode = true;
          pathMode1 = false;
          pathMode2 = false;
          pathMode3 = false;
          car.setSpeed(carSpeed);
          aimlessDriveControl();
          
        }
        else if (message.toInt() == 0) {
          
          driveMode = false;
          pathMode1 = false;
          pathMode2 = false;
          pathMode3 = false;
          manualControl();
          
        } else if (message.toInt() == 2)
        {
          driveMode = false;
          pathMode1 = true;
          pathMode2 = false;
          pathMode3 = false;
          path01();

        } else if (message.toInt() == 3)
        {
          driveMode = false;
          pathMode1 = false;
          pathMode2 = true;
          pathMode3 = false;
          path02();

        }else if (message.toInt() == 4)
        {
          driveMode = false;
          pathMode1 = false;
          pathMode2 = false;
          pathMode3 = true;
          path03();

        }

      }
      else {
        Serial.println(topic + " " + message);
      }
    });
  }
}


void loop() {

  if (driveMode == true && pathMode1 == false && pathMode2 == false && pathMode3 == false )
  {
    aimlessDriveControl();
  }
  else if (driveMode == false  && pathMode1 == false && pathMode2 == false && pathMode3 == false)
  {
    manualControl();
  }

  else if (driveMode == false  && pathMode1 == true && pathMode2 == false && pathMode3 == false)
  {
    path01();
    detectObstacle();
    
  } else if (driveMode == false  && pathMode1 == false && pathMode2 == true && pathMode3 == false)
  {
    path02();
    detectObstacle();

  }
  else if (driveMode == false  && pathMode1 == false && pathMode2 == false && pathMode3 == true)
  {
    path02();
    detectObstacle();

  }






  if (mqtt.connected()) {
    mqtt.loop();
    const auto currentTime = millis();

#ifdef __SMCE__
    static auto previousFrame = 0UL;
    if (currentTime - previousFrame >= 65) {
      previousFrame = currentTime;
      Camera.readFrame(frameBuffer.data());
      mqtt.publish("/smartcar/camera", frameBuffer.data(), frameBuffer.size(),
                   false, 0);
    }

#endif

  }

#ifdef __SMCE__
  // Avoid over-using the CPU if we are running in the emulator
  delay(1);
#endif
}




void manualControl()
{ // handle serial input if there is any



  if (Serial.available())
  {
    char input = Serial.read(); // read everything that has been received so far and log down
    // the last entry
    switch (input)
    {
      case 'a': // rotate counter-clockwise going forward
        car.setSpeed(fSpeed);
        car.setAngle(lDegrees);
        break;
      case 'd': // turn clock-wise
        car.setSpeed(fSpeed);
        car.setAngle(rDegrees);
        break;
      case 'w': // go ahead
        car.setSpeed(fSpeed);
        car.setAngle(0);
        break;
      case 's': // go back
        car.setSpeed(bSpeed);
        car.setAngle(0);
        break;
      default: // if you receive something that you don't know, just stop
        car.setSpeed(0);
        car.setAngle(0);
    }
  }
}




void aimlessDriveControl()
{
  auto distance_front_IR = FrontIR.getDistance();
  auto US_front_distance = US_front.getDistance();


  if (US_front_distance > 100 && US_front_distance < 200 || distance_front_IR > 100 && distance_front_IR < 125)
  {
    Serial.print("Obstacle has been detected");
    car.setSpeed(carSpeed - SPEED_INCREMENT);
  }

  else if (US_front_distance > 0 && US_front_distance <= 100 || distance_front_IR > 0 && distance_front_IR <= 100)
  {

    car.setSpeed(0);
    static long previousTransmission = 0UL;
    long currentTime = millis();

    const auto oneSecond = 1000UL;


    Serial.println(currentTime);
    if (currentTime - previousTransmission >= oneSecond * delayTime)
    {

      car.setSpeed(-50);
      delay(1000);
      randomAngle();
      delay(2000);
      car.setAngle(0);
      car.setSpeed(carSpeed);
      previousTransmission = currentTime;
      currentTime = 0UL;
    }
  }

  else
  {
    car.setSpeed(carSpeed);
  }
}


void randomAngle() //Generate a random angle between 1 and 180 degrees each time to try to avoid an obstacle
{
  int angle = angleToTurn * rand() % 180;
  car.setAngle(angle);

}


void costumerDelay()    //Publish to a button on the website and when then button is pressed
                        // the car gives a 1-minute delay so the customer can interact with the car.
{
  car.setSpeed(70);
  static long previousTransmission = 0UL;
  long currentTime = millis();
  const auto oneSecond = 1000UL;
  Serial.println(currentTime);
  if (currentTime - previousTransmission >= oneSecond * 60)
  {
    previousTransmission = currentTime;
    currentTime = 0UL;
    car.setSpeed(100);
  }
}



bool freeToGo = false;
unsigned int firstInterval = 4000UL;


void path01()
{
  pathMode1 = true;
  pathMode2 = false;
  pathMode3 = false;
  freeToGo = true;
  auto distance_front_IR = FrontIR.getDistance();
  auto US_front_distance = US_front.getDistance();
if (detectObstacle && freeToGo)
  {
  car.setSpeed(50);
  detectObstacle();
  delay(1500);
  detectObstacle();
  delay(1500);
  detectObstacle();
  car.setAngle(60);
  detectObstacle();
  delay(2000);
  detectObstacle();
  car.setAngle(0);
  detectObstacle();
  car.setSpeed(60);
  detectObstacle();
  delay(3000);
  detectObstacle();
  car.setAngle(60);
  detectObstacle();
  delay(2000);
  detectObstacle();
  car.setAngle(0);
  detectObstacle();
  car.setSpeed(50);
  detectObstacle();
  delay(1500);
  detectObstacle();
  delay(1500);
  detectObstacle();
  car.setAngle(60);
  detectObstacle();
  delay(2000);
  detectObstacle();
  car.setAngle(0);
  }else 
  {
    car.setSpeed(0);
  }

}



void path02()
{
  pathMode1 = false;
  pathMode2 = true;
  pathMode3 = false;
  freeToGo = true;

  auto distance_front_IR = FrontIR.getDistance();
  auto US_front_distance = US_front.getDistance();

  if (detectObstacle && freeToGo)
  {
    car.setSpeed(50);
    detectObstacle();
    delay(2000);
    detectObstacle();
    delay(2000);
    detectObstacle();
    car.setAngle(90);
    detectObstacle();
    delay(1500);
    detectObstacle();
    car.setAngle(0);
    detectObstacle();
    car.setSpeed(50);
    detectObstacle();
    delay(1500);
    detectObstacle();
    car.setAngle(90);
    detectObstacle();
    delay(1000);
    detectObstacle();
    car.setAngle(0);
    detectObstacle();
    car.setSpeed(50);
    detectObstacle();
    delay(2000);
    detectObstacle();
    delay(2000);
    detectObstacle();
    car.setAngle(-90);
    detectObstacle();
    delay(1000);
    detectObstacle();
    car.setAngle(0);
    detectObstacle();
    car.setSpeed(50);
    detectObstacle();
    delay(1500);
    detectObstacle();
    car.setAngle(-90);
    detectObstacle();
    delay(1000);
    detectObstacle();
    car.setAngle(0);
    detectObstacle();
    car.setSpeed(50);
    detectObstacle();
    delay(4000);
    detectObstacle();
    car.setAngle(-180);
    detectObstacle();
    delay(3000);
    detectObstacle();
    car.setAngle(0);
    detectObstacle();
  }else 
  {
    car.setSpeed(0);
  }

}

void path03()
{
  pathMode1 = false;
  pathMode2 = false;
  pathMode3 = true;
  freeToGo = true;
  auto distance_front_IR = FrontIR.getDistance();
  auto US_front_distance = US_front.getDistance();

  if (detectObstacle && freeToGo)
  {
  car.setSpeed(50);
  detectObstacle();
  delay(1000);
  detectObstacle();
  delay(1000);
  detectObstacle();
  delay(1000);
  detectObstacle();
  delay(1000);
  detectObstacle();
  car.setAngle(90);
  detectObstacle();
  delay(1000);
  detectObstacle();
  car.setAngle(0);
  detectObstacle();
  car.setSpeed(50);
  detectObstacle();
  delay(1000);
  detectObstacle();
  delay(1000);
  detectObstacle();
  delay(1000);
  detectObstacle();
  delay(1000);
  detectObstacle();
  car.setAngle(90);
  detectObstacle();
  delay(1000);
  detectObstacle();
  car.setAngle(0);
  detectObstacle();
  }else
  {
    car.setSpeed(0);
  }
}


bool detectObstacle()  //To detect obstacle and stop the car when facing an obstacle.
                       //We assume that the obstacle is a customer in this case,
                       //since the path is designed to make the car roam freely in an obstacle-free environment.
                     
{
  auto distance_front_IR = FrontIR.getDistance();
  auto US_front_distance = US_front.getDistance();

  if (US_front_distance > 50 && US_front_distance <= 100 || distance_front_IR > 50 && distance_front_IR <= 100)
  {
    car.setSpeed(30);
    
  }
  if (US_front_distance > 0 && US_front_distance <= 50 || distance_front_IR > 0 && distance_front_IR <= 50)
  {
    freeToGo = false;
    Serial.println("An obstacle is found!!!");
    car.setSpeed(0);
  }

  return false;
}
