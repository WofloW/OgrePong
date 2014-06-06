// My header file.  This should go first!
#include "World.h"

// Ogre header files
#include "Ogre.h"
#include "OgreMath.h"
#include "OgreSceneManager.h"
#include "OgreFontManager.h"
#include "OgreTextAreaOverlayElement.h"


// IOS (Input system) header files

#include <ois/ois.h>

// Other input files for my project
#include "Camera.h"
#include "InputHandler.h"

#include "win32/OgreTimerImp.h"




World::World(Ogre::SceneManager *sceneManager, InputHandler *input)   : mSceneManager(sceneManager), mInputHandler(input)
{
	srand( time(NULL));
	choose_state = 0;
	state = -1; // -1:choose vs AI or 2 players
	leftScore = 0; //left Score
	rightScore = 0; //right Score
	ball_x_speed = 0;	//ball x speed
	ball_y_speed = 0;	//ball y speed


	//timing introduction : First  if timer1->getMilliseconds() >countdown_time, special event appears
	//						Second (event_start) special event start
	//								           if no actived effect, {if event_timer2->getMilliseconds() > event_countdown_time, special event ends. timer1 resets, event_timer2 resets, go first}
	//												     {if active effect, event_timer2 resets }   {if event_timer2->getMilliseconds() > event_countdown_time, special event ends, effect ends.}
	countdown_time = 2000;		//last special event disappears or effect disappears +countdown_time = new special event appear  1000 = 1s
	event_countdown_time = 6500;		//special event1 last for event_countdown_time
	//effect_countdown_time = 6000;		//effect last for effect_countdown_time
	timer1 = new Ogre::Timer();
	event_timer2 = new Ogre::Timer();

	event_start = false;

	RADIANS_PER_SECOND = 0.5;

	original_bar_width = 10;
	original_bar_speed = 40;
	left_bar_width = 10;  //left bar width
	left_bar_speed = 40;	//left bar speed

	right_bar_width = 10;  //left bar width
	right_bar_speed = 40;	//left bar speed

	ball_radius = 1;			//orginal bonus_radius = 1
	bonus_radius = 5;		// orginal bonus_radius  = 1.25

	drop_width = 12;  //drop width   original drop width = 6
	event_trigged = false;	//default, special event is not trigged.


	// Global illumination for now.  Adding individual light sources will make you scene look more realistic
	mSceneManager->setAmbientLight(Ogre::ColourValue(1,1,1));

	// First, we create a scene node, and attach a model to it.  coin.mesh is defined in the Content directory,
	//  under models.  Note that you should probably place all of this creation code into a separate method (or methods)
	//  for your final game
	ent1 =SceneManager()->createEntity("ball.mesh");			//ball
	ball = SceneManager()->getRootSceneNode()->createChildSceneNode();
	ball->attachObject(ent1);

	ent2 =SceneManager()->createEntity("bar.mesh");		//left bar
	left_bar = SceneManager()->getRootSceneNode()->createChildSceneNode();
	left_bar->attachObject(ent2);
	left_bar -> setPosition(-45,0,0);
	//left_bar -> scale(1,1.5,1);

	ent3 =SceneManager()->createEntity("bar.mesh");		//right bar
	right_bar = SceneManager()->getRootSceneNode()->createChildSceneNode();
	right_bar->attachObject(ent3);
	right_bar -> setPosition(45,0,0);
	//right_bar -> scale(1,1.5,1);


	ent4 =SceneManager()->createEntity("bound.mesh");		//up bar
	upBound = SceneManager()->getRootSceneNode()->createChildSceneNode();
	upBound->attachObject(ent4);
	upBound -> setPosition(0,30,0);
	upBound -> scale(45,1,1);

	ent5 =SceneManager()->createEntity("bound.mesh");		//down bar
	downBound = SceneManager()->getRootSceneNode()->createChildSceneNode();
	downBound->attachObject(ent5);
	downBound -> setPosition(0,-30,0);
	downBound -> scale(45,1,1);

	ent6 =SceneManager()->createEntity("monkey.mesh");		//left monkey
	leftMonkey = SceneManager()->getRootSceneNode()->createChildSceneNode();
	leftMonkey->attachObject(ent6);
	leftMonkey -> setPosition(left_bar->getPosition().x-2,left_bar->getPosition().y-1,-1);
	leftMonkey->pitch(Ogre::Degree(90));
	leftMonkey->yaw(Ogre::Degree(180));
	leftMonkey->roll(Ogre::Degree(-90));
	leftMonkey -> scale(2,2,2);


	ent7 =SceneManager()->createEntity("monkey.mesh");		//right monkey
	rightMonkey = SceneManager()->getRootSceneNode()->createChildSceneNode();
	rightMonkey->attachObject(ent7);
	rightMonkey -> setPosition(right_bar->getPosition().x+2,right_bar->getPosition().y-1,-1);
	rightMonkey->pitch(Ogre::Degree(90));
	rightMonkey->roll(Ogre::Degree(-90));
	rightMonkey -> scale(2,2,2);


	ent8 =SceneManager()->createEntity("drop.mesh");		//random appear brick to bounce the ball
	drop = SceneManager()->getRootSceneNode()->createChildSceneNode();  //appear brick  special event
	drop ->scale(1,drop_width/6,1);

	ent9 =SceneManager()->createEntity("widenbonus.mesh");		
	ent10 = SceneManager()->createEntity("shortenbonus.mesh");
	ent11 = SceneManager()->createEntity("wormhole.mesh");
	ent12 = SceneManager()->createEntity("wormhole.mesh");
	ent13 = SceneManager()->createEntity("fastbonus.mesh");
	ent14 = SceneManager()->createEntity("slowbonus.mesh");

	bonus = SceneManager()->getRootSceneNode()->createChildSceneNode();  
	bonus ->scale(bonus_radius/1.25,bonus_radius/1.25,bonus_radius/1.25);
	bonus2 = SceneManager()->getRootSceneNode()->createChildSceneNode();  
	bonus2 ->scale(bonus_radius/1.25,bonus_radius/1.25,bonus_radius/1.25);

	

	


	// Now that we have a scene node, we can move, scale and rotate it as we please.  We'll scale this up a smidge
	// just as an example.  
	//   (take a look at the header file for Ogre::SceneNode -- a good IDE is your friend, here)

	

	// Yeah, this should be done automatically for all fonts referenced in an overlay file.
	//  But there is a bug in the OGRE code so we need to do it manually.
	Ogre::ResourceManager::ResourceMapIterator iter = Ogre::FontManager::getSingleton().getResourceIterator();
	while (iter.hasMoreElements()) 
	{ 
		iter.getNext()->load(); 
	}

	// Now we will show the sample overlay.  Look in the file Content/Overlays/Example to
	// see how this overlay is defined
	Ogre::OverlayManager& om = Ogre::OverlayManager::getSingleton();
	choose = om.getByName("Choose");
	Ogre::Overlay *overly = om.getByName("Sample");
	Ogre::Overlay *dialog = om.getByName("Dialog");
	overly->show();
	dialog->show();
	choose->show();
	choosetext1 = "->    VS AI";
	choosetext2 = "      VS Player2";
}




void 
World::Think(float time)
{
	
	gameState(time);
	controlPart(time);
	rotateBall(time);
	displayText(time);
		 
}


void 
World::gameState(float time)			//main state transition function
{
	


	ball ->translate (time * ball_x_speed, time * ball_y_speed, 0); //ball moves all the time  with ball_x_speed and ball_y_speed
	if (leftScore >=5 || rightScore >=5)
	{
		choose->show();
		choosetext1 = "       GAME OVER";
		choosetext2 = (leftScore >=5)?"       1P Wins" :"       2P Wins";
		text1 = "";
		text2 = "";
		state = -1;
		leftScore = 0;
		rightScore = 0;
		ball->setPosition(0,0,0);
		ball_x_speed = 0;
		ball_y_speed = 0;
	}


	switch (state)
	{
	case -1:
		specialEventReset();		//reset special event to initial place and clear the dialog
		if (mInputHandler->IsKeyDown(OIS::KC_UP))
		{
			choosetext1 = "->    VS AI";
			choosetext2 = "      VS Player2";
			choose_state = 0;
		}
		else if (mInputHandler->IsKeyDown(OIS::KC_DOWN))
		{
			choosetext1 = "      VS AI";
			choosetext2 = "->    VS Player2";
			choose_state = 1;
		}
		if (mInputHandler->IsKeyDown(OIS::KC_RETURN))
		{
				state = 0;
				choose->hide();
		}
		text1 = "VS AI  1P: W&S OR UP&DOWN";
		text2 = "VS 2P  1P: W&S  2P:UP&DOWN";
		break;

	case 0:							//start state   press up or down or w or s to start the game

		specialEventReset();		//reset special event to initial place and clear the dialog
		ball->setPosition(0,0,0);
		
		
		if (state == 0 && ((mInputHandler->IsKeyDown(OIS::KC_UP)) ||(mInputHandler->IsKeyDown(OIS::KC_DOWN) ) || (mInputHandler->IsKeyDown(OIS::KC_W)) || (mInputHandler->IsKeyDown(OIS::KC_S) )) )
		 {
			 ball_x_speed = (rand()%2*2-1)*30;				//randomly serve a ball left or right with speed 30 
			 ball_y_speed = (rand()%2*2-1)*Ogre::Math::RangeRandom(30,40); //randomly serve a ball up or down with speed 30-40
			 if (choose_state == 0)
				state = 1;
			 else if (choose_state == 1)
				state = 2;
		 }

		break;
	case 1:							//1P moving state
		text1 = "Special event dialog:";
		specialEventAppear(time);	//start special event
		if (ball->getPosition().y + time * ball_y_speed +ball_radius> upBound->getPosition().y-1 && ball_y_speed > 0)			//ball hits the upbound
			ball_y_speed = -ball_y_speed;
		else if (ball->getPosition().y + time * ball_y_speed - ball_radius<downBound->getPosition().y+1 && ball_y_speed < 0)		//ball hits the downbound
			ball_y_speed = -ball_y_speed;
		
		if ( ball->getPosition().x + time * ball_x_speed -ball_radius < left_bar->getPosition().x+1)							//ball hits the left bar
		{
			if ( (ball->getPosition().y < left_bar->getPosition().y +left_bar_width/2) && (ball->getPosition().y > left_bar->getPosition().y -left_bar_width/2) )			//left bar gets the ball
			{
				ball_x_speed = -ball_x_speed;
				if ( (mInputHandler->IsKeyDown(OIS::KC_W) || mInputHandler->IsKeyDown(OIS::KC_UP)) && (left_bar->getPosition().y +left_bar_width/2 < upBound->getPosition().y-1))		//add spin (move the ball in bar-moving direction)
				{
					if (ball-> getPosition().y +2 + ball_radius < upBound->getPosition().y-1)		//ball can't be moved over the upbound
						ball ->translate (0, 2, 0);
					ball_y_speed +=5;
				}
				else if ((mInputHandler->IsKeyDown(OIS::KC_S) || mInputHandler->IsKeyDown(OIS::KC_DOWN))  && (left_bar->getPosition().y -left_bar_width/2 >downBound->getPosition().y+1))	//add spin (move the ball in bar-moving direction)
				{
					if (ball-> getPosition().y -2 + ball_radius > upBound->getPosition().y+1)	//ball can't be moved below the downbound
						ball ->translate (0, -2, 0);
					ball_y_speed -=5;
				}
			}
			else																			//left bar loses the ball
			{
				state = 0;
				rightScore++;
				text1 = "Left player lose";
			}
		}
		else if ( ball->getPosition().x + time * ball_x_speed +ball_radius >right_bar->getPosition().x-1)						//ball hits the right bar
		{
			if ( (ball->getPosition().y < right_bar->getPosition().y +right_bar_width/2) && (ball->getPosition().y > right_bar->getPosition().y -right_bar_width/2) )			//right bar gets the ball
			{
				ball_x_speed = -ball_x_speed;
			}
			else																		   //right bar loses the ball
			{
				state = 0;
				leftScore++;
				text1 = "Right player lose";
			}
		}
		break;

	case 2:							//2P moving state
		text1 = "Special event dialog:";
		specialEventAppear(time);	//start special event
		if (ball->getPosition().y + time * ball_y_speed +ball_radius> upBound->getPosition().y-1)			//ball hits the upbound
			ball_y_speed = -ball_y_speed;
		else if (ball->getPosition().y + time * ball_y_speed - ball_radius<downBound->getPosition().y+1 )		//ball hits the downbound
			ball_y_speed = -ball_y_speed;
		
		if ( ball->getPosition().x + time * ball_x_speed -ball_radius < left_bar->getPosition().x+1)							//ball hits the left bar
		{
			if ( (ball->getPosition().y < left_bar->getPosition().y +left_bar_width/2) && (ball->getPosition().y > left_bar->getPosition().y -left_bar_width/2) )			//left bar gets the ball
			{
				ball_x_speed = -ball_x_speed;
				if (mInputHandler->IsKeyDown(OIS::KC_W) && (left_bar->getPosition().y +left_bar_width/2 < upBound->getPosition().y-1))		//add spin (move the ball in bar-moving direction)
				{
					if (ball-> getPosition().y +2 + ball_radius < upBound->getPosition().y-1)		//ball can't be moved over the upbound
						ball ->translate (0, 2, 0);
					ball_y_speed +=5;
				}
				else if (mInputHandler->IsKeyDown(OIS::KC_S)  && (left_bar->getPosition().y -left_bar_width/2 >downBound->getPosition().y+1))	//add spin (move the ball in bar-moving direction)
				{
					if (ball-> getPosition().y -2 + ball_radius > upBound->getPosition().y+1)	//ball can't be moved below the downbound
						ball ->translate (0, -2, 0);
					ball_y_speed -=5;
				}
			}
			else																			//left bar loses the ball
			{
				state = 0;
				rightScore++;
				text1 = "Left player lose";
			}
		}
		else if ( ball->getPosition().x + time * ball_x_speed +ball_radius >right_bar->getPosition().x-1)						//ball hits the right bar
		{
			if ( (ball->getPosition().y < right_bar->getPosition().y +right_bar_width/2) && (ball->getPosition().y > right_bar->getPosition().y -right_bar_width/2) )			//right bar gets the ball
			{
				ball_x_speed = -ball_x_speed;
				if (mInputHandler->IsKeyDown(OIS::KC_UP) && (right_bar->getPosition().y +right_bar_width/2 < upBound->getPosition().y-1))	//add spin
				{
					if (ball-> getPosition().y +2 + ball_radius < upBound->getPosition().y-1)		//ball can't be moved over the upbound
						ball ->translate (0, 2, 0);
					ball_y_speed +=5;
				}
				else if (mInputHandler->IsKeyDown(OIS::KC_DOWN)  && (right_bar->getPosition().y -right_bar_width/2 >downBound->getPosition().y+1))	//add spin
					if (ball-> getPosition().y -2 + ball_radius > upBound->getPosition().y+1)	//ball can't be moved below the downbound
						ball ->translate (0, -2, 0);
					ball_y_speed -=5;
			}
			else																		   //right bar loses the ball
			{
				state = 0;
				leftScore++;
				text1 = "Right player lose";
			}
		}
		break;

	default:
		break;
	} 
}

void
World::controlPart(float time)
{
	// We can move the single object around using the input manager ...
	if (state != -1)		//In menu, pressing up  shouldn't move the paddle.
		if (choose_state == 0)			//1P
		{
			if ((mInputHandler->IsKeyDown(OIS::KC_W) || mInputHandler->IsKeyDown(OIS::KC_UP)) && (left_bar->getPosition().y +left_bar_width/2 < upBound->getPosition().y-1) )		//move the left bar up
			 {
				 left_bar->translate(0, time * left_bar_speed, 0);
				 leftMonkey->translate(0, time * left_bar_speed, 0);
			 }
			 else if ((mInputHandler->IsKeyDown(OIS::KC_S) || mInputHandler->IsKeyDown(OIS::KC_DOWN)) && (left_bar->getPosition().y -left_bar_width/2 >downBound->getPosition().y+1) )		//move the left bar down
			 {
				 left_bar->translate(0, -time * left_bar_speed, 0);
				 leftMonkey->translate(0, -time * left_bar_speed, 0);
			 }
		}
		else if  (choose_state == 1)		//2P
		{
			if (mInputHandler->IsKeyDown(OIS::KC_W) && (left_bar->getPosition().y +left_bar_width/2 < upBound->getPosition().y-1) )		//move the left bar up
			 {
				 left_bar->translate(0, time * left_bar_speed, 0);
				 leftMonkey->translate(0, time * left_bar_speed, 0);
			 }
			 else if (mInputHandler->IsKeyDown(OIS::KC_S) && (left_bar->getPosition().y -left_bar_width/2 >downBound->getPosition().y+1) )		//move the left bar down
			 {
				 left_bar->translate(0, -time * left_bar_speed, 0);
				 leftMonkey->translate(0, -time * left_bar_speed, 0);
			 }

			if (mInputHandler->IsKeyDown(OIS::KC_UP) && (right_bar->getPosition().y +right_bar_width/2 < upBound->getPosition().y-1) )		//move the right bar up
			 {
				 right_bar->translate(0, time * left_bar_speed, 0);
				 rightMonkey->translate(0, time * left_bar_speed, 0);
			 }
			 else if (mInputHandler->IsKeyDown(OIS::KC_DOWN) && (right_bar->getPosition().y -right_bar_width/2 >downBound->getPosition().y+1) )		//move the right bar down
			 {
				 right_bar->translate(0, -time * right_bar_speed, 0);
				 rightMonkey->translate(0, -time * right_bar_speed, 0);
			 }
		}
}

void 
World::specialEventAppear(float time)
{
	switch (special_event)		//special event select
		{
		case 0:						//special event countdown
			if (timer1->getMilliseconds() > countdown_time )
			{
				//special_event = 7;
				special_event = rand()%7+1;
			}
			event_start = true;

			break;
		case 1:						//drop a brick
			if (event_start)	
			{
				do   //When the bricks appears, the ball can't contain in the bricks
				{
					drop_x = Ogre::Math::RangeRandom(-10,10);
					drop_y = Ogre::Math::RangeRandom(-10,10);
				} while ((ball->getPosition().x-1 < drop_x+1) && (ball->getPosition().x+1 > drop_x-1) && (ball->getPosition().y < drop_y +drop_width/2) && (ball->getPosition().y > drop_y -drop_width/2));
				text2 = "Brick appears";
				drop ->attachObject(ent8);
				
				drop->setPosition(drop_x,drop_y,0);
				event_start = false;
			}

			
			if ( (ball->getPosition().x-ball_radius + time * ball_x_speed < drop->getPosition().x+1) && (ball->getPosition().x+ball_radius + time * ball_x_speed > drop->getPosition().x-1))			//ball hits the brick left or right
				if ((ball->getPosition().y < drop_y +drop_width/2) && (ball->getPosition().y > drop_y -drop_width/2))
					ball_x_speed = -ball_x_speed;

			if ( (ball->getPosition().x-ball_radius  < drop->getPosition().x+1) && (ball->getPosition().x+ball_radius > drop->getPosition().x-1))			//ball hits the brick up or down
				if ((ball->getPosition().y + time * ball_y_speed < drop_y +drop_width/2) && (ball->getPosition().y  + time * ball_y_speed > drop_y -drop_width/2))
					ball_y_speed = -ball_y_speed;
			
			if (event_timer2->getMilliseconds() > event_countdown_time)
			{
				specialEventReset();
			}
			break;

		case 2:		//bonus widen my paddle
			if (event_start)
			{
				bonus->attachObject(ent9);
				
				do   //When the bricks appears, the ball can't contain in the bonus object
				{
					drop_x = Ogre::Math::RangeRandom(-10,10);
					drop_y = Ogre::Math::RangeRandom(-10,10);
				} while (calc_distance(ball->getPosition().x,ball->getPosition().y, drop_x,drop_y)<= (bonus_radius+ ball_radius));
				text2 = "Bonus appears(widen my paddle)";
				bonus->setPosition(drop_x,drop_y,0);
				event_start = false;
			}
			bonus->yaw(Ogre::Radian(time * RADIANS_PER_SECOND));
			
			
			if ( !event_trigged && (calc_distance(ball->getPosition().x,ball->getPosition().y, drop_x,drop_y) <= (bonus_radius+ ball_radius)))			//ball hits the bonus and can't hit the bonus twice
				{
					if (ball_x_speed>0)
					{
						left_bar_width = 2*original_bar_width;
						
						text2 = "Left got twice width";
					}
					else
					{
						right_bar_width = 2*original_bar_width;
						text2 = "Right got twice width";
					}
					left_bar->scale(1,left_bar_width/original_bar_width,1);
					right_bar->scale(1,right_bar_width/original_bar_width,1);
					bonus->detachAllObjects();
					event_timer2->reset();
					
					event_trigged = true;
				}

			if (event_timer2->getMilliseconds() > event_countdown_time)
			{
				specialEventReset();
			}
					
			break;
		case 3:		//bonus shorten rival's paddle
			if (event_start)
			{
				bonus->attachObject(ent10);
				
				do   //When the bricks appears, the ball can't contain in the bonus object
				{
					drop_x = Ogre::Math::RangeRandom(-10,10);
					drop_y = Ogre::Math::RangeRandom(-10,10);
				} while (calc_distance(ball->getPosition().x,ball->getPosition().y, drop_x,drop_y)<= (bonus_radius+ ball_radius));
				text2 = "Bonus appears(shorten rival's paddle)";
				bonus->setPosition(drop_x,drop_y,0);
				event_start = false;
			}

			bonus->yaw(Ogre::Radian(time * RADIANS_PER_SECOND));
			
			
			if ( !event_trigged && (calc_distance(ball->getPosition().x,ball->getPosition().y, drop_x,drop_y) <= (bonus_radius+ ball_radius)))			//ball hits the bonus and can't hit the bonus twice
				{
					if (ball_x_speed>0)
					{
						right_bar_width = original_bar_width*3/4;
						
						text2 = "right paddle is shortened";
					}
					else
					{
						left_bar_width = original_bar_width*3/4;
						text2 = "left paddle is shortened";
					}
					left_bar->scale(1,left_bar_width/original_bar_width,1);
					right_bar->scale(1,right_bar_width/original_bar_width,1);
					bonus->detachAllObjects();
					event_timer2->reset();
					event_trigged = true;
				}

			if (event_timer2->getMilliseconds() > event_countdown_time)
			{
				specialEventReset();
			}
					
			break;
		case 4:		//wormhole
			if (event_start)	
			{
				bonus->attachObject(ent11);
				bonus2->attachObject(ent12);
				
				do   //When the bricks appears, the ball can't contain in the bonus object
				{
					wormhole1_x = Ogre::Math::RangeRandom(-20,20);
					wormhole1_y = Ogre::Math::RangeRandom(-10,0);
				} while (calc_distance(ball->getPosition().x,ball->getPosition().y, wormhole1_x,wormhole1_y)<= (bonus_radius+ ball_radius));

				do   //When the bricks appears, the ball can't contain in the bonus object
				{
					wormhole2_x = Ogre::Math::RangeRandom(-20,20);
					wormhole2_y = Ogre::Math::RangeRandom(0,10);
				} while (calc_distance(ball->getPosition().x,ball->getPosition().y, wormhole2_x,wormhole2_y)<= (bonus_radius+ ball_radius));

				text2 = "Wormhole appears";
				bonus->setPosition(wormhole1_x,wormhole1_y,0);
				bonus2->setPosition(wormhole2_x,wormhole2_y,0);
				event_start = false;
				
			}
			bonus->yaw(Ogre::Radian(time * RADIANS_PER_SECOND));
			bonus->roll(Ogre::Radian(time * RADIANS_PER_SECOND));
			bonus2->roll(Ogre::Radian(-time * RADIANS_PER_SECOND));

			
			if (  (calc_distance(ball->getPosition().x,ball->getPosition().y, wormhole1_x,wormhole1_y) <= (bonus_radius+ ball_radius)))			//ball hits the wormhole1 and is moved to wormhole2
				{
					ball->setPosition(wormhole2_x,wormhole2_y,0);
				}
			/*
			if (  (calc_distance(ball->getPosition().x,ball->getPosition().y, wormhole2_x,wormhole2_y) <= (bonus_radius+ ball_radius)))			//ball hits the wormhole2 and is moved to wormhole1
				{
					ball->setPosition(wormhole1_x,wormhole1_y,0);
				}*/
			
			
			if (event_timer2->getMilliseconds() > event_countdown_time)
			{
				specialEventReset();
			}
			break;
		case 5:		//bonus speed up my paddle
			if (event_start)
			{
				bonus->attachObject(ent13);
				
				do   //When the bricks appears, the ball can't contain in the bonus object
				{
					drop_x = Ogre::Math::RangeRandom(-10,10);
					drop_y = Ogre::Math::RangeRandom(-10,10);
				} while (calc_distance(ball->getPosition().x,ball->getPosition().y, drop_x,drop_y)<= (bonus_radius+ ball_radius));
				text2 = "Bonus appears(speed up my paddle)";
				bonus->setPosition(drop_x,drop_y,0);
				event_start = false;
			}

			bonus->yaw(Ogre::Radian(time * RADIANS_PER_SECOND));
			
			
			if ( !event_trigged && (calc_distance(ball->getPosition().x,ball->getPosition().y, drop_x,drop_y) <= (bonus_radius+ ball_radius)))			//ball hits the bonus and can't hit the bonus twice
				{
					if (ball_x_speed>0)
					{
						left_bar_speed *= 1.5;
						
						text2 = "Left paddle is faster";
					}
					else
					{
						right_bar_speed *= 1.5;
						text2 = "Right paddle is faster";
					}
					bonus->detachAllObjects();
					event_timer2->reset();
					event_trigged = true;
				}
			if (event_timer2->getMilliseconds() > event_countdown_time)
			{
				specialEventReset();
			}
			break;
		case 6:		//bonus slow down rival's paddle
			if (event_start)
			{
				bonus->attachObject(ent14);
				
				do   //When the bricks appears, the ball can't contain in the bonus object
				{
					drop_x = Ogre::Math::RangeRandom(-10,10);
					drop_y = Ogre::Math::RangeRandom(-10,10);
				} while (calc_distance(ball->getPosition().x,ball->getPosition().y, drop_x,drop_y)<= (bonus_radius+ ball_radius));
				text2 = "Bonus appears(slow down rival's paddle)";
				bonus->setPosition(drop_x,drop_y,0);
				event_start = false;
			}

			bonus->yaw(Ogre::Radian(time * RADIANS_PER_SECOND));
			
			
			if ( !event_trigged && (calc_distance(ball->getPosition().x,ball->getPosition().y, drop_x,drop_y) <= (bonus_radius+ ball_radius)))			//ball hits the bonus and can't hit the bonus twice
				{
					if (ball_x_speed>0)
					{
						right_bar_speed *= 0.8;
						
						text2 = "Right paddle is slower";
					}
					else
					{
						left_bar_speed *= 0.8;
						text2 = "Left paddle is slower";
					}
					bonus->detachAllObjects();
					event_timer2->reset();
					event_trigged = true;
				}
			if (event_timer2->getMilliseconds() > event_countdown_time)
			{
				specialEventReset();
			}
					
			break;
		case 7:		//gravity
			if (event_start)
			{
				text2 = "Gravity";
				event_start = false;
			}
			

			ball_y_speed -= time * 50;


			if (event_timer2->getMilliseconds() > event_countdown_time)
			{
				specialEventReset();
			}
					
			break; 
		default:
			break;
		}
}


void 
World::specialEventReset()
{
	
	timer1->reset();
	event_timer2->reset();
	text2 = "";
	
	switch (special_event)
	{
		case 1:		//brick
			drop->detachAllObjects();
			break;
		case 2:		//widen
		case 3:		//shorten
			bonus->detachAllObjects();
			if (event_trigged)
			{
				left_bar->scale(1,original_bar_width/left_bar_width,1);
				right_bar->scale(1,original_bar_width/right_bar_width,1);
				left_bar_width = original_bar_width;
				right_bar_width = original_bar_width;
			}
			break;
		case 4:		//wormhole
			bonus->detachAllObjects();
			bonus2->detachAllObjects();
			break;
		case 5:		//speed up
		case 6:		//slow down
			bonus->detachAllObjects();
			if (event_trigged)
			{
				left_bar_speed = original_bar_speed;
				right_bar_speed = original_bar_speed;
			}
			break;
		case 7:
			break;
		default:
			break;
	}
	event_trigged = false;
	special_event = 0;

	

}

void 
World::rotateBall(float time)			//rotate  ball
{
	if (ball_x_speed > 0)
		ball_x_speed += time * 2;		//ball x speed up
	else if (ball_x_speed < 0)
		ball_x_speed -= time * 2;
	// Some other fun stuff to try:   ball roll pitch yaw all the time
	 ball->pitch(Ogre::Radian(time * RADIANS_PER_SECOND));
	 ball->yaw(Ogre::Radian(time * RADIANS_PER_SECOND));
	 ball->roll(Ogre::Radian(time * RADIANS_PER_SECOND));
}


void 
World::displayText(float time)			//display text
{
	Ogre::OverlayManager& om = Ogre::OverlayManager::getSingleton();			//display score

	Ogre::TextAreaOverlayElement *menu1 = (Ogre::TextAreaOverlayElement *) om.getOverlayElement("Choose/Panel/Text1");	//menu  choose 1P OR 2P
	 menu1->setCaption(choosetext1);
	 Ogre::TextAreaOverlayElement *menu2 = (Ogre::TextAreaOverlayElement *) om.getOverlayElement("Choose/Panel/Text2");		//display the special event
	 menu2->setCaption(choosetext2);

	
	 //timer debug
	 //text1 ="delta"+ Ogre::StringConverter::toString(time)+"countdown_time"+ Ogre::StringConverter::toString(countdown_time)+"event_countdown_time"+ Ogre::StringConverter::toString(event_countdown_time);
	 //text2 ="countdown" +Ogre::StringConverter::toString(timer1->getMilliseconds())+"event countdown"+Ogre::StringConverter::toString(event_timer2->getMilliseconds());//+"deltaTime"+Ogre::StringConverter::toString(deltaTime);

	

	Ogre::TextAreaOverlayElement *te = (Ogre::TextAreaOverlayElement *) om.getOverlayElement("Sample/Panel/Text2");
	te->setCaption( Ogre::StringConverter::toString(leftScore)+" : "+Ogre::StringConverter::toString(rightScore));
	 Ogre::TextAreaOverlayElement *dia1 = (Ogre::TextAreaOverlayElement *) om.getOverlayElement("Dialog/Panel/Text1");
	 dia1->setCaption(text1);
	 Ogre::TextAreaOverlayElement *dia2 = (Ogre::TextAreaOverlayElement *) om.getOverlayElement("Dialog/Panel/Text2");		//display the special event
	 dia2->setCaption(text2);
}

float
World::calc_distance(float x1, float y1, float x2, float y2)
{
	return sqrt( pow((x1-x2),2) + pow(y1-y2,2));
}


