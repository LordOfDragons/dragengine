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
	ceCACameraShot( const ceCACameraShot &action );
	/** Cleans up the camera shot conversation action. */
	virtual ~ceCACameraShot();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the name of the camera shot to use. */
	inline const decString &GetName() const{ return pName; }
	/** Sets the name of the camera shot to use. */
	void SetName( const char *name );
	/** Retrieves the duration. */
	inline float GetDuration() const{ return pDuration; }
	/** Sets the duration. */
	void SetDuration( float duration );
	/** Retrieves the camera target id. */
	inline const decString &GetCameraTarget() const{ return pCameraTarget; }
	/** Sets the camera target id. */
	void SetCameraTarget( const char *target );
	/** Retrieves the look-at target id. */
	inline const decString &GetLookAtTarget() const{ return pLookAtTarget; }
	/** Sets the look-at target id. */
	void SetLookAtTarget( const char *target );
	
	/** Create a copy of this action. */
    virtual ceConversationAction *CreateCopy() const;
	/*@}*/
};

#endif
