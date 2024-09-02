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

#include "deMCDetail.h"
#include "deMCCommon.h"
#include "../deModioResourceUrl.h"


// Class deMCDetail
/////////////////////

deServiceObject::Ref deMCDetail::Avatar( Modio::ModID modId, const Modio::Detail::Avatar &avatar ){
	const deServiceObject::Ref so( deServiceObject::Ref::New( new deServiceObject ) );
	
	so->SetStringChildAt( "original", deModioResourceUrl::FormatUrl(
		"mod", modId, "avatar", "original" ) );
	so->SetStringChildAt( "thumb50x50", deModioResourceUrl::FormatUrl(
		"mod", modId, "avatar", "thumb50x50" ) );
	so->SetStringChildAt( "thumb100x100", deModioResourceUrl::FormatUrl(
		"mod", modId, "avatar", "thumb100x100" ) );
	
	return so;
}

deServiceObject::Ref deMCDetail::Avatar( Modio::UserID userId, const Modio::Detail::Avatar &avatar ){
	const deServiceObject::Ref so( deServiceObject::Ref::New( new deServiceObject ) );
	
	so->SetStringChildAt( "original", deModioResourceUrl::FormatUrl(
		"user", userId, "avatar", "original" ) );
	so->SetStringChildAt( "thumb50x50", deModioResourceUrl::FormatUrl(
		"user", userId, "avatar", "thumb50x50" ) );
	so->SetStringChildAt( "thumb100x100", deModioResourceUrl::FormatUrl(
		"user", userId, "avatar", "thumb100x100" ) );
	
	return so;
}

deServiceObject::Ref deMCDetail::Image( Modio::ModID modId, int index,
const Modio::Detail::Image &image ){
	const deServiceObject::Ref so( deServiceObject::Ref::New( new deServiceObject ) );
	
	so->SetStringChildAt( "original", deModioResourceUrl::FormatUrl(
		"mod", modId, "gallery", index, "original" ) );
	so->SetStringChildAt( "thumb320x180", deModioResourceUrl::FormatUrl(
		"mod", modId, "gallery", index, "thumb320x180" ) );
	so->SetStringChildAt( "thumb1280x720", deModioResourceUrl::FormatUrl(
		"mod", modId, "gallery", index, "thumb1280x720" ) );
	
	return so;
}

deServiceObject::Ref deMCDetail::ImageList( Modio::ModID modId,
const std::vector<Modio::Detail::Image> &list ){
	const deServiceObject::Ref so( deServiceObject::NewList() );
	const int count = ( int )list.size();
	int i;
	
	for( i=0; i<count; i++ ){
		so->AddChild( Image( modId, i, list[ i ] ) );
	}
	
	return so;
}

deServiceObject::Ref deMCDetail::Logo( Modio::ModID modId, const Modio::Detail::Logo &logo ){
	const deServiceObject::Ref so( deServiceObject::Ref::New( new deServiceObject ) );
	
	so->SetStringChildAt( "original", deModioResourceUrl::FormatUrl(
		"mod", modId, "logo", "original" ) );
	so->SetStringChildAt( "thumb320x180", deModioResourceUrl::FormatUrl(
		"mod", modId, "logo", "thumb320x180" ) );
	so->SetStringChildAt( "thumb640x360", deModioResourceUrl::FormatUrl(
		"mod", modId, "logo", "thumb640x360" ) );
	so->SetStringChildAt( "thumb1280x720", deModioResourceUrl::FormatUrl(
		"mod", modId, "logo", "thumb1280x720" ) );
	
	return so;
}
