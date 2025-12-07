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

#ifndef _DEOXRACTION_H_
#define _DEOXRACTION_H_

#include "../deoxrBasics.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

class deoxrActionSet;


/**
 * Oxr action.
 */
class deoxrAction : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<deoxrAction> Ref;
	
	
	/** Type. */
	enum eType{
		etInputBool,
		etInputFloat,
		etInputVector2,
		etInputPose,
		etOutputVibration
	};
	
	
	
private:
	deoxrActionSet *pActionSet;
	
	const eType pType;
	const decString pName;
	const decString pLocalizedName;
	XrAction pAction;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create session. */
	deoxrAction(deoxrActionSet &actionSet, eType type, const char *name, const char *localizedName);
	
	/** Create session. */
	deoxrAction(deoxrActionSet &actionSet, eType type, const char *name,
		const char *localizedName, const XrPath *subactionPath, int subactionPathCount);
	
protected:
	/** Clean up space. */
	virtual ~deoxrAction();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Action set or nullptr if dropped. */
	inline deoxrActionSet *GetActionSet() const{return pActionSet;}
	
	/** Type. */
	inline eType GetType() const{return pType;}
	
	/** Name. */
	inline const decString &GetName() const{return pName;}
	
	/** Localized name. */
	inline const decString &GetLocalizedName() const{return pLocalizedName;}
	
	/** Action or XR_NULL_HANDLE if dropped. */
	inline XrAction GetAction() const{return pAction;}
	
	/** Drop action set. */
	void DropActionSet();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif

