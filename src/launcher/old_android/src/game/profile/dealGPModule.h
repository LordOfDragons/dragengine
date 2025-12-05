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

#ifndef _DEALGPMODULE_H_
#define _DEALGPMODULE_H_

#include "dealGPMParameterList.h"
#include "../../deObject.h"
#include "../../common/string/decString.h"

class dealIEngineInstance;
class dealLauncher;



/**
 * \brief Game Profile Module.
 * 
 * Stores properties for a named module in a game profile. Can be applied to any version of the module.
 * 
 */
class dealGPModule : public deObject{
private:
	decString pName;
	dealGPMParameterList pParameterList;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<dealGPModule> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game profile module. */
	dealGPModule();
	
	/** \brief Create copy of game profile module. */
	dealGPModule( const dealGPModule &module );
	
	/** \brief Clean up game profile. */
	virtual ~dealGPModule();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set module name. */
	void SetName( const char *name );
	
	/** \brief Parameter list. */
	inline dealGPMParameterList &GetParameterList(){ return pParameterList; }
	inline const dealGPMParameterList &GetParameterList() const{ return pParameterList; }
	
	/** \brief Copy from another module. */
	void CopyFrom( const dealGPModule &module );
	
	/**
	 * \brief Set parameters for a module version if possible.
	 * \details Invalid parameters are silently ignored and do not cause an exception.
	 *          Version has to be set to the module version to operate on.
	 */
	void ApplyParameters( const char *version, dealLauncher &launcher, dealIEngineInstance &engineInstance ) const;
	/*@}*/
};

#endif
