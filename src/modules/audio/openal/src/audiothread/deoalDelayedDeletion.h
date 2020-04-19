/* 
 * Drag[en]gine OpenAL Audio Module
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

#ifndef _DEOALDELAYEDDELETION_H_
#define _DEOALDELAYEDDELETION_H_

class deoalAudioThread;


/** \brief Delete OpenAL objects properly after audioing is done in the audio thread. */
class deoalDelayedDeletion{
private:
	/** \brief Previous entry in the linked list or \em NULL. */
	deoalDelayedDeletion *pLLPrev;
	
	/** \brief Next entry in the linked list or \em NULL. */
	deoalDelayedDeletion *pLLNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create delayed deletion object. */
	deoalDelayedDeletion();
	
	/** \brief Clean up delayed file write object. */
	virtual ~deoalDelayedDeletion();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Delete objects. */
	virtual void DeleteObjects( deoalAudioThread &audioThread ) = 0;
	/*@}*/
	
	
	
	/**
	 * \name Linked list.
	 * \warning For use by deoalDelayedOperations only.
	 */
	/*@{*/
	/** \brief Previous entry in the linked list or \em NULL. */
	inline deoalDelayedDeletion *GetLLPrev() const{ return pLLPrev; }
	
	/** \brief Set previous entry in the linked list or \em NULL. */
	void SetLLPrev( deoalDelayedDeletion *prev );
	
	/** \brief Next entry in the linked list or \em NULL. */
	inline deoalDelayedDeletion *GetLLNext() const{ return pLLNext; }
	
	/** \brief Set next entry in the linked list or \em NULL. */
	void SetLLNext( deoalDelayedDeletion *next );
	/*@}*/
};

#endif
