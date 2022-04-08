



#include "TemplatePhototaxis/include/TemplatePhototaxisWorldObserver.h"




TemplatePhototaxisWorldObserver::TemplatePhototaxisWorldObserver( World* world ) :  WorldObserver( world )
{
  
    _world = world;

    // ==== loading project-specific properties

    gProperties.checkAndGetPropertyValue("gControllerType",&TemplatePhototaxisSharedData::gControllerType,true);
    gProperties.checkAndGetPropertyValue("gNbHiddenLayers",&TemplatePhototaxisSharedData::gNbHiddenLayers,true);
    gProperties.checkAndGetPropertyValue("gNbNeuronsPerHiddenLayer",&TemplatePhototaxisSharedData::gNbNeuronsPerHiddenLayer,true);
    gProperties.checkAndGetPropertyValue("gNeuronWeightRange",&TemplatePhototaxisSharedData::gNeuronWeightRange,true);

    gProperties.checkAndGetPropertyValue("gEvaluationTime",&TemplatePhototaxisSharedData::gEvaluationTime,true);
    gProperties.checkAndGetPropertyValue("maturationDelayDefaultValue",&TemplatePhototaxisSharedData::maturationDelayDefaultValue,true);
    gProperties.checkAndGetPropertyValue("memorySlidingWindowSize",&TemplatePhototaxisSharedData::memorySlidingWindowSize,true);
    gProperties.checkAndGetPropertyValue("transferVolume",&TemplatePhototaxisSharedData::transferVolume,true);
    gProperties.checkAndGetPropertyValue("reservoirSize",&TemplatePhototaxisSharedData::reservoirSize,true);
    gProperties.checkAndGetPropertyValue("gSelectionMethod",&TemplatePhototaxisSharedData::gSelectionMethod,true);
    gProperties.checkAndGetPropertyValue("timeout",&TemplatePhototaxisSharedData::timeout,true);
    gProperties.checkAndGetPropertyValue("strictlySuperiorSelection",&TemplatePhototaxisSharedData::strictlySuperiorSelection,true);
    gProperties.checkAndGetPropertyValue("transferFilter",&TemplatePhototaxisSharedData::transferFilter,true);
    

    gProperties.checkAndGetPropertyValue("TranslationSpeed",&TemplatePhototaxisSharedData::TranslationSpeed,true);
    gProperties.checkAndGetPropertyValue("RotationalSpeed",&TemplatePhototaxisSharedData::RotationalSpeed,true);
    gProperties.checkAndGetPropertyValue("DiffusionSpeed",&TemplatePhototaxisSharedData::DiffusionSpeed,true);
    gProperties.checkAndGetPropertyValue("frequencyLightChange",&TemplatePhototaxisSharedData::frequencyLightChange,true);
    gProperties.checkAndGetPropertyValue("lightmode",&TemplatePhototaxisSharedData::lightmode,true);


    
    

    gProperties.checkAndGetPropertyValue("gProbaMutation",&TemplatePhototaxisSharedData::gProbaMutation,true);
    gProperties.checkAndGetPropertyValue("gMutationVolume",&TemplatePhototaxisSharedData::gMutationVolume,true);
    gProperties.checkAndGetPropertyValue("gProbaMutationUniform",&TemplatePhototaxisSharedData::gProbaMutationUniform,true);
    gProperties.checkAndGetPropertyValue("gSigmaRef",&TemplatePhototaxisSharedData::gSigmaRef,true);


    gProperties.checkAndGetPropertyValue("gSnapshots",&TemplatePhototaxisSharedData::gSnapshots,true);
    gProperties.checkAndGetPropertyValue("gSnapshotsFrequency",&TemplatePhototaxisSharedData::gSnapshotsFrequency,true);
    gProperties.checkAndGetPropertyValue("gLiteLogInformation",&TemplatePhototaxisSharedData::gLiteLogInformation,true);
    gProperties.checkAndGetPropertyValue("gLifeSpanLogInformation",&TemplatePhototaxisSharedData::gLifeSpanLogInformation,true);
    gProperties.checkAndGetPropertyValue("gDistanceLightLogInformation",&TemplatePhototaxisSharedData::gDistanceLightLogInformation,true);
    gProperties.checkAndGetPropertyValue("gCommunicationLogInformation",&TemplatePhototaxisSharedData::gCommunicationLogInformation,true);
    gProperties.checkAndGetPropertyValue("communicationTypeRegistration",&TemplatePhototaxisSharedData::communicationTypeRegistration,true);
    gProperties.checkAndGetPropertyValue("gFatLogInformation",&TemplatePhototaxisSharedData::gFatLogInformation,true);
    gProperties.checkAndGetPropertyValue("gCoordinationLogInformation",&TemplatePhototaxisSharedData::gCoordinationLogInformation,true);
    gProperties.checkAndGetPropertyValue("gMutateLogInformation",&TemplatePhototaxisSharedData::gMutateLogInformation,true);
    gProperties.checkAndGetPropertyValue("gCoordinationSnapshotsFrequency",&TemplatePhototaxisSharedData::gCoordinationSnapshotsFrequency,true);
    gProperties.checkAndGetPropertyValue("gCoordinationLogTime",&TemplatePhototaxisSharedData::gCoordinationLogTime,true);
    gProperties.checkAndGetPropertyValue("ggenomeLogInformation",&TemplatePhototaxisSharedData::ggenomeLogInformation,true);
    gProperties.checkAndGetPropertyValue("genomFile",&TemplatePhototaxisSharedData::genomFile,true);
    
    // ==== create specific "lite" logger file
    
    if(TemplatePhototaxisSharedData::gLiteLogInformation){
        std::string litelogFullFilename = gLogDirectoryname + "/lite_" + gLogFilename;
        TemplatePhototaxisSharedData::gLitelogFile.open(litelogFullFilename.c_str());
        if(!TemplatePhototaxisSharedData::gLitelogFile) {
            std::cerr << "[CRITICAL] Cannot open \"lite\" log file " << litelogFullFilename << "." << std::endl << std::endl;
            exit(-1);
        }
        TemplatePhototaxisSharedData::gLitelogManager = new LogManager();
        TemplatePhototaxisSharedData::gLitelogManager->setLogFile(TemplatePhototaxisSharedData::gLitelogFile);
        TemplatePhototaxisSharedData::gLitelogManager->write("generation,iteration,minFitness,maxFitness,avgFitnessNormalized,first_quartile,median,third_quartile\n");
        TemplatePhototaxisSharedData::gLitelogManager->flush(); 
    }
    /***  DistanceFromLight  Log initialization ***/
    if(TemplatePhototaxisSharedData::gDistanceLightLogInformation){
        std::string DistanceLightFullFilename = gLogDirectoryname + "/distanceLight_" + gLogFilename;
        TemplatePhototaxisSharedData::gDistanceLightlogFile.open(DistanceLightFullFilename.c_str());
        if(!TemplatePhototaxisSharedData::gDistanceLightlogFile) {
            std::cerr << "[CRITICAL] Cannot open \"distanceLight_\" log file " << DistanceLightFullFilename << "." << std::endl << std::endl;
            exit(-1);
        }
        TemplatePhototaxisSharedData::gDistanceLightlogManager= new LogManager();
        TemplatePhototaxisSharedData::gDistanceLightlogManager->setLogFile(TemplatePhototaxisSharedData::gDistanceLightlogFile);
        TemplatePhototaxisSharedData::gDistanceLightlogManager->write("generation,iteration,minDistanceLight,maxDistanceLight,avgdistanceLighNormalized,first_quartile_DistanceLight,median_DistanceLight,third_quartile_DistanceLight\n");
        TemplatePhototaxisSharedData::gDistanceLightlogManager->flush(); 
    }
    
    /*** Communication Log initialization ***/
    if(TemplatePhototaxisSharedData::gCommunicationLogInformation){
        std::string comlogFullFilename = gLogDirectoryname + "/com_" + gLogFilename;
        TemplatePhototaxisSharedData::gComlogFile.open(comlogFullFilename.c_str());
        if(!TemplatePhototaxisSharedData::gComlogFile) {
            std::cerr << "[CRITICAL] Cannot open \"com\" log file " << comlogFullFilename << "." << std::endl << std::endl;
            exit(-1);
        }
        TemplatePhototaxisSharedData::gComlogManager = new LogManager();
        TemplatePhototaxisSharedData::gComlogManager->setLogFile(TemplatePhototaxisSharedData::gComlogFile);
        TemplatePhototaxisSharedData::gComlogManager->write("generation,iteration,source,target\n");
        TemplatePhototaxisSharedData::gComlogManager->flush(); 
    }

     /*** Full coordination Log initialization ***/
    if(TemplatePhototaxisSharedData::gCoordinationLogInformation){
        std::string coordinationlogFullFilename = gLogDirectoryname + "/coordination_" + gLogFilename;
        TemplatePhototaxisSharedData::gcoordinationlogFile.open(coordinationlogFullFilename.c_str());
        if(!TemplatePhototaxisSharedData::gcoordinationlogFile) {
            std::cerr << "[CRITICAL] Cannot open \"coordination\" log file " << coordinationlogFullFilename << "." << std::endl << std::endl;
            exit(-1);
        }
        TemplatePhototaxisSharedData::gcoordinationlogManager= new LogManager();
        TemplatePhototaxisSharedData::gcoordinationlogManager->setLogFile(TemplatePhototaxisSharedData::gcoordinationlogFile);
        TemplatePhototaxisSharedData::gcoordinationlogManager->write("particle,generation,frame,nb_log,x,y,action, orientation\n");
        TemplatePhototaxisSharedData::gcoordinationlogManager->flush(); 
    }
    
    /*** Full fitness Log initialization ***/
    if(TemplatePhototaxisSharedData::gFatLogInformation){
        std::string fatlogFullFilename = gLogDirectoryname + "/fat_" + gLogFilename;
        TemplatePhototaxisSharedData::gFatlogFile.open(fatlogFullFilename.c_str());
        if(!TemplatePhototaxisSharedData::gFatlogFile) {
            std::cerr << "[CRITICAL] Cannot open \"fat\" log file " << fatlogFullFilename << "." << std::endl << std::endl;
            exit(-1);
        }
    }
    
     /***  lifespan cycle Log initialization ***/
     if(TemplatePhototaxisSharedData::gLifeSpanLogInformation){
        std::string lifeSpanlogFullFilename = gLogDirectoryname + "/lifeSpan_" + gLogFilename;
        TemplatePhototaxisSharedData::gLifeSpanlogFile.open(lifeSpanlogFullFilename.c_str());
        if(!TemplatePhototaxisSharedData::gLifeSpanlogFile) {
            std::cerr << "[CRITICAL] Cannot open \"lifeSapn\" log file " << lifeSpanlogFullFilename << "." << std::endl << std::endl;
            exit(-1);
        }
        TemplatePhototaxisSharedData::gLifeSpanlogManager= new LogManager();
        TemplatePhototaxisSharedData::gLifeSpanlogManager->setLogFile(TemplatePhototaxisSharedData::gLifeSpanlogFile);
        TemplatePhototaxisSharedData::gLifeSpanlogManager->write("generation,iteration,agent_id,lifespan,birthdate\n");
        TemplatePhototaxisSharedData::gLifeSpanlogManager->flush();
     }

     if(TemplatePhototaxisSharedData::gMutateLogInformation){
        std::string mutatelogFullFilename = gLogDirectoryname + "/mutate_" + gLogFilename;
        TemplatePhototaxisSharedData::gMutatelogFile.open(mutatelogFullFilename.c_str());
        if(!TemplatePhototaxisSharedData::gMutatelogFile) {
            std::cerr << "[CRITICAL] Cannot open \"mutate\" log file " << mutatelogFullFilename << "." << std::endl << std::endl;
            exit(-1);
        }
        TemplatePhototaxisSharedData::gMutatelogManager = new LogManager();
        TemplatePhototaxisSharedData::gMutatelogManager->setLogFile(TemplatePhototaxisSharedData::gMutatelogFile);
        TemplatePhototaxisSharedData::gMutatelogManager->write("generation,iteration,agent,nb_Gen_Total,nb_Gen_muate\n");
        TemplatePhototaxisSharedData::gMutatelogManager->flush(); 
    }
    if (TemplatePhototaxisSharedData::ggenomeLogInformation){
        std::string geneomelogFullFilename = gLogDirectoryname + "/genome_" + gLogFilename;
        TemplatePhototaxisSharedData::gGenomelogFile.open(geneomelogFullFilename.c_str());
        if(!TemplatePhototaxisSharedData::gGenomelogFile) {
            std::cerr << "[CRITICAL] Cannot open \"genome\" log file " << geneomelogFullFilename << "." << std::endl << std::endl;
            exit(-1);
        }
    }
    
    


    // * light places confugurations 
    // Square  arene 1000 * 1000
    // light mode : 0 right left , 1 center light 

    TemplatePhototaxisSharedData::lightPlaceIndice = 0;
    if (TemplatePhototaxisSharedData::lightmode == 0){
            TemplatePhototaxisSharedData::lightplaces.push_back(std::make_pair(992, 500));
            TemplatePhototaxisSharedData::lightplaces.push_back(std::make_pair(8, 500));
            TemplatePhototaxisSharedData::maxDistanceFromLight = getEuclideanDistance(992.0 , 500.0,0.0, 0.0);
    }
    else{
        if (TemplatePhototaxisSharedData::lightmode == 1){
            TemplatePhototaxisSharedData::lightplaces.push_back(std::make_pair(500.0 , 500.0));
            TemplatePhototaxisSharedData::maxDistanceFromLight = getEuclideanDistance(500.0 , 500.0,0.0, 0.0);

        }
        else{
            std::cout<<"lightmode not correct : "<<TemplatePhototaxisSharedData::lightmode<<"\n";
            exit(0);
        }
    }
    
}

TemplatePhototaxisWorldObserver::~TemplatePhototaxisWorldObserver()
{
    if(TemplatePhototaxisSharedData::gMutateLogInformation){
        TemplatePhototaxisSharedData::gMutatelogFile.close();
    }
    if(TemplatePhototaxisSharedData::gDistanceLightLogInformation){
        TemplatePhototaxisSharedData::gDistanceLightlogFile.close();
    }
    
    if(TemplatePhototaxisSharedData::gLiteLogInformation){
        TemplatePhototaxisSharedData::gLitelogFile.close();
    }
    if(TemplatePhototaxisSharedData::gCommunicationLogInformation){
        TemplatePhototaxisSharedData::gComlogFile.close();
    }
    
    if(TemplatePhototaxisSharedData::gLifeSpanLogInformation){
        TemplatePhototaxisSharedData::gLifeSpanlogFile.close();
    }
    
    if(TemplatePhototaxisSharedData::gFatLogInformation){
        TemplatePhototaxisSharedData::gFatlogFile.close();
    }
    if(TemplatePhototaxisSharedData::gCoordinationLogInformation){
        TemplatePhototaxisSharedData::gcoordinationlogFile.close();
    }
    if(TemplatePhototaxisSharedData::ggenomeLogInformation){
        TemplatePhototaxisSharedData::gGenomelogFile.close();
    }
}

void TemplatePhototaxisWorldObserver::initPre()
{
    // nothing to do.
}

void TemplatePhototaxisWorldObserver::initPost()
{
    
    if(TemplatePhototaxisSharedData::gFatLogInformation){
        TemplatePhototaxisSharedData::gFatlogManager = new LogManager();
        TemplatePhototaxisSharedData::gFatlogManager->setLogFile(TemplatePhototaxisSharedData::gFatlogFile);
        std::string labels = "generation,iteration";
        for (int i = 0 ; i != gNbOfRobots ; i++){
            labels += ",agent_" + std::to_string(i);
        }
        labels += "\n";
        TemplatePhototaxisSharedData::gFatlogManager->write(labels);
        TemplatePhototaxisSharedData::gFatlogManager->flush();
    }

    if(TemplatePhototaxisSharedData::ggenomeLogInformation){
        TemplatePhototaxisSharedData::gGenomelogManager = new LogManager();
        TemplatePhototaxisSharedData::gGenomelogManager->setLogFile(TemplatePhototaxisSharedData::gGenomelogFile);
        std::string labels = "generation,iteration";
        for (int i = 0 ; i != gNbOfRobots ; i++){
            labels += ",agent_" + std::to_string(i);
        }
        labels += "\n";
        TemplatePhototaxisSharedData::gGenomelogManager->write(labels);
        TemplatePhototaxisSharedData::gGenomelogManager->flush();
    }

}

void TemplatePhototaxisWorldObserver::stepPre()
{

    updateMonitoring();

    updateEnvironment();

}

void TemplatePhototaxisWorldObserver::stepPost( )
{

    // nothing to do
}



void TemplatePhototaxisWorldObserver::updateMonitoring()
{
   

    
    // log at end of generation.
    if( gWorld->getIterations() % TemplatePhototaxisSharedData::gEvaluationTime == TemplatePhototaxisSharedData::gEvaluationTime-1 )
    {
        monitorPopulation();
    }


    // * Every N generations, take a video (duration: one generation time)
    if ( TemplatePhototaxisSharedData::gSnapshots )
    {
        if ( ( gWorld->getIterations() ) % ( TemplatePhototaxisSharedData::gEvaluationTime * TemplatePhototaxisSharedData::gSnapshotsFrequency ) == 0 )
        {
            if ( gVerbose ){
                std::cout << "[START] Video recording: generation #" << (gWorld->getIterations() / TemplatePhototaxisSharedData::gEvaluationTime ) << " Iterations: "<<gWorld->getIterations()<<".\n";
            }
            gTrajectoryMonitorMode = 0;
            initTrajectoriesMonitor();
        }
        else{
            if ( ( gWorld->getIterations() ) % ( TemplatePhototaxisSharedData::gEvaluationTime * TemplatePhototaxisSharedData::gSnapshotsFrequency ) == TemplatePhototaxisSharedData::gEvaluationTime - 1 )
            {
                if ( gVerbose ){
                    std::cout << "[STOP]  Video recording: generation #" << (gWorld->getIterations() / TemplatePhototaxisSharedData::gEvaluationTime ) <<  " Iterations: "<<gWorld->getIterations()<<".\n";
                }
                saveTrajectoryImage();
            }
        }  
    }
    // Every M generations, log Coordination information
    if(TemplatePhototaxisSharedData::gCoordinationLogInformation){
        if ( (( gWorld->getIterations() ) % ( TemplatePhototaxisSharedData::gCoordinationLogTime * TemplatePhototaxisSharedData::gCoordinationSnapshotsFrequency ) >= 0) and (( gWorld->getIterations() ) % ( TemplatePhototaxisSharedData::gCoordinationLogTime * TemplatePhototaxisSharedData::gCoordinationSnapshotsFrequency ) <= TemplatePhototaxisSharedData::gCoordinationLogTime - 1) )
        {
            std::string  coordination = "";
            for ( int i = 0 ; i != gNbOfRobots ; i++ )
            {
                TemplatePhototaxisController *ctl = dynamic_cast<TemplatePhototaxisController*>(gWorld->getRobot(i)->getController());
                if(ctl == NULL){
                    std::cerr << "TemplatePhototaxisController *ctl in TemplatePhototaxisWorldObserver is NULL\n";
                    exit(-1);
                }
                Point2d agentCoordination = ctl->getPosition();
                coordination +=  std::to_string(i)+","+std::to_string(gWorld->getIterations()/ TemplatePhototaxisSharedData::gEvaluationTime)+","+std::to_string(gWorld->getIterations())+","+std::to_string(gWorld->getIterations()/ TemplatePhototaxisSharedData::gCoordinationLogTime)+","+std::to_string(agentCoordination.x)+ ","+std::to_string(agentCoordination.y)+","+std::to_string(ctl->getAction())+","+std::to_string(ctl->getOrientation())+"\n";
            }
            TemplatePhototaxisSharedData::gcoordinationlogManager->write(coordination);
            TemplatePhototaxisSharedData::gcoordinationlogManager->flush();
        }
    }
    //add coordiantaion juste before changing light
    if(TemplatePhototaxisSharedData::gCoordinationLogInformation and TemplatePhototaxisSharedData::lightmode != 1 ){
        if ( (gWorld->getIterations() % TemplatePhototaxisSharedData::frequencyLightChange <=  (TemplatePhototaxisSharedData::frequencyLightChange -1)) and   (gWorld->getIterations() % TemplatePhototaxisSharedData::frequencyLightChange >  (TemplatePhototaxisSharedData::frequencyLightChange  -TemplatePhototaxisSharedData::gCoordinationLogTime -1 )))
        {
            std::string  coordination = "";
            for ( int i = 0 ; i != gNbOfRobots ; i++ )
            {
                TemplatePhototaxisController *ctl = dynamic_cast<TemplatePhototaxisController*>(gWorld->getRobot(i)->getController());
                if(ctl == NULL){
                    std::cerr << "TemplatePhototaxisController *ctl in TemplatePhototaxisWorldObserver is NULL\n";
                    exit(-1);
                }
                Point2d agentCoordination = ctl->getPosition();
                coordination +=  std::to_string(i)+","+std::to_string(gWorld->getIterations()/ TemplatePhototaxisSharedData::gEvaluationTime)+","+std::to_string(gWorld->getIterations())+","+std::to_string(gWorld->getIterations()/ TemplatePhototaxisSharedData::gCoordinationLogTime)+","+std::to_string(agentCoordination.x)+ ","+std::to_string(agentCoordination.y)+","+std::to_string(ctl->getAction())+","+std::to_string(ctl->getOrientation())+"\n";
            }
            TemplatePhototaxisSharedData::gcoordinationlogManager->write(coordination);
            TemplatePhototaxisSharedData::gcoordinationlogManager->flush();
        }
    }
}

void TemplatePhototaxisWorldObserver::monitorPopulation( bool localVerbose )

{

    
    if(TemplatePhototaxisSharedData::gFatLogInformation || TemplatePhototaxisSharedData::gLiteLogInformation || TemplatePhototaxisSharedData::gDistanceLightLogInformation || TemplatePhototaxisSharedData::ggenomeLogInformation ){

    
        
        int activeCount = 0;

        double fitnessAgent = 0.0;
        double *fitness_tab = new double[gNbOfRobots];
        double sumOfFitnesses = 0;
        double minFitness = DBL_MAX;
        double maxFitness = -DBL_MAX;
        double first_quartile = 0;
        double median = 0;
        double third_quartile = 0;  
       
        double distanceLighAgent = 0.0;
        double *distanceLight_tab = new double[gNbOfRobots];
        double sumOfDistanceLight = 0.0;
        double minDistanceLight = DBL_MAX;
        double maxDistanceLight = -DBL_MAX;
        double first_quartile_DistanceLight = 0.0;
        double median_DistanceLight = 0.0;
        double third_quartile_DistanceLight = 0.0;

        
        std::string fatValues = "";
        if(TemplatePhototaxisSharedData::gFatLogInformation){
            fatValues += std::to_string(gWorld->getIterations()/TemplatePhototaxisSharedData::gEvaluationTime)+","+std::to_string(gWorld->getIterations());
        }

        std::string gensValues = "";
        if(TemplatePhototaxisSharedData::ggenomeLogInformation  ){
                gensValues = std::to_string(gWorld->getIterations()/TemplatePhototaxisSharedData::gEvaluationTime )+","+std::to_string(gWorld->getIterations());
         }

        for ( int i = 0 ; i != gNbOfRobots ; i++ )
        {
            activeCount++;
            TemplatePhototaxisController *ctl = dynamic_cast<TemplatePhototaxisController*>(gWorld->getRobot(i)->getController());
            if(ctl == NULL){
                std::cerr << "[ERROR] TemplatePhototaxisController *ctl in PhototaxieWorldObserver is NULL\n";
                exit(-1);
            }
            if(TemplatePhototaxisSharedData::ggenomeLogInformation  ){
                std::string gene =   ctl->getGenome();
                gensValues += "," + gene;
            }
                
            fitnessAgent = ctl->getFitnessSecond();
            if (fitnessAgent < 0  || fitnessAgent > TemplatePhototaxisSharedData::memorySlidingWindowSize   )
            {
                    std::cerr << "[ERROR] monitorPopulation error with ctl->getFitness(id="<< ctl->getId() << " = " << fitnessAgent << ")\n";
                    exit (-2);
            }
            distanceLighAgent = ctl->getAveragedistanceFromLight();
            if (distanceLighAgent <= 0  || distanceLighAgent >= 1 )
            {
                    std::cerr << "[ERROR] monitorPopulation error with ctl->getAveragedistanceFromLight(id="<< ctl->getId() << " = " << distanceLighAgent << ")\n";
                    exit (-2);
            } 
            if(TemplatePhototaxisSharedData::gFatLogInformation){
                fatValues += "," + std::to_string(fitnessAgent);
            }
            
            
            if(TemplatePhototaxisSharedData::gLiteLogInformation ){
                fitness_tab[i] = fitnessAgent;
                sumOfFitnesses += fitnessAgent;
                if ( fitnessAgent > maxFitness )
                {
                    maxFitness = fitnessAgent;
                }
                if ( fitnessAgent < minFitness )
                {
                    minFitness =fitnessAgent;
                }
            }
            if(TemplatePhototaxisSharedData::gDistanceLightLogInformation){
                distanceLight_tab[i] = distanceLighAgent;
                sumOfDistanceLight += distanceLighAgent;
                if ( distanceLighAgent > maxDistanceLight )
                    maxDistanceLight = distanceLighAgent;

                if ( distanceLighAgent < minDistanceLight )
                {
                    minDistanceLight =distanceLighAgent;
                }
            }
            
        }
        if(TemplatePhototaxisSharedData::ggenomeLogInformation  ){
            gensValues += "\n";
            TemplatePhototaxisSharedData::gGenomelogManager->write(gensValues);
            TemplatePhototaxisSharedData::gGenomelogManager->flush();
        }

        if(TemplatePhototaxisSharedData::gFatLogInformation){
            fatValues += "\n";
            TemplatePhototaxisSharedData::gFatlogManager->write(fatValues);
            TemplatePhototaxisSharedData::gFatlogManager->flush();

        }

        if(TemplatePhototaxisSharedData::gLiteLogInformation){
     
            double avgFitnessNormalized = 0.0;
            std::sort(fitness_tab, fitness_tab+gNbOfRobots);
            first_quartile = fitness_tab[(int)round(gNbOfRobots*0.25)];
            median = fitness_tab[(int)round(gNbOfRobots*0.5)];
            third_quartile = fitness_tab[(int)round(gNbOfRobots*0.75)];
            avgFitnessNormalized = sumOfFitnesses/activeCount;
             if ( gVerbose && localVerbose )
            {
                std::cout << "[ gen:" << (gWorld->getIterations()/TemplatePhototaxisSharedData::gEvaluationTime) << "\tit:" << gWorld->getIterations() << "\tpop:" << activeCount << "\tminFitness:" << minFitness << "\tmaxFitness:" << maxFitness << "\tavgFitnessNormalized:" << avgFitnessNormalized << " ]\n";
            }

            std::string sLitelog = ""
            + std::to_string(gWorld->getIterations()/TemplatePhototaxisSharedData::gEvaluationTime)
            + ","
            + std::to_string(gWorld->getIterations())
            + ","
            + std::to_string(minFitness)
            + ","
            + std::to_string(maxFitness)
            + ","
            + std::to_string(avgFitnessNormalized)
            + ","
            + std::to_string(first_quartile)
            + ","
            + std::to_string(median)
            + ","
            + std::to_string(third_quartile);

            TemplatePhototaxisSharedData::gLitelogManager->write(sLitelog);
            TemplatePhototaxisSharedData::gLitelogManager->flush(); 
            TemplatePhototaxisSharedData::gLitelogFile << std::endl; 
        }

        if(TemplatePhototaxisSharedData::gDistanceLightLogInformation){

            double avgdistanceLighNormalized = 0.0;
            std::sort(distanceLight_tab, distanceLight_tab+gNbOfRobots);
            first_quartile_DistanceLight = distanceLight_tab[(int)round(gNbOfRobots*0.25)];
            median_DistanceLight = distanceLight_tab[(int)round(gNbOfRobots*0.5)];
            third_quartile_DistanceLight = distanceLight_tab[(int)round(gNbOfRobots*0.75)];
            avgdistanceLighNormalized = sumOfDistanceLight/activeCount;
            if ( gVerbose && localVerbose )
            {
                std::cout << "[ gen:" << (gWorld->getIterations()/TemplatePhototaxisSharedData::gEvaluationTime) << "\tit:" << gWorld->getIterations() << "\tpop:" << activeCount << "\tminDistance:" << minDistanceLight << "\tmaxDistance:" << maxDistanceLight << "\tavgDistance:" << avgdistanceLighNormalized << " ]\n";
            }

            std::string sDistanceLightlog = ""
            + std::to_string(gWorld->getIterations()/TemplatePhototaxisSharedData::gEvaluationTime)
            + ","
            + std::to_string(gWorld->getIterations())
            + ","
            + std::to_string(minDistanceLight)
            + ","
            + std::to_string(maxDistanceLight)
            + ","
            + std::to_string(avgdistanceLighNormalized)
            + ","
            + std::to_string(first_quartile_DistanceLight)
            + ","
            + std::to_string(median_DistanceLight)
            + ","
            + std::to_string(third_quartile_DistanceLight);
            
            TemplatePhototaxisSharedData::gDistanceLightlogManager->write(sDistanceLightlog);
            TemplatePhototaxisSharedData::gDistanceLightlogManager->flush();  
            TemplatePhototaxisSharedData::gDistanceLightlogFile << std::endl;
            
        }
        delete[] fitness_tab;
        delete[] distanceLight_tab;
    }
}



void TemplatePhototaxisWorldObserver::updateEnvironment()
{
    if ( gWorld->getIterations() == 0){
        gLandmarks[0]->setColor(255,255,0);  
        gLandmarks[0]->setCoordinates(TemplatePhototaxisSharedData::lightplaces[TemplatePhototaxisSharedData::lightPlaceIndice ].first, TemplatePhototaxisSharedData::lightplaces[TemplatePhototaxisSharedData::lightPlaceIndice ].second );      

    }
    else{
        // if light is not fixe in the center 
        if(  TemplatePhototaxisSharedData::lightmode != 1){
            if ( gWorld->getIterations() % TemplatePhototaxisSharedData::frequencyLightChange == 0 ){
                if(TemplatePhototaxisSharedData::lightPlaceIndice == 0){
                    TemplatePhototaxisSharedData::lightPlaceIndice = 1;
                }
                else{
                    TemplatePhototaxisSharedData::lightPlaceIndice = 0;
                }
                gLandmarks[0]->setCoordinates(TemplatePhototaxisSharedData::lightplaces[TemplatePhototaxisSharedData::lightPlaceIndice].first, TemplatePhototaxisSharedData::lightplaces[TemplatePhototaxisSharedData::lightPlaceIndice].second );
            }
        }
    } 
}

