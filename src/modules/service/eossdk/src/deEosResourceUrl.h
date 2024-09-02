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

#ifndef _DEEOSRESOURCEURL_H_
#define _deEosRESOURCEURL_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringList.h>


/**
 * Resource URL.
 */
class deEosResourceUrl{
public:
	decString url;
	decString type;
	decStringList components;
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create resource URL. */
	deEosResourceUrl( const decString &url );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Component at index or throws invalid URL exceptionn. */
	const decString &getComponentAt( int index, const char *paramName = "url" ) const;
	
	static decString FormatUrl( const char *part1, const char *part2,
		const char *part3, const char *part4 );
	/*@}*/
};

#endif
