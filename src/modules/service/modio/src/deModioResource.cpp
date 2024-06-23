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

#include "deModioResource.h"


// Class deModioResource
//////////////////////////

// Constructor, destructor
////////////////////////////

deModioResource::deModioResource( const decString &aurl, Modio::ModID amodId,
	Modio::LogoSize alogoSize ) :
url( aurl ),
type( etLogo ),
modId( amodId ),
logoSize( alogoSize ),
gallerySize( Modio::GallerySize::Original ),
galleryIndex( 0 ),
avatarSize( Modio::AvatarSize::Original ){
}

deModioResource::deModioResource( const decString &aurl, Modio::ModID amodId,
	Modio::GallerySize agallerySize, Modio::GalleryIndex agalleryIndex ) :
url( aurl ),
type( etGalleryImage ),
modId( amodId ),
logoSize( Modio::LogoSize::Original ),
gallerySize( agallerySize ),
galleryIndex( agalleryIndex ),
avatarSize( Modio::AvatarSize::Original ){
}

deModioResource::deModioResource( const decString &aurl, Modio::ModID amodId,
	Modio::AvatarSize aavatarSize ) :
url( aurl ),
type( etAvatar ),
modId( amodId ),
logoSize( Modio::LogoSize::Original ),
gallerySize( Modio::GallerySize::Original ),
galleryIndex( 0 ),
avatarSize( aavatarSize ){
}



// Register
/////////////

void deModioResource::Register( decObjectDictionary &map, const Modio::ModInfo &mod ){
	Register( map, mod.ModId, mod.ModLogo );
	Register( map, mod.ModId, mod.GalleryImages );
	Register( map, mod.ModId, mod.ProfileSubmittedBy.Avatar );
}

void deModioResource::Register( decObjectDictionary &map, Modio::ModID modId,
const Modio::GalleryList &images ){
	const size_t size = images.Size();
	size_t i;
	
	for( i=0; i<size; i++ ){
		Register( map, modId, images[ i ], i );
	}
}

void deModioResource::Register( decObjectDictionary &map, Modio::ModID modId,
const Modio::Detail::Image &image, Modio::GalleryIndex galleryIndex ){
	if( ! image.Original.empty() ){
		map.SetAt( image.Original.c_str(), Ref::New( new deModioResource(
			image.Original.c_str(), modId, Modio::GallerySize::Original, galleryIndex ) ) );
	}
	if( ! image.Thumb320x180.empty() ){
		map.SetAt( image.Thumb320x180.c_str(), Ref::New( new deModioResource(
			image.Thumb320x180.c_str(), modId, Modio::GallerySize::Thumb320, galleryIndex ) ) );
	}
	if( ! image.Thumb1280x720.empty() ){
		map.SetAt( image.Thumb1280x720.c_str(), Ref::New( new deModioResource(
			image.Thumb1280x720.c_str(), modId, Modio::GallerySize::Thumb1280, galleryIndex ) ) );
	}
}

void deModioResource::Register( decObjectDictionary &map, Modio::ModID modId,
const Modio::Detail::Logo &logo ){
	if( ! logo.Original.empty() ){
		map.SetAt( logo.Original.c_str(), Ref::New( new deModioResource(
			logo.Original.c_str(), modId, Modio::LogoSize::Original ) ) );
	}
	if( ! logo.Thumb320x180.empty() ){
		map.SetAt( logo.Thumb320x180.c_str(), Ref::New( new deModioResource(
			logo.Thumb320x180.c_str(), modId, Modio::LogoSize::Thumb320 ) ) );
	}
	if( ! logo.Thumb640x360.empty() ){
		map.SetAt( logo.Thumb640x360.c_str(), Ref::New( new deModioResource(
			logo.Thumb640x360.c_str(), modId, Modio::LogoSize::Thumb640 ) ) );
	}
	if( ! logo.Thumb1280x720.empty() ){
		map.SetAt( logo.Thumb1280x720.c_str(), Ref::New( new deModioResource(
			logo.Thumb1280x720.c_str(), modId, Modio::LogoSize::Thumb1280 ) ) );
	}
}

void deModioResource::Register( decObjectDictionary &map, Modio::ModID modId,
const Modio::Detail::Avatar &avatar ){
	if( ! avatar.Original.empty() ){
		map.SetAt( avatar.Original.c_str(), Ref::New( new deModioResource(
			avatar.Original.c_str(), modId, Modio::AvatarSize::Original ) ) );
	}
	if( ! avatar.Thumb50x50.empty() ){
		map.SetAt( avatar.Thumb50x50.c_str(), Ref::New( new deModioResource(
			avatar.Thumb50x50.c_str(), modId, Modio::AvatarSize::Thumb50 ) ) );
	}
	if( ! avatar.Thumb100x100.empty() ){
		map.SetAt( avatar.Thumb100x100.c_str(), Ref::New( new deModioResource(
			avatar.Thumb100x100.c_str(), modId, Modio::AvatarSize::Thumb100 ) ) );
	}
}
