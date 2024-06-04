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

#ifndef _CEWPCONVERSATION_H_
#define _CEWPCONVERSATION_H_

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class ceWindowProperties;
class ceWPConversationListener;
class ceConversation;
class ceGesture;
class ceFacePose;
class ceCameraShot;
class ceTarget;
class ceControllerValue;



/**
 * \brief Conversation property window.
 */
class ceWPConversation : public igdeContainerScroll{
private:
	ceWindowProperties &pWindowProperties;
	ceWPConversationListener *pListener;
	ceConversation *pConversation;
	
	igdeEditPathReference pPathImportConvo;
	igdeButtonReference pBtnImportConvoAdd;
	igdeListBoxReference pListImportConvoPath;
	
	igdeComboBoxFilterReference pCBGesture;
	igdeButtonReference pBtnGesture;
	igdeTextFieldReference pEditGestureAnimator;
	igdeCheckBoxReference pChkGestureHold;
	igdeTextFieldReference pEditGestureDuration;
	
	igdeComboBoxFilterReference pCBFacePose;
	igdeButtonReference pBtnFacePose;
	igdeComboBoxReference pCBFPController;
	igdeTextFieldReference pEditFPControllerValue;
	igdeButtonReference pBtnFPController;
	
	igdeComboBoxFilterReference pCBTarget;
	igdeButtonReference pBtnTarget;
	igdeComboBoxFilterReference pCBTargetActorID;
	igdeTextFieldReference pEditTargetEntityID;
	igdeTextFieldReference pEditTargetBone;
	igdeEditVectorReference pEditTargetPosition;
	igdeEditVectorReference pEditTargetOrientation;
	
	igdeComboBoxFilterReference pCBCameraShot;
	igdeButtonReference pBtnCameraShot;
	igdeTextFieldReference pEditCShotActorCount;
	igdeComboBoxFilterReference pCBCameraShotCameraTarget;
	igdeEditVectorReference pEditCShotOffCamFrom;
	igdeEditVectorReference pEditCShotOffCamTo;
	igdeComboBoxFilterReference pCBCameraShotLookAtTarget;
	igdeEditVectorReference pEditCShotOffLookAtFrom;
	igdeEditVectorReference pEditCShotOffLookAtTo;
	igdeEditVectorReference pEditCShotCamOrbitFrom;
	igdeEditVectorReference pEditCShotCamOrbitTo;
	igdeTextFieldReference pEditCShotCamDistFrom;
	igdeTextFieldReference pEditCShotCamDistTo;
	igdeEditVectorReference pEditCShotPosFrom;
	igdeEditVectorReference pEditCShotPosTo;
	igdeEditVectorReference pEditCShotRotFrom;
	igdeEditVectorReference pEditCShotRotTo;
	igdeTextFieldReference pEditCShotTiltFrom;
	igdeTextFieldReference pEditCShotTiltTo;
	igdeTextFieldReference pEditCShotFovFrom;
	igdeTextFieldReference pEditCShotFovTo;
	igdeCheckBoxReference pChkCShotLockUpAxis;
	igdeCheckBoxReference pChkCShotAlignTargets;
	igdeCheckBoxReference pChkCShotRelativeToLookAt;
	igdeCheckBoxReference pChkCShotLockCameraTarget;
	igdeCheckBoxReference pChkCShotLockLookAtTarget;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	ceWPConversation( ceWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up panel. */
	virtual ~ceWPConversation();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Properties windows. */
	inline ceWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** \brief Conversation. */
	inline ceConversation *GetConversation() const{ return pConversation; }
	
	/** \brief Set conversation. */
	void SetConversation( ceConversation *conversation );
	
	/** \brief Update conversation. */
	void UpdateConversation();
	
	/** \brief Conversation path changed. */
	void OnConversationPathChanged();
	
	
	
	/** \brief Update import conversation path list. */
	void UpdateImportConvoPathList();
	
	/** \brief Get text from import conversation path. */
	const decString &GetPathImportConvo() const;
	
	/** \brief Selected import conversation path or empty path if no selection. */
	decString GetImportConvo() const;
	
	
	
	/** \brief Active gesture. */
	ceGesture *GetGesture() const;
	
	/** \brief Update list of gestures. */
	void UpdateGestureList();
	
	/** \brief Select active gesture. */
	void SelectActiveGesture();
	
	/** \brief Update active gesture. */
	void UpdateGesture();
	
	
	
	/** \brief Active face pose. */
	ceFacePose *GetFacePose() const;
	
	/** \brief Update list of face poses. */
	void UpdateFacePoseList();
	
	/** \brief Select active face pose. */
	void SelectActiveFacePose();
	
	/** \brief Update active face pose. */
	void UpdateFacePose();
	
	/** \brief Active controller value. */
	ceControllerValue *GetFacePoseController() const;
	
	/** \brief Update list of face pose controllers. */
	void UpdateFPControllerList();
	
	/** \brief Select face pose controller. */
	void SelectFacePoseController( ceControllerValue *controller );
	
	/** \brief Update the active face pose controller. */
	void UpdateFPController();
	
	
	
	/** \brief Active target. */
	ceTarget *GetTarget() const;
	
	/** \brief Update list of targets. */
	void UpdateTargetList();
	
	/** \brief Select active target. */
	void SelectActiveTarget();
	
	/** \brief Update active target. */
	void UpdateTarget();
	
	
	
	/** \brief Active camera shot. */
	ceCameraShot *GetCameraShot() const;
	
	/** \brief Update list of camera shots. */
	void UpdateCameraShotList();
	
	/** \brief Select active camera shot. */
	void SelectActiveCameraShot();
	
	/** \brief Update active camera shot. */
	void UpdateCameraShot();
	
	
	
	/** \brief Update actor id lists. */
	void UpdateActorIDLists();
	/*@}*/
};

#endif
