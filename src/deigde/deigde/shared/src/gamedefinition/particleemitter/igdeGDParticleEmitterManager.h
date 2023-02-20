/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEGDPARTICLEEMITTERMANAGER_H_
#define _IGDEGDPARTICLEEMITTERMANAGER_H_

#include "igdeGDParticleEmitterList.h"
#include "../igdeGDCategoryReference.h"

#include <dragengine/common/string/decString.h>

class igdeGameDefinition;



/**
 * \brief Game definition particle emitter manager.
 */
class DE_DLL_EXPORT igdeGDParticleEmitterManager{
private:
	igdeGDParticleEmitterList pEmitterList;
	igdeGDCategoryReference pCategories;
	decString pDefaultPath;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create particle emitter manager. */
	igdeGDParticleEmitterManager();
	
	/** \brief Clean up particle emitter manager. */
	~igdeGDParticleEmitterManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Emitter list. */
	inline const igdeGDParticleEmitterList &GetEmitterList() const{ return pEmitterList; }
	
	/** \brief Add emitter. */
	void AddEmitter( igdeGDParticleEmitter *emitter );
	
	/** \brief Remove emitter. */
	void RemoveEmitter( igdeGDParticleEmitter *emitter );
	
	/** \brief Remove all emitters. */
	void RemoveAllEmitters();
	
	/** \brief Top level category object. */
	inline igdeGDCategory *GetCategories() const{ return pCategories; }
	
	/** \brief Path of default emitter. */
	inline const decString &GetDefaultPath() const{ return pDefaultPath; }
	
	/** \brief Set path of default emitter. */
	void SetDefaultPath( const char *path );
	
	/**
	 * \brief Updates the particle emitter manager using another particle emitter manager.
	 * 
	 * Adds copies of particle emitters in the given particle emitter manager .
	 * If the particle emitter exists already it is replaced.
	 */
	void UpdateWith( const igdeGDParticleEmitterManager &particleEmitterManager );
	/*@}*/
};

#endif
