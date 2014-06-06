#ifndef __World_h_
#define __World_h_
#include <string>
#include "OgreSceneNode.h"
#include "OgreOverlayManager.h"
#include "OgreOverlay.h"

// Forward delcarations of Ogre classes.  Note the Ogre namespace!
namespace Ogre {
    class SceneNode;
    class SceneManager;
}

// Forward delcarations of our own classes
class PongCamera;
class InputHandler;


class World
{
public:
	
    World(Ogre::SceneManager *sceneManager, InputHandler *input);

    // You'll want various methods to access & change your world here
    
    void Think(float time);
	
	void addCamera(PongCamera *c) { mCamera = c; }
	void gameState(float time);
	void controlPart(float time);
	void specialEventAppear(float time);
	void specialEventReset();
	void rotateBall(float time);
	void displayText(float time);
	float calc_distance(float x1, float y1, float x2, float y2);


	Ogre::SceneManager *SceneManager() { return mSceneManager; }

	Ogre::SceneNode *ball;
	Ogre::SceneNode *left_bar;
	Ogre::SceneNode *right_bar;
	Ogre::SceneNode *upBound;
	Ogre::SceneNode *downBound;
	Ogre::SceneNode *leftMonkey;
	Ogre::SceneNode *rightMonkey;
	Ogre::SceneNode *drop;
	Ogre::SceneNode *bonus;
	Ogre::SceneNode *bonus2;

	Ogre::Entity *ent1;
	Ogre::Entity *ent2;
	Ogre::Entity *ent3;
	Ogre::Entity *ent4;
	Ogre::Entity *ent5;
	Ogre::Entity *ent6;
	Ogre::Entity *ent7;
	Ogre::Entity *ent8;
	Ogre::Entity *ent9;
	Ogre::Entity *ent10;
	Ogre::Entity *ent11;
	Ogre::Entity *ent12;
	Ogre::Entity *ent13;
	Ogre::Entity *ent14;


	Ogre::Timer*   timer1; // Define the timer
	Ogre::Timer*   event_timer2; // Define the event timer

	int choose_state;	//0:1P  1:2P
	int state;			//game state  -1:choose 1P or 2P  0: waiting for serving 1:playing
	bool event_start;
	float ball_x_speed;		//ball x speed
	float ball_y_speed;		//ball y speed
	int leftScore;			//left score
	int rightScore;			//right score
	int special_event;		//special event number   0:nothing, 1:random brick, 2:wide paddle, 3:short paddle, 4: two wormholes, 5:speed up paddle, 6:slow down paddle, 7:gravity
	


	std::string choosetext1;
	std::string choosetext2;

	std::string text1;
	std::string text2;		//special event display

	float left_bar_speed;
	float right_bar_speed;
	float left_bar_width;		//original left bar width = 10
	float right_bar_width;		//original right bar width = 10
	float original_bar_width;
	float original_bar_speed;
	bool event_trigged;



	float drop_x;
	float drop_y;
	float wormhole1_x;
	float wormhole1_y;
	float wormhole2_x;
	float wormhole2_y;
	
	float ball_radius;
	float bonus_radius;
	float drop_width;		//original drop width = 6

	float  RADIANS_PER_SECOND;

	Ogre::Overlay *choose;



	float      countdown_time; // last special event disappears or effect disappears +countdown_time = new special event appear
	float      event_countdown_time; // special event1 last for event_countdown_time
	float		effect_countdown_time;	//special event trigged duration
	


protected:


	Ogre::SceneManager *mSceneManager;

	InputHandler *mInputHandler;
	PongCamera *mCamera;



	// Here is where you keep all your world data.
	//  You probably want to use containers (arrays / lists / classes / etc) to ogranize them, 
	//    instead of a whole list of variables.  

	

};

#endif