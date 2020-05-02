# SoalShiftSISOP20_modul4_A05

**Intan Kusuma R  (05111840000020)**

**Rafid Ferdianto (05111840000032)**

Source code: [ssfs.c](https://github.com/Raferto/SoalShiftSISOP20_modul4_A05/blob/master/ssfs.c)
## Soal 1
* Jika sebuah direktori dibuat dengan awalan “encv1_”, maka direktori tersebut akan menjadi direktori terenkripsi menggunakan metode enkripsi v1. 
* Jika sebuah direktori di-rename dengan awalan “encv1_”, maka direktori tersebut akan menjadi direktori terenkripsi menggunakan metode enkripsi v1. 
* Apabila sebuah direktori terenkripsi di-rename menjadi tidak terenkripsi, maka isi adirektori tersebut akan terdekrip. 
* Setiap pembuatan direktori terenkripsi baru (mkdir ataupun rename) akan tercatat ke sebuah database/log berupa file. 
* Dalam penamaan file ‘/’ diabaikan, dan ekstensi tidak perlu di encrypt. 
* Metode enkripsi pada suatu direktori juga berlaku kedalam direktori lainnya yang ada didalamnya.
* Semua file yang berada dalam direktori ter enkripsi menggunakan caesar cipher dengan key.

  ```9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO```

* Fungsi `getExt`untuk mendapatkan ekstensi dari file
```c
char *getExt (char *str) {
  char *ext = strrchr (str, '.');
  if (ext == NULL)
    ext = "";
  return ext;
}
```
* Fungsi `encrypt1` untuk melakukan enkripsi
```c
void encrypt1(char *enc){
  if(!strcmp(enc,".") || !strcmp(enc,"..")) 
    return;
  int len=strlen(getExt(enc));
  for ( int i = 0; i < strlen(enc)-len ;i++) {
    if(enc[i] != '/'){
      for (int j = 0; j < strlen(code); j++) {
        if(enc[i] == code[j]) {
          enc[i] = code[(j+key) % strlen(code)];
          break;
        }
      }
    }
  }
}
```
* Fungsi `decrypt1` untuk melakukan dekripsi
```c
void decrypt1(char *dec){
  if(!strcmp(dec,".") || !strcmp(dec,"..")) 
    return;
  int f1=0;
  int f2=0;
  if(strncmp(dec,"encv1_",6)==0){
    f1 = 1;
    f2 = 1;
  }
  int len=strlen(getExt(dec));
  for ( int i = 0; i < strlen(dec)-len ;i++) {
    if(dec[i] == '/'){
      f1=0;
    }
    if((f1!=1 && dec[i] != '/') && f2==1){
      for (int j = 0; j < strlen(code); j++) {
        if(dec[i] == code[j]) {
          dec[i] = code[(j+strlen(code)-key) % strlen(code)];
          break;
        }
      }
    }
  }
}
```
* Belum mengerjakan no 1d, setiap pembuatan direktori terenkripsi baru (mkdir ataupun rename) akan tercatat ke sebuah database/log berupa file.

**Kendala**

* Kurang memahami fungsi apa saja yang harus dipakai untuk menyelesaikan soal

**Screenshot Hasil**

![1](https://user-images.githubusercontent.com/61036923/80866195-a4340d80-8cb7-11ea-8575-6ee20db42107.png)
![2](https://user-images.githubusercontent.com/61036923/80866196-a4cca400-8cb7-11ea-82ba-447c32d00d6b.png)
![3](https://user-images.githubusercontent.com/61036923/80866197-a5653a80-8cb7-11ea-9f52-9f7e4fe80541.png)

Setelah rename `encv1_folder` menjadi `folder`

![4](https://user-images.githubusercontent.com/61036923/80866199-a5653a80-8cb7-11ea-99a6-8098b897527f.png)
![5](https://user-images.githubusercontent.com/61036923/80866190-a1d1b380-8cb7-11ea-8630-feca24cc80f9.png)
![6](https://user-images.githubusercontent.com/61036923/80866193-a39b7700-8cb7-11ea-80db-6d7941489c99.png)
