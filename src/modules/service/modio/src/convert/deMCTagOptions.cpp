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

#include "deMCTagOptions.h"
#include "deMCUser.h"
#include "deMCDetail.h"
#include "deMCCommon.h"


// Class deMCTagOptions
/////////////////////////

deServiceObject::Ref deMCTagOptions::ModTagOptions( const Modio::ModTagOptions &tagOptions ){
	Modio::List<std::vector, Modio::ModTagInfo>::const_iterator iter;
	const deServiceObject::Ref so( deServiceObject::NewList() );
	for( iter = tagOptions.begin(); iter != tagOptions.end(); iter++ ){
		so->AddChild( ModTagInfo( *iter ) );
	}
	return so;
}

deServiceObject::Ref deMCTagOptions::ModTagInfo( const Modio::ModTagInfo &info ){
	const deServiceObject::Ref so( deServiceObject::Ref::NewWith() );
	so->SetStringChildAt( "name", info.TagGroupName.c_str() );
	so->SetChildAt( "nameTranslations", deMCCommon::StringMap( info.TagGroupNameLocData ) );
	so->SetBoolChildAt( "allowMultipleSelection", info.bAllowMultipleSelection );
	
	const deServiceObject::Ref soTags( deServiceObject::NewList() );
	const std::vector<Modio::ModTagLocalizationData> &tags = info.TagGroupValueLocData;
	std::vector<Modio::ModTagLocalizationData>::const_iterator iterTags;
	for( iterTags = tags.cbegin(); iterTags != tags.cend(); iterTags++ ){
		const deServiceObject::Ref soTag( deServiceObject::Ref::NewWith() );
		soTag->SetStringChildAt( "name", iterTags->Tag.c_str() );
		soTag->SetChildAt( "nameTranslations", deMCCommon::StringMap( iterTags->Translations ) );
		soTags->AddChild( soTag );
	}
	so->SetChildAt( "tags", soTags );
	
	return so;
}
