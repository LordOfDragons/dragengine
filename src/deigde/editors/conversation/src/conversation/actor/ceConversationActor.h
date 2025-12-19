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

#ifndef _CECONVERSATIONACTOR_H_
#define _CECONVERSATIONACTOR_H_

#include "pose/ceActorPose.h"
#include "speechAnimation/ceSpeechAnimation.h"
#include "../playback/command/cePlaybackCommand.h"
#include "../facepose/ceFacePose.h"
#include "../target/ceTarget.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTList.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/collection/decTDictionary.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorInstance.h>
#include <dragengine/resources/sound/deSpeaker.h>

class ceGesture;
class cePlayback;
class ceConversation;

class igdeEnvironment;
class deLogger;
class deSound;



/**
 * \brief Conversation actor.
 */
class ceConversationActor : public deObject{
public:
	typedef deTObjectReference<ceConversationActor> Ref;
	typedef decTDictionary<decString,int> ParameterMap;
	typedef decTObjectOrderedSet<ceActorPose> PoseList;
	
	
private:
	class cGesture : public deObject{
	public:
		typedef deTObjectReference<cGesture> Ref;
		ceGesture *gesture;
		float pause;
		float length;
		cGesture(ceGesture *agesture, float apause, float alength) :
			gesture(agesture), pause(apause), length(alength){}
	};
	
	class cFacePose : public deObject{
	public:
		typedef deTObjectReference<cFacePose> Ref;
		ceFacePose::Ref facePose;
		float pause;
		float length;
		cFacePose(ceFacePose *afacePose, float apause, float alength) :
			facePose(afacePose), pause(apause), length(alength){}
	};
	
	class cLookAt : public deObject{
	public:
		typedef deTObjectReference<cLookAt> Ref;
		ceTarget::Ref lookAt;
		float pause;
		float duration;
		cLookAt(ceTarget *alookAt, float apause, float aduration) :
			lookAt(alookAt), pause(apause), duration(aduration){}
	};
	
	
	
private:
	ceConversation *pConversation;
	
	igdeEnvironment &pEnvironment;
	deComponent::Ref pEngComponent;
	deAnimatorInstance::Ref pEngAnimatorInstance;
	deAnimatorInstance::Ref pEngGestureAnimatorInstance;
	deAnimator::Ref pEngFacePoseAnimator;
	deAnimatorInstance::Ref pEngFacePoseAnimatorInstance;
	deAnimator::Ref pEngEyesAnimator;
	deAnimatorInstance::Ref pEngEyesAnimatorInstance;
	deSpeaker::Ref pEngSpeaker;
	
	decString pNameGestureProgress;
	decString pNameGesturePlayback;
	decString pNameGestureVariation;
	decString pNameGestureBlink;
	decString pNameGestureFadeIn;
	decString pNameGestureFadeOut;
	
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
	cePlaybackCommand::List pCommands;
	ParameterMap pParameter;
	
	ceSpeechAnimation::Ref pSpeechAnimation;
	
	PoseList pPoses;
	ceActorPose::Ref pActivePose;
	
	float pHeadLeftRight;
	float pHeadUpDown;
	float pEyesLeftRight;
	float pEyesUpDown;
	
	decTObjectList<cGesture> pPlayGestures;
	int pPlayGesturePos;
	float pPlayGestureElapsed;
	bool pPlayGestureRunning;
	
	decTObjectList<cFacePose> pPlayFacePoses;
	int pPlayFacePosePos;
	float pPlayFacePoseElapsed;
	bool pPlayFacePoseRunning;
	ceFacePose::Ref pPlayLastFacePose;
	ceFacePose::Ref pPlayCurFacePose;
	
	decTObjectList<cLookAt> pPlayHeadLAs;
	int pPlayHeadLAPos;
	float pPlayHeadLAElapsed;
	bool pPlayHeadLARunning;
	ceTarget::Ref pPlayLastHeadLA;
	ceTarget::Ref pPlayCurHeadLA;
	
	decTObjectList<cLookAt> pPlayEyesLAs;
	int pPlayEyesLAPos;
	float pPlayEyesLAElapsed;
	bool pPlayEyesLARunning;
	ceTarget::Ref pPlayLastEyesLA;
	ceTarget::Ref pPlayCurEyesLA;
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
	ceConversationActor(igdeEnvironment &environment);
	
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
	inline const deAnimatorInstance::Ref &GetEngineGestureAnimatorInstance() const{ return pEngGestureAnimatorInstance; }
	/** Retrieves the engine speaker. */
	inline const deSpeaker::Ref &GetEngineSpeaker() const{ return pEngSpeaker; }
	
	/** Retrieves the parent conversation or nullptr if not set. */
	inline ceConversation *GetConversation() const{ return pConversation; }
	/** Sets the parent conversation or nullptr if not set. */
	void SetConversation(ceConversation *conversation);
	
	/** Update actor. */
	void Update(cePlayback &playback, float elapsed);
	
	/** Retrieves the model path. */
	inline const decString &GetPathModel() const{ return pPathModel; }
	/** Sets the model path. */
	void SetPathModel(const char *path);
	/** Retrieves the skin path. */
	inline const decString &GetPathSkin() const{ return pPathSkin; }
	/** Sets the skin path. */
	void SetPathSkin(const char *path);
	/** Retrieves the rig path. */
	inline const decString &GetPathRig() const{ return pPathRig; }
	/** Sets the rig path. */
	void SetPathRig(const char *path);
	/** Retrieves the speech animation path. */
	inline const decString &GetPathSpeechAnimation() const{ return pPathSpeechAnimation; }
	/** Sets the speech animation path. */
	void SetPathSpeechAnimation(const char *path);
	/** Retrieves the face pose animator path. */
	inline const decString &GetPathFacePoseAnimator() const{ return pPathFacePoseAnimator; }
	/** Sets the face pose animator path. */
	void SetPathFacePoseAnimator(const char *path);
	/** Retrieves the eyes animator path. */
	inline const decString &GetPathEyesAnimator() const{ return pPathEyesAnimator; }
	/** Sets the eyes animator path. */
	void SetPathEyesAnimator(const char *path);
	/** Retrieves the actor id. */
	inline const decString &GetID() const{ return pID; }
	/** Sets the actor id. */
	void SetID(const char *id);
	/** Retrieves the actor alias id. */
	inline const decString &GetAliasID() const{ return pAliasID; }
	/** Sets the actor alias id. */
	void SetAliasID(const char *id);
	/** Retrieves the text box name. */
	inline const decUnicodeString &GetTextBoxName() const{ return pTextBoxName; }
	/** Sets the text box name. */
	void SetTextBoxName(const decUnicodeString &name);
	
	/** Retrieves the position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	/** Sets the position. */
	void SetPosition(const decVector &position);
	/** Retrieves the orientation. */
	inline const decVector &GetOrientation() const{ return pOrientation; }
	/** Sets the orientation. */
	void SetOrientation(const decVector &orientation);
	
	/** \brief Head rotator bone name. */
	inline const decString &GetBoneHeadRotator() const{ return pBoneHeadRotator; }
	
	/** \brief Set head rotator bone name. */
	void SetBoneHeadRotator(const char *bone);
	
	/** \brief Commands. */
	inline cePlaybackCommand::List &GetCommands(){ return pCommands; }
	inline const cePlaybackCommand::List &GetCommands() const{ return pCommands; }
	
	/** \brief Parameters. */
	inline ParameterMap &GetParameters(){ return pParameter; }
	inline const ParameterMap &GetParameter() const{ return pParameter; }
	
	/** Retrieves the speech animation. */
	inline const ceSpeechAnimation::Ref &GetSpeechAnimation() const{ return pSpeechAnimation; }
	
	/** \brief Poses. */
	inline PoseList &GetPoses(){ return pPoses; }
	inline const PoseList &GetPoses() const{ return pPoses; }
	
	/** \brief Active pose or \em nullptr. */
	inline const ceActorPose::Ref &GetActivePose() const{ return pActivePose; }
	
	/** \brief Set active pose or \em nullptr. */
	void SetActivePose(ceActorPose *pose);
	
	/** \brief Notify all poses changed. */
	void NotifyPosesChanged();
	
	/** \brief Notify all pose gestures changed. */
	void NotifyPoseGesturesChanged(ceActorPose *pose);
	
	/** Play a voice audio sound file. */
	void PlayVoiceAudio(deSound *sound);
	
	/** Retrieves the head left-right angle. */
	inline float GetHeadLeftRight() const{ return pHeadLeftRight; }
	/** Sets the head left-right angle. */
	void SetHeadLeftRight(float angle);
	/** Retrieves the head up-down angle. */
	inline float GetHeadUpDown() const{ return pHeadUpDown; }
	/** Sets the head up-down angle. */
	void SetHeadUpDown(float angle);
	
	/** Retrieves the eyes left-right angle. */
	inline float GetEyesLeftRight() const{ return pEyesLeftRight; }
	/** Sets the eyes left-right angle. */
	void SetEyesLeftRight(float angle);
	/** Retrieves the eyes up-down angle. */
	inline float GetEyesUpDown() const{ return pEyesUpDown; }
	/** Sets the eyes up-down angle. */
	void SetEyesUpDown(float angle);
	
	/** Remove all play gestures. */
	void RemoveAllPlayGestures();
	/** Adds a play gesture. */
	void AddPlayGesture(ceGesture *gesture, float pause, float length);
	
	/** Remove all play face poses. */
	void RemoveAllPlayFacePoses();
	/** Adds a play face pose. */
	void AddPlayFacePose(ceFacePose *facePose, float pause, float length);
	
	/** Remove all play head look-ats. */
	void RemoveAllPlayHeadLookAts();
	/** Adds a play head look-at. */
	void AddPlayHeadLookAt(ceTarget *lookAt, float pause, float duration);
	
	/** Remove all play head look-ats. */
	void RemoveAllPlayEyesLookAts();
	/** Adds a play head look-at. */
	void AddPlayEyesLookAt(ceTarget *lookAt, float pause, float duration);
	
	/** \brief Determines if the actor causes waiting if the playback checks for actor waiting. */
	inline bool GetWaiting() const{ return pWaiting; }
	/** \brief Sets if the actor causes waiting if the playback checks for actor waiting. */
	void SetWaiting(bool waiting);
	
	/** Retrieves the bone matrix if the bone exists or the identity matrix otherwise. */
	decMatrix GetBoneMatrix(const char *bone) const;
	
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
	
	void pUpdateAnimatorInstance(float elapsed);
	void pUpdatePlayGesture(float elapsed);
	void pUpdatePlayFacePose(float elapsed);
	void pUpdatePlayHeadLookAt(cePlayback &playback, float elapsed);
	void pUpdatePlayEyesLookAt(cePlayback &playback, float elapsed);
	void pUpdateBlinking(float elapsed);
};

#endif
