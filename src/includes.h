#pragma once

#ifdef WIN32

#include <irrlicht.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include "BulletCollision/CollisionDispatch/btGhostObject.h"

#include <cstdlib>

using namespace irr;
#else

#include <irrlicht/irrlicht.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include "BulletCollision/CollisionDispatch/btGhostObject.h"

#include <cstdlib>

using namespace irr;
#endif
