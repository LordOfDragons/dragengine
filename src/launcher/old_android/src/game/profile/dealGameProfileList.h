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

#ifndef _DEALGAMEPROFILELIST_H_
#define _DEALGAMEPROFILELIST_H_

#include "../../common/collection/decObjectList.h"

class dealGameProfile;
class dealLauncher;



/**
 * \brief Game Profile List.
 */
class dealGameProfileList{
private:
	decObjectList pProfiles;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create profile list. */
	dealGameProfileList();
	
	/** \brief Clean up profile list. */
	~dealGameProfileList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of profiles. */
	int GetCount() const;
	
	/** \brief Profile at the given position. */
	dealGameProfile *GetAt( int index ) const;
	
	/** \brief Profile with the given name or \em NULL if not found. */
	dealGameProfile *GetNamed( const char *name ) const;
	
	/** \brief Profile exists. */
	bool Has( dealGameProfile *profile ) const;
	
	/** \brief Profile with the given name exists. */
	bool HasNamed( const char *name ) const;
	
	/** \breif Index of a profile or -1 if not found. */
	int IndexOf( dealGameProfile *profile ) const;
	
	/** \brief Index of a profile with the given name or -1 if not found. */
	int IndexOfNamed( const char *name ) const;
	
	/** \brief Add profile. */
	void Add( dealGameProfile *profile );
	
	/** \brief Remove profile. */
	void Remove( dealGameProfile *profile );
	
	/** \brief Remove all profiles. */
	void RemoveAll();
	
	/** \brief Validate profiles. */
	void ValidateProfiles( dealLauncher &launcher );
	/*@}*/
};

#endif
