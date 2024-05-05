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
#include <stdint.h>

#include "deClassNavigatorPath.h"
#include "../file/deClassFileReader.h"
#include "../file/deClassFileWriter.h"
#include "../debug/deClassDebugDrawer.h"
#include "../math/deClassDVector.h"
#include "../math/deClassDMatrix.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/navigation/navigator/deNavigatorPath.h>



struct sNavPathNatDat{
	deNavigatorPath *path;
};



// Constructors, Destructors
//////////////////////////////

// public func new()
deClassNavigatorPath::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsNavPath,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassNavigatorPath::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sNavPathNatDat &nd = *( ( sNavPathNatDat* )p_GetNativeData( myself ) );
	
	// clear ( important )
	nd.path = NULL;
	
	// create path
	nd.path = new deNavigatorPath;
}

// public func new( NavigatorPath path )
deClassNavigatorPath::nfNewCopy::nfNewCopy( const sInitData &init ) : dsFunction( init.clsNavPath,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsNavPath ); // path
}
void deClassNavigatorPath::nfNewCopy::RunFunction( dsRunTime *rt, dsValue *myself ){
	sNavPathNatDat &nd = *( ( sNavPathNatDat* )p_GetNativeData( myself ) );
	const deClassNavigatorPath &clsNavPath = *( ( deClassNavigatorPath* )GetOwnerClass() );
	
	// clear ( important )
	nd.path = NULL;
	
	// create path
	nd.path = new deNavigatorPath( clsNavPath.GetNavigatorPath( rt->GetValue( 0 )->GetRealObject() ) );
}

// public func destructor()
deClassNavigatorPath::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsNavPath,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassNavigatorPath::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sNavPathNatDat &nd = *( ( sNavPathNatDat* )p_GetNativeData( myself ) );
	
	if( nd.path ){
		delete nd.path;
		nd.path = NULL;
	}
}



// Management
///////////////

// public func int getCount()
deClassNavigatorPath::nfGetCount::nfGetCount( const sInitData &init ) : dsFunction( init.clsNavPath,
"getCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}
void deClassNavigatorPath::nfGetCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deNavigatorPath &path = *( ( ( const sNavPathNatDat * )p_GetNativeData( myself ) )->path );
	
	rt->PushInt( path.GetCount() );
}

// public func DVector getAt( int index )
deClassNavigatorPath::nfGetAt::nfGetAt( const sInitData &init ) : dsFunction( init.clsNavPath,
"getAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVector ){
	p_AddParameter( init.clsInteger ); // index
}
void deClassNavigatorPath::nfGetAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deNavigatorPath &path = *( ( ( const sNavPathNatDat * )p_GetNativeData( myself ) )->path );
	const deScriptingDragonScript &ds = ( ( deClassNavigatorPath* )GetOwnerClass() )->GetDS();
	const int index = rt->GetValue( 0 )->GetInt();
	
	ds.GetClassDVector()->PushDVector( rt, path.GetAt( index ) );
}

// public func void setAt( int index, DVector point )
deClassNavigatorPath::nfSetAt::nfSetAt( const sInitData &init ) :
dsFunction( init.clsNavPath, "setAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // index
	p_AddParameter( init.clsDVector ); // point
}
void deClassNavigatorPath::nfSetAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	deNavigatorPath &path = *( ( ( sNavPathNatDat* )p_GetNativeData( myself ) )->path );
	const deScriptingDragonScript &ds = ( ( deClassNavigatorPath* )GetOwnerClass() )->GetDS();
	const int index = rt->GetValue( 0 )->GetInt();
	const decDVector &point = ds.GetClassDVector()->GetDVector( rt->GetValue( 0 )->GetRealObject() );
	
	path.SetAt( index, point );
}

// public func void add( DVector point )
deClassNavigatorPath::nfAdd::nfAdd( const sInitData &init ) : dsFunction( init.clsNavPath,
"add", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsDVector ); // point
}
void deClassNavigatorPath::nfAdd::RunFunction( dsRunTime *rt, dsValue *myself ){
	deNavigatorPath &path = *( ( ( const sNavPathNatDat * )p_GetNativeData( myself ) )->path );
	const deScriptingDragonScript &ds = ( ( deClassNavigatorPath* )GetOwnerClass() )->GetDS();
	
	const decDVector &point = ds.GetClassDVector()->GetDVector( rt->GetValue( 0 )->GetRealObject() );
	
	path.Add( point );
}

// public func void addPath( NavigatorPath path )
deClassNavigatorPath::nfAddPath::nfAddPath( const sInitData &init ) : dsFunction( init.clsNavPath,
"addPath", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsNavPath ); // path
}
void deClassNavigatorPath::nfAddPath::RunFunction( dsRunTime *rt, dsValue *myself ){
	deNavigatorPath &path = *( ( ( const sNavPathNatDat * )p_GetNativeData( myself ) )->path );
	deClassNavigatorPath &clsNavPath = *( ( deClassNavigatorPath* )GetOwnerClass() );
	
	path.AddPath( clsNavPath.GetNavigatorPath( rt->GetValue( 0 )->GetRealObject() ) );
}

// public func void removeFrom( int index )
deClassNavigatorPath::nfRemoveFrom::nfRemoveFrom( const sInitData &init ) : dsFunction( init.clsNavPath,
"removeFrom", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // index
}
void deClassNavigatorPath::nfRemoveFrom::RunFunction( dsRunTime *rt, dsValue *myself ){
	deNavigatorPath &path = *( ( ( const sNavPathNatDat * )p_GetNativeData( myself ) )->path );
	
	path.RemoveFrom( rt->GetValue( 0 )->GetInt() );
}

// public func void removeAll()
deClassNavigatorPath::nfRemoveAll::nfRemoveAll( const sInitData &init ) : dsFunction( init.clsNavPath,
"removeAll", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassNavigatorPath::nfRemoveAll::RunFunction( dsRunTime *rt, dsValue *myself ){
	deNavigatorPath &path = *( ( ( const sNavPathNatDat * )p_GetNativeData( myself ) )->path );
	
	path.RemoveAll();
}

// public func void transform( DMatrix matrix )
deClassNavigatorPath::nfTransform::nfTransform( const sInitData &init ) : dsFunction( init.clsNavPath,
"transform", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsDMatrix ); // matrix
}
void deClassNavigatorPath::nfTransform::RunFunction( dsRunTime *rt, dsValue *myself ){
	deNavigatorPath &path = *( ( ( const sNavPathNatDat * )p_GetNativeData( myself ) )->path );
	const deScriptingDragonScript &ds = ( ( deClassNavigatorPath* )GetOwnerClass() )->GetDS();
	
	const decDMatrix &matrix = ds.GetClassDMatrix()->GetDMatrix( rt->GetValue( 0 )->GetRealObject() );
	const int count = path.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		path.SetAt( i, matrix * path.GetAt( i ) );
	}
}

// public func NavigatorPath transformed( DMatrix matrix )
deClassNavigatorPath::nfTransformed::nfTransformed( const sInitData &init ) : dsFunction( init.clsNavPath,
"transformed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsNavPath ){
	p_AddParameter( init.clsDMatrix ); // matrix
}
void deClassNavigatorPath::nfTransformed::RunFunction( dsRunTime *rt, dsValue *myself ){
	deNavigatorPath &path = *( ( ( const sNavPathNatDat * )p_GetNativeData( myself ) )->path );
	deClassNavigatorPath &clsNavPath = *( ( deClassNavigatorPath* )GetOwnerClass() );
	
	const decDMatrix &matrix = clsNavPath.GetDS().GetClassDMatrix()->GetDMatrix( rt->GetValue( 0 )->GetRealObject() );
	const int count = path.GetCount();
	deNavigatorPath transformedPath;
	int i;
	for( i=0; i<count; i++ ){
		transformedPath.Add( matrix * path.GetAt( i ) );
	}
	
	clsNavPath.PushNavigatorPath( rt, transformedPath );
}

// public func float getLength()
deClassNavigatorPath::nfGetLength::nfGetLength( const sInitData &init ) : dsFunction( init.clsNavPath,
"getLength", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}
void deClassNavigatorPath::nfGetLength::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deNavigatorPath &path = *( ( ( const sNavPathNatDat * )p_GetNativeData( myself ) )->path );
	const int count = path.GetCount();
	double length = 0.0f;
	
	if( count > 0 ){
		decDVector last( path.GetAt( 0 ) );
		int i;
		
		for( i=1; i<count; i++ ){
			const decDVector next( path.GetAt( i ) );
			length += ( next - last ).Length();
			last = next;
		}
	}
	
	rt->PushFloat( ( float )length );
}

// public func float getLengthTo( int endIndex )
deClassNavigatorPath::nfGetLengthTo::nfGetLengthTo( const sInitData &init ) : dsFunction( init.clsNavPath,
"getLengthTo", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
	p_AddParameter( init.clsInteger ); // endIndex
}
void deClassNavigatorPath::nfGetLengthTo::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deNavigatorPath &path = *( ( ( const sNavPathNatDat * )p_GetNativeData( myself ) )->path );
	const int endIndex = rt->GetValue( 0 )->GetInt();
	const int count = path.GetCount();
	if( endIndex < 0 || endIndex >= count ){
		DSTHROW( dueOutOfBoundary );
	}
	
	decDVector last( path.GetAt( 0 ) );
	double length = 0.0f;
	int i;
	
	for( i=1; i<=endIndex; i++ ){
		const decDVector next( path.GetAt( i ) );
		length += ( next - last ).Length();
		last = next;
	}
	
	rt->PushFloat( ( float )length );
}

// public func float getLengthFrom( int startIndex )
deClassNavigatorPath::nfGetLengthFrom::nfGetLengthFrom( const sInitData &init ) : dsFunction( init.clsNavPath,
"getLengthFrom", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
	p_AddParameter( init.clsInteger ); // startIndex
}
void deClassNavigatorPath::nfGetLengthFrom::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deNavigatorPath &path = *( ( ( const sNavPathNatDat * )p_GetNativeData( myself ) )->path );
	const int startIndex = rt->GetValue( 0 )->GetInt();
	const int count = path.GetCount();
	if( startIndex < 0 || startIndex >= count ){
		DSTHROW( dueOutOfBoundary );
	}
	
	decDVector last( path.GetAt( startIndex ) );
	double length = 0.0f;
	int i;
	
	for( i=startIndex+1; i<count; i++ ){
		const decDVector next( path.GetAt( i ) );
		length += ( next - last ).Length();
		last = next;
	}
	
	rt->PushFloat( ( float )length );
}

// public func float getLengthBetween( int startIndex, int endIndex )
deClassNavigatorPath::nfGetLengthBetween::nfGetLengthBetween( const sInitData &init ) : dsFunction( init.clsNavPath,
"getLengthBetween", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
	p_AddParameter( init.clsInteger ); // startIndex
	p_AddParameter( init.clsInteger ); // endIndex
}
void deClassNavigatorPath::nfGetLengthBetween::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deNavigatorPath &path = *( ( ( const sNavPathNatDat * )p_GetNativeData( myself ) )->path );
	const int startIndex = rt->GetValue( 0 )->GetInt();
	const int endIndex = rt->GetValue( 1 )->GetInt();
	const int count = path.GetCount();
	if( startIndex < 0 || endIndex < startIndex  || endIndex >= count ){
		DSTHROW( dueOutOfBoundary );
	}
	
	decDVector last( path.GetAt( startIndex ) );
	double length = 0.0f;
	int i;
	
	for( i=1; i<=endIndex; i++ ){
		const decDVector next( path.GetAt( i ) );
		length += ( next - last ).Length();
		last = next;
	}
	
	rt->PushFloat( ( float )length );
}



// File Handling
//////////////////

// static public func NavigatorPath readFromFile( FileReader reader )
deClassNavigatorPath::nfReadFromFile::nfReadFromFile( const sInitData &init ) : dsFunction( init.clsNavPath,
"readFromFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsNavPath ){
	p_AddParameter( init.clsFileReader ); // reader
}
void deClassNavigatorPath::nfReadFromFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassNavigatorPath &clsNavPath = *( ( deClassNavigatorPath* )GetOwnerClass() );
	decBaseFileReader * const reader = clsNavPath.GetDS().GetClassFileReader()->
		GetFileReader( rt->GetValue( 0 )->GetRealObject() );
	
	if( ! reader ){
		DSTHROW( dueNullPointer );
	}
	
	const int version = reader->ReadByte();
	deNavigatorPath path;
	decDVector point;
	int i, count;
	
	switch( version ){
	case 0:
		count = reader->ReadUShort();
		for( i=0; i<count; i++ ){
			path.Add( reader->ReadDVector() );
		}
		break;
		
	default:
		DSTHROW( dueInvalidParam );
	}
	
	clsNavPath.PushNavigatorPath( rt, path );
}

// public func void writeToFile( FileWriter writer )
deClassNavigatorPath::nfWriteToFile::nfWriteToFile( const sInitData &init ) : dsFunction( init.clsNavPath,
"writeToFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFileWriter ); // writer
}
void deClassNavigatorPath::nfWriteToFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deNavigatorPath &path = *( ( ( const sNavPathNatDat * )p_GetNativeData( myself ) )->path );
	const deScriptingDragonScript &ds = ( ( deClassNavigatorPath* )GetOwnerClass() )->GetDS();
	decBaseFileWriter * const writer = ds.GetClassFileWriter()->GetFileWriter( rt->GetValue( 0 )->GetRealObject() );
	
	if( ! writer ){
		DSTHROW( dueNullPointer );
	}
	
	writer->WriteByte( 0 ); // version 0
	
	const int count = path.GetCount();
	writer->WriteUShort( count );
	
	int i;
	for( i=0; i<count; i++ ){
		writer->WriteDVector( path.GetAt( i ) );
	}
}



// public func void updateDebugDrawer( DebugDrawer debugDrawer, int shape, float scale )
deClassNavigatorPath::nfUpdateDebugDrawer::nfUpdateDebugDrawer( const sInitData &init ) : dsFunction( init.clsNavPath,
"updateDebugDrawer", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsDebugDrawer ); // debugDrawer
	p_AddParameter( init.clsInteger ); // shape
	p_AddParameter( init.clsFloat ); // scale
}
void deClassNavigatorPath::nfUpdateDebugDrawer::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deNavigatorPath &path = *( ( ( const sNavPathNatDat * )p_GetNativeData( myself ) )->path );
	const deScriptingDragonScript &ds = ( ( deClassNavigatorPath* )GetOwnerClass() )->GetDS();
	
	deDebugDrawer * const debugDrawer = ds.GetClassDebugDrawer()->GetDebugDrawer( rt->GetValue( 0 )->GetRealObject() );
	if( ! debugDrawer ){
		DSTHROW( dueNullPointer );
	}
	
	decShapeList &shapes = debugDrawer->GetShapeAt( rt->GetValue( 1 )->GetInt() )->GetShapeList();
	const float scale = rt->GetValue( 2 )->GetFloat();
	const decDMatrix matrix( decDMatrix::CreateWorld(
		debugDrawer->GetPosition(), debugDrawer->GetOrientation() ).QuickInvert() );
	
	shapes.RemoveAll();
	
	const int pointCount = path.GetCount();
	if( pointCount > 0 ){
		const float boxSize = 0.02f * scale;
		const decVector upAlt( 0.0f, 0.0f, 1.0f );
		const decVector up( 0.0f, 1.0f, 0.0f );
		decShape *shape = NULL;
		int i;
		
		try{
			// start of path
			decVector lastPoint( matrix * path.GetAt( 0 ) );
			
			shape = new decShapeSphere( 0.05f * scale, lastPoint );
			shapes.Add( shape );
			shape = NULL;
			
			// end of path
			if( pointCount > 1 ){
				shape = new decShapeSphere( 0.05f * scale, ( matrix * path.GetAt( pointCount - 1 ) ).ToVector() );
				shapes.Add( shape );
				shape = NULL;
			}
			
			// path segments
			for( i=0; i<pointCount; i++ ){
				const decVector nextPoint( matrix * path.GetAt( i ) );
				const decVector direction( nextPoint - lastPoint );
				
				const decVector halfExtends( boxSize, boxSize, direction.Length() * 0.5f );
				if( halfExtends.z < 0.001f ){
					continue;
				}
				
				const decVector view( direction.Normalized() );
				decQuaternion orientation;
				
				if( fabsf( view.y ) < 0.99f ){
					orientation = decMatrix::CreateWorld( lastPoint, view, up ).ToQuaternion();
					
				}else{
					orientation = decMatrix::CreateWorld( lastPoint, view, upAlt ).ToQuaternion();
				}
				
				shape = new decShapeBox( halfExtends, ( lastPoint + nextPoint ) * 0.5f, orientation );
				shapes.Add( shape );
				shape = NULL;
			}
			
		}catch( ... ){
			if( shape ){
				delete shape;
			}
			throw;
		}
	}
	
	debugDrawer->NotifyShapeContentChanged();
}



// public func int hashCode()
deClassNavigatorPath::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsNavPath, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}

void deClassNavigatorPath::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deNavigatorPath * const path = ( ( const sNavPathNatDat * )p_GetNativeData( myself ) )->path;
	
	rt->PushInt( ( int )( intptr_t )path );
}

// public func bool equals( Object object )
deClassNavigatorPath::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsNavPath, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObject ); // object
}
void deClassNavigatorPath::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deNavigatorPath * const path = ( ( const sNavPathNatDat * )p_GetNativeData( myself ) )->path;
	deClassNavigatorPath * const clsNavPath = ( deClassNavigatorPath* )GetOwnerClass();
	dsValue * const object = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( object, clsNavPath ) ){
		rt->PushBool( false );
		
	}else{
		const deNavigatorPath * const otherPath = ( ( const sNavPathNatDat * )p_GetNativeData( object ) )->path;
		rt->PushBool( path == otherPath );
	}
}



// Class deClassNavigatorPath
///////////////////////////////

// Constructor
////////////////

deClassNavigatorPath::deClassNavigatorPath( deScriptingDragonScript &ds ) :
dsClass( "NavigatorPath", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ),
pDS( ds )
{
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sNavPathNatDat ) );
}

deClassNavigatorPath::~deClassNavigatorPath(){
}



// Management
///////////////

void deClassNavigatorPath::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	init.clsNavPath = this;
	
	init.clsBool = engine->GetClassBool();
	init.clsFloat = engine->GetClassFloat();
	init.clsInteger = engine->GetClassInt();
	init.clsObject = engine->GetClassObject();
	init.clsString = engine->GetClassString();
	init.clsVoid = engine->GetClassVoid();
	
	init.clsDVector = pDS.GetClassDVector();
	init.clsFileReader = pDS.GetClassFileReader();
	init.clsFileWriter = pDS.GetClassFileWriter();
	init.clsDebugDrawer = pDS.GetClassDebugDrawer();
	init.clsDMatrix = pDS.GetClassDMatrix();
	
	AddFunction( new nfNew( init ) );
	AddFunction( new nfNewCopy( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetCount( init ) );
	AddFunction( new nfGetAt( init ) );
	AddFunction( new nfSetAt( init ) );
	AddFunction( new nfAdd( init ) );
	AddFunction( new nfRemoveFrom( init ) );
	AddFunction( new nfRemoveAll( init ) );
	AddFunction( new nfTransform( init ) );
	AddFunction( new nfTransformed( init ) );
	AddFunction( new nfGetLength( init ) );
	AddFunction( new nfGetLengthTo( init ) );
	AddFunction( new nfGetLengthFrom( init ) );
	AddFunction( new nfGetLengthBetween( init ) );
	
	AddFunction( new nfReadFromFile( init ) );
	AddFunction( new nfWriteToFile( init ) );
	
	AddFunction( new nfUpdateDebugDrawer( init ) );
	
	AddFunction( new nfHashCode( init ) );
	AddFunction( new nfEquals( init ) );
	
	CalcMemberOffsets();
}

deNavigatorPath &deClassNavigatorPath::GetNavigatorPath( dsRealObject *myself ) const {
	if( ! myself ){
		DSTHROW( dueNullPointer );
	}
	
	return *( ( ( const sNavPathNatDat * )p_GetNativeData( myself->GetBuffer() ) )->path );
}

void deClassNavigatorPath::PushNavigatorPath( dsRunTime *rt, const deNavigatorPath &path ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	rt->CreateObjectNakedOnStack( this );
	sNavPathNatDat &nd = *( ( sNavPathNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
	nd.path = NULL;
	
	try{
		nd.path = new deNavigatorPath( path );
		
	}catch( ... ){
		rt->RemoveValues( 1 ); // remove pushed object
		throw;
	}
}
