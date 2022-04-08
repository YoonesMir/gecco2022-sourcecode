#include "TemplatePhototaxis/include/TemplatePhototaxisSharedData.h"


int TemplatePhototaxisSharedData::gControllerType = 1; // controller type (0: MLP, 1: Perceptron)
int TemplatePhototaxisSharedData::gNbHiddenLayers = 1; // default: 1
int TemplatePhototaxisSharedData::gNbNeuronsPerHiddenLayer = 2; // default: 5
int TemplatePhototaxisSharedData::gNeuronWeightRange = 800; // default: 800.0 (ie. weights are in [-400,+400[



    // Buffer HIT 

int TemplatePhototaxisSharedData::gEvaluationTime = 400;
int TemplatePhototaxisSharedData::maturationDelayDefaultValue = 400; // suggest: superior or equal to memorySlidingWindowSize
int TemplatePhototaxisSharedData::memorySlidingWindowSize= 400; // 100...400.. 800
double TemplatePhototaxisSharedData::transferVolume = 0.5; // in [0.0,1.0]. If 0.0: exactly *one* gene sent. If 1.0: transmit full genome. inbetween: transfer random genes.
int  TemplatePhototaxisSharedData::reservoirSize = 3; // Size of the reservoir
int  TemplatePhototaxisSharedData::gSelectionMethod = 1; // 0 random , 1 : best , 2 FitProp
int TemplatePhototaxisSharedData::timeout = 1000000; // timeout in case the robots dont have a full  reservoir, after this time the robot go for selection 
bool TemplatePhototaxisSharedData::strictlySuperiorSelection = false; // accept if other.fitness > my.fitness or if other.fitness >= my.fitness? (default is false, i.e. ">=")
bool TemplatePhototaxisSharedData::transferFilter = false;




double TemplatePhototaxisSharedData::TranslationSpeed = 1.0; 
double TemplatePhototaxisSharedData::RotationalSpeed = 1.0;
double TemplatePhototaxisSharedData::DiffusionSpeed = 0.1;
int TemplatePhototaxisSharedData::frequencyLightChange = 100000; // changing light form left side the arena to right side and reverse
int TemplatePhototaxisSharedData::lightmode = 0;// 0 right lefi light , 1 center light


// light places

double TemplatePhototaxisSharedData::maxDistanceFromLight = 0.0;
int TemplatePhototaxisSharedData::lightPlaceIndice = 0;
std::vector<std::pair<double,double>>  TemplatePhototaxisSharedData::lightplaces;


// Variation
double TemplatePhototaxisSharedData::gProbaMutation = 0.01;// ~ __1__ robot over __150__ mutates once every __400*10__ iterations for *each* robot.
double TemplatePhototaxisSharedData::gMutationVolume = 0.5; // in [0.0,1.0]. If 0.0: exactly *one* gene muted. If 1.0: mute full genome. inbetween: mute random genes.
double TemplatePhototaxisSharedData::gProbaMutationUniform = 1.0; 
// Normal mutation
double TemplatePhototaxisSharedData::gSigmaRef = 0.0; //! reference value of sigma


// Log informations  by text or snapshots

bool TemplatePhototaxisSharedData::gSnapshots = false; // take snapshots
int TemplatePhototaxisSharedData::gSnapshotsFrequency = 50; // every N generations



bool TemplatePhototaxisSharedData::gLiteLogInformation = false;
std::ofstream TemplatePhototaxisSharedData::gLitelogFile;
LogManager *TemplatePhototaxisSharedData::gLitelogManager = NULL;


bool TemplatePhototaxisSharedData::gLifeSpanLogInformation = false;
std::ofstream TemplatePhototaxisSharedData::gLifeSpanlogFile;
LogManager *TemplatePhototaxisSharedData::gLifeSpanlogManager = NULL;

bool TemplatePhototaxisSharedData::gDistanceLightLogInformation = false;
std::ofstream TemplatePhototaxisSharedData::gDistanceLightlogFile;
LogManager *TemplatePhototaxisSharedData::gDistanceLightlogManager = NULL;

bool TemplatePhototaxisSharedData::gCommunicationLogInformation = false;
int TemplatePhototaxisSharedData::communicationTypeRegistration = 0; // 0 : register olny 
std::ofstream TemplatePhototaxisSharedData::gComlogFile;
LogManager *TemplatePhototaxisSharedData::gComlogManager = NULL;

bool TemplatePhototaxisSharedData::gFatLogInformation = false;
std::ofstream TemplatePhototaxisSharedData::gFatlogFile;
LogManager *TemplatePhototaxisSharedData::gFatlogManager = NULL;

bool TemplatePhototaxisSharedData::gCoordinationLogInformation = false;
std::ofstream TemplatePhototaxisSharedData::gcoordinationlogFile;
LogManager *TemplatePhototaxisSharedData::gcoordinationlogManager = NULL;
int TemplatePhototaxisSharedData::gCoordinationSnapshotsFrequency = 50;
int  TemplatePhototaxisSharedData::gCoordinationLogTime = 1000;


bool TemplatePhototaxisSharedData::gMutateLogInformation = false;
std::ofstream TemplatePhototaxisSharedData::gMutatelogFile;
LogManager *TemplatePhototaxisSharedData::gMutatelogManager = NULL;


bool TemplatePhototaxisSharedData::ggenomeLogInformation = false;
std::ofstream TemplatePhototaxisSharedData::gGenomelogFile;
LogManager *TemplatePhototaxisSharedData::gGenomelogManager = NULL;
std::string TemplatePhototaxisSharedData::genomFile = "LEARNING";

