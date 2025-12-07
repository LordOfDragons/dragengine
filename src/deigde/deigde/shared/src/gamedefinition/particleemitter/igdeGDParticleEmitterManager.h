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

#ifndef _IGDEGDPARTICLEEMITTERMANAGER_H_
#define _IGDEGDPARTICLEEMITTERMANAGER_H_

#include "igdeGDParticleEmitterList.h"
#include "../igdeGDCategory.h"

#include <dragengine/common/string/decString.h>

class igdeGameDefinition;



/**
 * \brief Game definition particle emitter manager.
 */
class DE_DLL_EXPORT igdeGDParticleEmitterManager{
private:
	igdeGDParticleEmitterList pEmitterList;
	igdeGDCategory::Ref pCategories;
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
	inline const igdeGDParticleEmitterList &GetEmitterList() const{return pEmitterList;}
	
	/** \brief Add emitter. */
	void AddEmitter(igdeGDParticleEmitter *emitter);
	
	/** \brief Remove emitter. */
	void RemoveEmitter(igdeGDParticleEmitter *emitter);
	
	/** \brief Remove all emitters. */
	void RemoveAllEmitters();
	
	/** \brief Top level category object. */
	inline igdeGDCategory *GetCategories() const{return pCategories;}
	
	/** \brief Path of default emitter. */
	inline const decString &GetDefaultPath() const{return pDefaultPath;}
	
	/** \brief Set path of default emitter. */
	void SetDefaultPath(const char *path);
	
	/**
	 * \brief Updates the particle emitter manager using another particle emitter manager.
	 * 
	 * Adds copies of particle emitters in the given particle emitter manager .
	 * If the particle emitter exists already it is replaced.
	 */
	void UpdateWith(const igdeGDParticleEmitterManager &particleEmitterManager);
	/*@}*/
};

#endif
