
/**
 * @author Yoones Mirhosseini
 *
 */

#ifndef TEMPLATEPHOTOTAXISSHAREDDATA_H
#define TEMPLATEPHOTOTAXISSHAREDDATA_H


#include <vector>
#include "Utilities/LogManager.h"

class TemplatePhototaxisSharedData {
    
    // cf. super class for many parameter values.
    // Add here parameters that are specific to current implementation.
    
public:


    // Controller
    static int gControllerType; // controller type (0: MLP, 1: Perceptron)
    static int gNbHiddenLayers; // default: 1
    static int gNbNeuronsPerHiddenLayer; // default: 5
    static int gNeuronWeightRange; // default: 800.0 (ie. weights are in [-400,+400[

    

  
    
    static int gEvaluationTime;
    static int maturationDelayDefaultValue; // suggest: superior or equal to memorySlidingWindowSize
    static int memorySlidingWindowSize; // 100...400.. 800
    static double transferVolume; // in [0.0,1.0]. If 0.0: exactly *one* gene sent. If 1.0: transmit full genome. inbetween: transfer random genes.
    static int  reservoirSize; // Size of the reservoir
    static int  gSelectionMethod; // 0 random , 1 : best , 2 FitProp
    static int timeout; // timeout in case the robots dont have a full  reservoir, after this time the robot go for selection 
    static bool strictlySuperiorSelection; // accept if other.fitness > my.fitness or if other.fitness >= my.fitness? (default is false, i.e. ">=")
    static bool transferFilter;
    



    static double TranslationSpeed; 
    static double RotationalSpeed;
    static double DiffusionSpeed;
    static int frequencyLightChange; // changing light form left side the arena to right side and reverse
    static int  lightmode; //   0 right left ligtt , 1 center light 


    static double maxDistanceFromLight;
    static int lightPlaceIndice;
    static std::vector<std::pair<double,double>>  lightplaces;


    // Variation
    static double gProbaMutation;// ~ __1__ robot over __150__ mutates once every __400*10__ iterations for *each* robot.
    static double gMutationVolume; // in [0.0,1.0]. If 0.0: exactly *one* gene muted. If 1.0: mute full genome. inbetween: mute random genes.
    static double gProbaMutationUniform; 
    // Normal mutation
    static double gSigmaRef; //! reference value of sigma
  
    
    // Log informations  by text or snapshots

    static bool gSnapshots; // take snapshots
    static int gSnapshotsFrequency; // every N generations

 

    static bool gLiteLogInformation;
    static std::ofstream gLitelogFile;
    static LogManager *gLitelogManager;
    

    static bool gLifeSpanLogInformation;
    static std::ofstream gLifeSpanlogFile;
    static LogManager *gLifeSpanlogManager;

    static bool gDistanceLightLogInformation;
    static std::ofstream gDistanceLightlogFile;
    static LogManager *gDistanceLightlogManager;

    static bool gCommunicationLogInformation;
    static int communicationTypeRegistration; // 0 : register olny 
    static std::ofstream gComlogFile;
    static LogManager *gComlogManager;
    
    static bool gFatLogInformation;
    static std::ofstream gFatlogFile;
    static LogManager *gFatlogManager;

    static bool gCoordinationLogInformation;
    static std::ofstream gcoordinationlogFile;
    static LogManager *gcoordinationlogManager;
    static int gCoordinationSnapshotsFrequency;
    static int gCoordinationLogTime;

    static bool gMutateLogInformation;
    static std::ofstream gMutatelogFile;
    static LogManager *gMutatelogManager;

    static bool ggenomeLogInformation;
    static std::ofstream gGenomelogFile;
    static LogManager *gGenomelogManager;
    static std::string  genomFile;


}; 


#endif
