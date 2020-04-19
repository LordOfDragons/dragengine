/* 
 * Drag[en]gine IGDE Game Definition Editor
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


#ifndef _GDEUOCPARTICLEEMITTERTOGGLECASTING_H_
#define _GDEUOCPARTICLEEMITTERTOGGLECASTING_H_

#include <deigde/undo/igdeUndo.h>

class gdeOCParticleEmitter;
class gdeObjectClass;



/**
 * \brief Undo action object class particle emitter toggle activated.
 */
class gdeUOCParticleEmitterToggleCasting : public igdeUndo{
private:
	gdeObjectClass *pObjectClass;
	gdeOCParticleEmitter *pParticleEmitter;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo action. */
	gdeUOCParticleEmitterToggleCasting( gdeObjectClass *objectClass, gdeOCParticleEmitter *particleEmitter );
	
protected:
	/** \brief Clean up undo action. */
	virtual ~gdeUOCParticleEmitterToggleCasting();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo. */
	virtual void Undo();
	
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
};

#endif
