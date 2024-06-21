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


// Class deMCDetail
/////////////////////

deServiceObject::Ref deMCDetail::Avatar( const Modio::Detail::Avatar &avatar ){
	const deServiceObject::Ref so( deServiceObject::Ref::New( new deServiceObject ) );
	
	so->SetStringChildAt( "filename", avatar.Filename.c_str() );
	so->SetStringChildAt( "original", avatar.Original.c_str() );
	so->SetStringChildAt( "thumb50x50", avatar.Thumb50x50.c_str() );
	so->SetStringChildAt( "thumb100x100", avatar.Thumb100x100.c_str() );
	
	return so;
}

deServiceObject::Ref deMCDetail::Image( const Modio::Detail::Image &image ){
	const deServiceObject::Ref so( deServiceObject::Ref::New( new deServiceObject ) );
	
	so->SetStringChildAt( "filename", image.Filename.c_str() );
	so->SetStringChildAt( "original", image.Original.c_str() );
	so->SetStringChildAt( "thumb320x180", image.Thumb320x180.c_str() );
	so->SetStringChildAt( "thumb1280x720", image.Thumb1280x720.c_str() );
	
	return so;
}

deServiceObject::Ref deMCDetail::ImageList( const std::vector<Modio::Detail::Image> &list ){
	const deServiceObject::Ref so( deServiceObject::NewList() );
	
	std::vector<Modio::Detail::Image>::const_iterator iter;
	for( iter = list.cbegin(); iter != list.cend(); iter++ ){
		so->AddChild( Image( *iter ) );
	}
	
	return so;
}

deServiceObject::Ref deMCDetail::Logo( const Modio::Detail::Logo &logo ){
	const deServiceObject::Ref so( deServiceObject::Ref::New( new deServiceObject ) );
	
	so->SetStringChildAt( "filename", logo.Filename.c_str() );
	so->SetStringChildAt( "original", logo.Original.c_str() );
	so->SetStringChildAt( "thumb320x180", logo.Thumb320x180.c_str() );
	so->SetStringChildAt( "thumb640x360", logo.Thumb640x360.c_str() );
	so->SetStringChildAt( "thumb1280x720", logo.Thumb1280x720.c_str() );
	
	return so;
}
