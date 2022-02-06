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
	deoxrActionSet( deoxrInstance &instance, const char *name = "dragengine",
		const char *localizedName = "Drag[en]gine" );
	
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
	deoxrAction *GetActionAt( int index ) const;
	
	/** Named action or nullptr. */
	deoxrAction *GetActionNamed( const char *name ) const;
	
	/** Add action. */
	deoxrAction *AddAction( deoxrAction::eType, const char *name, const char *localizedName );
	
	/** Add boolean action. */
	deoxrAction *AddBoolAction( const char *name, const char *localizedName );
	
	/** Add float action. */
	deoxrAction *AddFloatAction( const char *name, const char *localizedName );
	
	/** Add vector2 action. */
	deoxrAction *AddVector2Action( const char *name, const char *localizedName );
	
	/** Add pose action. */
	deoxrAction *AddPoseAction( const char *name, const char *localizedName );
	
	/** Add vibration action. */
	deoxrAction *AddVibrationAction( const char *name, const char *localizedName );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif

