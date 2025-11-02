/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2006 Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef BULLET_COLLISION_COMMON_H
#define BULLET_COLLISION_COMMON_H

///Common headerfile includes for Bullet Collision Detection

///Bullet's btCollisionWorld and btCollisionObject definitions
#include "chrono/collision/bullet/BulletCollision/CollisionDispatch/btCollisionWorld.h"
#include "chrono/collision/bullet/BulletCollision/CollisionDispatch/btCollisionObject.h"

///Collision Shapes
#include "chrono/collision/bullet/BulletCollision/CollisionShapes/btBoxShape.h"
#include "chrono/collision/bullet/BulletCollision/CollisionShapes/btSphereShape.h"
#include "chrono/collision/bullet/BulletCollision/CollisionShapes/btCapsuleShape.h"
#include "chrono/collision/bullet/BulletCollision/CollisionShapes/btCylinderShape.h"
#include "chrono/collision/bullet/BulletCollision/CollisionShapes/btCylindricalShellShape.h"  /* ***CHRONO*** */
#include "chrono/collision/bullet/BulletCollision/CollisionShapes/btConeShape.h"
#include "chrono/collision/bullet/BulletCollision/CollisionShapes/btStaticPlaneShape.h"
#include "chrono/collision/bullet/BulletCollision/CollisionShapes/btConvexHullShape.h"
#include "chrono/collision/bullet/BulletCollision/CollisionShapes/btTriangleMesh.h"
#include "chrono/collision/bullet/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.h"
#include "chrono/collision/bullet/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h"
#include "chrono/collision/bullet/BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.h"
#include "chrono/collision/bullet/BulletCollision/CollisionShapes/btTriangleMeshShape.h"
#include "chrono/collision/bullet/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.h"
#include "chrono/collision/bullet/BulletCollision/CollisionShapes/btCompoundShape.h"
#include "chrono/collision/bullet/BulletCollision/CollisionShapes/btTetrahedronShape.h"
#include "chrono/collision/bullet/BulletCollision/CollisionShapes/btEmptyShape.h"
#include "chrono/collision/bullet/BulletCollision/CollisionShapes/btMultiSphereShape.h"
#include "chrono/collision/bullet/BulletCollision/CollisionShapes/btUniformScalingShape.h"

///Narrowphase Collision Detector
#include "chrono/collision/bullet/BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.h"

//#include "chrono/collision/bullet/BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.h"
#include "chrono/collision/bullet/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h"

///Dispatching and generation of collision pairs (broadphase)
#include "chrono/collision/bullet/BulletCollision/CollisionDispatch/btCollisionDispatcher.h"
#include "chrono/collision/bullet/BulletCollision/BroadphaseCollision/btSimpleBroadphase.h"
#include "chrono/collision/bullet/BulletCollision/BroadphaseCollision/btAxisSweep3.h"
#include "chrono/collision/bullet/BulletCollision/BroadphaseCollision/btDbvtBroadphase.h"

///Math library & Utils
#include "chrono/collision/bullet/LinearMath/btQuaternion.h"
#include "chrono/collision/bullet/LinearMath/btTransform.h"
#include "chrono/collision/bullet/LinearMath/btDefaultMotionState.h"
#include "chrono/collision/bullet/LinearMath/btQuickprof.h"
#include "chrono/collision/bullet/LinearMath/btIDebugDraw.h"
#include "chrono/collision/bullet/LinearMath/btSerializer.h"

#endif  //BULLET_COLLISION_COMMON_H
