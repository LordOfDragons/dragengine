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

#ifndef _DESKINPROPERTYVIDEO_H_
#define _DESKINPROPERTYVIDEO_H_

#include "deSkinProperty.h"
#include "../../video/deVideoReference.h"
#include "../../../common/string/decString.h"


/**
 * \brief Skin texture video property.
 * 
 * Uses frames of a video as texture. The video is automatically played back
 * by the Graphic Module. By default the playback time is shared across all
 * instances this property is used in. This ensures all instances fit together
 * if used next to each other no matter when the user of the skin has been
 * added to the world. For certain use cases though like special effects it
 * is possible to use unique playback time for each instance.
 */
class deSkinPropertyVideo : public deSkinProperty{
private:
	decString pPath;
	deVideoReference pVideo;
	bool pSharedTime;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create skin property. */
	deSkinPropertyVideo( const char *type );
	
	/** \brief Clean up skin property. */
	virtual ~deSkinPropertyVideo();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Video path. */
	inline const decString &GetPath() const{ return pPath; }
	
	/** \brief Set video path. */
	void SetPath( const char *path );
	
	/** \brief Video or NULL if not set. */
	inline deVideo *GetVideo() const{ return pVideo; }
	
	/** \brief Set video or NULL if not set. */
	void SetVideo( deVideo *video );
	
	/** \brief Playback time is shared across instances. */
	inline bool GetSharedTime() const{ return pSharedTime; }
	
	/** \brief Set if playback time is shared across instances. */
	void SetSharedTime( bool shareTime );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit property. */
	virtual void Visit( deSkinPropertyVisitor &visitor );
	/*@}*/
};

#endif
