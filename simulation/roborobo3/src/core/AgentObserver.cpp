/*
 *  AgentObserver.cpp
 *  roborobo-online
 *
 *  Created by Nicolas on 24/03/09.
 *  Copyright 2009. All rights reserved.
 *
 */

#include "Observers/AgentObserver.h"

AgentObserver::AgentObserver( )
{
}

AgentObserver::AgentObserver( RobotWorldModel *__wm )
{
	_wm = (RobotWorldModel*)__wm;
}

AgentObserver::~AgentObserver()
{
	// nothing to do.
}

void AgentObserver::reset()
{
    std::cout<<"AgentObserver::reset\n";
	// nothing to do.
}

void AgentObserver::stepPre()
{
    std::cout<<"AgentObserver::stepPre\n";
    // nothing to do.
}

void AgentObserver::stepPost()
{
    std::cout<<"AgentObserver::stepPost\n";
    // nothing to do.
}

Controller* AgentObserver::getController()
{
    return gWorld->getRobot(_wm->getId())->getController();
}
