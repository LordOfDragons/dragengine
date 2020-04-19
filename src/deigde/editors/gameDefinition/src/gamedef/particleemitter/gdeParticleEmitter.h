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

#ifndef _GDEPARTICLEEMITTER_H_
#define _GDEPARTICLEEMITTER_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decStringSet.h>

class gdeGameDefinition;


/**
 * \brief Particle emitter.
 */
class gdeParticleEmitter : public deObject{
private:
	gdeGameDefinition *pGameDefinition;
	
	decString pPath;
	decString pName;
	decString pDescription;
	decString pCategory;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create particle emitter. */
	gdeParticleEmitter();
	
	/** \brief Create particle emitter. */
	gdeParticleEmitter( const char *path, const char *name );
	
	/** \brief Create copy of particle emitter. */
	gdeParticleEmitter( const gdeParticleEmitter &particleEmitter );
	
	/** \brief Clean up particle emitter. */
	virtual ~gdeParticleEmitter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent game definition. */
	inline gdeGameDefinition *GetGameDefinition() const{ return pGameDefinition; }
	
	/** \brief Set parent game definition. */
	void SetGameDefinition( gdeGameDefinition *gamedef );
	
	
	
	/** \brief Path. */
	inline const decString &GetPath() const{ return pPath; }
	
	/** \brief Set path. */
	void SetPath( const char *path );
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	/** \brief Description. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description. */
	void SetDescription( const char *description );
	
	/** \brief Cathegory. */
	inline const decString &GetCategory() const{ return pCategory; }
	
	/** \brief Set cathegory. */
	void SetCategory( const char *category );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
