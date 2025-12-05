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

#ifndef _IGDEGDPARTICLEEMITTER_H_
#define _IGDEGDPARTICLEEMITTER_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/image/deImage.h>

class deParticleEmitter;
class deParticleEmitterController;
class igdeGameDefinition;



/**
 * \brief Game definition particle emitter.
 */
class DE_DLL_EXPORT igdeGDParticleEmitter : public deObject{
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeGDParticleEmitter> Ref;


private:
	decString pPath;
	decString pName;
	decString pDescription;
	decString pCategory;
	deImage::Ref pPreviewImages[ 3 ];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create particle emitter. */
	igdeGDParticleEmitter( const char *path, const char *name );
	
	/** \brief Create copy of game definition particle emitter. */
	igdeGDParticleEmitter( const igdeGDParticleEmitter &particleEmitter );
	
protected:
	/** \brief Clean up particle emitter. */
	virtual ~igdeGDParticleEmitter();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
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
	
	/** \brief Category. */
	inline const decString &GetCategory() const{ return pCategory; }
	
	/** \brief Set cathegory. */
	void SetCategory( const char *category );
	
	/** \brief Preview image or NULL if not loaded yet. */
	deImage *GetPreviewImage( int size ) const;
	
	/** \brief Set preview image or NULL if not loaded yet. */
	void SetPreviewImage( int size, deImage *image );
	/*@}*/
};

#endif
