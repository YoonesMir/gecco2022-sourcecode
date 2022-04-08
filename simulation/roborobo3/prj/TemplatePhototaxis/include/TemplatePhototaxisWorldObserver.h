/**
 *  @author Yoones Mirhosseini 12-08-2020
 *
 */

#ifndef TEMPLATEPHOTOTAXISWORLDOBSERVER_H
#define TEMPLATEPHOTOTAXISWORLDOBSERVER_H


#include "Observers/WorldObserver.h"
#include "WorldModels/RobotWorldModel.h"
#include "World/World.h"
#include "RoboroboMain/roborobo.h"
#include "Utilities/Graphics.h"

#include "TemplatePhototaxis/include/TemplatePhototaxisController.h"
#include "TemplatePhototaxis/include/TemplatePhototaxisSharedData.h"


class World;

class TemplatePhototaxisWorldObserver : public WorldObserver
{
public:
    TemplatePhototaxisWorldObserver(World *world);
    ~TemplatePhototaxisWorldObserver();
    
    void initPre() override;
    void initPost() override;

    void stepPre() override;
    void stepPost() override;
    
  

protected:  
    void updateEnvironment();
    void updateMonitoring();
    void monitorPopulation( bool localVerbose = true );
    
};

#endif



