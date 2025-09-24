int PWMA=5;//Right side 
int PWMB=4;//Left side 
int DA=0;//Right reverse 
int DB=2;//Left reverse 

void setup(){ 
 // Debug console 
 Serial.begin(9600); 
 pinMode(PWMA, OUTPUT); 
 pinMode(PWMB, OUTPUT); 
 pinMode(DA, OUTPUT); 
 pinMode(DB, OUTPUT); 
} 

void loop(){ 
  digitalWrite(PWMA, HIGH); 
  digitalWrite(DA, LOW); 

  digitalWrite(PWMB, HIGH); 
  digitalWrite(DB, HIGH); 
  delay(5000);

  digitalWrite(PWMA, HIGH); 
  digitalWrite(DA, HIGH); 
  
  digitalWrite(PWMB, HIGH); 
  digitalWrite(DB, LOW); 
  delay(5000);
  
} 

/*
int x = 0;
int y = 1;
if(x==-1 && y==-1){             //Backward and Left     
     digitalWrite(PWMA, LOW); 
     digitalWrite(DA, LOW); 
     
     digitalWrite(PWMB, HIGH); 
     digitalWrite(DB, HIGH); 
}else if(x==-1 && y==0){        //Left Turn 
     digitalWrite(PWMA, 450); 
     digitalWrite(DA, HIGH); 
    
     digitalWrite(PWMB, 450); 
     digitalWrite(DB, LOW); 
}else if(x==-1 && y==1){        //Forward and Left     
     digitalWrite(PWMA, LOW); 
     digitalWrite(DA, LOW); 
    
     digitalWrite(PWMB, HIGH); 
     digitalWrite(DB, LOW); 
}else if(x==0 && y==-1){        //Backward     
     digitalWrite(PWMA, HIGH); 
     digitalWrite(DA, HIGH); 
     
     digitalWrite(PWMB, HIGH); 
     digitalWrite(DB, HIGH);
}else if(x==0 && y==0){        //Stay 
     digitalWrite(PWMA, LOW); 
     digitalWrite(DA, LOW); 
     
     digitalWrite(PWMB, LOW); 
     digitalWrite(DB, LOW); 
}else if(x==0 && y==1){        //Forward 
     digitalWrite(PWMA, HIGH); 
     digitalWrite(DA, LOW); 
    
     digitalWrite(PWMB, HIGH); 
     digitalWrite(DB, LOW); 
}else if(x==1 && y==-1){        //Backward and Right     
     digitalWrite(PWMA, HIGH); 
     digitalWrite(DA, HIGH); 
     
     digitalWrite(PWMB, LOW); 
     digitalWrite(DB, LOW); 
}else if(x==1 && y==0){        //Right turn 
     digitalWrite(PWMA, 450); 
     digitalWrite(DA, LOW); 
     
     digitalWrite(PWMB, 450); 
     digitalWrite(DB, HIGH); 
}else if(x==1 && y==1){        //Forward and Right 
     digitalWrite(PWMA, HIGH); 
     digitalWrite(DA, LOW); 
     
     digitalWrite(PWMB, LOW); 
     digitalWrite(DB, LOW);
 } 
 */
