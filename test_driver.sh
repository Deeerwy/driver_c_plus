#!/bin/bash

# Тестовый скрипт для драйвера

echo "=== Тестирование драйвера ==="

# Проверяем права
if [ "$EUID" -ne 0 ]; then 
    echo "Пожалуйста, запустите с правами root (sudo)"
    exit 1
fi

echo "1. Сборка драйвера..."
make clean
make

if [ $? -ne 0 ]; then
    echo "Ошибка сборки!"
    exit 1
fi

echo "2. Загрузка модуля..."
insmod simple_driver.ko

echo "3. Проверка загрузки..."
lsmod | grep simple_driver

echo "4. Проверка созданного устройства..."
ls -l /dev/simple_driver

echo "5. Чтение из устройства..."
cat /dev/simple_driver

echo "6. Запись в устройство..."
echo "Test message from user" > /dev/simple_driver

echo "7. Проверка сообщений ядра..."
dmesg | tail -10

echo "8. Выгрузка модуля..."
rmmod simple_driver

echo "9. Очистка..."
make clean

echo "=== Тестирование завершено ==="   