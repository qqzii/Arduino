#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define ONE_WIRE_BUS 2                // пин ардуино к которому подключен DS18B20
#define OLED_RESET 4                  

DeviceAddress thermometerAddress;     //нужно для хранения адреса датчика

Adafruit_SSD1306 display(OLED_RESET); // создание экземпляра отображения
OneWire oneWire(ONE_WIRE_BUS);        // экземпляр oneWire для связи с температурой IC
DallasTemperature tempSensor(&oneWire);  // ссылка OneWire для датчика


void setup()   {

  Serial.begin(9600);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // инициализация адреса дисплея
  display.clearDisplay();                     // очистить буфер отображения
  display.display();                          // обновить дисплей

  Serial.println("DS18B20 Temperature IC Test");
  Serial.println("Locating devices...");
  tempSensor.begin();                         // инициализация датчика температуры

  if (!tempSensor.getAddress(thermometerAddress, 0))
    Serial.println("Unable to find Device.");
  else {
    Serial.print("Device 0 Address: ");
    printAddress(thermometerAddress);
    Serial.println();
  }

  tempSensor.setResolution(thermometerAddress, 11);      // настраиваем чувтвительность от 9 до 12 бит( я поставил 11)
}


void loop() {

  tempSensor.requestTemperatures();                      // оправиваем датчик
  displayTemp(tempSensor.getTempC(thermometerAddress));  // покажем на OLED дисплее

  delay(500);                                            // обновление показаний каждые полсекунды
}

void displayTemp(float temperatureReading) {             

  // настройка размера OLED-текста и печать данных о температуре
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Temp: ");

  // show temperature °C
  display.print(temperatureReading, 2);  // округление до 2 знаков после запятой
  display.println(" C");
  Serial.print(temperatureReading);      // печать в сериал, если не нужно закомменть или удали
  Serial.print("°");
  Serial.print("C  ");


  display.display();                    // обновление дисплея с новым текстом
}


// печать адреса устройства из массива адресов в сериал
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
