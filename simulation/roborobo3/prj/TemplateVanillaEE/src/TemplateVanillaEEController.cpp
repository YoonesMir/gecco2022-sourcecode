/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "TemplateVanillaEE/include/TemplateVanillaEEController.h"
#include "TemplateVanillaEE/include/TemplateVanillaEESharedData.h"

#include "World/World.h"
#include "RoboroboMain/roborobo.h"
#include "WorldModels/RobotWorldModel.h"

using namespace Neural;

TemplateVanillaEEController::TemplateVanillaEEController( RobotWorldModel *wm ) : TemplateEEController( wm )
{
    rewards = new SlidingWindow(TemplateVanillaEESharedData::memorySlidingWindowSize);
    rewards->setAlwaysPositiveSum(true);
    distances = new SlidingWindow(TemplateVanillaEESharedData::memorySlidingWindowSize);
    distances->setAlwaysPositiveSum(true);
    xCoords = new SlidingWindow(TemplateVanillaEESharedData::coordSlidingWindowSize);
    yCoords = new SlidingWindow(TemplateVanillaEESharedData::coordSlidingWindowSize);
    // superclass constructor called before this baseclass constructor.
    resetFitness(); // superconstructor calls parent method.
}

TemplateVanillaEEController::~TemplateVanillaEEController()
{
    // superclass destructor automatically called after this baseclass destructor.
}

void TemplateVanillaEEController::stepController()
{
    TemplateEEController::stepController();
}

void TemplateVanillaEEController::initController()
{
    TemplateEEController::initController();
}

void TemplateVanillaEEController::initEvolution()
{
    TemplateEEController::initEvolution();
}

void TemplateVanillaEEController::stepEvolution()
{
    TemplateEEController::stepEvolution();
}

void TemplateVanillaEEController::performSelection()
{
    TemplateEEController::performSelection();
}

void TemplateVanillaEEController::performVariation()
{
    TemplateEEController::performVariation();
}

void TemplateVanillaEEController::broadcastGenome()
{
    TemplateEEController::broadcastGenome();
}

bool TemplateVanillaEEController::sendGenome( TemplateEEController* __targetRobotController )
{
    return TemplateEEController::sendGenome(__targetRobotController);
}

bool TemplateVanillaEEController::receiveGenome( Packet* p )
{
    return TemplateEEController::receiveGenome(p);
}

double TemplateVanillaEEController::getFitness()
{
    //return TemplateEEController::getFitness();
    return _wm->_fitnessValue;
}

void TemplateVanillaEEController::resetFitness()
{
    //TemplateEEController::resetFitness();
    _dSumTravelled = 0;
    rewards->reset(0);
    distances->reset(0);
    xCoords->reset(_wm->getXReal());
    yCoords->reset(_wm->getYReal());
}

void TemplateVanillaEEController::updateFitness()
{
    // nothing to do -- updating is performed in TemplateVanillaEEController::AgentObserver (automatic event when energy item are captured)
}

void TemplateVanillaEEController::updateFitness2( int value )
{
    // Fitness 1
    rewards->push( value );

    // fitness 2

    xCoords->push(_wm->getXReal());
    yCoords->push(_wm->getYReal());

    double dist = getEuclideanDistance(
                    xCoords->getPrev( 0 ), // current position (same as _wm->getXreal()
                    yCoords->getPrev( 0 ),
                    xCoords->getPrev( -TemplateVanillaEESharedData::coordSlidingWindowSize+1 ), // oldest recorded position
                    yCoords->getPrev( -TemplateVanillaEESharedData::coordSlidingWindowSize+1 )
                    );

    if ( dist < 0 ) // debug
    {
        std::cout << "[ERROR] dist<0\n";
        std::cout << "[DEBUG] dist:" << (double)dist << " from: (" << _wm->getXReal() << "," << _wm->getYReal() << ")<=>(" << xCoords->getPrev( 0 ) << "," << yCoords->getPrev( 0 ) << ") vs (" << xCoords->getPrev(-TemplateVanillaEESharedData::coordSlidingWindowSize+1) << "," << yCoords->getPrev(-TemplateVanillaEESharedData::coordSlidingWindowSize+1) << ")\n";
        exit (-2);
    }

    distances->push ( dist );

    //if ( gRobotDisplayFocus == true && gRobotIndexFocus == _wm->getId() && gDisplayMode != 2 && debug == true )
    //    std::cout << "[DEBUG] dist:" << (double)dist << " from: (" << _wm->getXReal() << "," << _wm->getYReal() << ")<=>(" << xCoords->getPrev( 0 ) << "," << yCoords->getPrev( 0 ) << ") vs (" << xCoords->getPrev(-coordSlidingWindowSize+1) << "," << yCoords->getPrev(-coordSlidingWindowSize+1) << ")\n";

    // update "official" fitness

    switch ( TemplateVanillaEESharedData::fitnessFunction )
    {
        case 0:
            _wm->_fitnessValue = rewards->getSum();
            break;
        case 1:
            _wm->_fitnessValue = distances->getSum(); //distances->getPrev(0);
            break;
        default:
            std::cout << "[ERROR] unknown fitness function ("<<TemplateVanillaEESharedData::fitnessFunction<<"). Exit.\n";

    }

}

int TemplateVanillaEEController::getIterations(){
    return gWorld->getIterations();
}
void TemplateVanillaEEController::logCurrentState()
{
    TemplateEEController::logCurrentState();
}
