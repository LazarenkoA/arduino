#define SOUND_PIN A3

#include "VolAnalyzer.h" // амплитудный анализ звука
#include <GRGB.h>

VolAnalyzer analyzer(SOUND_PIN);
GRGB led(COMMON_CATHODE, 4, 3, 5);

int R = 0;
int G = 255;
int maxVol = 255;
int minVol = 0;

void setup() {
  Serial.begin(9600);
  pinMode(SOUND_PIN, INPUT);

  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  analyzer.setVolMin(minVol);
  analyzer.setVolMax(maxVol);

  led.setBrightness(255);

  // включить гамма-коррекцию (по умолч. выключено)
  led.setCRT(true);

  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);

  // установить HEX цвет (24 бита)
  //led.setHEX(0xFFAA00);
  //led.setHEX(0x00FF00, 120); // + "локальная" яркость [0-255]

}

void loop() {
 
  if (analyzer.tick()) {
    int vol = analyzer.getVol();
    //    Serial.print(vol);
    //    Serial.print(" ");
    //    Serial.println(brightness(vol));


    if (vol > 0) {
      led.enable();
      int br = map(vol, 0, maxVol, 10, 255);

      led.setHSVfast(map(vol, 0, maxVol, 80, 0), 255, br); // переход к красному через желтый
      //led.setHSVfast(map(vol, 0, maxVol, 80, 255), 255, br); // переход к красному через синий
    } else {
      led.disable();
    }


    // Serial.println("R: " + String(min(255, vol)) + "; G: " + String(G - (max(0, vol - 255))));

    //    Serial.print(' ');
    //    Serial.println("Min " + String(analyzer.getMin()));
    //    Serial.println("Max " + String(analyzer.getMax()));
    //    if(analyzer.pulse()) {
    //      Serial.println("Всплеск");
    //    }
    //    Serial.print(' ');
    //    Serial.print(analyzer.getMin());
    //    Serial.print(' ');
    //    Serial.println(analyzer.getMax());
    // delay(100);
  }


  //  int R = 0;
  //  int G = 255;
  //
  //  led.setRGB(R, G, 0);
  //  delay(1000);
  //
  //  while (G > 0) {
  //    if (R < 245) {
  //      R += 10;
  //    } else {
  //      G -= 10;
  //    }
  //    if (G < 0) {
  //      G = 0;
  //    }
  //    if (R > 255) {
  //      R = 255;
  //    }
  //    //}
  //
  //    //for (int i = 0; i <= 500; i++) {
  //    //      if (i > 255) {
  //    //        G -= 10;
  //    //      } else {
  //    //        R += 10;
  //    //      }
  //
  //    //delay(2);
  //    led.setRGB(R, G, 0);
  //   // Serial.println("R: " + String(R) + "; G: " + String(G));
  //  }
  // // delay(1000);

}

int brightness(int vol) {
  return (255 / float(maxVol)) * pow(2.71, float(vol) * (log(maxVol)) / float(maxVol));
}
