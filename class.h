/*
This header file contains the definations of Player Coin and Board classes
Author:Akhil

*/

using namespace std;

#include <string> 

class Player {
private:
	int turnFlag, coinFlag;
	bool isDecisionMade;
	string name;
	float score, timePenalty ,lastTime;
// coinFlag 1 means white
public:
	Player();

	float getScore() const;
	float getTimePenalty() const;
	void setTimePenalty();
	float getLastTime() const;
	void setLastTime(float timehere);
	bool getDecision() const;
	void toggleDecision();
	int getTurnFlag() const;
	int getCoinType() const;
	// if choice is 1 his coins are white else black
   void setCoinType(int choice);
	string getName() const;
	void setScore(float value);
   void setName(string myname);
	void toggleFlag();

};



class Coin {
private:   // Accessible by members of this class only
   // private data members (variables)
float x, y, velx, vely, radius , R, G, B, value;
string type;
//bool isStriker;
 
public:    
	// Accessible by ALL
   // Declare prototype of member functions
   // Constructor with default values
   Coin(float xco = 0, float yco = 0 , float velinx = 0, float veliny = 0, 
   		float radiushere = 0.1314, string typehere = "white", float valuehere = 1);

   float getRadius() const;
   void setRadius(float radiusnow);
   float getValue() const;
   void setValue(float valuenow);
   string getType() const;
   void setType(string typenow);
   float getR() const;
   float getG() const;
   float getB() const;
   void setColor(float r, float g, float b);
   float getxco() const;
   float getyco() const;
   void setCO(float xco , float yco);
   void setxco(float xco);
   void setyco(float yco);
   float getxvel() const;
   float getyvel() const;
   void setVel(float xvel , float yvel);
   void setxvel(float xvel);
   void setyvel(float yvel);
   void drawCoin();
};



class Board{

public:
  
  void drawQuad(float x1, float y1, float x2, float y2, float x3, float y3,  float x4, float y4 );
  void drawDesignCircles(float x, float y);
  void drawHoles(float x, float y);
  void drawBoardNow();

};






