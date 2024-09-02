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

#ifndef _PROJTRPROFILELIST_H_
#define _PROJTRPROFILELIST_H_

#include <dragengine/common/collection/decObjectList.h>

class projTRProfile;
class deEngine;



/**
 * \brief Profiles
 */
class projTRProfileList{
private:
	decObjectList pProfiles;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create profiles. */
	projTRProfileList();
	
	/** \brief Clean up profiles. */
	~projTRProfileList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of profiles. */
	int GetCount() const;
	
	/** \brief Profile at index. */
	projTRProfile *GetAt( int index ) const;
	
	/** \brief Named profile or \em NULL if absent. */
	projTRProfile *GetNamed( const char *name ) const;
	
	/** \brief Profile is present. */
	bool Has( projTRProfile *profile ) const;
	
	/** \brief Named profile is present. */
	bool HasNamed( const char *name ) const;
	
	/** \brief Index of profile or -1 if absent. */
	int IndexOf( projTRProfile *profile ) const;
	
	/** \brief Index of named profile or -1 if absent. */
	int IndexOfNamed( const char *name ) const;
	
	/** \brief Add profile. */
	void Add( projTRProfile *profile );
	
	/** \brief Remove profile. */
	void Remove( projTRProfile *profile );
	
	/** \brief Remove all profiles. */
	void RemoveAll();
	
	
	
	/** \brief Validate profiles. */
	void ValidateProfiles( deEngine &engine );
	/*@}*/
};

#endif
