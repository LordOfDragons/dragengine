/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "declActionHelp.h"
#include "declActionGames.h"
#include "declActionDelga.h"
#include "declActionPatches.h"
#include "declRunGame.h"
#include "declListProfiles.h"
#include "../declLauncher.h"

#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Definitions
////////////////

#define LOGSOURCE "Launcher"



// Class declActionHelp
////////////////////////

// Constructor, destructor
////////////////////////////

declActionHelp::declActionHelp(declLauncher &launcher) :
pLauncher(launcher){
}

declActionHelp::~declActionHelp(){
}



// Management
///////////////

void declActionHelp::Run(){
	const decUnicodeArgumentList &argumentList = pLauncher.GetArgumentList();
	
	if(argumentList.GetArgumentCount() < 2){
		pLauncher.PrintSyntax();
		return;
	}
	
	const decString actionName = argumentList.GetArgumentAt(1)->ToUTF8();
	
	if(actionName == "run"){
		declRunGame(pLauncher).PrintSyntax();
		
	}else if(actionName == "delga"){
		declActionDelga(pLauncher).PrintSyntax();
		
	}else if(actionName == "games"){
		declActionGames(pLauncher).PrintSyntax();
		
	}else if(actionName == "patches"){
		declActionPatches(pLauncher).PrintSyntax();
		
	}else if(actionName == "profiles"){
		declListProfiles(pLauncher).PrintSyntax();
		
	}else{
		pLauncher.PrintSyntax();
		return;
	}
}
