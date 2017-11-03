# How to install

cd into the folder and open terminal inside folder and execute following commands

    sudo su
    make
    insmod ./driver.ko
    grep mydevice /proc/devices ( bunun çıktısında üç haneli numarayı not al)
    mknod /dev/mydevice0 c ### 0 (### yerine üç haneli numarayı yaz)
    chmod 666 /dev/mydevice0

# How message each other

Şimdilik kullanıcı id'si ile çalışmakta,

    cat /etc/passwd

komutu ile kullanıcı adına karşılık gelen id'i görebilirsiniz. Örnekteki "mkytr" kullanıcısının id'si 1000'dir.

    root~ echo "@1000 merhaba" > /dev/mydevice0
    ...
    mkytr~ cat /dev/mydevice0
    mkytr~ 0: merhaba