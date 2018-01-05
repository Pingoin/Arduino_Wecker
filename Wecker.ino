int klingel =3;
int aus=2;
int kick=42;

//Laufzeitvariablen
bool wecken=0;
bool blinken=0;




void setup() {
pinMode(klingel,OUTPUT);
pinMode(aus,INPUT);
pinMode(kick,INPUT);
}

void loop() {  
if (digitalRead(kick)==HIGH){
  wecken=1;
}
if (digitalRead(aus)==HIGH){
  wecken=0;
}
if (wecken){
  if (blinken) {
    digitalWrite(klingel,HIGH);
  }else{
    digitalWrite(klingel,LOW);    
  }
  blinken=!blinken;
}else{
  digitalWrite(klingel,LOW); 
}
delay(500);
}