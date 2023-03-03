//============================================================================
// Name        : Tetris.cpp
// Author      : Sibt ul Hussain
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game of Tetris...
//============================================================================
#ifndef TETRIS_CPP_
#define TETRIS_CPP_
#include "util.h"
#include <iostream>
#include<vector>
#include<algorithm>
#include<cstdlib>
#include<ctime>
#include<string>
#include<sys/wait.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sstream>
#include<cmath>      
using namespace std;


int fruita[5][2], snek[65][2], sky[65][65], hurdle[2], size=3, direct=1,hurdle2[2];
int time_passed, score=0, apple, level=1, hurdletime,special=0,sfruita[1][2],hurdletime2;
bool specialf;

void SetCanvasSize(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1); 
    glMatrixMode( GL_MODELVIEW);
    glLoadIdentity();
}
//Generate food

void fruit5()
{	
	if(time(NULL)-time_passed>=15)
	{	bool flag;
		for (int x=0;x<5;x++)
		{	
			fruita[x][0]=(rand()%63);
			fruita[x][1]=(rand()%63);
			do
			{
				if (sky[fruita[x][0]][fruita[x][1]] > 0)
				{
					flag=false;
					fruita[x][0]=(rand()%63);
					fruita[x][1]=(rand()%63);
				}
				else
					flag=true;
			} while (flag==false);
		}
		special++;
		if(special%2==0)
		{	
			sfruita[0][0]=(rand()%63);
			sfruita[0][1]=(rand()%63);
			specialf=true;	
		}
		time_passed=time(NULL);
	} 
}

//Don't need this function. 
	
/*void specialfruit()
{		
			sfruita[0][0]=(rand()%63);
			sfruita[0][1]=(rand()%63);
}*/


		
void fruit(int x)
{
	fruita[x][0]=(rand()%63);
	fruita[x][1]=(rand()%63);
}

void hurdles()
{
	if(time(NULL)-hurdletime==30)
	{
		//srand(time(NULL));
		hurdle[0]=rand()%50;
		hurdle[1]=rand()%50;
		hurdle2[0]=rand()%50;
		hurdle2[1]=rand()%50;
		hurdletime=time(NULL);
	}
}

/*void hurdles2()
{
	if(time(NULL)-hurdletime==30)
	{
		//srand(time(NULL));
		hurdle2[0]=rand()%50;
		hurdle2[1]=rand()%50;
		hurdletime2=time(NULL);
	}
}*/


void skyfunc()
{ 	
	for (int x=0;x<65;x++)
	{	
		for (int y=0;y<65;y++)
		{
			sky[x][y]=0;
		}
	}
	for (int x=0;x<size;x++)
	{
		sky[snek[x][0]][snek[x][1]]=1;
	}
	for (int x=0;x<5;x++)
	{
		sky[fruita[x][0]][fruita[x][1]]=2;
	}
	for(int x=0;x<10;x++)
	{
		sky[hurdle[0]+x][hurdle[1]]=3;
		sky[hurdle[0]][hurdle[1]+x]=3;
	}
	for(int x=0;x<10;x++)
	{
		sky[hurdle2[0]+x][hurdle2[1]]=3;
		sky[hurdle2[0]][hurdle2[1]+x]=3;
	}
	if (specialf==true)
		sky[sfruita[0][0]][sfruita[0][1]]=4;
}

void scorelines()
{
	DrawLine( 3*10,  62*10,  23*10,  62*10,  15*10,  colors[130]);
	DrawLine( 3*10,  62*10,  30+score*2,  62*10,  10*10, colors[33]);
	DrawString(500, 610, "Score:",colors[30]);
	DrawString(580, 610, to_string(score),colors[30]);	
	DrawString(310, 610, "Level:",colors[30]);
	DrawString(380, 610, to_string(level),colors[30]);
}

void drawingoncanvas()						
{	for (int x=0;x<65;x++)
	{	for(int y=0;y<65;y++)
		{	
			if (sky[x][y]==0)
			DrawSquare(x*10,y*10,11,colors[133]);
			if (sky[x][y]==1)
			DrawSquare(x*10,y*10,10,colors[70]);
			if (sky[x][y]==2)
			DrawSquare(x*10,y*10,10,colors[4]);
			if (sky[x][y]==3)
			DrawSquare(sky[x][y], sky[x][y], 10, colors[130]);
			if(special%2==0)
			{ 
				if (sky[x][y]==4)
				{DrawSquare(sky[x][y], sky[x][y], 10, colors[68]);}}
		}
	}
	DrawCircle(snek[0][0]*10+5 , snek[0][1]*10+5 , 6 , colors[5]);				
}

void border()
{	for (int x=0;x<65;x++)
	{	for(int y=0;y<65;y++)
		{	if ((y==0) or (y==64) or (x==0) or (x==64))
			{DrawSquare(x*10,y*10,11,colors[130]);}
		}
	}
}

void collisionwithhurdle()
{	
	if (sky[snek[0][0]][snek[0][1]]==3)
	exit(1);
}

void collisionwithbody()
{	
	if (sky[snek[0][0]][snek[0][1]]==1)
	exit(1);
}

void movingbody()
{	
	for (int x=size-1;x>0;x--)
	{	
		snek[x][0]=snek[x-1][0];
		snek[x][1]=snek[x-1][1];
	}
}

void movinghead()
{
	switch(direct)
	{
		case 1: snek[0][0]-=1;break;
		case 2: snek[0][0]+=1;break;
		case 3: snek[0][1]+=1;break;
		case 4: snek[0][1]-=1;break;
	}
}

void increasinglength ()
{
		score+=5;size++;
		if(direct==1)
		{
		snek[size-1][0]=snek[size-2][0]+1;
		snek[size-1][1]=snek[size-2][1];
		}
		if(direct==2)
		{
		snek[size-1][0]=snek[size-2][0]-1;
		snek[size-1][1]=snek[size-2][1];
		}
		if(direct==3)
		{
		snek[size-1][0]=snek[size-2][0]-1;
		snek[size-1][1]=snek[size-2][1];
		}
		if(direct==4)
		{
		snek[size-1][0]=snek[size-2][0]+1;
		snek[size-1][1]=snek[size-2][1];
		}
}

void eatingfood()
{	
	if (sky[snek[0][0]][snek[0][1]]==2)
	{	for (int x=0;x<5;x++)
		{
			if (snek[0][0]==fruita[x][0] and snek[0][1]==fruita[x][1])
				apple=x;
		}
		
	fruit(apple);
	increasinglength();
	}
	if(sky[snek[0][0]][snek[0][1]]==4)
	{
		score=score+15;
		increasinglength();
		specialf=false;
		/*specialfruit();
		special=0;*/
	}
}

void checkingedges()
{
	if (snek[0][0]<1)
	{	exit(1);}
	if (snek[0][0]>63)
	{	exit(1);}
	if (snek[0][1]<1)
	{	exit(1);}
	if (snek[0][1]>63)
	{	;}
}

void Display()
{
	if (score<=100)
	{glClearColor(0, 0.0, 0.0, 0);
	glClear(GL_COLOR_BUFFER_BIT); 
	
	fruit5();
	skyfunc();
	//fruit5();
	//specialfruit();
	hurdles();
	movingbody();
	movinghead();
	eatingfood();
	checkingedges();
	collisionwithhurdle();
	collisionwithbody();
	drawingoncanvas();
	scorelines();
	border();

   glutSwapBuffers(); }
else {exit(1);}
}

void inisnek()
{
	for (int x=0;x<size;x++)
	{	
		snek[x][0]=32+1*x;
		snek[x][1]=32;
	}
}

void NonPrintableKeys(int key, int x, int y) 
{
	if (key == GLUT_KEY_LEFT)  
	{	if(direct!=2)
		{direct=1;} 
	}
	else if (key == GLUT_KEY_RIGHT)
	{	if(direct!=1)
		{direct=2;} 
	}
	else if (key == GLUT_KEY_UP) 
	{	if(direct!=4)
		{direct=3;} 
	}
	else if (key == GLUT_KEY_DOWN) 
	{	if(direct!=3)
		{direct=4;} 
	}
    
     glutPostRedisplay();

}

/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
 * is pressed from the keyboard
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 * */


void PrintableKeys(unsigned char key, int x, int y) {
    if (key == KEY_ESC/* Escape key ASCII*/) {
        exit(1); // exit the program when escape key is pressed.
    }
    if (key == 'R' || key=='r'/* Escape key ASCII*/) {
        //exit(1); // exit the program when escape key is pressed.
    	//aswangle+=90;
    }
    
    else if (int(key) == 13)
    {  
	}
    
    glutPostRedisplay();
}

/*
 * This function is called after every 1000.0/FPS milliseconds
 * (FPS is defined on in the beginning).
 * You can use this function to animate objects and control the
 * speed of different moving objects by varying the constant FPS.
 *
 * */

void Timer(int m) {

// implement your functionality here
	glutPostRedisplay();
// once again we tell the library to call our Timer function after next 1000/FPS
    glutTimerFunc(500.0 / FPS, Timer, 0);
}

/*
 * our gateway main function
 * */
int main(int argc, char*argv[]) {
    int width = 650, height = 650; // i have set my window size to be 800 x 600
    InitRandomizer(); // seed the random number generator...
    glutInit(&argc, argv); // initialize the graphics library...

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
    glutInitWindowPosition(50, 50); // set the initial position of our window
    glutInitWindowSize(width, height); // set the size of our window
    glutCreateWindow("PF's Snake Game"); // set the title of our game window
    SetCanvasSize(width, height); // set the number of pixels...

	inisnek();
	skyfunc();
	time_passed=time(NULL)-15;
	hurdletime=time(NULL)-30;




// Register your functions to the library,
// you are telling the library names of function to call for different tasks.
//glutDisplayFunc(display); // tell library which function to call for drawing Canvas.
    glutDisplayFunc(Display); // tell library which function to call for drawing Canvas.
    glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
    glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
    glutTimerFunc(5.0 / FPS, Timer, 0);

// now handle the control to library and it will call our registered functions when
// it deems necessary...
    glutMainLoop();
    return 1;
}
#endif /* Snake Game */

