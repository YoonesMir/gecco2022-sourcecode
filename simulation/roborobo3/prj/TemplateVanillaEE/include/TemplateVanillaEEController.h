/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#ifndef TEMPLATEVANILLAEECONTROLLER_H
#define TEMPLATEVANILLAEECONTROLLER_H

#include "TemplateEE/include/TemplateEEController.h"
#include "Utilities/Packet.h"
#include "Utilities/SlidingWindow.h"

using namespace Neural;

class RobotWorldModel;

class TemplateVanillaEEController : public TemplateEEController
{
    public:

        TemplateVanillaEEController(RobotWorldModel *wm);
        ~TemplateVanillaEEController();

        double getFitness() override;
        int getIterations();
        void updateFitness2(int value); // project-specific


    protected:
        SlidingWindow* rewards;
        SlidingWindow* xCoords;
        SlidingWindow* yCoords;
        SlidingWindow* distances;

        void initController() override;
        void initEvolution() override;

        void stepController() override;
        void stepEvolution() override;

        void performSelection() override;
        void performVariation() override;

        void broadcastGenome() override;

        bool sendGenome( TemplateEEController* __targetRobotController ) override;
        bool receiveGenome( Packet* p ) override;

        void resetFitness() override;
        void updateFitness() override;

        void logCurrentState() override;
};


#endif
