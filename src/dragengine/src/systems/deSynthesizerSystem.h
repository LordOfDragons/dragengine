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

#ifndef _DESYNTHESIZERSYSTEM_H_
#define _DESYNTHESIZERSYSTEM_H_

#include "deBaseSystem.h"
#include "../common/math/decMath.h"

class deEngine;
class deLoadableModule;
class deBaseSynthesizerModule;
class deSound;
class deSynthesizer;
class deSynthesizerInstance;


/**
 * \brief Synthesizer system.
 *
 * The synthesizer system is a single type system providing sound support
 * for the engine. An synthesizer module is able to carry out the actions
 * stored in an synthesizer object.
 */
class DE_DLL_EXPORT deSynthesizerSystem : public deBaseSystem{
private:
	deBaseSynthesizerModule *pActiveModule;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer system. */
	deSynthesizerSystem(deEngine *engine);
	
	/** \brief Clean up synthesizer system. */
	virtual ~deSynthesizerSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Activate synthesizer module or NULL if none is active. */
	inline deBaseSynthesizerModule *GetActiveModule() const{ return pActiveModule; }
	
	/** \brief Create and assign peer for sound using the active module. */
	void LoadSound(deSound *sound);
	
	/** \brief Create and assign peer for synthesizer using the active module. */
	void LoadSynthesizer(deSynthesizer *synthesizer);
	
	/** \brief Create and assign peer for synthesizer instance using the active module. */
	void LoadSynthesizerInstance(deSynthesizerInstance *instance);
	/*@}*/
	
	
	
	/** \name Overloadables */
	/*@{*/
	/**
	 * \brief Set active loadable module.
	 * 
	 * Do not forget to call the super function.
	 */
	virtual void SetActiveModule(deLoadableModule *module);
	
	/**
	 * \brief Clearcross references and links that could lead to memory leaks.
	 * 
	 * Do not forget to call the super function.
	 */
	virtual void ClearPermanents();
	
	/** \brief Carry out here actions right after the system started up. */
	virtual void PostStart();
	
	/** \brief Carry out here actions right before the system shuts down. */
	virtual void PreStop();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
