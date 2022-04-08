/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */



#include "TemplateVanillaEE/include/TemplateVanillaEESharedData.h"

// cf. super class for many parameter values initialization.
// Add here initialization for parameters that are specific to current implementation.
//
// Quick help:
//  if parameter is already defined in TemplateEEShareData, then use TemplateEEShareData::parametername
//  to define a new parameter, do it in TemplateVanillaEESharedData.h, initialize it in TemplateVanillaEESharedData.cpp, then use TemplateVanillaEESharedData::parametername
//
int TemplateVanillaEESharedData::maturationDelayDefaultValue = 400;
int TemplateVanillaEESharedData::memorySlidingWindowSize = 400;
int TemplateVanillaEESharedData::coordSlidingWindowSize = 400;
int TemplateVanillaEESharedData::fitnessFunction = 0;
