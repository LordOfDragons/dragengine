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

#ifndef _IGDELOADPARTICLEEMITTER_H_
#define _IGDELOADPARTICLEEMITTER_H_

#include "../utils/igdeBaseXML.h"

#include <dragengine/common/string/decString.h>

class igdeEnvironment;
class igdeGameProject;

class deParticleEmitter;
class deParticleEmitterController;
class deParticleEmitterType;
class decBaseFileReader;


/**
 * \brief Load particle emitter file in the Drag[en]gine Particle Emitter format.
 */
class DE_DLL_EXPORT igdeLoadParticleEmitter : public igdeBaseXML{
private:
	igdeEnvironment &pEnvironment;
	
	decString pName;
	decString pPattern;
	decString pDefaultExtension;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create loader. */
	igdeLoadParticleEmitter( igdeEnvironment &environment, deLogger *logger,
		const char *loggerSource );
	
	/** \brief Clean up loader. */
	virtual ~igdeLoadParticleEmitter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name to display in a file dialog. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name to display in a file dialog. */
	void SetName( const char *name );
	
	/** \brief File pattern to display in a file dialog. */
	inline const decString &GetPattern() const{ return pPattern; }
	
	/** \brief Set file pattern to display in a file dialog. */
	void SetPattern( const char *pattern );
	
	/** \brief Default file extension. */
	inline const decString &GetDefaultExtension() const{ return pDefaultExtension; }
	
	/** \brief Set default file extension. */
	void SetDefaultExtension( const char *extension );
	
	
	
	/** \brief Load particle emitter using a file reader. */
	void Load( const decString &path, deParticleEmitter &particleEmitter, decBaseFileReader &reader );
	/*@}*/
	
	
	
private:
	void pReadEmitter( const decXmlElementTag &root, const char *basePath,
		deParticleEmitter &particleEmitter );
	
	void pReadController( const decXmlElementTag &root, deParticleEmitter &particleEmitter );
	
	void pReadType( const decXmlElementTag &root, const char *basePath,
		deParticleEmitter &particleEmitter, deParticleEmitterType &type );
	
	void pReadParameter( const decXmlElementTag &root, deParticleEmitter &particleEmitter,
		deParticleEmitterType &type );
};

#endif
