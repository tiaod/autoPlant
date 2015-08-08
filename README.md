# 自动浇花系统
制作过程：
http://www.guokr.com/post/694718/

使用到的库有：

温湿度模块：
https://github.com/adafruit/DHT-sensor-library

时间库：
http://www.pjrc.com/teensy/td_libs_Time.html

闹钟库（用来执行定时程序）：
http://www.pjrc.com/teensy/td_libs_TimeAlarms.html

引脚定义
```
#define RELAY1 9    //继电器1,节能模式时控制其他模块的电源
#define RELAY2 10   //继电器2,控制水阀
#define DHT11_PIN 2 //温湿度模块
#define SOIL_HUMIDITY_PIN A1 //土壤湿度模块
```

用户自定义设置
```
#define CHECK_INTERVAL 9600 //检查土壤湿度间隔的秒数(9600秒=3小时一次)
#define MAX_SOIL_HUMIDITY 600 //土壤的湿度阈值（大于这个值就会浇水）
#define WATER_TIME 5 //单次浇水的秒数（控制每次的浇水量）
#define IS_MINIMUN_POWER false //是否开启节能模式。用于太阳能电池供电的情况
```