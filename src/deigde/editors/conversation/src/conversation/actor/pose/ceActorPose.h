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

#ifndef _CEACTORPOSE_H_
#define _CEACTORPOSE_H_

#include "../controller/ceActorControllerList.h"
#include "../gesture/ceActorGestureList.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

class igdeEnvironment;
#include <dragengine/resources/animator/deAnimator.h>



/**
 * Conversation Actor Gesture.
 */
class ceActorPose : public deObject{
private:
	igdeEnvironment &pEnvironment;
	deAnimator::Ref pEngAnimator;
	
	decString pName;
	decString pPathAnimator;
	
	ceActorControllerList pControllers;
	ceActorGestureList pGestures;
	
	decStringList pControllerNames;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<ceActorPose> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** Create actor pose. */
	ceActorPose(igdeEnvironment &environment, const char *name);
	
	/** Create copy of actor pose. */
	ceActorPose(const ceActorPose &pose);
	
protected:
	/** Clean up actor pose. */
	virtual ~ceActorPose();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Environment. */
	inline igdeEnvironment &GetEnvironment() const{ return pEnvironment; }
	
	/** Engine animator or \em NULL. */
	inline const deAnimator::Ref &GetEngineAnimator() const{ return pEngAnimator; }
	
	
	
	/** Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Set name. */
	void SetName(const char *name);
	
	/** Animator path. */
	inline const decString &GetPathAnimator() const{ return pPathAnimator; }
	
	/** Set animator path. */
	void SetPathAnimator(const char *path);
	
	/** Controllers. */
	inline ceActorControllerList &GetControllers(){ return pControllers; }
	inline const ceActorControllerList &GetControllers() const{ return pControllers; }
	
	/** Gestures. */
	inline ceActorGestureList &GetGestures(){ return pGestures; }
	inline const ceActorGestureList &GetGestures() const{ return pGestures; }
	
	/** Controller names. */
	inline const decStringList &GetControllerNames() const{ return pControllerNames; }
	/*@}*/
	
	
	
private:
	void pLoadAnimator();
};

#endif
