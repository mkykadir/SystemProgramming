# Testler
Bu klasör altında çekirdek değişikliklerini test etmek için gereken programlar bulunmaktadır.

## myflag_updater.c

Bu test programı sistem çağrısının beklendiği gibi çalışıp çalışmadığını kontrol etmektedir. Derlemek için,

    gcc myflag_updater.c -o myflag_updater
    
komutunu çalıştırınız. Bu program bayrak değeri değiştirilmek istenen işlemin PID'ini ve bayrak değerini (0 veya 1) konsol argümanı olarak beklemektedir. Sistem çağrısı yönetici izni gerektirdiği için programın yönetici izinleri sağlandıktan sonra çalıştırılması gerekmektedir,

    sudo ./myflag_updater 12345 1
    
