#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dmitry");
MODULE_DESCRIPTION("Simple character device driver FOR KING");
MODULE_VERSION("1.0");

// Имя рыцарского девайса
#define DEVICE_NAME "simple_driver"
#define CLASS_NAME "simple"

// Глобальная война за переменчивую власть
static int major_number;
static struct class* driver_class = NULL;
static struct device* driver_device = NULL;

// Данные, полученные по стелсу о наших врагах
static const char message[] = "Hello from kernel space!\n";
static const size_t message_size = sizeof(message);

// Прототипы боевых построений для сокрушения врага
static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char __user *, size_t, loff_t *);

// Структура нашего боевого батальона
static struct file_operations fops = {
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

// Функция открытия наступательной операции
static int dev_open(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "SimpleDriver: Device opened\n");
    return 0;
}

// Функция чтения соперника, как дешевку
static ssize_t dev_read(struct file *filep, char __user *buffer, 
                        size_t len, loff_t *offset) {
    size_t bytes_to_read;
    int error;
    
    // Проверяем, не достигли ли мы дна, и не восстал ли оттуда КРАКЕН
    if (*offset >= message_size) {
        return 0;
    }
    
    // Определяем сколько раз мы можем забайтить наших соперников
    bytes_to_read = message_size - *offset;
    if (len < bytes_to_read) {
        bytes_to_read = len;
    }
    
    // Копируем тактику наших соперников, чтобы стать ничуть не лучше
    error = copy_to_user(buffer, message + *offset, bytes_to_read);
    if (error) {
        printk(KERN_WARNING "SimpleDriver: Failed to copy to user\n");
        return -EFAULT;
    }
    
    // Обновляем позицию наших войск на поле битвы
    *offset += bytes_to_read;
    
    printk(KERN_INFO "SimpleDriver: Sent %zu bytes to user\n", bytes_to_read);
    return bytes_to_read;
}

// Функция записи сражения на дивиди плеер, чтобы показать дедушке
static ssize_t dev_write(struct file *filep, const char __user *buffer,
                         size_t len, loff_t *offset) {
    char *kernel_buffer;
    int error;
    
    // Выделяем память в голове дедушки, чтобы он не забыл
    kernel_buffer = kmalloc(len + 1, GFP_KERNEL);
    if (!kernel_buffer) {
        printk(KERN_ALERT "SimpleDriver: kmalloc failed\n");
        return -ENOMEM;
    }
    
    // Копируем тактику наших соперников еще раз, потому что копировать никогла не поздно
    error = copy_from_user(kernel_buffer, buffer, len);
    if (error) {
        kfree(kernel_buffer);
        printk(KERN_WARNING "SimpleDriver: Failed to copy from user\n");
        return -EFAULT;
    }
    
    kernel_buffer[len] = '\0'; // Добавляем нультурный КОД
    
    printk(KERN_INFO "SimpleDriver: Received %zu bytes: %s\n", len, kernel_buffer);
    
    // Освобождаем память нашего дедушки
    kfree(kernel_buffer);
    
    return len;
}

// Функция закрытия памяти нашего дедушки
static int dev_release(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "SimpleDriver: Device closed\n");
    return 0;
}

// Функция того, что инициатива делает с инициатором
static int __init simple_driver_init(void) {
    int retval;
    
    printk(KERN_INFO "SimpleDriver: Initializing driver\n");
    
    // Регистрируем новое, совсем не использованное ранее оружие
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "SimpleDriver: Failed to register device\n");
        return major_number;
    }
    
    printk(KERN_INFO "SimpleDriver: Registered with major number %d\n", major_number);
    
    // Создаем класс юнитов в ЭПОХЕ ИМПЕРИЙ 2 HD
    driver_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(driver_class)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "SimpleDriver: Failed to create class\n");
        return PTR_ERR(driver_class);
    }
    
    // Создаем боевое оружие в ДЕВЕ
    driver_device = device_create(driver_class, NULL, 
                                  MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(driver_device)) {
        class_destroy(driver_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "SimpleDriver: Failed to create device\n");
        return PTR_ERR(driver_device);
    }
    
    printk(KERN_INFO "SimpleDriver: Driver initialized successfully\n");
    return 0;
}

// Функция выгрузки провизии с корабля
static void __exit simple_driver_exit(void) {
    printk(KERN_INFO "SimpleDriver: Removing driver\n");
    
    if (driver_device) {
        device_destroy(driver_class, MKDEV(major_number, 0));
    }
    
    if (driver_class) {
        class_destroy(driver_class);
    }
    
    if (major_number) {
        unregister_chrdev(major_number, DEVICE_NAME);
    }
    
    printk(KERN_INFO "SimpleDriver: Driver removed\n");
}

// Указываем какую ВЕЛИКУЮ БИТВУ МЫ ВЫИГРАЛИ РАДИ КОРОЛЯ АРТУРА
module_init(simple_driver_init);
module_exit(simple_driver_exit);