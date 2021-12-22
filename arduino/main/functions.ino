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
  
  byte ledBrightness = 60; //Options: 0=Off to 255=50mA
  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  byte sampleRate = 100; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings


}


unsigned long lastTime = 0;
unsigned long timerDelay = 10000; //10 sec

float enable_sensor() {
  lastTime = millis();

  while (true) {
    bufferLength = 100; //buffer length of 100 stores 4 seconds of samples running at 25sps

    //read the first 100 samples, and determine the signal range
    for (byte i = 0 ; i < bufferLength ; i++){
      while (particleSensor.available() == false) //do we have new data?
        particleSensor.check(); //Check the sensor for new data

      redBuffer[i] = particleSensor.getRed();
      irBuffer[i] = particleSensor.getIR();
      particleSensor.nextSample(); //We're finished with this sample so move to next sample
    }

    //calculate heart rate and SpO2 after first 100 samples (first 4 seconds of samples)
    maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);

    //Continuously taking samples from MAX30102.  Heart rate and SpO2 are calculated every 1 second
    while (1)
    {
      //dumping the first 25 sets of samples in the memory and shift the last 75 sets of samples to the top
      for (byte i = 25; i < 100; i++)
      {
        redBuffer[i - 25] = redBuffer[i];
        irBuffer[i - 25] = irBuffer[i];
      }

      //take 25 sets of samples before calculating the heart rate.
      for (byte i = 75; i < 100; i++)
      {
        while (particleSensor.available() == false) //do we have new data?
          particleSensor.check(); //Check the sensor for new data

        digitalWrite(readLED, !digitalRead(readLED)); //Blink onboard LED with every data read

        redBuffer[i] = particleSensor.getRed();
        irBuffer[i] = particleSensor.getIR();
        particleSensor.nextSample(); //We're finished with this sample so move to next sample

        //send samples and calculation result to terminal program through UART
        Serial.print(F("red="));
        Serial.print(redBuffer[i], DEC);
        Serial.print(F(", ir="));
        Serial.print(irBuffer[i], DEC);

        Serial.print(F(", HR="));
        Serial.print(heartRate, DEC);

        Serial.print(F(", HRvalid="));
        Serial.print(validHeartRate, DEC);

        Serial.print(F(", SPO2="));
        Serial.print(spo2, DEC);

        Serial.print(F(", SPO2Valid="));
        Serial.println(validSPO2, DEC);
      }

      //After gathering 25 new samples recalculate HR and SP02
      maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);

    if((millis() - lastTime) > timerDelay){
      break;
    }
  }
  return beatAvg;
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
