/* 
 * Drag[en]gine IGDE Synthesizer Editor
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


#ifndef _SEUSETSOURCESOUNDMAXSPEED_H_
#define _SEUSETSOURCESOUNDMAXSPEED_H_

#include <deigde/undo/igdeUndo.h>

class seSourceSound;



/**
 * \brief Undo set source sound maximum speed.
 */
class seUSetSourceSoundMaxSpeed : public igdeUndo{
private:
	seSourceSound *pSource;
	
	float pOldSpeed;
	float pNewSpeed;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo action. */
	seUSetSourceSoundMaxSpeed( seSourceSound *source, float newSpeed );
	
protected:
	/** \brief Clean up undo action. */
	virtual ~seUSetSourceSoundMaxSpeed();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo action. */
	virtual void Undo();
	
	/** \brief Redo action. */
	virtual void Redo();
	/*@}*/
};

#endif
