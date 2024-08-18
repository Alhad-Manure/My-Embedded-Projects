/*
  Wireless Joystic Remote Controller With GUI

  Sends the spped and Dirction values of Joystic input to reciever.
  Also displays information like speed, Battery status, Connection Status, etc.
  on GUI

  created 6 Sep 2021
  by Alhad Manure
*/

// Joystic Inputs
const int Y_pot=A0;
const int X_pot=A1;

// Battery level pin
const int Bat=A11;

// Blutooth Status Pin
const int Ble=23;

// Battery Voltage holding
uint16_t bat_avg[20];
uint8_t bat_ind=0;

// Presets for Joystick
uint16_t Ymid=513, Xmid=513;
uint16_t Ymax=765, Xmax=773;
uint16_t Ymin=253, Xmin=246;
uint16_t Xdiff=0, Ydiff=0;

// Method to calculate average
unsigned int avg(uint16_t arr[], uint16_t sze)
{
  unsigned int sum;
  uint8_t i;
  for(i=0; i<sze; i++)
  {
    sum+=arr[i]; 
  }
  return sum/sze;  
}

void end_string()
{
  Serial3.write(0xff);
  Serial3.write(0xff);
  Serial3.write(0xff);
}

// Recording last n Battery voltage readings
void read_bat_volt(uint8_t ind, uint16_t arr[], int pin)
{
  if(ind >= 20)
  {
    ind = 0;
  }
  bat_avg[ind]=analogRead(pin);
  ind++;
}

void setup()// put your setup code here, to run once:
{
  // Initialising UARTs
  Serial3.begin(9600);
  Serial.begin(9600);

  // Initialsing Input pins
  pinMode(Y_pot,INPUT);
  pinMode(X_pot,INPUT);
  pinMode(Bat,INPUT);
  pinMode(Ble,INPUT);

  // Initiating Battery status
  for(uint8_t i=0;i<20;i++)
  {
      bat_avg[i]=analogRead(Bat);
      delay(10);
  }

  // Display Initial Battery Voltage on GUI
  Serial3.print("j0.val=");
  Serial3.print(avg(bat_avg, 20));
  end_string();  
}

// Followig are Boundry limits for Joystic Inputs
// Y=765, 513, 253 
// X=773, 513, 246

void loop()// put your main code here, to run repeatedly:
{
  // Read Joystick position
  uint16_t Y=analogRead(Y_pot);
  uint16_t X=analogRead(X_pot);

  // Map original values to 8 bit PWM Resolution
  uint8_t Ypos=map(Y,Ymin,Ymax,0,255);
  uint8_t Xpos=map(X,Xmin,Xmax,0,255);

  // This character will flag the start of command
  Serial.write('a');

  // Send Y-axis and X-axis postions of joystick
  Serial.write(Ypos);
  delay(35);
  Serial.write(Xpos);
  delay(35);
  
  // Calculating joystic value from original position
  Ydiff = abs(Y-Ymid);
  Xdiff = abs(X-Xmid);

  // Calculating speedometer reading
  // And constraing it's value within max limit 
  uint16_t speedo = Xdiff+Ydiff;
  if(speedo>(Ymax-Ymid))
  {
    speedo=(Ymax-Ymid);
  }
  
  speedo = map(speedo, 0, Ymax-Ymid, 0, 220);
  if(speedo>=0 && speedo<=40)
  {
    speedo+=320;
  }
  else
  {
    speedo = map(speedo,40,220,0,220);
  }

  // Display speed
  Serial3.print("z0.val=");
  Serial3.print(speedo);
  end_string();
 
  // Display Battery voltage
  read_bat_volt(bat_ind, bat_avg, Bat);
  uint16_t bat_charge = avg(bat_avg, 20);
  bat_charge = map(bat_charge,706,1023,0,100);
  Serial3.print("j0.val=");
  Serial3.print(bat_charge);
  end_string();
 
  Serial3.print("n0.val=");
  Serial3.print(bat_charge);
  end_string();
 
  // Display connection Status
  if(digitalRead(Ble) == HIGH)
  {
    Serial3.print("t0.txt=");
    Serial3.print("\"");
    Serial3.print("Connected");
    Serial3.print("\"");
    end_string();  
  }
  else
  {
    Serial3.print("t0.txt=");
    Serial3.print("\"");
    Serial3.print("Not Connected");
    Serial3.print("\"");
    end_string();
  }
}
