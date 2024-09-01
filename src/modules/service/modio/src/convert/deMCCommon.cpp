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

#include "deMCCommon.h"

#include <dragengine/common/math/decMath.h>


// Class deMCCommon
/////////////////////

Modio::Portal deMCCommon::Portal( const deServiceObject &so ){
	const decString &sval = so.GetString();
	if( sval == "apple" ){
		return Modio::Portal::Apple;
		
	}else if( sval == "epicGamesStore" ){
		return Modio::Portal::EpicGamesStore;
		
	}else if( sval == "gog" ){
		return Modio::Portal::GOG;
		
	}else if( sval == "google" ){
		return Modio::Portal::Google;
		
	}else if( sval == "itchio" ){
		return Modio::Portal::Itchio;
		
	}else if( sval == "nintendo" ){
		return Modio::Portal::Nintendo;
		
	}else if( sval == "psn" ){
		return Modio::Portal::PSN;
		
	}else if( sval == "steam" ){
		return Modio::Portal::Steam;
		
	}else if( sval == "xboxLive" ){
		return Modio::Portal::XboxLive;
		
	}else{
		decString message;
		message.Format( "Invalid portal: %s", sval.GetString() );
		DETHROW_INFO( deeInvalidParam, message );
	}
}

Modio::MaturityOption deMCCommon::MaturityOption( const deServiceObject &so ){
	const decString &sval = so.GetString();
	if( sval == "alcohol" ){
		return Modio::MaturityOption::Alcohol;
		
	}else if( sval == "drugs" ){
		return Modio::MaturityOption::Drugs;
		
	}else if( sval == "violence" ){
		return Modio::MaturityOption::Violence;
		
	}else if( sval == "explicit" ){
		return Modio::MaturityOption::Explicit;
		
	}else{
		decString message;
		message.Format( "Invalid maturity option: %s", sval.GetString() );
		DETHROW_INFO( deeInvalidParam, message );
	}
}

deServiceObject::Ref deMCCommon::MaturityOption( Modio::MaturityOption option ){
	switch( option ){
	case Modio::MaturityOption::Alcohol:
		return deServiceObject::NewString( "alcohol" );
		
	case Modio::MaturityOption::Drugs:
		return deServiceObject::NewString( "drugs" );
		
	case Modio::MaturityOption::Violence:
		return deServiceObject::NewString( "violence" );
		
	case Modio::MaturityOption::Explicit:
		return deServiceObject::NewString( "explicit" );
		
	default:
		DETHROW( deeInvalidParam );
	}
}

deServiceObject::Ref deMCCommon::MetadataMap( const std::vector<Modio::Metadata> &list ){
	const deServiceObject::Ref so( deServiceObject::NewList() );
	
	std::vector<Modio::Metadata>::const_iterator iter;
	for( iter = list.cbegin(); iter != list.cend(); iter++ ){
		so->SetStringChildAt( iter->Key.c_str(), iter->Value.c_str() );
	}
	
	return so;
}

deServiceObject::Ref deMCCommon::ModTag( const Modio::ModTag &tag ){
	const deServiceObject::Ref so( deServiceObject::Ref::New( new deServiceObject ) );
	
	so->SetStringChildAt( "tag", tag.Tag.c_str() );
	so->SetStringChildAt( "tagLocalized", tag.TagLocalized.c_str() );
	
	return so;
}

deServiceObject::Ref deMCCommon::ModTagList( const std::vector<Modio::ModTag> &list ){
	const deServiceObject::Ref so( deServiceObject::NewList() );
	
	std::vector<Modio::ModTag>::const_iterator iter;
	for( iter = list.cbegin(); iter != list.cend(); iter++ ){
		so->AddChild( ModTag( *iter ) );
	}
	
	return so;
}

std::vector<std::string> deMCCommon::StringList( const deServiceObject &so ){
	const int count = so.GetChildCount();
	std::vector<std::string> list;
	int i;
	
	for( i=0; i<count; i++ ){
		list.push_back( so.GetChildAt( i )->GetString().GetString() );
	}
	
	return list;
}

deServiceObject::Ref deMCCommon::StringList( const std::vector<std::string> &list ){
	const deServiceObject::Ref so( deServiceObject::NewList() );
	
	std::vector<std::string>::const_iterator iter;
	for( iter = list.cbegin(); iter != list.cend(); iter++ ){
		so->AddStringChild( iter->c_str() );
	}
	
	return so;
}

std::map<std::string, std::string> deMCCommon::StringMap( const deServiceObject &so ){
	const decStringList keys( so.GetChildrenKeys() );
	std::map<std::string, std::string> map;
	const int count = keys.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const decString &key = keys.GetAt( i );
		map[ key.GetString() ] = so.GetChildAt( key )->GetString().GetString();
	}
	
	return map;
}

deServiceObject::Ref deMCCommon::StringMap( const std::map<std::string, std::string> &map ){
	const deServiceObject::Ref so( deServiceObject::Ref::New( new deServiceObject ) );
	
	std::map<std::string, std::string>::const_iterator iter;
	for( iter = map.cbegin(); iter != map.cend(); iter++ ){
		so->SetStringChildAt( iter->first.c_str(), iter->second.c_str() );
	}
	
	return so;
}

std::string deMCCommon::StringOrEmpty( const deServiceObject &so, const char *key ){
	const deServiceObject::Ref child( so.GetChildAt( key ) );
	return child ? std::string( child->GetString().GetString() ) : std::string();
}

Modio::ProfileMaturity deMCCommon::ProfileMaturity( const deServiceObject &so ){
	Modio::ProfileMaturity profileMaturity;
	const int count = so.GetChildCount();
	int i;
	
	for( i=0; i<count; i++ ){
		profileMaturity |= MaturityOption( so.GetChildAt( i ) );
	}
	
	return profileMaturity;
}

deServiceObject::Ref deMCCommon::ProfileMaturity( Modio::ProfileMaturity maturity ){
	static const Modio::MaturityOption flags[ 4 ] = {
		Modio::MaturityOption::Alcohol,
		Modio::MaturityOption::Drugs,
		Modio::MaturityOption::Violence,
		Modio::MaturityOption::Explicit };
	
	const deServiceObject::Ref so( deServiceObject::NewList() );
	
	int i;
	for( i=0; i<4; i++ ){
		if( maturity.HasFlag( flags[ i ] ) ){
			so->AddChild( MaturityOption( flags[ i ] ) );
		}
	}
	
	return so;
}

std::int64_t deMCCommon::DateTime( const deServiceObject &so ){
	deServiceObject::Ref so2;
	decDateTime dt;
	
	so2 = so.GetChildAt( "year" );
	if( so2 ){
		dt.SetYear( so2->GetInteger() );
	}
	
	so2 = so.GetChildAt( "month" );
	if( so2 ){
		dt.SetMonth( so2->GetInteger() );
	}
	
	so2 = so.GetChildAt( "day" );
	if( so2 ){
		dt.SetDay( so2->GetInteger() );
	}
	
	so2 = so.GetChildAt( "hour" );
	if( so2 ){
		dt.SetHour( so2->GetInteger() );
	}
	
	so2 = so.GetChildAt( "minute" );
	if( so2 ){
		dt.SetMinute( so2->GetInteger() );
	}
	
	so2 = so.GetChildAt( "second" );
	if( so2 ){
		dt.SetSecond( so2->GetInteger() );
	}
	
	return ( std::int64_t )dt.ToSystemTime();
}

deServiceObject::Ref deMCCommon::DateTime( std::int64_t datetime ){
	const deServiceObject::Ref so( deServiceObject::Ref::New( new deServiceObject ) );
	const decDateTime dt( ( TIME_SYSTEM )datetime );
	
	so->SetIntChildAt( "year", dt.GetYear() );
	so->SetIntChildAt( "month", dt.GetMonth() );
	so->SetIntChildAt( "day", dt.GetDay() + 1 );
	so->SetIntChildAt( "hour", dt.GetHour() );
	so->SetIntChildAt( "minute", dt.GetMinute() );
	so->SetIntChildAt( "second", dt.GetSecond() );
	
	return so;
}

std::int64_t deMCCommon::Int64( const deServiceObject &so ){
	return Int64( so.GetString() );
}

deServiceObject::Ref deMCCommon::Int64( std::int64_t value ){
	return deServiceObject::NewString( Int64ToString( value ) );
}

std::int64_t deMCCommon::Int64( const decString &string ){
	return ( std::int64_t )string.ToLongValid();
}

decString deMCCommon::Int64ToString( std::int64_t value ){
	decString string;
	string.AppendValue( ( long long )value );
	return string;
}

std::int64_t deMCCommon::ID( const deServiceObject &so ){
	return Int64( so );
}

deServiceObject::Ref deMCCommon::ID( std::int64_t id ){
	return Int64( id );
}

std::int64_t deMCCommon::ID( const decString &string ){
	return Int64( string );
}

decString deMCCommon::IDToString( std::int64_t id ){
	return Int64ToString( id );
}

std::vector<std::int64_t> deMCCommon::IDList( const deServiceObject &so ){
	const int count = so.GetChildCount();
	std::vector<std::int64_t> list;
	int i;
	
	for( i=0; i<count; i++ ){
		list.push_back( ID( *so.GetChildAt( i ) ) );
	}
	
	return list;
}

deServiceObject::Ref deMCCommon::FileMetadata( const Modio::FileMetadata &data ){
	const deServiceObject::Ref so( deServiceObject::Ref::New( new deServiceObject ) );
	
	so->SetChildAt( "metadataId", ID( data.MetadataId ) );
	so->SetChildAt( "modId", ID( data.ModId ) );
	so->SetChildAt( "dateAdded", DateTime( data.DateAdded ) );
	so->SetChildAt( "currentVirusScanStatus", VirusScanStatus( data.CurrentVirusScanStatus ) );
	so->SetChildAt( "currentVirusStatus", VirusStatus( data.CurrentVirusStatus ) );
	so->SetFloatChildAt( "filesize", ( float )data.Filesize );
	so->SetFloatChildAt( "filesizeUncompressed", ( float )data.FilesizeUncompressed );
	so->SetStringChildAt( "filename", data.Filename.c_str() );
	so->SetStringChildAt( "version", data.Version.c_str() );
	
	if( ! data.Changelog.empty() ){
		so->SetStringChildAt( "changelog", data.Changelog.c_str() );
	}
	
	if( ! data.MetadataBlob.empty() ){
		so->SetStringChildAt( "metadataBlob", data.MetadataBlob.c_str() );
	}
	
	so->SetStringChildAt( "downloadBinaryURL", data.DownloadBinaryURL.c_str() );
	
	if( data.DownloadExpiryDate != 0 ){
		so->SetChildAt( "downloadExpiryDate", DateTime( data.DownloadExpiryDate ) );
	}
	
	return so;
}

deServiceObject::Ref deMCCommon::VirusScanStatus( Modio::FileMetadata::VirusScanStatus status ){
	switch( status ){
	case Modio::FileMetadata::VirusScanStatus::NotScanned:
		return deServiceObject::NewString( "notScanned" );
		
	case Modio::FileMetadata::VirusScanStatus::ScanComplete:
		return deServiceObject::NewString( "scanComplete" );
		
	case Modio::FileMetadata::VirusScanStatus::InProgress:
		return deServiceObject::NewString( "inProgress" );
		
	case Modio::FileMetadata::VirusScanStatus::TooLargeToScan:
		return deServiceObject::NewString( "tooLargeToScan" );
		
	case Modio::FileMetadata::VirusScanStatus::FileNotFound:
		return deServiceObject::NewString( "fileNotFound" );
		
	case Modio::FileMetadata::VirusScanStatus::ErrorScanning:
		return deServiceObject::NewString( "errorScanning" );
		
	default:
		DETHROW( deeInvalidParam );
	}
}

deServiceObject::Ref deMCCommon::VirusStatus( Modio::FileMetadata::VirusStatus status ){
	switch( status ){
	case Modio::FileMetadata::VirusStatus::NoThreat:
		return deServiceObject::NewString( "noThreat" );
		
	case Modio::FileMetadata::VirusStatus::Malicious:
		return deServiceObject::NewString( "malicious" );
		
	case Modio::FileMetadata::VirusStatus::PotentiallyHarmful:
		return deServiceObject::NewString( "potentiallyHarmful" );
		
	default:
		DETHROW( deeInvalidParam );
	}
}

deServiceObject::Ref deMCCommon::ModManagementEventType( Modio::ModManagementEvent::EventType type ){
	switch( type ){
	case Modio::ModManagementEvent::EventType::BeginInstall:
		return deServiceObject::NewString( "beginInstall" );
		
	case Modio::ModManagementEvent::EventType::Installed:
		return deServiceObject::NewString( "installed" );
		
	case Modio::ModManagementEvent::EventType::BeginUninstall:
		return deServiceObject::NewString( "beginUninstall" );
		
	case Modio::ModManagementEvent::EventType::Uninstalled:
		return deServiceObject::NewString( "uninstalled" );
		
	case Modio::ModManagementEvent::EventType::BeginUpdate:
		return deServiceObject::NewString( "beginUpdate" );
		
	case Modio::ModManagementEvent::EventType::Updated:
		return deServiceObject::NewString( "updated" );
		
	case Modio::ModManagementEvent::EventType::BeginUpload:
		return deServiceObject::NewString( "beginUpload" );
		
	default:
		DETHROW( deeInvalidParam );
	}
}

void deMCCommon::Error( deServiceObject &so, const Modio::ErrorCode &error ){
	so.SetIntChildAt( "code", error.value() );
	so.SetStringChildAt( "error", error.category().name() );
	so.SetStringChildAt( "message", error.message().c_str() );
}

void deMCCommon::ErrorOutcome( deServiceObject &so, const Modio::ErrorCode &error ){
	if( error ) {
		so.SetBoolChildAt( "success", false );
		Error( so, error );
		
	} else {
		so.SetBoolChildAt( "success", true );
	}
}

deServiceObject::Ref deMCCommon::ModProgressState( const Modio::ModProgressInfo::EModProgressState &state ){
	switch( state ){
	case Modio::ModProgressInfo::EModProgressState::Initializing:
		return deServiceObject::NewString( "initializing" );
		
	case Modio::ModProgressInfo::EModProgressState::Downloading:
		return deServiceObject::NewString( "downloading" );
		
	case Modio::ModProgressInfo::EModProgressState::Extracting:
		return deServiceObject::NewString( "extracting" );
		
	case Modio::ModProgressInfo::EModProgressState::Compressing:
		return deServiceObject::NewString( "compressing" );
		
	case Modio::ModProgressInfo::EModProgressState::Uploading:
		return deServiceObject::NewString( "uploading" );
		
	default:
		DETHROW( deeInvalidParam );
	}
}

deServiceObject::Ref deMCCommon::ModProgressInfo( const Modio::ModProgressInfo &info ){
	const deServiceObject::Ref so( deServiceObject::Ref::New( new deServiceObject ) );
	const Modio::ModProgressInfo::EModProgressState state = info.GetCurrentState();
	const double total = decMath::max( ( double )info.GetTotalProgress( state ), 1.0 );
	const double current = ( double )info.GetCurrentProgress( state );
	const double progress = decMath::clamp( current / total, 0.0, 1.0 );
	
	so->SetChildAt( "id", deMCCommon::ID( info.ID ) );
	so->SetChildAt( "state", ModProgressState( state ) );
	so->SetFloatChildAt( "total", ( float )total );
	so->SetFloatChildAt( "current", ( float )current );
	so->SetFloatChildAt( "progress", ( float )progress );
	
	// estimated remaining time in seconds
	// so->SetFloatChildAt( "remainingTime", ( float )remainingTime );
	
	return so;
}

Modio::AuthenticationProvider deMCCommon::AuthenticationProvider( const deServiceObject &so ){
	const decString &sval = so.GetString();
	if( sval == "xboxLive" ){
		return Modio::AuthenticationProvider::XboxLive;
		
	}else if( sval == "steam" ){
		return Modio::AuthenticationProvider::Steam;
		
	}else if( sval == "gog" ){
		return Modio::AuthenticationProvider::GoG;
		
	}else if( sval == "itch" ){
		return Modio::AuthenticationProvider::Itch;
		
	}else if( sval == "switch" ){
		return Modio::AuthenticationProvider::Switch;
		
	}else if( sval == "discord" ){
		return Modio::AuthenticationProvider::Discord;
		
	}else if( sval == "psn" ){
		return Modio::AuthenticationProvider::PSN;
		
	}else if( sval == "oculus" ){
		return Modio::AuthenticationProvider::Oculus;
		
	}else if( sval == "epic" ){
		return Modio::AuthenticationProvider::Epic;
		
	}else if( sval == "openId" ){
		return Modio::AuthenticationProvider::OpenID;
		
	}else if( sval == "apple" ){
		return Modio::AuthenticationProvider::Apple;
		
	}else if( sval == "googleIdToken" ){
		return Modio::AuthenticationProvider::GoogleIDToken;
		
	}else if( sval == "googleServerSideToken" ){
		return Modio::AuthenticationProvider::GoogleServerSideToken;
		
	}else{
		decString message;
		message.Format( "Invalid authentication provider: %s", sval.GetString() );
		DETHROW_INFO( deeInvalidParam, message );
	}
}

deServiceObject::Ref deMCCommon::NeedAcceptTerms( const Modio::Terms &terms ){
	const deServiceObject::Ref so( deServiceObject::Ref::New( new deServiceObject ) );
	so->SetStringChildAt( "terms", terms.TermsText.c_str() );
	so->SetStringChildAt( "buttonAccept", terms.Buttons.AgreeText.c_str() );
	so->SetStringChildAt( "buttonDecline", terms.Buttons.DisagreeText.c_str() );
	
	const deServiceObject::Ref soLinks( deServiceObject::NewList() );
	soLinks->AddChild( TermsLink( terms.Links.Website ) );
	soLinks->AddChild( TermsLink( terms.Links.Terms ) );
	soLinks->AddChild( TermsLink( terms.Links.Privacy ) );
	soLinks->AddChild( TermsLink( terms.Links.Manage ) );
	so->SetChildAt( "links", soLinks );
	
	const deServiceObject::Ref soCredParams( deServiceObject::Ref::New( new deServiceObject ) );
	soCredParams->SetStringChildAt( "termsAccepted", "1" );
	so->SetChildAt( "credentialParameters", soCredParams );
	
	return so;
}

deServiceObject::Ref deMCCommon::TermsLink( const Modio::Terms::Link &link ){
	const deServiceObject::Ref so( deServiceObject::Ref::New( new deServiceObject ) );
	so->SetStringChildAt( "label", link.Text.c_str() );
	so->SetStringChildAt( "url", link.URL.c_str() );
	so->SetBoolChildAt( "required", link.bRequired );
	return so;
}

Modio::ReportType deMCCommon::ReportParamsType( const deServiceObject &so ){
	const decString &sval = so.GetString();
	
	if( sval == "broken" ){
		return Modio::ReportType::NotWorking;
		
	}else if( sval == "falseInformation" ){
		return Modio::ReportType::FalseInformation;
		
	}else if( sval == "copyrightInfringement" ){
		return Modio::ReportType::DMCA;
		
	}else if( sval == "inappropriateContent" ){
		return Modio::ReportType::RudeContent;
		
	}else if( sval == "illegalContent" ){
		return Modio::ReportType::IllegalContent;
		
	}else{
		return Modio::ReportType::Other;
	}
}
