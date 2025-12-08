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

#include "ceLoadSaveCTGS.h"
#include "../conversation/ceConversation.h"
#include "../conversation/playback/cePlayback.h"
#include "../conversation/playback/command/cePlaybackCommand.h"
#include "../conversation/playback/command/cePlaybackCommandList.h"
#include "../conversation/playback/variable/cePlaybackVariableList.h"
#include "../conversation/playback/variable/cePlaybackVariable.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <deigde/triggersystem/igdeTriggerTarget.h>



// Class ceLoadSaveCTGS
/////////////////////////

// Constructor, destructor
////////////////////////////

ceLoadSaveCTGS::ceLoadSaveCTGS(ceLoadSaveSystem &loadSaveSystem,
deLogger *logger, const char *loggerSource) :
igdeBaseXML(logger, loggerSource),
pLoadSaveSystem(loadSaveSystem),
pName("Conversation Test Game State"),
pPattern(".dectgs"){
}



// Loading and saving
///////////////////////

void ceLoadSaveCTGS::LoadCTGS(ceConversation &conversation, decBaseFileReader &reader){
	decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::NewWith());
	
	decXmlParser(GetLogger()).ParseXml(&reader, xmlDoc);
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if(!root || root->GetName() != "conversationTestGameState"){
		DETHROW(deeInvalidParam);
	}
	
	ReadGameState(*root, *conversation.GetPlayback());
	
	conversation.NotifyPlaybackCommandListChanged();
	conversation.NotifyPlaybackVarListChanged();
}

void ceLoadSaveCTGS::SaveCTGS(const ceConversation &conversation, decBaseFileWriter &writer){
	decXmlWriter xmlWriter(&writer);
	xmlWriter.WriteXMLDeclaration();
	WriteGameState(xmlWriter, *conversation.GetPlayback(), "conversationTestGameState");
}

void ceLoadSaveCTGS::WriteGameState(decXmlWriter &writer, const cePlayback &playback,
const char *tagName){
	writer.WriteOpeningTag(tagName, false, true);
	
	// write commands
	const cePlaybackCommandList &commandList = playback.GetCommandList();
	const int commandCount = commandList.GetCount();
	int i;
	
	for(i=0; i<commandCount; i++){
		const cePlaybackCommand &command = *commandList.GetAt(i);
		
		writer.WriteOpeningTagStart("command");
		writer.WriteAttributeBool("value", command.GetValue());
		writer.WriteOpeningTagEnd(false, false);
		writer.WriteTextString(command.GetCommand());
		writer.WriteClosingTag("command", false);
	}
	
	// write variables
	const cePlaybackVariableList &variableList = playback.GetVariableList();
	const int variableCount = variableList.GetCount();
	
	for(i=0; i<variableCount; i++){
		const cePlaybackVariable &variable = *variableList.GetAt(i);
		
		writer.WriteOpeningTagStart("variable");
		writer.WriteAttributeString("name", variable.GetName());
		writer.WriteOpeningTagEnd(false, false);
		writer.WriteTextInt(variable.GetValue());
		writer.WriteClosingTag("variable", false);
	}
	
	// write trigger table
	const igdeTriggerTargetList &triggerTable = playback.GetTriggerTable();
	const int triggerCount = triggerTable.GetCount();
	
	for(i=0; i<triggerCount; i++){
		const igdeTriggerTarget &trigger = *triggerTable.GetAt(i);
		
		writer.WriteOpeningTagStart("trigger");
		writer.WriteAttributeBool("fired", trigger.GetFired());
		writer.WriteAttributeBool("hasFired", trigger.GetHasFired());
		writer.WriteOpeningTagEnd(false, false);
		writer.WriteTextString(trigger.GetName());
		writer.WriteClosingTag("trigger", false);
	}
	
	writer.WriteClosingTag(tagName, true);
}

void ceLoadSaveCTGS::ReadGameState(const decXmlElementTag &root, cePlayback &playback){
	cePlaybackVariableList &variableList = playback.GetVariables();
	igdeTriggerTargetList &triggerTable = playback.GetTriggerTable();
	cePlaybackCommandList &commandList = playback.GetCommands();
	const int elementCount = root.GetElementCount();
	int i;
	
	commandList.RemoveAll();
	variableList.RemoveAll();
	
	for(i=0; i<elementCount; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if(tagName == "command"){
			cePlaybackCommand::Ref command = NULL;
			
			try{
				command.TakeOver(new cePlaybackCommand(GetCDataString(*tag),
					GetAttributeBool(*tag, "value")));
				commandList.Add(command);
			}catch(const deException &){
				throw;
			}
			
		}else if(tagName == "variable"){
			variableList.Set(GetAttributeString(*tag, "name"), GetCDataInt(*tag));
			
		}else if(tagName == "trigger"){
			igdeTriggerTarget::Ref trigger = NULL;
			
			try{
				trigger.TakeOver(new igdeTriggerTarget(GetCDataString(*tag)));
				trigger->SetFired(GetAttributeBool(*tag, "fired"));
				trigger->SetHasFired(GetAttributeBool(*tag, "hasFired"));
				triggerTable.Add(trigger);
			}catch(const deException &){
				throw;
			}
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
}



// Private Functions
//////////////////////
