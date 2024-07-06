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

#include <dragengine/common/math/decMath.h>

#include "deMCFilterParams.h"
#include "deMCUser.h"
#include "deMCModInfo.h"
#include "deMCCommon.h"


// Class deMCFilterParams
///////////////////////////

deMCFilterParams::sSortBy deMCFilterParams::SortFieldType( const deServiceObject &so ){
	const decString &sval = so.GetString();
	if( sval == "id" ){
		return {
			Modio::FilterParams::SortFieldType::ID,
			Modio::FilterParams::SortDirection::Ascending
		};
		
	}else if( sval == "downloadsToday" ){
		return {
			Modio::FilterParams::SortFieldType::DownloadsToday,
			Modio::FilterParams::SortDirection::Descending
		};
		
	}else if( sval == "subscriberCount" ){
		return {
			Modio::FilterParams::SortFieldType::SubscriberCount,
			Modio::FilterParams::SortDirection::Descending
		};
		
	}else if( sval == "rating" ){
		return {
			Modio::FilterParams::SortFieldType::Rating,
			Modio::FilterParams::SortDirection::Descending
		};
		
	}else if( sval == "dateMarkedLive" ){
		return {
			Modio::FilterParams::SortFieldType::DateMarkedLive,
			Modio::FilterParams::SortDirection::Descending
		};
		
	}else if( sval == "dateUpdated" ){
		return {
			Modio::FilterParams::SortFieldType::DateUpdated,
			Modio::FilterParams::SortDirection::Descending
		};
		
	}else if( sval == "downloadsTotal" ){
		return {
			Modio::FilterParams::SortFieldType::DownloadsTotal,
			Modio::FilterParams::SortDirection::Descending
		};
		
	}else if( sval == "nameAscending" ){
		return {
			Modio::FilterParams::SortFieldType::Alphabetical,
			Modio::FilterParams::SortDirection::Ascending
		};
		
	}else if( sval == "nameDescending" ){
		return {
			Modio::FilterParams::SortFieldType::Alphabetical,
			Modio::FilterParams::SortDirection::Descending
		};
		
	}else{
		return {
			Modio::FilterParams::SortFieldType::ID,
			Modio::FilterParams::SortDirection::Ascending
		};
	}
}

Modio::FilterParams::RevenueFilterType deMCFilterParams::RevenueFilterType( const deServiceObject &so ){
	const decString &sval = so.GetString();
	
	if( sval == "free" ){
		return Modio::FilterParams::RevenueFilterType::Free;
		
	}else if( sval == "paid" ){
		return Modio::FilterParams::RevenueFilterType::Paid;
		
	}else if( sval == "freeAndPaid" ){
		return Modio::FilterParams::RevenueFilterType::FreeAndPaid;
		
	}else{
		return Modio::FilterParams::RevenueFilterType::FreeAndPaid;
	}
}

Modio::FilterParams deMCFilterParams::FilterParams( const deServiceObject &so ){
	Modio::FilterParams filter;
	deServiceObject::Ref value;
	
	value = so.GetChildAt( "sortField" );
	if( value ){
		const sSortBy sortBy = SortFieldType( value );
		filter.SortBy( sortBy.field, sortBy.direction );
	}
	
	value = so.GetChildAt( "nameContains" );
	if( value && value->GetChildCount() > 0 ){
		filter.NameContains( deMCCommon::StringList( value ) );
	}
	
	value = so.GetChildAt( "matchingAuthor" );
	if( value ){
		filter.MatchingAuthors( deMCUser::UserIDList( value ) );
	}
	
	value = so.GetChildAt( "matchingIds" );
	if( value ){
		filter.MatchingIDs( deMCModInfo::ModIDList( value ) );
	}
	
	value = so.GetChildAt( "excludingIDs" );
	if( value ){
		filter.ExcludingIDs( deMCModInfo::ModIDList( value ) );
	}
	
	value = so.GetChildAt( "markedLiveAfter" );
	if( value ){
		const decDateTime dt( deMCCommon::DateTime( *value ) );
		const std::chrono::system_clock::time_point tp {
			std::chrono::seconds{ ( std::int64_t ) dt.ToSystemTime() } };
		filter.MarkedLiveAfter( tp );
	}
	
	value = so.GetChildAt( "markedLiveBefore" );
	if( value ){
		const decDateTime dt( deMCCommon::DateTime( *value ) );
		const std::chrono::system_clock::time_point tp {
			std::chrono::seconds{ ( std::int64_t ) dt.ToSystemTime() } };
		filter.MarkedLiveBefore( tp );
	}
	
	value = so.GetChildAt( "withTags" );
	if( value ){
		filter.WithTags( deMCCommon::StringList( value ) );
	}
	
	value = so.GetChildAt( "withoutTags" );
	if( value ){
		filter.WithoutTags( deMCCommon::StringList( value ) );
	}
	
	value = so.GetChildAt( "metadataLike" );
	if( value ){
		filter.MetadataLike( value->GetString().GetString() );
	}
	
	value = so.GetChildAt( "revenueType" );
	if( value ){
		filter.RevenueType( RevenueFilterType( value ) );
	}
	
	value = so.GetChildAt( "disallowMatureContent" );
	if( value && value->GetBoolean() ){
		filter.DisallowMatureContent();
	}
	
	value = so.GetChildAt( "withMatureContentFlags" );
	if( value ){
		filter.WithMatureContentFlags( ( Modio::MaturityOption )deMCCommon::ProfileMaturity( *value ).RawValue() );
	}
	
	std::size_t startIndex = 0, resultCount = 100;
	
	value = so.GetChildAt( "startIndex" );
	if( value ){
		startIndex = decMath::max( value->GetInteger(), 0 );
	}
	
	value = so.GetChildAt( "resultCount" );
	if( value ){
		resultCount = decMath::clamp( value->GetInteger(), 1, 100 );
	}
	
	filter.IndexedResults( startIndex, resultCount );
	return filter;
}
