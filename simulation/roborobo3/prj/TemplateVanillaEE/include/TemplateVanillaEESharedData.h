/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */



#ifndef TEMPLATEVANILLAEESHAREDDATA_H
#define TEMPLATEVANILLAEESHAREDDATA_H

#include "TemplateEE/include/TemplateEESharedData.h"

class TemplateVanillaEESharedData : TemplateEESharedData {

    // cf. super class for many parameter values.
    // Add here parameters that are specific to current implementation.
public:
    static int maturationDelayDefaultValue; // suggest: superior or equal to memorySlidingWindowSize

    static int memorySlidingWindowSize; // 100...400
    static int coordSlidingWindowSize; // 3...10...400
    static int fitnessFunction; // 0: collect ; 1: wander

};


#endif
