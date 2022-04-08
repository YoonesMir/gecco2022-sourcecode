/*
 *  WorldObserver.cpp
 *  roborobo-online
 *
 *  Created by Nicolas on 20/03/09.
 *  Copyright 2009. All rights reserved.
 *
 */

#include "Observers/WorldObserver.h"
#include "World/World.h"


WorldObserver::WorldObserver( World *__world ) 
{
	_world = __world;
}

WorldObserver::~WorldObserver()
{
	// nothing to do.
}

void WorldObserver::initPre() // called *before* first initializing Objects and Robots
{
	std::cout<<"WorldObserver::initPre\n";
	// nothing to do.
}

void WorldObserver::initPost() // called *after* first initializing Objects and Robots
{
	std::cout<<"WorldObserver::initPost\n";
    // nothing to do.
}

void WorldObserver::stepPre() // see header file for remark on stepPre/stepPost
{
	std::cout<<"WorldObserver::stepPre\n";
	// nothing to do.
}

void WorldObserver::stepPost()
{
	std::cout<<"WorldObserver::stepPost\n";
    // nothing to do.
}
