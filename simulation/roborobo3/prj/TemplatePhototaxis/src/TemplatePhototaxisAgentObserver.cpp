/**
 * @author Yoones Mirhosseini 12-08-2020
 *
 */

#include "TemplatePhototaxis/include/TemplatePhototaxisAgentObserver.h"


TemplatePhototaxisAgentObserver::TemplatePhototaxisAgentObserver( RobotWorldModel *wm )
{ 
     _wm = (RobotWorldModel*)wm;
}

TemplatePhototaxisAgentObserver::~TemplatePhototaxisAgentObserver()
{
    // superclass destructor called before
}

void TemplatePhototaxisAgentObserver::reset()
{
    // nothing to do.
}

void TemplatePhototaxisAgentObserver::stepPre()
{  
    // nothing to do.
}

void TemplatePhototaxisAgentObserver::stepPost(){

  // nothing to do.
}