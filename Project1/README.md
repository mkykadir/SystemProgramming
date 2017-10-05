# Project1

Projenin detaylı anlatımı ve istenilenleri görmek için 
    project1-kernel-2017.pdf 
dosyasını inceleyiniz.

## Özet

Bu projede Linux çekirdek kodunun güncellenerek her işlem için yalnızca 0 veya 1 değerlerini kabul edecek bir bayrak değeri tutulması istenmiştir. Bu bayrak değeri bir sistem çağrısı ile güncellenecektir ve bu sistem çağrısının yazılması da öğrenciden beklenmektedir. Sistem çağırısı PID'i verilen işlemin bayrak değerini günceller ancak sistem çağrısı yalnızca yönetici izni ile çalıştırılabilir. Eğer işlemin nice değeri 10'dan büyük ve bayrak değeri 1 ise exit çağrısından sonra işlemin tüm çocukları öldürülür, eğer nice değeri 10'dan küçük veya bayrak değeri 0 ise exit normal şekilde çalışır.

## Testler

Proje için gereken testler
    tests/
klasörü altında sunulmuştur. Testler hakkında detaylı bilgi için klasör içindeki yardıma bakınız.
