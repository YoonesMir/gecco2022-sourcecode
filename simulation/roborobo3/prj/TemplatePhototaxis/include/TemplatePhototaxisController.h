




#ifndef TEMPLATEPHOTOTAXISCONTROLLER_H
#define TEMPLATEPHOTOTAXISCONTROLLER_H

#include "RoboroboMain/common.h"
#include "Controllers/Controller.h"
#include <neuralnetworks/NeuralNetwork.h>
#include "Utilities/Packet.h"
#include "Utilities/SlidingWindow.h"
#include "World/World.h"
#include "RoboroboMain/roborobo.h"
#include "WorldModels/RobotWorldModel.h"
#include "neuralnetworks/MLP.h"
#include "neuralnetworks/Perceptron.h"
#include "Utilities/Misc.h"
#include <sstream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>


#include "TemplatePhototaxis/include/TemplatePhototaxisSharedData.h"



#define NB_SENSORS 15
#include "Utilities/Sensorbelt.h"

using namespace Neural;


class RobotWorldModel;

class TemplatePhototaxisController : public Controller
{
    public:

        TemplatePhototaxisController(RobotWorldModel *wm);
        ~TemplatePhototaxisController();

        void step() override;

        double getFitness();
        double getFitnessSecond();
        
        int getBirthdate();
        int getIteration();
        int getAction();

       
        double getAveragedistanceFromLight();

        std::string getGenome();
        
        
        

    protected:
        //General
        double _fitnessValue;
        int _iteration;
        int _birthdate;
        void reset() override;

        // Controller
        int  _action;
        std::vector<double> _parameters;
        NeuralNetwork* _nn;
        double _minValue;
        double _maxValue;
        unsigned int _nbInputs;
        unsigned int _nbOutputs;
        unsigned int _nbHiddenLayers;
        std::vector<unsigned int>* _nbNeuronsPerHiddenLayer;
        std::vector<double> getInputs();
        void stepController() ;
        void setIOcontrollerSize();
        void defaultBehavior(int signal);
        void initController();
        void createNN();
        unsigned int computeRequiredNumberOfWeights();

        //Genom
        std::vector<double> _currentGenome;
        std::vector<double> _importedGenome; // cas de run PostMortem
        bool _isNewGenome;
        bool getNewGenomeStatus(); 
        void setNewGenomeStatus( bool __status );
        

        //Rewards 
        SlidingWindow* _rewards;
        SlidingWindow* _distancesFromLight;
        SlidingWindow* _rewardsSecond;
        void resetFitness() ;
        void updateFitness() ;

        // communication
        std::map< std::pair<int,int>, std::vector<double> > _genomesList;
        std::map< std::pair<int,int>, double > _fitnessValueList;       
        void broadcastGenome();
        bool sendGenome( TemplatePhototaxisController* __targetRobotController ) ;
        bool receiveGenome( Packet* p ) ;
        void clearReservoir();
        
        // selection
        int _packetLength_HIT; // automatically set in constructor. in [ 1 , _currentGenome.size()-1 ]
        void performSelection();
        void transfertGenomes(std::pair<int, int> selectedIndex);
        bool isAuthorizedSelection(std::pair<int, int> selectedIndex);  
        std::pair<int, int> selectRandomGenome();
        std::pair<int, int> selectBestGenome();
        std::pair<int, int> selectFitProp();


        // Evolution
        int _cptTimerout;
        int _maturationDelay;
        int _lastLifeChange;
        void initEvolution() ;
        void stepEvolution() ;
        void mapGenotypeToPhenotype();

        //variation
        std::map< std::pair<int,int>, double > _sigmaList;
        double _currentSigma;
        void performVariation();
        int mutateMixte(double sigma);
        double getBoundedGaussianValue(double value, double sigma, double minValue, double maxValue );
 
};


#endif
