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

#ifndef _GDEOCINHERIT_H_
#define _GDEOCINHERIT_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>



/**
 * \brief Inherit from another game definition class.
 */
class gdeOCInherit : public deObject{
private:
	decString pName;
	decString pPropertyPrefix;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<gdeOCInherit> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create inherit object. */
	gdeOCInherit(const char *name);
	
	/** \brief Create copy of inherit object. */
	gdeOCInherit(const gdeOCInherit &inherit);
	
protected:
	/** \brief Cleans up the link. */
	virtual ~gdeOCInherit();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name of class to inherit if present. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name of class to inherit if present. */
	void SetName(const char *name);
	
	/** \brief Prefix to add to properties defined in inherited class. */
	inline const decString &GetPropertyPrefix() const{ return pPropertyPrefix; }
	
	/** \brief Set prefix to add to properties defined in inherited class. */
	void SetPropertyPrefix(const char *prefix);
	/*@}*/
};

#endif
