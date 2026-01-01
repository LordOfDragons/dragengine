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

#include "ceLoadSaveCTA.h"
#include "ceLoadSaveSystem.h"
#include "../conversation/ceConversation.h"
#include "../conversation/actor/ceConversationActor.h"
#include "../conversation/actor/controller/ceActorController.h"
#include "../conversation/actor/gesture/ceActorGesture.h"
#include "../conversation/actor/pose/ceActorPose.h"
#include "../conversation/playback/command/cePlaybackCommand.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlParser.h>



// Class ceLoadSaveCTA
////////////////////////

// Constructor, destructor
////////////////////////////

ceLoadSaveCTA::ceLoadSaveCTA(ceLoadSaveSystem &loadSaveSystem,
deLogger *logger, const char *loggerSource) :
igdeBaseXML(logger, loggerSource),
pLoadSaveSystem(loadSaveSystem),
pName("Conversation Test Actor"),
pPattern(".decta"){
}



// Loading and saving
///////////////////////

void ceLoadSaveCTA::LoadCTA(ceConversationActor &actor, decBaseFileReader &reader){
	decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::New());
	
	decXmlParser(GetLogger()).ParseXml(&reader, xmlDoc);
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if(!root || root->GetName() != "conversationTestActor"){
		DETHROW(deeInvalidParam);
	}
	
	ReadActor(*root, actor);
}

void ceLoadSaveCTA::SaveCTA(const ceConversationActor &actor, decBaseFileWriter &writer){
	decXmlWriter xmlWriter(&writer);
	xmlWriter.WriteXMLDeclaration();
	WriteActor(xmlWriter, actor, "conversationTestActor");
}



void ceLoadSaveCTA::WriteActor(decXmlWriter &writer, const ceConversationActor &actor,
const char *tagName){
	writer.WriteOpeningTag(tagName, false, true);
	
	writer.WriteDataTagString("id", actor.GetID());
	writer.WriteDataTagString("aliasID", actor.GetAliasID());
	WriteVector(writer, "position", actor.GetPosition());
	WriteVector(writer, "orientation", actor.GetOrientation());
	writer.WriteDataTagString("model", actor.GetPathModel());
	writer.WriteDataTagString("skin", actor.GetPathSkin());
	writer.WriteDataTagString("rig", actor.GetPathRig());
	writer.WriteDataTagString("speechAnimation", actor.GetPathSpeechAnimation());
	writer.WriteDataTagString("eyesAnimator", actor.GetPathEyesAnimator());
	writer.WriteDataTagString("facePoseAnimator", actor.GetPathFacePoseAnimator());
	writer.WriteDataTagString("boneHeadRotator", actor.GetBoneHeadRotator());
	
	actor.GetPoses().Visit([&writer, this](const ceActorPose &pose){
		pWritePose(writer, pose);
	});
	
	actor.GetCommands().Visit([&writer](const cePlaybackCommand &c){
		writer.WriteOpeningTagStart("command");
		writer.WriteAttributeBool("value", c.GetValue());
		writer.WriteOpeningTagEnd(false, false);
		writer.WriteTextString(c.GetCommand());
		writer.WriteClosingTag("command", false);
	});
	
	actor.GetParameter().Visit([&writer](const decString &name, int value){
		writer.WriteOpeningTagStart("parameter");
		writer.WriteAttributeString("name", name);
		writer.WriteOpeningTagEnd(false, false);
		writer.WriteTextInt(value);
		writer.WriteClosingTag("parameter", false);
	});
	
	// active pose can be stored only now
	if(actor.GetActivePose()){
		writer.WriteDataTagString("activePose", actor.GetActivePose()->GetName());
	}
	
	writer.WriteClosingTag(tagName);
}

void ceLoadSaveCTA::ReadActor(const decXmlElementTag &root, ceConversationActor &actor){
	const int elementCount = root.GetElementCount();
	int i;
	
	for(i=0; i<elementCount; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if(tagName == "id"){
			actor.SetID(GetCDataString(*tag));
			
		}else if(tagName == "aliasID"){
			actor.SetAliasID(GetCDataString(*tag));
			
		}else if(tagName == "position"){
			decVector position;
			ReadVector(*tag, position);
			actor.SetPosition(position);
			
		}else if(tagName == "orientation"){
			decVector orientation;
			ReadVector(*tag, orientation);
			actor.SetOrientation(orientation);
			
		}else if(tagName == "boneHeadRotator"){
			actor.SetBoneHeadRotator(GetCDataString(*tag));
			
		}else if(tagName == "model"){
			actor.SetPathModel(GetCDataString(*tag));
			
		}else if(tagName == "skin"){
			actor.SetPathSkin(GetCDataString(*tag));
			
		}else if(tagName == "rig"){
			actor.SetPathRig(GetCDataString(*tag));
			
		}else if(tagName == "speechAnimation"){
			actor.SetPathSpeechAnimation(GetCDataString(*tag));
			
		}else if(tagName == "eyesAnimator"){
			actor.SetPathEyesAnimator(GetCDataString(*tag));
			
		}else if(tagName == "facePoseAnimator"){
			actor.SetPathFacePoseAnimator(GetCDataString(*tag));
			
		}else if(tagName == "pose"){
			pReadPose(*tag, actor);
			
		}else if(tagName == "command"){
			cePlaybackCommand::Ref command;
			
			try{
				command = cePlaybackCommand::Ref::New(GetCDataString(*tag),
					GetAttributeBool(*tag, "value"));
				actor.GetCommands().Add(command);
			}catch(const deException &){
				throw;
			}
			
		}else if(tagName == "parameter"){
			actor.GetParameters().SetAt(GetAttributeString(*tag, "name"), GetCDataInt(*tag));
			
		}else if(tagName == "activePose"){
			const decString &name = GetCDataString(*tag);
			actor.SetActivePose(actor.GetPoses().FindOrDefault([&](const ceActorPose &p){
				return p.GetName() == name;
			}));
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
}



// Private Functions
//////////////////////

void ceLoadSaveCTA::pWritePose(decXmlWriter &writer, const ceActorPose &pose){
	writer.WriteOpeningTagStart("pose");
	writer.WriteAttributeString("name", pose.GetName());
	writer.WriteOpeningTagEnd();
	
	writer.WriteDataTagString("animator", pose.GetPathAnimator());
	
	// controllers
	pose.GetControllers().Visit([&writer, this](const ceActorController *c){
		pWriteController(writer, *c);
	});
	
	// gestures
	pose.GetGestures().Visit([&writer](const ceActorGesture &g){
		writer.WriteOpeningTagStart("gesture");
		writer.WriteAttributeString("name", g.GetName());
		writer.WriteOpeningTagEnd(false, false);
		writer.WriteTextString(g.GetPathAnimator());
		writer.WriteClosingTag("gesture", false);
	});
	
	writer.WriteClosingTag("pose");
}

void ceLoadSaveCTA::pWriteController(decXmlWriter &writer, const ceActorController &controller){
	writer.WriteOpeningTagStart("controller");
	writer.WriteAttributeString("name", controller.GetName());
	writer.WriteOpeningTagEnd();
	
	switch(controller.GetUpdateType()){
	case ceActorController::eutConstant:
		writer.WriteDataTagString("update", "constant");
		break;
		
	case ceActorController::eutElapsedTime:
		writer.WriteDataTagString("update", "elapsedTime");
		break;
		
	case ceActorController::eutHeadLeftRight:
		writer.WriteDataTagString("update", "headLeftRight");
		break;
		
	case ceActorController::eutHeadUpDown:
		writer.WriteDataTagString("update", "headUpDown");
		break;
		
	case ceActorController::eutEyesLeftRight:
		writer.WriteDataTagString("update", "eyesLeftRight");
		break;
		
	case ceActorController::eutEyesUpDown:
		writer.WriteDataTagString("update", "eyesUpDown");
		break;
		
	default:
		DETHROW(deeInvalidParam);
	}
	
	writer.WriteDataTagFloat("value", controller.GetValue());
	WriteVector(writer, "vector", controller.GetVector());
	
	writer.WriteClosingTag("controller");
}



void ceLoadSaveCTA::pReadPose(const decXmlElementTag &root, ceConversationActor &actor){
	const decString poseName(GetAttributeString(root, "name"));
	if(actor.GetPoses().HasMatching([&poseName](const ceActorPose &p){ return p.GetName() == poseName; })){
		decString text;
		text.Format("Pose with name '%s' exists already", poseName.GetString());
		LogWarnGenericProblem(root, text);
		return;
	}
	
	const ceActorPose::Ref pose(ceActorPose::Ref::New(actor.GetEnvironment(), poseName));
	const int elementCount = root.GetElementCount();
	int i;
	
	for(i=0; i<elementCount; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if(tagName == "animator"){
			pose->SetPathAnimator(GetCDataString(*tag));
			
		}else if(tagName == "controller"){
			const ceActorController::Ref controller(ceActorController::Ref::New());
			
			if(HasAttribute(*tag, "index")){ // deprecated
				controller->SetName(GetAttributeString(*tag, "index"));
				
			}else{
				controller->SetName(GetAttributeString(*tag, "name"));
			}
			
			pReadController(*tag, controller);
			pose->GetControllers().Add(controller);
			
		}else if(tagName == "gesture"){
			ceActorGesture::Ref gesture;
			
			const decString gestureName(GetAttributeString(*tag, "name"));
			
			if(pose->GetGestures().HasMatching([&gestureName](const ceActorGesture &g){ return g.GetName() == gestureName; })){
				decString text;
				text.Format("Gesture with name '%s' exists already in pose '%s'",
					gestureName.GetString(), poseName.GetString());
				LogWarnGenericProblem(*tag, text);
				continue;
			}
			
			try{
				gesture = ceActorGesture::Ref::New(actor.GetEnvironment(), gestureName);
				gesture->SetPathAnimator(GetCDataString(*tag));
				pose->GetGestures().Add(gesture);
			}catch(const deException &){
				throw;
			}
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
	
	actor.GetPoses().Add(pose);
}

void ceLoadSaveCTA::pReadController(const decXmlElementTag &root, ceActorController &controller){
	const int elementCount = root.GetElementCount();
	int i;
	
	for(i =0; i <elementCount; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if(tagName == "update"){
			const char * const typeName = GetCDataString(*tag);
			
			if(strcmp(typeName, "constant") == 0){
				controller.SetUpdateType(ceActorController::eutConstant);
				
			}else if(strcmp(typeName, "elapsedTime") == 0){
				controller.SetUpdateType(ceActorController::eutElapsedTime);
				
			}else if(strcmp(typeName, "headLeftRight") == 0){
				controller.SetUpdateType(ceActorController::eutHeadLeftRight);
				
			}else if(strcmp(typeName, "headUpDown") == 0){
				controller.SetUpdateType(ceActorController::eutHeadUpDown);
				
			}else if(strcmp(typeName, "eyesLeftRight") == 0){
				controller.SetUpdateType(ceActorController::eutEyesLeftRight);
				
			}else if(strcmp(typeName, "eyesUpDown") == 0){
				controller.SetUpdateType(ceActorController::eutEyesUpDown);
				
			}else{
				LogErrorUnknownValue(*tag, typeName);
			}
			
		}else if(tagName == "value"){
			controller.SetValue(GetCDataFloat(*tag));
			
		}else if(tagName == "vector"){
			decVector vector;
			ReadVector(*tag, vector);
			controller.SetVector(vector);
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
}
