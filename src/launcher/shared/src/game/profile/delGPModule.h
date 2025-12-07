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

#ifndef _DELGPMODULE_H_
#define _DELGPMODULE_H_

#include "delGPMParameterList.h"
#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

class delEngineInstance;
class delLauncher;


/**
 * \brief Game Profile Module.
 * 
 * Stores properties for a named module in a game profile. Can be applied to
 * any version of the module.
 */
class DE_DLL_EXPORT delGPModule : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<delGPModule> Ref;
	
	
private:
	decString pName;
	delGPMParameterList pParameters;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game profile module. */
	delGPModule(const char *name = "");
	
	/** \brief Create copy of game profile module. */
	delGPModule(const delGPModule &module);
	
protected:
	/** \brief Clean up game profile. */
	virtual ~delGPModule();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Module name. */
	inline const decString &GetName() const{return pName;}
	
	/** \brief Set module name. */
	void SetName(const char *name);
	
	/** \brief Parameters. */
	inline delGPMParameterList &GetParameters(){return pParameters;}
	inline const delGPMParameterList &GetParameters() const{return pParameters;}
	
	/**
	 * \brief Set parameters for a module version if possible.
	 * 
	 * Invalid parameters are silently ignored and do not cause an exception.
	 * Version has to be set to the module version to operate on.
	 */
	void ApplyParameters(const char *version, delLauncher &launcher,
		delEngineInstance &engineInstance) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy module. */
	delGPModule &operator=(const delGPModule &module);
	/*@}*/
};

#endif
