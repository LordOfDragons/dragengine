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

#include "desynCommandExecuter.h"
#include "desynBasics.h"
#include "deDESynthesizer.h"
#include "desynConfiguration.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>



// Class desynCommandExecuter
///////////////////////////////

// Constructor, destructor
////////////////////////////

desynCommandExecuter::desynCommandExecuter(deDESynthesizer &module) :
pModule(module){
}

desynCommandExecuter::~desynCommandExecuter(){
}



// Management
///////////////

void desynCommandExecuter::ExecuteCommand(const decUnicodeArgumentList &command, decUnicodeString &answer){
	if(command.GetArgumentCount() == 0){
		answer.SetFromUTF8("No command provided.");
		
	}else{
		if(command.MatchesArgumentAt(0, "help")){
			CmdHelp(command, answer);
			
		}else{
			answer.SetFromUTF8("Unknown command '");
			answer += *command.GetArgumentAt(0);
			answer.AppendFromUTF8("'.");
		}
	}
}



void desynCommandExecuter::CmdHelp(const decUnicodeArgumentList &command, decUnicodeString &answer){
	answer.SetFromUTF8("help => Displays this help screen.\n");
}
