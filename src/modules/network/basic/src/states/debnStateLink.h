/* 
 * Drag[en]gine Basic Network Module
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

#ifndef _DEBNSTATELINK_H_
#define _DEBNSTATELINK_H_

class debnState;
class debnConnection;



/**
 * \brief State link.
 */
class debnStateLink{
public:
	/** \brief Link states. */
	enum eLinkStates{
		elsDown,
		elsListening,
		elsUp
	};
	
	
	
private:
	debnState *pState;
	int pIdentifier;
	int pLinkState;
	
	debnConnection &pConnection;
	bool pChanged;
	
	bool *pValueChanged;
	int pValueCount;
	
	debnStateLink *pPreviousLink;
	debnStateLink *pNextLink;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create state link. */
	debnStateLink( debnState *bnState, debnConnection &connection );
	
	/** \brief Clean up state link. */
	~debnStateLink();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief State or NULL if dropped. */
	inline debnState *GetState() const{ return pState; }
	
	/** \brief Drop state. */
	void DropState();
	
	/** \brief Unique identifier. */
	inline int GetIdentifier() const{ return pIdentifier; }
	
	/** \brief Set unique identifier. */
	void SetIdentifier( int identifier );
	
	/** \brief Link state. */
	inline int GetLinkState() const{ return pLinkState; }
	
	/** \brief Set link state. */
	void SetLinkState( int linkState );
	
	/** \brief Connection. */
	inline debnConnection &GetConnection() const{ return pConnection; }
	
	/** \brief State link changed. */
	inline bool GetChanged() const{ return pChanged; }
	
	/** \brief Set if state link changed. */
	void SetChanged( bool changed );
	
	/** \brief Value changed. */
	bool GetValueChangedAt( int index ) const;
	
	/** \brief Set if value changed. */
	void SetValueChangedAt( int index, bool changed );
	
	/** \brief One or more values are marked changed. */
	bool HasChangedValues() const;
	
	/** \brief Reset changed of all values and state link. */
	void ResetChanged();
	/*@}*/
	
	
	
	/** \name Linked List ( for debnLinkQueue only ) */
	/*@{*/
	inline debnStateLink *GetPreviousLink() const{ return pPreviousLink; }
	void SetPreviousLink( debnStateLink *link );
	inline debnStateLink *GetNextLink() const{ return pNextLink; }
	void SetNextLink( debnStateLink *link );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
