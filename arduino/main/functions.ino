void Setup_pins() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
  T0 = 25 + 273.15;
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //max 30102 init
  particleSensor.begin(Wire, I2C_SPEED_FAST);
  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red on
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED

}


unsigned long lastTime = 0;
unsigned long timerDelay = 10000; //10 sec

float enable_sensor() {
  lastTime = millis();

  while (true) {
    long irValue = particleSensor.getIR();

    if (checkForBeat(irValue) == true){
      //sensed a beat
      long delta = millis() - lastBeat;
      lastBeat = millis();

      beatsPerMinute = 60 / (delta / 1000.0);

      if (beatsPerMinute < 255 && beatsPerMinute > 20)
      {
        rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
        rateSpot %= RATE_SIZE; //Wrap variable

        //Take average of readings
        beatAvg = 0;
        for (byte x = 0 ; x < RATE_SIZE ; x++)
          beatAvg += rates[x];
        beatAvg /= RATE_SIZE;
      }
    }

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
