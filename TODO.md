- 2021.09.03
	1. Объединить сервер и приложение для меню.
		- [ ] Перенести сервер в приложение.
		- [ ] Cоединениt сервера и приложения напрямую, без использования последовательных портов.
		- [ ] Оставить возможность подключения к серверу ПИ по последовательному порту.
	2. Добавить серверу возможность выбора версии прошивки МК.
		- [ ] Вынести конфигурацию параметров/команд сервера в JSON.
		- [ ] Конфигурация должна поддерживать разные прошивки МК.
		- [ ] Сделать тесты для проверки соответствия конфигурации и ПО МК.
	3. Настройки сервера.
		- [ ] Добавить сохранение текущих настроек.
		- [ ] Добавить возможность сохранения и загрузки файла настроек.
	4. Управление.
		- [x] Проверить где встречаются пункты с номерами и их необходимость замены. 
			М.б. необходимость в проверке типа аппарата / совместимости отсутствует? И можно их заменять всегда?
	5. Проверить
		- [ ] Работу команды считывания типа АК и времени до него (0x0A).

- 2021.03.02
	1. Изменения в классе времени (и даты в том числе)
		- [ ] Проверить синхронизацию времени по Modbus.
		- [ ] Проверить синхронизацию времени по МЭК-101
		- [ ] Проверить установку времени с Конфигуратора
		- [x] Проверить установку времени с клавиатуры

- 2020.03.06 
	1. Синхронизация времени по МЭК-101.
		- [ ] Нужно учитывать время с момента приема сообщения из МЭК-101 до отправки в БСП.