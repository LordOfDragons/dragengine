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

#ifndef _CECONVERSATIONACTOR_H_
#define _CECONVERSATIONACTOR_H_

#include "pose/ceActorPoseList.h"
#include "parameters/ceActorParameterList.h"
#include "../playback/command/cePlaybackCommandList.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>

class cePlayback;
class ceConversation;
class ceSpeechAnimation;
class ceGesture;
class ceFacePose;
class ceTarget;

class igdeEnvironment;
class deComponent;
class deAnimator;
class deAnimatorInstance;
class deLogger;
class deSpeaker;
class deSound;



/**
 * \brief Conversation actor.
 */
class ceConversationActor : public deObject{
private:
	struct sGesture{
		ceGesture *gesture;
		float pause;
		float length;
	};
	
	struct sFacePose{
		ceFacePose *facePose;
		float pause;
		float length;
	};
	
	struct sLookAt{
		ceTarget *lookAt;
		float pause;
		float duration;
	};
	
	
	
private:
	ceConversation *pConversation;
	
	igdeEnvironment &pEnvironment;
	deComponent *pEngComponent;
	deAnimatorInstance *pEngAnimatorInstance;
	deAnimatorInstance *pEngGestureAnimatorInstance;
	deAnimator *pEngFacePoseAnimator;
	deAnimatorInstance *pEngFacePoseAnimatorInstance;
	deAnimator *pEngEyesAnimator;
	deAnimatorInstance *pEngEyesAnimatorInstance;
	deSpeaker *pEngSpeaker;
	
	decString pNameGestureProgress;
	decString pNameGesturePlayback;
	decString pNameGestureVariation;
	decString pNameGestureBlink;
	
	decString pPathModel;
	decString pPathSkin;
	decString pPathRig;
	decString pPathSpeechAnimation;
	decString pPathFacePoseAnimator;
	decString pPathEyesAnimator;
	decString pID;
	decString pAliasID;
	decUnicodeString pTextBoxName;
	decVector pPosition;
	decVector pOrientation;
	decString pBoneHeadRotator;
	cePlaybackCommandList pCommands;
	ceActorParameterList pParameter;
	
	ceSpeechAnimation *pSpeechAnimation;
	
	ceActorPoseList pPoses;
	ceActorPose *pActivePose;
	
	float pHeadLeftRight;
	float pHeadUpDown;
	float pEyesLeftRight;
	float pEyesUpDown;
	
	sGesture *pPlayGestures;
	int pPlayGestureCount;
	int pPlayGestureSize;
	int pPlayGesturePos;
	float pPlayGestureElapsed;
	bool pPlayGestureRunning;
	
	sFacePose *pPlayFacePoses;
	int pPlayFacePoseCount;
	int pPlayFacePoseSize;
	int pPlayFacePosePos;
	float pPlayFacePoseElapsed;
	bool pPlayFacePoseRunning;
	ceFacePose *pPlayLastFacePose;
	ceFacePose *pPlayCurFacePose;
	
	sLookAt *pPlayHeadLAs;
	int pPlayHeadLACount;
	int pPlayHeadLASize;
	int pPlayHeadLAPos;
	float pPlayHeadLAElapsed;
	bool pPlayHeadLARunning;
	ceTarget *pPlayLastHeadLA;
	ceTarget *pPlayCurHeadLA;
	
	sLookAt *pPlayEyesLAs;
	int pPlayEyesLACount;
	int pPlayEyesLASize;
	int pPlayEyesLAPos;
	float pPlayEyesLAElapsed;
	bool pPlayEyesLARunning;
	ceTarget *pPlayLastEyesLA;
	ceTarget *pPlayCurEyesLA;
	int pPlayEyesBoneDisable;
	
	float pBlinkFreqMin;
	float pBlinkFreqMax;
	float pBlinkNext;
	float pBlinkElapsed;
	
	bool pWaiting;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new conversation actor. */
	ceConversationActor( igdeEnvironment &environment );
	
protected:
	/** Cleans up the conversation actor. */
	virtual ~ceConversationActor();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Environment. */
	inline igdeEnvironment &GetEnvironment() const{ return pEnvironment; }
	
	/** Retrieves the gesture animator instance. */
	inline deAnimatorInstance *GetEngineGestureAnimatorInstance() const{ return pEngGestureAnimatorInstance; }
	/** Retrieves the engine speaker. */
	inline deSpeaker *GetEngineSpeaker() const{ return pEngSpeaker; }
	
	/** Retrieves the parent conversation or NULL if not set. */
	inline ceConversation *GetConversation() const{ return pConversation; }
	/** Sets the parent conversation or NULL if not set. */
	void SetConversation( ceConversation *conversation );
	
	/** Update actor. */
	void Update( cePlayback &playback, float elapsed );
	
	/** Retrieves the model path. */
	inline const decString &GetPathModel() const{ return pPathModel; }
	/** Sets the model path. */
	void SetPathModel( const char *path );
	/** Retrieves the skin path. */
	inline const decString &GetPathSkin() const{ return pPathSkin; }
	/** Sets the skin path. */
	void SetPathSkin( const char *path );
	/** Retrieves the rig path. */
	inline const decString &GetPathRig() const{ return pPathRig; }
	/** Sets the rig path. */
	void SetPathRig( const char *path );
	/** Retrieves the speech animation path. */
	inline const decString &GetPathSpeechAnimation() const{ return pPathSpeechAnimation; }
	/** Sets the speech animation path. */
	void SetPathSpeechAnimation( const char *path );
	/** Retrieves the face pose animator path. */
	inline const decString &GetPathFacePoseAnimator() const{ return pPathFacePoseAnimator; }
	/** Sets the face pose animator path. */
	void SetPathFacePoseAnimator( const char *path );
	/** Retrieves the eyes animator path. */
	inline const decString &GetPathEyesAnimator() const{ return pPathEyesAnimator; }
	/** Sets the eyes animator path. */
	void SetPathEyesAnimator( const char *path );
	/** Retrieves the actor id. */
	inline const decString &GetID() const{ return pID; }
	/** Sets the actor id. */
	void SetID( const char *id );
	/** Retrieves the actor alias id. */
	inline const decString &GetAliasID() const{ return pAliasID; }
	/** Sets the actor alias id. */
	void SetAliasID( const char *id );
	/** Retrieves the text box name. */
	inline const decUnicodeString &GetTextBoxName() const{ return pTextBoxName; }
	/** Sets the text box name. */
	void SetTextBoxName( const decUnicodeString &name );
	
	/** Retrieves the position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	/** Sets the position. */
	void SetPosition( const decVector &position );
	/** Retrieves the orientation. */
	inline const decVector &GetOrientation() const{ return pOrientation; }
	/** Sets the orientation. */
	void SetOrientation( const decVector &orientation );
	
	/** \brief Head rotator bone name. */
	inline const decString &GetBoneHeadRotator() const{ return pBoneHeadRotator; }
	
	/** \brief Set head rotator bone name. */
	void SetBoneHeadRotator( const char *bone );
	
	/** \brief Commands. */
	inline cePlaybackCommandList &GetCommands(){ return pCommands; }
	inline const cePlaybackCommandList &GetCommands() const{ return pCommands; }
	
	/** \brief Parameters. */
	inline ceActorParameterList &GetParameters(){ return pParameter; }
	inline const ceActorParameterList &GetParameter() const{ return pParameter; }
	
	/** Retrieves the speech animation. */
	inline ceSpeechAnimation *GetSpeechAnimation() const{ return pSpeechAnimation; }
	
	/** \brief Poses. */
	inline ceActorPoseList &GetPoses(){ return pPoses; }
	inline const ceActorPoseList &GetPoses() const{ return pPoses; }
	
	/** \brief Active pose or \em NULL. */
	inline ceActorPose *GetActivePose() const{ return pActivePose; }
	
	/** \brief Set active pose or \em NULL. */
	void SetActivePose( ceActorPose *pose );
	
	/** \brief Notify all poses changed. */
	void NotifyPosesChanged();
	
	/** \brief Notify all pose gestures changed. */
	void NotifyPoseGesturesChanged( ceActorPose *pose );
	
	/** Play a voice audio sound file. */
	void PlayVoiceAudio( deSound *sound );
	
	/** Retrieves the head left-right angle. */
	inline float GetHeadLeftRight() const{ return pHeadLeftRight; }
	/** Sets the head left-right angle. */
	void SetHeadLeftRight( float angle );
	/** Retrieves the head up-down angle. */
	inline float GetHeadUpDown() const{ return pHeadUpDown; }
	/** Sets the head up-down angle. */
	void SetHeadUpDown( float angle );
	
	/** Retrieves the eyes left-right angle. */
	inline float GetEyesLeftRight() const{ return pEyesLeftRight; }
	/** Sets the eyes left-right angle. */
	void SetEyesLeftRight( float angle );
	/** Retrieves the eyes up-down angle. */
	inline float GetEyesUpDown() const{ return pEyesUpDown; }
	/** Sets the eyes up-down angle. */
	void SetEyesUpDown( float angle );
	
	/** Remove all play gestures. */
	void RemoveAllPlayGestures();
	/** Adds a play gesture. */
	void AddPlayGesture( ceGesture *gesture, float pause, float length );
	
	/** Remove all play face poses. */
	void RemoveAllPlayFacePoses();
	/** Adds a play face pose. */
	void AddPlayFacePose( ceFacePose *facePose, float pause, float length );
	
	/** Remove all play head look-ats. */
	void RemoveAllPlayHeadLookAts();
	/** Adds a play head look-at. */
	void AddPlayHeadLookAt( ceTarget *lookAt, float pause, float duration );
	
	/** Remove all play head look-ats. */
	void RemoveAllPlayEyesLookAts();
	/** Adds a play head look-at. */
	void AddPlayEyesLookAt( ceTarget *lookAt, float pause, float duration );
	
	/** \brief Determines if the actor causes waiting if the playback checks for actor waiting. */
	inline bool GetWaiting() const{ return pWaiting; }
	/** \brief Sets if the actor causes waiting if the playback checks for actor waiting. */
	void SetWaiting( bool waiting );
	
	/** Retrieves the bone matrix if the bone exists or the identity matrix otherwise. */
	decMatrix GetBoneMatrix( const char *bone ) const;
	
	/** \brief Reset actor. */
	void Reset();
	
	/** Notifies all that the actor changed. */
	void NotifyActorChanged();
	/*@}*/
	
private:
	void pCleanUp();
	
	void pUpdateComponent();
	void pUpdateSpeechAnimation();
	void pUpdateFacePoseAnimator();
	void pUpdateEyesAnimator();
	
	void pRepositionComponent();
	void pRepositionSpeaker();
	
	void pUpdateAnimatorInstance( float elapsed );
	void pUpdatePlayGesture( float elapsed );
	void pUpdatePlayFacePose( float elapsed );
	void pUpdatePlayHeadLookAt( cePlayback &playback, float elapsed );
	void pUpdatePlayEyesLookAt( cePlayback &playback, float elapsed );
	void pUpdateBlinking( float elapsed );
};

#endif
