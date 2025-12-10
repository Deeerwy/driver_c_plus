# Simple Linux Driver FOR KING ARTUR

Простой драйвер символьного устройства для Linux.

## Требования
- Linux с ядром версии 5.x+
- Установленные заголовки ядра
- Права root для тестирования

## Установка заголовков ядра

### Ubuntu/Debian
```bash
sudo apt update
sudo apt install linux-headers-$(uname -r) build-essential