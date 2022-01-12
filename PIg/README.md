README
======

# Настройка

## Настройка ОС

### Настройка ОС Windows

- Добавить переменную среды **AVR_FIND_ROOT_PATH** с указанием пути до toolchain avr. Например:
```
C:\MyProgram\avr8-gnu-toolchain-win32_x86
```
- Может потребоваться установка nmake.

### Настройка ОС Linux

## Настройка QT

- Добавить инструмент сборки без QT
- Игнорирование предупреждений *avr-gcc: suppress warning "'__progmem__' attribute ignored"*:
	- Перейти в настройки *Tools/Options/C++/Code Model* и выбрать вкладку *Diagnostic Configuration*
	- Скопировать имеющиеся настройки, например *Check for almost everythin*, и добавить в них *-Wno-attributes*. В *Windows* набор настроек выглядит так:
	```
	-Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-unused-macros -Wno-newline-eof -Wno-exit-time-destructorsWno-global-constructors -Wno-gnu-zero-variadic-macro-arguments -Wno-documentation -Wno-shadow -Wno-switch-enum -Wno-missing-prototypes -Wno-used-but-marked-unused -Wno-attributes
	```

