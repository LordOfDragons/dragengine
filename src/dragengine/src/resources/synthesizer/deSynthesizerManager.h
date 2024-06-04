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

#ifndef _DESYNTHESIZERMANAGER_H_
#define _DESYNTHESIZERMANAGER_H_

#include "../deResourceManager.h"
#include "../deResourceList.h"

class deEngine;
class deSynthesizer;


/**
 * \brief Synthesizer resource manager.
 */
class DE_DLL_EXPORT deSynthesizerManager : public deResourceManager{
private:
	deResourceList pSynthesizers;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer resource manager. */
	deSynthesizerManager( deEngine *engine );
	
	/** \brief Clean up synthesizer resource manager and report leaking resources. */
	~deSynthesizerManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of synthesizer resources. */
	int GetSynthesizerCount() const;
	
	/** \brief Root synthesizer resource for iteration purpose. */
	deSynthesizer *GetRootSynthesizer() const;
	
	/** \brief Create new synthesizer object. */
	deSynthesizer *CreateSynthesizer();
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	/** \brief Synthesizer system peers of all stored resources have to be created. */
	virtual void SystemSynthesizerLoad();
	
	/** \brief Synthesizer system peers of all stored resources have to be freed. */
	virtual void SystemSynthesizerUnload();
	/*@}*/
	
	
	
	/**
	 * \name Resource only functions.
	 * 
	 * Those functions are only for resource objects and should never be
	 * called directly from an application.
	 */
	/*@{*/
	void RemoveResource( deResource *resource );
	/*@}*/
};

#endif
