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

#ifndef _DEOALSKIN_H_
#define _DEOALSKIN_H_

#include <dragengine/systems/modules/audio/deBaseAudioSkin.h>

class deAudioOpenAL;
class deoalASkin;

class deSkin;



/**
 * \brief Skin resource peer.
 */
class deoalSkin : public deBaseAudioSkin{
private:
	deAudioOpenAL &pOal;
	const deSkin &pSkin;
	deoalASkin *pASkin;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create skin peer. */
	deoalSkin( deAudioOpenAL &oal, const deSkin &skin );
	
	/** \brief Clean up skin peer. */
	virtual ~deoalSkin();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deAudioOpenAL &GetOal() const{ return pOal; }
	
	/** \brief Skin. */
	inline const deSkin &GetSkin() const{ return pSkin; }
	
	/** \brief Audio skin. */
	inline deoalASkin *GetASkin() const{ return pASkin; }
	/*@}*/
};

#endif
