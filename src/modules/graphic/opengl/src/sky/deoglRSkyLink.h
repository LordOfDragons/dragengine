/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLRSKYLINK_H_
#define _DEOGLRSKYLINK_H_

#include <dragengine/common/math/decMath.h>

class deoglRSkyInstance;

class deSkyLink;



/**
 * Render sky link.
 */
class deoglRSkyLink{
private:
	const int pController;
	const int pRepeat;
	
	float *pSamples;
	int pSampleCount;
	
	float pUpperLimit;
	
	bool pDisabled;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create sky link. */
	deoglRSkyLink( const deSkyLink &link );
	
	/** Clean up sky link. */
	~deoglRSkyLink();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Controller is disabled. */
	inline bool IsDisabled() const{ return pDisabled; }
	
	/** Get value of link. */
	float GetValue( const deoglRSkyInstance &instance ) const;
	/*@}*/
	
	
	
private:
	void pUpdateSamples( const deSkyLink &link );
};

#endif
