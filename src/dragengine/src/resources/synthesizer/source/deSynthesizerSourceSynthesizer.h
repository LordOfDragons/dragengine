/* 
 * Drag[en]gine Game Engine
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

#ifndef _DESYNTHESIZERSOURCESYNTHESIZER_H_
#define _DESYNTHESIZERSOURCESYNTHESIZER_H_

#include "deSynthesizerSource.h"

class deSound;


/**
 * \brief Synthesizer source using another synthesizer.
 */
class deSynthesizerSourceSynthesizer : public deSynthesizerSource{
private:
	deSynthesizer *pSynthesizer;
	
	int *pConnections;
	int pConnectionCount;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer source. */
	deSynthesizerSourceSynthesizer();
	
protected:
	/**
	 * \brief Clean up synthesizer source.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deSynthesizerSourceSynthesizer();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Synthesizer or NULL if silent. */
	inline deSynthesizer *GetSynthesizer() const{ return pSynthesizer; }
	
	/** \brief Set synthesizer or NULL if silent. */
	void SetSynthesizer( deSynthesizer *synthesizer );
	
	
	
	/**
	 * \brief Update connection count.
	 * 
	 * You have to call this method after you changed the child synthesizer without
	 * first removing it from this synthesizer.
	 */
	void UpdateConnectionCount();
	
	/**
	 * \brief Number of connections.
	 * 
	 * This is the number of controllers in the child synthesizer if present or 0 if absent.
	 */
	inline int GetConnectionCount() const{ return pConnectionCount; }
	
	/** \brief Index of local controller for child controller. */
	int GetConnectionAt( int childController ) const;
	
	/** \brief Set index of local controller for child controller. */
	void SetConnectionAt( int childController, int localController );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit source. */
	virtual void Visit( deSynthesizerSourceVisitor &visitor );
	/*@}*/
};

#endif
