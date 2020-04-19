/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#ifndef _GDEOCENVMAPPROBELIST_H_
#define _GDEOCENVMAPPROBELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeOCEnvMapProbe;



/**
 * \brief Object class probe list.
 */
class gdeOCEnvMapProbeList{
private:
	decObjectOrderedSet pProbes;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object class environment map probe list. */
	gdeOCEnvMapProbeList();
	
	/** \brief Create copy of object class environment map probe list. */
	gdeOCEnvMapProbeList( const gdeOCEnvMapProbeList &list );
	
	/** \brief Clean up object class probe list. */
	~gdeOCEnvMapProbeList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of object class environment map probes. */
	int GetCount() const;
	
	/** \brief Object class environment map probe at position. */
	gdeOCEnvMapProbe *GetAt( int index ) const;
	
	/** \brief Index of named object class environment map probe or -1 if absent. */
	int IndexOf( gdeOCEnvMapProbe *probe ) const;
	
	/** \brief Object class environment map is present. */
	bool Has( gdeOCEnvMapProbe *probe ) const;
	
	/** \brief Add object class environment map probe. */
	void Add( gdeOCEnvMapProbe *probe );
	
	/** \brief Remove object class environment map probe. */
	void Remove( gdeOCEnvMapProbe *probe );
	
	/** \brief Remove all object class environment map probes. */
	void RemoveAll();
	
	/** \brief Set from another object class environment map probe list. */
	gdeOCEnvMapProbeList &operator=( const gdeOCEnvMapProbeList &list );
	/*@}*/
};

#endif
