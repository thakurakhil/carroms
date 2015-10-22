/*
This file contains the implimentations of the defination of function provided in the header file
classes implimented:Player, Coin, Board

Author : Akhil
*/

using namespace std;

#include "class.h"
#include <GL/glut.h>
#include <math.h>

#define PI 3.1415926535897932384626433832795



	Player::Player(){
		score = 30;
		turnFlag = 0;
		coinFlag = -1;
		isDecisionMade = false;
		lastTime = 0;
		timePenalty = 0;
		name = "anonymous";

	}

	float Player::getScore() const{
		return score;
	}

	float Player::getTimePenalty() const{
		return timePenalty;
	}

	void Player::setTimePenalty(){

		timePenalty = timePenalty + 1;
	}

	float Player::getLastTime() const{
		return lastTime;
	}

	void Player::setLastTime(float timehere){

		lastTime = timehere;
	}

	bool Player::getDecision() const{
		return isDecisionMade;
	}

	void Player::toggleDecision(){

		isDecisionMade = !isDecisionMade;
	}

	int Player::getTurnFlag() const{
		return turnFlag;
	}

	int Player::getCoinType() const{
		return coinFlag;
	}
	
	// if choice is 1 his coins are white else black
	void Player::setCoinType(int choice){
		coinFlag = choice;
	}



	string Player::getName() const{
		return name;
	}

	void Player::setScore(float value){
		score += value;
	}
	void Player::setName(string myname){
		name = myname;
	}

	void Player::toggleFlag(){
		if(turnFlag == 0){
			turnFlag = 1;

		}
		else{
			turnFlag = 0;
		}
	}




	// Accessible by ALL
   // Declare prototype of member functions
   // Constructor with default values
   Coin::Coin(float xco , float yco  , float velinx , float veliny , 
   		float radiushere , string typehere , float valuehere ){
   	x = xco;
   	y = yco;
   	velx = velinx;
   	vely = veliny;
   	radius = radiushere;
   	value = valuehere;
   	type = typehere;
   	R = 0.0f;
   	G = 0.0f;
   	B = 0.0f;   }

   //~Coin(){
   	 //cout << "Object is being deleted" << endl;
   //}
 
   float Coin::getRadius() const{
   	return radius;
   }
   
   void Coin::setRadius(float radiusnow){
   	radius = radiusnow;
   }

   float Coin::getValue() const{
   	return value;
   }
   
   void Coin::setValue(float valuenow){
   	value = valuenow;
   }

   string Coin::getType() const{
   	return type;
   }
   
   void Coin::setType(string typenow){
   	type = typenow;
   }
   
   float Coin::getR() const{
   	return R;
   }
   float Coin::getG() const{
   	return G;
   }
   float Coin::getB() const{
   	return B;
   }
   
   void Coin::setColor(float r, float g, float b){
   	R = r;
   	G = g;
   	B = b;
   }

   float Coin::getxco() const{
   	return x;
   }
   
   float Coin::getyco() const{
   	return y;
   }
   
   void Coin::setCO(float xco , float yco){
   	x = xco;
   	y = yco;
   }

   void Coin::setxco(float xco){
   	x = xco;
   }

   void Coin::setyco(float yco){
   	y = yco;
   }

	

   float Coin::getxvel() const{
   	return velx;
   }
   
   float Coin::getyvel() const{
   	return vely;
   }
   
   void Coin::setVel(float xvel , float yvel){
   	velx = xvel;
   	vely = yvel;
   }
   
   void Coin::setxvel(float xvel){
   	velx = xvel;
   }
   void Coin::setyvel(float yvel){
   	vely = yvel;
   }

   void Coin::drawCoin(){

      glBegin(GL_POLYGON);
   glColor3f(this->R, this->G, this->B);
   for(double i = 0; i < 2 * PI; i += PI / 20)
      glVertex3f(this->x + cos(i) * this->radius, this->y + sin(i) * this->radius, 0.0);
   glEnd();
   
   glBegin(GL_POLYGON);
   if(this->G == 0)
      glColor3f(1.0f, 1.0f, 1.0f);
   else
      glColor3f(0.0f, 0.0f, 0.0f);
   for(double i = 0; i < 2 * PI; i += PI / 20)
      glVertex3f(this->x + cos(i) * (this->radius/2), this->y+ sin(i) * (this->radius/2), 0.0);
   glEnd();
 

   }



/* some fuction to draw shapre like rectangles,cirlces,etc*/

void Board::drawQuad(float x1, float y1, float x2, float y2, float x3, float y3,  float x4, float y4 ){
   
   glBegin(GL_QUADS); //Begin quadrilateral coordinates

   glVertex3f(x1, y1, 0.0f);

   glVertex3f(x2, y2, 0.0f);

   glVertex3f(x3, y3, 0.0f);

   glVertex3f(x4, y4, 0.0f);

   glEnd(); //End quadrilateral coordinates


}

void Board::drawDesignCircles(float x, float y){

   glColor3f(1.0f, 0.0f, 0.0f);

   glBegin(GL_POLYGON);
    for(double i = 0; i < 2 * PI; i += PI / 20) 
      glVertex3f(x+ cos(i) * 0.1314, y+ sin(i) * 0.1314, 0.0);
   glEnd();
   glColor3f(0.96f, 0.88f, 0.62f);

   glBegin(GL_POLYGON);
    
    for(double i = 0; i < 2 * PI; i += PI / 20) 
      glVertex3f(x+ cos(i) * 0.0951, y+ sin(i) * 0.0951, 0.0);
   glEnd();
   
}

void Board::drawHoles(float x, float y){

   
   glBegin(GL_POLYGON);
   for(double i = 0; i < 2 * PI; i += PI / 20) 
      glVertex3f(x+ cos(i) * 0.15, y+ sin(i) * 0.15, 0.0);

   glEnd();
   
}

void Board::drawBoardNow(){

   drawQuad(2.8, 2.8, -2.8, 2.8, -2.8, 2.4, 2.8, 2.4);
   drawQuad(2.8, 2.8, 2.4, 2.8, 2.4, -2.8, 2.8, -2.8);
   drawQuad(2.8, -2.8, 2.8, -2.4, -2.8, -2.4, -2.8, -2.8);
   drawQuad(-2.4, 2.8, -2.8, 2.8, -2.8, -2.8, -2.4, -2.8);
   glColor3f(0.96f, 0.88f, 0.62f);
   drawQuad(2.4, 2.4, -2.4, 2.4, -2.4, -2.4, 2.4, -2.4);
   glColor3f(0.0f, 0.0f, 0.0f);
   //begin design in board
   glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
   //glEnable(GL_POLYGON_OFFSET_LINE);
   //glPolygonOffset(-1.f,-1.f);
    drawQuad(1.8, 2.0, -1.8, 2.0, -1.8, -2.0, 1.8, -2.0);
    drawQuad(2.0, 1.8, -2.0, 1.8, -2.0, -1.8, 2.0, -1.8);
    drawQuad(1.8, 1.8, -1.8, 1.8, -1.8, -1.8, 1.8, -1.8);
    glColor3f(0.0f, 0.0f, 0.0f);
   glBegin(GL_POLYGON);
    
    for(double i = 0; i < 2 * PI; i += PI / 20) 
      glVertex3f(cos(i) * 0.7158375, sin(i) * 0.7158375, 0.0);
   glEnd();

   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //draw red circles at each line
   drawDesignCircles(1.7, 1.9);
   drawDesignCircles(-1.7, 1.9);
   drawDesignCircles(1.7, -1.9);
   drawDesignCircles(-1.7, -1.9);
   drawDesignCircles(-1.9, -1.7);
   drawDesignCircles(1.9, -1.7);
   drawDesignCircles(-1.9, 1.7);
   drawDesignCircles(1.9, 1.7);

   //Draw Holes
   
   glColor3f(0.0f, 0.0f, 0.0f);
   drawHoles(2.25, 2.25);
   drawHoles(-2.25, 2.25);
   drawHoles(2.25, -2.25);
   drawHoles(-2.25, -2.25);
   
}



