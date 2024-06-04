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
