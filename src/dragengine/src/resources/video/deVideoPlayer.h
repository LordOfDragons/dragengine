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

#ifndef _DEVIDEOPLAYER_H_
#define _DEVIDEOPLAYER_H_

#include "deVideo.h"
#include "../deResource.h"
#include "../../common/math/decMath.h"

class deVideoPlayerManager;
class deBaseGraphicVideoPlayer;
class deBaseAudioVideoPlayer;


/**
 * \brief Play back video resources.
 */
class DE_DLL_EXPORT deVideoPlayer : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deVideoPlayer> Ref;
	
	
public:
	/** \brief Play state. */
	enum ePlayState{
		/** \brief Playback is stopped. */
		epsStopped,
		
		/** \brief Video is being played back. */
		epsPlaying,
		
		/** \brief Playback is paused. */
		epsPaused
	};
	
	
	
private:
	deVideo::Ref pVideo;
	
	ePlayState pPlayState;
	bool pLooping;
	float pPlayFrom;
	float pPlayTo;
	float pPlaySpeed;
	float pPlayPosition;
	
	deBaseGraphicVideoPlayer *pPeerGraphic;
	deBaseAudioVideoPlayer *pPeerAudio;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create video player. */
	deVideoPlayer(deVideoPlayerManager *manager);
	
protected:
	/**
	 * \brief Clean up video player.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~deVideoPlayer() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Video or NULL if not set. */
	inline deVideo *GetVideo() const{ return pVideo; }
	
	/** \brief Set video or NULL if not set. */
	void SetVideo(deVideo *video);
	
	
	
	/** \brief Video is looping instead of being played back once. */
	inline bool GetLooping() const{ return pLooping; }
	
	/** \brief Set if video is looping instead of being played back once. */
	void SetLooping(bool looping);
	
	
	
	/** \brief Start play position in seconds. */
	inline float GetPlayFrom() const{ return pPlayFrom; }
	
	/** \brief End play position in seconds. */
	inline float GetPlayTo() const{ return pPlayTo; }
	
	/** \brief Set play range. */
	void SetPlayRange(float fromTime, float toTime);
	
	/** \brief Play speed. */
	inline float GetPlaySpeed() const{ return pPlaySpeed; }
	
	/** \brief Set play speed. */
	void SetPlaySpeed(float playSpeed);
	
	/** \brief Current play position in seconds. */
	inline float GetPlayPosition() const{ return pPlayPosition; }
	
	/** \brief Set current play position in seconds. */
	void SetPlayPosition(float position);
	
	
	
	/** \brief play state. */
	inline ePlayState GetPlayState() const{ return pPlayState; }
	
	/** \brief Video is playing back. */
	inline bool GetPlaying() const{ return pPlayState == epsPlaying; }
	
	/** \brief Video is paused. */
	inline bool GetPaused() const{ return pPlayState == epsPaused; }
	
	/** \brief Video is stopped. */
	inline bool GetStopped() const{ return pPlayState == epsStopped; }
	
	/** \brief Set play state. */
	void SetPlayState(ePlayState playState);
	
	/** \brief Start playing back if not already playing back. */
	void Play();
	
	/** \brief Stops playing back if not stopped. */
	void Stop();
	
	/** \brief Pause playing back if not paused. */
	void Pause();
	
	
	
	/**
	 * \brief Update video player.
	 * 
	 * Advances the play position if playing back.
	 */
	void Update(float elapsed);
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic system peer. */
	inline deBaseGraphicVideoPlayer *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphic system peer. */
	void SetPeerGraphic(deBaseGraphicVideoPlayer *peer);
	
	/** \brief Audio system peer. */
	inline deBaseAudioVideoPlayer *GetPeerAudio() const{ return pPeerAudio; }
	
	/** \brief Set audio system peer. */
	void SetPeerAudio(deBaseAudioVideoPlayer *peer);
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pSetPlayPosition(float position, bool seeking);
};

#endif
