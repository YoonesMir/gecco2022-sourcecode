/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "TemplateVanillaEE/include/TemplateVanillaEEAgentObserver.h"
#include "TemplateVanillaEE/include/TemplateVanillaEEController.h"
#include "WorldModels/RobotWorldModel.h"
#include "World/PhysicalObject.h"

TemplateVanillaEEAgentObserver::TemplateVanillaEEAgentObserver( RobotWorldModel *wm ) : TemplateEEAgentObserver ( wm )
{
    // superclass constructor called before
}

TemplateVanillaEEAgentObserver::~TemplateVanillaEEAgentObserver()
{
    // superclass destructor called before
}

/*
 * Manage foraging of energy items.
 * If walked on, the item disappear (callback to item object) and agent's fitness function is updated.
 * Assume that *only* energy point's footprint can be walked upon. That means that for this code to correctly run you should avoid placing any objects detectable through the robot's ground sensor.
 *
 */
void TemplateVanillaEEAgentObserver::stepPre()
{
    // * update fitness (if needed)
    if ( _wm->isAlive() )
    {

        if ( PhysicalObject::isInstanceOf(_wm->getGroundSensorValue()) )
        {
            dynamic_cast<TemplateVanillaEEController*>(getController())->updateFitness2(1);
        }
        else
        {
            dynamic_cast<TemplateVanillaEEController*>(getController())->updateFitness2(0);
        }
        /*
        if (dynamic_cast<TemplateVanillaEEController*>(getController())->getIterations()< 1000000){
            if ( PhysicalObject::isInstanceOf(_wm->getGroundSensorValue()) )
            {
                dynamic_cast<TemplateVanillaEEController*>(getController())->updateFitness2(1);
            }
            else
            {
                dynamic_cast<TemplateVanillaEEController*>(getController())->updateFitness2(0);
            }

        } else {
            if ( _wm->isAlive() && PhysicalObject::isInstanceOf(_wm->getGroundSensorValue()) )
            {
                _wm->_fitnessValue = _wm->_fitnessValue + 1;
            }

        }
        */

    }

    TemplateEEAgentObserver::stepPre();
}
