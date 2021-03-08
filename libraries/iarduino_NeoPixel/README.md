[![](https://iarduino.ru/img/logo.svg)](https://iarduino.ru)[![](https://wiki.iarduino.ru/img/git-shop.svg?3)](https://iarduino.ru) [![](https://wiki.iarduino.ru/img/git-wiki.svg?2)](https://wiki.iarduino.ru) [![](https://wiki.iarduino.ru/img/git-lesson.svg?2)](https://lesson.iarduino.ru)[![](https://wiki.iarduino.ru/img/git-forum.svg?2)](http://forum.trema.ru)

# iarduino\_NeoPixel

**This is a library for Arduino IDE. It allows to control [NeoPixel](https://iarduino.ru/shop/Expansion-payments/neopixel-trema-modul.html) modules made by iArduino.ru**

**Данная библиотека для Arduino IDE позволяет управлять модулями адресных светодиодов [NeoPixel](https://iarduino.ru/shop/Expansion-payments/neopixel-trema-modul.html) от [iArduino.ru](https://iarduino.ru)**

> Подробнее про установку библиотеки читайте в нашей [инструкции](https://wiki.iarduino.ru/page/Installing_libraries/).

> Подробнее про подключение к [Arduino UNO](https://iarduino.ru/shop/boards/arduino-uno-r3.html)/[Piranha UNO](https://iarduino.ru/shop/boards/piranha-uno-r3.html) читайте на нашей [wiki](https://wiki.iarduino.ru/page/adresnye-svetodiody-moduli-neopixel/#h3_3)


| Модель | Ссылка на магазин |
|---|---|
| <p>Адресные светодиоды, модули NeoPixel</p> <img src="https://wiki.iarduino.ru/img/resources/784/784.svg" width="100px"></img>| https://iarduino.ru/shop/Expansion-payments/neopixel-trema-modul.html |


## Назначение функций:

**Подробное описание работы с библиотекой и примеры смотрите на [нашем сайте](https://wiki.iarduino.ru/page/adresnye-svetodiody-moduli-neopixel/#h3_6)**

**Подключаем библиотеку**

```C++
#include <iarduino_NeoPixel.h> // Подключаем библиотеку.
iarduino_NeoPixel ОБЪЕКТ( №_ВЫВОДА , КОЛИЧЕСТВО_СВЕТОДИОДОВ ); //Создаём объект
```



**Инициализация работы** 

```C++
ОБЪЕКТ.begin(); // Инициализация работы с адресными светодиодами.
```

**Указание цвета** 

```C++
ОБЪЕКТ.setColor( №_СВЕТОДИОДА , ЦВЕТ [ , ЯРКОСТЬ ] ); // Указание цвета для адресного светодиода.
```

**Запись цветов в светодиоды** 

```C++
ОБЪЕКТ.write(); // Запись всех установленных цветов в светодиоды.
```

### Дополнительные (необязательные) функции:

**Получение количества светодиодов** 

```C++
ОБЪЕКТ.count(); // Получение количества используемых светодиодов.
```

**Получение цветов** 

```C++
ОБЪЕКТ.getPointer(); // Получение указателя на массив цветов светодиодов.
```

