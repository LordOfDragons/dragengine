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

#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeSpinTextField.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include <deigde/gui/properties/igdeWPCamera.h>
#include <deigde/gui/properties/igdeWPSky.h>
#include <deigde/gui/properties/igdeWPWObject.h>
#include <deigde/gui/properties/igdeWPTriggerTable.h>


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
	ceWPViewListener::Ref pListener;
	ceConversation::Ref pConversation;
	
	igdeWPSky::Ref pWPSky;
	igdeWPWObject::Ref pWPEnvObject;
	igdeWPCamera::Ref pWPCamera;
	igdeWPCamera::Ref pWPCameraFree;
	
	igdeComboBox::Ref pCBProps;
	igdeButton::Ref pBtnPropMenu;
	igdeTextField::Ref pEditPropName;
	igdeTextField::Ref pEditPropClass;
	igdeButton::Ref pBtnPropClass;
	igdeEditVector::Ref pEditPropPos;
	igdeEditVector::Ref pEditPropRot;
	igdeCheckBox::Ref pChkPropVisible;
	
	igdeSpinTextField::Ref pSpinActor;
	igdeButton::Ref pBtnActorMenu;
	igdeTextField::Ref pEditActorID;
	igdeTextField::Ref pEditActorAliasID;
	igdeEditVector::Ref pEditActorPos;
	igdeEditVector::Ref pEditActorOri;
	igdeEditPath::Ref pEditActorPathModel;
	igdeEditPath::Ref pEditActorPathSkin;
	igdeEditPath::Ref pEditActorPathRig;
	igdeEditPath::Ref pEditActorPathSpeechAnim;
	igdeEditPath::Ref pEditActorPathEyeAnimator;
	igdeEditPath::Ref pEditActorPathFaceAnimator;
	igdeTextField::Ref pEditActorBoneHeadRotator;
	igdeCheckBox::Ref pChkActorWaiting;
	
	igdeComboBox::Ref pCBActorPose;
	igdeButton::Ref pBtnActorPoseMenu;
	igdeEditPath::Ref pEditActorPosePathAnimator;
	igdeComboBox::Ref pCBActorPoseController;
	igdeButton::Ref pBtnActorPoseControllerMenu;
	igdeComboBox::Ref pCBActorPoseControllerUpdateType;
	igdeTextField::Ref pEditActorPoseControllerValue;
	igdeEditVector::Ref pEditActorPoseControllerVector;
	
	igdeComboBox::Ref pCBActorGesture;
	igdeButton::Ref pBtnActorGestureMenu;
	igdeEditPath::Ref pEditActorGesturePathAnimator;
	
	igdeListBox::Ref pListActorCommands;
	igdeListBox::Ref pListActorParameters;
	
	igdeSpinTextField::Ref pSpinCoordSys;
	igdeButton::Ref pBtnCoordSysMenu;
	igdeTextField::Ref pEditCoordSysID;
	igdeTextField::Ref pEditCoordSysAliasID;
	igdeEditVector::Ref pEditCoordSysPosition;
	igdeEditVector::Ref pEditCoordSysRotation;
	
	igdeComboBoxFilter::Ref pCBPlaybackFile;
	igdeComboBoxFilter::Ref pCBPlaybackTopic;
	igdeButton::Ref pBtnPlaybackSelectTopic;
	igdeButton::Ref pBtnPlaybackRewind;
	igdeCheckBox::Ref pChkPlaybackRunning;
	igdeCheckBox::Ref pChkPlaybackPaused;
	igdeCheckBox::Ref pChkPlaybackAutoAdvanceCommands;
	igdeComboBox::Ref pCBPlaybackCameraHandling;
	igdeListBox::Ref pListPlaybackCommands;
	igdeListBox::Ref pListPlaybackVars;
	
	igdeWPTriggerTable::Ref pWPPlaybackTriggerTable;
	
	igdeComboBox::Ref pCBPlaybackMissingWords;
	igdeButton::Ref pBtnPlaybackMissingWordsMenu;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create panel. */
	ceWPView(ceWindowProperties &windowProperties);
	
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
	inline const ceConversation::Ref &GetConversation() const{ return pConversation; }
	
	/** Set conversation. */
	void SetConversation(ceConversation *conversation);
	
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
	void SelectActorPoseGesture(ceActorGesture *gesture);
	
	/** Update actor gesture. */
	void UpdateActorGesture();
	
	/** Update actor controllers. */
	void UpdateActorControllers();
	
	/** Update actor controller. */
	void UpdateActorController();
	
	/** Select active actor pose. */
	void SelectActorPoseController(ceActorController *controller);
	
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
