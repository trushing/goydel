#include "entity.h"

//Hoping to use this class to abstract irrlicht and bullet objects into a single body.

Entity::Entity(scene::ISceneManager * irrSceneIn, btDiscreteDynamicsWorld *bullSceneIn, io::path pathName, btVector3 * position, btScalar TMass, bool capsule) {

	mountPoint = NULL;

	pointer = NULL;

	next = NULL;
	prev = NULL;
	
	//Invulnerable
	health = -1;

	irrScene = irrSceneIn;
	bullScene = bullSceneIn;

	node = irrScene->addEmptySceneNode();

	scene::IAnimatedMesh * mesh = irrScene->getMesh(pathName);
	render = irrScene->addAnimatedMeshSceneNode( mesh );

	//render->setParent(node);

	render->setMaterialFlag(video::EMF_LIGHTING, true);
	render->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

	//render->addShadowVolumeSceneNode();

	//node->setPosition(core::vector3df(position->x(), position->y(), position->z()));

	render->setPosition(core::vector3df(0,-render->getTransformedBoundingBox().getExtent().Y / 2,0));

	//Bullet body

	btCollisionShape *Shape;

	if (capsule) {
		Shape = new btCapsuleShape(render->getTransformedBoundingBox().getExtent().Y / 2, render->getTransformedBoundingBox().getExtent().X / 2);
	} else {

		//if (capsule == 2) {
			Shape = new btBoxShape(btVector3(render->getTransformedBoundingBox().getExtent().X / 2, render->getTransformedBoundingBox().getExtent().Y / 2, render->getTransformedBoundingBox().getExtent().Z / 2));		
		//} else {
			//btTriangleMesh* trimesh = IrrMeshToBullet(render->getMesh(), render->getScale());
			//Shape = new btConvexTriangleMeshShape(trimesh);
		//}
	}
	// Add mass
	btVector3 LocalInertia;
	Shape->calculateLocalInertia(TMass, LocalInertia);

	// Create the rigid body object
	btTransform Transform;
	Transform.setIdentity();
	Transform.setOrigin(*position);

	btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);
	physics = new btRigidBody(TMass, MotionState, Shape, LocalInertia);

	physics->setUserPointer(this);

	bullScene->addRigidBody(physics);
	
	physics->setFriction(1.f);
	
	physics->setActivationState(DISABLE_DEACTIVATION);

	render->setMaterialFlag(video::EMF_FOG_ENABLE, true);

	node->setPosition(core::vector3df(physics->getCenterOfMassPosition().x(), physics->getCenterOfMassPosition().y(), physics->getCenterOfMassPosition().z()));
	render->setPosition(node->getPosition());
	
	core::vector3df Euler;

	const btQuaternion& TQuat = physics->getOrientation();
	core::quaternion q(TQuat.getX(), TQuat.getY(), TQuat.getZ(), TQuat.getW());
	q.toEuler(Euler);
	Euler *= core::RADTODEG;
	node->setRotation(Euler);
	render->setRotation(Euler);

	physics->getCollisionShape()->setMargin(1.5);
		
}

btTriangleMesh* Entity::IrrMeshToBullet(scene::IMesh* pMesh,const core::vector3df& scaling)
{
	//Taken from http://irrlicht.sourceforge.net/forum/viewtopic.php?t=39007

	btVector3 vertices[3];
	u32 i,j,k,index,numVertices,numIndices;
	u16* mb_indices;
	btTriangleMesh *pTriMesh = new btTriangleMesh();
	for (i=0; i<pMesh->getMeshBufferCount(); i++)
	{
		scene::IMeshBuffer* mb=pMesh->getMeshBuffer(i);
		if(mb->getVertexType()==video::EVT_STANDARD)
		{
			video::S3DVertex* mb_vertices=(video::S3DVertex*)mb->getVertices();
			mb_indices = mb->getIndices();
			numVertices = mb->getVertexCount();
			numIndices = mb->getIndexCount();
			for(j=0;j<numIndices;j+=3)
			{
				for (k=0;k<3;k++)
				{
					index = mb_indices[j+k];
					vertices[k] = btVector3(mb_vertices[index].Pos.X*scaling.X, mb_vertices[index].Pos.Y*scaling.Y, mb_vertices[index].Pos.Z*scaling.Z);
				}
				pTriMesh->addTriangle(vertices[0], vertices[1], vertices[2]);
			}
		}
		else if(mb->getVertexType()==video::EVT_2TCOORDS)
		{
			video::S3DVertex2TCoords* mb_vertices=(video::S3DVertex2TCoords*)mb->getVertices();
			mb_indices = mb->getIndices();
			numVertices = mb->getVertexCount();
			numIndices = mb->getIndexCount();
			for(j=0;j<numIndices;j+=3)
			{
				for (k=0;k<3;k++)
				{
					index = mb_indices[j+k];
					vertices[k] = btVector3(mb_vertices[index].Pos.X*scaling.X, mb_vertices[index].Pos.Y*scaling.Y, mb_vertices[index].Pos.Z*scaling.Z);
				}
				pTriMesh->addTriangle(vertices[0], vertices[1], vertices[2]);
			}
		}
	}
	return pTriMesh;
}

void Entity::makeConcave() {

	
	btCollisionShape *Shape;

	btTriangleMesh* trimesh = IrrMeshToBullet(render->getMesh(), render->getScale());
	Shape = new btBvhTriangleMeshShape(trimesh, false);

	btVector3 LocalInertia;

	Shape->calculateLocalInertia(physics->getInvMass(), LocalInertia);

	physics->setCollisionShape(Shape);
	//physics = new btRigidBody(physics->getInvMass(), MotionState, Shape, LocalInertia);

	node->setPosition(core::vector3df(physics->getCenterOfMassPosition().x(), physics->getCenterOfMassPosition().y(), physics->getCenterOfMassPosition().z()));
	render->setPosition(node->getPosition());

	//render->setRotation(core::vector3df(180, 0, 
}

void Entity::makeConvex() {
	
	btCollisionShape *Shape;

	btTriangleMesh* trimesh = IrrMeshToBullet(render->getMesh(), render->getScale());
	Shape = new btConvexTriangleMeshShape(trimesh);

	btVector3 LocalInertia;

	Shape->calculateLocalInertia(physics->getInvMass(), LocalInertia);

	physics->setCollisionShape(Shape);
	//physics = new btRigidBody(physics->getInvMass(), MotionState, Shape, LocalInertia);

	node->setPosition(core::vector3df(physics->getCenterOfMassPosition().x(), physics->getCenterOfMassPosition().y(), physics->getCenterOfMassPosition().z()));
	render->setPosition(node->getPosition());

	//render->setRotation(core::vector3df(180, 0, 
}

void Entity::setHealth(int newHealth) {
	health = newHealth;
}

void Entity::setId(int newId) {
	physics->setUserIndex(newId);
	//2 is vehicle
}

void Entity::mount(btVector3 * point) {
	mountPoint = point;
}

int Entity::getId() {
	return physics->getUserIndex();
}

void Entity::setPointer(void * newPointer) {
	pointer = newPointer;
}

void * Entity::getPointer() {
	return pointer;
}

void Entity::damage(f32 amount, btVector3 * hitpoint, btVector3 * hitorigin) {

	//destroy();

	printf("%i\n", health);
	//printf("%i\n", health);

	//physics->applyCentralForce((*hitpoint - *hitorigin) * 5000);

	btVector3 relPoint = *hitpoint;
	relPoint -= physics->getCenterOfMassPosition();

	physics->applyForce((*hitpoint - *hitorigin) * 5000, relPoint);

	if (health > 0) {

		if (health > amount) {
			health -= amount;
		} else {
			remove();
		}

	}
	//physics->applyForce((*hitpoint - *hitorigin), (*hitpoint - *hitorigin) * 5000);
}

void Entity::setMaterial(io::path pathName) {
	render->setMaterialType( video::EMT_SOLID );

	render->setMaterialTexture(0, irrScene->getVideoDriver()->getTexture(pathName));

	//render->setMaterialType( video::EMT_SOLID );
}

void Entity::setAutoClean(bool clean) {
	autoClean = clean;
}

void Entity::remove() {

	printf("\nremoving");

	prev->next = next;

	if (next !=NULL) {		
		next->prev = prev;
	} else {
		printf("\ndeleted tail");
	}
		
	node->remove();
	render->remove();
	bullScene->removeCollisionObject(physics);

	node = NULL;

	if (autoClean)
		delete this;

}

void Entity::setNext(Entity * entity) {
	next = entity;
}

void Entity::setPrev(Entity * entity) {
	prev = entity;
}