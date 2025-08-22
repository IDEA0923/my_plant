#include "BluetoothSerial.h"

BluetoothSerial CON1;
#define pin_of_the_motor D2
#define pin_of_the_lamp D2

#define BUT1        //lamp
#define BUT2        //motor
#define BUT3

bool was_update = 0;
struct ST {
  int motor;
  int lamp;
  bool lamp_on;
  bool motor_on;
};

ST status;
void setup(){
  pinMode(BUT1 , INPUT);
  pinMode(BUT2 , INPUT);

  status.motor = 0; status.lamp = 0;status.lamp_on =1;status.motor_on =1;
  Serial.begin(115200);
  CON1.begin("MyPlants");
}
void lamp_onoff(){
  if(status.lamp_on){
      status.lamp_on = 0;
      analogWrite(pin_of_the_lamp , 0);
    }else{
      status.lamp_on = 1;
      analogWrite(pin_of_the_lamp , status.lamp);
    }
}
void motor_onoff(){
  if(status.motor_on){
      status.motor_on = 0;
      analogWrite(pin_of_the_motor , 0);
    }else{
      status.motor_on = 1;
      analogWrite(pin_of_the_motor , status.motor);
    }
}
int st_of_ten(int a){
  int i = 1;
  while(a > 0){
    i= i * 10;
    a--;
  }
  return i;
}
int get_val(String msg){
  String buff= "";
  for(int i = 2 ; ((int)msg[i] >= (int)'0' ) && ((int)msg[i] <= (int)'9') ; i++){
    buff = buff + msg[i];
  }
  int val = 0;
  int ind = 0;
  for(int a = buff.length() ; a > 0; a--){
    
    val = val +( ( (int)buff[a-1] - (int)'0' ) * st_of_ten(ind));
    ind++;
  }
  return val;
}

String read_the_command(){
  String ans = "";
  while(CON1.available()){
    ans = ans + (char)CON1.read();
  }
  return ans;
}
void snd(String command){
  if(CON1.hasClient()){
    CON1.write((const uint8_t*)command.c_str() , command.length());
  }
}
void loop(){
  if(digitalRead(BUT1) == HIGH){//lamp
    lamp_onoff();
  }
  if(digitalRead(BUT2) == HIGH){//motor
    motor_onoff();
  }
  if(CON1.available()){
    String comm = read_the_command();
    Serial.print(comm);
    if(comm[0] =='G'){
      if(comm[1] == 'M'){
        snd(String(status.motor)+'\n');
      }else if(comm[1]== 'L'){
        snd(String(status.lamp)+'\n');
      }
    }else if(comm[0] == 'S'){
      was_update = 1;
      if(comm[1] == 'M'){
        Serial.println("command:"+comm+"!");
        status.motor = get_val(comm);
        Serial.println("motor:"+(String)status.motor+"!");
      }else if(comm[1]== 'L'){
        Serial.println("command:"+comm+"!");
        status.lamp = get_val(comm);
        Serial.println("lamp:"+(String)status.lamp+"!");
      }
    }else if(comm[0] == 'O'){
      if(comm[1] == 'M'){
        Serial.println("command:"+comm+"!");
        motor_onoff();
        
      }else if(comm[1]== 'L'){
        Serial.println("command:"+comm+"!");
        lamp_onoff();
      }
    }
  }

  if(was_update){
    analogWrite(pin_of_the_motor , status.motor);
    analogWrite(pin_of_the_lamp , status.lamp);
    Serial.println("UPDATE!");
    status.lamp_on =1;status.motor_on=1;
    was_update = 0;
  }

}