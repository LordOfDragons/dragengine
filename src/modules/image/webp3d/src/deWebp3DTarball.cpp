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

#include <dragengine/dragengine_configuration.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef OS_W32
	#include <dragengine/app/deOSWindows.h>
#else
	#include <sys/time.h>
#endif

#include "deWebp3DTarball.h"
#include "deWebp3DImageInfo.h"
#include "deWebp3DModule.h"

#include <webp/encode.h>
#include <webp/demux.h>

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
	info.hasAlpha = false;
	info.isGrayscale = false;
	
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
			z = decString( header.name ).GetMiddle( 1, -5 ).ToInt();

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
				info.isGrayscale = info2D.isGrayscale;
				firstImage = false;
					
			// otherwise check if the properties match
			}else{
				if( info2D.width != info.width
				|| info2D.height != info.height
				|| info2D.hasAlpha != info.hasAlpha
				|| info2D.isGrayscale != info.isGrayscale ){
					pModule.LogErrorFormat(
						"the files in the archive '%s' do not match in size or format.",
						info.GetFilename().GetString() );
					DETHROW_INFO( deeInvalidFileFormat, info.GetFilename() );
				}
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
			z = decString( header.name ).GetMiddle( 1, -5 ).ToInt();
			Load2DImage( infos, file, filesize, imageData + strideImage * ( int )z );
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
	struct timeval curtime;
	unsigned int checksum;
	int z, r;
	
	// build the header with the default parameters. we only have to replace
	// the name, file size and checksum later on and the rest stays the same
	sTarballHeader header;
	memset( &header, '\0', 512 );
	
	#ifdef OS_W32
		strcpy_s( &header.mode[ 0 ], sizeof( header.mode ), "100600 " );
		strcpy_s( &header.uid[ 0 ], sizeof( header.uid ),   "  1750 " );
		strcpy_s( &header.gid[ 0 ], sizeof( header.gid ),   "   144 " );
	#else
		strcpy( &header.mode[ 0 ], "100600 " );
		strcpy( &header.uid[ 0 ],  "  1750 " );
		strcpy( &header.gid[ 0 ],  "   144 " );
	#endif
	
	#ifdef OS_W32
		{
		ULARGE_INTEGER x;
        ULONGLONG usec;
        static const ULONGLONG epoch_offset_us = 11644473600000000ULL;

		FILETIME filetime;
		GetSystemTimeAsFileTime( &filetime );

		x.LowPart = filetime.dwLowDateTime;
        x.HighPart = filetime.dwHighDateTime;
        usec = x.QuadPart / 10  -  epoch_offset_us;
        curtime.tv_sec  = ( long )( usec / 1000000ULL );
        curtime.tv_usec = ( long )( usec % 1000000ULL );
		}
	#else
		gettimeofday( &curtime, nullptr );
	#endif

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
		#ifdef OS_W32
			sprintf_s( &header.name[ 0 ], sizeof( header.name ), "z%hu.webp", ( unsigned short )z );
		#else
			sprintf( &header.name[ 0 ], "z%hu.webp", ( unsigned short )z );
		#endif
		
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
	info2D.isGrayscale = false;
	
	// look for exif user comments to handle the "grayscale" problem
	WebPData wpdata = { ( const uint8_t* )data->GetPointer(), ( size_t )size };
	WebPDemuxer * const demux = WebPDemux( &wpdata );
	DEASSERT_NOTNULL( demux )
	
	const uint32_t flags = WebPDemuxGetI( demux, WEBP_FF_FORMAT_FLAGS );
	if( ( flags & EXIF_FLAG ) == EXIF_FLAG ){
		WebPChunkIterator iter = {};
		if( WebPDemuxGetChunk( demux, "EXIF", 1, &iter ) == 1 ){
			static const char * const tagGrayscale = "dewebp:grayscale";
			static const int tagGrayscaleLen = (int)strlen( tagGrayscale );
			
			const char * const exif = ( const char* )iter.chunk.bytes;
			const int exifLen = ( int )iter.chunk.size;
			int i;
			
			for( i=0; i<exifLen; i++ ){
				if( strncmp( exif + i, tagGrayscale, tagGrayscaleLen ) == 0 ){
					info2D.isGrayscale = true;
					break;
				}
			}
			
			WebPDemuxReleaseChunkIterator( &iter );
		}
	}
	
	WebPDemuxDelete( demux );
}

void deWebp3DTarball::Load2DImage( deWebp3DImageInfo &info3D,
decBaseFileReader &file, int size, void *imagedata ){
	DEASSERT_NOTNULL( imagedata )
	
	const decMemoryFile::Ref data( decMemoryFile::Ref::New( new decMemoryFile( "data" ) ) );
	data->Resize( size );
	file.Read( data->GetPointer(), size );
	
	uint8_t *readTarget = nullptr;
	sRGBA8 *bufferRGBA = nullptr;
	sRGB8 *bufferRGB = nullptr;
	uint8_t *result = nullptr;
	int i;
	
	try{
		const int pixelCount = info3D.width * info3D.height;
		
		if( info3D.hasAlpha ){
			if( info3D.isGrayscale ){
				bufferRGBA = new sRGBA8[ pixelCount ];
				readTarget = ( uint8_t* )bufferRGBA;
				
			}else{
				readTarget = ( uint8_t* )imagedata;
			}
			
			result = WebPDecodeRGBAInto( ( const uint8_t* )data->GetPointer(), size,
				readTarget, pixelCount * 4, info3D.width * 4 );
			
		}else{
			if( info3D.isGrayscale ){
				bufferRGB = new sRGB8[ pixelCount ];
				readTarget = ( uint8_t* )bufferRGB;
				
			}else{
				readTarget = ( uint8_t* )imagedata;
			}
			
			result = WebPDecodeRGBInto( ( const uint8_t* )data->GetPointer(), size,
				readTarget, pixelCount * 3, info3D.width * 3 );
		}
		
		if( ! result ){
			DETHROW( deeInvalidFileFormat );
		}
		
		if( bufferRGBA ){
			sGrayscaleAlpha8 *dest = ( sGrayscaleAlpha8* )imagedata;
			
			for( i=0; i<pixelCount; i++ ){
				dest[ i ].value = bufferRGBA[ i ].red;
				dest[ i ].alpha = bufferRGBA[ i ].alpha;
			}
			
			delete [] bufferRGBA;
			bufferRGBA = nullptr;
		}
		
		if( bufferRGB ){
			sGrayscale8 *dest = ( sGrayscale8* )imagedata;
			
			for( i=0; i<pixelCount; i++ ){
				dest[ i ].value = bufferRGB[ i ].red;
			}
			
			delete [] bufferRGB;
			bufferRGB = nullptr;
		}
		
	}catch( const deException & ){
		if( bufferRGBA ){
			delete [] bufferRGBA;
		}
		if( bufferRGB ){
			delete [] bufferRGB;
		}
		throw;
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
		
		file.Write( output, ( int )size );
		
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
