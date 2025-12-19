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

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ceConversation.h"
#include "ceConversationListener.h"
#include "action/ceConversationAction.h"
#include "actor/ceConversationActor.h"
#include "camerashot/ceCameraShot.h"
#include "facepose/ceFacePose.h"
#include "gesture/ceGesture.h"
#include "file/ceConversationFile.h"
#include "target/ceTarget.h"
#include "topic/ceConversationTopic.h"
#include "textbox/ceTextBox.h"
#include "playerChoiceBox/cePlayerChoiceBox.h"
#include "playback/cePlayback.h"
#include "infobox/ceConversationInfoBox.h"
#include "coordsystem/ceCoordSystem.h"
#include "prop/ceProp.h"
#include "../loadsave/ceLoadSaveSystem.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gamedefinition/class/igdeGDClassManager.h>
#include <deigde/gui/igdeCamera.h>
#include <deigde/gui/wrapper/igdeWObject.h>
#include <deigde/gui/wrapper/igdeWSky.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationManager.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorManager.h>
#include <dragengine/resources/animator/deAnimatorInstance.h>
#include <dragengine/resources/animator/deAnimatorInstanceManager.h>
#include <dragengine/resources/animator/deAnimatorLink.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimation.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleStateManipulator.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentManager.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/sound/deMicrophone.h>
#include <dragengine/resources/sound/deMicrophoneManager.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/world/deWorldManager.h>
#include <dragengine/systems/deAudioSystem.h>
#include <dragengine/resources/sound/deSpeakerManager.h>
#include <dragengine/resources/sound/deSpeaker.h>



// Definitions
////////////////

#define LOGSOURCE "Speech Animator Editor"



// Class ceConversation
////////////////////////

// Constructor, destructor
////////////////////////////

ceConversation::ceConversation(igdeEnvironment *environment) : igdeEditableEntity(environment){
	deEngine * const engine = GetEngine();
	
	pSky = nullptr;
	
	pActiveTarget = nullptr;
	pActiveCameraShot = nullptr;
	pActiveGesture = nullptr;
	pActiveFacePose = nullptr;
	pActiveFile = nullptr;
	pActiveActor = nullptr;
	pActiveCoordSystem = nullptr;
	pActiveProp = nullptr;
	
	pCamera = nullptr;
	pCameraFree = nullptr;
	
	pScreenRatio = 1.6f; // 16:10 wide screen
	pShowRuleOfThirdsAid = false;
	
	pPasteSnippetDialogParams.delayCameraShot = 0.5f;
	pPasteSnippetDialogParams.delayActorSpeak = 0.5f;
	pPasteSnippetDialogParams.scaleActorSpeak = 1.0f;
	
	try{
		SetFilePath("new.deconvo");
		pCTSPath = "test.dects";
		pCTAPath = "test.decta";
		pCTFIPath = "test.dectfi";
		pLangPackPath = "/content/languages/english.delangpack";
		pLangPackEntryName = "convo.group.topic.s1";
		
		// create world
		pEngWorld = engine->GetWorldManager()->CreateWorld();
		pEngWorld->SetGravity(decVector(0.0f, 0.0f, 0.0f));
		pEngWorld->SetDisableLights(false);
		pEngWorld->SetAmbientLight(decColor(0.0f, 0.0f, 0.0f));
		
		// create camera
		pCamera = new igdeCamera(engine);
		
		pCamera->SetEngineWorld(pEngWorld);
		
		pCamera->Reset();
		pCamera->SetFov(90.0f);
		pCamera->SetHighestIntensity(20.0f);
		pCamera->SetLowestIntensity(20.0f);
		pCamera->SetAdaptionTime(0.0f);
		pCamera->SetDistance(0.0f);
		pCamera->SetOrientation(decVector(0.0f, 180.0f, 0.0f));
		pCamera->SetPosition(decDVector(0.0, 1.5, 5.0));
		
		// create free camera
		pCameraFree = new igdeCamera(engine);
		pCameraFree->Reset();
		pCameraFree->SetFov(90.0f);
		pCameraFree->SetHighestIntensity(20.0f);
		pCameraFree->SetLowestIntensity(20.0f);
		pCameraFree->SetAdaptionTime(0.0f);
		pCameraFree->SetDistance(0.0f);
		pCameraFree->SetOrientation(decVector(0.0f, 180.0f, 0.0f));
		pCameraFree->SetPosition(decDVector(0.0, 1.5, 5.0));
		
		// create microphone
		decLayerMask layerMaskAudio;
		layerMaskAudio.SetBit(elmAudio);
		
		pEngMicrophone = engine->GetMicrophoneManager()->CreateMicrophone();
		pEngMicrophone->SetMuted(false);
		pEngMicrophone->SetType(deMicrophone::emtPoint); // directed in fact but that's for later
		pEngMicrophone->SetLayerMask(layerMaskAudio);
		pEngWorld->AddMicrophone(pEngMicrophone);
		
		// create voice audio preview speaker
		pEngSpeakerVAPreview = engine->GetSpeakerManager()->CreateSpeaker();
		pEngSpeakerVAPreview->SetLooping(false);
		pEngSpeakerVAPreview->SetMuted(false);
		pEngSpeakerVAPreview->SetPlaySpeed(1.0f);
		pEngSpeakerVAPreview->SetLayerMask(layerMaskAudio);
		pEngMicrophone->AddSpeaker(pEngSpeakerVAPreview);
		
		// create sky
		
		pSky = new igdeWSky(*environment);
		pSky->SetGDDefaultSky();
		pSky->SetWorld(pEngWorld);
		
		// create the environment wrapper object
		pEnvObject = igdeWObject::Ref::New(*environment);
		pEnvObject->SetWorld(pEngWorld);
		pEnvObject->SetPosition(decDVector(0.0, 0.0, 0.0));
		
		decLayerMask layerMaskCollider;
		layerMaskCollider.SetBit(0);
		pEnvObject->SetCollisionFilter(decCollisionFilter(layerMaskCollider)); // just so something collides
		pEnvObject->SetCollisionFilterFallback(decCollisionFilter(layerMaskCollider)); // just so something collides
		
		pEnvObject->SetGDClassName("IGDETestTerrain");
		
		// create text box
		pTextBox = ceTextBox::Ref::New(*engine, *GetLogger());
		
		// create player choice box
		pPlayerChoiceBox = cePlayerChoiceBox::Ref::New(*this);
		
		// create info box
		pInfoBox = ceConversationInfoBox::Ref::New(*this);
		
		// Create playback
		pPlayback = cePlayback::Ref::New(*this);
		
	}catch(const deException &e){
		pCleanUp();
		e.PrintError();
		throw;
	}
}

ceConversation::~ceConversation(){
	pCleanUp();
}



// Management
///////////////

void ceConversation::Dispose(){
	RemoveAllProps();
	RemoveAllCoordSystems();
	RemoveAllActors();
	RemoveAllFiles();
	RemoveAllFacePoses();
	RemoveAllGestures();
	RemoveAllCameraShots();
	RemoveAllTargets();
	
	pImportedConversations.Visit([](ceConversation &c){
		c.Dispose();
	});
	pImportedConversations.RemoveAll();
	
	GetUndoSystem()->RemoveAll();
}



void ceConversation::SetScreenRatio(float ratio){
	if(fabsf(ratio - pScreenRatio) > 1e-5f){
		pScreenRatio = ratio;
		NotifyConversationChanged();
	}
}

void ceConversation::SetShowRuleOfThirdsAid(bool showRuleOfThirdsAid){
	if(showRuleOfThirdsAid != pShowRuleOfThirdsAid){
		pShowRuleOfThirdsAid = showRuleOfThirdsAid;
		NotifyViewChanged();
	}
}



void ceConversation::Reset(){
	GetUndoSystem()->RemoveAll();
}

void ceConversation::ActivateMicrophone(){
	GetEngine()->GetAudioSystem()->SetActiveMicrophone(pEngMicrophone);
}

void ceConversation::Update(float elapsed){
	pPlayback->Update(elapsed);
	
	const int count = pActorList.GetCount();
	int i;
	for(i=0; i<count; i++){
		pActorList.GetAt(i)->Update(pPlayback, elapsed);
	}
	
	pEnvObject->Update(elapsed);
	
	pEngWorld->Update(elapsed);
}



void ceConversation::SetCTSPath(const char *path){
	pCTSPath = path;
}

void ceConversation::SetCTAPath(const char *path){
	pCTAPath = path;
}

void ceConversation::SetCTFIPath(const char *path){
	pCTFIPath = path;
}

void ceConversation::SetLangPackPath(const char *path){
	pLangPackPath = path;
}

void ceConversation::SetLangPackEntryName(const char *name){
	pLangPackEntryName = name;
}

void ceConversation::SetLanguagePack(ceLangPack *langpack){
	if(langpack == pLangPack){
		return;
	}
	
	pLangPack = langpack;
	NotifyLanguagePackChanged();
}



// Imported conversations
///////////////////////////

void ceConversation::SetImportConversationPath(const decStringList &list){
	pImportConversationPath = list;
}

void ceConversation::UpdateImportedConversations(ceLoadSaveSystem &lssystem){
	// we reload all conversation to be on the save side. this is less efficient than keeping
	// alread loaded conversations around but avoids potential problems if files changed
	pImportedConversations.Visit([](ceConversation &c){
		c.Dispose();
	});
	pImportedConversations.RemoveAll();
	
	const int count = pImportConversationPath.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		try{
			pImportedConversations.Add(lssystem.LoadConversation(decPath::AbsolutePathUnix(
				pImportConversationPath.GetAt(i), GetDirectoryPath()).GetPathUnix()));
			
		}catch(const deException &e){
			GetLogger()->LogException(LOGSOURCE, e);
			// ignore unusable files
		}
	}
}



// Targets
////////////

void ceConversation::AddTarget(ceTarget *target){
	DEASSERT_NOTNULL(target)
	DEASSERT_FALSE(pTargets.HasMatching([&](const ceTarget &t){ return t.GetName() == target->GetName(); }))
	DEASSERT_NULL(target->GetConversation())
	
	pTargets.Add(target);
	target->SetConversation(this);
	NotifyTargetStructureChanged();
	
	if(!pActiveTarget){
		SetActiveTarget(target);
	}
}

void ceConversation::RemoveTarget(ceTarget *target){
	if(!target || !pTargets.Has(target)){
		DETHROW(deeInvalidParam);
	}
	
	if(target == pActiveTarget){
		if(pTargets.GetCount() == 1){
			SetActiveTarget(nullptr);
			
		}else{
			if(pTargets.First() == target){
				SetActiveTarget(pTargets.GetAt(1));
				
			}else{
				SetActiveTarget(pTargets.First());
			}
		}
	}
	
	target->SetConversation(nullptr);
	pTargets.Remove(target);
	NotifyTargetStructureChanged();
}

void ceConversation::RemoveAllTargets(){
	SetActiveTarget(nullptr);
	
	pTargets.Visit([](ceTarget &t){ t.SetConversation(nullptr); });
	pTargets.RemoveAll();
	NotifyTargetStructureChanged();
}

void ceConversation::SetActiveTarget(ceTarget *target){
	if(target != pActiveTarget){
		pActiveTarget = target;
		NotifyActiveTargetChanged();
	}
}

ceTarget *ceConversation::GetTargetNamed(const char *name) const{
	ceTarget *target = pTargets.FindOrDefault([&](const ceTarget &t){ return t.GetName() == name; });
	if(!target){
		pImportedConversations.FindReverseOrDefault([&](const ceConversation &c){
			return target = c.GetTargetNamed(name);
		});
	}
	return target;
}

ceTarget::List ceConversation::AllTargets() const{
	ceTarget::List list;
	AllTargets(list);
	return list;
}

void ceConversation::AllTargets(ceTarget::List &list) const{
	list += pTargets;
	pImportedConversations.VisitReverse([&](const ceConversation &c){
		c.AllTargets(list);
	});
}



// Camera Shots
/////////////////

void ceConversation::AddCameraShot(ceCameraShot *cameraShot){
	DEASSERT_NOTNULL(cameraShot)
	DEASSERT_FALSE(pCameraShots.HasMatching([&](const ceCameraShot &c){ return c.GetName() == cameraShot->GetName(); }))
	DEASSERT_NULL(cameraShot->GetConversation())
	
	pCameraShots.Add(cameraShot);
	cameraShot->SetConversation(this);
	NotifyCameraShotStructureChanged();
	
	if(!pActiveCameraShot){
		SetActiveCameraShot(cameraShot);
	}
}

void ceConversation::RemoveCameraShot(ceCameraShot *cameraShot){
	const ceCameraShot::Ref guard(cameraShot);
	pCameraShots.Remove(cameraShot);
	
	if(cameraShot == pActiveCameraShot){
		if(pCameraShots.GetCount() == 1){
			SetActiveCameraShot(nullptr);
			
		}else{
			if(pCameraShots.First() == cameraShot){
				SetActiveCameraShot(pCameraShots.GetAt(1));
				
			}else{
				SetActiveCameraShot(pCameraShots.First());
			}
		}
	}
	
	cameraShot->SetConversation(nullptr);
	pCameraShots.Remove(cameraShot);
	NotifyCameraShotStructureChanged();
}

void ceConversation::RemoveAllCameraShots(){
	SetActiveCameraShot(nullptr);
	
	pCameraShots.Visit([](ceCameraShot &c){ c.SetConversation(nullptr); });
	pCameraShots.RemoveAll();
	NotifyCameraShotStructureChanged();
}

void ceConversation::SetActiveCameraShot(ceCameraShot *cameraShot){
	if(cameraShot != pActiveCameraShot){
		pActiveCameraShot = cameraShot;
		NotifyActiveCameraShotChanged();
	}
}

ceCameraShot *ceConversation::GetCameraShotNamed(const char *name) const{
	ceCameraShot *cameraShot = pCameraShots.FindOrDefault([&](const ceCameraShot &c){ return c.GetName() == name; });
	if(!cameraShot){
		pImportedConversations.FindReverseOrDefault([&](const ceConversation &c){
			return cameraShot = c.GetCameraShotNamed(name);
		});
	}
	return cameraShot;
}

ceConversation::CameraShotList ceConversation::GetAllCameraShots() const{
	CameraShotList list;
	GetAllCameraShots(list);
	return list;
}

void ceConversation::GetAllCameraShots(CameraShotList &list) const{
	list += pCameraShots;
	pImportedConversations.VisitReverse([&](const ceConversation &c){
		c.GetAllCameraShots(list);
	});
}



// Gestures
/////////////

void ceConversation::AddGesture(ceGesture *gesture){
	DEASSERT_NOTNULL(gesture)
	DEASSERT_FALSE(pGestures.HasMatching([&](const ceGesture &g){ return g.GetName() == gesture->GetName(); }))
	DEASSERT_NULL(gesture->GetConversation())
	
	pGestures.Add(gesture);
	gesture->SetConversation(this);
	NotifyGestureStructureChanged();
	
	if(!pActiveGesture){
		SetActiveGesture(gesture);
	}
}

void ceConversation::RemoveGesture(ceGesture *gesture){
	const ceGesture::Ref guard(gesture);
	pGestures.Remove(gesture);
	
	if(gesture == pActiveGesture){
		if(pGestures.GetCount() == 1){
			SetActiveGesture(nullptr);
			
		}else{
			if(pGestures.First() == gesture){
				SetActiveGesture(pGestures.GetAt(1));
				
			}else{
				SetActiveGesture(pGestures.First());
			}
		}
	}
	
	gesture->SetConversation(nullptr);
	NotifyGestureStructureChanged();
}

void ceConversation::RemoveAllGestures(){
	SetActiveGesture(nullptr);
	
	pGestures.Visit([](ceGesture &g){ g.SetConversation(nullptr); });
	pGestures.RemoveAll();
	NotifyGestureStructureChanged();
}

void ceConversation::SetActiveGesture(ceGesture *gesture){
	if(gesture != pActiveGesture){
		pActiveGesture = gesture;
		NotifyActiveGestureChanged();
	}
}

ceGesture *ceConversation::GetGestureNamed(const char *name) const{
	ceGesture *gesture = pGestures.FindOrDefault([&](const ceGesture &g){ return g.GetName() == name; });
	if(!gesture){
		pImportedConversations.FindReverseOrDefault([&](const ceConversation &c){
			return gesture = c.GetGestureNamed(name);
		});
	}
	return gesture;
}

ceGesture::List ceConversation::AllGestures() const{
	ceGesture::List list;
	AllGestures(list);
	return list;
}

void ceConversation::AllGestures(ceGesture::List &list) const{
	list += pGestures;
	pImportedConversations.VisitReverse([&](const ceConversation &c){
		c.AllGestures(list);
	});
}



// Face Poses
//////////////

void ceConversation::AddFacePose(ceFacePose *facePose){
	DEASSERT_NOTNULL(facePose)
	DEASSERT_FALSE(pFacePoses.HasMatching([&](const ceFacePose &f){ return f.GetName() == facePose->GetName(); }))
	DEASSERT_NULL(facePose->GetConversation())
	
	pFacePoses.Add(facePose);
	facePose->SetConversation(this);
	NotifyFacePoseStructureChanged();
	
	if(!pActiveFacePose){
		SetActiveFacePose(facePose);
	}
}

void ceConversation::RemoveFacePose(ceFacePose *facePose){
	const ceFacePose::Ref guard(facePose);
	pFacePoses.Remove(facePose);
	
	if(facePose == pActiveFacePose){
		if(pFacePoses.GetCount() == 1){
			SetActiveFacePose(nullptr);
			
		}else{
			if(pFacePoses.First() == facePose){
				SetActiveFacePose(pFacePoses.GetAt(1));
				
			}else{
				SetActiveFacePose(pFacePoses.First());
			}
		}
	}
	
	facePose->SetConversation(nullptr);
	NotifyFacePoseStructureChanged();
}

void ceConversation::RemoveAllFacePoses(){
	SetActiveFacePose(nullptr);
	
	pFacePoses.Visit([](ceFacePose &f){ f.SetConversation(nullptr); });
	pFacePoses.RemoveAll();
	NotifyFacePoseStructureChanged();
}

void ceConversation::SetActiveFacePose(ceFacePose *facePose){
	if(facePose != pActiveFacePose){
		pActiveFacePose = facePose;
		NotifyActiveFacePoseChanged();
	}
}

ceFacePose *ceConversation::GetFacePoseNamed(const char *name) const{
	ceFacePose *facePose = pFacePoses.FindOrDefault([&](const ceFacePose &f){ return f.GetName() == name; });
	if(!facePose){
		pImportedConversations.FindReverseOrDefault([&](const ceConversation &c){
			return facePose = c.GetFacePoseNamed(name);
		});
	}
	return facePose;
}

ceFacePose::List ceConversation::AllFacePoses() const{
	ceFacePose::List list;
	AllFacePoses(list);
	return list;
}

void ceConversation::AllFacePoses(ceFacePose::List &list) const{
	list += pFacePoses;
	pImportedConversations.VisitReverse([&](const ceConversation &c){
		c.AllFacePoses(list);
	});
}



// Files
//////////

void ceConversation::AddFile(ceConversationFile *file){
	DEASSERT_NOTNULL(file)
	DEASSERT_FALSE(pFiles.HasMatching([&](const ceConversationFile &f){ return f.GetID() == file->GetID(); }))
	DEASSERT_NULL(file->GetConversation())
	
	pFiles.Add(file);
	file->SetConversation(this);
	NotifyFileStructureChanged();
	
	if(!pActiveFile){
		SetActiveFile(file);
	}
}

void ceConversation::RemoveFile(ceConversationFile *file){
	const ceConversationFile::Ref guard(file);
	pFiles.Remove(file);
	
	if(file == pActiveFile){
		if(pFiles.GetCount() == 1){
			SetActiveFile(nullptr);
			
		}else{
			if(pFiles.First() == file){
				SetActiveFile(pFiles.GetAt(1));
				
			}else{
				SetActiveFile(pFiles.First());
			}
		}
	}
	
	file->SetConversation(nullptr);
	NotifyFileStructureChanged();
}

void ceConversation::RemoveAllFiles(){
	SetActiveFile(nullptr);
	
	pFiles.Visit([](ceConversationFile &f){ f.SetConversation(nullptr); });
	pFiles.RemoveAll();
	NotifyFileStructureChanged();
}

void ceConversation::SetActiveFile(ceConversationFile *file){
	if(file == pActiveFile){
		return;
	}
	pActiveFile = file;
	NotifyActiveFileChanged();
}

ceConversationFile *ceConversation::GetFileWithID(const char *id) const{
	ceConversationFile *file = pFiles.FindOrDefault([&](const ceConversationFile &f){ return f.GetID() == id; });
	if(!file){
		pImportedConversations.FindReverseOrDefault([&](const ceConversation &c){
			return file = c.GetFileWithID(id);
		});
	}
	return file;
}

ceConversationFile::List ceConversation::AllFiles() const{
	ceConversationFile::List list;
	AllFiles(list);
	return list;
}

void ceConversation::AllFiles(ceConversationFile::List &list) const{
	list += pFiles;
	pImportedConversations.VisitReverse([&](const ceConversation &c){
		c.AllFiles(list);
	});
}

ceConversationTopic *ceConversation::GetTopicWithID(const char * fileName, const char *topicName) const{
	ceConversationTopic *topic = nullptr;
	ceConversationFile *file = pFiles.FindOrDefault([&](const ceConversationFile &f){ return f.GetID() == fileName; });
	if(file){
		topic = file->GetTopics().FindOrDefault( [&](const ceConversationTopic &t){ return t.GetID() == topicName; });
	}
	if(!topic){
		pImportedConversations.FindReverseOrDefault([&](const ceConversation &c){
			return topic = c.GetTopicWithID(fileName, topicName);
		});
	}
	return topic;
}

ceConversationTopic::List ceConversation::AllTopics(const char *fileName) const{
	ceConversationTopic::List list;
	AllTopics(fileName, list);
	return list;
}

void ceConversation::AllTopics(const char *fileName, ceConversationTopic::List &list) const{
	ceConversationFile *file = pFiles.FindOrDefault([&](const ceConversationFile &f){ return f.GetID() == fileName; });
	if(file){
		list += file->GetTopics();
	}
	pImportedConversations.VisitReverse([&](const ceConversation &c){
		c.AllTopics(fileName, list);
	});
}



// Actors
///////////

void ceConversation::AddActor(ceConversationActor *actor){
	pActorList.Add(actor);
	actor->SetConversation(this);
	NotifyActorStructureChanged();
	
	if(!pActiveActor){
		SetActiveActor(actor);
	}
}

void ceConversation::RemoveActor(ceConversationActor *actor){
	if(!actor || actor->GetConversation() != this) DETHROW(deeInvalidParam);
	
	if(actor == pActiveActor){
		if(pActorList.GetCount() == 1){
			SetActiveActor(nullptr);
			
		}else{
			if(pActorList.GetAt(0) == actor){
				SetActiveActor(pActorList.GetAt(1));
				
			}else{
				SetActiveActor(pActorList.GetAt(0));
			}
		}
	}
	
	actor->SetConversation(nullptr);
	pActorList.Remove(actor);
	NotifyActorStructureChanged();
}

void ceConversation::RemoveAllActors(){
	const int count = pActorList.GetCount();
	int i;
	
	SetActiveActor(nullptr);
	
	for(i=0; i<count; i++){
		pActorList.GetAt(i)->SetConversation(nullptr);
	}
	pActorList.RemoveAll();
	NotifyActorStructureChanged();
}

bool ceConversation::HasActiveActor() const{
	return pActiveActor != nullptr;
}

void ceConversation::SetActiveActor(ceConversationActor *actor){
	if(actor != pActiveActor){
		pActiveActor = actor;
		NotifyActiveActorChanged();
	}
}



// Coordinate systems
///////////////////////

void ceConversation::AddCoordSystem(ceCoordSystem *coordSystem){
	pCoordSystemList.Add(coordSystem);
	coordSystem->SetConversation(this);
	NotifyCoordSystemStructureChanged();
	
	if(!pActiveCoordSystem){
		SetActiveCoordSystem(coordSystem);
	}
}

void ceConversation::RemoveCoordSystem(ceCoordSystem *coordSystem){
	if(!coordSystem || coordSystem->GetConversation() != this){
		DETHROW(deeInvalidParam);
	}
	
	if(coordSystem == pActiveCoordSystem){
		if(pCoordSystemList.GetCount() == 1){
			SetActiveCoordSystem(nullptr);
			
		}else{
			if(pCoordSystemList.GetAt(0) == coordSystem){
				SetActiveCoordSystem(pCoordSystemList.GetAt(1));
				
			}else{
				SetActiveCoordSystem(pCoordSystemList.GetAt(0));
			}
		}
	}
	
	coordSystem->SetConversation(nullptr);
	pCoordSystemList.Remove(coordSystem);
	NotifyCoordSystemStructureChanged();
}

void ceConversation::RemoveAllCoordSystems(){
	const int count = pCoordSystemList.GetCount();
	int i;
	
	SetActiveCoordSystem(nullptr);
	
	for(i=0; i<count; i++){
		pCoordSystemList.GetAt(i)->SetConversation(nullptr);
	}
	pCoordSystemList.RemoveAll();
	NotifyCoordSystemStructureChanged();
}

bool ceConversation::HasActiveCoordSystem() const{
	return pActiveCoordSystem != nullptr;
}

void ceConversation::SetActiveCoordSystem(ceCoordSystem *coordSystem){
	if(coordSystem != pActiveCoordSystem){
		pActiveCoordSystem = coordSystem;
		NotifyActiveCoordSystemChanged();
	}
}



// Props
//////////

void ceConversation::AddProp(ceProp *prop){
	pProps.Add(prop);
	prop->SetConversation(this);
	NotifyPropStructureChanged();
	
	if(!pActiveProp){
		SetActiveProp(prop);
	}
}

void ceConversation::RemoveProp(ceProp *prop){
	const ceProp::Ref guard(prop);
	pProps.Remove(prop);
	
	if(prop == pActiveProp){
		if(pProps.GetCount() == 1){
			SetActiveProp(nullptr);
			
		}else{
			if(pProps.First() == prop){
				SetActiveProp(pProps.GetAt(1));
				
			}else{
				SetActiveProp(pProps.First());
			}
		}
	}
	
	prop->SetConversation(nullptr);
	NotifyPropStructureChanged();
}

void ceConversation::RemoveAllProps(){
	SetActiveProp(nullptr);
	
	pProps.Visit([](ceProp &p){ p.SetConversation(nullptr); });
	pProps.RemoveAll();
	NotifyPropStructureChanged();
}

bool ceConversation::HasActiveProp() const{
	return pActiveProp != nullptr;
}

void ceConversation::SetActiveProp(ceProp *prop){
	if(prop != pActiveProp){
		pActiveProp = prop;
		NotifyActivePropChanged();
	}
}



// Listeners
//////////////

void ceConversation::AddListener(ceConversationListener *listener){
	DEASSERT_NOTNULL(listener)
	pListeners.Add(listener);
}

void ceConversation::RemoveListener(ceConversationListener *listener){
	pListeners.RemoveIfPresent(listener);
}



void ceConversation::NotifyStateChanged(){
	pListeners.Visit([&](ceConversationListener &l){ l.StateChanged(this); });
}

void ceConversation::NotifyUndoChanged(){
	pListeners.Visit([&](ceConversationListener &l){
		l.UndoChanged(this);
	});
}

void ceConversation::NotifyViewChanged(){
	pListeners.Visit([&](ceConversationListener &l){
		l.ViewChanged(this);
	});
}

void ceConversation::NotifyConversationChanged(){
	pListeners.Visit([&](ceConversationListener &l){
		l.ConversationChanged(this);
	});
	
	SetChanged(true);
}

void ceConversation::NotifySkyChanged(){
	pListeners.Visit([&](ceConversationListener &l){
		l.SkyChanged(this);
	});
}

void ceConversation::NotifyEnvObjectChanged(){
	pListeners.Visit([&](ceConversationListener &l){
		l.EnvObjectChanged(this);
	});
}

void ceConversation::NotifyCameraChanged(){
	pListeners.Visit([&](ceConversationListener &l){
		l.CameraChanged(this);
	});
	
	pEngMicrophone->SetPosition(pCamera->GetPosition());
	pEngMicrophone->SetOrientation(pCamera->GetViewMatrix().ToQuaternion());
}



void ceConversation::NotifyTargetStructureChanged(){
	pListeners.Visit([&](ceConversationListener &l){
		l.TargetStructureChanged(this);
	});
	
	SetChanged(true);
}

void ceConversation::NotifyTargetChanged(ceTarget *target){
	pListeners.Visit([&](ceConversationListener &l){
		l.TargetChanged(this, target);
	});
	
	SetChanged(true);
}

void ceConversation::NotifyActiveTargetChanged(){
	pListeners.Visit([&](ceConversationListener &l){
		l.ActiveTargetChanged(this);
	});
}



void ceConversation::NotifyCameraShotStructureChanged(){
	pListeners.Visit([&](ceConversationListener &l){
		l.CameraShotStructureChanged(this);
	});
	
	SetChanged(true);
}

void ceConversation::NotifyCameraShotChanged(ceCameraShot *cameraShot){
	pListeners.Visit([&](ceConversationListener &l){
		l.CameraShotChanged(this, cameraShot);
	});
	
	SetChanged(true);
}

void ceConversation::NotifyActiveCameraShotChanged(){
	pListeners.Visit([&](ceConversationListener &l){
		l.ActiveCameraShotChanged(this);
	});
}



void ceConversation::NotifyGestureStructureChanged(){
	pListeners.Visit([&](ceConversationListener &l){
		l.GestureStructureChanged(this);
	});
	
	SetChanged(true);
}

void ceConversation::NotifyGestureChanged(ceGesture *gesture){
	pListeners.Visit([&](ceConversationListener &l){
		l.GestureChanged(this, gesture);
	});
	
	SetChanged(true);
}

void ceConversation::NotifyActiveGestureChanged(){
	pListeners.Visit([&](ceConversationListener &l){
		l.ActiveGestureChanged(this);
	});
}



void ceConversation::NotifyFacePoseControllerNamesChanged(){
	pListeners.Visit([&](ceConversationListener &l){
		l.FacePoseControllerNamesChanged(this);
	});
	
	SetChanged(true);
}

void ceConversation::NotifyFacePoseStructureChanged(){
	pListeners.Visit([&](ceConversationListener &l){
		l.FacePoseStructureChanged(this);
	});
	
	SetChanged(true);
}

void ceConversation::NotifyFacePoseChanged(ceFacePose *facePose){
	pListeners.Visit([&](ceConversationListener &l){
		l.FacePoseChanged(this, facePose);
	});
	
	SetChanged(true);
}

void ceConversation::NotifyActiveFacePoseChanged(){
	pListeners.Visit([&](ceConversationListener &l){
		l.ActiveFacePoseChanged(this);
	});
}



void ceConversation::NotifyFileStructureChanged(){
	pListeners.Visit([&](ceConversationListener &l){
		l.FileStructureChanged(this);
	});
	
	SetChanged(true);
}

void ceConversation::NotifyFileChanged(ceConversationFile *file){
	pListeners.Visit([&](ceConversationListener &l){
		l.FileChanged(this, file);
	});
	
	SetChanged(true);
}

void ceConversation::NotifyActiveFileChanged(){
	pListeners.Visit([&](ceConversationListener &l){
		l.ActiveFileChanged(this);
	});
}



void ceConversation::NotifyTopicStructureChanged(ceConversationFile *file){
	pListeners.Visit([&](ceConversationListener &l){
		l.TopicStructureChanged(this, file);
	});
	
	SetChanged(true);
}

void ceConversation::NotifyTopicChanged(ceConversationFile *file, ceConversationTopic *topic){
	pListeners.Visit([&](ceConversationListener &l){
		l.TopicChanged(this, file, topic);
	});
	
	SetChanged(true);
}

void ceConversation::NotifyActiveTopicChanged(ceConversationFile *file){
	pListeners.Visit([&](ceConversationListener &l){
		l.ActiveTopicChanged(this, file);
	});
}



void ceConversation::NotifyActionStructureChanged(ceConversationFile *file, ceConversationTopic *topic, ceConversationAction *action){
	pListeners.Visit([&](ceConversationListener &l){
		l.ActionStructureChanged(this, file, topic, action);
	});
	
	SetChanged(true);
}

void ceConversation::NotifyActionChanged(ceConversationFile *file, ceConversationTopic *topic, ceConversationAction *action){
	pListeners.Visit([&](ceConversationListener &l){
		l.ActionChanged(this, file, topic, action);
	});
	
	SetChanged(true);
}

void ceConversation::NotifyConditionStructureChanged(ceConversationFile *file,
ceConversationTopic *topic, ceConversationAction *action){
	pListeners.Visit([&](ceConversationListener &l){
		l.ConditionStructureChanged(this, file, topic, action);
	});
	
	SetChanged(true);
}

void ceConversation::NotifyConditionChanged(ceConversationFile *file, ceConversationTopic *topic,
ceConversationAction *action, ceConversationCondition *condition){
	pListeners.Visit([&](ceConversationListener &l){
		l.ConditionChanged(this, file, topic, action, condition);
	});
	
	SetChanged(true);
}

void ceConversation::NotifyActiveChanged(ceConversationFile *file, ceConversationTopic *topic){
	pListeners.Visit([&](ceConversationListener &l){
		l.ActiveChanged(this, file, topic);
	});
}



void ceConversation::NotifyActorStructureChanged(){
	pPlayback->SetActorCount(pActorList.GetCount());
	
	pListeners.Visit([&](ceConversationListener &l){
		l.ActorStructureChanged(this);
	});
}

void ceConversation::NotifyActorChanged(ceConversationActor *actor){
	pListeners.Visit([&](ceConversationListener &l){
		l.ActorChanged(this, actor);
	});
}

void ceConversation::NotifyActorActivePoseChanged(ceConversationActor *actor){
	pListeners.Visit([&](ceConversationListener &l){
		l.ActorActivePoseChanged(this, actor);
	});
}

void ceConversation::NotifyActorCommandsChanged(ceConversationActor *actor){
	pListeners.Visit([&](ceConversationListener &l){
		l.ActorCommandsChanged(this, actor);
	});
}

void ceConversation::NotifyActorPosesChanged(ceConversationActor *actor){
	pListeners.Visit([&](ceConversationListener &l){
		l.ActorPosesChanged(this, actor);
	});
}

void ceConversation::NotifyActorPoseGesturesChanged(ceConversationActor *actor, ceActorPose *pose){
	pListeners.Visit([&](ceConversationListener &l){
		l.ActorPoseGesturesChanged(this, actor, pose);
	});
}

void ceConversation::NotifyActorParametersChanged(ceConversationActor *actor){
	pListeners.Visit([&](ceConversationListener &l){
		l.ActorParametersChanged(this, actor);
	});
}

void ceConversation::NotifyActiveActorChanged(){
	pListeners.Visit([&](ceConversationListener &l){
		l.ActiveActorChanged(this);
	});
}



void ceConversation::NotifyCoordSystemStructureChanged(){
	pListeners.Visit([&](ceConversationListener &l){
		l.CoordSystemStructureChanged(this);
	});
}

void ceConversation::NotifyCoordSystemChanged(ceCoordSystem *coordSystem){
	pListeners.Visit([&](ceConversationListener &l){
		l.CoordSystemChanged(this, coordSystem);
	});
}

void ceConversation::NotifyActiveCoordSystemChanged(){
	pListeners.Visit([&](ceConversationListener &l){
		l.ActiveCoordSystemChanged(this);
	});
}



void ceConversation::NotifyPropStructureChanged(){
	pListeners.Visit([&](ceConversationListener &l){
		l.PropStructureChanged(this);
	});
}

void ceConversation::NotifyPropChanged(ceProp *prop){
	pListeners.Visit([&](ceConversationListener &l){
		l.PropChanged(this, prop);
	});
}

void ceConversation::NotifyActivePropChanged(){
	pListeners.Visit([&](ceConversationListener &l){
		l.ActivePropChanged(this);
	});
}



void ceConversation::NotifyPlaybackChanged(){
	pListeners.Visit([&](ceConversationListener &l){
		l.PlaybackChanged(this);
	});
}

void ceConversation::NotifyPlaybackCommandListChanged(){
	pListeners.Visit([&](ceConversationListener &l){
		l.PlaybackCommandListChanged(this);
	});
}

void ceConversation::NotifyPlaybackVarListChanged(){
	pListeners.Visit([&](ceConversationListener &l){
		l.PlaybackVarListChanged(this);
	});
}

void ceConversation::NotifyPlaybackTriggerTableChanged(){
	pListeners.Visit([&](ceConversationListener &l){
		l.PlaybackTriggerTableChanged(this);
	});
}

void ceConversation::NotifyPlaybackMissingWordsChanged(){
	pListeners.Visit([&](ceConversationListener &l){
		l.PlaybackMissingWordsChanged(this);
	});
}

void ceConversation::NotifyLanguagePackChanged(){
	pListeners.Visit([&](ceConversationListener &l){
		l.LanguagePackChanged(this);
	});
}



// Private Functions
//////////////////////

void ceConversation::pCleanUp(){
	pListeners.RemoveAll();
	
	Dispose();
	pPlayback = nullptr;
	
	if(pSky){
		delete pSky;
	}
	pEnvObject = nullptr;
	RemoveAllProps();
	
	if(pEngSpeakerVAPreview && pEngMicrophone){
		pEngMicrophone->RemoveSpeaker(pEngSpeakerVAPreview);
	}
	if(pEngMicrophone){
		if(GetEngine()->GetAudioSystem()->GetActiveMicrophone() == pEngMicrophone){
			GetEngine()->GetAudioSystem()->SetActiveMicrophone(nullptr);
		}
		if(pEngWorld){
			pEngWorld->RemoveMicrophone(pEngMicrophone);
		}
	}
	if(pCameraFree){
		delete pCameraFree;
	}
	if(pCamera){
		delete pCamera;
	}
}
