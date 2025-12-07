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

#ifndef _CECACAMERASHOT_H_
#define _CECACAMERASHOT_H_

#include "ceConversationAction.h"

#include <dragengine/common/string/decString.h>



/**
 * \brief Camera Shot Conversation Action.
 * Sets the camera shot to use. Camera shots are defined globally in the conversation
 * with a unique name.
 */
class ceCACameraShot : public ceConversationAction{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<ceCACameraShot> Ref;
	
	
private:
	decString pName;
	float pDuration;
	decString pCameraTarget;
	decString pLookAtTarget;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new camera shot conversation action. */
	ceCACameraShot();
	/** Creates a new camera shot conversation action. */
	ceCACameraShot(const ceCACameraShot &action);
	/** Cleans up the camera shot conversation action. */
	virtual ~ceCACameraShot();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the name of the camera shot to use. */
	inline const decString &GetName() const{return pName;}
	/** Sets the name of the camera shot to use. */
	void SetName(const char *name);
	/** Retrieves the duration. */
	inline float GetDuration() const{return pDuration;}
	/** Sets the duration. */
	void SetDuration(float duration);
	/** Retrieves the camera target id. */
	inline const decString &GetCameraTarget() const{return pCameraTarget;}
	/** Sets the camera target id. */
	void SetCameraTarget(const char *target);
	/** Retrieves the look-at target id. */
	inline const decString &GetLookAtTarget() const{return pLookAtTarget;}
	/** Sets the look-at target id. */
	void SetLookAtTarget(const char *target);
	
	/** Create a copy of this action. */
    virtual ceConversationAction *CreateCopy() const;
	/*@}*/
};

#endif
