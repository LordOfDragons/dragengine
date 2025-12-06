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

#include "deMCModInfo.h"
#include "deMCUser.h"
#include "deMCDetail.h"
#include "deMCCommon.h"
#include "../config/deModioUserConfig.h"


// Class deMCModInfo
//////////////////////

std::vector<Modio::ModID> deMCModInfo::ModIDList( const deServiceObject &so ){
	const int count = so.GetChildCount();
	std::vector<Modio::ModID> list;
	int i;
	
	for( i=0; i<count; i++ ){
		list.push_back( ( Modio::ModID )deMCCommon::ID( *so.GetChildAt( i ) ) );
	}
	
	return list;
}

deServiceObject::Ref deMCModInfo::ModServerSideStatus( Modio::ModServerSideStatus status ){
	switch( status ){
	case Modio::ModServerSideStatus::NotAccepted:
		return deServiceObject::NewString( "notAccepted" );
		
	case Modio::ModServerSideStatus::Accepted:
		return deServiceObject::NewString( "accepted" );
		
	case Modio::ModServerSideStatus::Deleted:
		return deServiceObject::NewString( "deleted" );
		
	default:
		DETHROW( deeInvalidParam );
	}
}

deServiceObject::Ref deMCModInfo::ModInfo( const Modio::ModInfo &info, const deModioUserConfig &config ){
	const deServiceObject::Ref so( deServiceObject::Ref::NewWith() );
	const decString strModId( deMCCommon::IDToString( info.ModId ) );
	so->SetChildAt( "modId", deServiceObject::NewString( strModId ) );
	
	if( ! info.ProfileName.empty() ){
		so->SetStringChildAt( "profileName", info.ProfileName.c_str() );
	}
	
	if( ! info.ProfileSummary.empty() ){
		so->SetStringChildAt( "profileSummary", info.ProfileSummary.c_str() );
	}
	
	if( ! info.ProfileDescription.empty() ){
		so->SetStringChildAt( "profileDescription", info.ProfileDescription.c_str() );
	}
	
	if( ! info.ProfileDescriptionPlaintext.empty() ){
		so->SetStringChildAt( "profileDescriptionPlaintext", info.ProfileDescriptionPlaintext.c_str() );
	}
	
	if( ! info.ProfileURL.empty() ){
		so->SetStringChildAt( "profileURL", info.ProfileURL.c_str() );
	}
	
	so->SetChildAt( "profileSubmittedBy", deMCUser::User( info.ProfileSubmittedBy ) );
	
	if( info.ProfileDateAdded != 0 ){
		so->SetChildAt( "profileDateAdded", deMCCommon::DateTime( info.ProfileDateAdded ) );
	}
	
	if( info.ProfileDateUpdated != 0 ){
		so->SetChildAt( "profileDateUpdated", deMCCommon::DateTime( info.ProfileDateUpdated ) );
	}
	
	if( info.ProfileDateLive != 0 ){
		so->SetChildAt( "profileDateLive", deMCCommon::DateTime( info.ProfileDateLive ) );
	}
	
	so->SetChildAt( "profileMaturityOption", deMCCommon::ProfileMaturity( info.ProfileMaturityOption ) );
	
	if( ! info.MetadataBlob.empty() ){
		so->SetStringChildAt( "metadataBlob", info.MetadataBlob.c_str() );
	}
	
	if( info.FileInfo.has_value() ){
		so->SetChildAt( "fileInfo", deMCCommon::FileMetadata( *info.FileInfo ) );
	}
	
	if( ! info.MetadataKvp.empty() ){
		so->SetChildAt( "metadataKvp", deMCCommon::MetadataMap( info.MetadataKvp ) );
	}
	
	if( ! info.Tags.empty() ){
		so->SetChildAt( "tags", deMCCommon::ModTagList( info.Tags ) );
	}
	
	if( info.GalleryImages.Size() != 0 ){
		so->SetChildAt( "galleryImages", deMCDetail::ImageList( info.ModId, info.GalleryImages.GetRawList() ) );
	}
	
	if( info.YoutubeURLs.Size() != 0 ){
		so->SetChildAt( "youtubeUrls", deMCCommon::StringList( info.YoutubeURLs.GetRawList() ) );
	}
	
	if( info.SketchfabURLs.Size() != 0 ){
		so->SetChildAt( "sketchfabUrls", deMCCommon::StringList( info.SketchfabURLs.GetRawList() ) );
	}
	
	so->SetChildAt( "stats", ModStats( info.Stats ) );
	so->SetChildAt( "modLogo", deMCDetail::Logo( info.ModId, info.ModLogo ) );
	so->SetStringChildAt( "version", info.Version.c_str() );
	so->SetChildAt( "modStatus", ModServerSideStatus( info.ModStatus ) );
	so->SetBoolChildAt( "publicVisible", info.Visibility == Modio::ObjectVisibility::Public );
	so->SetChildAt( "price", deMCCommon::UInt64( info.Price ) );
	so->SetBoolChildAt( "dependencies", info.Dependencies );
	
	const Modio::Rating rating = config.GetUserRating( strModId );
	switch( rating ){
	case Modio::Rating::Positive:
		so->SetIntChildAt( "userRating", 1 );
		break;
		
	case Modio::Rating::Negative:
		so->SetIntChildAt( "userRating", 0 );
		break;
		
	default:
		break;
	}
	
	return so;
}

deServiceObject::Ref deMCModInfo::ModStats( const Modio::ModStats &stats ){
	const deServiceObject::Ref so( deServiceObject::Ref::NewWith() );
	
	so->SetFloatChildAt( "popularityRankPosition", ( float )stats.PopularityRankPosition );
	so->SetFloatChildAt( "popularityRankTotalMods", ( float )stats.PopularityRankTotalMods );
	so->SetFloatChildAt( "downloadsTotal", ( float )stats.DownloadsTotal );
	so->SetFloatChildAt( "subscribersTotal", ( float )stats.SubscribersTotal );
	so->SetFloatChildAt( "ratingTotal", ( float )stats.RatingTotal );
	so->SetFloatChildAt( "ratingPositive", ( float )stats.RatingPositive );
	so->SetFloatChildAt( "ratingNegative", ( float )stats.RatingNegative );
	so->SetFloatChildAt( "ratingPercentagePositive", ( float )stats.RatingPercentagePositive );
	so->SetFloatChildAt( "ratingWeightedAggregate", ( float )stats.RatingWeightedAggregate );
	so->SetStringChildAt( "ratingDisplayText", stats.RatingDisplayText.c_str() );
	
	return so;
}

deServiceObject::Ref deMCModInfo::ModCollectionEntry(
const Modio::ModCollectionEntry &status, const deModioUserConfig &config ){
	const deServiceObject::Ref so( deServiceObject::Ref::NewWith() );
	
	so->SetChildAt( "id", deMCCommon::ID( status.GetID() ) );
	so->SetChildAt( "state", ModState( status.GetModState() ) );
	so->SetChildAt( "info", ModInfo( status.GetModProfile(), config ) );
	if( status.GetSizeOnDisk().has_value() ){
		so->SetFloatChildAt( "sizeOnDisk", ( float )*status.GetSizeOnDisk() );
	}
	
	return so;
}

deServiceObject::Ref deMCModInfo::ModState( Modio::ModState state ){
	switch( state ){
	case Modio::ModState::InstallationPending:
		return deServiceObject::NewString( "installationPending" );
		
	case Modio::ModState::Installed:
		return deServiceObject::NewString( "installed" );
		
	case Modio::ModState::UpdatePending:
		return deServiceObject::NewString( "updatePending" );
		
	case Modio::ModState::Downloading:
		return deServiceObject::NewString( "downloading" );
		
	case Modio::ModState::Extracting:
		return deServiceObject::NewString( "extracting" );
		
	case Modio::ModState::UninstallPending:
		return deServiceObject::NewString( "uninstallPending" );
		
	default:
		DETHROW( deeInvalidParam );
	}
}
