/*
 * TemplateEEConfigurationLoader.h
 */

#ifndef TEMPLATEPHOTOTAXISCONFIGURATIONLOADER_H
#define TEMPLATEPHOTOTAXISCONFIGURATIONLOADER_H


#include "Config/ConfigurationLoader.h"

class TemplatePhototaxisConfigurationLoader : public ConfigurationLoader
{
	private:

	public:
		TemplatePhototaxisConfigurationLoader();
		~TemplatePhototaxisConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotWorldModel *make_RobotWorldModel();
		AgentObserver *make_AgentObserver(RobotWorldModel* wm) ;
		Controller *make_Controller(RobotWorldModel* wm) ;
};



#endif
