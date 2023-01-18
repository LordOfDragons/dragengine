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

#ifndef _DEOGLSKINSTATECONTROLLER_H_
#define _DEOGLSKINSTATECONTROLLER_H_

#include <dragengine/common/collection/decObjectList.h>

class deoglRSkin;
class deoglSharedVideoPlayer;
class deoglSharedVideoPlayerList;
class deoglSkinState;
class deoglSkinTexture;
class deoglWorld;
class deVideoPlayer;



/**
 * Skin state controller.
 */
class deoglSkinStateController{
private:
	deoglSharedVideoPlayer **pSharedVideoPlayers;
	int pSharedVideoPlayerCount;
	
	decObjectList pVideoPlayers;
	
	bool pHasCalculatedProperties;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin state controller. */
	deoglSkinStateController();
	
	/** Clean up skin state controller. */
	~deoglSkinStateController();
	/*@}*/
	
	
	
	/** \name Management. */
	/*@{*/
	/** Number of shared video players. */
	inline int GetVideoPlayerCount() const{ return pSharedVideoPlayerCount; }
	
	/** Set number of video players. */
	void SetVideoPlayerCount( int count );
	
	/** Video player by index or \em NULL if not existing. */
	deVideoPlayer *GetVideoPlayerAt( int index ) const;
	
	/** Shared video player by index or \em NULL if not existing. */
	deoglSharedVideoPlayer *GetSharedVideoPlayerAt( int index ) const;
	
	/** Set video player at index or \em NULL if not existing. */
	void SetVideoPlayerAt( int index, deVideoPlayer *videoPlayer );
	
	/** Set shared video player at index or \em NULL if not existing. */
	void SetSharedVideoPlayerAt( int index, deoglSharedVideoPlayer *videoPlayer );
	
	
	
	/** Has calculated properties. */
	inline bool GetHasCalculatedProperties() const{ return pHasCalculatedProperties; }
	
	/** Set if has calculated properties. */
	void SetHasCalculatedProperties( bool hasCalculatedProperties );
	
	
	
	/** Requires sync every frame update. */
	bool RequiresSyncEveryFrameUpdate() const;
	
	/** Requires prepare renderables. */
	bool RequiresPrepareRenderables() const;
	
	/** Init skin state. */
	void Init( deoglSkinState &skinState, deoglRSkin *skin, deoglWorld *world );
	
	/** Init skin state. */
	void Init( deoglSkinState &skinState, deoglRSkin *skin, int textureIndex, deoglWorld *world );
	
	
	
	/** Reset time. */
	void ResetTime();
	
	/** Update time. */
	void AdvanceTime( float timeStep );
	
	/** Synchronize to render. */
	void SyncToRender();
	
	/** Clear. */
	void Clear();
	/*@}*/
	
	
	
private:
	void pUpdateSkinVideoPlayers( deoglSkinState &skinState, deoglSkinTexture &texture, deoglWorld &world );
};

#endif
