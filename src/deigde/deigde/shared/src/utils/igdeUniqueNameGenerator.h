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

#ifndef _IGDEUNIQUENAMEGENERATOR_H_
#define _IGDEUNIQUENAMEGENERATOR_H_

#include <functional>
#include <dragengine/common/string/decString.h>


/**
 * \brief Unique name generation.
 * 
 * Simplifies the process of obtaining a unique name for an object.
 */
class DE_DLL_EXPORT igdeUniqueNameGenerator{
private:
	int pStartNumber = 2;
	decString pPrefix = " #";
	std::function<bool(const decString &)> pIsUnique;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create unique name generator. */
	igdeUniqueNameGenerator();
	
	/** \brief Create unique name generator. */
	explicit igdeUniqueNameGenerator(const std::function<bool(const decString &)> &isUnique);
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Start number. */
	inline int GetStartNumber() const{ return pStartNumber; }
	
	/** \brief Set start number. */
	void SetStartNumber(int startNumber);
	
	/** \brief Prefix. */
	inline const decString &GetPrefix() const{ return pPrefix; }
	
	/** \brief Set prefix. */
	void SetPrefix(const decString &prefix);
	
	/** \brief Unique function. */
	inline const std::function<bool(const decString &)> &GetIsUnique() const{ return pIsUnique; }
	
	/** \brief Set unique function. */
	void SetIsUnique(const std::function<bool(const decString &)> &isUnique);
	
	
	/** \brief Get unique name. */
	decString Generate(const decString &name) const;
	
	/** \brief Get unique name with allowed secondary name. */
	decString Generate(const decString &name, const decString &allowedName) const;
};

#endif
