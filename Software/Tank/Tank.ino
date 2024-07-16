#include <PS2X_lib.h>
#define PS2_DAT        13  //14    
#define PS2_CMD        11  //15
#define PS2_SEL        10  //16
#define PS2_CLK        12  //17

#define pressures   false
#define rumble      false

// 9 to 15 PWM pins

#define INDICATOR_LED
#define BUZZER

#define LEFT_WHEEL_FORWARD
#define LEFT_WHEEL_BACKWARD
#define RIGHT_WHEEL_FORWARD
#define RIGHT_WHEEL_BACKWARD

#define LEFT_ROLLER_FIRE
#define LEFT_ROLLER_RELOAD
#define RIGHT_ROLLER_FIRE
#define RIGHT_ROLLER_RELOAD

#define ARM

#define INTAKE_FAN_DRAW
#define INTAKE_FAN_BLOW

#define LOADER_PUSH
#define LOADER_PULL

#define LOADER_FAN_DRAW
#define LOADER_FAN_BLOW

PS2X ps2x;

int error = 0;
byte type = 0;
byte vibrate = 0;

void setup(){
 
  Serial.begin(57600);
  
  delay(300);
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
      Serial.print("PUSH");
    }

    if(ps2x.Button(PSB_PAD_DOWN)) {
      Serial.print("PULL");
    }

    if(ps2x.Button(PSB_PAD_RIGHT)) { // Can be replace Button() function with ButtonPressed()
      reload = !reload;
      Serial.print("RELOAD TOGGLED");
      delay(200);
    }

    if(ps2x.Button(PSB_PAD_LEFT)) { // Can be replace Button() function with ButtonPressed()
      fire = !fire;
      Serial.print("FIRE TOGGLED");
      delay(200);
    }

    if(ps2x.Button(PSB_TRIANGLE)) {
      Serial.print("HAND UP");
    }

    if(ps2x.Button(PSB_CROSS)) {
      Serial.print("HAND DOWN");
    }

    if(ps2x.Button(PSB_SQUARE)) {
      Serial.print("INTAKE FAN TOGGLED");
      delay(200);
    }
    
    if(ps2x.Button(PSB_CIRCLE)) {
      Serial.print("LOADER FAN TOGGLED");
      delay(200);
    }

    Serial.print(ps2x.Analog(PSS_LY), DEC);
    Serial.print(" ");
    Serial.println(ps2x.Analog(PSS_RX), DEC);    
  // delay(50);
  }
}
