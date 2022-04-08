#if defined PRJ_TEMPLATEPHOTOTAXIS|| !defined MODULAR

#include "Config/TemplatePhototaxisConfigurationLoader.h"

#include "TemplatePhototaxis/include/TemplatePhototaxisWorldObserver.h"
#include "TemplatePhototaxis/include/TemplatePhototaxisAgentObserver.h"
#include "TemplatePhototaxis/include/TemplatePhototaxisController.h"

#include "WorldModels/RobotWorldModel.h"


TemplatePhototaxisConfigurationLoader::TemplatePhototaxisConfigurationLoader()
{
}

TemplatePhototaxisConfigurationLoader::~TemplatePhototaxisConfigurationLoader()
{
	//nothing to do
}

WorldObserver* TemplatePhototaxisConfigurationLoader::make_WorldObserver(World* wm)
{
	return new TemplatePhototaxisWorldObserver(wm);
}

RobotWorldModel* TemplatePhototaxisConfigurationLoader::make_RobotWorldModel()
{
	return new RobotWorldModel();
}

AgentObserver* TemplatePhototaxisConfigurationLoader::make_AgentObserver(RobotWorldModel* wm)
{
	return new TemplatePhototaxisAgentObserver(wm);
}

Controller* TemplatePhototaxisConfigurationLoader::make_Controller(RobotWorldModel* wm)
{
	return new TemplatePhototaxisController(wm);
}

#endif
