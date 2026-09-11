/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _IGDEENVIRONMENTLISTENER_H_
#define _IGDEENVIRONMENTLISTENER_H_

#include <dragengine/deObject.h>


/**
 * \brief Environment listener.
 */
class igdeEnvironmentListener : public deObject{
public:
	/** \brief Strong reference. */
	using Ref = deTObjectReference<igdeEnvironmentListener>;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	
	/** \brief Create listener. */
	igdeEnvironmentListener();
	
protected:
	/** \brief Clean up listener. */
	~igdeEnvironmentListener() override;
	/*@}*/
	
	
public:
	/** \name Events */
	/*@{*/
	/** \brief Game engine is about to be started. */
	virtual void OnBeforeEngineStart();
	
	/** \brief Game engine has been started. */
	virtual void OnAfterEngineStart();
	
	/** \brief Game engine is about to be stopped. */
	virtual void OnBeforeEngineStop();
	
	/** \brief Game engine has been stopped. */
	virtual void OnAfterEngineStop();
	
	/** \brief Game like frame update. */
	virtual void OnFrameUpdate(float elapsed);
	
	/** \brief Configuration changed. */
	virtual void OnConfigurationChanged();
	
	/** \brief Game project has changed. */
	virtual void OnGameProjectChanged();
	
	/** \brief Project game definition changed. */
	virtual void OnProjectGameDefinitionChanged();
};

#endif
