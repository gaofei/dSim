//
//  PhysEngine.cpp
//  test
//
//  Created by XuHao on 14-4-15.
//  Copyright (c) 2014年 xuhao. All rights reserved.
//

#include "PhysEngine.h"
int PhysEngine::init()
{
    mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
    if(!mFoundation)
    {
        printf("Failed\n");
        return -1;
    }
    auto mProfileZoneManager = &PxProfileZoneManager::createProfileZoneManager(mFoundation);
    
    if(!mProfileZoneManager)
    {
        printf("PxProfileZoneManager::createProfileZoneManager failed!");
        return -1;
    }
    bool recordMemoryAllocations = true;
    mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation,
                               PxTolerancesScale(), recordMemoryAllocations, mProfileZoneManager );
    if(!mPhysics)
        printf("PxCreatePhysics failed!");
    sceneDesc=new PxSceneDesc(mPhysics->getTolerancesScale());
    sceneDesc->gravity = PxVec3(0.0f, 0, -9.78f);
    //customizeSceneDesc(sceneDesc);
    if(!sceneDesc->cpuDispatcher)
    {
        auto mCpuDispatcher = PxDefaultCpuDispatcherCreate(1);
        if(!mCpuDispatcher)
        {
            printf("PxDefaultCpuDispatcherCreate failed!");
            return -1;
        }
        sceneDesc->cpuDispatcher    = mCpuDispatcher;
    }
    
    if(!sceneDesc->filterShader)
    {
        sceneDesc->filterShader    = gDefaultFilterShader;
    }
    mScene = mPhysics->createScene(*sceneDesc);
    if (!mScene)
    {
        printf("createScene failed!");
        return -1;
    }
    return 0;
}

PhysEngine::PhysEngine()
{
    init();
    //setupActor();
    //sim(1);
    PxMaterial* aMaterial;
    aMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.5f);    //static friction, dynamic friction, restitution
    PxRigidStatic*plane;
    plane =  PxCreatePlane(*mPhysics, PxPlane(PxVec3(0,0,1), 0), *aMaterial);
    mScene->addActor(*plane);
}

/*
xmodel* PhysEngine::addDemo()
    {
        xmodel * x0 = new xmodel(this->mPhysics,this->mScene);
        return x0;
    }
*/
int PhysEngine::sim(double time)

{
    for (int j=0; j<time*10000; j++)
    {
        mScene->simulate(1e-4);
        mScene->fetchResults(true);
    }
    return 0;
}