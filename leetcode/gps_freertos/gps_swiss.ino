#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

#define QUEUE_LENGTH 5
QueueHandle_t queue_handle = NULL;

struct Button {
    const uint8_t PIN;
    uint32_t numberKeyPresses;
    bool pressed;
};
Button button2 = {18, 0, false};


char val[10] = {0};
void IRAM_ATTR isr() {
    button2.numberKeyPresses += 1;
    button2.pressed = true;
    val[2] = val[1] + 1;
    
//    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
//    if (xQueueSendFromISR(queue_handle, &val, &xHigherPriorityTaskWoken)!= pdTRUE) {
//        // do nothing
//    }
}

// define two tasks for Blink & AnalogRead
void TaskBlink( void *pvParameters );
void TaskAnalogReadA3( void *pvParameters );


// the setup function runs once when you press reset or power the board
void setup() {

  queue_handle = xQueueCreate(QUEUE_LENGTH, sizeof(int));
  
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);

  pinMode(button2.PIN, INPUT_PULLUP);
  attachInterrupt(button2.PIN, isr, FALLING);
  
  // Now set up two tasks to run independently.
  xTaskCreatePinnedToCore(
    TaskBlink
    ,  "TaskBlink"
    ,  1024
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
    TaskAnalogReadA3
    ,  "AnalogReadA3"
    ,  1024  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);



  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop()
{
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskBlink(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
    
  If you want to know what pin the on-board LED is connected to on your ESP32 model, check
  the Technical Specs of your board.
*/

  // initialize digital LED_BUILTIN on pin 13 as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  for (;;) // A Task shall never return or exit.
  {
    static int value_local = 0;
    if (xQueueReceive(queue_handle, &value_local, 5000) != pdTRUE) {
      Serial.println("Press something!");
    } else {
      Serial.print("value_local: ");
      Serial.print(value_local);
      Serial.println();
    }
    Serial.print("num presses: ");
    Serial.print(button2.numberKeyPresses);
    Serial.println();
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    vTaskDelay(100);  // one tick delay (15ms) in between reads for stability
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    vTaskDelay(100);  // one tick delay (15ms) in between reads for stability
  }
}

void TaskAnalogReadA3(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  
/*
  AnalogReadSerial
  Reads an analog input on pin A3, prints the result to the serial monitor.
  Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
  Attach the center pin of a potentiometer to pin A3, and the outside pins to +5V and ground.

  This example code is in the public domain.
*/

  for (;;)
  {
    // read the input on analog pin A3:
    int sensorValueA3 = analogRead(A3);
    // print out the value you read:
    Serial.println(sensorValueA3);
//    static int value = 10.0;
//    value = value + 1;    
//    if (xQueueSend(queue_handle, &value, portMAX_DELAY)!= pdTRUE) {
//      Serial.println("queue is full..");
//    }
    vTaskDelay(1000);  // one tick delay (15ms) in between reads for stability
  }
}
