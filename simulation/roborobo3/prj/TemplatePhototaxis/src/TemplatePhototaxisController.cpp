
#include "TemplatePhototaxis/include/TemplatePhototaxisController.h"


/*******************************************************************************/
// Public method
/*******************************************************************************/


void split(const std::string &chaine, char delimiteur, std::vector<std::string> &elements)
{
    std::stringstream ss(chaine);
    std::string sousChaine;
    while (getline(ss, sousChaine, delimiteur))
    {
    elements.push_back(sousChaine);
    }
}

std::vector<float> split_vect(const std::string &chaine, char delimiteur) 
{
    
    std::vector<float> genom;
    std::vector<std::string> values;
    split(chaine, delimiteur, values);
    for (unsigned int  i = 0; i < values.size();i++){
        float val = stod(values[i]);
        genom.push_back(val);
    }
    return genom;
}

TemplatePhototaxisController::TemplatePhototaxisController( RobotWorldModel *wm )  {
 
    _wm = wm;
    _nn = NULL;
    _fitnessValue = 0.0;
    _lastLifeChange = 0;
    _maturationDelay = TemplatePhototaxisSharedData::maturationDelayDefaultValue;
    _cptTimerout = TemplatePhototaxisSharedData::timeout;
    _action = 0;
    _minValue = -1.0;
    _maxValue = 1.0;
    _iteration = 0;
    _birthdate = 0;
    _currentSigma = TemplatePhototaxisSharedData::gSigmaRef;

    if ( _wm->_cameraSensorsNb != NB_SENSORS )
    {
        std::cerr << "[CRITICAL] This project assumes robot specifications with " << NB_SENSORS << " sensors (provided: " << _wm->_cameraSensorsNb << " sensors). STOP.\n";
        exit(-1);
    }
    if ( gNbOfLandmarks > 0 )
        _wm->updateLandmarkSensor(); 
    _wm->setAlive(true);
    _wm->setRobotLED_colorValues( 0,0,0 );
    


    _rewards = new SlidingWindow(TemplatePhototaxisSharedData::memorySlidingWindowSize);
    _rewards->setAlwaysPositiveSum(true);
    _rewardsSecond = new SlidingWindow(TemplatePhototaxisSharedData::memorySlidingWindowSize);
    _rewardsSecond->setAlwaysPositiveSum(true);
    _rewardsSecond->reset(0.0);
    _distancesFromLight = new SlidingWindow(TemplatePhototaxisSharedData::memorySlidingWindowSize);
    _distancesFromLight->setAlwaysPositiveSum(true);
    _distancesFromLight->reset(0.0);
    if (TemplatePhototaxisSharedData::genomFile != "LEARNING"){
        std::string line;
        std::ifstream myFile(TemplatePhototaxisSharedData::genomFile);
        if(!myFile.is_open()){
            std::cout<<"File genom not found : "<<TemplatePhototaxisSharedData::genomFile<<"\n";
            exit(0);
        }
        int cpt = 0;
        while (cpt <= this->getId()){
            std::getline(myFile, line);
            cpt +=1;
        }

        std::istringstream iss(line);
        char delimiter = '_';
        std::vector<float> genom = split_vect(line, delimiter); 
        for(unsigned int i = 0 ; i< genom.size(); i++){
            _importedGenome.push_back(genom[i]);    
        }
        
        

    } 
    reset();
    resetFitness();

    if ( TemplatePhototaxisSharedData::transferVolume == 0.0 )
        _packetLength_HIT = 1; // default minimal value
    else if ( TemplatePhototaxisSharedData::transferVolume == 1.0 )
        _packetLength_HIT = (int)_currentGenome.size();
    else
        _packetLength_HIT = (int)_currentGenome.size() * TemplatePhototaxisSharedData::transferVolume;

}



TemplatePhototaxisController::~TemplatePhototaxisController()
{
    _parameters.clear();
    delete _nn;
    _nn = NULL;
}

void TemplatePhototaxisController::step() 
{
    _iteration++;
    updateFitness();
    stepEvolution();
    stepController();
}

bool TemplatePhototaxisController::getNewGenomeStatus() { return _isNewGenome; }
void TemplatePhototaxisController::setNewGenomeStatus( bool __status ) { _isNewGenome = __status; }

double TemplatePhototaxisController::getFitness(){
    return this->_fitnessValue;
}

double TemplatePhototaxisController::getFitnessSecond(){
    return _rewardsSecond->getSum();
}



int TemplatePhototaxisController::getBirthdate(){
    return this->_birthdate;
}

int TemplatePhototaxisController::getIteration(){
    return this->_iteration;
}

int TemplatePhototaxisController::getAction(){
    return this->_action;
}




double TemplatePhototaxisController::getAveragedistanceFromLight(){
    return this->_distancesFromLight->getAverageValue();
}

std::string TemplatePhototaxisController::getGenome(){
    std::string value = "";
    std::vector<double> g = _currentGenome;
    unsigned int i = 0;
    for( i = 0; i < g.size() -1 ; i++){
        value += std::to_string(g[i])+'_';
    }
    value += std::to_string(g[i]);
    return value;

}


//General


void TemplatePhototaxisController::reset()
{
    this->initController();
    this->initEvolution();
}

/*******************************************************************************/
// Controller

std::vector<double> TemplatePhototaxisController::getInputs(){

    std::vector<double> inputs;
    
    double lightIntesity =  this->_distancesFromLight->getPrev(0);
    inputs.push_back(lightIntesity);
    return inputs;
}

void TemplatePhototaxisController::stepController()
{

    std::vector<double> inputs = this->getInputs();    
    std::vector<double> parametersTmp;
    int s = _parameters.size();
    for (int i = 0 ; i < s ; i++){
       /* if (_parameters[i] > 1 || _parameters[i] < -1 ){
            std::cout << "[ERROR] Weight not between -1 and +1 "<<_parameters[i] <<" "<<i<<" Exit"<<"\n";
            exit(-1);
        }*/
        parametersTmp.push_back(_parameters[i]);
    }
  
  
    
    _nn->setWeights(parametersTmp); 
    _nn->setInputs(inputs);
    _nn->step();
    std::vector<double> outputs = _nn->readOut();  
    
    s = outputs.size();
    for (int i = 0 ; i < s ; i++){
        if (outputs[i] > 1 || outputs[i] < -1 ){
            std::cout << "[ERROR] outputs not between -1 and +1 "<<outputs[i] <<" "<<i<<" Exit"<<"\n";
            exit(-1);
        }
    }

    if(outputs[0] >= 0){
        this->_action = 0;
        _wm->setRobotLED_colorValues( 125,0,0 );
        defaultBehavior(0);
    }
    else{
        this->_action = 1;
        _wm->setRobotLED_colorValues( 0,0,125 );
        defaultBehavior(1);
    }
}

void TemplatePhototaxisController::setIOcontrollerSize()
{
    _nbInputs = 0;
    if (gSensoryInputs_lightDetection){
         _nbInputs += 1;
    }
        
    _nbOutputs = 0;
    if (gSensoryInputs_lightDetection)
        _nbOutputs += 1;
}

// TO DO : noise modulable en foction de la vitesse
void TemplatePhototaxisController::defaultBehavior(int signal)
{
    
    double distanceOnMyLeft = 0.0;
    double distanceOnMyRight = 0.0;
   for(int i=0 ; i <=6 ; i++){
            distanceOnMyLeft  += _wm->getNormalizedDistanceValueFromCameraSensor(i);
    }
    for(int i=8; i <=14 ; i++){
            distanceOnMyRight += _wm->getNormalizedDistanceValueFromCameraSensor(i);
    }
    setTranslation(TemplatePhototaxisSharedData::TranslationSpeed);
    ///   AntiAlignment
    if (signal == 0){

        if ( distanceOnMyLeft < distanceOnMyRight ){
            setRotation( -(TemplatePhototaxisSharedData::RotationalSpeed )); 
        }
        else{
            if ( distanceOnMyRight < distanceOnMyLeft ){
                setRotation( +( TemplatePhototaxisSharedData::RotationalSpeed) );
            }
            else{
                setRotation( TemplatePhototaxisSharedData::DiffusionSpeed * randgaussian());
            }
        } 
    }
    
    else{
        if ( distanceOnMyLeft > distanceOnMyRight ){
            setRotation( -(TemplatePhototaxisSharedData::RotationalSpeed) );
            
        }
       else{
            if ( distanceOnMyRight > distanceOnMyLeft ){
                setRotation( +(TemplatePhototaxisSharedData::RotationalSpeed) );
            }
            else{
                setRotation( TemplatePhototaxisSharedData::DiffusionSpeed * randgaussian() );
            }
        }
    } 
}

void TemplatePhototaxisController::initController()
{
    _nbHiddenLayers = TemplatePhototaxisSharedData::gNbHiddenLayers;
    _nbNeuronsPerHiddenLayer = new std::vector<unsigned int>(_nbHiddenLayers);
    for(unsigned int i = 0; i < _nbHiddenLayers; i++)
        (*_nbNeuronsPerHiddenLayer)[i] = TemplatePhototaxisSharedData::gNbNeuronsPerHiddenLayer;

    createNN();
}

void TemplatePhototaxisController::createNN()
{
    setIOcontrollerSize(); // compute #inputs and #outputs

    if ( _nn != NULL ) // useless: delete will anyway check if nn is NULL or not.
        delete _nn;

    switch ( TemplatePhototaxisSharedData::gControllerType )
    {
        case 0:
        {
            // MLP
	    //std::cout<< "MLP "<<_parameters.size() << ", " << _nbInputs << ", " <<_nbOutputs<<' '<< std::endl;
	    //std::cout<< _nbHiddenLayers << ", " << (*_nbNeuronsPerHiddenLayer)[0] << std::endl;
            _nn = new MLP(_parameters, _nbInputs, _nbOutputs, *(_nbNeuronsPerHiddenLayer), true);
            break;
        }
        case 1:
        {
            // PERCEPTRON
            //std::cout<<"PERCEPTRON : "<<_parameters.size()<<" "<<_nbInputs<<" "<< _nbOutputs<<"\n";
            _nn = new Perceptron(_parameters, _nbInputs, _nbOutputs);
            break;
        }
        
        default: // default: no controller
            std::cerr << "[ERROR] gController type unknown (value: " << TemplatePhototaxisSharedData::gControllerType << ").\n";
            exit(-1);
    };
}

unsigned int TemplatePhototaxisController::computeRequiredNumberOfWeights()
{
    unsigned int res = _nn->getRequiredNumberOfWeights();
    return res;
}

/*******************************************************************************/
//Rewards 

void TemplatePhototaxisController::resetFitness() {
    _rewards->reset(0.0);
    this->_fitnessValue = 0.0;
}

void TemplatePhototaxisController::updateFitness() {
    
    Point2d coord = this->getPosition();

    double mydist =  getEuclideanDistance(TemplatePhototaxisSharedData::lightplaces[TemplatePhototaxisSharedData::lightPlaceIndice].first,TemplatePhototaxisSharedData::lightplaces[TemplatePhototaxisSharedData::lightPlaceIndice].second,coord.x,coord.y);
    //mydist normalisé entre 0 et 1
    mydist /= TemplatePhototaxisSharedData::maxDistanceFromLight;
    if ( mydist > 1 ||  mydist < 0){

        std::cerr<<"[ERROR]  Error of distance value "<< mydist<<" "<<coord.x<<" "<<coord.y<<"\n";
        std::cout<<TemplatePhototaxisSharedData::lightPlaceIndice<<"\n";
        std::cout<<"maxDistanceFromLight  "<<TemplatePhototaxisSharedData::maxDistanceFromLight<<"\n";
        std::cout<<TemplatePhototaxisSharedData::lightplaces[TemplatePhototaxisSharedData::lightPlaceIndice].first<<" "<<TemplatePhototaxisSharedData::lightplaces[TemplatePhototaxisSharedData::lightPlaceIndice].second<<"\n";
        exit(-1);
    }
    this->_distancesFromLight->push(mydist);
    _rewards->push( 1.0 - mydist );
    _rewardsSecond->push( 1.0 - mydist );
    this->_fitnessValue = _rewards->getSum();
    
}

/*******************************************************************************/
// communication

void TemplatePhototaxisController::broadcastGenome(){
    for( int i = 0 ; i < _wm->_cameraSensorsNb; i++)
    {
        int targetIndex = _wm->getObjectIdFromCameraSensor(i);

        if ( targetIndex >= gRobotIndexStartOffset && targetIndex < gRobotIndexStartOffset+gNbOfRobots )   // sensor ray bumped into a robot : communication is possible
        {
            targetIndex = targetIndex - gRobotIndexStartOffset; // convert image registering index into robot id.

            TemplatePhototaxisController* targetRobotController = dynamic_cast<TemplatePhototaxisController*>(gWorld->getRobot(targetIndex)->getController());

            if ( ! targetRobotController )
            {
                std::cerr << "Error from robot " << _wm->getId() << " : the observer of robot " << targetIndex << " is not compatible." << std::endl;
                exit(-1);
            }
            if (TemplatePhototaxisSharedData::genomFile == "LEARNING"){
                sendGenome(targetRobotController);
            }
            
        }
    }
}

bool TemplatePhototaxisController::sendGenome( TemplatePhototaxisController* __targetRobotController ) {

    if ( _maturationDelay > 0 || getNewGenomeStatus() == true ){
        return false;
    }

    Packet* p = new Packet();
    p->senderId = std::make_pair(_wm->getId(), _birthdate);
    p->genome = _currentGenome;
    p->sigma = _currentSigma;
    p->fitness = getFitness();
    bool retValue = ((TemplatePhototaxisController*)__targetRobotController)->receiveGenome(p);
    if (retValue and TemplatePhototaxisSharedData::gCommunicationLogInformation){
      std::string comLog = std::to_string((this->_iteration-1)/TemplatePhototaxisSharedData::gEvaluationTime)
          +","+std::to_string(this->_iteration)
          +","+std::to_string(this->getId())
          +","+std::to_string(((TemplatePhototaxisController*)__targetRobotController)->getId())
          + "\n";
      TemplatePhototaxisSharedData::gComlogManager->write(comLog);
      TemplatePhototaxisSharedData::gComlogManager->flush();
    } 
    delete p;
    return retValue;
}

bool TemplatePhototaxisController::receiveGenome(Packet* p)
{

    if ( getNewGenomeStatus()== true || _maturationDelay > 0 ){
            if(TemplatePhototaxisSharedData::communicationTypeRegistration == 2){
                return true;
            }
            return false;
    }
    if(TemplatePhototaxisSharedData::genomFile != "LEARNING"){
        std::cout<<"PostMortme Run : call to receiveGenome "<<"\n";
        exit(0);
    }
  
    std::map<std::pair<int,int>, std::vector<double> >::const_iterator it = _genomesList.find(p->senderId);
    if ( it != _genomesList.end() ) // this exact agent's genome is already stored. Exact means: same robot, same generation. Then: update fitness value (the rest in unchanged)
    {
        _fitnessValueList[p->senderId] = p->fitness; // update with most recent fitness
        if (TemplatePhototaxisSharedData::communicationTypeRegistration == 0){
            return false;
        }
        return true;
    }
    
    if(((int)_genomesList.size() )>=  TemplatePhototaxisSharedData::reservoirSize)
    {
        if(TemplatePhototaxisSharedData::communicationTypeRegistration == 2){
            return true;
        }
        return false;
    }
    _genomesList[p->senderId] = p->genome;
    _sigmaList[p->senderId] = p->sigma;
    _fitnessValueList[p->senderId] = p->fitness;
    return true;
}


void TemplatePhototaxisController::clearReservoir()
{
    _genomesList.clear(); // empty the list of received genomes
    _sigmaList.clear();
    _fitnessValueList.clear();
}

/*******************************************************************************/
 // selection
 void TemplatePhototaxisController::performSelection() {


    if( _maturationDelay <= 0){
        int buffsize = ((int)_genomesList.size());
        if (TemplatePhototaxisSharedData::genomFile != "LEARNING" and buffsize > 0){
            std::cout<<"Erro : buffsize > 0 and genomFile != NULL\n";
            exit(0);
        }
        if((buffsize  >= TemplatePhototaxisSharedData::reservoirSize) || ((buffsize >= 1 ) and _cptTimerout <= 0) ) 
        {
            std::pair<int, int> index;       
            switch (TemplatePhototaxisSharedData::gSelectionMethod) {
                case 0:
                    index = selectRandomGenome();
                    break;
                case 1:
                    index = selectBestGenome();
                    break;
                case 2:
                    index = selectFitProp();
                    break;
                default:
                    std::cerr << "[ERROR] unknown selection Op (selectionOp = " << TemplatePhototaxisSharedData::gSelectionMethod << ")\n";
                    exit(-1);
            }
            if(isAuthorizedSelection(index)){
                setNewGenomeStatus(false);
                transfertGenomes(index);
                this->performVariation();
                setNewGenomeStatus(true);
            }
            else{
                clearReservoir();
            }
            
        } 
    }
}

void TemplatePhototaxisController::transfertGenomes(std::pair<int, int> selectedIndex){
    std::vector<int> indexes;
    for ( unsigned int i = 0 ; i < _currentGenome.size(); i++ )
        indexes.push_back(i);
    if ( TemplatePhototaxisSharedData::transferVolume < 1.0 )
        std::random_shuffle(indexes.begin(), indexes.end());
    for ( int i = 0 ; i < _packetLength_HIT ; i++ )
        _currentGenome[indexes[i]] = _genomesList[selectedIndex][indexes[i]];
    this->_birthdate += 1;

}

bool TemplatePhototaxisController::isAuthorizedSelection(std::pair<int, int> selectedIndex){

    if(TemplatePhototaxisSharedData::transferFilter == true){
        if(( TemplatePhototaxisSharedData::strictlySuperiorSelection == true and _fitnessValueList[selectedIndex] <= getFitness() ) or ( TemplatePhototaxisSharedData::strictlySuperiorSelection == false and _fitnessValueList[selectedIndex] < getFitness() ) ){
            return false;
        }
    }
    return true;
}

std::pair<int, int> TemplatePhototaxisController::selectRandomGenome() {
    int randomIndex = randint()%_genomesList.size();
    std::map<std::pair<int,int>, std::vector<double> >::iterator it = _genomesList.begin();
    while (randomIndex !=0 )
    {
        it ++;
        randomIndex --;
    }
    std::pair<int,int> index  = (*it).first;
    return index;

}

std::pair<int, int> TemplatePhototaxisController::selectBestGenome() {

    std::pair<int,int> bestId;
    std::map<std::pair<int,int>, double >::iterator fitnessesIt = _fitnessValueList.begin();

    double bestFitnessValue = (*fitnessesIt).second;
    bestId = (*fitnessesIt).first;
    ++fitnessesIt;

    int nbSimilar = 0;

    for ( int i = 1 ; fitnessesIt != _fitnessValueList.end(); ++fitnessesIt, i++) {
        if ( (*fitnessesIt).second >= bestFitnessValue ) {
            if ( (*fitnessesIt).second > bestFitnessValue ) {
                bestFitnessValue = (*fitnessesIt).second;
                bestId = (*fitnessesIt).first;
                nbSimilar = 0;
            } else {
                nbSimilar++;
            }
        }
    }
    if ( nbSimilar > 0 ) {
        int count = 0;
        int randomPick = randint()%nbSimilar;
        if ( randomPick != 0 ) {
            fitnessesIt = _fitnessValueList.begin();
            for ( int i = 0 ; ; ++fitnessesIt, i++) {
                if ( (*fitnessesIt).second == bestFitnessValue ) {
                    if ( count == randomPick ) {
                        bestId = (*fitnessesIt).first;
                        break;
                    }
                    count++;
                }
            }
        }
    }
    return bestId;
}


std::pair<int, int> TemplatePhototaxisController::selectFitProp() {
    std::pair<int,int> selectId;
    double sumOfFit = 0;
    std::map<std::pair<int,int>, double >::iterator fitnessesIt = _fitnessValueList.begin();
    for ( ; fitnessesIt != _fitnessValueList.end(); ++fitnessesIt ) {
        sumOfFit += (*fitnessesIt).second;
    }
    double fitnessTarget = random01()*sumOfFit;
    double currentSum = 0;
    fitnessesIt = _fitnessValueList.begin();
    for ( ; fitnessesIt != _fitnessValueList.end(); ++fitnessesIt ){
        currentSum += (*fitnessesIt).second;
        if ( currentSum >= fitnessTarget ){
            selectId = (*fitnessesIt).first;
            break;
        }
    }
    return selectId;
}

/*******************************************************************************/
// Evolution

void TemplatePhototaxisController::initEvolution() {
    
    
    _currentGenome.clear();

    unsigned int const nbGene = computeRequiredNumberOfWeights();
    if (TemplatePhototaxisSharedData::genomFile != "LEARNING"){
            for ( unsigned int i = 0 ; i != nbGene ; i++ )
         {
            _currentGenome.push_back(_importedGenome[i]);

        }
    
    }
    else{
        for ( unsigned int i = 0 ; i != nbGene ; i++ )
         {
            _currentGenome.push_back(  ((double)(randint()%TemplatePhototaxisSharedData::gNeuronWeightRange)/(TemplatePhototaxisSharedData::gNeuronWeightRange/2)-1.0)); // weights: random init between -1 and +1

        }

    }
   
    setNewGenomeStatus(true);
    clearReservoir();
}

void TemplatePhototaxisController::stepEvolution() {
    

   if ( getNewGenomeStatus() ) // check for new NN parameters
    {
    
        
        if(TemplatePhototaxisSharedData::gLifeSpanLogInformation){
           
            std::string lifeSpanValues = std::to_string((this->_iteration-1)/TemplatePhototaxisSharedData::gEvaluationTime)+","+std::to_string(this->_iteration-1)
            +","+std::to_string(this->getId())
            +","+std::to_string(this->_iteration - this->_lastLifeChange- 1)
            +","+std::to_string(this->_birthdate)
            +"\n";
            TemplatePhototaxisSharedData::gLifeSpanlogManager->write(lifeSpanValues);
            TemplatePhototaxisSharedData::gLifeSpanlogManager->flush();
        }
        
        mapGenotypeToPhenotype();
        setNewGenomeStatus(false);
        
        _maturationDelay = TemplatePhototaxisSharedData::maturationDelayDefaultValue;
        _cptTimerout = TemplatePhototaxisSharedData::timeout;
        resetFitness();
        clearReservoir();
        _lastLifeChange = this->_iteration -1;

    }
    else
    {
        if(TemplatePhototaxisSharedData::genomFile == "LEARNING"){
            if(_maturationDelay > 0){
            _maturationDelay--;
            }
            else
            {
        
                // send genome to neighbours 
                broadcastGenome();
                if(_cptTimerout > 0)
                {
                    _cptTimerout--;
                }
                performSelection(); 
            } 
        }
    }
}

void TemplatePhototaxisController::mapGenotypeToPhenotype()
{

    _parameters.clear();
    _parameters = _currentGenome;
   
   
    
}


//variation
/*******************************************************************************/

void TemplatePhototaxisController::performVariation()
{
    if ( TemplatePhototaxisSharedData::gProbaMutation  > 0 and TemplatePhototaxisSharedData::gProbaMutation >= random01() and getNewGenomeStatus() == false and _maturationDelay  <= 0  ) // global mutation rate (whether this genome will get any mutation or not) - default: always
    {
        int nbGenesToMutate =  mutateMixte(_currentSigma); 
        if(TemplatePhototaxisSharedData::gMutateLogInformation){
            std::string mutateInfo = std::to_string((this->_iteration-1)/TemplatePhototaxisSharedData::gEvaluationTime)
            +","
            +std::to_string(this->_iteration-1)
            +","
            +std::to_string(this->getId())
            +","
            +std::to_string((int) _currentGenome.size())
            +","+
            std::to_string(nbGenesToMutate)
            +"\n";
            TemplatePhototaxisSharedData::gMutatelogManager->write(mutateInfo);
            TemplatePhototaxisSharedData::gMutatelogManager->flush();
        }

    }
}

double TemplatePhototaxisController::getBoundedGaussianValue(double value, double sigma, double minValue, double maxValue )
{
    double retValue = value + randgaussian()*sigma; 
    
    // bouncing upper/lower bounds
    if ( retValue < _minValue )
    {
        
        double range = maxValue - minValue; 
        double overflow = - ( (double)retValue - minValue ); 
        overflow = overflow - 2*range * (int)( overflow / (2*range) );
        if ( overflow < range ){
            retValue = minValue + overflow;
        }   
        else // overflow btw range and range*2
        {
            retValue = minValue + range - (overflow-range);
        }
            
    }
    else if ( retValue > maxValue )
    {
        double range = maxValue - minValue;
        double overflow = (double)retValue - maxValue;
        overflow = overflow - 2*range * (int)( overflow / (2*range) );
        if ( overflow < range )
            retValue = maxValue - overflow;
        else // overflow btw range and range*2
            retValue = maxValue - range + (overflow-range);
    }
    
    return retValue;
}


int TemplatePhototaxisController::mutateMixte(double sigma){
    int nbGenesToMutate = int(TemplatePhototaxisSharedData::gMutationVolume * _currentGenome.size());
    if (nbGenesToMutate <= 0){
        nbGenesToMutate = 1;
    }
    std::vector<int> indexes;
    for ( unsigned int i = 0 ; i < _currentGenome.size(); i++ ){
        indexes.push_back(i);
    }
    std::random_shuffle ( indexes.begin(), indexes.end() );
    for ( int i = 0 ; i < nbGenesToMutate ; i++ ){
        if (random01() <= TemplatePhototaxisSharedData::gProbaMutationUniform){
            _currentGenome[indexes[i]] = random01() * ( _maxValue - _minValue )  + _minValue;
        }
        else{
            double tmp = getBoundedGaussianValue( _currentGenome[indexes[i]], sigma, _minValue, _maxValue );
            _currentGenome[indexes[i]] = tmp;
        }
    }
    return nbGenesToMutate;
}

/*******************************************************************************/
