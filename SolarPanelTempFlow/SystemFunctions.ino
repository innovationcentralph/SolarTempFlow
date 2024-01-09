float readSurfaceTemp(){
  
  surfaceTemp.requestTemperatures(); // Send the command to get temperatures
  float tempC = surfaceTemp.getTempCByIndex(0);

  // Check if reading was successful
  if(tempC != DEVICE_DISCONNECTED_C) 
  {
    return tempC;
  } 
  else
  {
    Serial.println("Failed to read from DSB sensor!");
    return 0;
  }
}


float readAmbientTemp(){
  float t = ambientTemp.readTemperature();

  if (isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return 0;
  }

  else{
    return t;
  }
}

void Flow()
{
   count++; //Every time this function is called, increment "count" by 1
}

void speedControl() {
  
  // Accelerate from zero to maximum speed
  for (int i = 0; i < 256; i++) {
    analogWrite(MOTOR_CONTROL_PIN, i);
    Serial.println("PWM: " + String(i));
    delay(20);
  }
  
  
}

float getFlowRate(){
    count = 0;      // Reset the counter so we start counting from 0 again
    //interrupts();   //Enables interrupts on the Arduino
    delay (1000);   //Wait 1 second
    //noInterrupts(); //Disable the interrupts on the Arduino
  
    //Start the math
    flowRate = (count * CAL_FACTOR);        //Take counted pulses in the last second and multiply by 2.25mL
    flowRate = flowRate * 60;         //Convert seconds to minutes, giving you mL / Minute
    flowRate = flowRate / 1000;       //Convert mL to Liters, giving you Liters / Minute
  
    Serial.println(flowRate);         //Print the variable flowRate to Serial

    return flowRate;
}
