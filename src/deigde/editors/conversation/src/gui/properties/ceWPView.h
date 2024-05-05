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

#ifndef _CEWPVIEW_H_
#define _CEWPVIEW_H_

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/igdeSpinTextFieldReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>
#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include <deigde/gui/properties/igdeWPCameraReference.h>
#include <deigde/gui/properties/igdeWPSkyReference.h>
#include <deigde/gui/properties/igdeWPWObjectReference.h>
#include <deigde/gui/properties/igdeWPTriggerTableReference.h>


class ceProp;
class ceCoordSystem;
class ceWindowProperties;
class ceConversationActor;
class ceActorPose;
class ceActorGesture;
class ceActorController;
class ceConversationFile;
class ceConversationTopic;

class saeGameDefinition;
class ceWPViewListener;
class ceConversation;



/**
 * View property window.
 */
class ceWPView : public igdeContainerScroll{
private:
	ceWindowProperties &pWindowProperties;
	ceWPViewListener *pListener;
	ceConversation *pConversation;
	
	igdeWPSkyReference pWPSky;
	igdeWPWObjectReference pWPEnvObject;
	igdeWPCameraReference pWPCamera;
	igdeWPCameraReference pWPCameraFree;
	
	igdeComboBoxReference pCBProps;
	igdeButtonReference pBtnPropMenu;
	igdeTextFieldReference pEditPropName;
	igdeTextFieldReference pEditPropClass;
	igdeButtonReference pBtnPropClass;
	igdeEditVectorReference pEditPropPos;
	igdeEditVectorReference pEditPropRot;
	igdeCheckBoxReference pChkPropVisible;
	
	igdeSpinTextFieldReference pSpinActor;
	igdeButtonReference pBtnActorMenu;
	igdeTextFieldReference pEditActorID;
	igdeTextFieldReference pEditActorAliasID;
	igdeEditVectorReference pEditActorPos;
	igdeEditVectorReference pEditActorOri;
	igdeEditPathReference pEditActorPathModel;
	igdeEditPathReference pEditActorPathSkin;
	igdeEditPathReference pEditActorPathRig;
	igdeEditPathReference pEditActorPathSpeechAnim;
	igdeEditPathReference pEditActorPathEyeAnimator;
	igdeEditPathReference pEditActorPathFaceAnimator;
	igdeTextFieldReference pEditActorBoneHeadRotator;
	igdeCheckBoxReference pChkActorWaiting;
	
	igdeComboBoxReference pCBActorPose;
	igdeButtonReference pBtnActorPoseMenu;
	igdeEditPathReference pEditActorPosePathAnimator;
	igdeComboBoxReference pCBActorPoseController;
	igdeButtonReference pBtnActorPoseControllerMenu;
	igdeComboBoxReference pCBActorPoseControllerUpdateType;
	igdeTextFieldReference pEditActorPoseControllerValue;
	igdeEditVectorReference pEditActorPoseControllerVector;
	
	igdeComboBoxReference pCBActorGesture;
	igdeButtonReference pBtnActorGestureMenu;
	igdeEditPathReference pEditActorGesturePathAnimator;
	
	igdeListBoxReference pListActorCommands;
	igdeListBoxReference pListActorParameters;
	
	igdeSpinTextFieldReference pSpinCoordSys;
	igdeButtonReference pBtnCoordSysMenu;
	igdeTextFieldReference pEditCoordSysID;
	igdeTextFieldReference pEditCoordSysAliasID;
	igdeEditVectorReference pEditCoordSysPosition;
	igdeEditVectorReference pEditCoordSysRotation;
	
	igdeComboBoxFilterReference pCBPlaybackFile;
	igdeComboBoxFilterReference pCBPlaybackTopic;
	igdeButtonReference pBtnPlaybackSelectTopic;
	igdeButtonReference pBtnPlaybackRewind;
	igdeCheckBoxReference pChkPlaybackRunning;
	igdeCheckBoxReference pChkPlaybackPaused;
	igdeCheckBoxReference pChkPlaybackAutoAdvanceCommands;
	igdeComboBoxReference pCBPlaybackCameraHandling;
	igdeListBoxReference pListPlaybackCommands;
	igdeListBoxReference pListPlaybackVars;
	
	igdeWPTriggerTableReference pWPPlaybackTriggerTable;
	
	igdeComboBoxReference pCBPlaybackMissingWords;
	igdeButtonReference pBtnPlaybackMissingWordsMenu;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create panel. */
	ceWPView( ceWindowProperties &windowProperties );
	
protected:
	/** Clean up panel. */
	virtual ~ceWPView();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Properties window. */
	inline ceWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** Conversation. */
	inline ceConversation *GetConversation() const{ return pConversation; }
	
	/** Set conversation. */
	void SetConversation( ceConversation *conversation );
	
	/** Update view. */
	void UpdateView();
	
	/** Update sky. */
	void UpdateSky();
	
	/** Update environment object. */
	void UpdateEnvObject();
	
	/** Update camera. */
	void UpdateCamera();
	
	/** Active prop. */
	ceProp *GetProp() const;
	
	/** Update prop list. */
	void UpdatePropList();
	
	/** Update prop. */
	void UpdateProp();
	
	/** Active actor. */
	ceConversationActor *GetActor() const;
	
	/** Active actor pose. */
	ceActorPose *GetActorPose() const;
	
	/** Active actor pose. */
	ceActorController *GetActorPoseController() const;
	
	/** Active actor gesture. */
	ceActorGesture *GetActorGesture() const;
	
	/** Update actors. */
	void UpdateActors();
	
	/** Update actor poses. */
	void UpdateActorPoses();
	
	/** Select active actor pose. */
	void SelectActiveActorPose();
	
	/** Update actor pose. */
	void UpdateActorPose();
	
	/** Update actor gestures. */
	void UpdateActorGestures();
	
	/** Select actor pose gesture. */
	void SelectActorPoseGesture( ceActorGesture *gesture );
	
	/** Update actor gesture. */
	void UpdateActorGesture();
	
	/** Update actor controllers. */
	void UpdateActorControllers();
	
	/** Update actor controller. */
	void UpdateActorController();
	
	/** Select active actor pose. */
	void SelectActorPoseController( ceActorController *controller );
	
	/** Update actor commands list. */
	void UpdateActorCommands();
	
	/** Update actor parameter list. */
	void UpdateActorParameters();
	
	/** Active coordinate system. */
	ceCoordSystem *GetCoordSys() const;
	
	/** Update coordinate systems. */
	void UpdateCoordSystems();
	
	/** Update playback file list. */
	void UpdatePlaybackFileList();
	
	/** Selected playback file. */
	ceConversationFile *GetSelectedPlaybackFile() const;
	
	/** Update playback topic list. */
	void UpdatePlaybackTopicList();
	
	/** Selected playback topic. */
	ceConversationTopic *GetSelectedPlaybackTopic() const;
	
	/** Update playback parameters. */
	void UpdatePlayback();
	
	/** Update playback commands list. */
	void UpdatePlaybackCommands();
	
	/** Update playback variables list. */
	void UpdatePlaybackVariables();
	
	/** Update playback trigger table. */
	void UpdatePlaybackTriggerTable();
	
	/** Update playback missing words. */
	void UpdatePlaybackMissingWords();
	/*@}*/
};

#endif
