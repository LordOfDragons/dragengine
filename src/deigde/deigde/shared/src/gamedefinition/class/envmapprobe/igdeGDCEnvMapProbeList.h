/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEGDCENVMAPPROBELIST_H_
#define _IGDEGDCENVMAPPROBELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeGDCEnvMapProbe;



/**
 * \brief Game Definition Class Environment Map Probe List.
 */
class igdeGDCEnvMapProbeList{
private:
	decObjectOrderedSet pProbes;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	igdeGDCEnvMapProbeList();
	
	/** \brief Create copy of a game definition environment map probe list. */
	igdeGDCEnvMapProbeList( const igdeGDCEnvMapProbeList &list );
	
	/** \brief Clean up list. */
	~igdeGDCEnvMapProbeList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of environment map probes. */
	int GetCount() const;
	
	/** \brief Environment map probe by index. */
	igdeGDCEnvMapProbe *GetAt( int index ) const;
	
	/** \brief Index of an environment map probe or -1 if not found. */
	int IndexOf( igdeGDCEnvMapProbe *probe ) const;
	
	/** \brief Determines if an environment map probe exists. */
	bool Has( igdeGDCEnvMapProbe *probe ) const;
	
	/** \brief Adds an environment map probe. */
	void Add( igdeGDCEnvMapProbe *probe );
	
	/** \brief Inserts a new environment map probe. */
	void InsertAt( igdeGDCEnvMapProbe *probe, int index );
	
	/** \brief Moves an environment map probe to a new position. */
	void MoveTo( igdeGDCEnvMapProbe *probe, int index );
	
	/** \brief Removes an environment map probe. */
	void Remove( igdeGDCEnvMapProbe *probe );
	
	/** \brief Removes all environment map probes. */
	void RemoveAll();
	
	/** \brief Set this list to a deep copy of another list. */
	void SetToDeepCopyFrom( const igdeGDCEnvMapProbeList &list );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set list from another list. */
	igdeGDCEnvMapProbeList &operator=( const igdeGDCEnvMapProbeList &list );
	/*@}*/
};

#endif
