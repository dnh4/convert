/*!
 * \file acrxEntryPoint.h
 * \brief Point d'entree du code ObjectARX, fait office de main
 * \author Dinahasina
 * \date 15 juillet 2019
 */

#pragma once
#include "cmdConvert.h"

void initApp();
void initCmd( AcString sCmdName, GcRxFunctionPtr FunctionAddr );
void unloadApp();
extern "C" AcRx::AppRetCode acrxEntryPoint( AcRx::AppMsgCode msg, void* appId );


//Declarer le groupe name
AcString sGroupName = "TEMPLATE";