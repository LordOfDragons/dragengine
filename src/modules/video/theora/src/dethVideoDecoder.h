/* 
 * Drag[en]gine Theora Video Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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

#ifndef _DETHVIDEODECODER_H_
#define _DETHVIDEODECODER_H_

#include <vorbis/codec.h>
#include <theora/codec.h>
#include <theora/theoradec.h>

#include <dragengine/systems/modules/video/deBaseVideoDecoder.h>

class dethOggReader;
class deVideoTheora;



/**
 * \brief Theora video decoder.
 */
class dethVideoDecoder : public deBaseVideoDecoder{
public:
	/** \brief Conversion parameters. */
	struct sConversionParamers{
		unsigned char offY;
		unsigned char offCb;
		unsigned char offCr;
		float exclY;
		float exclCb;
		float exclCr;
		float kr;
		float kb;
		
		float c1;
		float c2;
		float c3;
		float c4;
		
		float matrix[12];
	};
	
	/** \brief Pixel formats. */
	enum ePixelFormats{
		/** \brief 4:4:4 */
		epf444,
		
		/** \brief 4:2:2 */
		epf422,
		
		/** \brief 4:2:0 */
		epf420,
		
		/** \brief Unsupported. */
		epfUnsupported
	};
	
	
	
private:
	deVideoTheora &pModule;
	dethOggReader *pReader;
	
	int pWidth;
	int pHeight;
	int pPixelFormat;
	int pFrameCount;
	int pFrameRate;
	decColorMatrix3 pClrConvMat;
	
	int pFrameWidth;
	int pFrameHeight;
	int pPictureX;
	int pPictureY;
	int pInternalPixelFormat;
	
	sConversionParamers pConvParams;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create decoder. */
	dethVideoDecoder( deVideoTheora &module, decBaseFileReader *file );
	
	/** \brief Clean up decoder. */
	virtual ~dethVideoDecoder();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief File position in frames from the beginning. */
	virtual int GetPosition();
	
	/** \brief Set file position in frames from the beginning. */
	virtual void SetPosition( int position );
	
	/**
	 * \brief Decode next frame into buffer and advances file position.
	 * 
	 * Depending on the pixel format one or two buffers are provided. \em buffer1 is
	 * always used and is of size width/height. \em buffer2 is only provided if one
	 * of the reduced pixel formats is used. In this case \em buffer1 contains 1
	 * color plane and \em buffer2 2 color planes with half or quarter size.
	 * Otherwise \em buffer1 contains all three color planes. In all cases the color
	 * planes are interleaved.
	 * 
	 * If successful the file position is advanced. Returns \em true if the frame
	 * has been decoded successfully. Otherwise \em fals is returned and an error
	 * is signaled using the engine error signaling.
	 */
	virtual bool DecodeFrame( void *buffer1, int size1, void *buffer2, int size2 );
	
	/** \brief Create default conversion parameters. */
	void DefConvParams( sConversionParamers &convParams );
	
	/** \brief Convert colors. */
	void ConvertColors( void *buffer );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
