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

#ifndef _DEALGPDISABLEMODULEVERSION_H_
#define _DEALGPDISABLEMODULEVERSION_H_

#include "../../deObject.h"
#include "../../common/string/decString.h"



/**
 * \brief Game profile module version disabling.
 */
class dealGPDisableModuleVersion : public deObject{
private:
	decString pName;
	decString pVersion;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<dealGPDisableModuleVersion> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create disabled module version. */
	dealGPDisableModuleVersion();
	
	/** \brief Create disabled module version. */
	dealGPDisableModuleVersion(const char *name, const char *version);
	
	/** \brief Create copy of disabled module version. */
	dealGPDisableModuleVersion(const dealGPDisableModuleVersion &copy);
	
	/** \brief Clean up disabled module version. */
	~dealGPDisableModuleVersion() override;
	/*@}*/
	
	
	
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
	 * \brief Disable module versions share the same name and version.
	 */
	bool operator==(const dealGPDisableModuleVersion &other) const;
	
	/**
	 * \brief Two disable module versions do not share the same name and version.
	 */
	bool operator!=(const dealGPDisableModuleVersion &other) const;
	
	/** \brief Set disable module version to another one. */
	dealGPDisableModuleVersion &operator=(const dealGPDisableModuleVersion &other);
	/*@}*/
};

#endif
