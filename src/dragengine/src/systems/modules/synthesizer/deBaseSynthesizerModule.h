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

#ifndef _DEBASESYNTHESIZERMODULE_H_
#define _DEBASESYNTHESIZERMODULE_H_

#include "../deBaseModule.h"

class deBaseSynthesizerSound;
class deBaseSynthesizerSynthesizer;
class deBaseSynthesizerSynthesizerInstance;

class deSound;
class deSynthesizer;
class deSynthesizerInstance;


/**
 * \brief Base synthesizer module.
 */
class DE_DLL_EXPORT deBaseSynthesizerModule : public deBaseModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deBaseSynthesizerModule(deLoadableModule &loadableModule);
	
	/** \brief Clean up module. */
	virtual ~deBaseSynthesizerModule();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/**
	 * \brief Init module and prepare for synthesizer processing.
	 * \returns true on success or false on failure.
	 */
	virtual bool Init() = 0;
	
	/**
	 * \brief Clean up module.
	 * 
	 * All resources have to be freed and running threads stopped or killed if needed.
	 */
	virtual void CleanUp() = 0;
	/*@}*/
	
	
	
	/** \name Synthesizer Management */
	/*@{*/
	/** \brief Create peer for sound. */
	virtual deBaseSynthesizerSound *CreateSound(deSound *sound) = 0;
	
	/**
	 * \brief Create peer for synthesizer or NULL if no explicit peer is required.
	 * 
	 * Default implementation return NULL.
	 */
	virtual deBaseSynthesizerSynthesizer *CreateSynthesizer(deSynthesizer *synthesizer) = 0;
	
	/**
	 * \brief Create peer for synthesizer instance or NULL if no explicit peer is required.
	 * 
	 * Default implementation return NULL.
	 */
	virtual deBaseSynthesizerSynthesizerInstance *CreateSynthesizerInstance(
		deSynthesizerInstance *instance) = 0;
	/*@}*/
};

#endif
