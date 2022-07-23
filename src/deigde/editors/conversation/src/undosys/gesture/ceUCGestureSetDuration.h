/* 
 * Drag[en]gine IGDE Conversation Editor
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

#ifndef _CEUCGESTURESETDURATION_H_
#define _CEUCGESTURESETDURATION_H_

#include "../../conversation/gesture/ceGesture.h"

#include <deigde/undo/igdeUndo.h>


/**
 * Undo Action Gesture Set Length.
 */
class ceUCGestureSetDuration : public igdeUndo{
private:
	const ceGesture::Ref pGesture;
	
	float pOldValue;
	float pNewValue;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create undo. */
	ceUCGestureSetDuration( ceGesture *gesture, float newValue );
	
protected:
	/** Clean up undo. */
	virtual ~ceUCGestureSetDuration();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Undo. */
	virtual void Undo();
	
	/** Redo. */
	virtual void Redo();
	/*@}*/
};

#endif
