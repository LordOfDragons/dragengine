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

#include "igdeDEConsole.h"
#include "igdeDialogEngine.h"
#include "../igdeUIHelper.h"
#include "../igdeComboBox.h"
#include "../igdeTextArea.h"
#include "../igdeTextField.h"
#include "../igdeContainer.h"
#include "../event/igdeComboBoxListener.h"
#include "../event/igdeTextFieldListener.h"
#include "../model/igdeListItem.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decHelperFunctions.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/common/string/unicode/decUnicodeLineBuffer.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deBaseModule.h>
#include <dragengine/systems/modules/deLoadableModule.h>



// Actions
////////////

class igdeDEConsole_TextCommand : public igdeTextFieldListener{
	igdeDEConsole &pPanel;
	
public:
	typedef deTObjectReference<igdeDEConsole_TextCommand> Ref;
	
	igdeDEConsole_TextCommand(igdeDEConsole &panel) : pPanel(panel){}
	
	virtual void OnEnterKey(igdeTextField*){
		pPanel.SendCommand();
	}
};



// Class igdeDEConsole
////////////////////////
	
// Constructor, destructor
////////////////////////////

igdeDEConsole::igdeDEConsole(igdeDialogEngine &windowEngine) :
igdeContainerFlow(windowEngine.GetEnvironment(), igdeContainerFlow::eaY, igdeContainerFlow::esFirst),
pDialogEngine(windowEngine),
pLog(1000)
{
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelper();
	
	
	helper.EditString(*this, "Console Logs", pEditLogs, 10, {});
	
	igdeContainerFlow::Ref line(igdeContainerFlow::Ref::New(
		env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst));
	AddChild(line);
	
	helper.EditString(line, "Command to send", pEditCommand, igdeDEConsole_TextCommand::Ref::New(*this));
	
	helper.ComboBox(line, "Module to send command to", pCBModule, {});
	pCBModule->SetDefaultSorter();
	
	
	UpdateModulesList();
}

igdeDEConsole::~igdeDEConsole(){
}



// Management
///////////////

void igdeDEConsole::UpdateModulesList(){
	const deModuleSystem &moduleSystem = *GetEngine()->GetModuleSystem();
	const int count = moduleSystem.GetModuleCount();
	int i;
	
	for(i=0; i<count; i++){
		deLoadableModule * const module = moduleSystem.GetModuleAt(i);
		if(!pCBModule->HasItem(module->GetName())){
			pCBModule->AddItem(module->GetName());
		}
	}
	pCBModule->SortItems();
}

void igdeDEConsole::AddToConsole(decUnicodeString &text){
	const bool atBottom = pEditLogs->GetBottomLine() == pEditLogs->GetLineCount() - 1;
	
	pLog.AddMultipleLines(text);
	
	decUnicodeString output;
	pLog.FillLinesInto(output);
	pEditLogs->SetText(output.ToUTF8());
	
	if(atBottom){
		pEditLogs->SetBottomLine(pEditLogs->GetLineCount() - 1);
	}
}

void igdeDEConsole::ClearConsole(){
	pLog.Clear();
	pEditLogs->ClearText();
}

void igdeDEConsole::SendCommand(){
	if(!pCBModule->GetSelectedItem()){
		return;
	}
	
	// if this is a special command do it
	if(pEditCommand->GetText() == "/clear"){
		ClearConsole();
		return;
	}
	
	// determine which module to send the command to
	deLoadableModule * const loadableModule = GetEngine()->GetModuleSystem()->
		GetModuleNamed(pCBModule->GetSelectedItem()->GetText());
	if(!loadableModule){
		return;
	}
	
	deBaseModule * const module = loadableModule->GetModule();
	if(!module){
		return;
	}
	
	// get the command and build an argument list from it
	decUnicodeArgumentList argList;
	decUnicodeString command;
	
	command.SetFromUTF8(pEditCommand->GetText());
	argList.ParseCommand(command);
	if(argList.GetArgumentCount() == 0){
		return;
	}
	
	// add commend to send
	decUnicodeString answer;
	answer.SetFromUTF8(loadableModule->GetName());
	answer.AppendFromUTF8(" > ");
	answer += command;
	AddToConsole(answer);
	
	// send command to the selected module
	try{
		answer.SetFromUTF8("");
		module->SendCommand(argList, answer);
		
	}catch(const deException &e){
		answer.SetFromUTF8(DEJoin(e.FormatOutput(), "\n"));
	}
	
	if(answer.GetLength() > 0 && answer.GetAt(answer.GetLength() - 1) != '\n'){
		answer.AppendFromUTF8("\n");
	}
	
	AddToConsole(answer);
}
