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

#ifndef _GDEFILEPATTERN_H_
#define _GDEFILEPATTERN_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>



/**
 * \brief File pattern.
 */
class gdeFilePattern : public deObject{
private:
	decString pName;
	decString pPattern;
	decString pDefExtension;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create file pattern. */
	gdeFilePattern();
	
	/** \brief Create file pattern. */
	gdeFilePattern( const char *name, const char *pattern, const char *defaultExtension );
	
	/** \brief Create copy of file pattern. */
	gdeFilePattern( const gdeFilePattern &filePattern );
	
	/** \brief Clean up file pattern. */
	virtual ~gdeFilePattern();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	/** \brief File pattern. */
	inline const decString &GetPattern() const{ return pPattern; }
	
	/** \brief Set file pattern. */
	void SetPattern( const char *pattern );
	
	/** \brief Default extension. */
	inline const decString &GetDefaultExtension() const{ return pDefExtension; }
	
	/** \brief Set default extension. */
	void SetDefaultExtension( const char *extension );
	/*@}*/
};

#endif
