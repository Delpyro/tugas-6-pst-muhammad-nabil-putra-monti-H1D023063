#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inisialisasi LCD pada alamat 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Definisi Pin Tombol
#define BTN_A 2
#define BTN_B 3
#define BTN_PAUSE 4

// Tipe data untuk dikirim ke Queue
enum TipeEvent { GOL_A, GOL_B, TOGGLE_PAUSE, TIDAK_ADA };

// Handle untuk antrean (Queue)
QueueHandle_t queueEvent;

// Variabel Global Pertandingan
int skorA = 0;
int skorB = 0;
bool isPaused = true;
int waktuSisa = 45 * 60; // 45 menit

// Prototipe Task
void TaskWasit(void *pvParameters);
void TaskDisplay(void *pvParameters);

void setup() {
  Serial.begin(9600);
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("T-A:0      T-B:0");
  lcd.setCursor(0, 1);
  lcd.print("WAKTU: PAUSED   ");

  pinMode(BTN_A, INPUT_PULLUP);
  pinMode(BTN_B, INPUT_PULLUP);
  pinMode(BTN_PAUSE, INPUT_PULLUP);

  // Membuat Queue untuk 5 antrean event
  queueEvent = xQueueCreate(5, sizeof(TipeEvent));

  if (queueEvent != NULL) {
    xTaskCreate(TaskWasit, "Task Wasit", 128, NULL, 1, NULL);
    xTaskCreate(TaskDisplay, "Task Display", 128, NULL, 2, NULL);
  }
}

void loop() {
  // Kosong, dikendalikan FreeRTOS
}

// --- TASK 1: Membaca Tombol (Wasit) ---
void TaskWasit(void *pvParameters) {
  bool lastA = HIGH, lastB = HIGH, lastPause = HIGH;
  TipeEvent eventYangDikirim = TIDAK_ADA;

  for (;;) {
    bool currA = digitalRead(BTN_A);
    bool currB = digitalRead(BTN_B);
    bool currPause = digitalRead(BTN_PAUSE);

    if (lastA == HIGH && currA == LOW) eventYangDikirim = GOL_A;
    else if (lastB == HIGH && currB == LOW) eventYangDikirim = GOL_B;
    else if (lastPause == HIGH && currPause == LOW) eventYangDikirim = TOGGLE_PAUSE;

    if (eventYangDikirim != TIDAK_ADA) {
      xQueueSend(queueEvent, &eventYangDikirim, 0);
      eventYangDikirim = TIDAK_ADA;
      vTaskDelay(250 / portTICK_PERIOD_MS); // Debounce
    }

    lastA = currA; lastB = currB; lastPause = currPause;
    vTaskDelay(50 / portTICK_PERIOD_MS); 
  }
}

// --- TASK 2: Countdown Timer & Update Layar ---
void TaskDisplay(void *pvParameters) {
  TipeEvent eventDiterima;
  int tickCounter = 0;

  for (;;) {
    // Mengecek Queue secara sinkron (Non-blocking)
    if (xQueueReceive(queueEvent, &eventDiterima, 0) == pdPASS) {
      if (eventDiterima == GOL_A) skorA++;
      else if (eventDiterima == GOL_B) skorB++;
      else if (eventDiterima == TOGGLE_PAUSE) isPaused = !isPaused;
      
      lcd.setCursor(0, 0);
      lcd.print("T-A:"); lcd.print(skorA); 
      lcd.print("      T-B:"); lcd.print(skorB);
    }

    // Logika Countdown
    if (!isPaused) {
      tickCounter++;
      if (tickCounter >= 10 && waktuSisa > 0) { // 10 tick = 1 detik
        waktuSisa--;
        tickCounter = 0;
        
        int menit = waktuSisa / 60;
        int detik = waktuSisa % 60;
        
        lcd.setCursor(0, 1);
        lcd.print("WAKTU: ");
        if(menit < 10) lcd.print("0");
        lcd.print(menit);
        lcd.print(":");
        if(detik < 10) lcd.print("0");
        lcd.print(detik);
        lcd.print(" ");
      }
    } else {
      lcd.setCursor(0, 1);
      lcd.print("WAKTU: PAUSED   ");
    }

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}