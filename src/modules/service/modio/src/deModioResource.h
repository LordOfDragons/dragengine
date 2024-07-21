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

#ifndef _DEMODIORESOURCE_H_
#define _DEMODIORESOURCE_H_

// has to come before the deImage.h include since on windows an external
// library included by mod.io fails to build if windows.h is included
// before this library
#include "modio.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectDictionary.h>
#include <dragengine/resources/image/deImage.h>


/**
 * Resource storing information to know what GetModMediaAsync variation to call.
 */
class deModioResource : public deObject{
public:
	typedef deTObjectReference<deModioResource> Ref;
	
	enum eType{
		etLogo,
		etGalleryImage,
		etAvatar
	};
	
	decString url;
	eType type;
	Modio::ModID modId;
	Modio::LogoSize logoSize;
	Modio::GallerySize gallerySize;
	Modio::GalleryIndex galleryIndex;
	Modio::AvatarSize avatarSize;
	deImage::Ref image;
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create resource. */
	deModioResource( const decString &url, Modio::ModID modId, Modio::LogoSize logoSize );
	
	deModioResource( const decString &url, Modio::ModID modId, Modio::GallerySize gallerySize,
		Modio::GalleryIndex galleryIndex );
	
	deModioResource( const decString &url, Modio::ModID modId, Modio::AvatarSize avatarSize );
	/*@}*/
	
	
	/** \name Registring */
	/*@{*/
	/** Register resource. */
	static void Register( decObjectDictionary &map, const Modio::ModInfo &mod );
	
	static void Register( decObjectDictionary &map, Modio::ModID modId,
		const Modio::GalleryList &images );
	
	static void Register( decObjectDictionary &map, Modio::ModID modId,
		const Modio::Detail::Image &image, Modio::GalleryIndex galleryIndex );
	
	static void Register( decObjectDictionary &map, Modio::ModID modId,
		const Modio::Detail::Logo &logo );
	
	static void Register( decObjectDictionary &map, Modio::ModID modId,
		const Modio::Detail::Avatar &avatar );
	/*@}*/
};

#endif
