/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
