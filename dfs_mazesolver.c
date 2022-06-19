#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
// beklemeturu'nu 1 yaparsaniz 1 saniye bekler, 0 yaparsaniz yaklasýk olarak girdiginiz ms kadar bekler (pc'den pc'ye fark görülebilir)
void bekle(int final){
	int beklemeturu = 0;
	int ms = 200;
	
	int i,j,k,m=200;
	ms=ms*10000;
	if(final==1){
	ms=2;
	m=2;	
	}
	
	if(beklemeturu == 1){
		sleep(1);
	}
	else{
	for(i=0;i<ms;i++){
	for(j=0;j<m;j++)
	k=0;		
	}
	}
}

// dosya okuma icin satir sayisini buluyoruz
int satirsay(char *dosyaadi,int* sutun){
	FILE *fp=fopen(dosyaadi,"r");
	int durum;
	int satir=0;
	if(fp==NULL){
	printf("dosya bulunamadi");}

	while(!feof(fp)){
		durum=fgetc(fp);
		if (durum=='\n'){
			satir++;
		}
		else{
// toplam eleman sayisi totalde tutuluyor				
			*sutun = *sutun + 1;
		}
	}
	satir++;
	fclose(fp);
// sutun ilk basta toplam eleman sayisini tutuyor sonra satira bolerek gercek degerini buluyoruz	
	*sutun = *sutun / satir;
	return satir;
}
// dosyadan labirenti okuyoruz
void Dosyaoku(char *dosyaadi,char **matris,int* baslangicSatiri, int* baslangicSutunu, int **matrisHareket){
	FILE *fp=fopen(dosyaadi,"r");
	char durum;
	int i=0,satir=0;
	if(fp==NULL){
	printf("hata olustu");}

	while(!feof(fp)){
		durum=fgetc(fp);
		printf("%c",durum);
		if (durum=='\n'){
// \n gelince yeni satýra geciyoruz			
			satir++;
			i = -1;
		}
		else{
// duruma göre gelen sembol için belirlenen sayilar matrisHareket'e kayýt ediliyor duvar 0 ,yol 1 ,bitis 9	
			matris[satir][i] = durum;
			if(matris[satir][i] == 'c')
			matrisHareket[satir][i] = 9;
			else if(matris[satir][i] == '-' || matris[satir][i] == '+' || matris[satir][i] == '|')
			matrisHareket[satir][i] = 0;
			else
			matrisHareket[satir][i] = 1;
// b harfi geldiginde koordinantlarini kayit ediyoruz			
			if(durum == 'b'){
				*baslangicSatiri = satir;
				*baslangicSutunu = i;
			}

		}
		i++;
	}
	fclose(fp);
}
// hatritayi ekrana yazdiriyoruz
void print_maze(int ToplamSatir, int ToplamSutun,char **maze,int **mazeHareket){
	int i, j;
	printf("\n");
	for (i = 0; i < ToplamSatir; ++i) {
		for (j = 0; j < ToplamSutun; ++j) {
// gezilen yerlere * diger yerlere gercek haritadaki sembolu koyuyoruz (mazeHareket[i][j]!=2 oranýn gezilmedigi anlamina geliyor)			
			if(mazeHareket[i][j]!=2 || maze[i][j] == 'b' || maze[i][j] == 'c')
			printf("%c", maze[i][j]);
			else
			printf("*");
		}
		printf("\n");
	}	
	printf("\n");
}
// yon veya bitme durumuna gore ozel print yapiyoruz
void print_score_gidilenYon(int SCORE, int yon, int final){
	if(final==1){
		printf("\n FINAL SKOR : %d",SCORE);
		printf("\n BITIS NOKTASINA ULASILDI \n");
	}
	else if(yon == 1){
			printf("\n toplam skor : %d",SCORE);
			printf("\n gidilen yon : KUZEY \n ");	
	}
	else if(yon == 2){
			printf("\n toplam skor : %d",SCORE);
			printf("\n gidilen yon : BATI \n ");	
	}
	else if(yon == 3){
			printf("\n toplam skor : %d",SCORE);
			printf("\n gidilen yon : DOGU \n ");	
	}
	else if(yon == 4){
			printf("\n toplam skor : %d",SCORE);
			printf("\n gidilen yon : GUNEY \n ");	
	}
	else{
			printf("\n toplam skor : %d",SCORE);
			printf("\n geri donuluyor \n ");
	}
}

// duvar 0, yol 1, gezildi 2, hedef 9, geri donuldu 3
int dfs(int satir, int sutun, int **mazeHareket, int toplamSatir, int toplamSutun, char **maze, int *SCORE, int *final){ // SCORE ekle // satir sutun baþlangýç noktasý
// 9 olmasi demek bitise ulastigimiz anlamina geliyor. 9 olma durumunu kontrol ediyoruz 4 farkli olasilik icinde
	if (mazeHareket[satir - 1][sutun] == 9 || mazeHareket[satir][sutun - 1] == 9 || mazeHareket[satir + 1][sutun] == 9 || mazeHareket[satir][sutun + 1] == 9) {
			mazeHareket[satir][sutun] = 2;
			bekle(*final);
			system ("cls");
			*SCORE = score_hesapla(satir,sutun,mazeHareket,*SCORE,maze);
			print_maze(toplamSatir,toplamSutun,maze,mazeHareket);
			print_score_gidilenYon(*SCORE,1,*final);
		*final = 1;
		return 1;
	}
// eger sonuca ulasilmissa hicbir islem yapma	
	if (*final != 1) {
		mazeHareket[satir][sutun] = 3;
		
// yukari yonlu hareket icin		
		if (mazeHareket[satir - 1][sutun] == 1){
// hareket edildi anlaminda 2 olarak isaretleniyor			
			mazeHareket[satir][sutun] = 2;
			bekle(*final);
			system ("cls");
// konumuna gore skoru hesaplaniyor (cýkmaz sokak veya elma toplama durumlari)			
			*SCORE = score_hesapla(satir,sutun,mazeHareket,*SCORE,maze);
// son durumlar ve harita ekrana yazdiriliyor			
			print_maze(toplamSatir,toplamSutun,maze,mazeHareket);
			print_score_gidilenYon(*SCORE,1,*final);
// yeni konumla beraber fonksiyon tekrar cagiriliyor (dfs kurali)			
			dfs(satir - 1, sutun,mazeHareket,toplamSatir,toplamSutun,maze,SCORE,final);
		}
// sola hareket		
		if (mazeHareket[satir][sutun - 1] == 1){
			mazeHareket[satir][sutun] = 2;
			bekle(*final);
			system ("cls");
// konumuna gore skoru hesaplaniyor (cýkmaz sokak veya elma toplama durumlari)				
			*SCORE = score_hesapla(satir,sutun,mazeHareket,*SCORE,maze);
// son durumlar ve harita ekrana yazdiriliyor			
			print_maze(toplamSatir,toplamSutun,maze,mazeHareket);
			print_score_gidilenYon(*SCORE,2,*final);
// yeni konumla beraber fonksiyon tekrar cagiriliyor (dfs kurali)			
			dfs(satir, sutun - 1,mazeHareket,toplamSatir,toplamSutun,maze,SCORE,final);
		} 		
// saga hareket		
		if (mazeHareket[satir][sutun + 1] == 1){
			mazeHareket[satir][sutun] = 2;
			bekle(*final);
			system ("cls");
// konumuna gore skoru hesaplaniyor (cýkmaz sokak veya elma toplama durumlari)				
			*SCORE = score_hesapla(satir,sutun,mazeHareket,*SCORE,maze);
// son durumlar ve harita ekrana yazdiriliyor			
			print_maze(toplamSatir,toplamSutun,maze,mazeHareket);
			print_score_gidilenYon(*SCORE,3,*final);
// yeni konumla beraber fonksiyon tekrar cagiriliyor (dfs kurali)			
			dfs(satir, sutun + 1,mazeHareket,toplamSatir,toplamSutun,maze,SCORE,final);		
		}
// asagi hareket		
		if (mazeHareket[satir + 1][sutun] == 1){
			mazeHareket[satir][sutun] = 2;
			bekle(*final);
			system ("cls");
// konumuna gore skoru hesaplaniyor (cýkmaz sokak veya elma toplama durumlari)				
			*SCORE = score_hesapla(satir,sutun,mazeHareket,*SCORE,maze);
// son durumlar ve harita ekrana yazdiriliyor			
			print_maze(toplamSatir,toplamSutun,maze,mazeHareket);
			print_score_gidilenYon(*SCORE,4,*final);
// yeni konumla beraber fonksiyon tekrar cagiriliyor (dfs kurali)			
			dfs(satir + 1, sutun,mazeHareket,toplamSatir,toplamSutun,maze,SCORE,final);
		} 			
	}
// eger son durumda degilsek ve hareket edilecek hic bir bosluk yoksa geri donuyoruz	
	if(*final != 1){
		//bekle(*final);
		system ("cls");
// bu kisim sadece gorsellik icin cikmaz yolun sonuna gidip sonra donuyor		
		mazeHareket[satir][sutun] = 2;
		print_maze(toplamSatir,toplamSutun,maze,mazeHareket);
		print_score_gidilenYon(*SCORE,5,*final);
		bekle(*final);
		system ("cls");
// geri dondugumuz icin o konum geri donuldu anlaminda 3 olarak isaretleniyor		
		mazeHareket[satir][sutun] = 3;
		*SCORE = score_hesapla(satir,sutun,mazeHareket,*SCORE,maze);
// son durumlar ve harita ekrana yazdiriliyor			
		print_maze(toplamSatir,toplamSutun,maze,mazeHareket);
		print_score_gidilenYon(*SCORE,5,*final);
// bu sefer fonksiyon cagirmiyoruz ki bir onceki konumdan devam edilsin		
	}
}

// puanlari hesapliyoruz elma ve cikmaz sokak icin
int score_hesapla(int satir, int sutun, int **mazeHareket,int SCORE,char **maze){ 
	int i=0;
// ilk ve son durumda ne olursa olsun puan yok	
	if(maze[satir][sutun]!='b' && maze[satir][sutun]!='c'){

		if (mazeHareket[satir][sutun - 1] == 0){
		i++;
		} 
		if (mazeHareket[satir + 1][sutun] == 0){
			i++;
		} 
		if (mazeHareket[satir - 1][sutun] == 0){
			i++;
		}
		if (mazeHareket[satir][sutun + 1] == 0){
			i++;	
		}
	}	
// eger bulundugumuz konumun 3 tarafi duvarlarla kapli ise orasi cikmaz sokaktir	
	if(i>2)
	SCORE = SCORE - 5;
// elma bulduysak puan kazaniyoruz ve elmayi haritadan siliyoruz	
	if(maze[satir][sutun]== 'O'){
	SCORE = SCORE + 10;	
	maze[satir][sutun] = ' ';		
	}
	return SCORE;
}

int main(int argc, char *argv[]) {
char tmp[32];
int i,N,satir,sutun=0,baslangicSatiri,baslangicSutunu,SCORE=0,final=0; 

printf("\n labirentin bulundugu dosyanin adini giriniz (.txt dahil) \n"); 
scanf("%s",tmp);

satir=satirsay(tmp,&sutun);
// sayilardan olusan hareket labirenti ile sembollerden olusan ana labirenti tutmak icin yer ayiriyoruz
char **maze = (char **)malloc((satir + 1) * sizeof(char*));
int **mazeHareket = (int **)malloc((satir + 1) * sizeof(int*));
for(i=0; i<(satir + 1); i++){
    maze[i] = (char *)malloc((sutun + 1) * sizeof(char));
    mazeHareket[i] = (int *)malloc((sutun + 1) * sizeof(int));
}
Dosyaoku(tmp,maze,&baslangicSatiri,&baslangicSutunu,mazeHareket);
system ("cls");

printf("labirentin boyutlari en: %d boy: %d",sutun,satir);
printf("\n\n                          LABIRENT \n \n");
print_maze(satir, sutun,maze,mazeHareket);
printf("\n Baslamak icin herhangi bir sayi giriniz : ");
scanf("%d",&N);
// dfs basliyor ve sonuca ulasilana kadar donuyor
dfs(baslangicSatiri,baslangicSutunu,mazeHareket,satir,sutun,maze,&SCORE,&final);

    free(mazeHareket);
    free(maze);
	return 0;
} 
// sizin verdiðiniz labirentten farklý olarak 9 kere çýkmaz sokaða girerek bitiþe ulaþan benim güncellediðim yenilenmiþ labirent örneði
/*
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|     |     |                             |
+ +-+ + +-+-+ +-+-+-+-+-+-+-+-+-+ +-+-+-+-+
|   |   |     |   |   |         |   |     |
+-+ + +-+ +-+-+ + + + + +-+-+-+ +-+ + +-+-+
|   | |   |     |   | |     | | |       | |
+ +-+ + +-+ +-+-+-+-+-+-+ +-+-+-+-+-+-+ +-+
| |   | | | |         | | |   |     | |   |
+ +-+-+ +-+ + +-+-+-+ + + + + +-+ + +-+-+ +
|     | |   |   |   | | |   |   | |       |
+-+-+ + + +-+-+ +-+ + +-+-+-+-+ + +-+-+-+-+
| |   | |     |   | |         | |       | |
+ + +-+ +-+-+-+-+ + +-+-+-+-+ + +-+ +-+ +-+
| | |             |         | |   | | |   |
+ + + +-+-+-+-+-+-+-+ +-+-+ + +-+ + + +-+ +
| |     |   |             | |   | |   |   |
+ +-+-+ + + + +-+ +-+-+ +-+-+ + + +-+ + +-+
|       | | |   |     | |     | | | | |   |
+ + +-+-+ + +-+ + +-+-+-+ +-+-+-+ + + +-+ +
| | |     |   | |   |   |       | |   | | |
+ + + +-+-+-+-+ +-+ + +-+-+-+-+ + + +-+-+ +
| |               | |   | | |   |     | | |
+ +-+-+-+-+-+-+-+-+-+-+ +-+-+ +-+-+-+ +-+ +
|       |   | |     | | | |         | |   |
+-+-+-+ + + + + +-+ +-+ +-+ +-+ +-+-+ + +-+
|   |   | | | | | |     | | |   |   | |   |
+ +-+ +-+ + + + +-+-+-+-+-+ + +-+ + + +-+-+
|       | | | |           | | |   | |     |
+ +-+-+ +-+ + +-+-+-+-+-+ + + +-+-+-+-+-+ +
|b          |           |   |         |  c|
+-+-+-+-+-+-+---+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
