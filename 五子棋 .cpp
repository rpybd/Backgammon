#include <stdio.h>
#include <stdlib.h>

#define player1 1
#define player2 2
#define pc 3

int playerDropX, playerDropY;
int table[16][16];
typedef struct DIRCount{
	int Dir[8][2];
	int direction[8];
}dirCount;
dirCount DC={{{1,0},{1,1},{1,-1},{-1,0},{-1,1},{-1,-1},{0,1},{0,-1}},{0,1,2,3,4,5,6,7}};
typedef struct T{
	int a[3][10];
	char dir[8];
}TYPE; 
TYPE type={{{9,-1}, {109,99}, {29,19,129,119}}};

void drawBoard(){//绘制棋盘及棋局 
	printf("\n");
	printf("    A B C D E F G H I J K L M N O \n"); 
	for(int i = 1; i <= 15; i++)
	{	
		printf("%2d ", i);
		for(int j = 1; j <= 15; j++){
			switch(table[i][j]){
				case 0: printf("┼");break;
				case player1: printf("●");break;
				case player2: printf("○");break;
				case pc: printf("○");break;
			}	
		}
		printf("\n");
	}
	printf("══════════════════════════\n");
}
int victoryJudge(int p, int q, int Now){//胜利判断 
	int w=1,s=1,e=1,z=1,i;
	for(i=1;i<5;i++)
		if(q+i<16&&table[p][q+i]==Now)
			w++;
		else break;//上
	for(i=1;i<5;i++)
		if(q-i>0&&table[p][q-i]==Now)
			w++;
		else break;//下
	if(w>=5)
		return 1;

	for(i=1;i<5;i++)
		if(p+i<16&&table[p+i][q]==Now)
			s++;
		else break;//左
	for(i=1;i<5;i++)
		if(p-i>0&&table[p-i][q]==Now)
			s++;
	else break;//右
	if(s>=5)
		return 1;

	for(i=1;i<5;i++)
		if(p+i<16&&q+i<16&&table[p+i][q+i]==Now)
			e++;
		else break;//正斜
	for(i=1;i<5;i++)
		if(p-i>0&&q-i>0&&table[p-i][q-i]==Now)
			e++;
		else break;//正斜
	if(e>=5)
		return 1;

	for(i=1;i<5;i++)
		if(p+i<16&&q-i>0&&table[p+i][q-i]==Now)
			z++;
		else break;//反斜 
	for(i=1;i<5;i++)
		if(p-i>0&&q+i<16&&table[p-i][q+i]==Now)
			z++;
		else break;//反斜 
	if(z>=5)
		return 1;
		
	return 0;
}
void playerDrop(int now){//人类玩家落子并显示 
	int pX, pY;
	printf("玩家%d请落子：", now);
	scanf("%d %d", &pX, &pY);
	while(pX<1||pX>15||pY<1||pY>15||table[pX][pY]!=0){
		printf("棋子出界或所选位置已有棋子\n请落子：");
		scanf("%d %d", &pX, &pY);
	}	
	playerDropX=pX;
	playerDropY=pY;
	table[playerDropX][playerDropY]=now;
	system("cls");
	drawBoard();
}
int firstDrop(int dir,int pX,int pY) //返回空点pq在n方向上的棋型 
{	
	int k,m=0;
	if(pX<=0||pX>=14||pY<=0||pY>=14)
		k=-2;
	else{ 
		pX+=DC.Dir[dir][0];
		pY+=DC.Dir[dir][1];	
	}	
	switch(table[pY][pX]){	
		case pc:{
			while(table[pY][pX]==pc) {
				m++; 
				pX+=DC.Dir[dir][0];
				pY+=DC.Dir[dir][1];
				if(pX<0||pX>14||pY<0||pY>14) { 
					k=m+type.a[0][0]; 
					return k; 
				} 
			}	
			if(table[pY][pX]==0) 
				k=m+type.a[0][1]; 
			else 
				k=m+type.a[0][0]; 
		}break;	
		case player1:{	
			while(table[pY][pX]==player1) {
				m++;
				pX+=DC.Dir[dir][0];
				pY+=DC.Dir[dir][1]; 
				if(pX<0||pX>14||pY<0||pY>14) { 
					k=m+type.a[1][0]; 
					return k; 
				} 
			}	
			if(table[pY][pX]==0) 
				k=m+type.a[1][1]; 
			else 
				k=m+type.a[1][0]; 
		}break;
		case 0:{
			pX+=DC.Dir[dir][0];
			pY+=DC.Dir[dir][1];	
			if(pX<0||pX>14||pY<0||pY>14) { 
				k=-3; 
				return k; 
			}
			switch(table[pY][pX]){	
				case pc:{
					while(table[pY][pX]==pc) { 
						m++;
						pX+=DC.Dir[dir][0];
						pY+=DC.Dir[dir][1]; 
						if(pX<0||pX>14||pY<0||pY>14) { 
							k=m+type.a[2][0]; 
							return k; 
						}
					}	
					if(table[pY][pX]==0) k=m+type.a[2][1]; 	
					else k=m+type.a[2][0]; 
				}break;	
				case player1:{

					while(table[pY][pX]==player1) { 
						m++;
						pX+=DC.Dir[dir][0];
						pY+=DC.Dir[dir][1];
						if(pX<0||pX>14||pY<0||pY>14) { 
							k=m+type.a[2][2]; 
							return k; 
						} 
					}	
					if(table[pY][pX]==0) 
						k=m+type.a[2][3]; 
					else 
						k=m+type.a[2][2]; 
				}break;	
				case 0: k=-1; break; 
			}
		}break;	
	}	
	return k;
}
int countValue(int p,int q) //计算并返回pq点价值 
{	
	int n=1,k=0,k1,k2,temp; 
	int a[2][4][4]={40,400,3000,10000,6,10,600,10000,20,120,200,0,6,10,500,0,30,300,2500,5000,2,8,300,8000,26,160,0,0,4,20,300,0};		
	while(n!=5){	
		k1=firstDrop(n,p,q); n+=4;
		k2=firstDrop(n,p,q); n-=3;
		temp=(k1>k2?k1:k2);
		k1=(k1<=k2?k1:k2);
		k2=temp;
		typedef struct position{
			int x1,x2;
			int y1,y2;
			int z1,z2;
		}position;
		position p={((k1/10)/10)%10,((k1/10)/10)%10,(k1/10)%10,(k1/10)%10,k1%10,k2%10};	
		if(k1==-1) { 
			if(k2<0) { 
				k+=0; 
				continue; 
			} else 
				k+=a[p.x2][p.y2][p.z2]+5; 
			continue; 
		}; 
		if(k1==-2) { 
			if(k2<0) { 
				k+=0; 
				continue; 
			} else 
				k+=a[p.x2][p.y2][p.z2]/2; 
			continue; 
		}; 
		if(k1==-3) { 
			if(k2<0) { 
				k+=0; 
				continue; 
			} else 
				k+=a[p.x2][p.y2][p.z2]/3; 
			continue; 
		}; 
		if(((k1>-1&&k1<4)&&((k2>-1&&k2<4)||(k2>9&&k2<14)))||((k1>99&&k1<104)&&((k2>99&&k2<104)||(k2>109&&k2<114)))){	
			if(p.z1+p.z2>=2) { 
				k+=a[p.x2][p.y2][3]; 
				continue; 
			}else { 
				k+=a[p.x2][p.y2][p.z1+p.z2+1]; 
				continue; 
			}
		}	
		if(((k1>9&&k1<14)&&(k2>9&&k2<14))||((k1>109&&k1<114)&&(k2>109&&k2<114))){ 	
			if(p.z1+p.z2>=2) { 
				k+=10000; 
				continue; 
			}else { 
				k+=0; 
				continue; 
			}
		}	
		if(((k1>-1&&k1<4)&&((k2>99&&k2<104)||(k2>109&&k2<114)))||((k1>9&&k1<14)&&((k2>99&&k2<104)||(k2>109&&k2<114)))){	
			if(p.z1==3||p.z2==3) { 
				k+=10000; 
				continue; 
			}else { 
				k+=a[p.x2][p.y2][p.z2]+a[p.x1][p.y1][p.z1]/4; 
				continue; 
			}
		}else { 
			k+=a[p.x1][p.y1][p.z1]+a[p.x2][p.y2][p.z2]; 
			continue; 
		} 
	}	
	return k;
}
void pcDrop(){//电脑落子 
	int value=0;
	int pX, pY;
	for(int i=1; i<16; i++)
		for(int j=1; j<16; j++)
			if(table[i][j]!=player1&&table[i][j]!=pc){
				if(countValue(j, i)>value){//取最有利点落子 
					value=countValue(j, i);
					playerDropY=j; playerDropX=i;
				}
			}		
	table[playerDropX][playerDropY]=pc;	
	system("cls");
	drawBoard();		
}

int main(){ 
	int choice;
	begin:int victory=0;
	for(int i=0; i<15; i++)//初始化棋盘 
		for(int j=0; j<15; j++)
			table[i][j]=0;
	system("cls");
	drawBoard();
	printf("欢迎来到游戏\n");
	printf("请选择对弈模式\n1.人机对弈\t2.人人对弈:");
	scanf("%d", &choice);
	while(choice!=1&&choice!=2){
		printf("\n请选择正确的模式:");
		scanf("%d", &choice);
	}
	if(choice==1){//人机对弈 
		system("cls");
		drawBoard();
		while(victoryJudge(playerDropX, playerDropY, player1)!=2){		
			playerDrop(player1);
			printf("玩家落子在: %d %d\n", playerDropX, playerDropY);
			victory=victoryJudge(playerDropX, playerDropY, player1);
			if(victory==1){//玩家胜利 
				printf("Victory!\n");
				printf("再来一局？\n1.新的挑战\t2.结束");
				int tryagain;
				scanf("%d", &tryagain);
				while(tryagain!=1&&tryagain!=2){
					printf("\n请选择：");
					scanf("%d", &tryagain);
				}
				if(tryagain==1)
					goto begin;
				else
					return 0;
			}else {
				pcDrop();
				printf("电脑落子在: %d %d\n", playerDropX, playerDropY);
				victory=victoryJudge(playerDropX, playerDropY, pc);
				if(victory==1){//玩家失败，电脑胜利 
					printf("败北~\n");
					printf("再来一局？\n1.新的挑战\t2.结束");
					int tryagain;
					scanf("%d", &tryagain);
					while(tryagain!=1&&tryagain!=0){
						printf("\n请选择：");
						scanf("%d", &tryagain);
					}
					if(tryagain==1)
						goto begin;
					else
						return 0;
				}
			}
		}
	}else if(choice==2){//人人对弈 
		int i;
		printf("0.先手\t1.后手:");
		scanf("%d", &i);
		while(i!=1&&i!=0){
			printf("\n请选择先后手：");
			scanf("%d", &i);
		}
		system("cls");
		drawBoard();
		while(victory==0){		
			playerDrop(player1+i);//玩家1 
			printf("玩家%d落子在: %d %d\n", 1+i, playerDropX, playerDropY);
			victory=victoryJudge(playerDropX, playerDropY, player1+i);
			if(victory==1){//胜利 
				printf("玩家%d胜利！\n", 1+i);
				printf("再来一局？\n1.新的挑战\t2.结束");
				int tryagain;
				scanf("%d", &tryagain);
				while(tryagain!=1&&tryagain!=2){
					printf("\n请选择：");
					scanf("%d", &tryagain);
				}
				if(tryagain==1)
					goto begin;
				else
					return 0;
			}else {
				playerDrop(player2-i);//玩家2 
				printf("玩家%d落在在: %d %d\n", 2-i, playerDropX, playerDropY);
				victory=victoryJudge(playerDropX, playerDropY, player2-i);
				if(victory==1){//胜利 
					printf("玩家%d胜利！\n", 2-i);
					printf("再来一局？\n1.新的挑战\t2.结束");
					int tryagain;
					scanf("%d", &tryagain);
					while(tryagain!=1&&tryagain!=2){
						printf("\n请选择：");
						scanf("%d", &tryagain);
					}
					if(tryagain==1)
						goto begin;
					else
						return 0;
				}
			}
		}
	}
}

