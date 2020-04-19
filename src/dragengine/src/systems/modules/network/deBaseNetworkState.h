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

#ifndef _DEBASENETWORKSTATE_H_
#define _DEBASENETWORKSTATE_H_

class deNetworkValue;


/**
 * \brief Network Module State Peer.
 */
class deBaseNetworkState{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deBaseNetworkState();
	
	/** \brief Clean up peer. */
	virtual ~deBaseNetworkState();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Value has been added. */
	virtual void ValueAdded( int index, deNetworkValue *value );
	
	/** \brief Value has changed. */
	virtual void ValueChanged( int index, deNetworkValue *value );
	
	/** \brief Precision has changed. */
	virtual void PrecisionChanged( int index, deNetworkValue *value );
	/*@}*/
};

#endif
