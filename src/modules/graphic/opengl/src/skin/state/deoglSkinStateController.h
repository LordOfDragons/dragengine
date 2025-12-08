/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
	bool pHasConstructedProperties;
	
	
	
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
	void SetVideoPlayerCount(int count);
	
	/** Video player by index or \em NULL if not existing. */
	deVideoPlayer *GetVideoPlayerAt(int index) const;
	
	/** Shared video player by index or \em NULL if not existing. */
	deoglSharedVideoPlayer *GetSharedVideoPlayerAt(int index) const;
	
	/** Set video player at index or \em NULL if not existing. */
	void SetVideoPlayerAt(int index, deVideoPlayer::Ref videoPlayer);
	
	/** Set shared video player at index or \em NULL if not existing. */
	void SetSharedVideoPlayerAt(int index, deoglSharedVideoPlayer *videoPlayer);
	
	
	
	/** Has calculated properties. */
	inline bool GetHasCalculatedProperties() const{ return pHasCalculatedProperties; }
	
	/** Set if has calculated properties. */
	void SetHasCalculatedProperties(bool hasCalculatedProperties);
	
	
	
	/** Has constructed properties. */
	inline bool GetHasConstructedProperties() const{ return pHasConstructedProperties; }
	
	/** Set if has constructed properties. */
	void SetHasConstructedProperties(bool hasConstructedProperties);
	
	
	
	/** Requires sync every frame update. */
	bool RequiresSyncEveryFrameUpdate() const;
	
	/** Requires prepare renderables. */
	bool RequiresPrepareRenderables() const;
	
	/** Init skin state. */
	void Init(deoglSkinState &skinState, deoglRSkin *skin, deoglWorld *world);
	
	/** Init skin state. */
	void Init(deoglSkinState &skinState, deoglRSkin *skin, int textureIndex, deoglWorld *world);
	
	
	
	/** Reset time. */
	void ResetTime();
	
	/** Update time. */
	void AdvanceTime(float timeStep);
	
	/** Synchronize to render. */
	void SyncToRender();
	
	/** Clear. */
	void Clear();
	/*@}*/
	
	
	
private:
	void pUpdateSkinVideoPlayers(deoglSkinState &skinState, deoglSkinTexture &texture, deoglWorld &world);
};

#endif
