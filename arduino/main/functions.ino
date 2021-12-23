void Setup_pins() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
  T0 = 25 + 273.15;
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //max 30102 init
   if (!particleSensor.begin(Wire, I2C_SPEED_FAST)){
     Serial.println(F("MAX30105 was not found. Please check wiring/power."));
     while (1);
   }
  
   byte ledBrightness = 60;
   byte sampleAverage = 4;
   byte ledMode = 2;
   byte sampleRate = 100; 
   int pulseWidth = 411; 
   int adcRange = 4096; 
   particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);
}

void send_data(String phone_number, String age, float temperature, float heart_rate, float spo2, bool has_cough, float lens_power) {
  Serial.print("[\"data\", \"" + phone_number + "\",\"" + age + "\",\"" + temperature + "\",\"" + heart_rate + "\",\"" + spo2 + "\",\"" + has_cough + "\",\"" + lens_power + "\"]\n");
}

void play_audio(String audio_filename) {
  Serial.print("[\"audio\", \"" + audio_filename + "\"]\n");
}
void print_report() {
  Serial.print("[\"print\"]\n");
}

 float enable_sensor() {
   bufferLength = 100;

   lcd.setCursor(0, 0);
   lcd.clear();
   lcd.print("Taking Samples");

   //first 100 samples
   for (byte i = 0 ; i < bufferLength ; i++){
     while (particleSensor.available() == false)
       particleSensor.check();

     redBuffer[i] = particleSensor.getRed();
     irBuffer[i] = particleSensor.getIR();
     particleSensor.nextSample();
   }

   lcd.clear();
   lcd.print("Now measuring");

   // first 100 samples (first 4 seconds of samples)
   maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
  
   while (1){
     for (byte i = 25; i < 100; i++){
       redBuffer[i - 25] = redBuffer[i];
       irBuffer[i - 25] = irBuffer[i];
     }

     //take 25 sets of samples before calculating the heart rate.
     for (byte i = 75; i < 100; i++){
       while (particleSensor.available() == false)
         particleSensor.check();

       redBuffer[i] = particleSensor.getRed();
       irBuffer[i] = particleSensor.getIR();
       particleSensor.nextSample();

       // Serial.print(F("red="));
       // Serial.print(redBuffer[i], DEC);
       // Serial.print(F(", ir="));
       // Serial.print(irBuffer[i], DEC);

       // Serial.print(F(", HR="));
       // Serial.print(heartRate, DEC);

       // Serial.print(F(", HRvalid="));
       // Serial.print(validHeartRate, DEC);

       // Serial.print(F(", SPO2="));
       // Serial.print(spo2, DEC);

       // Serial.print(F(", SPO2Valid="));
       // Serial.println(validSPO2, DEC);
     }
     //After gathering 25 new samples recalculate HR and SP02
     maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);

     if(validHeartRate == 1 && validSPO2 == 1){
       break;
     }
   }
 }
