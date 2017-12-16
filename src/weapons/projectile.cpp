#include "projectile.h"

Projectile::Projectile(scene::IBillboardSceneNode * spriteIn, btVector3 * positionIn, core::vector3df trajectoryIn, u32 destroyTimeIn, scene::IParticleSystemSceneNode * impactIn, f32 speed, f32 damageIn) {
	//btVector3 * position = positionIn;
	position = *positionIn;
	impact = impactIn;
	trajectory = trajectoryIn * speed; //speed modifier
	//INextPos = trajectory;

	damage = damageIn;

	//nextPos = btVector3(position.getX() + trajectory.X, position.getY() + trajectory.Y, position.getZ() + trajectory.Z);
	nextPos = position;
	//btrajectory = new btVector3(trajectoryIn->X, trajectoryIn->Y, trajectoryIn->Z);
	//batra
	//position = new btVector3(sprite->getPosition().X, sprite->getPosition().Y, sprite->getPosition().Z);
	sprite = spriteIn;
	sprite->setSize(0, 0, 0);
	destroyTime = destroyTimeIn; // X seconds to live
}

bool Projectile::update(u32 currTime, btCollisionWorld * world, u32 TDeltaTime) {
		
	if (currTime < destroyTime) {

		btCollisionWorld::ClosestRayResultCallback RayCallback(position, nextPos);
		world->rayTest(position, nextPos, RayCallback);

			//RayCallback.
    
		if (RayCallback.hasHit()) {

			//printf("projectile destroyed");

			Entity * object = static_cast <Entity*> (RayCallback.m_collisionObject->getUserPointer());

			object->damage(damage, &RayCallback.m_hitPointWorld, &position);

			/*if (RayCallback.m_collisionObject->getUserIndex() == 1) {

				printf("\nHEADSHOT\n");

				core::vector3df * hitpoint = new core::vector3df(RayCallback.m_hitPointWorld.x(),RayCallback.m_hitPointWorld.y(),RayCallback.m_hitPointWorld.z());

				object->damage(damage * 3, RayCallback.m_hitPointWorld);

			} else if(RayCallback.m_collisionObject->getUserIndex() == 0) {
				object->damage(damage, &core::vector3df(RayCallback.m_hitPointWorld.x(),RayCallback.m_hitPointWorld.y(),RayCallback.m_hitPointWorld.z()));

			} else if (RayCallback.m_collisionObject->getUserIndex() == 3) {
				printf("shield hit");
			}*/

				
			//printf("projectile has hit");

			sprite->setVisible(false);

			impact->setPosition(core::vector3df(RayCallback.m_hitPointWorld.getX(), RayCallback.m_hitPointWorld.getY(), RayCallback.m_hitPointWorld.getZ()));

			//impact->setParent(sprite);
			impact->setVisible(true);

			destroyTime = currTime;

			//return false;

		} else {
			position = nextPos;

			sprite->setPosition(core::vector3df(position.x(),position.y(),position.z()));

			//sprite->setPosition(pos);
			nextPos.setX(position.getX() + trajectory.X * TDeltaTime);
			nextPos.setY(position.getY() + trajectory.Y * TDeltaTime);
			nextPos.setZ(position.getZ() + trajectory.Z * TDeltaTime);
			//printf("updating projectile");
				
		}

		return true;			
			
	} else {
		//impact->remove();

		impact->setEmitter(0);
		impact->remove();
		sprite->remove();
		//delete sprite;
		//printf("removed sprite");

		//delete position;
		//delete impact;
		//delete sprite;
		delete this;
		return false;
	}
}