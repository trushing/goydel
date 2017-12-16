#pragma once
#include "../entity.h"

using namespace irr;

class Projectile {

private:

	f32 damage;

	u32 destroyTime;
	core::vector3df trajectory;	
	btVector3 position;
	btVector3 nextPos;
	//core::vector3df INextPos;
	//btVector3 * btrajectory;

	scene::IParticleSystemSceneNode* impact;

	scene::IBillboardSceneNode * sprite;

public:	

	Projectile(scene::IBillboardSceneNode * spriteIn, btVector3 * positionIn, core::vector3df trajectoryIn, u32 destroyTimeIn, scene::IParticleSystemSceneNode * impactIn, f32 speed, f32 damageIn);

	bool update(u32 currTime, btCollisionWorld * world, u32 TDeltaTime);

};
