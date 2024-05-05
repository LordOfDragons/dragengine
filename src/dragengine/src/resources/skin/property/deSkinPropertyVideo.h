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
class DE_DLL_EXPORT deSkinPropertyVideo : public deSkinProperty{
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
