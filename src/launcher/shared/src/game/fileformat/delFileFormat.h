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

#ifndef _DELFILEFORMAT_H_
#define _DELFILEFORMAT_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/common/collection/decTList.h>

#include <dragengine/systems/deModuleSystem.h>


/**
 * \brief File Format.
 */
class DE_DLL_EXPORT delFileFormat{
public:
	/** \brief List type. */
	using List = decTList<delFileFormat>;
	
	
private:
	deModuleSystem::eModuleTypes pType;
	decString pPattern;
	bool pSupported;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create file format. */
	delFileFormat();
	
	/** \brief Create file format. */
	delFileFormat(deModuleSystem::eModuleTypes type, const char *pattern);
	
	/** \brief Copy file format. */
	delFileFormat(const delFileFormat &other);
	delFileFormat& operator=(const delFileFormat &other);
	
	/** \brief Move file format. */
	delFileFormat(delFileFormat &&other) noexcept;
	delFileFormat& operator=(delFileFormat &&other) noexcept;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief File type. */
	inline deModuleSystem::eModuleTypes GetType() const{ return pType; }
	
	/** \brief Set type. */
	void SetType(deModuleSystem::eModuleTypes type);
	
	/** \brief Pattern. */
	inline const decString &GetPattern() const{ return pPattern; }
	
	/** \brief Set pattern. */
	void SetPattern(const char *pattern);
	
	/** \brief File format is supported. */
	inline bool GetSupported() const{ return pSupported; }
	
	/** \brief Set if file format is supported. */
	void SetSupported(bool supported);
	/*@}*/
};

#endif
