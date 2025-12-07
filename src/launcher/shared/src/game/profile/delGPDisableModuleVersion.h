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

#ifndef _DELGPDISABLEMODULEVERSION_H_
#define _DELGPDISABLEMODULEVERSION_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>



/**
 * \brief Game profile module version disabling.
 */
class DE_DLL_EXPORT delGPDisableModuleVersion : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<delGPDisableModuleVersion> Ref;
	
	
private:
	decString pName;
	decString pVersion;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game profile disable module version. */
	delGPDisableModuleVersion();
	
	/** \brief Create game profile disable module version. */
	delGPDisableModuleVersion(const char *name, const char *version);
	
	/** \brief Create copy of game profile disable module version. */
	delGPDisableModuleVersion(const delGPDisableModuleVersion &copy);
	
protected:
	/** \brief Clean up game profile disable module version. */
	virtual ~delGPDisableModuleVersion();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Module name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set module name. */
	void SetName(const char *name);
	
	/** \brief Module version. */
	inline const decString &GetVersion() const{ return pVersion; }
	
	/** \brief Set module version. */
	void SetVersion(const char *version);
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/**
	 * \brief Determine if two disable module versions are the same.
	 * 
	 * Two disable module versions are the same if they have the same name and version.
	 */
	bool operator==(const delGPDisableModuleVersion &other) const;
	
	/**
	 * \brief Determine if two disable module versions are not the same.
	 * 
	 * Two disable module versions are the same if they have the same name and version.
	 */
	bool operator!=(const delGPDisableModuleVersion &other) const;
	
	/** \brief Set disable module version to another one. */
	delGPDisableModuleVersion &operator=(const delGPDisableModuleVersion &other);
	/*@}*/
};

#endif
