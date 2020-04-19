/* 
 * Drag[en]gine IGDE Particle Emitter Editor
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

#ifndef _PEEUTYPESETSIMULATIONTYPE_H_
#define _PEEUTYPESETSIMULATIONTYPE_H_

#include <deigde/undo/igdeUndo.h>
#include <dragengine/resources/particle/deParticleEmitterType.h>

class peeType;



/**
 * \brief Undo action set type simulation type.
 */
class peeUTypeSetSimulationType : public igdeUndo{
private:
	peeType *pType;
	
	deParticleEmitterType::eSimulationTypes pOldSimType;
	deParticleEmitterType::eSimulationTypes pNewSimType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo action. */
	peeUTypeSetSimulationType( peeType *type, deParticleEmitterType::eSimulationTypes newSimType );
	
protected:
	/** \brief Clean up undo action. */
	virtual ~peeUTypeSetSimulationType();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Undo action. */
	virtual void Undo();
	
	/** \brief Redo action. */
	virtual void Redo();
	/*@}*/
};

#endif
