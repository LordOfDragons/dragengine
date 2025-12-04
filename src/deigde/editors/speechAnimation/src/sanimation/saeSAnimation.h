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

#ifndef _SAESANIMATION_H_
#define _SAESANIMATION_H_

#include "phoneme/saePhonemeList.h"
#include "dictionary/saeWordList.h"

#include <deigde/editableentity/igdeEditableEntity.h>

#include <dragengine/common/collection/decObjectSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorInstance.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/world/deWorld.h>

class igdeWSky;
class igdeCamera;
class saeSAnimationListener;



/**
 * Editable Sky.
 */
class saeSAnimation : public igdeEditableEntity{
public:
	/** Type holding strong reference. */
	typedef deTObjectReference<saeSAnimation> Ref;
	
	
	
public:
	/** Display modes. */
	enum eDisplayModes{
		/** Display active phoneme. */
		edmPhoneme,
		/** Display active word. */
		edmWord
	};
	
private:
	deWorld::Ref pEngWorld;
	igdeWSky *pSky;
	deComponent::Ref pEngComponent;
	deAnimator::Ref pEngAnimator;
	deAnimatorInstance::Ref pEngAnimatorInstance;
	
	igdeCamera *pCamera;
	
	decString pDisplayModelPath;
	decString pDisplaySkinPath;
	decString pDisplayRigPath;
	
	decString pRigPath;
	decString pAnimationPath;
	
	decString pNeutralMoveName;
	decStringSet pNeutralVertexPositionSets;
	
	saePhonemeList pPhonemeList;
	saePhoneme *pActivePhoneme;
	
	saeWordList pWordList;
	saeWord *pActiveWord;
	
	eDisplayModes pDisplayMode;
	int pDispWordPos;
	float pDispWordElapsed;
	bool pDirtyAnimator;
	
	decObjectSet pListeners;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new speech animation. */
	saeSAnimation( igdeEnvironment *environment );
	/** Cleans up the speech animation. */
	virtual ~saeSAnimation();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the engine world. */
	inline deWorld *GetEngineWorld() const{ return pEngWorld; }
	
	/** Retrieves the animator. */
	inline deAnimator *GetEngineAnimator() const{ return pEngAnimator; }
	
	/** Component. */
	inline const deComponent::Ref &GetEngineComponent() const{ return pEngComponent; }
	
	/** Retrieves the sky. */
	inline igdeWSky *GetSky() const{ return pSky; }
	/** Retrieves the camera. */
	inline igdeCamera *GetCamera() const{ return pCamera; }
	
	/** Retrieves the display model path. */
	inline const decString &GetDisplayModelPath() const{ return pDisplayModelPath; }
	/** Sets the display model path. */
	void SetDisplayModelPath( const char *path );
	/** Retrieves the display skin path. */
	inline const decString &GetDisplaySkinPath() const{ return pDisplaySkinPath; }
	/** Sets the display skin path. */
	void SetDisplaySkinPath( const char *path );
	/** Retrieves the display rig path. */
	inline const decString &GetDisplayRigPath() const{ return pDisplayRigPath; }
	/** Sets the display rig path. */
	void SetDisplayRigPath( const char *path );
	
	/** Retrieves the rig path. */
	inline const decString &GetRigPath() const{ return pRigPath; }
	/** Sets the rig path. */
	void SetRigPath( const char *path );
	/** Retrieves the rig path. */
	inline const decString &GetAnimationPath() const{ return pAnimationPath; }
	/** Sets the rig path. */
	void SetAnimationPath( const char *path );
	
	/** Neutral move name. */
	inline const decString &GetNeutralMoveName() const{ return pNeutralMoveName; }
	
	/** Set neutral move name. */
	void SetNeutralMoveName( const char *name );
	
	/** Neutral vertex position sets. */
	inline const decStringSet &GetNeutralVertexPositionSets() const{ return pNeutralVertexPositionSets; }
	
	/** Set neutral vertex position sets. */
	void SetNeutralVertexPositionSets( const decStringSet &sets );
	
	/** Retrieves the display mode. */
	inline eDisplayModes GetDisplayMode() const{ return pDisplayMode; }
	/** Sets the display mode. */
	void SetDisplayMode( eDisplayModes displayMode );
	
	/** Rebuild animator if required. */
	void RebuildAnimator();
	
	/** Dispose of all resources. */
	void Dispose();
	/** Updates the sky. */
	void Update( float elapsed );
	/** Resets the sky. */
	void Reset();
	/*@}*/
	
	/** \name Phonemes */
	/*@{*/
	/** Retrieves the phoneme list read-only. */
	inline const saePhonemeList &GetPhonemeList() const{ return pPhonemeList; }
	/** Adds a new phoneme. */
	void AddPhoneme( saePhoneme *phoneme );
	/** Removes a phoneme. */
	void RemovePhoneme( saePhoneme *phoneme );
	/** Removes all phonemes. */
	void RemoveAllPhonemes();
	/** Retrieves the active texture or NULL if none is active. */
	inline saePhoneme *GetActivePhoneme() const{ return pActivePhoneme; }
	/** Determines if there is an active phoneme or not. */
	bool HasActivePhoneme() const;
	/** Sets the active phoneme or NULL if none is active. */
	void SetActivePhoneme( saePhoneme *phoneme );
	/*@}*/
	
	/** \name Words */
	/*@{*/
	/** Retrieves the word list read-only. */
	inline const saeWordList &GetWordList() const{ return pWordList; }
	/** Adds a new word. */
	void AddWord( saeWord *word );
	/** Removes a word. */
	void RemoveWord( saeWord *word );
	/** Removes all words. */
	void RemoveAllWords();
	/** Retrieves the active texture or NULL if none is active. */
	inline saeWord *GetActiveWord() const{ return pActiveWord; }
	/** Determines if there is an active word or not. */
	bool HasActiveWord() const;
	/** Sets the active word or NULL if none is active. */
	void SetActiveWord( saeWord *word );
	/*@}*/
	
	/** \name Notifiers */
	/*@{*/
	void AddListener( saeSAnimationListener *listener );
	/** Removes a listener. */
	void RemoveListener( saeSAnimationListener *listener );
	
	/** Notifies all listeners that the changed or saved state changed. */
	virtual void NotifyStateChanged();
	/** Notifies all listeners that the undo system changed. */
	virtual void NotifyUndoChanged();
	/** Notifies all that view properties changed. */
	void NotifyViewChanged();
	/** Notifies all that the speech animation changed. */
	void NotifySAnimationChanged();
	/** Notifies all that the model changed. */
	void NotifyModelChanged();
	/** Notifies all that the sky changed. */
	void NotifySkyChanged();
	/** Notifies all that the camera changed. */
	void NotifyCameraChanged();
	
	/** Notifies all that the phoneme count or order changed. */
	void NotifyPhonemeStructureChanged();
	/** Notifies all that a phoneme changed. */
	void NotifyPhonemeChanged( saePhoneme *phoneme );
	/** Notifies all that the active phoneme changed. */
	void NotifyActivePhonemeChanged();
	
	/** Notifies all that the word count or order changed. */
	void NotifyWordStructureChanged();
	/** Notifies all that a word changed. */
	void NotifyWordNameChanged( saeWord *word );
	/** Notifies all that a word changed. */
	void NotifyWordChanged( saeWord *word );
	/** Notifies all that the active word changed. */
	void NotifyActiveWordChanged();
	/*@}*/
	
private:
	void pCleanUp();
	
	void pUpdateComponent();
	void pUpdateAnimRig();
};

#endif
