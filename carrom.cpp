/*This file contains the main carrom game .
It includes the header finle class.h where the classes are defined and the implimentations are in the file class.cpp

Author : Akhil 
*/

using namespace std;

#include <GL/glut.h>
#include <math.h>
#include <string.h>
#include <sstream>
#include <ctime>
#include "class.h"

using namespace std;

///////////////////////// Necessary Constraints /////////////////////////

#define PI 3.1415926535897932384626433832795
#define alpha 0.96 //coefficient for restitution. inelastic collision
#define constraint 0.00003 //minimum velocity
#define mew 0.3 // coefficient of friction
#define gravity 0.00009 // gravity variable used in friction calculation

///////////////////////// global variables  in the game /////////////////////////

float updateRate = 10;
float power = 0.000;
float theta = 0;
bool isSpaceKeyPressed = false;
bool isStrikerInPosition = true;
bool firstStrike = false;
float score = 0;
float fin1x = -2.8 + 0.1614 , fin2x = -2.8 + 0.1614 ,finy = 2.6;
float coinsAtRest = 0;


Coin coinArray[8];
Player player1, player2;
Board board;

void drawBoard();
void initGame();
void update(int);
void execute();
void drawQuadd(float , float , float , float , float , float ,  float , float );
bool physics(Coin& , Coin& , float );//collision implimentation
void reduceVelocity(Coin& );
void checkFriction();
bool isCollide(Coin& , Coin& );
void checkHole(Coin&);
void checkCollision();
void drawScore();
void drawControl();
void updatePosition(Coin& );
void updateTime(int );
void checkLane();
void drawCoins();



//Called when a key is pressed
void handleKeypress(unsigned char key, int x, int y) {

	switch (key) {
		case 'a':
			if(!(theta < -1.57))
			theta -= 0.03;			// decrease rotation by 5 degrees
			break;
    	case 'c':
			if(!(theta > 1.57))
			theta += 0.03;			// increase rotation by 5 degrees
			break;
		case 32 : //  space
			isSpaceKeyPressed = true;
			execute();
			break;	
		case 27: //Escape key

			exit(0);

	}

}


void handleSpecialKeypress(int key, int x, int y) {
 
 	switch (key) {
 		case GLUT_KEY_UP:
 		if(player1.getTurnFlag()){
			if(!(power > 0.2))
			power += 0.006656;
			}
			else{
			if(!(power < -0.2))
			power -= 0.006656;
				
			}			// increase power
			break;

    	case GLUT_KEY_DOWN:
    	if(player1.getTurnFlag()){
    		if(!(power < 0.0))
			power -= 0.006656;	
			}
		else{
			if(!(power > 0.0))
			power += 0.006656;	
			
		}			// decrease power 
			break;
		case GLUT_KEY_LEFT:
  			if (isStrikerInPosition) {
  				if(!(coinArray[0].getxco() <= -1.65))
        		coinArray[0].setxco(coinArray[0].getxco() - 0.05);
  			}
  			break;
 		case GLUT_KEY_RIGHT:
			if (isStrikerInPosition) {
        		if(!(coinArray[0].getxco() >= 1.65))
        		coinArray[0].setxco(coinArray[0].getxco() + 0.05);
  			}
  			break;
 		}


}

int rightDown = -1;
int leftDown = -1;

void mouseButton(int button, int state, int x, int y) {

	double x1 = x / (double) 1550   - 0.5 ;
	double y1 = (1 - y / (double) 1000)   - 0.5;
		x1 = x1 * 9.968;
		y1 = y1 * 9.968;
	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {

		// when the button is released
		if (state == GLUT_UP) {
			leftDown = -1;
			isSpaceKeyPressed = true;
			execute();

		}
		else  {// state = GLUT_DOWN
			leftDown = 1;
		}
	}

	if (button == GLUT_RIGHT_BUTTON) {

		// when the button is released
		if (state == GLUT_UP) 
			rightDown = -1;
			
		else
			rightDown = 1;
		
	}
}


void mouseMove(int x, int y) {

		double x1 = x / (double) 1550   - 0.5 ;
	double y1 = (1 - y / (double) 1000)   - 0.5;
		x1 = x1 * 9.968;
		y1 = y1 * 9.968;

	// this will only be true when the left button is down
	
	if (leftDown > 0) {

	float m, inverse;
	m = (y1 - coinArray[0].getyco())/(x1 - coinArray[0].getxco());
	inverse = 1.5708 - fabs(atan(m));
	if(atan(m) <= 0){
		inverse = -1 * inverse;
	}
	theta = inverse;
	double a;
	a = y1 - coinArray[0].getyco();
	a = a / 10;
	if(player2.getTurnFlag()){
		a = -1 * a;
	}
	if(a < 0){
		power = 0;
	}
	else if(a > 0.2){
		power = 0.2;
	}

	else {
		power = a;
	}
	if(player2.getTurnFlag()){
		power = -power;

	}


	}



	if(rightDown > 0){
		if (isStrikerInPosition) {
				if(x1 > coinArray[0].getxco()){
        		if(!(coinArray[0].getxco() >= 1.65))
        		coinArray[0].setxco(coinArray[0].getxco() + 0.05);
        		}
        		else if(x1 < coinArray[0].getxco()){
        		if(!(coinArray[0].getxco() <= -1.65))
        		coinArray[0].setxco(coinArray[0].getxco() - 0.05);
        		}


  			}
  			
	}
}

//Initializes 3D rendering

void initRendering() {

	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);    // Enable coloring
    //glClearColor(0.996f, 0.8242f, 0.6054f, 1.0f);   // Setting a background color
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    initGame();
 

}



//Called when the window is resized

void handleResize(int w, int h) {

	//Tell OpenGL how to convert from coordinates to pixel values

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective

	//Set the camera perspective

	glLoadIdentity(); //Reset the camera

	gluPerspective(45.0,                  //The camera angle

				   (double)w / (double)h, //The width-to-height ratio

				   1.0,                   //The near z clipping coordinate

				   200.0);                //The far z clipping coordinate

}

void drawQuadd(float x1, float y1, float x2, float y2, float x3, float y3,  float x4, float y4 ){
	
	glBegin(GL_QUADS); //Begin quadrilateral coordinates

	glVertex3f(x1, y1, 0.0f);

	glVertex3f(x2, y2, 0.0f);

	glVertex3f(x3, y3, 0.0f);

	glVertex3f(x4, y4, 0.0f);

	glEnd(); //End quadrilateral coordinates


}


void drawBoard(){

board.drawBoardNow();

	
}

///////////////////////// impliments collision properties /////////////////////////

bool physics(Coin& coin1, Coin& coin2, float a){

	//dealing collisions
	float cosine, sine, u1, u2, v1, v2, y1, y2;
	cosine = (coin2.getxco() - coin1.getxco()) / a;
	sine = (coin2.getyco() - coin1.getyco()) / a;
	u1 = coin1.getyvel() * sine + coin1.getxvel() * cosine;
	u2 = coin2.getyvel() * sine + coin2.getxvel() * cosine;

	v1 = (u1*(1 - alpha) + u2*(1 + alpha))/2;
	v2 = (u1*(1 + alpha) + u2*(1 - alpha))/2;

	y1 = coin1.getyvel()*cosine - coin1.getxvel()*sine;
	y2 = coin2.getyvel()*cosine - coin2.getxvel()*sine;

	coin1.setVel( (v1*cosine - y1 * sine), (v1*sine + y1 * cosine));
	coin2.setVel( (v2*cosine - y2 * sine), (v2*sine + y2 * cosine));

	if(coin1.getxvel() != 0)
    coin1.setxco(coin1.getxco() + coin1.getxvel());// * 0.2 ;// fabs(striker.getxvel());
    if(coin1.getyvel() != 0)
    coin1.setyco(coin1.getyco() + coin1.getyvel());// * 0.2 ;// fabs(striker.getxvel());
    if(coin2.getxvel() != 0)
	coin2.setxco(coin2.getxco() + coin2.getxvel());// * 0.2 ;// fabs(striker.getxvel());
    if(coin2.getyvel() != 0)
    coin2.setyco(coin2.getyco() + coin2.getyvel());// * 0.2 ;// fabs(striker.getxvel());
    
	return true;

}


void reduceVelocity(Coin &coin){
	float a, b;
	a = coin.getxvel();
	b = coin.getxvel();
	if(a != 0){
		
		a = a - a *(mew * gravity * updateRate) / fabs(a);
		if(b * a <= 0)
			coin.setxvel(0);
		else
			coin.setxvel(a);
	}

	a = coin.getyvel();
	b = coin.getyvel();
	if(a != 0){
		
		a = a - a * (mew * gravity * updateRate) / fabs(a);
		if(b * a <= 0)
			coin.setyvel(0);
		else
			coin.setyvel(a);
	}
}

///////////////////////// impliments friction /////////////////////////

void checkFriction(){

	int i;
	for(i = 0; i< 8; i++){
	reduceVelocity(coinArray[i]);

	}
}

///////////////////////// checks collision between all /////////////////////////

bool isCollide(Coin& coin1, Coin& coin2){
	float a = ((coin1.getxco() - coin2.getxco())*(coin1.getxco() - coin2.getxco())) + ((coin1.getyco() - coin2.getyco())*(coin1.getyco() - coin2.getyco()));
	float distance = (coin1.getRadius() + coin2.getRadius()) * (coin1.getRadius() + coin2.getRadius());
	if(a <= distance){
		if(physics(coin1, coin2, sqrt(a))){
			if(!player1.getDecision()){ // is his coin color decided??
				if(coin2.getType() == "white"){
					if(player1.getTurnFlag()){
					player1.setCoinType(1);
					player2.setCoinType(0);
				}
				else{
					player1.setCoinType(0);
					player2.setCoinType(1);	
				}
					player1.toggleDecision();
					player2.toggleDecision();
				
				}

				else if(coin2.getType() == "black"){
					if(player1.getTurnFlag()){
					player1.setCoinType(0);
					player2.setCoinType(1);
				}
				else{
					player1.setCoinType(1);
					player2.setCoinType(0);	
				}player1.toggleDecision();
					player2.toggleDecision();
				
				}

				//cout << player1.getCoinType() << endl;
			}
			return true;
		}
	}


	return false;
}

///////////////////////// checks if the coin is in hole or not /////////////////////////

void checkHole(Coin& coin){
	int scoreHere = 0;
	if(
		(((coin.getxco() - 2.25)*(coin.getxco() - 2.25)) + ((coin.getyco() - 2.25)*(coin.getyco() - 2.25)) < 0.08491396) ||
		(((coin.getxco() + 2.25)*(coin.getxco() + 2.25)) + ((coin.getyco() - 2.25)*(coin.getyco() - 2.25)) < 0.08491396) ||
		(((coin.getxco() + 2.25)*(coin.getxco() + 2.25)) + ((coin.getyco() + 2.25)*(coin.getyco() + 2.25)) < 0.08491396) ||
		(((coin.getxco() - 2.25)*(coin.getxco() - 2.25)) + ((coin.getyco() + 2.25)*(coin.getyco() + 2.25)) < 0.08491396)

		){
		coin.setVel(0, 0);
		if(player1.getTurnFlag()){
		coin.setCO(fin1x, -1*finy);
		if(coin.getType() != "Striker")
		fin1x += 0.3;
		}
		else{
			coin.setCO(fin2x, finy);
			if(coin.getType() != "Striker")
			fin2x += 0.3;
		}
		float temp;
		temp = coin.getValue();
		if(player1.getDecision()){
			if(coin.getType() == "white"){
				if(player1.getTurnFlag()){
					if(player1.getCoinType() == 0)
						temp = -1 * temp /2 ;
				}
				else{
					if(player2.getCoinType() == 0)
						temp = -1 * temp /2;
				}
			}
			else if(coin.getType() == "black"){
				if(player1.getTurnFlag()){
					if(player1.getCoinType() == 1)
						temp = -1 * temp /2 ;
				}
				else{
					if(player2.getCoinType() == 1)
						temp = -1 * temp /2;
				}
			}
		}
		scoreHere = scoreHere + temp;
		}

	if(player1.getTurnFlag()){
		player1.setScore(scoreHere);
	}
	else if(player2.getTurnFlag()){
		player2.setScore(scoreHere);
	}
	if(fabs(coin.getxvel()) <= constraint){
		coin.setxvel(0);
		if(fabs(coin.getyvel()) <= constraint){
			coin.setyvel(0);
			coinsAtRest++;
		}
	}
	else if(fabs(coin.getyvel()) <= constraint){
		coin.setyvel(0);
			
	}
	

}

///////////////////////// collision checking starts here /////////////////////////

void checkCollision(){

	//striker and other coins

	//with red coin

	int i, j;
	for(i = 0; i < 8; i++){
		for(j = i+1 ; j < 8 ; j++){

			if(isCollide(coinArray[i], coinArray[j])){
				if(!firstStrike && isSpaceKeyPressed){
					if(player1.getTurnFlag()){
						if(coinArray[j].getType() == "white"){
							if(player1.getCoinType() == 0)
								player1.setScore(-1);
						}	
						else if(coinArray[j].getType() == "black"){
							if(player1.getCoinType() == 1)
								player1.setScore(-1);
							
						} 
					
				}
				else{
						if(coinArray[j].getType() == "white"){
							if(player2.getCoinType() == 0)
								player2.setScore(-1);
							}
						else if(coinArray[j].getType() == "black"){
							if(player2.getCoinType() == 1)
								player2.setScore(-1);
							} 
					}
				firstStrike = true;

				}
			}
		}
	}

	coinsAtRest = 0;
	for(i = 0 ; i< 8 ; i++){
	checkHole(coinArray[i]);
	}

	if(coinsAtRest == 8){
			if(!isStrikerInPosition){//&&	 ((striker.getxvel() == 0) && (striker.getyvel() == 0))){
		player1.toggleFlag();
		player2.toggleFlag();
		if(player1.getTurnFlag())
    	coinArray[0].setCO(0, -1.9);
    	
    	else if(player2.getTurnFlag())
    	coinArray[0].setCO(0, 1.9);
    	isStrikerInPosition = true;
    	firstStrike = false;
    	power = 0;
    	theta = 0;
    	isSpaceKeyPressed = false;
    }
    
    }

	coinsAtRest = 0;
	
}

///////////////////////// drawing currrent player indicator on the score board /////////////////////////
void drawIndicator(){

if(player1.getTurnFlag())
	glColor3f(0.0f, 1.0f, 0.0f);
	else
	glColor3f(1.0f, 0.0f, 0.0f);
	

	glBegin(GL_POLYGON);
    for(double i = 0; i < 2 * PI; i += PI / 20) //<-- Change this Value
		glVertex3f(3.4+ cos(i) * 0.0951, 2.12+ sin(i) * 0.0951, 0.0);
	glEnd();

	if(player2.getTurnFlag())
	glColor3f(0.0f, 1.0f, 0.0f);
	else
	glColor3f(1.0f, 0.0f, 0.0f);

	glBegin(GL_POLYGON);
    for(double i = 0; i < 2 * PI; i += PI / 20) //<-- Change this Value
		glVertex3f(-4.1+ cos(i) * 0.0951, 2.12+ sin(i) * 0.0951, 0.0);
	glEnd();

	if(player1.getDecision()){
	
	if(player1.getCoinType() == 1)
		glColor3f(0.0f, 0.0f, 0.0f);
	else
		glColor3f(1.0f, 1.0f, 1.0f);
	
	glBegin(GL_POLYGON);
    for(double i = 0; i < 2 * PI; i += PI / 20) //<-- Change this Value
		glVertex3f(-3.25+ cos(i) * 0.0951, 1.7+ sin(i) * 0.0951, 0.0);
	glEnd();

	if(player1.getCoinType() == 1)
		glColor3f(1.0f, 1.0f, 1.0f);
	else
		glColor3f(0.0f, 0.0f, 0.0f);
	
	glBegin(GL_POLYGON);
    for(double i = 0; i < 2 * PI; i += PI / 20) //<-- Change this Value
		glVertex3f(-3.25+ cos(i) * 0.04755, 1.7+ sin(i) * 0.04755, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
    for(double i = 0; i < 2 * PI; i += PI / 20) //<-- Change this Value
		glVertex3f(4.3+ cos(i) * 0.0951, 1.7+ sin(i) * 0.0951, 0.0);
	glEnd();

	if(player1.getCoinType() == 1)
		glColor3f(0.0f, 0.0f, 0.0f);
	else
		glColor3f(1.0f, 1.0f, 1.0f);

	glBegin(GL_POLYGON);
    for(double i = 0; i < 2 * PI; i += PI / 20) //<-- Change this Value
		glVertex3f(4.3+ cos(i) * 0.04755, 1.7+ sin(i) * 0.04755, 0.0);
	glEnd();
	
	}

}

///////////////////////// draw score board /////////////////////////

void drawScore(){

	glColor3f(0.96f, 0.88f, 0.62f);
	drawQuadd(4.6 ,2.4, 3.0, 2.4, 3.0, 0.0, 4.6, 0.0);
	drawQuadd(-4.6 ,2.4, -3.0, 2.4, -3.0, 0.0, -4.6, 0.0);

	drawIndicator();
	
	glColor3f( 0.0f, 0.0f, 0.0f );
    int len[6], i;
  	ostringstream ostr[6];
  	string playerString[2], scoreString[2], timeString[2], totalString[2];
    string choiceString, timeDisplayString, totalDisplayScore, scoreDisplayString;

  	ostr[0] << player1.getScore(); //use the string stream just like cout,
    ostr[1] << player2.getScore();
    ostr[2] << player1.getTimePenalty();
    ostr[3] << player2.getTimePenalty();
    ostr[4] << player1.getScore() - player1.getTimePenalty();
    ostr[5] << player2.getScore() - player2.getTimePenalty();
    
    //except the stream prints not to stdout but to a string.
    playerString[0] = "Player 1";
    playerString[1] = "Player 2";
    scoreDisplayString = "Score";
    choiceString = "Choice";
    timeDisplayString = "Time penalty";
  	totalDisplayScore = "Total Score";

  	scoreString[0] = ostr[0].str();
  	scoreString[1] = ostr[1].str();
    timeString[0] = ostr[2].str();
  	timeString[1] = ostr[3].str();
  	totalString[0] = ostr[4].str();
  	totalString[1] = ostr[5].str();
  
    len[0] = scoreString[0].length();
 	len[1] = scoreString[1].length();
 	len[2] = timeString[0].length();
 	len[3] = timeString[1].length();
 	len[4] = totalString[0].length();
 	len[5] = totalString[1].length();   

 	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( 0, 1550, 0, 1000 );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();	
	glLoadIdentity();
	int j, w, h;

	for(j = 0;j<2;j++){

		if(j ==0){
			w = 1210;
			h = 750;
		}
		else{
			w = 300;
			h = 750;
		}
	glRasterPos3f( w, h, 0 );  // move in 10 pixels from the left and bottom edges
	for ( i = 0; i < 8; ++i ) {
	    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, playerString[j][i]);
	}

	glRasterPos3f( w-60, h-50, 0 );  // move in 10 pixels from the left and bottom edges
	for (  i = 0; i < 6; ++i ) {
	    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, choiceString[i]);
	}
	

	glRasterPos3f( w-60, h-100, 0 );  // move in 10 pixels from the left and bottom edges
	for (  i = 0; i < 5; ++i ) {
	    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, scoreDisplayString[i]);
	}
	
	glRasterPos3f( w+80, h-100, 0 );  // move in 10 pixels from the left and bottom edges
	for (  i = 0; i < len[0+j]; ++i ) {
	    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, scoreString[j][i]);
	}

	glRasterPos3f( w-60, h-150, 0 );  // move in 10 pixels from the left and bottom edges
	for (  i = 0; i < 12; ++i ) {
	    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, timeDisplayString[i]);
	}
	
	glRasterPos3f( w+80, h-150, 0 );  // move in 10 pixels from the left and bottom edges
	for (  i = 0; i < len[2+j]; ++i ) {
	    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, timeString[j][i]);
	}

	glRasterPos3f( w-60, h-200, 0 );  // move in 10 pixels from the left and bottom edges
	for (  i = 0; i < 11; ++i ) {
	    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, totalDisplayScore[i]);
	}
	
	glRasterPos3f( w+80, h-200, 0 );  // move in 10 pixels from the left and bottom edges
	for (  i = 0; i < len[4+j]; ++i ) {
	    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, totalString[j][i]);
	}
}

	glPopMatrix();

	glMatrixMode( GL_PROJECTION );
	glPopMatrix();


}

///////////////////////// draw power and angle of striker /////////////////////////

void drawControl(){
	if(isStrikerInPosition){
	glColor3f(coinArray[0].getR(), coinArray[0].getG(), coinArray[0].getB());
	glBegin( GL_LINES);
	glVertex3f( coinArray[0].getxco(), coinArray[0].getyco(), 0.0f );
	glVertex3f( coinArray[0].getxco() + (10*power*sin(theta)), coinArray[0].getyco() + (10* power* cos(theta)), 0);
	glEnd();
	glBegin(GL_POLYGON);
   for(double i = 0; i < 2 * PI; i += PI / 20) //<-- Change this Value
		glVertex3f(coinArray[0].getxco() + (10*power*sin(theta))+ cos(i) * 0.05, coinArray[0].getyco() + (10* power* cos(theta)) + sin(i) * 0.05, 0.0);

	glEnd();
}
	
}

///////////////////////// check collison with  wall /////////////////////////

void updatePosition(Coin& coin){
if(coin.getxco() + coin.getRadius() >= 2.4 || coin.getxco() - coin.getRadius() <= -2.4)
        coin.setxvel(coin.getxvel() * -0.99);
    if(coin.getyco() + coin.getRadius() > 2.4 || coin.getyco() - coin.getRadius() < -2.4)
        coin.setyvel(coin.getyvel() * -0.99);

    
    // Update position of ball
    if(coin.getxvel() != 0)
    coin.setxco(coin.getxco() + coin.getxvel());
    if(coin.getyvel() != 0)
    coin.setyco(coin.getyco() + coin.getyvel());
}


///////////////////////// upsate ime for penalty /////////////////////////

void updateTime(int value){

	time_t now = time(0);
	tm *ltm = localtime(&now);

		if(player1.getTurnFlag()){
			if(fabs(ltm->tm_sec - player1.getLastTime())  >= 1){
				player1.setTimePenalty();
				//cout << player1.getLastTime()<< endl; 
				player1.setLastTime(ltm->tm_sec);
			}
		}
		

		if(player2.getTurnFlag()){
			if(fabs(ltm->tm_sec - player2.getLastTime()) >= 1){
				player2.setTimePenalty();
				player2.setLastTime(ltm->tm_sec);
			}
		}

		glutTimerFunc(900, updateTime, 0);
}

///////////////////////// checking if any coin is in the way of striker before execution /////////////////////////

void checkLane(){
	for (int i = 1 ; i < 8 ; i++){

	float a = ((coinArray[0].getxco() - coinArray[i].getxco())*(coinArray[0].getxco() - coinArray[i].getxco())) + 
				((coinArray[0].getyco() - coinArray[i].getyco())*(coinArray[0].getyco() - coinArray[i].getyco()));
	float distance = (coinArray[0].getRadius() + coinArray[i].getRadius()) * (coinArray[0].getRadius() + coinArray[i].getRadius());
	if(a<= distance){
		if(coinArray[0].getxco() <= coinArray[i].getxco()){
			coinArray[0].setxco(coinArray[0].getxco() + 4* coinArray[i].getRadius());
		}
		else{
			coinArray[0].setxco(coinArray[0].getxco() - 4* coinArray[i].getRadius());
		}
	}
	}
}

///////////////////////// main update function /////////////////////////

void update(int value) {
    
    checkCollision();
    checkFriction();

	for(int i = 0 ; i< 8 ; i++){
    updatePosition(coinArray[i]);
	}
  	if(isStrikerInPosition){
  		checkLane();
  	}
	checkCollision();
  	glutTimerFunc(updateRate, update, 0);  
}

///////////////////////// execute the striker /////////////////////////

void execute(){
	if(isSpaceKeyPressed && isStrikerInPosition){
	coinArray[0].setVel(power * sin(theta), power * cos(theta));
	power = 0;
	isStrikerInPosition = false;
	}
}

/////////////////////////  draw coins by class method /////////////////////////

void drawCoins(){
	int j;
	for(j = 0; j < 8 ; j++)
		coinArray[j].drawCoin();
}


void drawScene() {

	//Clear information from last draw

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective

	glLoadIdentity(); //Reset the drawing perspective
	glPushMatrix();
    glTranslatef(0.0f, 0.0f, -10.0f);
    glColor3f(0.60f, 0.39f, 0.12f);
    drawBoard();
    drawCoins();
    drawControl();
    drawScore();
	glPopMatrix();
	glutSwapBuffers(); 
}


///////////////////////// game inititalisation with coin placements /////////////////////////

void initGame(){
	int i;
	//striker
	coinArray[0].setCO(0, -1.9);
	//red coin
	coinArray[1].setCO(0, 0);


	coinArray[2].setCO(-0.35793375, 0.6199275);
	coinArray[3].setCO(0.7158375, 0);
	coinArray[4].setCO(-0.35793375, -0.6199275);
	
	coinArray[5].setCO(0.35793375, 0.6199275);
	coinArray[6].setCO(0.35793375, -0.6199275);
	coinArray[7].setCO(-0.7158375, 0);
	
	coinArray[0].setType("Striker");
	coinArray[0].setColor(0,0,1);
	coinArray[0].setValue(-5);
	coinArray[0].setRadius(0.14);

	coinArray[1].setType("red");
	coinArray[1].setColor(1,0,0);
	coinArray[1].setValue(50);

	for(i = 2; i < 5 ; i++){
		coinArray[i].setType("white");
		coinArray[i].setColor(1,1,1);
		coinArray[i].setValue(10);
	}

	for(i = 5; i < 8 ; i++){
		coinArray[i].setType("black");
		coinArray[i].setColor(0,0,0);
		coinArray[i].setValue(10);
	}

	player1.toggleFlag();
	
}	


int main(int argc, char** argv) {

	//Initialize GLUT
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowSize(1550, 1000); //Set the window size
	glutCreateWindow("carroms");

	initRendering(); //Initialize rendering
	glutDisplayFunc(drawScene);
	glutIdleFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutSpecialFunc(handleSpecialKeypress);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);
	glutReshapeFunc(handleResize);
	glutTimerFunc(updateRate, update, 0);
	glutTimerFunc(updateRate, updateTime, 0);

	glutMainLoop(); //Start the main loop


	return 0;
}