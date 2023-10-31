/* 
 * Drag[en]gine OpenAL Audio Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOALPMAXENVSLOTS_H_
#define _DEOALPMAXENVSLOTS_H_

#include "deoalParameterInt.h"


/**
 * Parameter maximum environment slot count.
 */
class deoalPMaxEnvSlots : public deoalParameterInt{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create parameter. */
	deoalPMaxEnvSlots( deAudioOpenAL &oal );
	
	/** Clean up parameter. */
	virtual ~deoalPMaxEnvSlots() override;
	/*@}*/
	
	
	
	/** \name Parameter Value */
	/*@{*/
	/** \brief Current value. */
	virtual int GetParameterInt() override;
	
	/** \brief Set current value. */
	virtual void SetParameterInt( int value ) override;
	/*@}*/
};

#endif
