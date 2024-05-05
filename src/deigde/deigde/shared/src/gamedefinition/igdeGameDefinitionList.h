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

#ifndef _IGDEGAMEDEFINITIONLIST_H_
#define _IGDEGAMEDEFINITIONLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeGameDefinition;



/**
 * \brief Game definition list.
 */
class DE_DLL_EXPORT igdeGameDefinitionList{
private:
	decObjectOrderedSet pGameDefinitions;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game definition list. */
	igdeGameDefinitionList();
	
	/** \brief Create copy of game definition list. */
	igdeGameDefinitionList( const igdeGameDefinitionList &list );
	
	/** \brief Clean up game definition list. */
	~igdeGameDefinitionList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of game definitions. */
	int GetCount() const;
	
	/** \brief Game definition at index. */
	igdeGameDefinition *GetAt( int index ) const;
	
	/** \brief Game definition with identifier or NULL if absent. */
	igdeGameDefinition *GetWithID( const char *id ) const;
	
	/** \brief Index of game definition or -1 if absent. */
	int IndexOf( igdeGameDefinition *gameDefinition ) const;
	
	/** \brief Game definition is present. */
	bool Has( igdeGameDefinition *gameDefinition ) const;
	
	/** \brief Game definition with identifier is present. */
	bool HasWithID( const char *id ) const;
	
	/** \brief Add game definition. */
	void Add( igdeGameDefinition *gameDefinition );
	
	/** \brief Remove game definition. */
	void Remove( igdeGameDefinition *gameDefinition );
	
	/** \brief Remove all game definitions. */
	void RemoveAll();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy list. */
	igdeGameDefinitionList &operator=( const igdeGameDefinitionList &list );
	/*@}*/
};

#endif
