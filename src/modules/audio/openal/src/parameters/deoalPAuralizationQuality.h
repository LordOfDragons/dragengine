/* 
 * Drag[en]gine OpenAL Audio Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOALPAURALIZATIONQUALITY_H_
#define _DEOALPAURALIZATIONQUALITY_H_

#include "deoalParameter.h"


/**
 * Parameter Auralization mode.
 */
class deoalPAuralizationQuality : public deoalParameter{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create parameter. */
	deoalPAuralizationQuality( deAudioOpenAL &oal );
	
	/** Clean up parameter. */
	virtual ~deoalPAuralizationQuality();
	/*@}*/
	
	
	
	/** \name Parameter Value */
	/*@{*/
	/** Current value. */
	virtual decString GetParameterValue();
	
	/** Set current value. */
	virtual void SetParameterValue( const char *value );
	/*@}*/
};

#endif
