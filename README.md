# Ön Hazırlık
Kodu çekmeye başlamadan önce sistemde git için ön hazırlık gerekmektedir.

    ~$ mkydir folder
    ~$ cd folder
    ~$ git init
    ~$ git remote add origin https://github.com/mkykadir/SystemProgramming.git
    
# Kodun Çekilmesi (Pull)

    ~/folder$ git pull origin master
    
# Kodun Yüklenmesi
Kod yüklenmeden önce derleme çıktılarının temizlenmesi gerekmektedir. Temizleme için;

    ~/folder$ cd Project1
    ~/folder/Project1$ cd linux-soruce-3.13.0
    ~/folder/Project1/linux-source-3.13.0$ make-kpkg clean
    ~/folder/Project1/linux-source-3.13.0$ cd ../..
    ~/folder$ git status
    ~/folder$ git add *.* (değişiklikleri ayrı commit edin)
    ~/folder$ git commit -m "Commit mesajı"
    ~/folder$ git push origin master
    
   
