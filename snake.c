#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define update() printf("\033[H\033[J") //wizard shit
#define gotoxy(x,y ) printf("\033[%d;%dH", y, x)	//top banner takes 2 lines. when calling, make sure to add 2 to second parameter


int main(void){
	char board[] = " 1                    S N A K E                             60\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\nX                                                            X 1\nX                                                            X\nX                                                            X\nX                                                            X\nX                                                            X\nX                                                            X\nX                                                            X\nX                                                            X\nX                                                            X\nX                                                            X 10\nX                                                            X\nX                                                            X\nX                                                            X\nX                                                            X\nX                                                            X\nX                                                            X\nX                                                            X\nX                                                            X\nX                                                            X\nX                                                            X 20\nX                                                            X\nX                                                            X\nX                                                            X\nX                                                            X\nX                                                            X\nX                                                            X\nX                                                            X\nX                                                            X\nX                                                            X\nX                                                            X 30\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n";
	char death[] = "\n   X  X   XXX     XXX  XXXX  XX   XXX\n   X  X   X  X    X  X X    X  X  X  X\n   X  X   XXX     X  X XXX  XXXX  X  X\n   X  X   X X     X  X X    X  X  X  X\n    XX    X  X    XXX  XXXX X  X  XXX\n";
	int i, xpos, ypos, xval, yval;
	int last_velocity;
	int score, length;
	int xfood, yfood, foodeaten;
	FILE *fpx, *fpy;
	//set initial position to be in center of screen
	xpos = 31;
	ypos = 15;
	score = 0;
	length = 1;
	foodeaten = 1; //starts at 1 to trigger generation

	//SNAKE BODY

	int snakex[1000]; //perhaps this is bad programming practice... but I don't expect anyone to get a score of 100 or more. Also I dont want to malloc
	int snakey[1000]; //these should be initialized to zero
	int snakegrow[1000];
	snakegrow[0] = 1;
	last_velocity = 2;




	//INITIAL TILT:
	int initx;
	int inity;

	fpx = fopen("/sys/bus/iio/devices/iio:device0/in_voltage0_raw","r");
	if(fscanf(fpx,"%d", &initx)){
		fclose(fpx);
	}
	else{
		fclose(fpx);
	}

	fpy = fopen("/sys/bus/iio/devices/iio:device0/in_voltage1_raw","r");
	if(fscanf(fpy,"%d", &inity)){
		fclose(fpy);
	}
	else{
		fclose(fpy);
	}



	do{
		//GET X AND Y VELOCITY

		fpx = fopen("/sys/bus/iio/devices/iio:device0/in_voltage0_raw","r");
		if(fscanf(fpx,"%d", &xval)){
			//calculate xvelo
			if(xval <initx - 200 && last_velocity != 2) {last_velocity =1;}
			else if(xval > initx + 200 && last_velocity != 1) {last_velocity =2;}
			//this would look better if i cared more

			fclose(fpx);
		}
		else{
			fclose(fpx);
		}
		fpy = fopen("/sys/bus/iio/devices/iio:device0/in_voltage1_raw","r");
		if(fscanf(fpy,"%d",&yval)){
			//calculate yvelo
			if(yval < inity - 200 && last_velocity != 4) {last_velocity =3;}
			else if(yval > inity + 200 && last_velocity != 3) {last_velocity =4;}

			fclose(fpy);
		}
		else{
			fclose(fpy);
		}

		//Last Velocity:
		//      
		//     4 
		//   1   2
		//     3
		//



		/*
		// AUTO SNAKE
		//optimize path towards food

		if(ypos < yfood && last_velocity != 3) {last_velocity = 4;}
		else if(ypos > yfood && last_velocity != 4) {last_velocity = 3;}
		else if(ypos == yfood && ((xpos < xfood && last_velocity == 1) || (xpos > xfood && last_velocity == 2))) {last_velocity = 3;}

		else if(xpos < xfood && last_velocity != 1) {last_velocity = 2;}
		else if(xpos > xfood && last_velocity != 2) {last_velocity = 1;}
		else if(xpos == xfood && ((ypos < yfood && last_velocity == 4) || (ypos > yfood && last_velocity == 3))) {last_velocity = 1;}


		//override last velocity if last velocity would have it kill itself
		for( i = 0; i < length -1; i++)
		{
			if(last_velocity == 1 && snakex[i] == xpos -1) { last_velocity = 4;}  //if going left kills itself go up
			if(last_velocity == 4 && snakey[i] == ypos +1) { last_velocity = 2;}  //if going up kills itself go right
			if(last_velocity == 2 && snakex[i] == xpos +1) { last_velocity = 1;}  //if going right kills itself go down
			if(last_velocity == 3 && snakey[i] == ypos -1) { last_velocity = 2;}  //if going down kills itself go left
		}

		*/







		//update head position:

		switch(last_velocity) {
		case 1:
			{
				xpos = xpos -1; //move left
				break;
			}
		case 2:
			{
				xpos = xpos +1; //move right
				break;
			}
		case 3:
			{
				ypos = ypos -1; //move down
				break;
			}
		case 4:
			{
				ypos = ypos +1; //move up
				break;
			}
		}

		//FOOD

		if(foodeaten == 1) {
			xfood = rand()%50+5;
			yfood = rand()%20+5;
			foodeaten = 0;
		} //food has been eaten, generate more food.


		if(xpos == xfood && ypos == yfood ){
			score ++;
			foodeaten = 1;

			//updating snake length
			length++;

			snakegrow[length-1] = 1; 
			//this will make it so that the array entries are nonzero, so the the program knows this part of the snake body exists now

		} //head of snake is at food. eat the food and generate more
		//Snake Array Update

		for (i = 200; i >= 0; i = i - 1){

			if(snakegrow[i] != 0)
			{		
				snakex[i] = snakex[i-1];
				snakey[i] = snakey[i-1];
			}
		}
		snakex[0] = xpos;
		snakey[0] = ypos;


		//DISPLAY WINDOW
		update();

		printf("%s", board);
		printf("\n\nSCORE:\t\t%d",score);
		//debugging print statemnets
		/*
		printf("\n\nSNAKE LENGTH: \t\t%d\n", length);
		printf("\nDebug: SnakeGrow: %d %d %d %d %d \n", snakegrow[0], snakegrow[1], snakegrow[2], snakegrow[3], snakegrow[4]);
		printf("\nDebug: Snakex : %d %d %d %d %d\n", snakex[0], snakex[1], snakex[2], snakex[3], snakex[4]);
		printf("\nDebug: Snakey : %d %d %d %d %d\n", snakey[0], snakey[1], snakey[2], snakey[3], snakey[4]);
		printf("\nxpos : %d\t ypos: %d\n", xpos, ypos);
		*/
		

		gotoxy(xfood, yfood+2);				//Display Food
		puts("F");

		//Display snake
		gotoxy(snakex[0], snakey[0]+2);
		puts("S");
		for (i = 1; i < length; i++){
			if (snakegrow[i] != 0){
				gotoxy(snakex[i], snakey[i]+2);
				puts("o"); 
			}
		}




		//DEATH SCREEN
		//if snake hits border
		if(xpos == 0 || xpos == 61 || ypos == 0 || ypos == 31)
		{
			//update();
			gotoxy(0, 60);
			printf("%s", death);
			printf("FINAL SCORE:\t%d\n", score);
			return 0;
		}

		//if snake eats itself
		for(i = 1; i < length; i++){
			if(snakex[0] == snakex[i] && snakey[0] == snakey[i])
			{
				//update();
				gotoxy(0,60);
				printf("%s",death);
				printf("FINAL SCORE:\t%d\n", score);
				return 0;
			}
		}


		usleep(100000);
		//END DISPLAY WINDOW
	} while(1);

}
