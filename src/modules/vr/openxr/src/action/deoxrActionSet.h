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

#ifndef _DEOXRACTIONSET_H_
#define _DEOXRACTIONSET_H_

#include "deoxrAction.h"
#include "../deoxrBasics.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>

class deoxrInstance;


/**
 * Oxr action set.
 */
class deoxrActionSet : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<deoxrActionSet> Ref;
	
	
private:
	deoxrInstance &pInstance;
	
	const decString pName;
	const decString pLocalizedName;
	XrActionSet pActionSet;
	decObjectOrderedSet pActions;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create action set. */
	deoxrActionSet(deoxrInstance &instance, const char *name = "dragengine",
		const char *localizedName = "Drag[en]gine");
	
protected:
	/** Clean up space. */
	virtual ~deoxrActionSet();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Instance. */
	inline deoxrInstance &GetInstance() const{ return pInstance; }
	
	/** Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Localized name. */
	inline const decString &GetLocalizedName() const{ return pLocalizedName; }
	
	/** ActionSet. */
	inline XrActionSet GetActionSet() const{ return pActionSet; }
	
	/** Count of actions. */
	int GetActionCount() const;
	
	/** Action at index. */
	deoxrAction *GetActionAt(int index) const;
	
	/** Named action or nullptr. */
	deoxrAction *GetActionNamed(const char *name) const;
	
	/** Add action. */
	void AddAction(deoxrAction *action);
	
	/** Add action. */
	deoxrAction *AddAction(deoxrAction::eType, const char *name, const char *localizedName);
	
	/** Add action. */
	deoxrAction *AddAction(deoxrAction::eType, const char *name, const char *localizedName,
		const XrPath *subactionPath, int subactionPathCount);
	
	/** Add boolean action. */
	deoxrAction *AddBoolAction(const char *name, const char *localizedName);
	
	/** Add float action. */
	deoxrAction *AddFloatAction(const char *name, const char *localizedName);
	
	/** Add vector2 action. */
	deoxrAction *AddVector2Action(const char *name, const char *localizedName);
	
	/** Add pose action. */
	deoxrAction *AddPoseAction(const char *name, const char *localizedName);
	
	/** Add vibration action. */
	deoxrAction *AddVibrationAction(const char *name, const char *localizedName);
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif

