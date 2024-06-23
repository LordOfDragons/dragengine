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

#ifndef _DEMCCOMMON_H_
#define _DEMCCOMMON_H_

#include "../modio.h"

#include <dragengine/resources/service/deServiceObject.h>


/**
 * Convert common data.
 */
class deMCCommon{
private:
	deMCCommon() = default;
	
public:
	/** Convert modio environment. */
	static Modio::Environment Environment( const deServiceObject &so );
	
	/** Convert modio portal. */
	static Modio::Portal Portal( const deServiceObject &so );
	
	/** Convert modio maturity option. */
	static Modio::MaturityOption MaturityOption( const deServiceObject &so );
	static deServiceObject::Ref MaturityOption( Modio::MaturityOption option );
	
	/** Convert modio Metadata. */
	static deServiceObject::Ref Metadata( const Modio::Metadata &data );
	
	/** Convert modio Metadata list. */
	static deServiceObject::Ref MetadataList( const std::vector<Modio::Metadata> &list );
	
	/** Convert modio ModTag. */
	static deServiceObject::Ref ModTag( const Modio::ModTag &tag );
	
	/** Convert modio ModTag list. */
	static deServiceObject::Ref ModTagList( const std::vector<Modio::ModTag> &list );
	
	/** Convert modio string list. */
	static std::vector<std::string> StringList( const deServiceObject &so );
	static deServiceObject::Ref StringList( const std::vector<std::string> &list );
	
	/** Convert maturity content flags. */
	static Modio::ProfileMaturity ProfileMaturity( const deServiceObject &so );
	static deServiceObject::Ref ProfileMaturity( Modio::ProfileMaturity maturity );
	
	/** Convert datetime. */
	static std::int64_t DateTime( const deServiceObject &so );
	static deServiceObject::Ref DateTime( std::int64_t datetime );
	
	/** Convert int64. */
	static std::int64_t Int64( const deServiceObject &so );
	static deServiceObject::Ref Int64( std::int64_t value );
	
	/** Convert identifier. */
	static std::int64_t ID( const deServiceObject &so );
	static deServiceObject::Ref ID( std::int64_t id );
	
	/** Convert identifier list. */
	static std::vector<std::int64_t> IDList( const deServiceObject &so );
	
	/** Convert modio FileMetadata. */
	static deServiceObject::Ref FileMetadata( const Modio::FileMetadata &data );
	
	/** Convert modio VirusScanStatus. */
	static deServiceObject::Ref VirusScanStatus( Modio::FileMetadata::VirusScanStatus status );
	
	/** Convert modio VirusStatus. */
	static deServiceObject::Ref VirusStatus( Modio::FileMetadata::VirusStatus status );
	/*@}*/
};

#endif
