🚀 Mars Kolonisi Yaşam Destek Simülatörü

C programlama dilinin temel yapıları kullanılarak geliştirilmiş, karakter tabanlı ve komut odaklı bir hayatta kalma simülasyon oyunudur. Oyuncu, Mars'taki küçük bir araştırma istasyonunda tek başına mahsur kalan bir mühendisi yönetir ve kurtarma ekibi ulaşana kadar 15 gün hayatta kalmaya çalışır.

📋 Proje Özeti ve Amaç

Bu proje, temel C programlama yapılarının (koşul ifadeleri, döngüler, aritmetik/mantıksal operatörler ve rastgele sayı üretimi) gerçekçi bir durum yönetimi senaryosu üzerinde uygulanması amacıyla geliştirilmiştir.

Simülasyon boyunca oyuncunun Sağlık, Enerji, Oksijen, Su, Yedek Parça, Modül Hasarı ve Hava Kilidi durumları takip edilir.

🎮 Oyun Mekanikleri ve Komutlar

Oyun başlangıcında oyuncudan zorluk seviyesi (K: Kolay, N: Normal, Z: Zor) seçmesi istenir. Ardından aşağıdaki tek karakterli komutlarla (büyük/küçük harf duyarsız) istasyon yönetilir:

Komut

İşlev

Açıklama

G

Keşif Görevi

Enerji (-15) ve Oksijen (-10) harcar. Mevcut kaynak durumuna göre belirlenen eşik değer üzerinden su, yedek parça bulma veya yaralanma ihtimallerini hesaplar.

O

Oksijen Üretimi

Enerji (-10) ve girilen miktarda su harcayarak oksijen üretir (Her 2 birim su = 3 birim oksijen). Tek sayı girilirse 1 birim su artık olarak depoda kalır.

T

Tamir İşlemi

Enerji (-10) ve modül hasar seviyesine (Hafif/Orta/Ağır) göre değişen miktarda yedek parça harcayarak istasyon hasarını onarır.

D

Durum Raporu

İstasyonun tüm güncel verilerini ve kritik seviye uyarılarını listeler. Tur harcamaz.

U

Uyu / Dinlen

Enerji (+25) kazandırır, oksijen (-12) tüketir. Oksijen seviyesi 20'nin üzerindeyse sağlık artar; altındaysa hipoksi nedeniyle sağlık azalır.

K

Kum Fırtınası

Zorluk seviyesine göre 3, 5 veya 7 tur süren bir kum fırtınasını simüle eder. Çift turlarda hasar 2 katına çıkar; hava kilidi mühürlüyse oksijen sızıntısı engellenir.

H

Hava Kilidi Şifresi

Güne özel üretilen ASCII tabanlı harf şifresini en fazla 3 denemede çözerek hava kilidini mühürlemeyi sağlar.

Q

Çıkış

Simülasyonu sonlandırır ve sonuç raporunu ekrana yazdırır.

⚙️ Teknik Mimari ve Kullanılan C Yapıları

Karar Yapıları (switch-case & if-else if-else):

Zorluk seçimi ve ana komut sistemi switch-case ile yönetilir. Büyük/küçük harf duyarlılığını ortadan kaldırmak için fall-through (case 'g': case 'G':) yapısı kullanılmıştır.

Tamir kademeleri ve keşif sonuçları çoklu if-else if-else zincirleriyle değerlendirilir.

Döngüler (for & do-while):

Ana Oyun ve Zorluk Döngüsü (do-while): Geçerli giriş alınana ve oyun sonu koşulları oluşana kadar akışı sürdürür.

Kum Fırtınası (for): Fırtına turlarını yönetir. Hava kilidi mühürlüyken oksijen sızıntısını atlamak için continue, yaşam belirtileri tükendiğinde fırtınayı kesmek için break kullanılır.

Hava Kilidi Şifresi (do-while): Şifre doğruluğunu ve kalan deneme hakkını tek bir bileşik mantıksal ifadede (!sifreCozuldu && kalanHak > 0) kontrol eder.

Mod (%) Operatörü Kullanımı:

Oksijen üretiminde su miktarının tek/çift kontrolünde (kullanilacakSu % 2),

Kum fırtınasında çift turların tespitinde (t % 2 == 0),

Her 3 geçerli komutta bir yeni güne geçişin hesaplanmasında (tur % 3 == 0) aktif olarak kullanılmıştır.

Rastgelelik: srand(time(NULL)) ve rand() % 100 fonksiyonları ile zar mekanizması kurulmuş, her zar atışı eşik değeriyle birlikte kullanıcıya gösterilmiştir.

🛠️ Kurulum ve Çalıştırma

Projeyi yerel bilgisayarınızda derlemek ve çalıştırmak için bir C derleyicisine (GCC, Clang, MinGW vb.) ihtiyacınız vardır.

Depoyu bilgisayarınıza klonlayın:

git clone https://github.com/kullanici-adiniz/mars-kolonisi-simulatoru.git
cd mars-kolonisi-simulatoru


Kaynak kodu derleyin:

gcc main.c -o mars_simulatoru


Programı çalıştırın:

Windows:

mars_simulatoru.exe


Linux / macOS:

./mars_simulatoru


🏆 Puanlama Sistemi

Simülasyon kazanma, kaybetme veya çıkış (Q) ile sonlandığında aşağıdaki formül üzerinden performans puanı hesaplanır:

$$\text{Puan} = (\text{Gün} \times 100) + \text{Sağlık} + \text{Oksijen} - \text{Modül Hasarı}$$

📄 Lisans

Bu proje eğitim ve akademik değerlendirme amacıyla geliştirilmiştir.
