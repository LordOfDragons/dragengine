/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MEMUSIC_H_
#define _MEMUSIC_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/sound/deSpeaker.h>

class meWorld;


/**
 * Play music to evaluate music/sound volume balancing.
 */
class meMusic : public deObject{
public:
	typedef deTObjectReference<meMusic> Ref;
	
	
private:
	meWorld &pWorld;
	decString pPath;
	deSpeaker::Ref pSpeaker;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates music. */
	meMusic( meWorld &world );
	
	/** Clean up music. */
	virtual ~meMusic() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Parent world. */
	inline meWorld &GetWorld() const{ return pWorld; }
	
	/** Music path. */
	inline const decString &GetPath() const{ return pPath; }
	
	/** Set music path. */
	void SetPath( const char *path );
	
	/** Volume. */
	float GetVolume() const;
	
	/** Set volume. */
	void SetVolume( float volume );
	
	/** Play. */
	void Play();
	
	/** Pause. */
	void Pause();
	
	/** Stop. */
	void Stop();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
