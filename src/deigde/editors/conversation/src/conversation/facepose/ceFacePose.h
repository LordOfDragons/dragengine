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

#ifndef _CEFACEPOSE_H_
#define _CEFACEPOSE_H_

#include "../../utils/ceControllerValueList.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

class ceConversation;



/**
 * \brief Face Pose.
 */
class ceFacePose : public deObject{
private:
	ceConversation *pConversation;
	
	decString pName;
	ceControllerValueList pControllers;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new face pose. */
	ceFacePose( const char *name = "Face Pose");
	/** Cleans up the face pose. */
	virtual ~ceFacePose();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the conversation or NULL if not set. */
	inline ceConversation *GetConversation() const{ return pConversation; }
	/** Sets the conversation or NULL if not set. */
	void SetConversation( ceConversation *conversation );
	
	/** Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	/** Sets the name. */
	void SetName( const char *name );
	/** Retrieves the controller value list. */
	inline ceControllerValueList &GetControllerList(){ return pControllers; }
	inline const ceControllerValueList &GetControllerList() const{ return pControllers; }
	/** Notifies all that the controller value list changed. */
	void NotifyControllersChanged();
	/*@}*/
};

#endif
