/* 
 * Drag[en]gine WebP-3D Image Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "deWebp3DTarball.h"
#include "deWebp3DImageInfo.h"
#include "deWebp3DModule.h"

#include <webp/encode.h>

#include <dragengine/resources/image/deImage.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/file/decMemoryFileWriter.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>



// Structures
///////////////

struct sTarballHeader{		/* byte offset */
	char name[100];			/*   0 */
	char mode[8];			/* 100 */
	char uid[8];			/* 108 */
	char gid[8];			/* 116 */
	char size[12];			/* 124 */
	char mtime[12];			/* 136 */
	char chksum[8];			/* 148 */
	char typeflag;			/* 156 */
	char linkname[100];		/* 157 */
	char magic[6];			/* 257 */
	char version[2];		/* 263 */
	char uname[32];			/* 265 */
	char gname[32];			/* 297 */
	char devmajor[8];		/* 329 */
	char devminor[8];		/* 337 */
	char prefix[155];		/* 345 */
	char padding[12];		/* 500 */
};							/* 512 */

#define REGTYPE				'0'		/* regular file */
#define AREGTYPE			'\0'	/* regular file */



// Class deWebp3DTarball
//////////////////////////

// Constructor, destructor
////////////////////////////

deWebp3DTarball::deWebp3DTarball( deWebp3DModule &module ) :
pModule( module ){
}

deWebp3DTarball::~deWebp3DTarball(){
}



// Management
///////////////

void deWebp3DTarball::Get3DImageInfos( deWebp3DImageInfo &info, decBaseFileReader &file ){
	bool firstImage = true;
	sTarballHeader header;
	sImageInfo info2D;
	unsigned short z;
	int i;
	
	info.width = 0;
	info.height = 0;
	info.depth = 0;
	
	while( true ){
		file.Read( &header, 512 );
		
		// we detect the 0-blocks just by checking the file names are 0 length.
		// since 0 length file names do not exist this should be accurate enough
		if( ! header.name[ 0 ] && ! header.linkname[ 0 ] ){
			break;
		}
		
		// save the position so we can quickly skip to the end of the file
		const int position = file.GetPosition();
		
		// determine the file size. spaces are skipped in front of the number.
		// after that the number is read as octal value. in the end would be
		// another space but we know this so we can skip it.
		bool skip = true;
		int filesize = 0;
		
		for( i=0; i<11; i++ ){
			if( skip ){
				if( header.size[ i ] != ' ' ){
					skip = false;
					filesize = ( int )( header.size[ i ] - '0' );
				}
				
			}else{
				if( header.size[ i ] == ' ' ){
					break;
				}
				
				filesize = filesize * 8 + ( int )( header.size[ i ] - '0' );
			}
		}
		
		// tar requires files to be aligned in chunks so determine the amount
		// of padding 0 bytes required
		int padding = 512 - ( filesize % 512 );
		if( padding == 512 ){
			padding = 0;
		}
		
		// only files are accepted
		if( header.typeflag == REGTYPE || header.typeflag == AREGTYPE ){
			// retrieve the z-coordinate
			if( sscanf( header.name, "z%hu.webp", &z ) == 1 ){
				// if the z coordinate is larger than the depth of the image found so far bump it up
				if( ( int )z >= info.depth ){
					info.depth = ( int )z + 1;
				}
				
				// retrieve the image properties
				Get2DImageInfos( info2D, file, filesize );
				
				// if this is the first image store them away
				if( firstImage ){
					info.width = info2D.width;
					info.height = info2D.height;
					info.hasAlpha = info2D.hasAlpha;
					firstImage = false;
					
				// otherwise check if the properties match
				}else{
					if( info2D.width != info.width
					|| info2D.height != info.height
					|| info2D.hasAlpha != info.hasAlpha ){
						pModule.LogErrorFormat(
							"the files in the archive '%s' do not match in size or format.",
							info.GetFilename().GetString() );
						DETHROW_INFO( deeInvalidFileFormat, info.GetFilename() );
					}
				}
				
			}else{
				pModule.LogErrorFormat(
					"invalid file name '%s' in the archive '%s'. has to be z<num>.webp with num=[0..depth-1].",
					header.name, info.GetFilename().GetString() );
				DETHROW_INFO( deeInvalidFileFormat, info.GetFilename() );
			}
		}
		
		// skip to the end of the file
		file.SetPosition( position + filesize + padding );
	}
	
	// no images in the archive
	if( firstImage ){
		pModule.LogErrorFormat(
			"no images in the archive '%s'. at least one image is required.",
			info.GetFilename().GetString() );
		DETHROW_INFO( deeInvalidFileFormat, info.GetFilename() );
	}
}

void deWebp3DTarball::Load3DImage( deWebp3DImageInfo &infos, decBaseFileReader &file, deImage &image ){
	DEASSERT_TRUE( image.GetBitCount() == 8 )
	
	const int strideLine = image.GetWidth() * image.GetComponentCount();
	const int strideImage = strideLine * image.GetHeight();
	char * const imageData = ( char* )image.GetData();
	sTarballHeader header;
	unsigned short z;
	int i;
	
	// read the archive with all files filling the images one by one
	while( true ){
		file.Read( &header, 512 );
		
		// we detect the 0-blocks just by checking the file names are 0 length.
		// since 0 length file names do not exist this should be accurate enough
		if( ! header.name[ 0 ] && ! header.linkname[ 0 ] ){
			break;
		}
		
		// save the position so we can quickly skip to the end of the file
		const int position = file.GetPosition();
		
		// determine the file size. spaces are skipped in front of the number.
		// after that the number is read as octal value. in the end would be
		// another space but we know this so we can skip it.
		bool skip = true;
		int filesize = 0;
		
		for( i=0; i<11; i++ ){
			if( skip ){
				if( header.size[ i ] != ' ' ){
					skip = false;
					filesize = ( int )( header.size[ i ] - '0' );
				}
				
			}else{
				if( header.size[ i ] == ' ' ){
					break;
				}
				
				filesize = filesize * 8 + ( int )( header.size[ i ] - '0' );
			}
		}
		
		// tar requires files to be aligned in chunks so determine the amount
		// of padding 0 bytes required
		int padding = 512 - ( filesize % 512 );
		if( padding == 512 ){
			padding = 0;
		}
		
		// only files are accepted
		if( header.typeflag == REGTYPE || header.typeflag == AREGTYPE ){
			// retrieve the z-coordinate
			if( sscanf( header.name, "z%hu.webp", &z ) == 1 ){
				// load image slice
				Load2DImage( infos, file, filesize, imageData + strideImage * ( int )z );
				
			}else{
				pModule.LogErrorFormat(
					"invalid file name '%s' in the archive '%s'. has to be z<num>.webp with num=[0..depth-1].",
					header.name, infos.GetFilename().GetString() );
				DETHROW_INFO( deeInvalidFileFormat, infos.GetFilename() );
			}
		}
		
		// skip to the end of the file
		file.SetPosition( position + filesize + padding );
	}
}

void deWebp3DTarball::Save3DImage( decBaseFileWriter &file, const deImage &image ){
	DEASSERT_TRUE( image.GetBitCount() == 8 )
	DEASSERT_TRUE( image.GetComponentCount() >= 3 )
	
	const char * const imageData = ( char* )image.GetData();
	const int strideLine = image.GetWidth() * image.GetComponentCount();
	const int strideImage = strideLine * image.GetHeight();
	const bool hasAlpha = image.GetComponentCount() == 4;
	char paddingBytes[ 512 ];
	unsigned int checksum;
	int z, r;
	
	// build the header with the default parameters. we only have to replace
	// the name, file size and checksum later on and the rest stays the same
	sTarballHeader header;
	memset( &header, '\0', 512 );
	
	strcpy( &header.mode[ 0 ], "100600 " );
	strcpy( &header.uid[ 0 ],  "  1750 " );
	strcpy( &header.gid[ 0 ],  "   144 " );
	
	struct timeval curtime;
	gettimeofday( &curtime, nullptr );
	header.mtime[ 11 ] = ' ';
	for( r=10; r>=0; r-- ){
		if( curtime.tv_sec ){
			header.mtime[ r ] = ( char )( '0' + ( char )( curtime.tv_sec & 0x7 ) );
			
		}else{ // no leading 0s please
			header.mtime[ r ] = ' ';
		}
		curtime.tv_sec >>= 3;
	}
	
	header.typeflag = ( char )REGTYPE;
	
	const unsigned char * const headerBytes = ( const unsigned char * )&header;
	
	// set the padding bytes to 0
	memset( &paddingBytes[ 0 ], '\0', 512 );
	
	// save file
	const decMemoryFile::Ref memoryFile( decMemoryFile::Ref::New( new decMemoryFile( "" ) ) );
	
	// write file by creating an archive with an image for each z coordinate
	for( z=0; z<image.GetDepth(); z++ ){
		// save slice to memory file
		Save2DImage( image.GetWidth(), image.GetHeight(), hasAlpha,
			decMemoryFileWriter::Ref::New( new decMemoryFileWriter( memoryFile, false ) ),
			imageData + ( strideImage * z ) );
		
		// determine the file size and padding. tar requires files to be
		// aligned in chunks so determine the amount of padding 0 bytes
		// required to fill up to the next chunk boundary
		int filesize = memoryFile->GetLength();
		int padding = 512 - ( filesize % 512 );
		if( padding == 512 ){
			padding = 0;
		}
		
		// update header
		sprintf( &header.name[ 0 ], "z%hu.webp", ( unsigned short )z );
		
		header.size[ 11 ] = ' ';
		for( r=10; r>=0; r-- ){
			if( filesize ){
				header.size[ r ] = ( char )( '0' + ( char )( filesize & 0x7 ) );
				
			}else{ // no leading 0s please
				header.size[ r ] = ' ';
			}
			filesize >>= 3;
		}
		
		memset( &header.chksum[ 0 ], ' ', 8 );
		checksum = 0;
		for( r=0; r<512; r++ ){
			checksum += headerBytes[ r ];
		}
		header.chksum[ 7 ] = '\0';
		header.chksum[ 6 ] = ' ';
		for( r=5; r>=0; r-- ){
			if( checksum ){
				header.chksum[ r ] = ( char )( '0' + ( char )( checksum & 0x7 ) );
				
			}else{ // no leading 0s please
				header.chksum[ r ] = ' ';
			}
			checksum >>= 3;
		}
		
		// write the header, file data and padding 0 bytes
		file.Write( &header, 512 );
		file.Write( memoryFile->GetPointer(), memoryFile->GetLength() );
		if( padding > 0 ){
			file.Write( &paddingBytes, padding );
		}
	}
	
	// write two padding blocks at the end
	file.Write( &paddingBytes, 512 );
	file.Write( &paddingBytes, 512 );
}



void deWebp3DTarball::Get2DImageInfos( sImageInfo &info2D, decBaseFileReader &file, int size ){
	const decMemoryFile::Ref data( decMemoryFile::Ref::New( new decMemoryFile( "data" ) ) );
	data->Resize( size );
	file.Read( data->GetPointer(), size );
	
	WebPBitstreamFeatures features;
	Assert( WebPGetFeatures( ( uint8_t* )data->GetPointer(), size, &features ) );
	
	info2D.width = features.width;
	info2D.height = features.height;
	info2D.hasAlpha = features.has_alpha;
}

void deWebp3DTarball::Load2DImage( deWebp3DImageInfo &info3D,
decBaseFileReader &file, int size, void *imagedata ){
	DEASSERT_NOTNULL( imagedata )
	
	const decMemoryFile::Ref data( decMemoryFile::Ref::New( new decMemoryFile( "data" ) ) );
	data->Resize( size );
	file.Read( data->GetPointer(), size );
	
	uint8_t *result = nullptr;
	
	if( info3D.hasAlpha ){
		result = WebPDecodeRGBAInto( ( const uint8_t* )data->GetPointer(), size,
			( uint8_t* )imagedata, info3D.width * info3D.height * 4, info3D.width * 4 );
		
	}else{
		result = WebPDecodeRGBInto( ( const uint8_t* )data->GetPointer(), size,
			( uint8_t* )imagedata, info3D.width * info3D.height * 3, info3D.width * 3 );
	}
	
	if( ! result ){
		DETHROW( deeInvalidFileFormat );
	}
}

void deWebp3DTarball::Save2DImage( int width, int height, bool hasAlpha,
decBaseFileWriter &file, const void *imagedata ){
	DEASSERT_NOTNULL( imagedata )
	
	uint8_t *output = nullptr;
	const float quality = 95.0f;
	size_t size = 0;
	
	try{
		if( hasAlpha ){
			// WebPEncodeLosslessRGB
			size = WebPEncodeRGBA( ( const uint8_t* )imagedata, width,
				height, width * 4, quality, &output );
			
		}else{
			size = WebPEncodeRGB( ( const uint8_t* )imagedata, width,
				height, width * 3, quality, &output );
		}
		
		if( size == 0 || ! output ){
			DETHROW( deeWriteFile );
		}
		
		file.Write( output, size );
		
		WebPFree( output );
		
	}catch( const deException & ){
		if( output ){
			WebPFree( output );
		}
		throw;
	}
}

void deWebp3DTarball::Assert( VP8StatusCode statusCode ) const{
	switch( statusCode ){
	case VP8_STATUS_OK:
		return;
		
	case VP8_STATUS_OUT_OF_MEMORY:
		DETHROW( deeOutOfMemory );
		
	case VP8_STATUS_INVALID_PARAM:
		DETHROW( deeInvalidParam );
		
	case VP8_STATUS_BITSTREAM_ERROR:
		DETHROW_INFO( deeInvalidFileFormat, "Bit stream error" );
		
	case VP8_STATUS_UNSUPPORTED_FEATURE:
		DETHROW_INFO( deeInvalidFileFormat, "Unsupported feature" );
		
	case VP8_STATUS_SUSPENDED:
		DETHROW_INFO( deeInvalidFileFormat, "Suspended" );
		
	case VP8_STATUS_USER_ABORT:
		DETHROW_INFO( deeInvalidFileFormat, "User abort" );
		
	case VP8_STATUS_NOT_ENOUGH_DATA:
		DETHROW_INFO( deeInvalidFileFormat, "Not enough data" );
		
	default:
		DETHROW( deeInvalidFileFormat );
	}
}
