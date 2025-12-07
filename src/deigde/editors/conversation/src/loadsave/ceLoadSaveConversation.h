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

#ifndef _CELOADSAVECONVO_H_
#define _CELOADSAVECONVO_H_

#include <deigde/utils/igdeBaseXML.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class ceCAIfElseCase;
class ceCAComment;
class ceCACoordSystemRemove;
class ceCACoordSystemAdd;
class ceCAActorAdd;
class ceCAActorCommand;
class ceCAActorRemove;
class ceCAActorSpeak;
class ceCACameraShot;
class ceCAGameCommand;
class ceCAIfElse;
class ceCameraShot;
class ceCAMusic;
class ceCAPlayerChoice;
class ceCAPlayerChoiceOption;
class ceCASetActorParameter;
class ceCASetVariable;
class ceCASnippet;
class ceCAStopConversation;
class ceCAStopTopic;
class ceCATrigger;
class ceCAWait;
class ceCConditionActorCommand;
class ceCConditionActorParameter;
class ceCConditionGameCommand;
class ceCConditionHasActor;
class ceCConditionActorInConversation;
class ceCConditionLogic;
class ceCConditionTrigger;
class ceCConditionVariable;
class ceControllerValueList;
class ceConversation;
class ceConversationAction;
class ceConversationActionList;
class ceConversationCondition;
class ceConversationConditionList;
class ceConversationFile;
class ceConversationTopic;
class ceFacePose;
class ceGesture;
class ceLoadSaveSystem;
class ceStripList;
class ceTarget;

class decStringList;
class decCurveBezier;
class decCurveBezierPoint;
class decBaseFileReader;
class decBaseFileWriter;
class decXmlWriter;
class decXmlElementTag;



/**
 * \brief Load/Save Conversation.
 */
class ceLoadSaveConversation : public igdeBaseXML{
private:
	ceLoadSaveSystem *pLSSys;
	decString pName;
	decString pPattern;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new loader. */
	ceLoadSaveConversation(ceLoadSaveSystem *lssys, deLogger *logger, const char *loggerSource);
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the name. */
	inline const decString &GetName() const{return pName;}
	/** Retrieves the pattern. */
	inline const decString &GetPattern() const{return pPattern;}
	
	/** Loads the conversation from file. */
	void LoadConversation(ceConversation &conversation, decBaseFileReader &reader, const char *filename);
	/** Saves the conversation to file. */
	void SaveConversation(const ceConversation &conversation, decBaseFileWriter &writer);
	/*@}*/
	
private:
	void pWriteConversation(decXmlWriter &writer, const ceConversation &conversation);
	void pWriteTarget(decXmlWriter &writer, const ceTarget &target);
	void pWriteCameraShot(decXmlWriter &writer, const ceCameraShot &cameraShot);
	void pWriteGesture(decXmlWriter &writer, const ceGesture &gesture);
	void pWriteFacePose(decXmlWriter &writer, const ceFacePose &facePose);
	void pWriteFile(decXmlWriter &writer, const ceConversationFile &file);
	void pWriteTopic(decXmlWriter &writer, const ceConversationTopic &topic);
	void pWriteActionList(decXmlWriter &writer, const ceConversationActionList &list);
	void pWriteActionCommon(decXmlWriter &writer, const ceConversationAction &action);
	void pWriteActionCameraShot(decXmlWriter &writer, const ceCACameraShot &action);
	void pWriteActionMusic(decXmlWriter &writer, const ceCAMusic &action);
	void pWriteActionActorSpeak(decXmlWriter &writer, const ceCAActorSpeak &action);
	void pWriteActionIfElse(decXmlWriter &writer, const ceCAIfElse &action);
	void pWriteActionIfElseCase(decXmlWriter &writer, const ceCAIfElseCase &ifcase);
	void pWriteActionPlayerChoice(decXmlWriter &writer, const ceCAPlayerChoice &action);
	void pWriteActionPlayerChoiceOption(decXmlWriter &writer, const ceCAPlayerChoiceOption &option);
	void pWriteActionStopConversation(decXmlWriter &writer, const ceCAStopConversation &action);
	void pWriteActionStopTopic(decXmlWriter &writer, const ceCAStopTopic &action);
	void pWriteActionSnippet(decXmlWriter &writer, const ceCASnippet &action);
	void pWriteActionSetVariable(decXmlWriter &writer, const ceCASetVariable &action);
	void pWriteActionSetActorParameter(decXmlWriter &writer, const ceCASetActorParameter &action);
	void pWriteActionActorCommand(decXmlWriter &writer, const ceCAActorCommand &action);
	void pWriteActionGameCommand(decXmlWriter &writer, const ceCAGameCommand &action);
	void pWriteActionWait(decXmlWriter &writer, const ceCAWait &action);
	void pWriteActionTrigger(decXmlWriter &writer, const ceCATrigger &action);
	void pWriteActionActorAdd(decXmlWriter &writer, const ceCAActorAdd &action);
	void pWriteActionActorRemove(decXmlWriter &writer, const ceCAActorRemove &action);
	void pWriteActionCoordSystemAdd(decXmlWriter &writer, const ceCACoordSystemAdd &action);
	void pWriteActionCoordSystemRemove(decXmlWriter &writer, const ceCACoordSystemRemove &action);
	void pWriteActionComment(decXmlWriter &writer, const ceCAComment &action);
	void pWriteStripList(decXmlWriter &writer, const ceStripList &list);
	void pWriteCurveBezier(decXmlWriter &writer, const decCurveBezier &curve);
	void pWriteCurveBezierPoint(decXmlWriter &writer, const decCurveBezierPoint &point);
	void pWriteCondition(decXmlWriter &writer, ceConversationCondition *condition);
	void pWriteConditionList(decXmlWriter &writer, const ceConversationConditionList &list);
	void pWriteConditionLogic(decXmlWriter &writer, const ceCConditionLogic &condition);
	void pWriteConditionHasActor(decXmlWriter &writer, const ceCConditionHasActor &condition);
	void pWriteConditionActorInConversation(decXmlWriter &writer, const ceCConditionActorInConversation &condition);
	void pWriteConditionVariable(decXmlWriter &writer, const ceCConditionVariable &condition);
	void pWriteConditionActorParameter(decXmlWriter &writer, const ceCConditionActorParameter &condition);
	void pWriteConditionActorCommand(decXmlWriter &writer, const ceCConditionActorCommand &condition);
	void pWriteConditionGameCommand(decXmlWriter &writer, const ceCConditionGameCommand &condition);
	void pWriteConditionTrigger(decXmlWriter &writer, const ceCConditionTrigger &condition);
	
	void pReadConversation(const decXmlElementTag &root, ceConversation &conversation);
	void pReadTarget(const decXmlElementTag &root, ceConversation &conversation);
	void pReadCameraShot(const decXmlElementTag &root, ceConversation &conversation);
	void pReadGesture(const decXmlElementTag &root, ceConversation &conversation);
	void pReadFacePose(const decXmlElementTag &root, ceConversation &conversation);
	void pReadFile(const decXmlElementTag &root, ceConversation &conversation);
	void pReadTopic(const decXmlElementTag &root, ceConversation &conversation, ceConversationFile &file);
	void pReadActionList(const decXmlElementTag &root, ceConversation &conversation, ceConversationActionList &list);
	ceConversationAction *pReadAction(const decXmlElementTag &root, ceConversation &conversation);
	bool pReadActionCommon(const decXmlElementTag &root, ceConversationAction &action);
	void pReadActionCameraShot(const decXmlElementTag &root, ceCACameraShot &action);
	void pReadActionMusic(const decXmlElementTag &root, ceCAMusic &action);
	void pReadActionActorSpeak(const decXmlElementTag &root, ceCAActorSpeak &action);
	void pReadActionIfElse(const decXmlElementTag &root, ceConversation &conversation, ceCAIfElse &action);
	void pReadActionIfElseCase(const decXmlElementTag &root, ceConversation &conversation, ceCAIfElse &action);
	void pReadActionPlayerChoice(const decXmlElementTag &root, ceConversation &conversation, ceCAPlayerChoice &action);
	void pReadActionPlayerChoiceOption(const decXmlElementTag &root, ceConversation &conversation, ceCAPlayerChoice &action);
	void pReadActionStopConversation(const decXmlElementTag &root, ceCAStopConversation &action);
	void pReadActionStopTopic(const decXmlElementTag &root, ceCAStopTopic &action);
	void pReadActionSnippet(const decXmlElementTag &root, ceCASnippet &action);
	void pReadActionSetVariable(const decXmlElementTag &root, ceCASetVariable &action);
	void pReadActionSetActorParameter(const decXmlElementTag &root, ceCASetActorParameter &action);
	void pReadActionActorCommand(const decXmlElementTag &root, ceCAActorCommand &action);
	void pReadActionGameCommand(const decXmlElementTag &root, ceCAGameCommand &action);
	void pReadActionWait(const decXmlElementTag &root, ceConversation &conversation, ceCAWait &action);
	void pReadActionTrigger(const decXmlElementTag &root, ceCATrigger &action);
	void pReadActionActorAdd(const decXmlElementTag &root, ceCAActorAdd &action);
	void pReadActionActorRemove(const decXmlElementTag &root, ceCAActorRemove &action);
	void pReadActionCoordSystemAdd(const decXmlElementTag &root, ceCACoordSystemAdd &action);
	void pReadActionCoordSystemRemove(const decXmlElementTag &root, ceCACoordSystemRemove &action);
	void pReadActionComment(const decXmlElementTag &root, ceCAComment &action);
	void pReadStripList(const decXmlElementTag &root, ceStripList &list);
	void pReadCurveBezier(const decXmlElementTag &root, decCurveBezier &curve);
	void pReadCurveBezierPoint(const decXmlElementTag &root, decCurveBezier &curve);
	void pReadVector(const decXmlElementTag &tag, decVector &vector);
	void pReadVector2(const decXmlElementTag &tag, decVector2 &vector);
	void pReadStringList(const decXmlElementTag &root, decStringList &list, const char *entryTagName);
	void pReadConditionList(const decXmlElementTag &root, ceConversation &conversation, ceConversationConditionList &list);
	ceConversationCondition *pReadCondition(const decXmlElementTag &root, ceConversation &conversation);
	void pReadConditionLogic(const decXmlElementTag &root, ceConversation &conversation, ceCConditionLogic &condition);
	void pReadConditionHasActor(const decXmlElementTag &root, ceCConditionHasActor &condition);
	void pReadConditionActorInConversation(const decXmlElementTag &root, ceCConditionActorInConversation &condition);
	void pReadConditionVariable(const decXmlElementTag &root, ceCConditionVariable &condition);
	void pReadConditionActorParameter(const decXmlElementTag &root, ceCConditionActorParameter &condition);
	void pReadConditionActorCommand(const decXmlElementTag &root, ceCConditionActorCommand &condition);
	void pReadConditionGameCommand(const decXmlElementTag &root, ceCConditionGameCommand &condition);
	void pReadConditionTrigger(const decXmlElementTag &root, ceCConditionTrigger &condition);
};

#endif
