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

#ifndef _DEOALENVPROBELISTENERCACHED_H_
#define _DEOALENVPROBELISTENERCACHED_H_

#include "deoalEnvProbeListener.h"



/**
 * \brief Environment probe listener parameters.
 * 
 * Stores the result of an environment probe for a specific listener position. Each probe can
 * store parameters for mutliple listener positions. This allows to cache calculated listener
 * parameters. This class stores only the listener specific parameters. Listener independent
 * parameters are stored in the owner probe itself.
 * 
 * The listener position is relative to the environment probe position.
 * 
 * Listeners have a last used number. This is used to drop long time not used listener
 * parameters to save memory and increasing look up speed.
 */
class deoalEnvProbeListenerCached : public deoalEnvProbeListener {
private:
	decVector pPosition;
	unsigned short pLastUsed;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create environment probe listener. */
	deoalEnvProbeListenerCached();
	
	/** \brief Clean up environment probe listener. */
	~deoalEnvProbeListenerCached();
	/*@}*/
	
	
	
	/** \name Manegement */
	/*@{*/
	/** \brief Listener position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set listener position. */
	void SetPosition( const decVector &position );
	
	/** \brief Last used. */
	inline unsigned short GetLastUsed() const{ return pLastUsed; }
	
	/** \brief Set last used. */
	void SetLastUsed( unsigned short lastUsed );
	/*@}*/
};

#endif

