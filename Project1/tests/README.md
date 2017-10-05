# Testler
Bu klasör altında çekirdek değişikliklerini test etmek için gereken programlar bulunmaktadır.

## myflag_updater.c

Bu test programı sistem çağrısının beklendiği gibi çalışıp çalışmadığını kontrol etmektedir. Derlemek için,

    gcc myflag_updater.c -o myflag_updater
    
komutunu çalıştırınız. Bu program bayrak değeri değiştirilmek istenen işlemin PID'ini ve bayrak değerini (0 veya 1) konsol argümanı olarak beklemektedir. Sistem çağrısı yönetici izni gerektirdiği için programın yönetici izinleri sağlandıktan sonra çalıştırılması gerekmektedir,

    sudo ./myflag_updater 12345 1

## exit_tester.c

Bu test programı exit sistem çağrısının bayrak değerine ve nice değerine göre hareket edip etmediğini kontrol etmek içindir. Derlemek için,

    gcc exit_tester.c -o exit_tester

komutunu çalıştırınız. Bu program parent işlem için bayrak değerini konsol argümanı olarak beklemektedir. Sistem çağrısı root izinlerine ihtiyaç duyduğu için bu programın root olarak çalıştırılması gerekmektedir. Nice değerini "renice" komutu ile başka bir terminal üzerinden parent işlem için 10'dan büyük bir değere ayarlamanız gerekmektedir.

    sudo ./exit_tester 1
    
