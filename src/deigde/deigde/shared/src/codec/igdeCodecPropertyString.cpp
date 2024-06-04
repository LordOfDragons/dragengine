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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "igdeCodecPropertyString.h"
#include "igdePropertyStringEncodeShapeList.h"
#include <dragengine/common/shape/decShapeCapsule.h>
#include <dragengine/common/shape/decShapeCylinder.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeList.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/common/shape/decShapeHull.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringList.h>



// Definitions
////////////////

static const char *vSeparators = " \t\n\r";



// Class igdeCodecPropertyString
//////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeCodecPropertyString::igdeCodecPropertyString(){
}

igdeCodecPropertyString::~igdeCodecPropertyString(){
}



// Management
///////////////



// Decoding
/////////////

int igdeCodecPropertyString::DecodeFixedFloatList( const char *string, float *values, int valueCount ) const{
	decStringList list = decString( string ).Split( vSeparators );
	int i, count = list.GetCount();
	
	if( count > valueCount ){
		count = valueCount;
	}
	
	for( i=0; i<count; i++ ){
		values[ i ] = list.GetAt( i ).ToFloatValid();
	}
	
	return count;
}

int igdeCodecPropertyString::DecodeFixedDoubleList( const char *string, double *values, int valueCount ) const{
	decStringList list = decString( string ).Split( vSeparators );
	int i, count = list.GetCount();
	
	if( count > valueCount ){
		count = valueCount;
	}
	
	for( i=0; i<count; i++ ){
		values[ i ] = list.GetAt( i ).ToDoubleValid();
	}
	
	return count;
}

int igdeCodecPropertyString::DecodeFixedIntList( const char *string, int *values, int valueCount ) const{
	decStringList list = decString( string ).Split( vSeparators );
	int i, count = list.GetCount();
	
	if( count > valueCount ){
		count = valueCount;
	}
	
	for( i=0; i<count; i++ ){
		values[ i ] = list.GetAt( i ).ToIntValid();
	}
	
	return count;
}

void igdeCodecPropertyString::DecodeVector( const char *string, decVector &vector ) const{
	if( DecodeFixedFloatList( string, &vector.x, 3 ) != 3 ){
		DETHROW( deeInvalidParam );
	}
}

void igdeCodecPropertyString::DecodeDVector( const char *string, decDVector &vector ) const{
	if( DecodeFixedDoubleList( string, &vector.x, 3 ) != 3 ){
		DETHROW( deeInvalidParam );
	}
}

void igdeCodecPropertyString::DecodeVector( const char *string, decVector &vector,
const decVector &defaultVector ) const{
	const int count = DecodeFixedFloatList( string, &vector.x, 3 );
	
	if( count < 1 ){
		vector.x = defaultVector.x;
	}
	if( count < 2 ){
		vector.y = defaultVector.y;
	}
	if( count < 3 ){
		vector.z = defaultVector.z;
	}
}

void igdeCodecPropertyString::DecodeDVector( const char *string, decDVector &vector,
const decDVector &defaultVector ) const{
	const int count = DecodeFixedDoubleList( string, &vector.x, 3 );
	
	if( count < 1 ){
		vector.x = defaultVector.x;
	}
	if( count < 2 ){
		vector.y = defaultVector.y;
	}
	if( count < 3 ){
		vector.z = defaultVector.z;
	}
}

void igdeCodecPropertyString::DecodeVector2( const char *string, decVector2 &vector ) const{
	if( DecodeFixedFloatList( string, &vector.x, 2 ) != 2 ){
		DETHROW( deeInvalidParam );
	}
}

void igdeCodecPropertyString::DecodeVector2( const char *string, decVector2 &vector,
const decVector2 &defaultVector ) const{
	const int count = DecodeFixedFloatList( string, &vector.x, 3 );
	
	if( count < 1 ){
		vector.x = defaultVector.x;
	}
	if( count < 2 ){
		vector.y = defaultVector.y;
	}
}

void igdeCodecPropertyString::DecodeVector4( const char *string, decVector4 &vector ) const{
	if( DecodeFixedFloatList( string, &vector.x, 4 ) != 4 ){
		DETHROW( deeInvalidParam );
	}
}

void igdeCodecPropertyString::DecodeDVector4( const char *string, decDVector4 &vector ) const{
	if( DecodeFixedDoubleList( string, &vector.x, 4 ) != 4 ){
		DETHROW( deeInvalidParam );
	}
}

void igdeCodecPropertyString::DecodeVector4( const char *string, decVector4 &vector,
const decVector4 &defaultVector ) const{
	const int count = DecodeFixedFloatList( string, &vector.x, 3 );
	
	if( count < 1 ){
		vector.x = defaultVector.x;
	}
	if( count < 2 ){
		vector.y = defaultVector.y;
	}
	if( count < 3 ){
		vector.z = defaultVector.z;
	}
	if( count < 4 ){
		vector.w = defaultVector.w;
	}
}

void igdeCodecPropertyString::DecodeDVector4( const char *string, decDVector4 &vector,
const decDVector4 &defaultVector ) const{
	const int count = DecodeFixedDoubleList( string, &vector.x, 3 );
	
	if( count < 1 ){
		vector.x = defaultVector.x;
	}
	if( count < 2 ){
		vector.y = defaultVector.y;
	}
	if( count < 3 ){
		vector.z = defaultVector.z;
	}
	if( count < 4 ){
		vector.w = defaultVector.w;
	}
}

void igdeCodecPropertyString::DecodeColor3( const char *string, decColor &color ) const{
	if( DecodeFixedFloatList( string, &color.r, 3 ) != 3 ){
		DETHROW( deeInvalidParam );
	}
}

void igdeCodecPropertyString::DecodeColor4( const char *string, decColor &color ) const{
	if( DecodeFixedFloatList( string, &color.r, 4 ) != 4 ){
		DETHROW( deeInvalidParam );
	}
}

void igdeCodecPropertyString::DecodeColor( const char *string, decColor &color,
const decColor &defaultColor ) const{
	const int count = DecodeFixedFloatList( string, &color.r, 4 );
	
	if( count < 1 ){
		color.r = defaultColor.r;
	}
	if( count < 2 ){
		color.g = defaultColor.g;
	}
	if( count < 3 ){
		color.b = defaultColor.b;
	}
	if( count < 4 ){
		color.a = defaultColor.a;
	}
}

void igdeCodecPropertyString::DecodeShapeList( const char *string, decShapeList &list ) const{
	const decStringList tokenShapeList = decString( string ).Split( vSeparators );
	decShapeCylinder *cylinder = NULL;
	decShapeCapsule *capsule = NULL;
	decShapeSphere *sphere = NULL;
	decShapeHull *hull = NULL;
	decQuaternion orientation;
	decShapeBox *box = NULL;
	decVector vector;
	int i, j;
	
	list.RemoveAll();
	
	try{
		const int tokenSphereCount = tokenShapeList.GetCount();
		
		for( i=0; i<tokenSphereCount; i++ ){
			const decStringList tokenParamList = tokenShapeList.GetAt( i ).Split( ':' );
			const decString &tokenShapeType = tokenParamList.GetAt( 0 );
			const int tokenParamCount = tokenParamList.GetCount();
			
			if( tokenShapeType == "sphere" ){
				sphere = new decShapeSphere( 1.0f );
				
				for( j=1; j<tokenParamCount; j++ ){
					const decStringList tokenValueList = tokenParamList.GetAt( j ).Split( ',' );
					const decString &tokenParamName = tokenValueList.GetAt( 0 );
					const int tokenValueCount = tokenValueList.GetCount();
					
					if( tokenParamName == "position" ){
						if( tokenValueCount != 4 ){
							DETHROW( deeInvalidParam );
						}
						
						sphere->SetPosition( decVector(
							tokenValueList.GetAt( 1 ).ToFloatValid(),
							tokenValueList.GetAt( 2 ).ToFloatValid(),
							tokenValueList.GetAt( 3 ).ToFloatValid() ) );
						
					}else if( tokenParamName == "radius" ){
						if( tokenValueCount != 2 ){
							DETHROW( deeInvalidParam );
						}
						
						sphere->SetRadius( tokenValueList.GetAt( 1 ).ToFloatValid() );
						
					}else{
						DETHROW( deeInvalidParam );
					}
				}
				
				list.Add( sphere );
				sphere = NULL;
				
			}else if( tokenShapeType == "box" ){
				box = new decShapeBox( decVector() );
				
				for( j=1; j<tokenParamCount; j++ ){
					const decStringList tokenValueList = tokenParamList.GetAt( j ).Split( ',' );
					const decString &tokenParamName = tokenValueList.GetAt( 0 );
					const int tokenValueCount = tokenValueList.GetCount();
					
					if( tokenParamName == "position" ){
						if( tokenValueCount != 4 ){
							DETHROW( deeInvalidParam );
						}
						
						box->SetPosition( decVector(
							tokenValueList.GetAt( 1 ).ToFloatValid(),
							tokenValueList.GetAt( 2 ).ToFloatValid(),
							tokenValueList.GetAt( 3 ).ToFloatValid() ) );
						
					}else if( tokenParamName == "rotation" ){
						if( tokenValueCount != 4 ){
							DETHROW( deeInvalidParam );
						}
						
						box->SetOrientation( decMatrix::CreateRotation(
							tokenValueList.GetAt( 1 ).ToFloatValid() * DEG2RAD,
							tokenValueList.GetAt( 2 ).ToFloatValid() * DEG2RAD,
							tokenValueList.GetAt( 3 ).ToFloatValid() * DEG2RAD ).ToQuaternion() );
						
					}else if( tokenParamName == "extends" ){
						if( tokenValueCount != 4 ){
							DETHROW( deeInvalidParam );
						}
						
						box->SetHalfExtends( decVector(
							tokenValueList.GetAt( 1 ).ToFloatValid(),
							tokenValueList.GetAt( 2 ).ToFloatValid(),
							tokenValueList.GetAt( 3 ).ToFloatValid() ) );
						
					}else{
						DETHROW( deeInvalidParam );
					}
				}
				
				list.Add( box );
				box = NULL;
				
			}else if( tokenShapeType == "cylinder" ){
				cylinder = new decShapeCylinder( 1.0f, 1.0f );
				
				for( j=1; j<tokenParamCount; j++ ){
					const decStringList tokenValueList = tokenParamList.GetAt( j ).Split( ',' );
					const decString &tokenParamName = tokenValueList.GetAt( 0 );
					const int tokenValueCount = tokenValueList.GetCount();
					
					if( tokenParamName == "position" ){
						if( tokenValueCount != 4 ){
							DETHROW( deeInvalidParam );
						}
						
						cylinder->SetPosition( decVector(
							tokenValueList.GetAt( 1 ).ToFloatValid(),
							tokenValueList.GetAt( 2 ).ToFloatValid(),
							tokenValueList.GetAt( 3 ).ToFloatValid() ) );
						
					}else if( tokenParamName == "rotation" ){
						if( tokenValueCount != 4 ){
							DETHROW( deeInvalidParam );
						}
						
						cylinder->SetOrientation( decMatrix::CreateRotation(
							tokenValueList.GetAt( 1 ).ToFloatValid() * DEG2RAD,
							tokenValueList.GetAt( 2 ).ToFloatValid() * DEG2RAD,
							tokenValueList.GetAt( 3 ).ToFloatValid() * DEG2RAD ).ToQuaternion() );
						
					}else if( tokenParamName == "height" ){
						if( tokenValueCount != 2 ){
							DETHROW( deeInvalidParam );
						}
						
						cylinder->SetHalfHeight( tokenValueList.GetAt( 1 ).ToFloatValid() );
						
					}else if( tokenParamName == "radius" ){
						if( tokenValueCount != 3 ){
							DETHROW( deeInvalidParam );
						}
						
						cylinder->SetTopRadius( tokenValueList.GetAt( 1 ).ToFloatValid() );
						cylinder->SetBottomRadius( tokenValueList.GetAt( 2 ).ToFloatValid() );
						
					}else if( tokenParamName == "topAxisScaling" ){
						if( tokenValueCount != 3 ){
							DETHROW( deeInvalidParam );
						}
						
						cylinder->SetTopAxisScaling( decVector2(
							tokenValueList.GetAt( 1 ).ToFloatValid(),
							tokenValueList.GetAt( 2 ).ToFloatValid() ) );
						
					}else if( tokenParamName == "bottomAxisScaling" ){
						if( tokenValueCount != 3 ){
							DETHROW( deeInvalidParam );
						}
						
						cylinder->SetBottomAxisScaling( decVector2(
							tokenValueList.GetAt( 1 ).ToFloatValid(),
							tokenValueList.GetAt( 2 ).ToFloatValid() ) );
						
					}else{
						DETHROW( deeInvalidParam );
					}
				}
				
				list.Add( cylinder );
				cylinder = NULL;
				
			}else if( tokenShapeType == "capsule" ){
				capsule = new decShapeCapsule( 1.0f, 1.0f );
				
				for( j=1; j<tokenParamCount; j++ ){
					const decStringList tokenValueList = tokenParamList.GetAt( j ).Split( ',' );
					const decString &tokenParamName = tokenValueList.GetAt( 0 );
					const int tokenValueCount = tokenValueList.GetCount();
					
					if( tokenParamName == "position" ){
						if( tokenValueCount != 4 ){
							DETHROW( deeInvalidParam );
						}
						
						capsule->SetPosition( decVector(
							tokenValueList.GetAt( 1 ).ToFloatValid(),
							tokenValueList.GetAt( 2 ).ToFloatValid(),
							tokenValueList.GetAt( 3 ).ToFloatValid() ) );
						
					}else if( tokenParamName == "rotation" ){
						if( tokenValueCount != 4 ){
							DETHROW( deeInvalidParam );
						}
						
						capsule->SetOrientation( decMatrix::CreateRotation(
							tokenValueList.GetAt( 1 ).ToFloatValid() * DEG2RAD,
							tokenValueList.GetAt( 2 ).ToFloatValid() * DEG2RAD,
							tokenValueList.GetAt( 3 ).ToFloatValid() * DEG2RAD ).ToQuaternion() );
						
					}else if( tokenParamName == "height" ){
						if( tokenValueCount != 2 ){
							DETHROW( deeInvalidParam );
						}
						
						capsule->SetHalfHeight( tokenValueList.GetAt( 1 ).ToFloatValid() );
						
					}else if( tokenParamName == "radius" ){
						if( tokenValueCount != 3 ){
							DETHROW( deeInvalidParam );
						}
						
						capsule->SetTopRadius( tokenValueList.GetAt( 1 ).ToFloatValid() );
						capsule->SetBottomRadius( tokenValueList.GetAt( 2 ).ToFloatValid() );
						
					}else if( tokenParamName == "topAxisScaling" ){
						if( tokenValueCount != 3 ){
							DETHROW( deeInvalidParam );
						}
						
						capsule->SetTopAxisScaling( decVector2(
							tokenValueList.GetAt( 1 ).ToFloatValid(),
							tokenValueList.GetAt( 2 ).ToFloatValid() ) );
						
					}else if( tokenParamName == "bottomAxisScaling" ){
						if( tokenValueCount != 3 ){
							DETHROW( deeInvalidParam );
						}
						
						capsule->SetBottomAxisScaling( decVector2(
							tokenValueList.GetAt( 1 ).ToFloatValid(),
							tokenValueList.GetAt( 2 ).ToFloatValid() ) );
						
					}else{
						DETHROW( deeInvalidParam );
					}
				}
				
				list.Add( capsule );
				capsule = NULL;
				
			}else if( tokenShapeType == "hull" ){
				hull = new decShapeHull;
				
				for( j=1; j<tokenParamCount; j++ ){
					const decStringList tokenValueList = tokenParamList.GetAt( j ).Split( ',' );
					const decString &tokenParamName = tokenValueList.GetAt( 0 );
					const int tokenValueCount = tokenValueList.GetCount();
					
					if( tokenParamName == "position" ){
						if( tokenValueCount != 4 ){
							DETHROW( deeInvalidParam );
						}
						
						hull->SetPosition( decVector(
							tokenValueList.GetAt( 1 ).ToFloatValid(),
							tokenValueList.GetAt( 2 ).ToFloatValid(),
							tokenValueList.GetAt( 3 ).ToFloatValid() ) );
						
					}else if( tokenParamName == "rotation" ){
						if( tokenValueCount != 4 ){
							DETHROW( deeInvalidParam );
						}
						
						hull->SetOrientation( decMatrix::CreateRotation(
							tokenValueList.GetAt( 1 ).ToFloatValid() * DEG2RAD,
							tokenValueList.GetAt( 2 ).ToFloatValid() * DEG2RAD,
							tokenValueList.GetAt( 3 ).ToFloatValid() * DEG2RAD ).ToQuaternion() );
						
					}else if( tokenParamName == "points" ){
						if( ( ( tokenValueCount - 1 ) % 3 ) != 0 ){
							DETHROW( deeInvalidParam );
						}
						
						const int pointCount = ( tokenValueCount - 1 ) / 3;
						hull->SetPointCount( pointCount );
						int k;
						for( k=0; k<pointCount; k++ ){
							const int index = 1 + k * 3;
							hull->SetPointAt( k, decVector(
								tokenValueList.GetAt( index ).ToFloatValid(),
								tokenValueList.GetAt( index + 1 ).ToFloatValid(),
								tokenValueList.GetAt( index + 2 ).ToFloatValid() ) );
						}
						
					}else{
						DETHROW( deeInvalidParam );
					}
				}
				
				list.Add( box );
				box = NULL;
				
			}else{
				DETHROW( deeInvalidParam );
			}
		}
		
	}catch( const deException & ){
		if( capsule ){
			delete capsule;
		}
		if( cylinder ){
			delete cylinder;
		}
		if( box ){
			delete box;
		}
		if( sphere ){
			delete sphere;
		}
		throw;
	}
}



// Encoding
/////////////

void igdeCodecPropertyString::EncodeStringList( const decStringList &list, decString &string ) const{
	const int count = list.GetCount();
	const char *separators = " \t\n\r";
	const int separatorCount = ( int )strlen( separators );
	const char *lstring;
	int i, j, subcount;
	bool quoteString;
	
	string = "";
	
	for( i=0; i<count; i++ ){
		lstring = list.GetAt( i ).GetString();
		
		if( i > 0 ){
			string.AppendCharacter( ' ' );
		}
		
		quoteString = false;
		for( j=0; j<separatorCount; j++ ){
			if( strchr( lstring, separators[ j ] ) ){
				quoteString = true;
				break;
			}
		}
		
		if( quoteString ){
			string.AppendCharacter( '"' );
		}
		
		subcount = ( int )strlen( lstring );
		for( j=0; j<subcount; j++ ){
			if( lstring[ j ] == '"' ){
				string.Append( "\\\"" );
				
			}else{
				string.AppendCharacter( lstring[ j ] );
			}
		}
		
		if( quoteString ){
			string.AppendCharacter( '"' );
		}
	}
}

void igdeCodecPropertyString::EncodeVector( const decVector &vector, decString &string ) const{
	string.Format( "%g %g %g", vector.x, vector.y, vector.z );
}

void igdeCodecPropertyString::EncodeDVector( const decDVector &vector, decString &string ) const{
	string.Format( "%g %g %g", vector.x, vector.y, vector.z );
}

void igdeCodecPropertyString::EncodeVector2( const decVector2 &vector, decString &string ) const{
	string.Format( "%g %g", vector.x, vector.y );
}

void igdeCodecPropertyString::EncodeVector4( const decVector4 &vector, decString &string ) const{
	string.Format( "%g %g %g %g", vector.x, vector.y, vector.z, vector.w );
}

void igdeCodecPropertyString::EncodeDVector4( const decDVector4 &vector, decString &string ) const{
	string.Format( "%g %g %g %g", vector.x, vector.y, vector.z, vector.w );
}

void igdeCodecPropertyString::EncodeColor3( const decColor &color, decString &string ) const{
	string.Format( "%g %g %g", color.r, color.g, color.b );
}

void igdeCodecPropertyString::EncodeColor4( const decColor &color, decString &string ) const{
	string.Format( "%g %g %g %g", color.r, color.g, color.b, color.a );
}

void igdeCodecPropertyString::EncodeShapeList( const decShapeList &list, decString &string ) const{
	igdePropertyStringEncodeShapeList visitor( &string );
	const int count = list.GetCount();
	int i;
	
	string = "";
	
	for( i=0; i<count; i++ ){
		list.GetAt( i )->Visit( visitor );
	}
}
