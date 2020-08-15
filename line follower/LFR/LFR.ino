#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3f,16,2);


#define sn 5
#define multiple 10
#define tp 52
#define mid 40
#define pRatio 50
#define iRatio 50
#define dRatio 10
#define sRatio 1



#define ledPin 22
#define pidPin A8
#define resetPin 51
#define setPin 50


#define memoryPin 100
#define senMem 200
#define prevSenMem 220
#define LSpeedPin  300
#define RSpeedPin  310
#define changePin  333
#define errorPin   330
#define lastErrorPin   331
#define lmPwm      351
#define lSign      350
#define rSign      360
#define rmPwm      361



#define leftMotor1  6
#define leftMotor2  5
#define rightMotor1 4
#define rightMotor2 3
#define rmPWM 2
#define lmPWM 7
#define lmSpeed 100
#define rmSpeed 100
#define speedMax 250





float kp, ki, kd, kpb, kpw, kib, kiw, kdb, kdw; 
float pidRatiob = 1.1, pidRatiow = 1.0, speedRatiob = 1.5, speedRatiow = 1.0, Pid;
int speedB, speedW, speedD, leap, i, j, k, f, n=0, cn=0, sensor[sn];
signed int e, error, lastError;




void rest(){
void(* rest) (void) = 0;}

void setup() {
  Serial.begin(9600);
  Serial.println("Beginning");

  lcd.init();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  
  delay(1000);
  pin();
  lcd.clear();
  setThings();
 // test();
  
  

}

void loop() {
  signed int  i, j,k;
  double pos;
  setThings;
  pos=IRread(0);
  if(romR(changePin)==1){
    lastErrorW(errorR());
  Serial.print("  got here : ");
   Serial.print(errorR());
   Serial.print("  shape : ");
   Serial.print(Shape());
   
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print(pos);
   lcd.setCursor(9,0);
   lcd.print(errorR());
   lcd.setCursor(14,0);
   lcd.print(Shape());
   lcd.setCursor(0,1);
   
   
   movement(Shape()) ;
   Serial.println("  ");
   PIDcal(errorR());
   delay(100);
  }   

  

}

char LOOP(){
  signed int  i, j,k;
  double pos;
  pos=IRread(0);
  if(romR(changePin)==1){
   //Serial.print(errorR());
   Serial.print("  shape : ");
   Serial.print(Shape());
   lcd.print(Shape());
  }
    
  }

void test(){
//    PIDcal();
//    delay(1000);
//    PIDcal();
//    delay(1000);
//    PIDcal();
//    delay(1000);

 
 }

void Lcd(){
  lcd.init();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
}

void lcdprint(int digit, signed int val){

  int l=length(val);
  int gap = digit-l;

  for(i=0; i<gap; i++){
    lcd.print(" ");
  }
  
   
}

int length(signed int x){
  for(i=0; x!=0; i++){
    j=x/10;
    k=x%10;
    x=j;
  }
  return i;
}





void pin(){
  int i,j;

  for(i=0; i<16; i++){
  pinMode(A0+i, INPUT);  
  }
  for(i=2; i<=13; i++){
  pinMode(i, INPUT);  
  }

  pinMode(setPin, INPUT);
  pinMode(resetPin, INPUT);
  digitalWrite(resetPin,LOW);
  
}

void setThings(){
  int i,j;
  
 // if(digitalRead(setPin)==HIGH){
 // Serial.println("Writing DATA....");
  kp=KP(0);  EEPROM.write(memoryPin+0, kp); //Serial.print("kp: "); Serial.print(kp); Serial.print(" ");
  ki=KI(0);  EEPROM.write(memoryPin+1, ki);  //   Serial.print("ki: "); Serial.print(ki); Serial.print(" ");
  kd=KD(0);  EEPROM.write(memoryPin+2, kd);//  Serial.print("kd: "); Serial.print(kd); Serial.print(" ");
  kpb=KP(1); EEPROM.write(memoryPin+3, kpb);//  Serial.print("kpb: "); Serial.print(kpb); Serial.print(" ");
  kib=KI(1); EEPROM.write(memoryPin+4, ki);  //   Serial.print("kib: "); Serial.print(kib); Serial.print(" ");
  kdb=KD(1); EEPROM.write(memoryPin+5, kd); // Serial.print("kdb: "); Serial.print(kdb); Serial.print(" ");
  kpw=KP(2); EEPROM.write(memoryPin+6, kpw);//  Serial.print("kpw: "); Serial.print(kpw); Serial.print(" ");
  kiw=KI(2); EEPROM.write(memoryPin+7, ki); //   Serial.print("kiw: "); Serial.print(kiw); Serial.print(" ");
  kdw=KD(2); EEPROM.write(memoryPin+8, kd);//  Serial.print("kdw: "); Serial.print(kdw); Serial.print(" ");
  leap=Leap(); EEPROM.write(memoryPin+9, leap);//  Serial.print("leap: "); Serial.print(leap); Serial.print(" ");

  speedD=initialSpeed(0); EEPROM.write(memoryPin+10, speedD);
  speedB=initialSpeed(1); EEPROM.write(memoryPin+11, speedB);
  speedW=initialSpeed(2); EEPROM.write(memoryPin+12, speedW);
  Serial.println(" ");
 // }

    readThings();
    if(digitalRead(setPin)==HIGH){
      setThings();
    }

}

void readThings(){
  int i,j;
  Serial.println("reading DATA....");
  kp=float(EEPROM.read(memoryPin+0))/pRatio;  Serial.print("kp: "); Serial.print(kp); Serial.print("    ");
  ki=float(EEPROM.read(memoryPin+1))/iRatio;      Serial.print("ki: "); Serial.print(ki); Serial.print("    ");
  kd=float(EEPROM.read(memoryPin+2))/dRatio;   Serial.print("kd: "); Serial.print(kd); Serial.println("    ");
  kpb=float(EEPROM.read(memoryPin+3))/pRatio; Serial.print("kpb: "); Serial.print(kpb); Serial.print("    ");
  kib=float(EEPROM.read(memoryPin+4))/iRatio;     Serial.print("kib: "); Serial.print(kib); Serial.print("    ");
  kdb=float(EEPROM.read(memoryPin+5))/dRatio;  Serial.print("kdb: "); Serial.print(kdb); Serial.println("    ");
  kpw=float(EEPROM.read(memoryPin+6))/pRatio; Serial.print("kpw: "); Serial.print(kpw); Serial.print("    ");
  kiw=float(EEPROM.read(memoryPin+7))/iRatio;     Serial.print("kiw: "); Serial.print(kiw); Serial.print("    ");
  kdw=float(EEPROM.read(memoryPin+8))/dRatio;  Serial.print("kdw: "); Serial.print(kdw); Serial.println("    ");
  speedD=EEPROM.read(memoryPin+10);        Serial.print("speedD: "); Serial.print(speedD); Serial.print("    ");
  speedB=EEPROM.read(memoryPin+11);        Serial.print("speedB: "); Serial.print(speedB); Serial.print("    ");
  speedW=EEPROM.read(memoryPin+12);        Serial.print("speedW: "); Serial.print(speedW); Serial.println("    ");
  leap=EEPROM.read(memoryPin+9);           Serial.print("leap: "); Serial.print(leap); Serial.println("   ");


  if(digitalRead(setPin)==HIGH){ 
  //lcd.clear();
  float kTotal=(kp+ki+kd);
  int kpp, kip, kdp;  
  kpp=int(kp*100/kTotal);
  kip=int(ki*100/kTotal);
  kdp=int(kd*100/kTotal);
  
  lcd.setCursor(0, 0); lcd.print(kpp); lcdprint(3, kdp);
  lcd.setCursor(6, 0); lcd.print(kip); lcdprint(3, kip);
  lcd.setCursor(12, 0); lcd.print(kdp); lcdprint(3, kdp);
  lcd.setCursor(0, 1); lcd.print(speedD); 
  lcd.setCursor(4, 1); lcd.print(speedB);
  lcd.setCursor(8, 1); lcd.print(speedW);
  lcd.setCursor(12, 1); lcd.print(leap); lcdprint(3, leap);
  delay(300);
  }
  

}


float KP(int x){
  float val;
  
  if(x==0){
  val=analogRead(pidPin+0);
  
//  Serial.print("   ");
//  Serial.print(val);
//  Serial.print("---");
  val=map(val, 0, 1023, 0, 100);
  //val=255-val;
  return val;
  }

  if(x==1){
  val=analogRead(pidPin+0);
//  Serial.print("   ");
//  Serial.print(val);
//  Serial.print("---");
  val=map(val, 0, 1023, 0, 100);
  //val=255-val;
  return val;
  }

  if(x==2){
  val=analogRead(pidPin+0);
//  Serial.print("   ");
//  Serial.print(val);
//  Serial.print("---");
  val=map(val, 0, 1023, 0, 100);
  //val=255-val;
  return val;
  }

  
}

float KI(int x){
  float val;
  
  if(x==0){
  val=analogRead(pidPin+1);
//  Serial.print("   ");
//  Serial.print(val);
//  Serial.print("---");
  val=map(val, 0, 1023, 0, 100);
  return val;
  }

 if(x==1){
  val=analogRead(pidPin+1);
//  Serial.print("   ");
//  Serial.print(val);
//  Serial.print("---");
  val=map(val, 0, 1023, 0, 100);
  return val;
  }

  if(x==2){
  val=analogRead(pidPin+1);
//  Serial.print("   ");
//  Serial.print(val);
//  Serial.print("---");
  val=map(val, 0, 1023, 0, 100);
  return val;
  }
  
}

float KD(int x){
  float val;
  
  if(x==0){
  val=analogRead(pidPin+2);
//  Serial.print("   ");
//  Serial.print(val);
//  Serial.print("---");
  val=map(val, 0, 1023, 0, 100);
  return val;
  }

   if(x==1){
  val=analogRead(pidPin+2);
//  Serial.print("   ");
//  Serial.print(val);
//  Serial.print("---");
  val=map(val, 0, 1023, 0, 100);
  return val;
  }

   if(x==2){
  val=analogRead(pidPin+2);
//  Serial.print("   ");
//  Serial.print(val);
//  Serial.print("---");
  val=map(val, 0, 1023, 0, 100);
  return val;
  }
  
  
}

float initialSpeed(int x){
  int i,j;
  float val;
  
  if(x==0){
  val=analogRead(pidPin+4);
//  Serial.print("   ");
//  Serial.print(val);
//  Serial.print("---");
  val=map(val, 0, 1023, 0, 255);
  return val;
  }

  if(x==1){
  val=analogRead(pidPin+4);
//  Serial.print("   ");
//  Serial.print(val);
//  Serial.print("---");
  val=map(val, 0, 1023, 0, 255);
  return val;
  }

  if(x==2){
  val=analogRead(pidPin+4);
//  Serial.print("   ");
//  Serial.print(val);
//  Serial.print("---");
  val=map(val, 0, 1023, 0, 255);
  return val;
  }
  
}

float Leap(){
  float val;
  
  val=analogRead(pidPin+3);
//  Serial.print("   ");
//  Serial.print(val);
//  Serial.print("---");
  val=map(val, 0, 1023, 0, 100);
  return val;
}

float romW(int add, int val){
  EEPROM.write(add, val);
   
}

float romR(int add){
  float val;
  val=EEPROM.read(add);
  return  val;
}

int errorW(signed int x){
  int a = avg();
  x = map(x, -1*a, a, 0, a*2);
  romW(errorPin, x);
  
}

signed int errorR(){
   int a = avg();
  signed int e = romR(errorPin);
  e= map(e,  0, a*2, -1*a, a);
  
  return e;
}

int lastErrorW(signed int x){
  int a = avg();
  x = map(x, -1*a, a, 0, a*2);
  romW(lastErrorPin, x);
  
}

signed int lastErrorR(){
   int a = avg();
  signed int e = romR(lastErrorPin);
  e= map(e,  0, a*2, -1*a, a);
  
  return e;
}

int pidW(signed int x){
  int a=255;                    Serial.print("                        ");//Serial.print(" got: "); Serial.print(x);   
  x = map(x, -255, 255, 0, 255); //Serial.print(" write: "); Serial.print(x);  Serial.print(" "); 
  romW(pidPin, x);
  
}

signed int pidR(){
  int a=255;
  signed int e = romR(pidPin);  
  e= map(e,  0, 255, -255, 255); //Serial.print(" read: "); Serial.print(e);  Serial.print(" "); 
  
  return e;
}






double IRread(int x){

  int i, j=0, k, f=0, c=0;
  double n=0, np;
  int sensor[sn];
  signed int pos;
  

  for(i=0, j=sn-1; i<sn; i++, j--){
    digitalWrite(tp,HIGH);
  k=analogRead(A0+i); 
  if(k>500){
    k=0;
    //Serial.print("  ");
   // romW(senMem+i, k);
 
  }
  else{
    k=1;  
    f++;
    digitalWrite(ledPin+i, HIGH);
   // Serial.print("**");
    //romW(senMem+i, k);
  }
  sensor[i]=k;
  romW(senMem+i, k);
  n=val2int(n,k,j); 
  romW(senMem+sn, f);
  
  }
  

  
  int cn=0;  
  cn=checkSensorChange(sensor,prevSenMem+i);
  romW(changePin, cn);
  


  
  if(cn==1){
  for(i=0; i<=sn; i++){
    int senVal=int(romR(senMem+i));
    romW(prevSenMem+i, senVal);
    Serial.print(" ");
    if(i==sn){
      Serial.print("---");
    }
    Serial.print(senVal);
  }
  Serial.print("     ");
  }


  if(cn==1){
    Serial.print("[");
    
  for(i=0; i<sn; i++){
    int senVal=int(romR(senMem+i));
    romW(prevSenMem+i, senVal);
    
    if(senVal==1){
    Serial.print("*");
    }
    else if(senVal==0){
    Serial.print(" ");
    }
  }
  
  Serial.print("]   ");
  //Serial.println(Pos()-avg());
  }


   if(cn==0){
    
   // follow();
   }

  errorW((Pos()-avg()));
  return n;

}

double val2int(double n,int k, int j){
  n=n+k*pow(10,j);
  return n;
}

int checkChange(int i,int add){

  if(i==romR(add)){
    return 1;
  }

  else{
    return 0;
  }
}

int checkSensorChange(int sensor[sn],int add){
  int cn=0,c=0, i, j, k; 
  for(i=0; i<sn; i++){
    //Serial.print(sensor[i]);
  c=checkChange(sensor[i],prevSenMem+i);
  if(c==1){
    cn=cn+c;
  } 
  else{
    cn--;
  }  
  romW(prevSenMem+i, sensor[i]);
  }



if(cn==sn){
  return 0;
}

else{
  return 1;
}
  
}

signed int Pos(){
  n=0;
  //Serial.print("  ");
  for (i=0; i<sn; i++) {
    sensor[i]=int(romR(senMem+i));
    //Serial.print(sensor[i]);
    n=n+sensor[i]*10*(i+1);
    
  }
  n=n/int(romR(senMem+sn));
 // Serial.print("   n : "); Serial.print(n);
  
  if(n==-1||n==0){
    return avg();
  }

  else{
  return n;
  }
  
}

int avg(){
  n=0;
  for(i=1; i<=sn; i++){
    n=n+i;
  }
  n=(n*multiple)/sn;
  //Serial.print("  avg : ");
  //Serial.print(n);
  //Serial.print("   ");
  return n;
}

char Shape(){
  float r;
int  l0=0, l1=0, r0=0, r1=0, m=0, t=0, z=0;
  e=errorR();
  r=e/sn;

  int m1, m2;
    
    if(sn%2==0){
      m1=sn/2;
      m2=m1-1;
    }

    if(sn%2==1){
      m1=sn/2;
      m2=m1;
    }

  
 for(i=0; i<sn; i++){
    sensor[i]=int(romR(senMem+i));
   if(sensor[i]==0){
    z++;
   }
  }
 
if((r>=.5 && r<=((sn-1)/2)) && (sensor[0]==0) && (sensor[sn-1]==0) || t==3){
    return 'R';
  }

else if((r<=-.5 && r>=(-1*(sn-1)/2)) && (sensor[0]==0) && (sensor[sn-1]==0) || t==3){
    return 'L';
  }


if(sensor[sn-1]==1 && sensor[0]==1){

      for(i=0; i<sn; i++){
        if(sensor[i]==1){
          t++;
        }
        else{
          t=3;
        }
      }
      
}

if(sensor[0]==1 && sensor[sn-1]!=1 ){

  for(i=0; i<=((sn-1)/2); i++){
    if(sensor[i]==1){
      l1++;
    }

    if(sensor[i]==0){
      l0++;
    }
  }


  for(i=((sn/2)+1); i<sn; i++){
    if(sensor[i]==1){
      r1++;
    }

    if(sensor[i]==0){
      r0++;
    }
  }
 
  if( ((l1-r1)==l1)) {
    return '<';
  }

  t=1;
}

if(sensor[sn-1]==1  && sensor[0]!=1 ){

  for(i=0; i<=(((sn-1)/2)-1); i++){
    if(sensor[i]==1){
      l1++;
    }

    if(sensor[i]==0){
      l0++;
    }
  }


  for(i=((sn-1)/2); i<sn; i++){
    if(sensor[i]==1){
      r1++;
    }

    if(sensor[i]==0){
      r0++;
    }
  }
 t=1;
 
   if( ((r1-l1)==r1) ){
    return '>';
  }

  t=1;
}


  if(t==sn){
    return 'T';
  }

    else if((sensor[m1]==1 || sensor[m2]==1) && (sensor[m1+1]!=1 && sensor[m1+2]!=1  && sensor[m1+3]!=1 && sensor[m2-1]!=1 && sensor[m2-2]!=1 && sensor[m2-3]!=1) ){
      
      return '.';
    }

    if( (sensor[m1]!=1 && sensor[m2]!=1 && z!=5) && ((sensor[m1+1]==sensor[m2-1]) || (sensor[m1+2]==sensor[m2-2]))){
      return 'V';
    }

     if(z==5){
      return '0';
      }

      else if(((sensor[m1]==1) && (sensor[m1-1]==1) && (sensor[m1+1]==1) && (sensor[0]!=1) && (sensor[sn-1]!=1)) ||  ((sensor[m2]==1) &&  (sensor[m2-1]==1) && (sensor[m2+1]==1) && (sensor[0]!=1) && (sensor[sn-1]!=1)) ){
        return 'I';
      }

      else if(((sensor[m1]==1) || (sensor[m2]==1)) && (sensor[m1+2]==1) && (sensor[m1+1]!=1) && (sensor[0]!=1) ){
        return '}';
      }      

      else if(((sensor[m2]==1) || (sensor[m1]==1)) && (sensor[m2-2]==1) && (sensor[m2-1]!=1) && (sensor[sn-1]!=1) ){
        return '{';
      }

      else{
        return '-';
      }

      

   
  
}

signed int bal(signed int x){
  if(x>=speedMax){
    return speedMax;
  }

  if(x<=(-1*speedMax)){
    return -1*speedMax;
  }

  
}

signed PIDcal(signed int error){
  int prop, intg, derv;             
  //readThings();                    
                                   // Serial.print("err : "); Serial.print(error); Serial.print("  ");
  lastError=(lastErrorR());          // Serial.print("Lsterr : "); Serial.print(lastError); Serial.print("  ");
  prop=(error);                      // Serial.print("p : "); Serial.print(prop); Serial.print(", "); Serial.print(kp); Serial.print("     ");
  intg=(error+lastError);        //   Serial.print("i : "); Serial.print(intg); Serial.print(", "); Serial.print(ki); Serial.print("     ");
  derv=(error-lastError);        //   Serial.print("d : "); Serial.print(derv); Serial.print(", "); Serial.print(kd); Serial.print("     ");

  
  Pid=float((prop*kp)+(derv*kd)+(intg*ki));
  
 // Serial.print("pidErrorN : "); Serial.print(Pid); Serial.print("     ");// Serial.print("     ");
  pidW(Pid);
  Serial.print("pid: "); Serial.print(pidR()); lcd.print(pidR()); Serial.print("    "); Serial.print("      ");
  
  signed int l = lmFix(Pid);
  signed int r = rmFix(Pid);
  setMotorSpeed(l, r);
 // aroundTheWorld();
  
  
  Serial.println("     ");

}



                     


signed int lmFix(signed int x){
   x = (lmSpeed+x);
   pwmW(1, x);
    Serial.print(pwmR(1)); Serial.print("     "); Serial.print("     "); 
   return x;
}

signed int rmFix(signed int x){
   x = (rmSpeed-x);
   pwmW(2, x);
   Serial.print(pwmR(2)); Serial.print("     "); Serial.print("     ");
   return x;
}

signed int pwmW(int c, signed int val){
  if(val>=0 && c==1){
    romW(lSign, 100);
    romW(lmPWM, val);
  }

  if(val>=0 && c==2){
    romW(rSign, 100);
    romW(rmPWM, val);
  }

  if(val<0 && c==1){
    romW(lSign, 111);
    romW(lmPWM, (val*(-1)));
  }

  if(val<0 && c==2){
    romW(rSign, 111);
    romW(rmPWM, (val*(-1)));
  }
  
}

signed int pwmR(int c){
  if(c==1 && romR(lSign)==100){
    Serial.print("  lm : ");
    Serial.print((romR(lmPWM)*(1)));
    Serial.print(" ");
    return (romR(lmPWM)*(1));
    
  }

  if(c==1 && romR(lSign)==111){
    Serial.print("  lm : ");
    Serial.print((romR(lmPWM)*(-1)));
    Serial.print(" ");
    return (romR(lmPWM)*(-1));
  }

  if(c==2 && romR(rSign)==100){
    Serial.print("  rm : ");
    Serial.print((romR(rmPWM)*(1)));
    Serial.print(" ");
    return (romR(rmPWM)*(1));
  }

  if(c==2 && romR(rSign)==111){
    Serial.print("  rm : ");
    Serial.print((romR(rmPWM)*(-1)));
    Serial.print(" ");
    return (romR(rmPWM)*(-1));
  }
}

void setMotorSpeed(signed int l, signed int r){
    Serial.print(" "); Serial.print(l); Serial.print(", ");  Serial.print(r); Serial.print("    ");
    lcd.setCursor(6,1); lcd.print(l); lcd.setCursor(12,1); lcd.print(r); 
    
    if(l<0){
      digitalWrite(leftMotor2, LOW);  digitalWrite(leftMotor1, HIGH); analogWrite(lmPWM, (l*-1));   
    }

     if(r<0){
      digitalWrite(rightMotor2, LOW);  digitalWrite(rightMotor1, HIGH); analogWrite(rmPWM, (r*-1));   
    }

     if(l>=0){
      digitalWrite(leftMotor1, LOW);  digitalWrite(leftMotor2, HIGH); analogWrite(lmPWM, (l*1));   
    }

     if(r>=0){
      digitalWrite(rightMotor1, LOW);  digitalWrite(rightMotor2, HIGH); analogWrite(lmPWM, (r*1));   
    }


      
}






void movement(char shape){dv 
        if(shape=='I' || shape=='.' || shape=='-'){
    //brake();
    delay(int(Leap()));
    straight();
  }




        if (shape=='<'){
    brake();
    delay(int(Leap()));
    char L=LOOP();
    
      if (L == '<'){
        brake();
        delay(int(Leap()));
        char L2=LOOP();
            if(L2  == '0'){
              turnLF();
            }
            if(L2  == '<'){
              turnLH();
            }
        
      }

      if (L == '0'){
        brake();
        delay(int(Leap()));
        char L2=LOOP();
            if(L2  == '0'){
              turnLB();
            }
            if(L2  == 'H'){
              turnLF();
            }
      }

      if (L  == 'R'){
        brake();
    delay(int(Leap()));
        turnRH();
      }

      if (L  == 'L'){
        brake();
    delay(int(Leap()));
        char L2=LOOP();
            if(L2  == '0' || L2  == '.' || L  == '-' ){
              turnLB();
            }
      
      }

      if (L  == '-' || L  == '.' ){
        brake();
    delay(int(Leap()));
        char L2=LOOP();
        if(L2  == '0'){
              turnLF();
            }
            if(L2  == '>'){
              turnRH();
            }
      }
      }
 



        if (shape=='>'){
    brake();
    delay(int(Leap()));
    char L=LOOP();
    
      if (L == '>'){
        brake();
        delay(int(Leap()));
        char L2=LOOP();
            if(L2  == '0'){
              turnRF();
            }
            if(L2  == '>'){
              turnRH();
            }
        
      }

      if (L == '0'){
        brake();
        delay(int(Leap()));
        char L2=LOOP();
            if(L2  == '0'){
              turnLB();
            }
            if(L2  == '>'){
              turnRF();
            }
      }

      if (L  == 'L'){
        brake();
    delay(int(Leap()));
        turnLH();
      }

      if (L  == 'R'){
        brake();
    delay(int(Leap()));
        char L2=LOOP();
            if(L2  == '0' || L2  == '.' || L  == '-' ){
              turnRB();
            }
      
      }

      if (L  == '-' || L  == '.' ){
        brake();
    delay(int(Leap()));
        char L2=LOOP();
        if(L2  == '0'){
              turnLF();
            }
            if(L2  == '<'){
              turnLH();
            }
      }
      }
  
  
  

         if (shape=='L'){
    brake();
    delay(int(Leap()));
    char L=LOOP();
    
      if (L == '<'){
        brake();
        delay(int(Leap()));
        char L2=LOOP();
            if(L2  == '0'){
              turnLH();
            }
            if(L2  == '<'){
              turnLH();
            }
        
      }

      if (L == '0'){
        brake();
        delay(int(Leap()));
        char L2=LOOP();
            if(L2  == '0'){
              turnLB();
            }
            if(L2  == 'H'){
              turnLF();
            }
      }

      if (L  == 'R'){
        brake();
    delay(int(Leap()));
        turnRR();
      }

      if (L  == 'L'){
        brake();
    delay(int(Leap()));
        char L2=LOOP();
            if(L2  == '0' || L2  == '.' || L  == '-' ){
              turnLH();
            }
      
      }

/*      if (L  == '-' || L  == '.' ){
//        brake();
//    delay(int(Leap()));
//        char L2=LOOP();
//        if(L2  == '0'){
//              turnLF();
//            }
//            if(L2  == '>'){
//              turnRH();
//            }
      } */
      }
  




          if (shape=='R'){
    brake();
    delay(int(Leap()));
    char L=LOOP();
    
      if (L == '>'){
        brake();
        delay(int(Leap()));
        char L2=LOOP();
            if(L2  == '0'){
              turnRH();
            }
            if(L2  == '>'){
              turnRH();
            }
        
      }

      if (L == '0'){
        brake();
        delay(int(Leap()));
        char L2=LOOP();
            if(L2  == '0'){
              turnRF();
            }
            if(L2  == 'R'){
              turnRH();
            }
      }

      if (L  == 'L'){
        brake();
    delay(int(Leap()));
        turnLR();
      }

      if (L  == 'R'){
        brake();
    delay(int(Leap()));
        char L2=LOOP();
            if(L2  == '0' || L2  == '.' || L  == '-' ){
              turnRH();
            }
      
      }

/*      if (L  == '-' || L  == '.' ){
//        brake();
//    delay(int(Leap()));
//        char L2=LOOP();
//        if(L2  == '0'){
//              turnLF();
//            }
//            if(L2  == '>'){
//              turnRH();
//            }
//      }*/
     
      }
   
  
  
  
  }

void turnRH(){
  
}

void turnRF(){
  
}

void turnRR(){
  
}

void turnRB(){
  
}

void turnLH(){
  
}

void turnLF(){
  
}

void turnLR(){
  
}

void turnLB(){
  
}

void follow(){
  
}

void brake(){
  
}

void straight(){
  
}

void back(){
  
}

void aroundTheWorld(){
  int l=(255-leap*2);int  r=(leap*2-255);
  Serial.print(" "); Serial.print(l); Serial.print(", ");  Serial.print(r); Serial.print("    ");  
    setMotorSpeed(l,r);  
  
}

