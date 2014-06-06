#include "AIManager.h"
#include "World.h"
// Ogre header files
#include "Ogre.h"
#include "OgreMath.h"
#include "OgreSceneManager.h"
#include "OgreSceneNode.h"
#include "OgreOverlayManager.h"
#include "OgreOverlay.h"
#include "OgreFontManager.h"
#include "OgreTextAreaOverlayElement.h"

AIManager::AIManager(World *world) : mWorld(world)
{
    // Any initialization that needs to be done ...

	
}

AIManager::~AIManager()
{
    // Clean up after yourself ...
}

void 
AIManager::Think(float time)
{
	

	// Do any "Global" AI thinking, and then call think for all of the actual AI's running about
	if (mWorld->choose_state == 0)
		if (mWorld->ball_x_speed>0)
			if (mWorld->ball->getPosition().y > mWorld->right_bar->getPosition().y && mWorld->right_bar->getPosition().y + mWorld->right_bar_width/2 < mWorld->upBound->getPosition().y-1)
			{
				mWorld->right_bar->translate(0,time * mWorld->right_bar_speed,0);
				mWorld->rightMonkey->translate(0,time * mWorld->right_bar_speed,0);
			}
			else if (mWorld->ball->getPosition().y < mWorld->right_bar->getPosition().y && mWorld->right_bar->getPosition().y -mWorld->right_bar_width/2 >mWorld->downBound->getPosition().y+1)
			{
				mWorld->right_bar->translate(0,-time * mWorld->right_bar_speed,0);
				mWorld->rightMonkey->translate(0,-time * mWorld->right_bar_speed,0);
			}

}

