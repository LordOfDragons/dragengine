/* 
 * Drag[en]gine IGDE Conversation Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
class ceConversationFile;
class ceConversationTopic;

class saeGameDefinition;
class ceWPViewListener;
class ceConversation;



/**
 * \brief View property window.
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
	igdeComboBoxReference pCBPlaybackCameraHandling;
	igdeListBoxReference pListPlaybackCommands;
	igdeListBoxReference pListPlaybackVars;
	
	igdeWPTriggerTableReference pWPPlaybackTriggerTable;
	
	igdeComboBoxReference pCBPlaybackMissingWords;
	igdeButtonReference pBtnPlaybackMissingWordsMenu;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	ceWPView( ceWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up panel. */
	virtual ~ceWPView();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Conversation. */
	inline ceConversation *GetConversation() const{ return pConversation; }
	
	/** \brief Set conversation. */
	void SetConversation( ceConversation *conversation );
	
	/** \brief Update view. */
	void UpdateView();
	
	/** \brief Update sky. */
	void UpdateSky();
	
	/** \brief Update environment object. */
	void UpdateEnvObject();
	
	/** \brief Update camera. */
	void UpdateCamera();
	
	/** \brief Active prop. */
	ceProp *GetProp() const;
	
	/** \brief Update prop list. */
	void UpdatePropList();
	
	/** \brief Update prop. */
	void UpdateProp();
	
	/** \brief Active actor. */
	ceConversationActor *GetActor() const;
	
	/** \brief Active actor pose. */
	ceActorPose *GetActorPose() const;
	
	/** \brief Active actor gesture. */
	ceActorGesture *GetActorGesture() const;
	
	/** \brief Update actors. */
	void UpdateActors();
	
	/** \brief Update actor poses. */
	void UpdateActorPoses();
	
	/** \brief Select active actor pose. */
	void SelectActiveActorPose();
	
	/** \brief Update actor pose. */
	void UpdateActorPose();
	
	/** \brief Update actor gestures. */
	void UpdateActorGestures();
	
	/** \brief Select actor pose gesture. */
	void SelectActorPoseGesture( ceActorGesture *gesture );
	
	/** \brief Update actor gesture. */
	void UpdateActorGesture();
	
	/** \brief Update actor controllers. */
	void UpdateActorControllers();
	
	/** \brief Update actor controller. */
	void UpdateActorController();
	
	/** \brief Update actor commands list. */
	void UpdateActorCommands();
	
	/** \brief Update actor parameter list. */
	void UpdateActorParameters();
	
	/** \brief Active coordinate system. */
	ceCoordSystem *GetCoordSys() const;
	
	/** \brief Update coordinate systems. */
	void UpdateCoordSystems();
	
	/** \brief Update playback file list. */
	void UpdatePlaybackFileList();
	
	/** \brief Selected playback file. */
	ceConversationFile *GetSelectedPlaybackFile() const;
	
	/** \brief Update playback topic list. */
	void UpdatePlaybackTopicList();
	
	/** \brief Selected playback topic. */
	ceConversationTopic *GetSelectedPlaybackTopic() const;
	
	/** \brief Update playback parameters. */
	void UpdatePlayback();
	
	/** \brief Update playback commands list. */
	void UpdatePlaybackCommands();
	
	/** \brief Update playback variables list. */
	void UpdatePlaybackVariables();
	
	/** \brief Update playback trigger table. */
	void UpdatePlaybackTriggerTable();
	
	/** \brief Update playback missing words. */
	void UpdatePlaybackMissingWords();
	/*@}*/
};

#endif
