/* 
 * Drag[en]gine IGDE Synthesizer Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _SEEFFECTLIST_H_
#define _SEEFFECTLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class seEffect;



/**
 * \brief Effect list.
 */
class seEffectList{
private:
	decObjectOrderedSet pEffects;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create effect list. */
	seEffectList();
	
	/** \brief Create copy of effect list. */
	seEffectList( const seEffectList &copy );
	
	/** \brief Clean up effect list. */
	~seEffectList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of effects. */
	int GetCount() const;
	
	/**
	 * \brief Effect at index.
	 * \throws deeInvalidParam \em index is less than 0 or greater than GetCount()-1.
	 */
	seEffect *GetAt( int index ) const;
	
	/**
	 * \brief Index of effect or -1 if not present in the list.
	 * \throws deeInvalidParam \em effect is \em NULL.
	 */
	int IndexOf( seEffect *effect ) const;
	
	/**
	 * \brief Determine if a effect is present in the list.
	 * \throws deeInvalidParam \em effect is \em NULL.
	 */
	bool Has( seEffect *effect ) const;
	
	/**
	 * \brief Add effect.
	 * \throws deeInvalidParam \em effect is \em NULL.
	 */
	void Add( seEffect *effect );
	
	/**
	 * \brief Insert effect.
	 * \throws deeInvalidParam \em effect is \em NULL.
	 * \throws deeInvalidParam \em index is less than 0 or greater than GetCount()-1.
	 */
	void Insert( seEffect *effect, int index );
	
	/**
	 * \brief Move effect.
	 * \throws deeInvalidParam \em effect is \em NULL.
	 * \throws deeInvalidParam \em from is less than 0 or greater than GetCount()-1.
	 * \throws deeInvalidParam \em to is less than 0 or greater than GetCount().
	 */
	void Move( seEffect *effect, int index );
	
	/**
	 * \brief Remove effect.
	 * \throws deeInvalidParam \em effect is \em NULL.
	 * \throws deeInvalidParam \em effect is not present in the list.
	 */
	void Remove( seEffect *effect );
	
	/** \brief Remove all effects. */
	void RemoveAll();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy effect list to this effect list. */
	seEffectList &operator=( const seEffectList &list );
	
	/** \brief Add effects from effect list if not present in this effect list. */
	seEffectList &operator+=( const seEffectList &list );
	/*@}*/
};

#endif
