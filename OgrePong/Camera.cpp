#include "OgreCamera.h"
#include "Camera.h"
#include "World.h"
#include "OgreVector3.h"
// IOS (Input system) header files


#include <ois/ois.h>


// Other input files for my project
#include "Camera.h"
#include "InputHandler.h"


PongCamera::PongCamera(Ogre::Camera *renderCamera, World *world, InputHandler *input) :
mRenderCamera(renderCamera), mWorld(world), mInputHandler(input)
{
 mRenderCamera->setNearClipDistance(2);
}


void
PongCamera::Think(float time)
{
 /*
 const float CAMERA_SPEED = 30;
 // Any code needed here to move the camera about per frame
 //  (use mRenderCamera to get the actual render camera, of course!)
 if (mInputHandler->IsKeyDown(OIS::KC_A))
  {
  
  if (mInputHandler->IsKeyDown( OIS::KC_LSHIFT ))
  //mRenderCamera->setPosition(mRenderCamera->getPosition() + Ogre::Vector3(time * CAMERA_SPEED, 0, 0));
 mRenderCamera->moveRelative(Ogre::Vector3(time * CAMERA_SPEED,0,0));
  else
  mRenderCamera->yaw(Ogre::Degree(0.5));
  }
  else if (mInputHandler->IsKeyDown(OIS::KC_D))
  {
  
  if (mInputHandler->IsKeyDown( OIS::KC_LSHIFT ))
  //mRenderCamera->setPosition(mRenderCamera->getPosition() - Ogre::Vector3(time * CAMERA_SPEED, 0, 0));
 mRenderCamera->moveRelative(Ogre::Vector3(-time * CAMERA_SPEED,0,0));
  else
  mRenderCamera->yaw(Ogre::Degree(-0.5));
  }
 else if (mInputHandler->IsKeyDown(OIS::KC_W))
  {
  if (mInputHandler->IsKeyDown( OIS::KC_LSHIFT ))
  //mRenderCamera->setPosition(mRenderCamera->getPosition() + Ogre::Vector3(0, time * CAMERA_SPEED, 0));
  mRenderCamera->moveRelative(Ogre::Vector3(0,time * CAMERA_SPEED,0));
  else
  mRenderCamera->pitch(Ogre::Degree(0.5));


  }
 else if (mInputHandler->IsKeyDown(OIS::KC_S))
  {
  if (mInputHandler->IsKeyDown( OIS::KC_LSHIFT ))
  //mRenderCamera->setPosition(mRenderCamera->getPosition() - Ogre::Vector3(0, time * CAMERA_SPEED, 0));
   mRenderCamera->moveRelative(Ogre::Vector3(0,-time * CAMERA_SPEED,0));
  else
 mRenderCamera->pitch(Ogre::Degree(-0.5));
  }
  */
  
  

}








