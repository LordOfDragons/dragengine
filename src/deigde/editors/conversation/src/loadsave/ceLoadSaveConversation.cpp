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

#include "ceLoadSaveConversation.h"
#include "../conversation/ceConversation.h"
#include "../conversation/action/ceConversationAction.h"
#include "../conversation/action/ceCACameraShot.h"
#include "../conversation/action/ceCAMusic.h"
#include "../conversation/action/ceCAActorSpeak.h"
#include "../conversation/action/ceCAIfElse.h"
#include "../conversation/action/ceCAPlayerChoice.h"
#include "../conversation/action/ceCAPlayerChoiceOption.h"
#include "../conversation/action/ceCASnippet.h"
#include "../conversation/action/ceCAStopConversation.h"
#include "../conversation/action/ceCAStopTopic.h"
#include "../conversation/action/ceCASetVariable.h"
#include "../conversation/action/ceCASetActorParameter.h"
#include "../conversation/action/ceCAActorCommand.h"
#include "../conversation/action/ceCAGameCommand.h"
#include "../conversation/action/ceCAWait.h"
#include "../conversation/action/ceCATrigger.h"
#include "../conversation/action/ceCAActorAdd.h"
#include "../conversation/action/ceCAActorRemove.h"
#include "../conversation/action/ceCACoordSystemAdd.h"
#include "../conversation/action/ceCACoordSystemRemove.h"
#include "../conversation/action/ceCAComment.h"
#include "../conversation/action/ceCAIfElseCase.h"
#include "../conversation/camerashot/ceCameraShot.h"
#include "../conversation/condition/ceConversationCondition.h"
#include "../conversation/condition/ceCConditionLogic.h"
#include "../conversation/condition/ceCConditionHasActor.h"
#include "../conversation/condition/ceCConditionActorInConversation.h"
#include "../conversation/condition/ceCConditionVariable.h"
#include "../conversation/condition/ceCConditionActorParameter.h"
#include "../conversation/condition/ceCConditionActorCommand.h"
#include "../conversation/condition/ceCConditionGameCommand.h"
#include "../conversation/condition/ceCConditionTrigger.h"
#include "../conversation/facepose/ceFacePose.h"
#include "../conversation/file/ceConversationFile.h"
#include "../conversation/gesture/ceGesture.h"
#include "../conversation/strip/ceStrip.h"
#include "../conversation/target/ceTarget.h"
#include "../conversation/topic/ceConversationTopic.h"
#include "../gui/ceWindowMain.h"
#include "../utils/ceControllerValue.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/curve/decCurveBezierPoint.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlParser.h>



// Class ceLoadSaveConversation
////////////////////////////////

// Constructor, destructor
////////////////////////////

ceLoadSaveConversation::ceLoadSaveConversation(ceLoadSaveSystem *lssys, deLogger *logger, const char *loggerSource) :
igdeBaseXML(logger, loggerSource){
	pLSSys = lssys;
	
	pName = "Conversation";
	pPattern = ".deconvo";
}



// Loading and saving
///////////////////////

void ceLoadSaveConversation::LoadConversation(ceConversation &conversation,
decBaseFileReader &reader, const char *filename){
	decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::New());
	
	decXmlParser(GetLogger()).ParseXml(&reader, xmlDoc);
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if(!root || strcmp(root->GetName(), "conversation") != 0){
		DETHROW(deeInvalidParam);
	}
	
	conversation.SetFilePath(filename);
	pReadConversation(*root, conversation);
	conversation.SetSaved(true);
	conversation.SetChanged(false);
}

void ceLoadSaveConversation::SaveConversation(const ceConversation &conversation, decBaseFileWriter &writer){
	decXmlWriter xmlWriter(&writer);
	
	xmlWriter.WriteXMLDeclaration();
	
	pWriteConversation(xmlWriter, conversation);
}



// Private Functions
//////////////////////

void ceLoadSaveConversation::pWriteConversation(decXmlWriter &writer, const ceConversation &conversation){
	int i, count;
	
	writer.WriteOpeningTag("conversation", false, true);
	
	const decStringList &importConversationPath = conversation.GetImportConversationPath();
	count = importConversationPath.GetCount();
	if(count > 0){
		writer.WriteNewline();
		for(i=0; i<count; i++){
			writer.WriteDataTagString("import", importConversationPath.GetAt(i));
		}
	}
	
	if(conversation.GetTargets().IsNotEmpty()){
		writer.WriteNewline();
		conversation.GetTargets().Visit([&](const ceTarget &t){ pWriteTarget(writer, t); });
	}
	
	if(conversation.GetCameraShotList().IsNotEmpty()){
		writer.WriteNewline();
		conversation.GetCameraShotList().Visit([&](const ceCameraShot &c){ pWriteCameraShot(writer, c); });
	}
	
	if(conversation.GetGestures().IsNotEmpty()){
		writer.WriteNewline();
		conversation.GetGestures().Visit([&](const ceGesture &g){ pWriteGesture(writer, g); });
	}
	
	const decStringList &facePoseControllerNameList = conversation.GetFacePoseControllerNameList();
	count = facePoseControllerNameList.GetCount();
	if(count > 0){
		writer.WriteNewline();
		writer.WriteOpeningTag("facePoseControllerNames");
		for(i=0; i<count; i++){
			writer.WriteDataTagString("name", facePoseControllerNameList.GetAt(i));
		}
		writer.WriteClosingTag("facePoseControllerNames");
	}
	
	if(conversation.GetFacePoseList().IsNotEmpty()){
		writer.WriteNewline();
		conversation.GetFacePoseList().Visit([&](const ceFacePose &f){ pWriteFacePose(writer, f); });
	}
	
	if(conversation.GetFiles().IsNotEmpty()){
		writer.WriteNewline();
		conversation.GetFiles().Visit([&](const ceConversationFile &f){ pWriteFile(writer, f); });
	}
	
	writer.WriteClosingTag("conversation", true);
}

void ceLoadSaveConversation::pWriteTarget(decXmlWriter &writer, const ceTarget &target){
	const decVector &orientation = target.GetOrientation();
	const decVector &position = target.GetPosition();
	
	writer.WriteOpeningTagStart("target");
	writer.WriteAttributeString("name", target.GetName());
	writer.WriteOpeningTagEnd();
	
	if(!target.GetActor().IsEmpty()){
		writer.WriteDataTagString("actor", target.GetActor());
	}
	if(!target.GetCoordSystem().IsEmpty()){
		writer.WriteDataTagString("coordSystem", target.GetCoordSystem());
	}
	
	if(!target.GetBone().IsEmpty()){
		writer.WriteDataTagString("bone", target.GetBone());
	}
	
	writer.WriteOpeningTagStart("position");
	writer.WriteAttributeFloat("x", position.x);
	writer.WriteAttributeFloat("y", position.y);
	writer.WriteAttributeFloat("z", position.z);
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteOpeningTagStart("orientation");
	writer.WriteAttributeFloat("x", orientation.x);
	writer.WriteAttributeFloat("y", orientation.y);
	writer.WriteAttributeFloat("z", orientation.z);
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteClosingTag("target");
}

void ceLoadSaveConversation::pWriteCameraShot(decXmlWriter &writer, const ceCameraShot &cameraShot){
	writer.WriteOpeningTagStart("cameraShot");
	writer.WriteAttributeString("name", cameraShot.GetName());
	writer.WriteOpeningTagEnd();
	
	writer.WriteDataTagInt("actorCount", cameraShot.GetActorCount());
	
	// camera target
	writer.WriteDataTagString("cameraTarget", cameraShot.GetCameraTarget());
	
	const decVector &offsetCameraFrom = cameraShot.GetOffsetCameraFrom();
	writer.WriteOpeningTagStart("offsetCameraFrom");
	writer.WriteAttributeFloat("x", offsetCameraFrom.x);
	writer.WriteAttributeFloat("y", offsetCameraFrom.y);
	writer.WriteAttributeFloat("z", offsetCameraFrom.z);
	writer.WriteOpeningTagEnd(true);
	
	const decVector &offsetCameraTo = cameraShot.GetOffsetCameraTo();
	writer.WriteOpeningTagStart("offsetCameraTo");
	writer.WriteAttributeFloat("x", offsetCameraTo.x);
	writer.WriteAttributeFloat("y", offsetCameraTo.y);
	writer.WriteAttributeFloat("z", offsetCameraTo.z);
	writer.WriteOpeningTagEnd(true);
	
	const decVector &cameraOrbitFrom = cameraShot.GetCameraOrbitFrom();
	writer.WriteOpeningTagStart("cameraOrbitFrom");
	writer.WriteAttributeFloat("x", cameraOrbitFrom.x);
	writer.WriteAttributeFloat("y", cameraOrbitFrom.y);
	writer.WriteAttributeFloat("z", cameraOrbitFrom.z);
	writer.WriteOpeningTagEnd(true);
	
	const decVector &cameraOrbitTo = cameraShot.GetCameraOrbitTo();
	writer.WriteOpeningTagStart("cameraOrbitTo");
	writer.WriteAttributeFloat("x", cameraOrbitTo.x);
	writer.WriteAttributeFloat("y", cameraOrbitTo.y);
	writer.WriteAttributeFloat("z", cameraOrbitTo.z);
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteDataTagFloat("cameraDistanceFrom", cameraShot.GetCameraDistanceFrom());
	writer.WriteDataTagFloat("cameraDistanceTo", cameraShot.GetCameraDistanceTo());
	
	// look-at target
	writer.WriteDataTagString("lookAtTarget", cameraShot.GetLookAtTarget());
	
	const decVector &offsetLookAtFrom = cameraShot.GetOffsetLookAtFrom();
	writer.WriteOpeningTagStart("offsetLookAtFrom");
	writer.WriteAttributeFloat("x", offsetLookAtFrom.x);
	writer.WriteAttributeFloat("y", offsetLookAtFrom.y);
	writer.WriteAttributeFloat("z", offsetLookAtFrom.z);
	writer.WriteOpeningTagEnd(true);
	
	const decVector &offsetLookAtTo = cameraShot.GetOffsetLookAtTo();
	writer.WriteOpeningTagStart("offsetLookAtTo");
	writer.WriteAttributeFloat("x", offsetLookAtTo.x);
	writer.WriteAttributeFloat("y", offsetLookAtTo.y);
	writer.WriteAttributeFloat("z", offsetLookAtTo.z);
	writer.WriteOpeningTagEnd(true);
	
	// local parameters
	const decVector &posFrom = cameraShot.GetPositionFrom();
	writer.WriteOpeningTagStart("positionFrom");
	writer.WriteAttributeFloat("x", posFrom.x);
	writer.WriteAttributeFloat("y", posFrom.y);
	writer.WriteAttributeFloat("z", posFrom.z);
	writer.WriteOpeningTagEnd(true);
	
	const decVector &posTo = cameraShot.GetPositionTo();
	writer.WriteOpeningTagStart("positionTo");
	writer.WriteAttributeFloat("x", posTo.x);
	writer.WriteAttributeFloat("y", posTo.y);
	writer.WriteAttributeFloat("z", posTo.z);
	writer.WriteOpeningTagEnd(true);
	
	const decVector &rotFrom = cameraShot.GetRotationFrom();
	writer.WriteOpeningTagStart("rotationFrom");
	writer.WriteAttributeFloat("x", rotFrom.x);
	writer.WriteAttributeFloat("y", rotFrom.y);
	writer.WriteAttributeFloat("z", rotFrom.z);
	writer.WriteOpeningTagEnd(true);
	
	const decVector &rotTo = cameraShot.GetRotationTo();
	writer.WriteOpeningTagStart("rotationTo");
	writer.WriteAttributeFloat("x", rotTo.x);
	writer.WriteAttributeFloat("y", rotTo.y);
	writer.WriteAttributeFloat("z", rotTo.z);
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteDataTagFloat("tiltFrom", cameraShot.GetTiltFrom());
	writer.WriteDataTagFloat("tiltTo", cameraShot.GetTiltTo());
	writer.WriteDataTagFloat("fovFrom", cameraShot.GetFovFrom());
	writer.WriteDataTagFloat("fovTo", cameraShot.GetFovTo());
	
	writer.WriteDataTagBool("lockUpAxis", cameraShot.GetLockUpAxis());
	writer.WriteDataTagBool("alignTargets", cameraShot.GetAlignTargets());
	writer.WriteDataTagBool("relativeToLookAt", cameraShot.GetRelativeToLookAt());
	writer.WriteDataTagBool("lockCameraTarget", cameraShot.GetLockCameraTarget());
	writer.WriteDataTagBool("lockLookAtTarget", cameraShot.GetLockLookAtTarget());
	
	/*
	for(i=0; i<ceCameraShot::EP_COUNT; i++){
		writer.WriteOpeningTagStart("curve");
		writer.WriteAttributeString("id", vParameterNames[i]);
		writer.WriteOpeningTagEnd();
		
		pWriteCurveBezier(writer, cameraShot.GetParameterCurve(i));
		
		writer.WriteClosingTag("curve", true);
	}
	*/
	
	writer.WriteClosingTag("cameraShot");
}

void ceLoadSaveConversation::pWriteGesture(decXmlWriter &writer, const ceGesture &gesture){
	writer.WriteOpeningTagStart("gesture");
	writer.WriteAttributeString("name", gesture.GetName());
	writer.WriteOpeningTagEnd();
	
	writer.WriteDataTagString("animator", gesture.GetAnimator());
	if(gesture.GetHold()){
		writer.WriteDataTagBool("hold", gesture.GetHold());
	}
	writer.WriteDataTagFloat("duration", gesture.GetDuration());
	
	writer.WriteClosingTag("gesture");
}

void ceLoadSaveConversation::pWriteFacePose(decXmlWriter &writer, const ceFacePose &facePose){
	writer.WriteOpeningTagStart("facePose");
	writer.WriteAttributeString("name", facePose.GetName());
	writer.WriteOpeningTagEnd();
	
	facePose.GetControllers().Visit([&](const ceControllerValue &c){
		writer.WriteOpeningTagStart("controller");
		
		if(c.GetControllerIndex() == -1){
			writer.WriteAttributeString("name", c.GetController());
			
		}else{ // deprecated
			writer.WriteAttributeInt("index", c.GetControllerIndex());
		}
		
		writer.WriteAttributeFloat("value", c.GetValue());
		writer.WriteOpeningTagEnd(true);
	});
	
	writer.WriteClosingTag("facePose");
}

void ceLoadSaveConversation::pWriteFile(decXmlWriter &writer, const ceConversationFile &file){
	writer.WriteOpeningTagStart("group");
	writer.WriteAttributeString("id", file.GetID());
	writer.WriteOpeningTagEnd();
	
	file.GetTopics().Visit([&](const ceConversationTopic &t){ pWriteTopic(writer, t); });
	
	writer.WriteClosingTag("group");
}

void ceLoadSaveConversation::pWriteTopic(decXmlWriter &writer, const ceConversationTopic &topic){
	writer.WriteOpeningTagStart("topic");
	writer.WriteAttributeString("id", topic.GetID());
	writer.WriteOpeningTagEnd();
	
	writer.WriteOpeningTag("actions");
	pWriteActionList(writer, topic.GetActions());
	writer.WriteClosingTag("actions");
	
	writer.WriteClosingTag("topic");
}

void ceLoadSaveConversation::pWriteActionList(decXmlWriter &writer, const ceConversationAction::List &list){
	const int count = list.GetCount();
	ceConversationAction::Ref action;
	int i;
	
	for(i=0; i<count; i++){
		action = list.GetAt(i);
		
		if(action->GetType() == ceConversationAction::eatCameraShot){
			pWriteActionCameraShot(writer, (ceCACameraShot&)*action);
			
		}else if(action->GetType() == ceConversationAction::eatMusic){
			pWriteActionMusic(writer, (ceCAMusic&)*action);
			
		}else if(action->GetType() == ceConversationAction::eatActorSpeak){
			pWriteActionActorSpeak(writer, (ceCAActorSpeak&)*action);
			
		}else if(action->GetType() == ceConversationAction::eatIfElse){
			pWriteActionIfElse(writer, (ceCAIfElse&)*action);
			
		}else if(action->GetType() == ceConversationAction::eatPlayerChoice){
			pWriteActionPlayerChoice(writer, (ceCAPlayerChoice&)*action);
			
		}else if(action->GetType() == ceConversationAction::eatStopConversation){
			pWriteActionStopConversation(writer, (ceCAStopConversation&)*action);
			
		}else if(action->GetType() == ceConversationAction::eatStopTopic){
			pWriteActionStopTopic(writer, (ceCAStopTopic&)*action);
			
		}else if(action->GetType() == ceConversationAction::eatSnippet){
			pWriteActionSnippet(writer, (ceCASnippet&)*action);
			
		}else if(action->GetType() == ceConversationAction::eatSetVariable){
			pWriteActionSetVariable(writer, (ceCASetVariable&)*action);
			
		}else if(action->GetType() == ceConversationAction::eatSetActorParameter){
			pWriteActionSetActorParameter(writer, (ceCASetActorParameter&)*action);
			
		}else if(action->GetType() == ceConversationAction::eatActorCommand){
			pWriteActionActorCommand(writer, (ceCAActorCommand&)*action);
			
		}else if(action->GetType() == ceConversationAction::eatGameCommand){
			pWriteActionGameCommand(writer, (ceCAGameCommand&)*action);
			
		}else if(action->GetType() == ceConversationAction::eatWait){
			pWriteActionWait(writer, (ceCAWait&)*action);
			
		}else if(action->GetType() == ceConversationAction::eatTrigger){
			pWriteActionTrigger(writer, (ceCATrigger&)*action);
			
		}else if(action->GetType() == ceConversationAction::eatActorAdd){
			pWriteActionActorAdd(writer, (ceCAActorAdd&)*action);
			
		}else if(action->GetType() == ceConversationAction::eatActorRemove){
			pWriteActionActorRemove(writer, (ceCAActorRemove&)*action);
			
		}else if(action->GetType() == ceConversationAction::eatCoordSystemAdd){
			pWriteActionCoordSystemAdd(writer, (ceCACoordSystemAdd&)*action);
			
		}else if(action->GetType() == ceConversationAction::eatCoordSystemRemove){
			pWriteActionCoordSystemRemove(writer, (ceCACoordSystemRemove&)*action);
			
		}else if(action->GetType() == ceConversationAction::eatComment){
			pWriteActionComment(writer, (ceCAComment&)*action);
		}
	}
}

void ceLoadSaveConversation::pWriteActionCommon(decXmlWriter &writer, const ceConversationAction &action){
	if(!action.GetWaitForActor()){
		writer.WriteDataTagBool("waitForActor", action.GetWaitForActor());
	}
	if(action.GetWaitSpeakOnly()){
		writer.WriteDataTagBool("waitSpeakOnly", action.GetWaitSpeakOnly());
	}
	if(!action.GetWaitForActorID().IsEmpty()){
		writer.WriteDataTagString("waitForActorID", action.GetWaitForActorID());
	}
	if(fabsf(action.GetDelay()) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("delay", action.GetDelay());
	}
}

void ceLoadSaveConversation::pWriteActionCameraShot(decXmlWriter &writer, const ceCACameraShot &action){
	writer.WriteOpeningTag("cameraShot");
	
	pWriteActionCommon(writer, action);
	writer.WriteDataTagString("name", action.GetName());
	writer.WriteDataTagFloat("duration", action.GetDuration());
	
	if(!action.GetCameraTarget().IsEmpty()){
		writer.WriteDataTagString("cameraTarget", action.GetCameraTarget());
	}
	if(!action.GetLookAtTarget().IsEmpty()){
		writer.WriteDataTagString("lookAtTarget", action.GetLookAtTarget());
	}
	
	writer.WriteClosingTag("cameraShot");
}

void ceLoadSaveConversation::pWriteActionMusic(decXmlWriter &writer, const ceCAMusic &action){
	writer.WriteOpeningTag("music");
	
	pWriteActionCommon(writer, action);
	writer.WriteDataTagString("name", action.GetName());
	
	writer.WriteClosingTag("music");
}

void ceLoadSaveConversation::pWriteActionActorSpeak(decXmlWriter &writer, const ceCAActorSpeak &action){
	writer.WriteOpeningTag("actorSpeak");
	
	pWriteActionCommon(writer, action);
	
	writer.WriteDataTagString("actor", action.GetActor());
	
	if(!action.GetTextBoxText().IsEmpty()){
		WriteMultilineString(writer, "textBoxText", action.GetTextBoxText().ToUTF8());
	}
	if(!action.GetTextBoxTextTranslate().IsEmpty()){
		writer.WriteDataTagString("textBoxTextTranslate", action.GetTextBoxTextTranslate());
	}
	if(!action.GetTextBoxTextStyle().IsEmpty()){
		writer.WriteDataTagString("textBoxTextStyle", action.GetTextBoxTextStyle());
	}
	if(!action.GetPathSound().IsEmpty()){
		writer.WriteDataTagString("pathSound", action.GetPathSound());
	}
	if(action.GetWords().GetCount() > 0){
		writer.WriteOpeningTag("words");
		pWriteStripList(writer, action.GetWords());
		writer.WriteClosingTag("words");
	}
	if(action.GetFacePoses().GetCount() > 0){
		writer.WriteOpeningTag("facePoses");
		pWriteStripList(writer, action.GetFacePoses());
		writer.WriteClosingTag("facePoses");
	}
	if(action.GetGestures().GetCount() > 0){
		writer.WriteOpeningTag("gestures");
		pWriteStripList(writer, action.GetGestures());
		writer.WriteClosingTag("gestures");
	}
	if(action.GetBodyLookAts().GetCount() > 0){
		writer.WriteOpeningTag("bodyLookAts");
		pWriteStripList(writer, action.GetBodyLookAts());
		writer.WriteClosingTag("bodyLookAts");
	}
	if(action.GetHeadLookAts().GetCount() > 0){
		writer.WriteOpeningTag("headLookAts");
		pWriteStripList(writer, action.GetHeadLookAts());
		writer.WriteClosingTag("headLookAts");
	}
	if(action.GetEyesLookAtList().GetCount() > 0){
		writer.WriteOpeningTag("eyesLookAts");
		pWriteStripList(writer, action.GetEyesLookAtList());
		writer.WriteClosingTag("eyesLookAts");
	}
	if(!action.GetMovement().IsEmpty()){
		writer.WriteDataTagString("movement", action.GetMovement());
	}
	if(action.GetMinSpeechTime() > 0.0f){
		writer.WriteDataTagFloat("minSpeechTime", action.GetMinSpeechTime());
	}
	if(!action.GetUseSpeechAnimation()){
		writer.WriteDataTagBool("useSpeechAnimation", false);
	}
	
	writer.WriteClosingTag("actorSpeak");
}

void ceLoadSaveConversation::pWriteActionIfElse(decXmlWriter &writer, const ceCAIfElse &action){
	writer.WriteOpeningTag("ifElse");
	
	pWriteActionCommon(writer, action);
	
	action.GetCases().Visit([&](const ceCAIfElseCase &c){ pWriteActionIfElseCase(writer, c); });
	
	if(action.GetElseActions().GetCount() > 0){
		writer.WriteOpeningTag("else");
		pWriteActionList(writer, action.GetElseActions());
		writer.WriteClosingTag("else");
	}
	
	writer.WriteClosingTag("ifElse");
}

void ceLoadSaveConversation::pWriteActionIfElseCase(decXmlWriter &writer, const ceCAIfElseCase &ifcase){
	writer.WriteOpeningTag("case");
	
	if(ifcase.GetCondition()){
		writer.WriteOpeningTag("condition");
		pWriteCondition(writer, ifcase.GetCondition());
		writer.WriteClosingTag("condition");
	}
	
	if(ifcase.GetActions().GetCount() > 0){
		writer.WriteOpeningTag("actions");
		pWriteActionList(writer, ifcase.GetActions());
		writer.WriteClosingTag("actions");
	}
	
	writer.WriteClosingTag("case");
}

void ceLoadSaveConversation::pWriteActionPlayerChoice(decXmlWriter &writer, const ceCAPlayerChoice &action){
	writer.WriteOpeningTag("playerChoice");
	
	pWriteActionCommon(writer, action);
	
	if(!action.GetVariableName().IsEmpty()){
		writer.WriteDataTagString("variable", action.GetVariableName());
	}
	
	if(action.GetActions().GetCount() > 0){
		writer.WriteOpeningTag("actions");
		pWriteActionList(writer, action.GetActions());
		writer.WriteClosingTag("actions");
	}
	
	action.GetOptions().Visit([&](const ceCAPlayerChoiceOption &o){
		pWriteActionPlayerChoiceOption(writer, o);
	});
	
	writer.WriteClosingTag("playerChoice");
}

void ceLoadSaveConversation::pWriteActionPlayerChoiceOption(decXmlWriter &writer, const ceCAPlayerChoiceOption &option){
	writer.WriteOpeningTag("option");
	
	writer.WriteDataTagString("text", option.GetText().ToUTF8());
	
	if(option.GetCondition()){
		writer.WriteOpeningTag("condition");
		pWriteCondition(writer, option.GetCondition());
		writer.WriteClosingTag("condition");
	}
	
	if(option.GetActions().GetCount() > 0){
		writer.WriteOpeningTag("actions");
		pWriteActionList(writer, option.GetActions());
		writer.WriteClosingTag("actions");
	}
	
	writer.WriteClosingTag("option");
}

void ceLoadSaveConversation::pWriteActionStopConversation(decXmlWriter &writer, const ceCAStopConversation &action){
	writer.WriteOpeningTag("stopConversation");
	pWriteActionCommon(writer, action);
	writer.WriteClosingTag("stopConversation");
}

void ceLoadSaveConversation::pWriteActionStopTopic(decXmlWriter &writer, const ceCAStopTopic &action){
	writer.WriteOpeningTag("stopTopic");
	pWriteActionCommon(writer, action);
	writer.WriteClosingTag("stopTopic");
}

void ceLoadSaveConversation::pWriteActionSnippet(decXmlWriter &writer, const ceCASnippet &action){
	writer.WriteOpeningTag("snippet");
	pWriteActionCommon(writer, action);
	writer.WriteDataTagString("group", action.GetFile());
	writer.WriteDataTagString("topic", action.GetTopic());
	if(action.GetCreateSideLane()){
		writer.WriteDataTagBool("createSideLane", action.GetCreateSideLane());
	}
	writer.WriteClosingTag("snippet");
}

void ceLoadSaveConversation::pWriteActionSetVariable(decXmlWriter &writer, const ceCASetVariable &action){
	writer.WriteOpeningTag("setVariable");
	pWriteActionCommon(writer, action);
	
	writer.WriteDataTagString("name", action.GetName());
	
	switch(action.GetOperator()){
	case ceCASetVariable::eopSet:
		writer.WriteDataTagString("operator", "set");
		break;
		
	case ceCASetVariable::eopIncrement:
		writer.WriteDataTagString("operator", "increment");
		break;
		
	case ceCASetVariable::eopDecrement:
		writer.WriteDataTagString("operator", "decrement");
		break;
		
	case ceCASetVariable::eopRandom:
		writer.WriteDataTagString("operator", "random");
		break;
		
	default:
		DETHROW(deeInvalidParam);
	}
	
	writer.WriteDataTagInt("value", action.GetValue());
	if(!action.GetValueVariable().IsEmpty()){
		writer.WriteDataTagString("valueVariable", action.GetValueVariable());
	}
	
	writer.WriteClosingTag("setVariable");
}

void ceLoadSaveConversation::pWriteActionSetActorParameter(decXmlWriter &writer, const ceCASetActorParameter &action){
	writer.WriteOpeningTag("setActorParameter");
	pWriteActionCommon(writer, action);
	
	writer.WriteDataTagString("actor", action.GetActor());
	
	writer.WriteDataTagString("name", action.GetName());
	
	if(action.GetOperator() == ceCASetActorParameter::eopSet){
		writer.WriteDataTagString("operator", "set");
		
	}else if(action.GetOperator() == ceCASetActorParameter::eopIncrement){
		writer.WriteDataTagString("operator", "increment");
		
	}else if(action.GetOperator() == ceCASetActorParameter::eopDecrement){
		writer.WriteDataTagString("operator", "decrement");
		
	}else if(action.GetOperator() == ceCASetActorParameter::eopRandom){
		writer.WriteDataTagString("operator", "random");
	}
	
	writer.WriteDataTagInt("value", action.GetValue());
	if(!action.GetValueVariable().IsEmpty()){
		writer.WriteDataTagString("valueVariable", action.GetValueVariable());
	}
	
	writer.WriteClosingTag("setActorParameter");
}

void ceLoadSaveConversation::pWriteActionActorCommand(decXmlWriter &writer, const ceCAActorCommand &action){
	writer.WriteOpeningTag("actorCommand");
	pWriteActionCommon(writer, action);
	
	writer.WriteDataTagString("actor", action.GetActor());
	
	writer.WriteDataTagString("command", action.GetCommand());
	
	writer.WriteClosingTag("actorCommand");
}

void ceLoadSaveConversation::pWriteActionGameCommand(decXmlWriter &writer, const ceCAGameCommand &action){
	writer.WriteOpeningTag("gameCommand");
	pWriteActionCommon(writer, action);
	
	writer.WriteDataTagString("command", action.GetCommand());
	
	writer.WriteClosingTag("gameCommand");
}

void ceLoadSaveConversation::pWriteActionWait(decXmlWriter &writer, const ceCAWait &action){
	writer.WriteOpeningTag("wait");
	
	pWriteActionCommon(writer, action);
	
	if(action.GetCondition()){
		writer.WriteOpeningTag("condition");
		pWriteCondition(writer, action.GetCondition());
		writer.WriteClosingTag("condition");
	}
	
	writer.WriteDataTagFloat("interval", action.GetInterval());
	
	if(action.GetActions().GetCount() > 0){
		writer.WriteOpeningTag("actions");
		pWriteActionList(writer, action.GetActions());
		writer.WriteClosingTag("actions");
	}
	
	writer.WriteClosingTag("wait");
}

void ceLoadSaveConversation::pWriteActionTrigger(decXmlWriter &writer, const ceCATrigger &action){
	writer.WriteOpeningTag("trigger");
	pWriteActionCommon(writer, action);
	
	writer.WriteDataTagString("name", action.GetName());
	
	if(action.GetAction() == ceCATrigger::eaFire){
		writer.WriteDataTagString("action", "fire");
		
	}else if(action.GetAction() == ceCATrigger::eaReset){
		writer.WriteDataTagString("action", "reset");
		
	}else if(action.GetAction() == ceCATrigger::eaPulse){
		writer.WriteDataTagString("action", "pulse");
		
	}else if(action.GetAction() == ceCATrigger::eaFullReset){
		writer.WriteDataTagString("action", "fullReset");
	}
	
	writer.WriteClosingTag("trigger");
}

void ceLoadSaveConversation::pWriteActionActorAdd(decXmlWriter &writer, const ceCAActorAdd &action){
	writer.WriteOpeningTag("actorAdd");
	pWriteActionCommon(writer, action);
	
	writer.WriteDataTagString("actor", action.GetID());
	if(!action.GetAliasID().IsEmpty()){
		writer.WriteDataTagString("aliasID", action.GetAliasID());
	}
	
	writer.WriteClosingTag("actorAdd");
}

void ceLoadSaveConversation::pWriteActionActorRemove(decXmlWriter &writer, const ceCAActorRemove &action){
	writer.WriteOpeningTag("actorRemove");
	pWriteActionCommon(writer, action);
	
	writer.WriteDataTagString("actor", action.GetActor());
	
	writer.WriteClosingTag("actorRemove");
}

void ceLoadSaveConversation::pWriteActionCoordSystemAdd(decXmlWriter &writer, const ceCACoordSystemAdd &action){
	writer.WriteOpeningTag("coordSystemAdd");
	pWriteActionCommon(writer, action);
	
	writer.WriteDataTagString("coordSystem", action.GetCoordSystemID());
	if(!action.GetAliasID().IsEmpty()){
		writer.WriteDataTagString("aliasID", action.GetAliasID());
	}
	
	writer.WriteClosingTag("coordSystemAdd");
}

void ceLoadSaveConversation::pWriteActionCoordSystemRemove(decXmlWriter &writer, const ceCACoordSystemRemove &action){
	writer.WriteOpeningTag("coordSystemRemove");
	pWriteActionCommon(writer, action);
	
	writer.WriteDataTagString("coordSystem", action.GetCoordSystemID());
	
	writer.WriteClosingTag("coordSystemRemove");
}

void ceLoadSaveConversation::pWriteActionComment(decXmlWriter &writer, const ceCAComment &action){
	writer.WriteOpeningTag("comment");
	
	writer.WriteDataTagString("comment", action.GetComment().GetReplacedString("\n", "\\n"));
	
	writer.WriteClosingTag("comment");
}

void ceLoadSaveConversation::pWriteCondition(decXmlWriter &writer,
ceConversationCondition *condition){
	switch(condition->GetType()){
	case ceConversationCondition::ectLogic:
		pWriteConditionLogic(writer, *((ceCConditionLogic*)condition));
		break;
		
	case ceConversationCondition::ectHasActor:
		pWriteConditionHasActor(writer, *((ceCConditionHasActor*)condition));
		break;
		
	case ceConversationCondition::ectActorInConversation:
		pWriteConditionActorInConversation(writer, *((ceCConditionActorInConversation*)condition));
		break;
		
	case ceConversationCondition::ectVariable:
		pWriteConditionVariable(writer, *((ceCConditionVariable*)condition));
		break;
		
	case ceConversationCondition::ectActorParameter:
		pWriteConditionActorParameter(writer, *((ceCConditionActorParameter*)condition));
		break;
		
	case ceConversationCondition::ectActorCommand:
		pWriteConditionActorCommand(writer, *((ceCConditionActorCommand*)condition));
		break;
		
	case ceConversationCondition::ectGameCommand:
		pWriteConditionGameCommand(writer, *((ceCConditionGameCommand*)condition));
		break;
		
	case ceConversationCondition::ectTrigger:
		pWriteConditionTrigger(writer, *((ceCConditionTrigger*)condition));
		break;
	}
}

void ceLoadSaveConversation::pWriteConditionList(decXmlWriter &writer,
const ceConversationCondition::List &list){
	list.Visit([&](ceConversationCondition *c){
		pWriteCondition(writer, c);
	});
}

void ceLoadSaveConversation::pWriteConditionLogic(decXmlWriter &writer,
const ceCConditionLogic &condition){
	writer.WriteOpeningTag("logic");
	
	switch(condition.GetOperator()){
	case ceCConditionLogic::eopNone:
		writer.WriteDataTagString("operator", "none");
		break;
		
	case ceCConditionLogic::eopAny:
		writer.WriteDataTagString("operator", "any");
		break;
		
	case ceCConditionLogic::eopAll:
		writer.WriteDataTagString("operator", "all");
		break;
	}
	
	if(condition.GetConditions().GetCount() > 0){
		writer.WriteOpeningTag("conditions");
		pWriteConditionList(writer, condition.GetConditions());
		writer.WriteClosingTag("conditions");
	}
	
	writer.WriteClosingTag("logic");
}

void ceLoadSaveConversation::pWriteConditionHasActor(decXmlWriter &writer,
const ceCConditionHasActor &condition){
	writer.WriteOpeningTag("hasActor");
	
	writer.WriteDataTagString("actor", condition.GetActor());
	if(condition.GetNegate()){
		writer.WriteDataTagBool("negate", condition.GetNegate());
	}
	
	writer.WriteClosingTag("hasActor");
}

void ceLoadSaveConversation::pWriteConditionActorInConversation(decXmlWriter &writer,
const ceCConditionActorInConversation &condition){
	writer.WriteOpeningTag("actorInConversation");
	
	writer.WriteDataTagString("actor", condition.GetActor());
	if(condition.GetNegate()){
		writer.WriteDataTagBool("negate", condition.GetNegate());
	}
	
	writer.WriteClosingTag("actorInConversation");
}

void ceLoadSaveConversation::pWriteConditionVariable(decXmlWriter &writer,
const ceCConditionVariable &condition){
	writer.WriteOpeningTag("variable");
	
	switch(condition.GetOperator()){
	case ceCConditionVariable::eopEqual:
		writer.WriteDataTagString("operator", "equal");
		break;
		
	case ceCConditionVariable::eopNotEqual:
		writer.WriteDataTagString("operator", "notEqual");
		break;
		
	case ceCConditionVariable::eopLess:
		writer.WriteDataTagString("operator", "less");
		break;
		
	case ceCConditionVariable::eopLessEqual:
		writer.WriteDataTagString("operator", "lessEqual");
		break;
		
	case ceCConditionVariable::eopGreater:
		writer.WriteDataTagString("operator", "greater");
		break;
		
	case ceCConditionVariable::eopGreaterEqual:
		writer.WriteDataTagString("operator", "greaterEqual");
		break;
	}
	
	writer.WriteDataTagString("variable", condition.GetVariable());
	writer.WriteDataTagInt("testValue", condition.GetTestValue());
	if(!condition.GetTestVariable().IsEmpty()){
		writer.WriteDataTagString("testVariable", condition.GetTestVariable());
	}
	
	writer.WriteClosingTag("variable");
}

void ceLoadSaveConversation::pWriteConditionActorParameter(decXmlWriter &writer,
const ceCConditionActorParameter &condition){
	writer.WriteOpeningTag("actorParameter");
	
	writer.WriteDataTagString("actor", condition.GetActor());
	
	switch(condition.GetOperator()){
	case ceCConditionActorParameter::eopEqual:
		writer.WriteDataTagString("operator", "equal");
		break;
		
	case ceCConditionActorParameter::eopNotEqual:
		writer.WriteDataTagString("operator", "notEqual");
		break;
		
	case ceCConditionActorParameter::eopLess:
		writer.WriteDataTagString("operator", "less");
		break;
		
	case ceCConditionActorParameter::eopLessEqual:
		writer.WriteDataTagString("operator", "lessEqual");
		break;
		
	case ceCConditionActorParameter::eopGreater:
		writer.WriteDataTagString("operator", "greater");
		break;
		
	case ceCConditionActorParameter::eopGreaterEqual:
		writer.WriteDataTagString("operator", "greaterEqual");
		break;
	}
	
	writer.WriteDataTagString("parameter", condition.GetParameter());
	writer.WriteDataTagInt("testValue", condition.GetTestValue());
	if(!condition.GetTestVariable().IsEmpty()){
		writer.WriteDataTagString("testVariable", condition.GetTestVariable());
	}
	
	writer.WriteClosingTag("actorParameter");
}

void ceLoadSaveConversation::pWriteConditionActorCommand(decXmlWriter &writer,
const ceCConditionActorCommand &condition){
	writer.WriteOpeningTag("actorCommand");
	
	writer.WriteDataTagString("actor", condition.GetActor());
	
	writer.WriteDataTagString("command", condition.GetCommand());
	if(condition.GetNegate()){
		writer.WriteDataTagBool("negate", condition.GetNegate());
	}
	
	writer.WriteClosingTag("actorCommand");
}

void ceLoadSaveConversation::pWriteConditionGameCommand(decXmlWriter &writer,
const ceCConditionGameCommand &condition){
	writer.WriteOpeningTag("gameCommand");
	
	writer.WriteDataTagString("command", condition.GetCommand());
	if(condition.GetNegate()){
		writer.WriteDataTagBool("negate", condition.GetNegate());
	}
	
	writer.WriteClosingTag("gameCommand");
}

void ceLoadSaveConversation::pWriteConditionTrigger(decXmlWriter &writer,
const ceCConditionTrigger &condition){
	writer.WriteOpeningTag("trigger");
	
	switch(condition.GetTestMode()){
	case ceCConditionTrigger::etmFired:
		writer.WriteDataTagString("testMode", "fired");
		break;
		
	case ceCConditionTrigger::etmNotFired:
		writer.WriteDataTagString("testMode", "notFired");
		break;
		
	case ceCConditionTrigger::etmEverFired:
		writer.WriteDataTagString("testMode", "everFired");
		break;
		
	case ceCConditionTrigger::etmNeverFired:
		writer.WriteDataTagString("testMode", "neverFired");
		break;
	}
	
	writer.WriteDataTagString("trigger", condition.GetTrigger());
	
	writer.WriteClosingTag("trigger");
}

void ceLoadSaveConversation::pWriteStripList(decXmlWriter &writer, const ceStrip::List &list){
	list.Visit([&](const ceStrip &s){
		writer.WriteOpeningTagStart("entry");
		if(s.GetPause() > FLOAT_SAFE_EPSILON){
			writer.WriteAttributeFloat("pause", s.GetPause());
		}
		writer.WriteAttributeFloat("duration", s.GetDuration());
		writer.WriteOpeningTagEnd(false, false);
		writer.WriteTextString(s.GetID());
		writer.WriteClosingTag("entry", false);
	});
}

void ceLoadSaveConversation::pWriteCurveBezier(decXmlWriter &writer, const decCurveBezier &curve){
	const int pointCount = curve.GetPointCount();
	int p;
	
	if(curve.GetInterpolationMode() == decCurveBezier::eimConstant){
		writer.WriteDataTagString("interpolation", "constant");
		
	}else if(curve.GetInterpolationMode() == decCurveBezier::eimLinear){
		writer.WriteDataTagString("interpolation", "linear");
		
	}else{
		writer.WriteDataTagString("interpolation", "bezier");
	}
	
	for(p=0; p<pointCount; p++){
		pWriteCurveBezierPoint(writer, curve.GetPointAt(p));
	}
}

void ceLoadSaveConversation::pWriteCurveBezierPoint(decXmlWriter &writer, const decCurveBezierPoint &point){
	writer.WriteOpeningTag("point", false, true);
	
	const decVector2 &coordinates = point.GetPoint();
	writer.WriteOpeningTagStart("coordinates");
	writer.WriteAttributeFloat("x", coordinates.x);
	writer.WriteAttributeFloat("y", coordinates.y);
	writer.WriteOpeningTagEnd(true);
	
	const decVector2 &handle1 = point.GetHandle1();
	writer.WriteOpeningTagStart("handle1");
	writer.WriteAttributeFloat("x", handle1.x);
	writer.WriteAttributeFloat("y", handle1.y);
	writer.WriteOpeningTagEnd(true);
	
	const decVector2 &handle2 = point.GetHandle2();
	writer.WriteOpeningTagStart("handle2");
	writer.WriteAttributeFloat("x", handle2.x);
	writer.WriteAttributeFloat("y", handle2.y);
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteClosingTag("point", true);
}



void ceLoadSaveConversation::pReadConversation(const decXmlElementTag &root, ceConversation &conversation){
	const int elementCount = root.GetElementCount();
	decStringList importConversationPath;
	int e;
	
	for(e=0; e<elementCount; e++){
		const decXmlElementTag * const tag = root.GetElementIfTag(e);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "target"){
			pReadTarget(*tag, conversation);
			
		}else if(tagName == "cameraShot"){
			pReadCameraShot(*tag, conversation);
			
		}else if(tagName == "pose"){
			// deprecated
			
		}else if(tagName == "gesture"){
			pReadGesture(*tag, conversation);
			
		}else if(tagName == "facePoseControllerNames"){
			pReadStringList(*tag, conversation.GetFacePoseControllerNameList(), "name");
			
		}else if(tagName == "facePose"){
			pReadFacePose(*tag, conversation);
			
		}else if(tagName == "lookAt"){
			// deprecated
			
		}else if(tagName == "group" || tagName == "file"){ // "file" DEPRECATED
			pReadFile(*tag, conversation);
			
		}else if(tagName == "import"){
			importConversationPath.Add(GetCDataString(*tag));
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
	
	conversation.SetImportConversationPath(importConversationPath);
	conversation.UpdateImportedConversations(*pLSSys);
}

void ceLoadSaveConversation::pReadTarget(const decXmlElementTag &root, ceConversation &conversation){
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	ceTarget::Ref target;
	int e;
	
	try{
		target = ceTarget::Ref::New();
		target->SetName(GetAttributeString(root, "name"));
		if(conversation.GetTargets().HasMatching([&](const ceTarget &t){ return t.GetName() == target->GetName(); })){
			LogErrorGenericProblemValue(root, target->GetName(), "A target with this name exists already");
		}
		
		for(e=0; e<elementCount; e++){
			tag = root.GetElementIfTag(e);
			
			if(tag){
				if(strcmp(tag->GetName(), "actor") == 0){
					target->SetActor(GetCDataString(*tag));
					
				}else if(strcmp(tag->GetName(), "coordSystem") == 0){
					target->SetCoordSystem(GetCDataString(*tag));
					
				}else if(strcmp(tag->GetName(), "bone") == 0){
					target->SetBone(GetCDataString(*tag));
					
				}else if(strcmp(tag->GetName(), "position") == 0){
					target->SetPosition(decVector(GetAttributeFloat(*tag, "x"),
						GetAttributeFloat(*tag, "y"), GetAttributeFloat(*tag, "z")));
					
				}else if(strcmp(tag->GetName(), "orientation") == 0){
					target->SetOrientation(decVector(GetAttributeFloat(*tag, "x"),
						GetAttributeFloat(*tag, "y"), GetAttributeFloat(*tag, "z")));
					
				}else{
					LogWarnUnknownTag(root, *tag);
				}
			}
		}
		
		conversation.AddTarget(target);
	}catch(const deException &){
		throw;
	}
}

void ceLoadSaveConversation::pReadCameraShot(const decXmlElementTag &root, ceConversation &conversation){
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	int e;
	
	const ceCameraShot::Ref cameraShot(ceCameraShot::Ref::New());
	cameraShot->SetName(GetAttributeString(root, "name"));
	if(conversation.GetCameraShotList().HasMatching([&](const ceCameraShot &c){ return c.GetName() == cameraShot->GetName(); })){
		LogErrorGenericProblemValue(root, cameraShot->GetName(), "A camera shot with this name exists already");
	}
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "actorCount") == 0){
				cameraShot->SetActorCount(GetCDataInt(*tag));
				
			}else if(strcmp(tag->GetName(), "cameraTarget") == 0){
				cameraShot->SetCameraTarget(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "offsetCameraFrom") == 0){
				cameraShot->SetOffsetCameraFrom(decVector(GetAttributeFloat(*tag, "x"),
					GetAttributeFloat(*tag, "y"), GetAttributeFloat(*tag, "z")));
				
			}else if(strcmp(tag->GetName(), "offsetCameraTo") == 0){
				cameraShot->SetOffsetCameraTo(decVector(GetAttributeFloat(*tag, "x"),
					GetAttributeFloat(*tag, "y"), GetAttributeFloat(*tag, "z")));
				
			}else if(strcmp(tag->GetName(), "cameraOrbitFrom") == 0){
				cameraShot->SetCameraOrbitFrom(decVector(GetAttributeFloat(*tag, "x"),
					GetAttributeFloat(*tag, "y"), GetAttributeFloat(*tag, "z")));
				
			}else if(strcmp(tag->GetName(), "cameraOrbitTo") == 0){
				cameraShot->SetCameraOrbitTo(decVector(GetAttributeFloat(*tag, "x"),
					GetAttributeFloat(*tag, "y"), GetAttributeFloat(*tag, "z")));
				
			}else if(strcmp(tag->GetName(), "cameraDistanceFrom") == 0){
				cameraShot->SetCameraDistanceFrom(GetCDataFloat(*tag));
				
			}else if(strcmp(tag->GetName(), "cameraDistanceTo") == 0){
				cameraShot->SetCameraDistanceTo(GetCDataFloat(*tag));
				
			}else if(strcmp(tag->GetName(), "lookAtTarget") == 0){
				cameraShot->SetLookAtTarget(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "offsetLookAtFrom") == 0){
				cameraShot->SetOffsetLookAtFrom(decVector(GetAttributeFloat(*tag, "x"),
					GetAttributeFloat(*tag, "y"), GetAttributeFloat(*tag, "z")));
				
			}else if(strcmp(tag->GetName(), "offsetLookAtTo") == 0){
				cameraShot->SetOffsetLookAtTo(decVector(GetAttributeFloat(*tag, "x"),
					GetAttributeFloat(*tag, "y"), GetAttributeFloat(*tag, "z")));
				
			}else if(strcmp(tag->GetName(), "positionFrom") == 0){
				cameraShot->SetPositionFrom(decVector(GetAttributeFloat(*tag, "x"),
					GetAttributeFloat(*tag, "y"), GetAttributeFloat(*tag, "z")));
				
			}else if(strcmp(tag->GetName(), "positionTo") == 0){
				cameraShot->SetPositionTo(decVector(GetAttributeFloat(*tag, "x"),
					GetAttributeFloat(*tag, "y"), GetAttributeFloat(*tag, "z")));
				
			}else if(strcmp(tag->GetName(), "rotationFrom") == 0){
				cameraShot->SetRotationFrom(decVector(GetAttributeFloat(*tag, "x"),
					GetAttributeFloat(*tag, "y"), GetAttributeFloat(*tag, "z")));
				
			}else if(strcmp(tag->GetName(), "rotationTo") == 0){
				cameraShot->SetRotationTo(decVector(GetAttributeFloat(*tag, "x"),
					GetAttributeFloat(*tag, "y"), GetAttributeFloat(*tag, "z")));
				
			}else if(strcmp(tag->GetName(), "tiltFrom") == 0){
				cameraShot->SetTiltFrom(GetCDataFloat(*tag));
				
			}else if(strcmp(tag->GetName(), "tiltTo") == 0){
				cameraShot->SetTiltTo(GetCDataFloat(*tag));
				
			}else if(strcmp(tag->GetName(), "fovFrom") == 0){
				cameraShot->SetFovFrom(GetCDataFloat(*tag));
				
			}else if(strcmp(tag->GetName(), "fovTo") == 0){
				cameraShot->SetFovTo(GetCDataFloat(*tag));
				
			}else if(strcmp(tag->GetName(), "alignTargets") == 0){
				cameraShot->SetAlignTargets(GetCDataBool(*tag));
				
			}else if(strcmp(tag->GetName(), "lockUpAxis") == 0){
				cameraShot->SetLockUpAxis(GetCDataBool(*tag));
				
			}else if(strcmp(tag->GetName(), "relativeToLookAt") == 0){
				cameraShot->SetRelativeToLookAt(GetCDataBool(*tag));
				
			}else if(strcmp(tag->GetName(), "lockCameraTarget") == 0){
				cameraShot->SetLockCameraTarget(GetCDataBool(*tag));
				
			}else if(strcmp(tag->GetName(), "lockLookAtTarget") == 0){
				cameraShot->SetLockLookAtTarget(GetCDataBool(*tag));
				
			/*
			}else if(strcmp(tag->GetName(), "curve") == 0){
				id = GetAttributeString(*tag, "id");
				
				for(i=0; i<ceCameraShot::EP_COUNT; i++){
					if(strcmp(id, vParameterNames[i]) == 0){
						curve.RemoveAllPoints();
						pReadCurveBezier(*tag, curve);
						cameraShot->SetParameterCurve(i, curve);
						break;
					}
				}
				if(i == ceCameraShot::EP_COUNT){
					LogErrorUnknownValue(*tag, id);
				}
			*/
				
			}else{
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
	
	conversation.AddCameraShot(cameraShot);
}

void ceLoadSaveConversation::pReadGesture(const decXmlElementTag &root, ceConversation &conversation){
	const int elementCount = root.GetElementCount();
	ceGesture::Ref gesture;
	const decXmlElementTag *tag;
	int e;
	
	try{
		gesture = ceGesture::Ref::New();
		gesture->SetName(GetAttributeString(root, "name"));
		if(conversation.GetGestures().HasMatching([&](const ceGesture &g){ return g.GetName() == gesture->GetName(); })){
			LogErrorGenericProblemValue(root, gesture->GetName(), "A gesture with this name exists already");
		}
		
		for(e=0; e<elementCount; e++){
			tag = root.GetElementIfTag(e);
			
			if(tag){
				if(strcmp(tag->GetName(), "animator") == 0){
					gesture->SetAnimator(GetCDataString(*tag));
					
				}else if(strcmp(tag->GetName(), "hold") == 0){
					gesture->SetHold(GetCDataBool(*tag));
					
				}else if(strcmp(tag->GetName(), "duration") == 0){
					gesture->SetDuration (GetCDataFloat(*tag));
					
				}else{
					LogWarnUnknownTag(root, *tag);
				}
			}
		}
		
		conversation.AddGesture(gesture);
	}catch(const deException &){
		throw;
	}
}

void ceLoadSaveConversation::pReadFacePose(const decXmlElementTag &root, ceConversation &conversation){
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	int e;
	
	const ceFacePose::Ref facePose(ceFacePose::Ref::New());
	facePose->SetName(GetAttributeString(root, "name"));
	if(conversation.GetFacePoseList().HasMatching([&](const ceFacePose &f){ return f.GetName() == facePose->GetName(); })){
		LogErrorGenericProblemValue(root, facePose->GetName(), "A face pose with this name exists already");
	}
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "controller") == 0){
				ceControllerValue::Ref entry;
				if(HasAttribute(*tag, "index")){ // deprecated
					entry = ceControllerValue::Ref::New(GetAttributeInt(*tag, "index"), GetAttributeFloat(*tag, "value"));
					
				}else{
					entry = ceControllerValue::Ref::New(GetAttributeString(*tag, "name"), GetAttributeFloat(*tag, "value"));
				}
				facePose->GetControllers().Add(entry);
				
			}else{
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
	
	conversation.AddFacePose(facePose);
}

void ceLoadSaveConversation::pReadFile(const decXmlElementTag &root, ceConversation &conversation){
	const int elementCount = root.GetElementCount();
	ceConversationFile::Ref file;
	const decXmlElementTag *tag;
	int e;
	
	try{
		file = ceConversationFile::Ref::New();
		file->SetID(GetAttributeString(root, "id"));
		if(conversation.GetFiles().HasMatching([&](const ceConversationFile &f){ return f.GetID() == file->GetID(); })){
			LogErrorGenericProblemValue(root, file->GetID(), "An file with this ID exists already");
		}
		
		for(e=0; e<elementCount; e++){
			tag = root.GetElementIfTag(e);
			
			if(tag){
				if(strcmp(tag->GetName(), "topic") == 0){
					pReadTopic(*tag, conversation, *file);
					
				}else{
					LogWarnUnknownTag(root, *tag);
				}
			}
		}
		
		conversation.AddFile(file);
	}catch(const deException &){
		throw;
	}
}

void ceLoadSaveConversation::pReadTopic(const decXmlElementTag &root, ceConversation &conversation, ceConversationFile &file){
	const int elementCount = root.GetElementCount();
	ceConversationTopic::Ref topic;
	const decXmlElementTag *tag;
	int e;
	
	try{
		topic = ceConversationTopic::Ref::New();
		topic->SetID(GetAttributeString(root, "id"));
		if(file.GetTopics().HasMatching([&](const ceConversationTopic &t){ return t.GetID() == topic->GetID(); })){
			LogErrorGenericProblemValue(root, topic->GetID(), "A topic with this ID exists already");
		}
		
		for(e=0; e<elementCount; e++){
			tag = root.GetElementIfTag(e);
			
			if(tag){
				if(strcmp(tag->GetName(), "actions") == 0){
					pReadActionList(*tag, conversation, topic->GetActions());
					if(topic->GetActions().GetCount() > 0){
						topic->SetActive(topic->GetActions().GetAt(0), nullptr);
					}
					
				}else{
					LogWarnUnknownTag(root, *tag);
				}
			}
		}
		
		file.AddTopic(topic);
	}catch(const deException &){
		throw;
	}
}

void ceLoadSaveConversation::pReadActionList(const decXmlElementTag &root,
ceConversation &conversation, ceConversationAction::List &list){
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	int e;
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			const ceConversationAction::Ref action(pReadAction(*tag, conversation));
			if(action){
				list.Add(action);
				
			}else{
				LogErrorUnknownTag(root, *tag);
			}
		}
	}
}

ceConversationAction::Ref ceLoadSaveConversation::pReadAction(const decXmlElementTag &root, ceConversation &conversation){
	if(strcmp(root.GetName(), "cameraShot") == 0){
		const ceCACameraShot::Ref a(ceCACameraShot::Ref::New());
		pReadActionCameraShot(root, a);
		return a;
		
	}else if(strcmp(root.GetName(), "music") == 0){
		const ceCAMusic::Ref a(ceCAMusic::Ref::New());
		pReadActionMusic(root, a);
		return a;
		
	}else if(strcmp(root.GetName(), "actorSpeak") == 0){
		const ceCAActorSpeak::Ref a(ceCAActorSpeak::Ref::New(conversation.GetEngine()));
		pReadActionActorSpeak(root, a);
		return a;
		
	}else if(strcmp(root.GetName(), "ifElse") == 0){
		const ceCAIfElse::Ref a(ceCAIfElse::Ref::New());
		pReadActionIfElse(root, conversation, a);
		return a;
		
	}else if(strcmp(root.GetName(), "playerChoice") == 0){
		const ceCAPlayerChoice::Ref a(ceCAPlayerChoice::Ref::New());
		pReadActionPlayerChoice(root, conversation, a);
		return a;
		
	}else if(strcmp(root.GetName(), "stopConversation") == 0){
		const ceCAStopConversation::Ref a(ceCAStopConversation::Ref::New());
		pReadActionStopConversation(root, a);
		return a;
		
	}else if(strcmp(root.GetName(), "stopTopic") == 0){
		const ceCAStopTopic::Ref a(ceCAStopTopic::Ref::New());
		pReadActionStopTopic(root, a);
		return a;
		
	}else if(strcmp(root.GetName(), "snippet") == 0){
		const ceCASnippet::Ref a(ceCASnippet::Ref::New());
		pReadActionSnippet(root, a);
		return a;
		
	}else if(strcmp(root.GetName(), "setVariable") == 0){
		const ceCASetVariable::Ref a(ceCASetVariable::Ref::New());
		pReadActionSetVariable(root, a);
		return a;
		
	}else if(strcmp(root.GetName(), "setActorParameter") == 0){
		const ceCASetActorParameter::Ref a(ceCASetActorParameter::Ref::New());
		pReadActionSetActorParameter(root, a);
		return a;
		
	}else if(strcmp(root.GetName(), "actorCommand") == 0){
		const ceCAActorCommand::Ref a(ceCAActorCommand::Ref::New());
		pReadActionActorCommand(root, a);
		return a;
		
	}else if(strcmp(root.GetName(), "gameCommand") == 0){
		const ceCAGameCommand::Ref a(ceCAGameCommand::Ref::New());
		pReadActionGameCommand(root, a);
		return a;
		
	}else if(strcmp(root.GetName(), "wait") == 0){
		const ceCAWait::Ref a(ceCAWait::Ref::New());
		pReadActionWait(root, conversation, a);
		return a;
		
	}else if(strcmp(root.GetName(), "trigger") == 0){
		const ceCATrigger::Ref a(ceCATrigger::Ref::New());
		pReadActionTrigger(root, a);
		return a;
		
	}else if(strcmp(root.GetName(), "actorAdd") == 0){
		const ceCAActorAdd::Ref a(ceCAActorAdd::Ref::New());
		pReadActionActorAdd(root, a);
		return a;
		
	}else if(strcmp(root.GetName(), "actorRemove") == 0){
		const ceCAActorRemove::Ref a(ceCAActorRemove::Ref::New());
		pReadActionActorRemove(root, a);
		return a;
		
	}else if(strcmp(root.GetName(), "coordSystemAdd") == 0){
		const ceCACoordSystemAdd::Ref a(ceCACoordSystemAdd::Ref::New());
		pReadActionCoordSystemAdd(root, a);
		return a;
		
	}else if(strcmp(root.GetName(), "coordSystemRemove") == 0){
		const ceCACoordSystemRemove::Ref a(ceCACoordSystemRemove::Ref::New());
		pReadActionCoordSystemRemove(root, a);
		return a;
		
	}else if(strcmp(root.GetName(), "comment") == 0){
		const ceCAComment::Ref a(ceCAComment::Ref::New());
		pReadActionComment(root, a);
		return a;
		
	}else{
		return {};
	}
}

bool ceLoadSaveConversation::pReadActionCommon(const decXmlElementTag &root, ceConversationAction &action){
	if(root.GetName() == "waitForActor"){
		action.SetWaitForActor(GetCDataBool(root));
		return true;
		
	}else if(root.GetName() == "waitSpeakOnly"){
		action.SetWaitSpeakOnly(GetCDataBool(root));
		return true;
		
	}else if(root.GetName() == "waitForActorID"){
		action.SetWaitForActorID(GetCDataString(root));
		return true;
		
	}else if(root.GetName() == "delay"){
		action.SetDelay(GetCDataFloat(root));
		return true;
		
	}else{
		return false;
	}
}

void ceLoadSaveConversation::pReadActionCameraShot(const decXmlElementTag &root, ceCACameraShot &action){
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	int e;
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "name") == 0){
				action.SetName(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "duration") == 0){
				action.SetDuration(GetCDataFloat(*tag));
				
			}else if(strcmp(tag->GetName(), "cameraTarget") == 0){
				action.SetCameraTarget(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "lookAtTarget") == 0){
				action.SetLookAtTarget(GetCDataString(*tag));
				
			}else if(!pReadActionCommon(*tag, action)){
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
}

void ceLoadSaveConversation::pReadActionMusic(const decXmlElementTag &root, ceCAMusic &action){
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	int e;
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "name") == 0){
				action.SetName(GetCDataString(*tag));
				
			}else if(!pReadActionCommon(*tag, action)){
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
}

void ceLoadSaveConversation::pReadActionActorSpeak(const decXmlElementTag &root, ceCAActorSpeak &action){
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	int e;
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "actor") == 0){
				action.SetActor(GetCDataString(*tag));
				
			}else if(tag->GetName() == "textBoxText"){
				action.SetTextBoxText(decUnicodeString::NewFromUTF8(ReadMultilineString(*tag)));
				
			}else if(tag->GetName() == "textBoxTextTranslate"){
				action.SetTextBoxTextTranslate(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "textBoxTextStyle") == 0){
				action.SetTextBoxTextStyle(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "pathSound") == 0){
				action.SetPathSound(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "words") == 0){
				pReadStripList(*tag, action.GetWords());
				
			}else if(strcmp(tag->GetName(), "facePoses") == 0){
				pReadStripList(*tag, action.GetFacePoses());
				
			}else if(strcmp(tag->GetName(), "gestures") == 0){
				pReadStripList(*tag, action.GetGestures());
				
			}else if(strcmp(tag->GetName(), "bodyLookAts") == 0){
				pReadStripList(*tag, action.GetBodyLookAts());
				
			}else if(strcmp(tag->GetName(), "headLookAts") == 0){
				pReadStripList(*tag, action.GetHeadLookAts());
				
			}else if(strcmp(tag->GetName(), "eyesLookAts") == 0){
				pReadStripList(*tag, action.GetEyesLookAts());
				
			}else if(strcmp(tag->GetName(), "movement") == 0){
				action.SetMovement(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "minSpeechTime") == 0){
				action.SetMinSpeechTime(GetCDataFloat(*tag));
				
			}else if(strcmp(tag->GetName(), "useSpeechAnimation") == 0){
				action.SetUseSpeechAnimation(GetCDataBool(*tag));
				
			}else if(!pReadActionCommon(*tag, action)){
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
}

void ceLoadSaveConversation::pReadActionIfElse(const decXmlElementTag &root, ceConversation &conversation, ceCAIfElse &action){
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	int e;
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "condition") == 0){ // DEPRECATED
				LogWarnGenericProblemTag(root, tag->GetName(), "DEPRECATED");
				ceCAIfElseCase::Ref ifcase;
				if(action.GetCases().GetCount() == 0){
					ifcase = ceCAIfElseCase::Ref::New();
					action.GetCases().Add(ifcase);
				}else{
					ifcase = action.GetCases().GetAt(0);
				}
				
				ceConversationCondition::List conditionList;
				pReadConditionList(*tag, conversation, conditionList);
				if(conditionList.GetCount() == 1){
					ifcase->SetCondition(conditionList.First());
				}else{
					LogErrorGenericProblemTag(root, tag->GetName(), "Exactly one condition expected but not found.");
				}
				
			}else if(strcmp(tag->GetName(), "if") == 0){ // DEPRECATED
				LogWarnGenericProblemTag(root, tag->GetName(), "DEPRECATED");
				ceCAIfElseCase::Ref ifcase;
				if(action.GetCases().GetCount() == 0){
					ifcase = ceCAIfElseCase::Ref::New();
					action.GetCases().Add(ifcase);
				}else{
					ifcase = action.GetCases().GetAt(0);
				}
				
				pReadActionList(*tag, conversation, ifcase->GetActions());
				
			}else if(strcmp(tag->GetName(), "case") == 0){
				pReadActionIfElseCase(*tag, conversation, action);
				
			}else if(strcmp(tag->GetName(), "else") == 0){
				pReadActionList(*tag, conversation, action.GetElseActions());
				
			}else if(!pReadActionCommon(*tag, action)){
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
}

void ceLoadSaveConversation::pReadActionIfElseCase(const decXmlElementTag &root, ceConversation &conversation, ceCAIfElse &action){
	const int elementCount = root.GetElementCount();
	ceCAIfElseCase::Ref ifcase;
	const decXmlElementTag *tag;
	int e;
	
	try{
		ifcase = ceCAIfElseCase::Ref::New();
		
		for(e=0; e<elementCount; e++){
			tag = root.GetElementIfTag(e);
			
			if(tag){
				if(strcmp(tag->GetName(), "condition") == 0){
					ceConversationCondition::List conditionList;
					pReadConditionList(*tag, conversation, conditionList);
					if(conditionList.GetCount() == 1){
						ifcase->SetCondition(conditionList.First());
					}else{
						LogErrorGenericProblemTag(root, tag->GetName(), "Exactly one condition expected but not found.");
					}
					
				}else if(strcmp(tag->GetName(), "actions") == 0){
					pReadActionList(*tag, conversation, ifcase->GetActions());
					
				}else{
					LogWarnUnknownTag(root, *tag);
				}
			}
		}
		
		action.GetCases().Add(ifcase);
	}catch(const deException &){
		throw;
	}
}

void ceLoadSaveConversation::pReadActionPlayerChoice(const decXmlElementTag &root, ceConversation &conversation, ceCAPlayerChoice &action){
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	int e;
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "variable") == 0){
				action.SetVariableName(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "actions") == 0){
				pReadActionList(*tag, conversation, action.GetActions());
				
			}else if(strcmp(tag->GetName(), "option") == 0){
				pReadActionPlayerChoiceOption(*tag, conversation, action);
				
			}else if(!pReadActionCommon(*tag, action)){
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
}

void ceLoadSaveConversation::pReadActionPlayerChoiceOption(const decXmlElementTag &root, ceConversation &conversation, ceCAPlayerChoice &action){
	const int elementCount = root.GetElementCount();
	ceCAPlayerChoiceOption::Ref option;
	const decXmlElementTag *tag;
	int e;
	
	try{
		option = ceCAPlayerChoiceOption::Ref::New();
		
		for(e=0; e<elementCount; e++){
			tag = root.GetElementIfTag(e);
			
			if(tag){
				if(strcmp(tag->GetName(), "text") == 0){
					option->SetText(decUnicodeString::NewFromUTF8(GetCDataString(*tag)));
					
				}else if(strcmp(tag->GetName(), "condition") == 0){
					ceConversationCondition::List conditions;
					pReadConditionList(*tag, conversation, conditions);
					
					if(conditions.GetCount() == 1){
						option->SetCondition(conditions.First());
						
					}else{
						LogErrorGenericProblemTag(root, tag->GetName(),
							"Exactly one condition expected.");
					}
					
				}else if(strcmp(tag->GetName(), "actions") == 0){
					pReadActionList(*tag, conversation, option->GetActions());
					
				}else{
					LogWarnUnknownTag(root, *tag);
				}
			}
		}
		
		action.GetOptions().Add(option);
	}catch(const deException &){
		throw;
	}
}

void ceLoadSaveConversation::pReadActionStopConversation(const decXmlElementTag &root, ceCAStopConversation &action){
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	int e;
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			if(!pReadActionCommon(*tag, action)){
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
}

void ceLoadSaveConversation::pReadActionStopTopic(const decXmlElementTag &root, ceCAStopTopic &action){
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	int e;
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			if(!pReadActionCommon(*tag, action)){
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
}

void ceLoadSaveConversation::pReadActionSnippet(const decXmlElementTag &root, ceCASnippet &action){
	const int elementCount = root.GetElementCount();
	int i;
	
	for(i=0; i<elementCount; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "group" || tagName == "file"){ // "file" DEPRECATED
			action.SetFile(GetCDataString(*tag));
			
		}else if(tagName == "topic"){
			action.SetTopic(GetCDataString(*tag));
			
		}else if(tagName == "createSideLane"){
			action.SetCreateSideLane(GetCDataBool(*tag));
			
		}else if(!pReadActionCommon(*tag, action)){
			LogWarnUnknownTag(root, *tag);
		}
	}
}

void ceLoadSaveConversation::pReadActionSetVariable(const decXmlElementTag &root, ceCASetVariable &action){
	const int elementCount = root.GetElementCount();
	int i;
	
	for(i=0; i<elementCount; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if(tagName == "name"){
			action.SetName(GetCDataString(*tag));
			
		}else if(tagName == "operator"){
			const char * const identifier = GetCDataString(*tag);
			
			if(strcmp(identifier, "set") == 0){
				action.SetOperator(ceCASetVariable::eopSet);
				
			}else if(strcmp(identifier, "increment") == 0){
				action.SetOperator(ceCASetVariable::eopIncrement);
				
			}else if(strcmp(identifier, "decrement") == 0){
				action.SetOperator(ceCASetVariable::eopDecrement);
				
			}else if(strcmp(identifier, "random") == 0){
				action.SetOperator(ceCASetVariable::eopRandom);
				
			}else{
				LogErrorUnknownValue(*tag, identifier);
			}
			
		}else if(tagName == "value"){
			action.SetValue(GetCDataInt(*tag));
			
		}else if(tagName == "valueVariable"){
			action.SetValueVariable(GetCDataString(*tag));
			
		}else if(!pReadActionCommon(*tag, action)){
			LogWarnUnknownTag(root, *tag);
		}
	}
}

void ceLoadSaveConversation::pReadActionSetActorParameter(const decXmlElementTag &root, ceCASetActorParameter &action){
	const int elementCount = root.GetElementCount();
	int i;
	
	for(i=0; i<elementCount; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if(tagName == "actor"){
			action.SetActor(GetCDataString(*tag));
			
		}else if(tagName == "name"){
			action.SetName(GetCDataString(*tag));
			
		}else if(tagName == "operator"){
			const char * const identifier = GetCDataString(*tag);
			
			if(strcmp(identifier, "set") == 0){
				action.SetOperator(ceCASetActorParameter::eopSet);
				
			}else if(strcmp(identifier, "increment") == 0){
				action.SetOperator(ceCASetActorParameter::eopIncrement);
				
			}else if(strcmp(identifier, "decrement") == 0){
				action.SetOperator(ceCASetActorParameter::eopDecrement);
				
			}else if(strcmp(identifier, "random") == 0){
				action.SetOperator(ceCASetActorParameter::eopRandom);
				
			}else{
				LogErrorUnknownValue(*tag, identifier);
			}
			
		}else if(tagName == "value"){
			action.SetValue(GetCDataInt(*tag));
			
		}else if(tagName == "valueVariable"){
			action.SetValueVariable(GetCDataString(*tag));
			
		}else if(!pReadActionCommon(*tag, action)){
			LogWarnUnknownTag(root, *tag);
		}
	}
}

void ceLoadSaveConversation::pReadActionActorCommand(const decXmlElementTag &root, ceCAActorCommand &action){
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	int e;
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "actor") == 0){
				action.SetActor(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "command") == 0){
				action.SetCommand(GetCDataString(*tag));
				
			}else if(!pReadActionCommon(*tag, action)){
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
}

void ceLoadSaveConversation::pReadActionGameCommand(const decXmlElementTag &root, ceCAGameCommand &action){
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	int e;
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "command") == 0){
				action.SetCommand(GetCDataString(*tag));
				
			}else if(!pReadActionCommon(*tag, action)){
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
}

void ceLoadSaveConversation::pReadActionWait(const decXmlElementTag &root, ceConversation &conversation, ceCAWait &action){
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	int e;
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "condition") == 0){
				ceConversationCondition::List conditionList;
				pReadConditionList(*tag, conversation, conditionList);
				if(conditionList.GetCount() == 1){
					action.SetCondition(conditionList.First());
				}else{
					LogErrorGenericProblemTag(root, tag->GetName(), "Exactly one condition expected but not found.");
				}
				
			}else if(strcmp(tag->GetName(), "interval") == 0){
				action.SetInterval(GetCDataFloat(*tag));
				
			}else if(strcmp(tag->GetName(), "actions") == 0){
				pReadActionList(*tag, conversation, action.GetActions());
				
			}else if(!pReadActionCommon(*tag, action)){
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
}

void ceLoadSaveConversation::pReadActionTrigger(const decXmlElementTag &root, ceCATrigger &action){
	const int elementCount = root.GetElementCount();
	int e;
	
	for(e=0; e<elementCount; e++){
		const decXmlElementTag * const tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "name") == 0){
				action.SetName(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "action") == 0){
				const char * const identifier = GetCDataString(*tag);
				
				if(strcmp(identifier, "fire") == 0){
					action.SetAction(ceCATrigger::eaFire);
					
				}else if(strcmp(identifier, "reset") == 0){
					action.SetAction(ceCATrigger::eaReset);
					
				}else if(strcmp(identifier, "pulse") == 0){
					action.SetAction(ceCATrigger::eaPulse);
					
				}else if(strcmp(identifier, "fullReset") == 0){
					action.SetAction(ceCATrigger::eaFullReset);
					
				}else{
					LogErrorUnknownValue(*tag, identifier);
				}
				
			}else if(!pReadActionCommon(*tag, action)){
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
}

void ceLoadSaveConversation::pReadActionActorAdd(const decXmlElementTag &root, ceCAActorAdd &action){
	const int elementCount = root.GetElementCount();
	int e;
	
	for(e=0; e<elementCount; e++){
		const decXmlElementTag * const tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "actor") == 0){
				action.SetID(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "aliasID") == 0){
				action.SetAliasID(GetCDataString(*tag));
				
			}else if(!pReadActionCommon(*tag, action)){
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
}

void ceLoadSaveConversation::pReadActionActorRemove(const decXmlElementTag &root, ceCAActorRemove &action){
	const int elementCount = root.GetElementCount();
	int e;
	
	for(e=0; e<elementCount; e++){
		const decXmlElementTag * const tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "actor") == 0){
				action.SetActor(GetCDataString(*tag));
				
			}else if(!pReadActionCommon(*tag, action)){
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
}

void ceLoadSaveConversation::pReadActionCoordSystemAdd(const decXmlElementTag &root, ceCACoordSystemAdd &action){
	const int elementCount = root.GetElementCount();
	int e;
	
	for(e=0; e<elementCount; e++){
		const decXmlElementTag * const tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "coordSystem") == 0){
				action.SetCoordSystemID(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "aliasID") == 0){
				action.SetAliasID(GetCDataString(*tag));
				
			}else if(!pReadActionCommon(*tag, action)){
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
}

void ceLoadSaveConversation::pReadActionCoordSystemRemove(const decXmlElementTag &root, ceCACoordSystemRemove &action){
	const int elementCount = root.GetElementCount();
	int e;
	
	for(e=0; e<elementCount; e++){
		const decXmlElementTag * const tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "coordSystem") == 0){
				action.SetCoordSystemID(GetCDataString(*tag));
				
			}else if(!pReadActionCommon(*tag, action)){
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
}

void ceLoadSaveConversation::pReadActionComment(const decXmlElementTag &root, ceCAComment &action){
	const int elementCount = root.GetElementCount();
	int i;
	
	for(i=0; i<elementCount; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		
		if(tag){
			if(strcmp(tag->GetName(), "comment") == 0){
				action.SetComment(decString(GetCDataString(*tag)).GetReplacedString("\\n", "\n"));
				
			}else{
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
}

void ceLoadSaveConversation::pReadConditionList(const decXmlElementTag &root,
ceConversation &conversation, ceConversationCondition::List &list){
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	int e;
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			const ceConversationCondition::Ref condition(pReadCondition(*tag, conversation));
			if(condition){
				list.Add(condition);
				
			}else{
				LogErrorUnknownTag(root, *tag);
			}
		}
	}
}

ceConversationCondition::Ref ceLoadSaveConversation::pReadCondition(const decXmlElementTag &root, ceConversation &conversation){
	if(strcmp(root.GetName(), "logic") == 0){
		const ceCConditionLogic::Ref c(ceCConditionLogic::Ref::New());
		pReadConditionLogic(root, conversation, c);
		return c;
		
	}else if(strcmp(root.GetName(), "hasActor") == 0){
		const ceCConditionHasActor::Ref c(ceCConditionHasActor::Ref::New());
		pReadConditionHasActor(root, c);
		return c;
		
	}else if(strcmp(root.GetName(), "actorInConversation") == 0){
		const ceCConditionActorInConversation::Ref c(ceCConditionActorInConversation::Ref::New());
		pReadConditionActorInConversation(root, c);
		return c;
		
	}else if(strcmp(root.GetName(), "variable") == 0){
		const ceCConditionVariable::Ref c(ceCConditionVariable::Ref::New());
		pReadConditionVariable(root, c);
		return c;
		
	}else if(strcmp(root.GetName(), "actorParameter") == 0){
		const ceCConditionActorParameter::Ref c(ceCConditionActorParameter::Ref::New());
		pReadConditionActorParameter(root, c);
		return c;
		
	}else if(strcmp(root.GetName(), "actorCommand") == 0){
		const ceCConditionActorCommand::Ref c(ceCConditionActorCommand::Ref::New());
		pReadConditionActorCommand(root, c);
		return c;
		
	}else if(strcmp(root.GetName(), "gameCommand") == 0){
		const ceCConditionGameCommand::Ref c(ceCConditionGameCommand::Ref::New());
		pReadConditionGameCommand(root, c);
		return c;
		
	}else if(strcmp(root.GetName(), "trigger") == 0){
		const ceCConditionTrigger::Ref c(ceCConditionTrigger::Ref::New());
		pReadConditionTrigger(root, c);
		return c;
		
	}else{
		return {};
	}
}

void ceLoadSaveConversation::pReadConditionLogic(const decXmlElementTag &root,
ceConversation &conversation, ceCConditionLogic &condition){
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	const char *identifier;
	int e;
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "operator") == 0){
				identifier = GetCDataString(*tag);
				
				if(strcmp(identifier, "none") == 0){
					condition.SetOperator(ceCConditionLogic::eopNone);
					
				}else if(strcmp(identifier, "any") == 0){
					condition.SetOperator(ceCConditionLogic::eopAny);
					
				}else if(strcmp(identifier, "all") == 0){
					condition.SetOperator(ceCConditionLogic::eopAll);
					
				}else{
					LogErrorUnknownValue(*tag, identifier);
				}
				
			}else if(strcmp(tag->GetName(), "conditions") == 0){
				pReadConditionList(*tag, conversation, condition.GetConditions());
				
			}else{
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
}

void ceLoadSaveConversation::pReadConditionHasActor(const decXmlElementTag &root,
ceCConditionHasActor &condition){
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	int e;
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "actor") == 0){
				condition.SetActor(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "negate") == 0){
				condition.SetNegate(GetCDataBool(*tag));
				
			}else{
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
}

void ceLoadSaveConversation::pReadConditionActorInConversation(const decXmlElementTag &root,
ceCConditionActorInConversation &condition){
	const int elementCount = root.GetElementCount();
	int i;
	
	for(i=0; i<elementCount; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		if(strcmp(tag->GetName(), "actor") == 0){
			condition.SetActor(GetCDataString(*tag));
			
		}else if(strcmp(tag->GetName(), "negate") == 0){
			condition.SetNegate(GetCDataBool(*tag));
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
}

void ceLoadSaveConversation::pReadConditionVariable(const decXmlElementTag &root, ceCConditionVariable &condition){
	const int elementCount = root.GetElementCount();
	int i;
	
	for(i=0; i<elementCount; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if(tagName == "operator"){
			const char * const identifier = GetCDataString(*tag);
			
			if(strcmp(identifier, "equal") == 0){
				condition.SetOperator(ceCConditionVariable::eopEqual);
				
			}else if(strcmp(identifier, "notEqual") == 0){
				condition.SetOperator(ceCConditionVariable::eopNotEqual);
				
			}else if(strcmp(identifier, "less") == 0){
				condition.SetOperator(ceCConditionVariable::eopLess);
				
			}else if(strcmp(identifier, "lessEqual") == 0){
				condition.SetOperator(ceCConditionVariable::eopLessEqual);
				
			}else if(strcmp(identifier, "greater") == 0){
				condition.SetOperator(ceCConditionVariable::eopGreater);
				
			}else if(strcmp(identifier, "greaterEqual") == 0){
				condition.SetOperator(ceCConditionVariable::eopGreaterEqual);
				
			}else{
				LogErrorUnknownValue(*tag, identifier);
			}
			
		}else if(tagName == "variable"){
			condition.SetVariable(GetCDataString(*tag));
			
		}else if(tagName == "testValue"){
			condition.SetTestValue(GetCDataInt(*tag));
			
		}else if(tagName == "testVariable"){
			condition.SetTestVariable(GetCDataString(*tag));
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
}

void ceLoadSaveConversation::pReadConditionActorParameter(const decXmlElementTag &root, ceCConditionActorParameter &condition){
	const int elementCount = root.GetElementCount();
	int i;
	
	for(i=0; i<elementCount; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if(tagName == "actor"){
			condition.SetActor(GetCDataString(*tag));
			
		}else if(tagName == "operator"){
			const char * const identifier = GetCDataString(*tag);
			
			if(strcmp(identifier, "equal") == 0){
				condition.SetOperator(ceCConditionActorParameter::eopEqual);
				
			}else if(strcmp(identifier, "notEqual") == 0){
				condition.SetOperator(ceCConditionActorParameter::eopNotEqual);
				
			}else if(strcmp(identifier, "less") == 0){
				condition.SetOperator(ceCConditionActorParameter::eopLess);
				
			}else if(strcmp(identifier, "lessEqual") == 0){
				condition.SetOperator(ceCConditionActorParameter::eopLessEqual);
				
			}else if(strcmp(identifier, "greater") == 0){
				condition.SetOperator(ceCConditionActorParameter::eopGreater);
				
			}else if(strcmp(identifier, "greaterEqual") == 0){
				condition.SetOperator(ceCConditionActorParameter::eopGreaterEqual);
				
			}else{
				LogErrorUnknownValue(*tag, identifier);
			}
			
		}else if(tagName == "parameter"){
			condition.SetParameter(GetCDataString(*tag));
			
		}else if(tagName == "testValue"){
			condition.SetTestValue(GetCDataInt(*tag));
			
		}else if(tagName == "testVariable"){
			condition.SetTestVariable(GetCDataString(*tag));
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
}

void ceLoadSaveConversation::pReadConditionActorCommand(const decXmlElementTag &root, ceCConditionActorCommand &condition){
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	int e;
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "actor") == 0){
				condition.SetActor(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "command") == 0){
				condition.SetCommand(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "negate") == 0){
				condition.SetNegate(GetCDataBool(*tag));
				
			}else{
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
}

void ceLoadSaveConversation::pReadConditionGameCommand(const decXmlElementTag &root, ceCConditionGameCommand &condition){
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	int e;
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "command") == 0){
				condition.SetCommand(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "negate") == 0){
				condition.SetNegate(GetCDataBool(*tag));
				
			}else{
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
}

void ceLoadSaveConversation::pReadConditionTrigger(const decXmlElementTag &root, ceCConditionTrigger &condition){
	const int elementCount = root.GetElementCount();
	int e;
	
	for(e=0; e<elementCount; e++){
		const decXmlElementTag * const tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "testMode") == 0){
				const char * const identifier = GetCDataString(*tag);
				
				if(strcmp(identifier, "fired") == 0){
					condition.SetTestMode(ceCConditionTrigger::etmFired);
					
				}else if(strcmp(identifier, "notFired") == 0){
					condition.SetTestMode(ceCConditionTrigger::etmNotFired);
					
				}else if(strcmp(identifier, "everFired") == 0){
					condition.SetTestMode(ceCConditionTrigger::etmEverFired);
					
				}else if(strcmp(identifier, "neverFired") == 0){
					condition.SetTestMode(ceCConditionTrigger::etmNeverFired);
					
				}else{
					LogErrorUnknownValue(*tag, identifier);
				}
				
			}else if(strcmp(tag->GetName(), "trigger") == 0){
				condition.SetTrigger(GetCDataString(*tag));
				
			}else{
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
}

void ceLoadSaveConversation::pReadStripList(const decXmlElementTag &root, ceStrip::List &list){
	const int elementCount = root.GetElementCount();
	int e;
	
	for(e=0; e<elementCount; e++){
		const decXmlElementTag * const tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "entry") == 0){
				float pause = 0.0f;
				
				if(HasAttribute(*tag, "pause")){
					pause = GetAttributeFloat(*tag, "pause");
				}
				
				list.Add(ceStrip::Ref::New(GetCDataString(*tag), GetAttributeFloat(*tag, "duration"), pause));
				
			}else{
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
}

void ceLoadSaveConversation::pReadCurveBezier(const decXmlElementTag &root, decCurveBezier &curve){
	int e, elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	const char *type;
	
	curve.RemoveAllPoints();
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "interpolation") == 0){
				type = GetCDataString(*tag);
				
				if(strcmp(type, "constant") == 0){
					curve.SetInterpolationMode(decCurveBezier::eimConstant);
					
				}else if(strcmp(type, "linear") == 0){
					curve.SetInterpolationMode(decCurveBezier::eimLinear);
					
				}else if(strcmp(type, "bezier") == 0){
					curve.SetInterpolationMode(decCurveBezier::eimBezier);
					
				}else{
					LogErrorUnknownValue(*tag, type);
				}
				
			}else if(strcmp(tag->GetName(), "point") == 0){
				pReadCurveBezierPoint(*tag, curve);
				
			}else{
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
}

void ceLoadSaveConversation::pReadCurveBezierPoint(const decXmlElementTag &root, decCurveBezier &curve){
	int e, elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	decVector2 coordinates;
	decVector2 handle1;
	decVector2 handle2;
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "coordinates") == 0){
				pReadVector2(*tag, coordinates);
				
			}else if(strcmp(tag->GetName(), "handle1") == 0){
				pReadVector2(*tag, handle1);
				
			}else if(strcmp(tag->GetName(), "handle2") == 0){
				pReadVector2(*tag, handle2);
				
			}else{
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
	
	curve.AddPoint(decCurveBezierPoint(coordinates, handle1, handle2));
}

void ceLoadSaveConversation::pReadVector(const decXmlElementTag &tag, decVector &vector){
	const decXmlAttValue *value;
	
	value = tag.FindAttribute("x");
	if(value){
		vector.x = (float)strtod(value->GetValue(), nullptr);
	}
	
	value = tag.FindAttribute("y");
	if(value){
		vector.y = (float)strtod(value->GetValue(), nullptr);
	}
	
	value = tag.FindAttribute("z");
	if(value){
		vector.z = (float)strtod(value->GetValue(), nullptr);
	}
}

void ceLoadSaveConversation::pReadVector2(const decXmlElementTag &tag, decVector2 &vector){
	const decXmlAttValue *value;
	
	value = tag.FindAttribute("x");
	if(value){
		vector.x = (float)strtod(value->GetValue(), nullptr);
	}
	
	value = tag.FindAttribute("y");
	if(value){
		vector.y = (float)strtod(value->GetValue(), nullptr);
	}
}

void ceLoadSaveConversation::pReadStringList(const decXmlElementTag &root, decStringList &list, const char *entryTagName){
	int e, elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), entryTagName) == 0){
				list.Add(GetCDataString(*tag));
				
			}else{
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
}
