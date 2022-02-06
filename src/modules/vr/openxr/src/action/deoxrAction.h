/* 
 * Drag[en]gine OpenXR
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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
	deoxrActionSet &pActionSet;
	
	const eType pType;
	const decString pName;
	const decString pLocalizedName;
	XrAction pAction;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create session. */
	deoxrAction( deoxrActionSet &actionSet, eType type, const char *name, const char *localizedName );
	
protected:
	/** Clean up space. */
	virtual ~deoxrAction();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Action set. */
	inline deoxrActionSet &GetActionSet() const{ return pActionSet; }
	
	/** Type. */
	inline eType GetType() const{ return pType; }
	
	/** Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Localized name. */
	inline const decString &GetLocalizedName() const{ return pLocalizedName; }
	
	/** Action. */
	inline XrAction GetAction() const{ return pAction; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif

