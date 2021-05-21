//Connor Wojtak
//5-07-2021
//visual_binary_adder
//Uses LEDs to represent binary numbers and allows for addition.

#define ledPinOne 8
#define ledPinTwo 9
#define ledPinThree 10
#define ledPinFour 11
#define ledPinOverflow 5
#define buttonPin 13

void setup() {
  Serial.begin(9600);
  pinMode(ledPinOne, OUTPUT);
  pinMode(ledPinTwo, OUTPUT);
  pinMode(ledPinThree, OUTPUT);
  pinMode(ledPinFour, OUTPUT);
  pinMode(ledPinOverflow, OUTPUT);
  pinMode(buttonPin, INPUT);

  int number = 0;
  while(number < 17){
    updateLEDS(number);
    delay(1000);
    number++;
  }
}

void loop() {
  
}

void updateLEDS(int num){
  digitalWrite(ledPinFour, LOW);
  digitalWrite(ledPinThree, LOW);
  digitalWrite(ledPinTwo, LOW);
  digitalWrite(ledPinOne, LOW);
  int baseSize = log2(num);
  if(baseSize >= 4){
    digitalWrite(ledPinOverflow, HIGH);
    return;
  }
  for(int i = baseSize; i > -1; i--){
    if(num - pow(2, i) > -1){
      lightLED(i);
      num = num - pow(2, i);
    }
  }
}

int log2(int num) {
  return log(num) / log(2);
}

void lightLED(int num){
  if(num == 3){
    digitalWrite(ledPinFour, HIGH); 
    return;
  }
  if(num == 2){
    digitalWrite(ledPinThree, HIGH); return;
  }
  if(num == 1){
    digitalWrite(ledPinTwo, HIGH); return;
  }
  if(num == 0){
    digitalWrite(ledPinOne, HIGH); return;
  }
}
