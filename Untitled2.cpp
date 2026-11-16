#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    /* Durum Degiskenleri */
    int saglik = 100, enerji = 100, oksijen = 100;
    int su = 0, yedekParca = 0, modulHasari = 0;
    int kilitMuhurlu = 0;
    int gun = 1, tur = 0;

    /* Zorluk ve Oyun Kontrol Degiskenleri */
    char zorluk, komut;
    int zorlukGecerli = 0;
    int firtinaSuresi = 5, firtinaHasarCarpani = 1;
    int kesifEsikBonus = 0;
    int oyunDevam = 1;
    int cikisSebebi = 0; /* 1: Q ile cikis, 2: Olum, 3: 15. gun tamamlandi */
    int puan = 0;

    /* Rastgelelik Baslatma */
    srand((unsigned int)time(NULL));

    printf("====================================================\n");
    printf("   MARS KOLONISI YASAM DESTEK SIMULATORU (15 GUN)   \n");
    printf("====================================================\n");

    /* 1. ZORLUK SEVIYESI SECIMI */
    do {
        printf("\nZorluk Seviyesi Secin [K: Kolay, N: Normal, Z: Zor]: ");
        scanf(" %c", &zorluk);

        switch (zorluk) {
            case 'k':
            case 'K':
                saglik = 100; enerji = 100; oksijen = 90;
                su = 20; yedekParca = 5; modulHasari = 10;
                firtinaSuresi = 3;
                firtinaHasarCarpani = 1;
                kesifEsikBonus = 15;
                zorlukGecerli = 1;
                printf(">> Zorluk: KOLAY secildi.\n");
                break;

            case 'n':
            case 'N':
                saglik = 90; enerji = 85; oksijen = 80;
                su = 12; yedekParca = 3; modulHasari = 20;
                firtinaSuresi = 5;
                firtinaHasarCarpani = 1;
                kesifEsikBonus = 0;
                zorlukGecerli = 1;
                printf(">> Zorluk: NORMAL secildi.\n");
                break;

            case 'z':
            case 'Z':
                saglik = 80; enerji = 75; oksijen = 70;
                su = 6; yedekParca = 1; modulHasari = 35;
                firtinaSuresi = 7;
                firtinaHasarCarpani = 2;
                kesifEsikBonus = -10;
                zorlukGecerli = 1;
                printf(">> Zorluk: ZOR secildi.\n");
                break;

            default:
                printf("[UYARI] Gecersiz zorluk seviyesi! Lutfen K, N veya Z giriniz.\n");
                break;
        }
    } while (!zorlukGecerli);

    /* 2. ANA KOMUT DONGUSU */
    do {
        int turHarca = 0;
        int zar, esik;

        printf("\n----------------------------------------------------\n");
        printf("GUN: %d / 15 | TUR: %d | Saglik: %d | Enerji: %d | Oksijen: %d\n",
               gun, tur, saglik, enerji, oksijen);
        printf("Su: %d | Yedek Parca: %d | Modul Hasari: %d | Kilit: %s\n",
               su, yedekParca, modulHasari, kilitMuhurlu ? "MUHURLU" : "ACIK");
        printf("Komutlar: [G]Kesif [O]Oksijen [T]Tamir [D]Durum [U]Uyu [K]Kum Firtinasi [H]Hava Kilidi [Q]Cikis\n");
        printf("Komutunuz: ");
        scanf(" %c", &komut);

        switch (komut) {
            /* --- G: KESIF GOREVI --- */
            case 'g':
            case 'G':
                if (enerji < 15 || oksijen < 10) {
                    printf("[IPTAL] Kesif icin yetersiz kaynak! (Gereken: 15 Enerji, 10 Oksijen)\n");
                } else {
                    int bulunanSu, bulunanParca, yaralanmaHasari;
                    enerji -= 15;
                    oksijen -= 10;
                    turHarca = 1;

                    if (enerji > 40 && oksijen > 30) {
                        esik = 65 + kesifEsikBonus;
                    } else {
                        esik = 35 + kesifEsikBonus;
                    }
                    if (esik < 10) esik = 10;
                    if (esik > 90) esik = 90;

                    zar = rand() % 100;
                    printf("[KESIF] Zar: %d / Esik: %d -> %s\n",
                           zar, esik, (zar < esik) ? "BASARILI" : "BASARISIZ");

                    if (zar < esik / 2) {
                        bulunanSu = 4 + (rand() % 5);
                        su += bulunanSu;
                        printf(">> Yeralti buz tabakasi bulundu! +%d birim Su eklendi.\n", bulunanSu);
                    } else if (zar < esik) {
                        bulunanParca = 1 + (rand() % 2);
                        yedekParca += bulunanParca;
                        printf(">> Eski kesif aracindan parca sokuldu! +%d Yedek Parca eklendi.\n", bulunanParca);
                    } else {
                        printf(">> Bolge tarandi ancak ise yarar kaynak bulunamadi (Eli bos donuldu).\n");
                    }

                    if (zar < 10 || modulHasari > 70) {
                        yaralanmaHasari = 12;
                        saglik -= yaralanmaHasari;
                        printf("[TEHLIKE] Dusuk sans veya yuksek modul hasari nedeniyle yaralandiniz! -%d Saglik\n",
                               yaralanmaHasari);
                    }
                }
                break;

            /* --- O: OKSIJEN URET --- */
            case 'o':
            case 'O':
                if (enerji < 10) {
                    printf("[IPTAL] Oksijen uretimi icin en az 10 Enerji gerekli!\n");
                } else if (su < 2) {
                    printf("[IPTAL] Oksijen uretimi icin en az 2 birim Su gerekli! (Mevcut Su: %d)\n", su);
                } else if (oksijen >= 100) {
                    printf("[IPTAL] Oksijen tanklari zaten tamamen dolu (100)!\n");
                } else {
                    int kullanilacakSu, artikSu, islenenSu, uretilenOksijen;
                    printf("Kullanilacak su miktarini girin (Mevcut: %d): ", su);
                    scanf("%d", &kullanilacakSu);

                    if (kullanilacakSu < 2 || kullanilacakSu > su) {
                        printf("[IPTAL] Gecersiz miktar! 2 ile %d arasinda bir deger girmelisiniz.\n", su);
                    } else {
                        artikSu = kullanilacakSu % 2;
                        islenenSu = kullanilacakSu - artikSu;
                        uretilenOksijen = (islenenSu / 2) * 3;

                        su -= islenenSu;
                        enerji -= 10;
                        oksijen += uretilenOksijen;
                        turHarca = 1;

                        printf("[OKSIJEN URETIMI] %d birim su islendi, +%d Oksijen uretildi.\n",
                               islenenSu, uretilenOksijen);
                        if (artikSu == 1) {
                            printf(">> Girilen miktar tek sayi oldugu icin 1 birim su artik olarak depoda kaldi.\n");
                        }
                    }
                }
                break;

            /* --- T: TAMIR --- */
            case 't':
            case 'T':
                if (enerji < 10) {
                    printf("[IPTAL] Tamir islemi icin en az 10 Enerji gerekli!\n");
                } else if (yedekParca <= 0) {
                    printf("[IPTAL] Hic yedek parcaniz yok! Tamir yapilamaz.\n");
                } else if (modulHasari == 0) {
                    printf("[IPTAL] Modul hasari bulunmuyor (Hasar: 0).\n");
                } else {
                    int gerekenParca, onarimMiktari;

                    if (modulHasari <= 30) {
                        gerekenParca = 1;
                        onarimMiktari = 15;
                        printf("[TAMIR - HAFIF HASAR] ");
                    } else if (modulHasari <= 70) {
                        gerekenParca = 2;
                        onarimMiktari = 30;
                        printf("[TAMIR - ORTA HASAR] ");
                    } else {
                        gerekenParca = 3;
                        onarimMiktari = 45;
                        printf("[TAMIR - AGIR HASAR] ");
                    }

                    if (yedekParca < gerekenParca) {
                        printf("Yetersiz yedek parca! Bu hasar seviyesi icin %d parca gerekiyor (Mevcut: %d).\n",
                               gerekenParca, yedekParca);
                    } else {
                        yedekParca -= gerekenParca;
                        enerji -= 10;
                        modulHasari -= onarimMiktari;
                        turHarca = 1;
                        printf("%d Yedek Parca harcandi, Modul Hasari %d azaldi.\n",
                               gerekenParca, onarimMiktari);
                    }
                }
                break;

            /* --- D: DURUM RAPORU --- */
            case 'd':
            case 'D':
                printf("\n================ DETAYLI DURUM RAPORU ================\n");
                printf("Gun: %d / 15 | Toplam Tur: %d\n", gun, tur);
                printf("Saglik      : %d / 100\n", saglik);
                printf("Enerji      : %d / 100\n", enerji);
                printf("Oksijen     : %d / 100\n", oksijen);
                printf("Su Stogu    : %d birim\n", su);
                printf("Yedek Parca : %d adet\n", yedekParca);
                printf("Modul Hasari: %d / 100\n", modulHasari);
                printf("Hava Kilidi : %s\n", kilitMuhurlu ? "MUHURLU (Guvenli)" : "ACIK (Riskli)");
                printf("------------------------------------------------------\n");

                if (oksijen < 25) {
                    printf("[KRITIK UYARI] Oksijen seviyesi tehlikeli derecede dusuk (< 25)!\n");
                }
                if (saglik < 30) {
                    printf("[KRITIK UYARI] Saglik durumu kritik (< 30)! Dinlenin.\n");
                }
                if (enerji < 20) {
                    printf("[KRITIK UYARI] Enerji tukenmek uzere (< 20)!\n");
                }
                if (modulHasari > 70) {
                    printf("[KRITIK UYARI] Modul hasari cok yuksek (> 70)! Govde butunlugu riskte.\n");
                }
                if (su < 4) {
                    printf("[KRITIK UYARI] Su stogu kritik seviyede (< 4)!\n");
                }

                if (saglik >= 30 && enerji >= 20 && oksijen >= 25 && modulHasari <= 70 && su >= 4) {
                    printf(">> TUM SISTEMLER NORMAL.\n");
                }
                printf("======================================================\n");
                turHarca = 0;
                break;

            /* --- U: UYU / DINLEN --- */
            case 'u':
            case 'U':
                turHarca = 1;
                enerji += 25;
                oksijen -= 12;

                if (oksijen >= 20) {
                    saglik += 15;
                    printf("[DINLENME] Verimli bir uyku cekildi. +25 Enerji, +15 Saglik, -12 Oksijen.\n");
                } else {
                    saglik -= 10;
                    printf("[DINLENME - HIPOKSI] Oksijen 20'nin altinda oldugu icin bogulma tehlikesi yasandi! +25 Enerji, -10 Saglik, -12 Oksijen.\n");
                }
                break;

            /* --- K: KUM FIRTINASI SIMULASYONU --- */
            case 'k':
            case 'K':
                if (enerji < 10) {
                    printf("[IPTAL] Firtina kalkanlarini yonetmek icin en az 10 Enerji gerekli!\n");
                } else {
                    int t;
                    int hasarAlinanTur = 0;
                    int toplamSaglikKaybi = 0;
                    int toplamOksijenKaybi = 0;

                    enerji -= 10;
                    turHarca = 1;

                    printf("\n>>> KUM FIRTINASI BASLADI! (Sure: %d Tur) <<<\n", firtinaSuresi);

                    for (t = 1; t <= firtinaSuresi; t++) {
                        int siddetCarpani = (t % 2 == 0) ? 2 : 1;
                        int firtinaZar = rand() % 100;
                        int firtinaEsik = 55;
                        int oxiKayip, mHasar, canKayip;

                        printf("  [Tur %d/%d%s] Zar: %d / Esik: %d -> ",
                               t, firtinaSuresi, (siddetCarpani == 2) ? " - SIDDETLI RUZGAR 2x" : "",
                               firtinaZar, firtinaEsik);

                        if (firtinaZar >= firtinaEsik) {
                            printf("BASARILI (Firtinadan zararsiz korundunuz)\n");
                        } else {
                            printf("BASARISIZ -> ");
                            if (firtinaZar < 18) {
                                if (kilitMuhurlu == 1) {
                                    printf("Hava kilidi muhurlu! Oksijen sizintisi engellendi (Atlandi).\n");
                                    continue;
                                }
                                oxiKayip = 8 * siddetCarpani * firtinaHasarCarpani;
                                oksijen -= oxiKayip;
                                toplamOksijenKaybi += oxiKayip;
                                hasarAlinanTur++;
                                printf("Oksijen sizintisi! -%d Oksijen\n", oxiKayip);
                            } else if (firtinaZar < 38) {
                                mHasar = 7 * siddetCarpani * firtinaHasarCarpani;
                                modulHasari += mHasar;
                                hasarAlinanTur++;
                                printf("Govdeye kaya carpti! +%d Modul Hasari\n", mHasar);
                            } else {
                                canKayip = 6 * siddetCarpani * firtinaHasarCarpani;
                                saglik -= canKayip;
                                toplamSaglikKaybi += canKayip;
                                hasarAlinanTur++;
                                printf("Basinc dalgalanmasi yaralanmaya yol acti! -%d Saglik\n", canKayip);
                            }
                        }

                        if (saglik <= 0 || oksijen <= 0) {
                            printf("  [KRITIK] Yasam belirtileri tukendi! Firtina simulasyonu kesiliyor...\n");
                            break;
                        }
                    }

                    printf(">>> FIRTINA OZETI: Hasar Alinan Tur: %d | Kaybedilen Saglik: %d | Kaybedilen Oksijen: %d <<<\n",
                           hasarAlinanTur, toplamSaglikKaybi, toplamOksijenKaybi);
                }
                break;

            /* --- H: HAVA KILIDI SIFRESI --- */
            case 'h':
            case 'H':
                if (kilitMuhurlu == 1) {
                    printf("[BILGI] Hava kilidi zaten muhurlu durumda!\n");
                } else if (enerji < 5) {
                    printf("[IPTAL] Kilit terminalini acmak icin en az 5 Enerji gerekli!\n");
                } else {
                    char hedefSifre = 'A' + (gun % 26);
                    char tahmin;
                    int kalanHak = 3;
                    int sifreCozuldu = 0;

                    turHarca = 1;
                    printf("\n[HAVA KILIDI TERMINALI] Guvenlik sifresini girin (3 Deneme Hakki).\n");
                    printf("IPUCU: Sifre, Ingiliz alfabesinde 'A' harfinden bugunun gun sayisi (%d) kadar ilerideki harftir.\n", gun);

                    do {
                        printf("Tahmininiz (Kalan Hak: %d): ", kalanHak);
                        scanf(" %c", &tahmin);

                        if (tahmin >= 'a' && tahmin <= 'z') {
                            tahmin = tahmin - ('a' - 'A');
                        }

                        if (tahmin == hedefSifre) {
                            sifreCozuldu = 1;
                            kilitMuhurlu = 1;
                            printf(">> [BASARILI] Sifre dogrulandi! Hava kilidi muhurlendi.\n");
                        } else {
                            kalanHak--;
                            enerji -= 5;
                            printf(">> [HATALI SIFRE] -5 Enerji. ");
                            if (kalanHak > 0) {
                                if (tahmin < hedefSifre) {
                                    printf("Ipucu: Alfabede ILERI git.\n");
                                } else {
                                    printf("Ipucu: Alfabede GERI git.\n");
                                }
                            }
                        }
                    } while (!sifreCozuldu && kalanHak > 0);

                    if (!sifreCozuldu) {
                        modulHasari += 20;
                        printf("[ALARM] 3 deneme basarisiz! Guvenlik kilitlendi ve patlama yasandi: +20 Modul Hasari.\n");
                    }
                }
                break;

            /* --- Q: CIKIS --- */
            case 'q':
            case 'Q':
                printf("\n[SISTEM] Simulasyon kullanici istegiyle sonlandiriliyor...\n");
                oyunDevam = 0;
                cikisSebebi = 1;
                break;

            /* --- TANIMSIZ KOMUT --- */
            default:
                printf("[UYARI] Tanimsiz komut '%c'! Lutfen gecerli bir komut girin (Tur harcanmadi).\n", komut);
                turHarca = 0;
                break;
        }

        /* 3. SINIRLANDIRMA KONTROLLERI */
        if (saglik < 0) saglik = 0;
        if (saglik > 100) saglik = 100;

        if (enerji < 0) enerji = 0;
        if (enerji > 100) enerji = 100;

        if (oksijen < 0) oksijen = 0;
        if (oksijen > 100) oksijen = 100;

        if (modulHasari < 0) modulHasari = 0;
        if (modulHasari > 100) modulHasari = 100;

        /* 4. ZAMAN AKISI VE GUN GECISI */
        if (turHarca == 1 && oyunDevam == 1) {
            tur++;
            if (tur % 3 == 0) {
                gun++;
                oksijen -= 8;
                printf("\n*** YENI GUN BASLADI (GUN %d) *** Gece boyunca -8 Oksijen tuketildi.\n", gun);

                if (modulHasari > 70) {
                    saglik -= 5;
                    printf("*** [UYARI] Yuksek modul hasari (>70) nedeniyle gece -5 Saglik kaybi yasandi! ***\n");
                }

                if (oksijen < 0) oksijen = 0;
                if (saglik < 0) saglik = 0;
            }
        }

        /* 5. OYUN SONU KOSULLARI KONTROLU */
        if (oyunDevam == 1) {
            if (saglik <= 0 || oksijen <= 0) {
                oyunDevam = 0;
                cikisSebebi = 2;
            } else if (gun > 15) {
                gun = 15;
                oyunDevam = 0;
                cikisSebebi = 3;
            }
        }

    } while (oyunDevam == 1);

    /* 6. OYUN SONU RAPORU VE PUAN HESAPLAMA */
    puan = (gun * 100) + saglik + oksijen - modulHasari;
    if (puan < 0) puan = 0;

    printf("\n====================================================\n");
    printf("                SIMULASYON SONUC RAPORU             \n");
    printf("====================================================\n");
    if (cikisSebebi == 3) {
        printf("DURUM: TEBRIKLER! 15 gun hayatta kaldiniz ve kurtarma ekibi ulasti!\n");
    } else if (cikisSebebi == 2) {
        printf("DURUM: KAYBETTINIZ! Yasam destek sistemleri coktu (%s tukendi).\n",
               (saglik <= 0) ? "Saglik" : "Oksijen");
    } else {
        printf("DURUM: Simulasyondan cikis yapildi.\n");
    }
    printf("----------------------------------------------------\n");
    printf("Hayatta Kalinan Gun : %d\n", gun);
    printf("Toplam Gecerli Tur  : %d\n", tur);
    printf("Son Saglik          : %d\n", saglik);
    printf("Son Enerji          : %d\n", enerji);
    printf("Son Oksijen         : %d\n", oksijen);
    printf("Son Su / Parca      : %d birim / %d adet\n", su, yedekParca);
    printf("Son Modul Hasari    : %d\n", modulHasari);
    printf("----------------------------------------------------\n");
    printf("TOPLAM PUAN         : %d\n", puan);
    printf("====================================================\n");

    return 0;
}
