/**
 * @author Yoones Mirhosseini 12-08-2020
 *
 */

#ifndef TEMPLATEPHOTOTAXISAGENTOBSERVER_H
#define TEMPLATEPHOTOTAXISAGENTOBSERVER_H





#include "WorldModels/RobotWorldModel.h"
#include "World/PhysicalObject.h"
#include "RoboroboMain/roborobo.h"
#include "Observers/AgentObserver.h"
#include "TemplatePhototaxis/include/TemplatePhototaxisController.h"
#include "TemplatePhototaxis/include/TemplatePhototaxisSharedData.h"


class RobotWorldModel;

class TemplatePhototaxisAgentObserver : public AgentObserver
{
	public:
		TemplatePhototaxisAgentObserver(RobotWorldModel *wm);
		~TemplatePhototaxisAgentObserver();
		
		void reset() override;
        void stepPre() override;
		void stepPost() override;	
};

#endif

