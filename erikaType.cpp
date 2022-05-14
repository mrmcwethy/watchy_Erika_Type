#include "erikaType.h"

#define DARKMODE false

const uint8_t BATTERY_SEGMENT_WIDTH = 7;
const uint8_t BATTERY_SEGMENT_HEIGHT = 11;
const uint8_t BATTERY_SEGMENT_SPACING = 9;
const uint8_t WEATHER_ICON_WIDTH = 48;
const uint8_t WEATHER_ICON_HEIGHT = 32;

#define TOP_LINE_Y 70
#define BOTTOM_LINE_Y 130
#define ICON_X 40
#define FONT1_FOR_BOTTOM Erika_Type_B12pt7b
#define FONT1_FOR_BOTTOM_SIZE 12
#define FONT2_FOR_BOTTOM Erika_Type_B10pt7b
#define FONT2_FOR_BOTTOM_SIZE 10
#define FONT1_FOR_TOP Erika_Type_B20pt7b
#define FONT1_FOR_TOP_SIZE 32
#define FONT2_FOR_TOP Erika_Type_B10pt7b
#define FONT2_FOR_TOP_SIZE 10
#define FONT_FOR_CENTER Erika_Type_B30pt7b
#define FONT_FOR_CENTER_SIZE 30

void erikaType::drawWatchFace(){
    display.fillScreen(DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);
    display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    drawTime();
    drawDate();
    drawSteps();
    drawWeather();
    drawBattery();
    display.drawBitmap(200-26-5,200-18-5, WIFI_CONFIGURED ? wifi : wifioff, 26, 18, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    if(BLE_CONFIGURED){
        display.drawBitmap(ICON_X, BOTTOM_LINE_Y+10, bluetooth, 13, 21, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    }
   
    display.drawLine(0,TOP_LINE_Y,200,TOP_LINE_Y,DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.drawLine(0,BOTTOM_LINE_Y,200,BOTTOM_LINE_Y,DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.drawLine(100,0,100,TOP_LINE_Y,DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.drawLine(100,BOTTOM_LINE_Y,100,200,DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
}

void erikaType::drawTime(){
    display.setFont(&FONT_FOR_CENTER);
    display.setCursor(5, 100+(FONT_FOR_CENTER_SIZE/2));
    int displayHour;
    if(HOUR_12_24==12){
      displayHour = ((currentTime.Hour+11)%12)+1;
    } else {
      displayHour = currentTime.Hour;
    }
    if(displayHour < 10){
        display.print(" ");
    }
    
    display.print(displayHour);
    display.print(":");
    if(currentTime.Minute < 10){
        display.print("0");
    }
    display.println(currentTime.Minute);
}

void erikaType::drawDate(){
    display.setFont(&FONT1_FOR_BOTTOM);

    int16_t  x1, y1;
    uint16_t w, h;

    String dayOfWeek = dayShortStr(currentTime.Wday);
    display.getTextBounds(dayOfWeek,0, 0, &x1, &y1, &w, &h);

    display.setCursor(110, BOTTOM_LINE_Y+FONT1_FOR_BOTTOM_SIZE+10);
    display.println(dayOfWeek);

    display.setCursor(110+w, BOTTOM_LINE_Y+FONT1_FOR_BOTTOM_SIZE+10);
    display.println(currentTime.Day);
    
    display.setFont(&FONT2_FOR_BOTTOM);
    String month = monthShortStr(currentTime.Month);
    display.setCursor(110, BOTTOM_LINE_Y+2*FONT1_FOR_BOTTOM_SIZE+20);
    display.println(month);



}
void erikaType::drawSteps(){
    display.setFont(&FONT1_FOR_TOP);
    // reset step counter at midnight
    if (currentTime.Hour == 0 && currentTime.Minute == 0){
      sensor.resetStepCounter();
    }
    uint32_t stepCount = sensor.getCounter();
 
    int16_t  x1, y1;
    uint16_t w, h;
    display.getTextBounds(String(stepCount), 0, 0, &x1, &y1, &w, &h);
    display.setCursor(100-10-w, TOP_LINE_Y-8);
    display.println(stepCount);
    
    display.setFont(&FONT2_FOR_TOP);
    display.getTextBounds(String("Steps"),0, 0, &x1, &y1, &w, &h);
    display.setCursor(100-10-w, TOP_LINE_Y-8-FONT1_FOR_TOP_SIZE-10);
    display.println("Steps");
}

void erikaType::drawBattery(){
    int8_t batteryLevel = 0;
    float VBAT = getBatteryVoltage();
    if(VBAT > 4.1){
        batteryLevel = 100;
    }
    else {
        batteryLevel = 100.0*(VBAT-3.8)/(4.1-3.8);
    }   

    int16_t  x1, y1;
    uint16_t w, h;
    String batt(String(batteryLevel)+"%"); 
    
    display.setFont(&FONT1_FOR_BOTTOM);
    display.getTextBounds(batt,0, 0, &x1, &y1, &w, &h);
    display.setCursor(100-10-w, BOTTOM_LINE_Y+FONT1_FOR_BOTTOM_SIZE+10);
    display.println(batt);

    display.setFont(&FONT2_FOR_BOTTOM);
    display.getTextBounds(String("Batt"),0, 0, &x1, &y1, &w, &h);
    display.setCursor(100-10-w, BOTTOM_LINE_Y+2*FONT1_FOR_BOTTOM_SIZE+20);
    display.println("Batt");

}

void erikaType::drawWeather(){

    weatherData currentWeather = getWeatherData();

    int8_t temperature = currentWeather.temperature;
    int16_t weatherConditionCode = currentWeather.weatherConditionCode;
     
    const unsigned char* weatherIcon = 0;

    //https://openweathermap.org/weather-conditions
    if(weatherConditionCode > 801){//Cloudy
    weatherIcon = cloudy;
    }else if(weatherConditionCode == 801){//Few Clouds
    weatherIcon = cloudsun;
    }else if(weatherConditionCode == 800){//Clear
    weatherIcon = sunny;
    }else if(weatherConditionCode >=700){//Atmosphere
    weatherIcon = atmosphere;
    }else if(weatherConditionCode >=600){//Snow
    weatherIcon = snow;
    }else if(weatherConditionCode >=500){//Rain
    weatherIcon = rain;
    }else if(weatherConditionCode >=300){//Drizzle
    weatherIcon = drizzle;
    }else if(weatherConditionCode >=200){//Thunderstorm
    weatherIcon = thunderstorm;
    }
    if (weatherIcon)
      display.drawBitmap(200-WEATHER_ICON_WIDTH, 5, weatherIcon, WEATHER_ICON_WIDTH, WEATHER_ICON_HEIGHT, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);

    int16_t  x1, y1;
    uint16_t w, h;

    display.setFont(&FONT1_FOR_TOP);
    display.getTextBounds(String(temperature),0, 0, &x1, &y1, &w, &h);
    display.setCursor(110, TOP_LINE_Y-8);
    display.println(temperature);
    
    display.drawBitmap(110+w+4, TOP_LINE_Y-FONT1_FOR_TOP_SIZE, currentWeather.isMetric ? celsius : fahrenheit, 26, 20, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
   
}
