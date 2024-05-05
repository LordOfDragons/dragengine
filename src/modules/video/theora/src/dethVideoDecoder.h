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
	int pFrameCount;
	float pFrameRate;
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
	 * Depending on the deVideo::GetComponentCount() \em buffer contains 1 to 4 components
	 * per color in the order R, G, B and A. The parameter \em size is provided as fail
	 * check for the decoder to ensure he is expecting the correct buffer size.
	 * 
	 * If successful the file position is advanced. Returns true if the frame
	 * has been decoded successfully. Otherwise \em fals is returned and an error
	 * is signaled using the engine error signaling.
	 */
	virtual bool DecodeFrame( void *buffer, int size );
	
	/** \brief Create default conversion parameters. */
	void DefConvParams( sConversionParamers &convParams );
	
	/** \brief Convert colors. */
	void ConvertColors( void *buffer );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
