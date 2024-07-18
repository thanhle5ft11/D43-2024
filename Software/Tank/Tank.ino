#include <PS2X_lib.h>
#include <Servo.h>

#define PS2_DAT        51 // MOSI   
#define PS2_CMD        50 // MISO
#define PS2_SEL        53 
#define PS2_CLK        52

#define pressures   false
#define rumble      false

// 9 to 15 PWM pins

#define INDICATOR_LED
#define BUZZER

#define LEFT_WHEEL_FORWARD 2
#define LEFT_WHEEL_BACKWARD 3
#define RIGHT_WHEEL_FORWARD 4
#define RIGHT_WHEEL_BACKWARD 5

#define LOADER_PUSH 6
#define LOADER_PULL 7

#define LEFT_ROLLER_FIRE 8
#define LEFT_ROLLER_RELOAD 9
#define RIGHT_ROLLER_FIRE 10
#define RIGHT_ROLLER_RELOAD 11

#define ARM_PIN 12

#define INTAKE_FAN_DRAW
#define INTAKE_FAN_BLOW

#define LOADER_FAN_DRAW
#define LOADER_FAN_BLOW

#define SPREAD_POSITION 0
#define FOLD_POSITION 105

#define STICK_THRESHOLD 50

PS2X ps2x;
Servo arm;

int error = 0;
byte type = 0;
byte vibrate = 0;

int LY = 0;
int RX = 0;

int mappedLY = 0;
int mappedRX = 0;

int leftSpeed = 0;
int rightSpeed = 0;

void configPinout();
int configController();
void move();

void setup() {
  configPinout();  
  Serial.begin(115200);  
  delay(300);
  configController();
}

void loop() {
  if(error == 1) //skip loop if no controller found
    return; 
  
  if(type == 2) //Guitar Hero Controller
    return;

  else { //DualShock Controller
    ps2x.read_gamepad(false, false); //read controller

    // if(ps2x.Button(PSB_START))
    //   Serial.println("START");
    // if(ps2x.Button(PSB_SELECT))
    //   Serial.println("SELECT");      

    if(ps2x.Button(PSB_PAD_UP)) {
      Serial.println("PUSH");
    }

    if(ps2x.Button(PSB_PAD_DOWN)) {
      Serial.println("PULL");
    }

    if(ps2x.Button(PSB_PAD_RIGHT)) {        // Can be replace Button() function with ButtonPressed()
      Serial.println("RELOAD TOGGLED");
      delay(200);
    }

    if(ps2x.Button(PSB_PAD_LEFT)) {         // Can be replace Button() function with ButtonPressed()
      Serial.println("FIRE TOGGLED");
      delay(200);
    }

    if(ps2x.Button(PSB_TRIANGLE)) {
      arm.write(FOLD_POSITION);
      Serial.println("FOLD UP");
    }

    if(ps2x.Button(PSB_CROSS)) {
      arm.write(SPREAD_POSITION);
      Serial.println("SPREAD OUT");
    }

    if(ps2x.Button(PSB_SQUARE)) {
      Serial.println("INTAKE FAN TOGGLED");
      delay(200);
    }
    
    if(ps2x.Button(PSB_CIRCLE)) {
      Serial.println("LOADER FAN TOGGLED");
      delay(200);
    }

    LY = ps2x.Analog(PSS_LY);
    RX = ps2x.Analog(PSS_RX);
    
    mappedLY = map(LY, 0, 255, 254, -254);
    mappedRX = map(RX, 0, 255, -254, 254);

    leftSpeed = mappedLY + mappedRX;
    rightSpeed = mappedLY - mappedRX;

    leftSpeed = constrain(leftSpeed, -254, 254);
    rightSpeed = constrain(rightSpeed, -254, 254);

    if (abs(leftSpeed) < STICK_THRESHOLD)
      leftSpeed = 0;
    if (abs(rightSpeed) < STICK_THRESHOLD)
      rightSpeed = 0;

    Serial.print(leftSpeed);
    Serial.print(' ');
    Serial.println(rightSpeed);
    
    move(leftSpeed, rightSpeed);
  }
}

void configPinout() {
  pinMode(LEFT_WHEEL_FORWARD, OUTPUT);
  pinMode(LEFT_WHEEL_BACKWARD, OUTPUT);
  pinMode(RIGHT_WHEEL_FORWARD, OUTPUT);
  pinMode(RIGHT_WHEEL_BACKWARD, OUTPUT);

  arm.attach(ARM_PIN);
}

int configController() {
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  
  if(error == 0)
    Serial.print("Controller found, configured successful");
  else if(error == 1)
    Serial.println("No controller found");   
  else if(error == 2)
    Serial.println("Controller found but not accepting commands");
  else if(error == 3)
    Serial.println("Controller found but refusing to enter Pressures mode");
  
  type = ps2x.readType(); 
  switch(type) {
    case 0:
      Serial.print("Unknown Controller");
      break;
    case 1:
      Serial.print("DualShock Controller");
      break;
    case 2:
      Serial.print("GuitarHero Controller");
      break;
    case 3:
      Serial.print("Wireless Sony DualShock Controller");
      break;
  }
  return error;
}

void move(int leftWheelSpeed, int rightWheelSpeed) {
  if (leftWheelSpeed > 0) {
    analogWrite(LEFT_WHEEL_FORWARD, leftWheelSpeed);
    analogWrite(LEFT_WHEEL_BACKWARD, 0);
  }
  else {
    analogWrite(LEFT_WHEEL_FORWARD, 0);
    analogWrite(LEFT_WHEEL_BACKWARD, abs(leftWheelSpeed));
  }

  if (rightWheelSpeed > 0) {
    analogWrite(RIGHT_WHEEL_FORWARD, rightWheelSpeed);
    analogWrite(RIGHT_WHEEL_BACKWARD, 0);
  }
  else {
    analogWrite(RIGHT_WHEEL_FORWARD, 0);
    analogWrite(RIGHT_WHEEL_BACKWARD, abs(rightWheelSpeed));
  }
}
