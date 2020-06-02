/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MECLREATTACHDECALS_H_
#define _MECLREATTACHDECALS_H_

#include "meCLCollect.h"

class igdeWObject;


/**
 * \brief Rettach decals.
 */
class meCLReattachDecals : public meCLCollect{
public:
	/**
	 * \brief Helper class for potential NULL world.
	 */
	class Helper{
	private:
		meCLReattachDecals *pVisitor;
		
	public:
		Helper( meWorld *world );
		~Helper();
		
		void ReattachDecals();
		
		void Collect( const decDVector &position, const decVector &minExtend,
			const decVector &maxExtend, const decQuaternion &orientation );
		
		void Collect( igdeWObject &wobject );
	};
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	meCLReattachDecals( meWorld *world );
	
	/** \brief Clean visitor. */
	virtual ~meCLReattachDecals();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Reattach collected decals. */
	void ReattachDecals();
	
	/** \brief Collect decals. */
	void Collect( const decDVector &position, const decVector &minExtend,
		const decVector &maxExtend, const decQuaternion &orientation );
	
	/** \brief Collect decals. */
	void Collect( igdeWObject &wobject );
	/*@}*/
};

#endif
