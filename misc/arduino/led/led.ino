#define REDPIN 5
#define GREENPIN 6
#define BLUEPIN 3
  
#define FADESPEED 20     // чем выше число, тем медленнее будет fade-эффект
  
void setup() {
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
}
  
  
void loop() {
  int r, g, b,i;
  
  // fade от голубого к фиолетовому
  for (r = 0; r < 256; r++) { 
    analogWrite(REDPIN, r);
    delay(FADESPEED);
  } 
  // fade от фиолетового к красному
  for (b = 255; b > 0; b--) { 
    analogWrite(BLUEPIN, b);
    delay(FADESPEED);
  } 
  // fade от красного к желтому
  for (g = 0; g < 256; g++) { 
    analogWrite(GREENPIN, g);
    delay(FADESPEED);
  } 
  // fade от желтого к зеленому
  for (r = 255; r > 0; r--) { 
    analogWrite(REDPIN, r);
    delay(FADESPEED);
  } 
  // fade от зеленого к зеленовато-голубому
  for (b = 0; b < 256; b++) { 
    analogWrite(BLUEPIN, b);
    delay(FADESPEED);
  } 
  // fade от зеленовато-голубого к голубому
  for (g = 255; g > 0; g--) { 
    analogWrite(GREENPIN, g);
    delay(FADESPEED);
  } 
  
   for (i = 255; i > 0; i--) { 
     analogWrite(REDPIN, i);
    analogWrite(GREENPIN, i);
    analogWrite(BLUEPIN, i);
    delay(FADESPEED);
  } 
}
