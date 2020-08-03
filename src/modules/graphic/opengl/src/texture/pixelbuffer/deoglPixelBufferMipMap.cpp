/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "deoglPixelBufferMipMap.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>



// Class deoglPixelBufferMipMap
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPixelBufferMipMap::deoglPixelBufferMipMap( deoglPixelBuffer::ePixelFormats format,
int width, int height, int depth, int maxLevel ){
	if( width < 1 || height < 1 || depth < 1 || maxLevel < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	int levelHeight = height;
	int levelWidth = width;
	int count;
	
	pPixelBuffers = NULL;
	pPixelBufferCount = 0;
	
	count = ( int )( floorf( log2f( ( height > width ) ? height : width ) ) );
	if( count > maxLevel ){
		count = maxLevel;
	}
	count++; // pixel buffer count is max mip map level + 1
	
	try{
		pPixelBuffers = new deoglPixelBuffer*[ count ];
		
		for( pPixelBufferCount=0; pPixelBufferCount<count; pPixelBufferCount++ ){
			pPixelBuffers[ pPixelBufferCount ] = new deoglPixelBuffer( format, levelWidth, levelHeight, depth );
			
			levelWidth >>= 1;
			if( levelWidth < 1 ){
				levelWidth = 1;
			}
			
			levelHeight >>= 1;
			if( levelHeight < 1 ){
				levelHeight = 1;
			}
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglPixelBufferMipMap::~deoglPixelBufferMipMap(){
	pCleanUp();
}



// Management
///////////////

deoglPixelBuffer *deoglPixelBufferMipMap::GetPixelBuffer( int level ) const{
	if( level < 0 || level >= pPixelBufferCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pPixelBuffers[ level ];
}



void deoglPixelBufferMipMap::CreateMipMaps(){
	CreateMipMaps( true, true, true, true );
}

void deoglPixelBufferMipMap::CreateMipMaps( bool maskRed, bool maskGreen, bool maskBlue, bool maskAlpha ){
	if( pPixelBufferCount < 2 ){
		return;
	}
	
	const deoglPixelBuffer &basePixelBuffer = *pPixelBuffers[ 0 ];
	int componentCount;
	int sourceOffset1;
	int sourceOffset2;
	int sourceOffset3;
	bool floatData;
	int i, x, y, z;
	
	pGetTypeParams( basePixelBuffer.GetFormat(), componentCount, floatData );
	
	if( componentCount < 1 ){
		maskRed = false;
	}
	if( componentCount < 2 ){
		maskGreen = false;
	}
	if( componentCount < 3 ){
		maskBlue = false;
	}
	if( componentCount < 4 ){
		maskAlpha = false;
	}
	
	for( i=1; i<pPixelBufferCount; i++ ){
		const deoglPixelBuffer &sourcePixelBuffer = *pPixelBuffers[ i - 1 ];
		deoglPixelBuffer &destinationPixelBuffer = *pPixelBuffers[ i ];
		const int destinationStride = destinationPixelBuffer.GetWidth() * componentCount;
		const int sourceStride = 2 * sourcePixelBuffer.GetWidth() * componentCount;
		const int sourceScale = 2 * componentCount;
		const int depth = destinationPixelBuffer.GetDepth();
		const int height = destinationPixelBuffer.GetHeight();
		const int width = destinationPixelBuffer.GetWidth();
		
		// calculate source offsets. this is a trick used to deal with the problematic situation for the
		// second highest mip map level which can be 2x1 or 1x2 instead of 2x2. in this situation wrong
		// pixels can be read leading to crashes in the worst case. if the width of the source pixel
		// buffer is 1 in any direction the respective offset is reduced to 0 preventing the problem.
		// in all other cases the offsets are set to the required values for down sampling
		sourceOffset1 = 0;
		sourceOffset2 = 0;
		sourceOffset3 = 0;
		
		if( sourcePixelBuffer.GetWidth() > 1 ){
			sourceOffset1 += componentCount;
			sourceOffset3 += componentCount;
		}
		if( sourcePixelBuffer.GetHeight() > 1 ){
			sourceOffset2 += sourcePixelBuffer.GetWidth() * componentCount;
			sourceOffset3 += sourcePixelBuffer.GetWidth() * componentCount;
		}
		
		// process mip map level
		if( floatData ){
			GLfloat *destinationPointer = ( GLfloat* )destinationPixelBuffer.GetPointer();
			const GLfloat *sourcePointer = ( const GLfloat * )sourcePixelBuffer.GetPointer();
			
			for( z=0; z<depth; z++ ){
				for( y=0; y<height; y++ ){
					for( x=0; x<width; x++ ){
						const GLfloat * const sp1 = sourcePointer + ( x * sourceScale );
						const GLfloat * const sp2 = sp1 + sourceOffset1;
						const GLfloat * const sp3 = sp1 + sourceOffset2;
						const GLfloat * const sp4 = sp1 + sourceOffset3;
						GLfloat * const dp = destinationPointer + x * componentCount;
						
						if( maskRed ){
							dp[0] = ( GLfloat )( ( sp1[0] + sp2[0] + sp3[0] + sp4[0] ) * 0.25f );
						}
						if( maskGreen ){
							dp[1] = ( GLfloat )( ( sp1[1] + sp2[1] + sp3[1] + sp4[1] ) * 0.25f );
						}
						if( maskBlue ){
							dp[2] = ( GLfloat )( ( sp1[2] + sp2[2] + sp3[2] + sp4[2] ) * 0.25f );
						}
						if( maskAlpha ){
							dp[3] = ( GLfloat )( ( sp1[3] + sp2[3] + sp3[3] + sp4[3] ) * 0.25f );
						}
					}
					
					sourcePointer += sourceStride;
					destinationPointer += destinationStride;
				}
			}
			
		}else{
			GLubyte *destinationPointer = ( GLubyte* )destinationPixelBuffer.GetPointer();
			const GLubyte *sourcePointer = ( const GLubyte * )sourcePixelBuffer.GetPointer();
			
			for( z=0; z<depth; z++ ){
				for( y=0; y<height; y++ ){
					for( x=0; x<width; x++ ){
						const GLubyte * const sp1 = sourcePointer + ( x * sourceScale );
						const GLubyte * const sp2 = sp1 + sourceOffset1;
						const GLubyte * const sp3 = sp1 + sourceOffset2;
						const GLubyte * const sp4 = sp1 + sourceOffset3;
						GLubyte * const dp = destinationPointer + x * componentCount;
						
						if( maskRed ){
							dp[0] = ( GLubyte )( ( sp1[0] + sp2[0] + sp3[0] + sp4[0] ) >> 2 );
						}
						if( maskGreen ){
							dp[1] = ( GLubyte )( ( sp1[1] + sp2[1] + sp3[1] + sp4[1] ) >> 2 );
						}
						if( maskBlue ){
							dp[2] = ( GLubyte )( ( sp1[2] + sp2[2] + sp3[2] + sp4[2] ) >> 2 );
						}
						if( maskAlpha ){
							dp[3] = ( GLubyte )( ( sp1[3] + sp2[3] + sp3[3] + sp4[3] ) >> 2 );
						}
					}
					
					sourcePointer += sourceStride;
					destinationPointer += destinationStride;
				}
			}
		}
	}
}

void deoglPixelBufferMipMap::CreateMipMapsMax(){
	CreateMipMapsMax( true, true, true, true );
}

void deoglPixelBufferMipMap::CreateMipMapsMax( bool maskRed, bool maskGreen, bool maskBlue, bool maskAlpha ){
	if( pPixelBufferCount < 2 ){
		return;
	}
	
	const deoglPixelBuffer &basePixelBuffer = *pPixelBuffers[ 0 ];
	int componentCount;
	int sourceOffset1;
	int sourceOffset2;
	int sourceOffset3;
	bool floatData;
	int i, x, y, z;
	
	pGetTypeParams( basePixelBuffer.GetFormat(), componentCount, floatData );
	
	if( componentCount < 1 ){
		maskRed = false;
	}
	if( componentCount < 2 ){
		maskGreen = false;
	}
	if( componentCount < 3 ){
		maskBlue = false;
	}
	if( componentCount < 4 ){
		maskAlpha = false;
	}
	
	for( i=1; i<pPixelBufferCount; i++ ){
		const deoglPixelBuffer &sourcePixelBuffer = *pPixelBuffers[ i - 1 ];
		deoglPixelBuffer &destinationPixelBuffer = *pPixelBuffers[ i ];
		const int destinationStride = destinationPixelBuffer.GetWidth() * componentCount;
		const int sourceStride = 2 * sourcePixelBuffer.GetWidth() * componentCount;
		const int sourceScale = 2 * componentCount;
		const int depth = destinationPixelBuffer.GetDepth();
		const int height = destinationPixelBuffer.GetHeight();
		const int width = destinationPixelBuffer.GetWidth();
		
		// calculate source offsets. this is a trick used to deal with the problematic situation for the
		// second highest mip map level which can be 2x1 or 1x2 instead of 2x2. in this situation wrong
		// pixels can be read leading to crashes in the worst case. if the width of the source pixel
		// buffer is 1 in any direction the respective offset is reduced to 0 preventing the problem.
		// in all other cases the offsets are set to the required values for down sampling
		sourceOffset1 = 0;
		sourceOffset2 = 0;
		sourceOffset3 = 0;
		
		if( sourcePixelBuffer.GetWidth() > 1 ){
			sourceOffset1 += componentCount;
			sourceOffset3 += componentCount;
		}
		if( sourcePixelBuffer.GetHeight() > 1 ){
			sourceOffset2 += sourcePixelBuffer.GetWidth() * componentCount;
			sourceOffset3 += sourcePixelBuffer.GetWidth() * componentCount;
		}
		
		// process mip map level
		if( floatData ){
			GLfloat *destinationPointer = ( GLfloat* )destinationPixelBuffer.GetPointer();
			const GLfloat *sourcePointer = ( const GLfloat * )sourcePixelBuffer.GetPointer();
			
			for( z=0; z<depth; z++ ){
				for( y=0; y<height; y++ ){
					for( x=0; x<width; x++ ){
						const GLfloat * const sp1 = sourcePointer + ( x * sourceScale );
						const GLfloat * const sp2 = sp1 + sourceOffset1;
						const GLfloat * const sp3 = sp1 + sourceOffset2;
						const GLfloat * const sp4 = sp1 + sourceOffset3;
						GLfloat * const dp = destinationPointer + x * componentCount;
						
						if( maskRed ){
							dp[0] = sp1[0];
							if( sp2[0] > dp[0] ){
								dp[0] = sp2[0];
							}
							if( sp3[0] > dp[0] ){
								dp[0] = sp3[0];
							}
							if( sp4[0] > dp[0] ){
								dp[0] = sp4[0];
							}
						}
						if( maskGreen ){
							dp[1] = sp1[1];
							if( sp2[1] > dp[1] ){
								dp[1] = sp2[1];
							}
							if( sp3[1] > dp[1] ){
								dp[1] = sp3[1];
							}
							if( sp4[1] > dp[1] ){
								dp[1] = sp4[1];
							}
						}
						if( maskBlue ){
							dp[2] = sp1[2];
							if( sp2[2] > dp[2] ){
								dp[2] = sp2[2];
							}
							if( sp3[2] > dp[2] ){
								dp[2] = sp3[2];
							}
							if( sp4[2] > dp[2] ){
								dp[2] = sp4[2];
							}
						}
						if( maskAlpha ){
							dp[3] = sp1[3];
							if( sp2[3] > dp[3] ){
								dp[3] = sp2[3];
							}
							if( sp3[3] > dp[3] ){
								dp[3] = sp3[3];
							}
							if( sp4[3] > dp[3] ){
								dp[3] = sp4[3];
							}
						}
					}
					
					sourcePointer += sourceStride;
					destinationPointer += destinationStride;
				}
			}
			
		}else{
			GLubyte *destinationPointer = ( GLubyte* )destinationPixelBuffer.GetPointer();
			const GLubyte *sourcePointer = ( const GLubyte * )sourcePixelBuffer.GetPointer();
			
			for( z=0; z<depth; z++ ){
				for( y=0; y<height; y++ ){
					for( x=0; x<width; x++ ){
						const GLubyte * const sp1 = sourcePointer + ( x * sourceScale );
						const GLubyte * const sp2 = sp1 + sourceOffset1;
						const GLubyte * const sp3 = sp1 + sourceOffset2;
						const GLubyte * const sp4 = sp1 + sourceOffset3;
						GLubyte * const dp = destinationPointer + x * componentCount;
						
						if( maskRed ){
							dp[0] = sp1[0];
							if( sp2[0] > dp[0] ){
								dp[0] = sp2[0];
							}
							if( sp3[0] > dp[0] ){
								dp[0] = sp3[0];
							}
							if( sp4[0] > dp[0] ){
								dp[0] = sp4[0];
							}
						}
						if( maskGreen ){
							dp[1] = sp1[1];
							if( sp2[1] > dp[1] ){
								dp[1] = sp2[1];
							}
							if( sp3[1] > dp[1] ){
								dp[1] = sp3[1];
							}
							if( sp4[1] > dp[1] ){
								dp[1] = sp4[1];
							}
						}
						if( maskBlue ){
							dp[2] = sp1[2];
							if( sp2[2] > dp[2] ){
								dp[2] = sp2[2];
							}
							if( sp3[2] > dp[2] ){
								dp[2] = sp3[2];
							}
							if( sp4[2] > dp[2] ){
								dp[2] = sp4[2];
							}
						}
						if( maskAlpha ){
							dp[3] = sp1[3];
							if( sp2[3] > dp[3] ){
								dp[3] = sp2[3];
							}
							if( sp3[3] > dp[3] ){
								dp[3] = sp3[3];
							}
							if( sp4[3] > dp[3] ){
								dp[3] = sp4[3];
							}
						}
					}
					
					sourcePointer += sourceStride;
					destinationPointer += destinationStride;
				}
			}
		}
	}
}

void deoglPixelBufferMipMap::CreateNormalMipMaps(){
	if( pPixelBufferCount < 2 ){
		return;
	}
	
	const deoglPixelBuffer &basePixelBuffer = *pPixelBuffers[ 0 ];
	int componentCount;
	bool floatData;
	int i, x, y, z;
	
	pGetTypeParams( basePixelBuffer.GetFormat(), componentCount, floatData );
	
	if( componentCount < 4 ){
		// without alpha this is the same as normal mip map creation without alpha
		CreateMipMaps( true, true, true, false );
		return;
	}
	
	const float normalFactor1 = 1.99215686f / 255.0f; // see doc/normalmap
	const float normalFactor2 = 0.99217224f; // see doc/normalmap
	const float varianceFactorFloat = 1.0f / HALF_PI;
	const float varianceFactorByte = 255.0f / HALF_PI;
	int sourceOffset1;
	int sourceOffset2;
	int sourceOffset3;
	decVector normal1;
	decVector normal2;
	decVector normal3;
	decVector normal4;
	decVector normalAverage;
	int dotMinInt;
	float dotMin;
	//float dot;
	
	for( i=1; i<pPixelBufferCount; i++ ){
		const deoglPixelBuffer &sourcePixelBuffer = *pPixelBuffers[ i - 1 ];
		deoglPixelBuffer &destinationPixelBuffer = *pPixelBuffers[ i ];
		const int destinationStride = destinationPixelBuffer.GetWidth() * componentCount;
		const int sourceStride = 2 * sourcePixelBuffer.GetWidth() * componentCount;
		const int sourceScale = 2 * componentCount;
		const int depth = destinationPixelBuffer.GetDepth();
		const int height = destinationPixelBuffer.GetHeight();
		const int width = destinationPixelBuffer.GetWidth();
		
		// calculate source offsets. this is a trick used to deal with the problematic situation for the
		// second highest mip map level which can be 2x1 or 1x2 instead of 2x2. in this situation wrong
		// pixels can be read leading to crashes in the worst case. if the width of the source pixel
		// buffer is 1 in any direction the respective offset is reduced to 0 preventing the problem.
		// in all other cases the offsets are set to the required values for down sampling
		sourceOffset1 = 0;
		sourceOffset2 = 0;
		sourceOffset3 = 0;
		
		if( sourcePixelBuffer.GetWidth() > 1 ){
			sourceOffset1 += componentCount;
			sourceOffset3 += componentCount;
		}
		if( sourcePixelBuffer.GetHeight() > 1 ){
			sourceOffset2 += sourcePixelBuffer.GetWidth() * componentCount;
			sourceOffset3 += sourcePixelBuffer.GetWidth() * componentCount;
		}
		
		// box filter the normal mip map level. this down samples both the normal map and
		// the normal variance map. the normal will be averaged while for the variance
		// the maximum is used
		// 
		// to calculate the deviation the minimum dot product between each normal and the
		// average normal is calculated. this gives the largest deviation from the average
		// normal. a different metric could be used but using the largest deviation results
		// in a conservative solution which is correct more often than other methods
		// 
		// the deviation is stored as a linear function mapping the spread angle in the range
		// of 0 to half-pi to the range from 0 to 1. since the deviation can be only positive
		// clamping is only required towards the largest allowed pixel value
		if( floatData ){
			GLfloat *destinationPointerFloat = ( GLfloat* )destinationPixelBuffer.GetPointer();
			const GLfloat *sourcePointerFloat = ( const GLfloat * )sourcePixelBuffer.GetPointer();
			
			for( z=0; z<depth; z++ ){
				for( y=0; y<height; y++ ){
					for( x=0; x<width; x++ ){
						const GLfloat * const sp1 = sourcePointerFloat + ( x * sourceScale );
						const GLfloat * const sp2 = sp1 + sourceOffset1;
						const GLfloat * const sp3 = sp1 + sourceOffset2;
						const GLfloat * const sp4 = sp1 + sourceOffset3;
						GLfloat * const dp = destinationPointerFloat + x * componentCount;
						
						// box filter the normal using average and the variance using maximum
						dp[0] = ( GLfloat )( ( sp1[0] + sp2[0] + sp3[0] + sp4[0] ) * 0.25f );
						dp[1] = ( GLfloat )( ( sp1[1] + sp2[1] + sp3[1] + sp4[1] ) * 0.25f );
						dp[2] = ( GLfloat )( ( sp1[2] + sp2[2] + sp3[2] + sp4[2] ) * 0.25f );
						
						dp[3] = sp1[3];
						if( sp2[3] > dp[3] ){
							dp[3] = sp2[3];
						}
						if( sp3[3] > dp[3] ){
							dp[3] = sp3[3];
						}
						if( sp4[3] > dp[3] ){
							dp[3] = sp4[3];
						}
						
						// obtain normals and calculate the average
						normal1.x = sp1[0];
						normal1.y = sp1[1];
						normal1.z = sp1[2];
						const float lenNormal1 = normal1.Length();
						if( lenNormal1 > FLOAT_SAFE_EPSILON ){
							normal1 /= lenNormal1;
							
						}else{
							normal1.Set( 0.0f, 0.0f, 1.0f );
						}
						
						normal2.x = sp2[0];
						normal2.y = sp2[1];
						normal2.z = sp2[2];
						const float lenNormal2 = normal2.Length();
						if( lenNormal2 > FLOAT_SAFE_EPSILON ){
							normal2 /= lenNormal2;
							
						}else{
							normal2.Set( 0.0f, 0.0f, 1.0f );
						}
						
						normal3.x = sp3[0];
						normal3.y = sp3[1];
						normal3.z = sp3[2];
						const float lenNormal3 = normal3.Length();
						if( lenNormal3 > FLOAT_SAFE_EPSILON ){
							normal3 /= lenNormal3;
							
						}else{
							normal3.Set( 0.0f, 0.0f, 1.0f );
						}
						
						normal4.x = sp4[0];
						normal4.y = sp4[1];
						normal4.z = sp4[2];
						const float lenNormal4 = normal4.Length();
						if( lenNormal4 > FLOAT_SAFE_EPSILON ){
							normal4 /= lenNormal4;
							
						}else{
							normal4.Set( 0.0f, 0.0f, 1.0f );
						}
						
						normalAverage = ( normal1 + normal2 + normal3 + normal4 ) * 0.25f;
						const float lenNormalAverage = normalAverage.Length();
						if( lenNormalAverage > FLOAT_SAFE_EPSILON ){
							normalAverage /= lenNormalAverage;
							
						}else{
							normalAverage.Set( 0.0f, 0.0f, 1.0f );
						}
						
						// calculate the largest deviation and convert it to the correct range
						/*
						dotMin = normal1 * normalAverage;
						
						dot = normal2 * normalAverage;
						if( dot < dotMin ){
							dotMin = dot;
						}
						
						dot = normal3 * normalAverage;
						if( dot < dotMin ){
							dotMin = dot;
						}
						
						dot = normal4 * normalAverage;
						if( dot < dotMin ){
							dotMin = dot;
						}
						*/
						dotMin = ( normal1 * normalAverage + normal2 * normalAverage + normal3 * normalAverage + normal4 * normalAverage ) * 0.25f;
						dotMin = decMath::clamp( dotMin, -1.0f, 1.0f ); // just to be on the safe side
						
						dotMin = acosf( dotMin ) * varianceFactorFloat;
						
						// add the variance to the box filtered variance clamp to the valid range
						dotMin += ( float )dp[3];
						
						if( dotMin < 1.0f ){
							dp[3] = ( GLfloat )dotMin;
							
						}else{
							dp[3] = 1.0f;
						}
					}
					
					sourcePointerFloat += sourceStride;
					destinationPointerFloat += destinationStride;
				}
			}
			
		}else{
			GLubyte *destinationPointerByte = ( GLubyte* )destinationPixelBuffer.GetPointer();
			const GLubyte *sourcePointerByte = ( const GLubyte * )sourcePixelBuffer.GetPointer();
			
			for( z=0; z<depth; z++ ){
				for( y=0; y<height; y++ ){
					for( x=0; x<width; x++ ){
						const GLubyte * const sp1 = sourcePointerByte + ( x * sourceScale );
						const GLubyte * const sp2 = sp1 + sourceOffset1;
						const GLubyte * const sp3 = sp1 + sourceOffset2;
						const GLubyte * const sp4 = sp1 + sourceOffset3;
						GLubyte * const dp = destinationPointerByte + x * componentCount;
						
						// box filter the normal using average and the variance using maximum
						dp[0] = ( GLubyte )( ( sp1[0] + sp2[0] + sp3[0] + sp4[0] ) >> 2 );
						dp[1] = ( GLubyte )( ( sp1[1] + sp2[1] + sp3[1] + sp4[1] ) >> 2 );
						dp[2] = ( GLubyte )( ( sp1[2] + sp2[2] + sp3[2] + sp4[2] ) >> 2 );
						
						dp[3] = sp1[3];
						if( sp2[3] > dp[3] ){
							dp[3] = sp2[3];
						}
						if( sp3[3] > dp[3] ){
							dp[3] = sp3[3];
						}
						if( sp4[3] > dp[3] ){
							dp[3] = sp4[3];
						}
						
						// obtain normals and calculate the average
						normal1.x = ( float )sp1[0] * normalFactor1 - normalFactor2;
						normal1.y = ( float )sp1[1] * normalFactor1 - normalFactor2;
						normal1.z = ( float )sp1[2] * normalFactor1 - normalFactor2;
						const float lenNormal1 = normal1.Length();
						if( lenNormal1 > FLOAT_SAFE_EPSILON ){
							normal1 /= lenNormal1;
							
						}else{
							normal1.Set( 0.0f, 0.0f, 1.0f );
						}
						
						normal2.x = ( float )sp2[0] * normalFactor1 - normalFactor2;
						normal2.y = ( float )sp2[1] * normalFactor1 - normalFactor2;
						normal2.z = ( float )sp2[2] * normalFactor1 - normalFactor2;
						const float lenNormal2 = normal2.Length();
						if( lenNormal2 > FLOAT_SAFE_EPSILON ){
							normal2 /= lenNormal2;
							
						}else{
							normal2.Set( 0.0f, 0.0f, 1.0f );
						}
						
						normal3.x = ( float )sp3[0] * normalFactor1 - normalFactor2;
						normal3.y = ( float )sp3[1] * normalFactor1 - normalFactor2;
						normal3.z = ( float )sp3[2] * normalFactor1 - normalFactor2;
						const float lenNormal3 = normal3.Length();
						if( lenNormal3 > FLOAT_SAFE_EPSILON ){
							normal3 /= lenNormal3;
							
						}else{
							normal3.Set( 0.0f, 0.0f, 1.0f );
						}
						
						normal4.x = ( float )sp4[0] * normalFactor1 - normalFactor2;
						normal4.y = ( float )sp4[1] * normalFactor1 - normalFactor2;
						normal4.z = ( float )sp4[2] * normalFactor1 - normalFactor2;
						const float lenNormal4 = normal4.Length();
						if( lenNormal4 > FLOAT_SAFE_EPSILON ){
							normal4 /= lenNormal4;
							
						}else{
							normal4.Set( 0.0f, 0.0f, 1.0f );
						}
						
						normalAverage = ( normal1 + normal2 + normal3 + normal4 ) * 0.25f;
						const float lenNormalAverage = normalAverage.Length();
						if( lenNormalAverage > FLOAT_SAFE_EPSILON ){
							normalAverage /= lenNormalAverage;
							
						}else{
							normalAverage.Set( 0.0f, 0.0f, 1.0f );
						}
						
						// calculate the largest deviation and convert it to the correct range
						dotMin = normal1 * normalAverage;
						
						/*
						dot = normal2 * normalAverage;
						if( dot < dotMin ){
							dotMin = dot;
						}
						
						dot = normal3 * normalAverage;
						if( dot < dotMin ){
							dotMin = dot;
						}
						
						dot = normal4 * normalAverage;
						if( dot < dotMin ){
							dotMin = dot;
						}
						*/
						dotMin = ( normal1 * normalAverage + normal2 * normalAverage + normal3 * normalAverage + normal4 * normalAverage ) * 0.25f;
						dotMin = decMath::clamp( dotMin, -1.0f, 1.0f ); // just to be on the safe side
						
						dotMinInt = ( int )( acosf( dotMin ) * varianceFactorByte );
						
						// add the variance to the box filtered variance clamp to the valid range
						dotMinInt += ( int )dp[3];
						
						if( dotMinInt < 255 ){
							dp[3] = ( GLubyte )dotMinInt;
							
						}else{
							dp[3] = 255;
						}
					}
					
					sourcePointerByte += sourceStride;
					destinationPointerByte += destinationStride;
				}
			}
		}
	}
}

void deoglPixelBufferMipMap::CreateRoughnessMipMaps( deoglPixelBufferMipMap &normalPixeBufferMipMap ){
	if( pPixelBufferCount < 2 ){
		return;
	}
	
	const deoglPixelBuffer &baseNormalPixelBuffer = *normalPixeBufferMipMap.GetPixelBuffer( 0 );
	const deoglPixelBuffer &basePixelBuffer = *pPixelBuffers[ 0 ];
	int normalComponentCount;
	int componentCount;
	bool normalFloatData;
	bool floatData;
	int i, x, y, z;
	
	pGetTypeParams( basePixelBuffer.GetFormat(), componentCount, floatData );
	pGetTypeParams( baseNormalPixelBuffer.GetFormat(), normalComponentCount, normalFloatData );
	
	if( componentCount < 4 ){
		return; // alpha is required
	}
	if( normalComponentCount < 3 ){
		return; // rgb required
	}
	
	// determine the level of the normal mip map matching best the base level of the roughness texture.
	// opengl chooses for a fragment rendered on the screen the mip map level depending on the size
	// of the mip map levels. more precisely it chooses the mip map level where the mapped size of a
	// texel to screen pixel is as close to 1 pixel as possible. thus for a given fragment opengl
	// only looks at the size of the mip map levels. thus mip map levels of the roughness texture have
	// to be processed with normal mip map levels matching the size. for this the mip map level matching
	// the base roughness mip map level is calculated. this is allowed to be negative since later on
	// a check is used to adjust roughness only if a valid normal mip map level exists
	const int baseNormalHeight = baseNormalPixelBuffer.GetHeight();
	const int baseNormalWidth = baseNormalPixelBuffer.GetWidth();
	const int baseNormalMipMapSize = ( baseNormalWidth > baseNormalHeight ) ? baseNormalWidth : baseNormalHeight;
	const int baseNormalLevel =  pPixelBufferCount - 1 - ( int )floorf( log2( ( float )baseNormalMipMapSize ) + 0.5f );
	const int normalMaxLevel = normalPixeBufferMipMap.GetPixelBufferCount() - 1;
	const int byteFactor = 1.0f / 255.0f;
	const GLfloat *normalPointerOrgFloat = NULL;
	const GLubyte *normalPointerOrgByte = NULL;
	const GLfloat *normalPointerFloat = NULL;
	const GLubyte *normalPointerByte = NULL;
	const GLfloat *sourcePointerFloat;
	GLfloat *destinationPointerFloat;
	const GLubyte *sourcePointerByte;
	GLubyte *destinationPointerByte;
	bool processNormalLevel;
	float normalXTransform;
	float normalYTransform;
	int sourceOffset1;
	int sourceOffset2;
	int sourceOffset3;
	int normalOffset1;
	int normalOffset2;
	int normalOffset3;
	int normalStride;
	int normalScale;
	int normalLevel;
	decVector normal1;
	decVector normal2;
	decVector normal3;
	decVector normal4;
	decVector normalAverage;
	float dotMin;
	float dot;
	
	// process the roughness mip map levels
	for( i=1; i<pPixelBufferCount; i++ ){
		//const deoglPixelBuffer &sourceNormalPixelBuffer = *normalPixeBufferMipMap.GetPixelBuffer( i - 1 );
		const deoglPixelBuffer &sourcePixelBuffer = *pPixelBuffers[ i - 1 ];
		deoglPixelBuffer &destinationPixelBuffer = *pPixelBuffers[ i ];
		const int destinationStride = destinationPixelBuffer.GetWidth() * componentCount;
		const int sourceStride = 2 * sourcePixelBuffer.GetWidth() * componentCount;
		const int sourceScale = 2 * componentCount;
		const int depth = destinationPixelBuffer.GetDepth();
		const int height = destinationPixelBuffer.GetHeight();
		const int width = destinationPixelBuffer.GetWidth();
		
		// calculate source offsets. this is a trick used to deal with the problematic situation for the
		// second highest mip map level which can be 2x1 or 1x2 instead of 2x2. in this situation wrong
		// pixels can be read leading to crashes in the worst case. if the width of the source pixel
		// buffer is 1 in any direction the respective offset is reduced to 0 preventing the problem.
		// in all other cases the offsets are set to the required values for down sampling
		sourceOffset1 = 0;
		sourceOffset2 = 0;
		sourceOffset3 = 0;
		
		if( sourcePixelBuffer.GetWidth() > 1 ){
			sourceOffset1 += componentCount;
			sourceOffset3 += componentCount;
		}
		if( sourcePixelBuffer.GetHeight() > 1 ){
			sourceOffset2 += sourcePixelBuffer.GetWidth() * componentCount;
			sourceOffset3 += sourcePixelBuffer.GetWidth() * componentCount;
		}
		
		// box filter the roughness mip map level
		if( floatData ){
			destinationPointerFloat = ( GLfloat* )destinationPixelBuffer.GetPointer();
			sourcePointerFloat = ( const GLfloat * )sourcePixelBuffer.GetPointer();
			
			for( z=0; z<depth; z++ ){
				for( y=0; y<height; y++ ){
					for( x=0; x<width; x++ ){
						const GLfloat * const sp1 = sourcePointerFloat + ( x * sourceScale );
						const GLfloat * const sp2 = sp1 + sourceOffset1;
						const GLfloat * const sp3 = sp1 + sourceOffset2;
						const GLfloat * const sp4 = sp1 + sourceOffset3;
						GLfloat * const dp = destinationPointerFloat + x * componentCount;
						
						dp[3] = ( GLfloat )( ( sp1[3] + sp2[3] + sp3[3] + sp4[3] ) * 0.25f );
					}
					
					sourcePointerFloat += sourceStride;
					destinationPointerFloat += destinationStride;
				}
			}
			
		}else{
			destinationPointerByte = ( GLubyte* )destinationPixelBuffer.GetPointer();
			sourcePointerByte = ( const GLubyte * )sourcePixelBuffer.GetPointer();
			
			for( z=0; z<depth; z++ ){
				for( y=0; y<height; y++ ){
					for( x=0; x<width; x++ ){
						const GLubyte * const sp1 = sourcePointerByte + ( x * sourceScale );
						const GLubyte * const sp2 = sp1 + sourceOffset1;
						const GLubyte * const sp3 = sp1 + sourceOffset2;
						const GLubyte * const sp4 = sp1 + sourceOffset3;
						GLubyte * const dp = destinationPointerByte + x * componentCount;
						
						dp[3] = ( GLubyte )( ( sp1[3] + sp2[3] + sp3[3] + sp4[3] ) >> 2 );
					}
					
					sourcePointerByte += sourceStride;
					destinationPointerByte += destinationStride;
				}
			}
		}
		
		// determine the normal map processing parameters if a normal map has to be used for this level
		processNormalLevel = false;
		normalLevel = baseNormalLevel + i;
		
		if( normalLevel > 0 && normalLevel <= normalMaxLevel ){ // perhaps < normalMaxLevel
			const deoglPixelBuffer &normalPixelBuffer1 = *normalPixeBufferMipMap.GetPixelBuffer( normalLevel - 1 );
			
			// this is a shortcut to avoid complicated calculations. if the source normal mip map level has any
			// dimension less than 2 (hence less than 2x2) the texture coordinate transformation factor has to
			// deal with a division by zero. to avoid this calculation only a source normal mip map level of the
			// size 2x2 or larger is used and the trouble case silently ignored. at very high mip map levels not
			// processing the normal map is impossible to spot but prevents the difficult cases
			if( normalPixelBuffer1.GetWidth() > 1 && normalPixelBuffer1.GetHeight() > 1 ){
				processNormalLevel = true;
				
				// for the offsets we use the same 2x2 pattern even if the normal mip map level and roughness
				// mip map level dimensions do not match. this is a rare case and leads anyways to debatable
				// results hence using the simple and fast 2x2 version is well enough
				const deoglPixelBuffer &normalPixelBuffer2 = *normalPixeBufferMipMap.GetPixelBuffer( normalLevel );
				normalXTransform = ( float )( ( normalPixelBuffer2.GetWidth() - 1 ) * 2 ) / ( float )( width - 1 );
				normalYTransform = ( float )( ( normalPixelBuffer2.GetHeight() - 1 ) * 2 ) / ( float )( height - 1 );
				normalStride = normalPixelBuffer1.GetWidth() * normalComponentCount;
				normalScale = 2 * normalComponentCount;
				normalOffset1 = normalComponentCount;
				normalOffset2 = normalPixelBuffer1.GetWidth() * normalComponentCount;
				normalOffset3 = normalPixelBuffer1.GetWidth() * normalComponentCount + normalComponentCount;
				
				if( normalFloatData ){
					normalPointerOrgFloat = ( const GLfloat * )normalPixelBuffer1.GetPointer();
					
				}else{
					normalPointerOrgByte = ( const GLubyte * )normalPixelBuffer1.GetPointer();
				}
			}
		}
		
		// modify the mip map level with the normal map information if existing
		if( processNormalLevel ){
			if( floatData ){
				destinationPointerFloat = ( GLfloat* )destinationPixelBuffer.GetPointer();
				sourcePointerFloat = ( const GLfloat * )sourcePixelBuffer.GetPointer();
				destinationPointerByte = NULL;
				sourcePointerByte = NULL;
				
			}else{
				destinationPointerFloat = NULL;
				sourcePointerFloat = NULL;
				destinationPointerByte = ( GLubyte* )destinationPixelBuffer.GetPointer();
				sourcePointerByte = ( const GLubyte * )sourcePixelBuffer.GetPointer();
			}
			
			for( z=0; z<depth; z++ ){
				for( y=0; y<height; y++ ){
					if( processNormalLevel ){
						const int ny = ( int )( normalYTransform * ( float )y );
						
						if( normalFloatData ){
							normalPointerFloat = normalPointerOrgFloat + ( normalStride * ny );
							
						}else{
							normalPointerByte = normalPointerOrgByte + ( normalStride * ny );
						}
					}
					
					for( x=0; x<width; x++ ){
						const int nx = ( int )( normalXTransform * ( float )x );
						
						// retrieve the four normals of the lower mip map level
						if( normalFloatData ){
							const GLfloat * const np1 = normalPointerFloat + ( nx * normalScale );
							const GLfloat * const np2 = np1 + normalOffset1;
							const GLfloat * const np3 = np1 + normalOffset2;
							const GLfloat * const np4 = np1 + normalOffset3;
							
							normal1.x = np1[0];
							normal1.y = np1[1];
							normal1.z = np1[2];
							
							normal2.x = np2[0];
							normal2.y = np2[1];
							normal2.z = np2[2];
							
							normal3.x = np3[0];
							normal3.y = np3[1];
							normal3.z = np3[2];
							
							normal4.x = np4[0];
							normal4.y = np4[1];
							normal4.z = np4[2];
							
						}else{
							const GLubyte * const np1 = normalPointerByte + ( nx * normalScale );
							const GLubyte * const np2 = np1 + normalOffset1;
							const GLubyte * const np3 = np1 + normalOffset2;
							const GLubyte * const np4 = np1 + normalOffset3;
							
							normal1.x = ( ( float )np1[0] / 127.5f ) - 1.0f;
							normal1.y = ( ( float )np1[1] / 127.5f ) - 1.0f;
							normal1.z = ( ( float )np1[2] / 127.5f ) - 1.0f;
							
							normal2.x = ( ( float )np2[0] / 127.5f ) - 1.0f;
							normal2.y = ( ( float )np2[1] / 127.5f ) - 1.0f;
							normal2.z = ( ( float )np2[2] / 127.5f ) - 1.0f;
							
							normal3.x = ( ( float )np3[0] / 127.5f ) - 1.0f;
							normal3.y = ( ( float )np3[1] / 127.5f ) - 1.0f;
							normal3.z = ( ( float )np3[2] / 127.5f ) - 1.0f;
							
							normal4.x = ( ( float )np4[0] / 127.5f ) - 1.0f;
							normal4.y = ( ( float )np4[1] / 127.5f ) - 1.0f;
							normal4.z = ( ( float )np4[2] / 127.5f ) - 1.0f;
						}
						
						// calculate the average. this is the same as the normal on the current mip map level.
						// it is faster to calculate it like this instead of trying to track the memory pointer
						// to read from the matching normal map level
						normalAverage = ( normal1 + normal2 + normal3 + normal4 ) * 0.25f;
						
						// calculate the minimum dot product between each normal and the average normal. this
						// gives the largest deviation from the average normal. a different metric could be
						// used but using the largest deviation results in a conservative solution which is
						// correct more often than other methods
						dotMin = normal1 * normalAverage;
						
						dot = normal2 * normalAverage;
						if( dot < dotMin ){
							dotMin = dot;
						}
						
						dot = normal3 * normalAverage;
						if( dot < dotMin ){
							dotMin = dot;
						}
						
						dot = normal4 * normalAverage;
						if( dot < dotMin ){
							dotMin = dot;
						}
						
						// calculate the roughness correction. the found deviation can be directly converted to
						// a roughness offset value. this is possible since roughness is defined as a linear
						// function mapping the spread angle in the range of 0 to half-pi to the range from 0
						// to 1. thus the deviation of the normals is an angle which in turn can be directly
						// converted to an increase in roughness
						dotMin = decMath::clamp( dotMin, -1.0f, 1.0f ); // just to be on the safe side
						dotMin = acosf( dotMin ) / HALF_PI;
						
						// apply the correction value to the roughness. since the correction is always positive
						// the resulting roughness value has to be only clamped at 1
						if( floatData ){
							GLfloat * const dp = destinationPointerFloat + x * componentCount;
							
							dotMin += ( float )dp[3];
							
							if( dotMin < 1.0f ){
								dp[3] = ( GLfloat )dotMin;
								
							}else{
								dp[3] = 1.0f;
							}
							
						}else{
							GLubyte * const dp = destinationPointerByte + x * componentCount;
							
							dotMin += ( float )dp[3] * byteFactor;
							
							if( dotMin < 1.0f ){
								dp[3] = ( GLubyte )( dotMin * 255.0f );
								
							}else{
								dp[3] = 255;
							}
						}
					}
					
					if( floatData ){
						sourcePointerFloat += sourceStride;
						destinationPointerFloat += destinationStride;
						
					}else{
						sourcePointerByte += sourceStride;
						destinationPointerByte += destinationStride;
					}
				}
			}
		}
	}
}



// Private Functions
//////////////////////

void deoglPixelBufferMipMap::pCleanUp(){
	if( pPixelBuffers ){
		while( pPixelBufferCount > 0 ){
			delete pPixelBuffers[ --pPixelBufferCount ];
		}
		delete [] pPixelBuffers;
	}
}

void deoglPixelBufferMipMap::pGetTypeParams( int pixelBufferType, int &componentCount, bool &floatData ) const{
	if( pixelBufferType == deoglPixelBuffer::epfByte1 ){
		componentCount = 1;
		floatData = false;
		
	}else if( pixelBufferType == deoglPixelBuffer::epfByte2 ){
		componentCount = 2;
		floatData = false;
		
	}else if( pixelBufferType == deoglPixelBuffer::epfByte3 ){
		componentCount = 3;
		floatData = false;
		
	}else if( pixelBufferType == deoglPixelBuffer::epfByte4 ){
		componentCount = 4;
		floatData = false;
		
	}else if( pixelBufferType == deoglPixelBuffer::epfFloat1 ){
		componentCount = 1;
		floatData = true;
		
	}else if( pixelBufferType == deoglPixelBuffer::epfFloat2 ){
		componentCount = 2;
		floatData = true;
		
	}else if( pixelBufferType == deoglPixelBuffer::epfFloat3 ){
		componentCount = 3;
		floatData = true;
		
	}else if( pixelBufferType == deoglPixelBuffer::epfFloat4 ){
		componentCount = 4;
		floatData = true;
		
	}else{
		DETHROW( deeInvalidParam );
	}
}
