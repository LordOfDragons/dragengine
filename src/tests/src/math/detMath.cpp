// includes
#include <stdio.h>
#include <stdlib.h>
#include "detMath.h"
#include "dragengine/common/math/decMath.h"
#include "dragengine/common/exceptions.h"

	

// class detMath
/////////////////////////////

// constructors, destructor
detMath::detMath(){ }
detMath::~detMath(){ }

// testing
void detMath::Prepare(){ }
void detMath::Run(){
	decVector pos, rot, scale, view, up;
	decMatrix m1, m2, m3, m4;
	int i;
	// some test values
	pos = decVector( 1.0, 3.0, -1.5 );
	rot = decVector( DEG2RAD * 20, DEG2RAD * -10, DEG2RAD * 80 );
	view = decMatrix::CreateRotation( rot ).TransformView();
	up = decMatrix::CreateRotation( rot ).TransformUp();
	scale = decVector( 2.0, 2.5, 0.5 );
	// test that equal works
	SetSubTestNum(0);
	m1 = m2 = decMatrix::CreateRotation( rot );
	ASSERT_TRUE( m2.IsEqualTo( m1 ) );
	m2 = decMatrix::CreateScale( scale );
	ASSERT_FALSE( m2.IsEqualTo( m1 ) );
	// test RT matrix
	SetSubTestNum(1);
	m1 = decMatrix::CreateRotation( rot ) * decMatrix::CreateTranslation( pos );
	m2 = decMatrix::CreateRT( rot, pos );
	ASSERT_TRUE( m2.IsEqualTo( m1 ) );
	// test SRT matrix
	SetSubTestNum(2);
	m1 = decMatrix::CreateScale( scale ) * decMatrix::CreateRotation( rot ) * decMatrix::CreateTranslation( pos );
	m2 = decMatrix::CreateSRT( scale, rot, pos );
	ASSERT_TRUE( m2.IsEqualTo( m1 ) );
	// test SVUT matrix
	SetSubTestNum(3);
	m1 = decMatrix::CreateScale( scale ) * decMatrix::CreateWorld( pos, view, up );
	m2 = decMatrix::CreateSVUT( scale, view, up, pos );
	ASSERT_TRUE( m2.IsEqualTo( m1 ) );
	// test euler
	SetSubTestNum(4);
	for( i = -89; i < 90; i += 1 ){
		rot = decVector( DEG2RAD * i, DEG2RAD * 0, DEG2RAD * 0 );
		m1 = decMatrix::CreateRotation( rot );
		m2 = decMatrix::CreateRotation( m1.GetEulerAngles() );
		ASSERT_TRUE( m2.IsEqualTo( m1 ) );

		rot = decVector( DEG2RAD * 0, DEG2RAD * 0, DEG2RAD * i );
		m1 = decMatrix::CreateRotation( rot );
		m2 = decMatrix::CreateRotation( m1.GetEulerAngles() );
		ASSERT_TRUE( m2.IsEqualTo( m1 ) );

		rot = decVector( DEG2RAD * 0, DEG2RAD * i, DEG2RAD * 0 );
		m1 = decMatrix::CreateRotation( rot );
		m2 = decMatrix::CreateRotation( m1.GetEulerAngles() );
		ASSERT_TRUE( m2.IsEqualTo( m1 ) );
	}
	rot = decVector( DEG2RAD * 20, DEG2RAD * -10, DEG2RAD * 80 );
	m1 = decMatrix::CreateRotation( rot );
	m2 = decMatrix::CreateRotation( m1.GetEulerAngles() );
	ASSERT_TRUE( m2.IsEqualTo( m1 ) );
	rot = decVector( DEG2RAD * 90, DEG2RAD * 0, DEG2RAD * 30 );
	m1 = decMatrix::CreateRotation( rot );
	m2 = decMatrix::CreateRotation( m1.GetEulerAngles() );
	ASSERT_TRUE( m2.IsEqualTo( m1 ) );
	rot = decVector( DEG2RAD * -90, DEG2RAD * 0, DEG2RAD * 30 );
	m1 = decMatrix::CreateRotation( rot );
	m2 = decMatrix::CreateRotation( m1.GetEulerAngles() );
	ASSERT_TRUE( m2.IsEqualTo( m1 ) );
	
	// quick stuff
	m1.SetSRT( scale, rot, pos );
	m2.SetSRT( scale * 0.5f, rot * 0.5f, pos * 0.5f );
	
	m3 = m1;
	m3.QuickAddTo( m2 );
	m4 = m1 + m2;
	m4.a44 = 1.0f; // because operator+ also adds a44 but QuickAddTo does not
	ASSERT_TRUE( m3.IsEqualTo( m4 ) );
	
	m4 = m1 * 0.8f;
	m4.a44 = 1.0f; // because operator* also multiplies a44 but QuickAddTo does not
	ASSERT_TRUE( m1.QuickMultiply( 0.8f ).IsEqualTo( m4 ) );
	
	ASSERT_FEQUAL( m1.QuickDeterminant(), m1.Determinant() );
	ASSERT_TRUE( m1.QuickInvert().IsEqualTo( m1.Invert() ) );
	ASSERT_TRUE( m1.QuickMultiply( m2 ).IsEqualTo( m1 * m2 ) );
	
	// temp test
	decVector trans( 1.5f, -3.7, 9.2f );
	decQuaternion quat = decMatrix::CreateRotation( DEG2RAD * 10.0, DEG2RAD * -25.0, DEG2RAD * 60.0 ).ToQuaternion();
	decMatrix mat1, mat2;
	decVector tps( 3.0f, -3.5f, 7.5f );
	mat1 = decMatrix::CreateTranslation( -trans ) * decMatrix::CreateFromQuaternion( quat )
		* decMatrix::CreateTranslation( trans );
	mat2.SetFromQuaternion( quat, trans );
	ASSERT_TRUE( ( mat1 * tps ).IsEqualTo( mat2 * tps ) );
	
	TestQuaternion();
}
void detMath::CleanUp(){ }
const char *detMath::GetTestName(){ return "Math"; }



void detMath::TestQuaternion(){
	SetSubTestNum( 5 );
	
	// initial values
	decQuaternion init0;
	ASSERT_FEQUAL( init0.x, 0.0f );
	ASSERT_FEQUAL( init0.y, 0.0f );
	ASSERT_FEQUAL( init0.z, 0.0f );
	ASSERT_FEQUAL( init0.w, 1.0f );
	
	decQuaternion init1( 2.0f, 5.0f, -3.0f, 10.0f );
	ASSERT_FEQUAL( init1.x, 2.0f );
	ASSERT_FEQUAL( init1.y, 5.0f );
	ASSERT_FEQUAL( init1.z, -3.0f );
	ASSERT_FEQUAL( init1.w, 10.0f );
	
	decQuaternion init2( init1 );
	ASSERT_FEQUAL( init2.x, 2.0f );
	ASSERT_FEQUAL( init2.y, 5.0f );
	ASSERT_FEQUAL( init2.z, -3.0f );
	ASSERT_FEQUAL( init2.w, 10.0f );
	
	// length
	ASSERT_FEQUAL( init0.Length(), 1.0f );
	ASSERT_FEQUAL( init1.Length(), sqrtf( 138.0f ) );
	
	// normalize
	decQuaternion norm1( init1 );
	norm1.Normalize();
	
	ASSERT_FEQUAL( norm1.x, init1.x / init1.Length() );
	ASSERT_FEQUAL( norm1.y, init1.y / init1.Length() );
	ASSERT_FEQUAL( norm1.z, init1.z / init1.Length() );
	ASSERT_FEQUAL( norm1.w, init1.w / init1.Length() );
	
	decQuaternion conj1( init1.Conjugate() );
	
	ASSERT_FEQUAL( conj1.x, -init1.x );
	ASSERT_FEQUAL( conj1.y, -init1.y );
	ASSERT_FEQUAL( conj1.z, -init1.z );
	ASSERT_FEQUAL( conj1.w, init1.w );
	
	// equality
	ASSERT_TRUE( init1.IsEqualTo( init1 ) );
	ASSERT_TRUE( init1.IsEqualTo( init2 ) );
	ASSERT_FALSE( init1.IsEqualTo( norm1 ) );
	ASSERT_FALSE( init1.IsEqualTo( conj1 ) );
	
	// set zero
	decQuaternion zero1( init1 );
	
	ASSERT_FEQUAL( zero1.x, init1.x );
	ASSERT_FEQUAL( zero1.y, init1.y );
	ASSERT_FEQUAL( zero1.z, init1.z );
	ASSERT_FEQUAL( zero1.w, init1.w );
	
	zero1.SetZero();
	ASSERT_FEQUAL( zero1.x, 0.0f );
	ASSERT_FEQUAL( zero1.y, 0.0f );
	ASSERT_FEQUAL( zero1.z, 0.0f );
	ASSERT_FEQUAL( zero1.w, 1.0f );
	
	// set values
	decQuaternion set1( init1 );
	
	ASSERT_FEQUAL( set1.x, init1.x );
	ASSERT_FEQUAL( set1.y, init1.y );
	ASSERT_FEQUAL( set1.z, init1.z );
	ASSERT_FEQUAL( set1.w, init1.w );
	
	set1.Set( 14.0f, -12.0f, 80.0f, 40.0f );
	ASSERT_FEQUAL( set1.x, 14.0f );
	ASSERT_FEQUAL( set1.y, -12.0f );
	ASSERT_FEQUAL( set1.z, 80.0f );
	ASSERT_FEQUAL( set1.w, 40.0f );
	
	set1.Set( init1 );
	ASSERT_FEQUAL( set1.x, init1.x );
	ASSERT_FEQUAL( set1.y, init1.y );
	ASSERT_FEQUAL( set1.z, init1.z );
	ASSERT_FEQUAL( set1.w, init1.w );
	
	// operator -
	decQuaternion oper1, oper2, oper3;
	
	oper1.Set( 1.0f, 2.0f, 3.0f, 4.0f );
	oper3 = -oper1;
	ASSERT_FEQUAL( oper1.x, 1.0f );
	ASSERT_FEQUAL( oper1.y, 2.0f );
	ASSERT_FEQUAL( oper1.z, 3.0f );
	ASSERT_FEQUAL( oper1.w, 4.0f );
	ASSERT_FEQUAL( oper3.x, -1.0f );
	ASSERT_FEQUAL( oper3.y, -2.0f );
	ASSERT_FEQUAL( oper3.z, -3.0f );
	ASSERT_FEQUAL( oper3.w, -4.0f );
	
	// operator =
	oper1.Set( 1.0f, 2.0f, 3.0f, 4.0f );
	oper3.Set( 10.0f, 20.0f, 30.0f, 40.0f );
	oper3 = oper1;
	ASSERT_FEQUAL( oper1.x, 1.0f );
	ASSERT_FEQUAL( oper1.y, 2.0f );
	ASSERT_FEQUAL( oper1.z, 3.0f );
	ASSERT_FEQUAL( oper1.w, 4.0f );
	ASSERT_FEQUAL( oper3.x, 1.0f );
	ASSERT_FEQUAL( oper3.y, 2.0f );
	ASSERT_FEQUAL( oper3.z, 3.0f );
	ASSERT_FEQUAL( oper3.w, 4.0f );
	
	// operator +=
	oper1.Set( 1.0f, 2.0f, 3.0f, 4.0f );
	oper3.Set( 10.0f, 20.0f, 30.0f, 40.0f );
	oper3 += oper1;
	ASSERT_FEQUAL( oper1.x, 1.0f );
	ASSERT_FEQUAL( oper1.y, 2.0f );
	ASSERT_FEQUAL( oper1.z, 3.0f );
	ASSERT_FEQUAL( oper1.w, 4.0f );
	ASSERT_FEQUAL( oper3.x, 11.0f );
	ASSERT_FEQUAL( oper3.y, 22.0f );
	ASSERT_FEQUAL( oper3.z, 33.0f );
	ASSERT_FEQUAL( oper3.w, 44.0f );
	
	// operator -=
	oper1.Set( 1.0f, 2.0f, 3.0f, 4.0f );
	oper3.Set( 10.0f, 20.0f, 30.0f, 40.0f );
	oper3 -= oper1;
	ASSERT_FEQUAL( oper1.x, 1.0f );
	ASSERT_FEQUAL( oper1.y, 2.0f );
	ASSERT_FEQUAL( oper1.z, 3.0f );
	ASSERT_FEQUAL( oper1.w, 4.0f );
	ASSERT_FEQUAL( oper3.x, 9.0f );
	ASSERT_FEQUAL( oper3.y, 18.0f );
	ASSERT_FEQUAL( oper3.z, 27.0f );
	ASSERT_FEQUAL( oper3.w, 36.0f );
	
	// operator *=
	oper3.Set( 1.0f, 2.0f, 3.0f, 4.0f );
	oper3 *= 10.0f;
	ASSERT_FEQUAL( oper3.x, 10.0f );
	ASSERT_FEQUAL( oper3.y, 20.0f );
	ASSERT_FEQUAL( oper3.z, 30.0f );
	ASSERT_FEQUAL( oper3.w, 40.0f );
	
	// operator /=
	oper3.Set( 1.0f, 2.0f, 3.0f, 4.0f );
	oper3 /= 5.0f;
	ASSERT_FEQUAL( oper3.x, 0.2f );
	ASSERT_FEQUAL( oper3.y, 0.4f );
	ASSERT_FEQUAL( oper3.z, 0.6f );
	ASSERT_FEQUAL( oper3.w, 0.8f );
	
	// operator +=
	oper1.Set( 1.0f, 2.0f, 3.0f, 4.0f );
	oper2.Set( 10.0f, 20.0f, 30.0f, 40.0f );
	oper3 = oper1 + oper2;
	ASSERT_FEQUAL( oper1.x, 1.0f );
	ASSERT_FEQUAL( oper1.y, 2.0f );
	ASSERT_FEQUAL( oper1.z, 3.0f );
	ASSERT_FEQUAL( oper1.w, 4.0f );
	ASSERT_FEQUAL( oper2.x, 10.0f );
	ASSERT_FEQUAL( oper2.y, 20.0f );
	ASSERT_FEQUAL( oper2.z, 30.0f );
	ASSERT_FEQUAL( oper2.w, 40.0f );
	ASSERT_FEQUAL( oper3.x, 11.0f );
	ASSERT_FEQUAL( oper3.y, 22.0f );
	ASSERT_FEQUAL( oper3.z, 33.0f );
	ASSERT_FEQUAL( oper3.w, 44.0f );
	
	// operator -
	oper1.Set( 1.0f, 2.0f, 3.0f, 4.0f );
	oper2.Set( 10.0f, 20.0f, 30.0f, 40.0f );
	oper3 = oper2 - oper1;
	ASSERT_FEQUAL( oper1.x, 1.0f );
	ASSERT_FEQUAL( oper1.y, 2.0f );
	ASSERT_FEQUAL( oper1.z, 3.0f );
	ASSERT_FEQUAL( oper1.w, 4.0f );
	ASSERT_FEQUAL( oper2.x, 10.0f );
	ASSERT_FEQUAL( oper2.y, 20.0f );
	ASSERT_FEQUAL( oper2.z, 30.0f );
	ASSERT_FEQUAL( oper2.w, 40.0f );
	ASSERT_FEQUAL( oper3.x, 9.0f );
	ASSERT_FEQUAL( oper3.y, 18.0f );
	ASSERT_FEQUAL( oper3.z, 27.0f );
	ASSERT_FEQUAL( oper3.w, 36.0f );
	
	// operator *
	oper1.Set( 1.0f, 2.0f, 3.0f, 4.0f );
	oper3 = oper1 * 10.0f;
	ASSERT_FEQUAL( oper1.x, 1.0f );
	ASSERT_FEQUAL( oper1.y, 2.0f );
	ASSERT_FEQUAL( oper1.z, 3.0f );
	ASSERT_FEQUAL( oper1.w, 4.0f );
	ASSERT_FEQUAL( oper3.x, 10.0f );
	ASSERT_FEQUAL( oper3.y, 20.0f );
	ASSERT_FEQUAL( oper3.z, 30.0f );
	ASSERT_FEQUAL( oper3.w, 40.0f );
	
	// operator /
	oper1.Set( 1.0f, 2.0f, 3.0f, 4.0f );
	oper3 = oper1 / 5.0f;
	ASSERT_FEQUAL( oper1.x, 1.0f );
	ASSERT_FEQUAL( oper1.y, 2.0f );
	ASSERT_FEQUAL( oper1.z, 3.0f );
	ASSERT_FEQUAL( oper1.w, 4.0f );
	ASSERT_FEQUAL( oper3.x, 0.2f );
	ASSERT_FEQUAL( oper3.y, 0.4f );
	ASSERT_FEQUAL( oper3.z, 0.6f );
	ASSERT_FEQUAL( oper3.w, 0.8f );
	
	// comparison
	
	// rotation
	decQuaternion qrot1;
	decQuaternion qrot2;
	decQuaternion qrot3;
	decMatrix mrot1;
	decMatrix mrot2;
	decMatrix mrot3;
	decVector pos1;
	decVector pos2;
	decVector pos3;
	int i, j, k;
	
	mrot1.SetRotation( 10.0f * DEG2RAD, -30.0f * DEG2RAD, 120.0f * DEG2RAD );
	qrot1 = mrot1.ToQuaternion();
	pos1.Set( 2.0f, 5.0f, 8.0f );
	ASSERT_TRUE( ( qrot1 * pos1 ).IsEqualTo( mrot1 * pos1 ) );
	
	mrot2.SetRotation( -50.0f * DEG2RAD, 20.0f * DEG2RAD, -250.0f * DEG2RAD );
	qrot2 = mrot2.ToQuaternion();
	pos2 = mrot1 * pos1;
	ASSERT_TRUE( ( qrot2 * pos2 ).IsEqualTo( mrot2 * pos2 ) );
	ASSERT_TRUE( ( qrot2 * ( qrot1 * pos1 ) ).IsEqualTo( mrot2 * ( mrot1 * pos1 ) ) );
	
	mrot3 = mrot1 * mrot2;
	qrot3 = qrot1 * qrot2;
	pos3 = mrot2 * pos2;
	ASSERT_TRUE( ( qrot3 * pos1 ).IsEqualTo( mrot3 * pos1, 1e-5f ) );
	ASSERT_TRUE( pos3.IsEqualTo( qrot3 * pos1, 1e-5f ) );
	
	for( i=-180; i<180; i+=5 ){
		for( j=-180; j<180; j+=5 ){
			for( k=-180; k<180; k+=5 ){
				mrot2.SetRotation( ( float )i * DEG2RAD, ( float )j * DEG2RAD, ( float )k * DEG2RAD );
				qrot2 = mrot2.ToQuaternion();
				qrot3 = qrot1 * qrot2;
				ASSERT_TRUE( ( qrot3 * pos1 ).IsEqualTo( qrot2 * ( qrot1 * pos1 ), 1e-5f ) );
			}
		}
	}
	
	decDMatrix dmrot1;
	decDMatrix dmrot2;
	decDMatrix dmrot3;
	decDVector dpos1;
	decDVector dpos2;
	decDVector dpos3;
	
	dmrot1.SetRotation( 10.0 * DEG2RAD, -30.0 * DEG2RAD, 120.0 * DEG2RAD );
	qrot1 = dmrot1.ToQuaternion();
	dpos1.Set( 2.0, 5.0, 8.0 );
	ASSERT_TRUE( ( qrot1 * dpos1 ).IsEqualTo( dmrot1 * dpos1, 0.00001 ) );
	
	dmrot2.SetRotation( -50.0 * DEG2RAD, 20.0 * DEG2RAD, -250.0 * DEG2RAD );
	qrot2 = dmrot2.ToQuaternion();
	dpos2 = dmrot1 * dpos1;
	ASSERT_TRUE( ( qrot2 * dpos2 ).IsEqualTo( dmrot2 * dpos2, 0.00001 ) );
	ASSERT_TRUE( ( qrot2 * ( qrot1 * dpos1 ) ).IsEqualTo( dmrot2 * ( dmrot1 * dpos1 ), 0.00001 ) );
	
	dmrot3 = dmrot1 * dmrot2;
	qrot3 = qrot1 * qrot2;
	dpos3 = dmrot2 * dpos2;
	ASSERT_TRUE( ( qrot3 * dpos1 ).IsEqualTo( dmrot3 * dpos1, 0.00001 ) );
	ASSERT_TRUE( dpos3.IsEqualTo( qrot3 * dpos1, 0.00001 ) );
	
	for( i=-180; i<180; i+=5 ){
		for( j=-180; j<180; j+=5 ){
			for( k=-180; k<180; k+=5 ){
				dmrot2.SetRotation( ( double )i * DEG2RAD, ( double )j * DEG2RAD, ( double )k * DEG2RAD );
				qrot2 = dmrot2.ToQuaternion();
				qrot3 = qrot1 * qrot2;
				ASSERT_TRUE( ( qrot3 * dpos1 ).IsEqualTo( qrot2 * ( qrot1 * dpos1 ), 1e-5f ) );
			}
		}
	}
	
	// euler conversion
	mrot1.SetRotation( 10.0f * DEG2RAD, -30.0f * DEG2RAD, 120.0f * DEG2RAD );
	qrot1 = mrot1.ToQuaternion();
	decVector euler1 = qrot1.GetEulerAngles();
	ASSERT_TRUE( euler1.IsEqualTo( decVector( 10.0f * DEG2RAD, -30.0f * DEG2RAD, 120.0f * DEG2RAD ), 1e-5f ) );
	ASSERT_TRUE( decQuaternion::CreateFromEuler( euler1 ).IsEqualTo( qrot1, 1e-5f ) );
	ASSERT_TRUE( decQuaternion::CreateFromEuler( euler1.x, euler1.y, euler1.z ).IsEqualTo( qrot1, 1e-5f ) );
	
	for( i=-180; i<180; i+=1 ){
		const float angle = ( float)i * DEG2RAD;
		ASSERT_TRUE( decQuaternion::CreateFromEulerX( angle ).SameRotation(
			decMatrix::CreateRotationX( angle ).ToQuaternion(), 0.001f ) );
		ASSERT_TRUE( decQuaternion::CreateFromEulerY( angle ).SameRotation(
			decMatrix::CreateRotationY( angle ).ToQuaternion(), 0.001f ) );
		ASSERT_TRUE( decQuaternion::CreateFromEulerZ( angle ).SameRotation(
			decMatrix::CreateRotationZ( angle ).ToQuaternion(), 0.001f ) );
	}
	
	// axis rotation
	mrot1.SetRotation( DEG2RAD * 20.0f, DEG2RAD * 50.0f, DEG2RAD * 40.0f );
	mrot2 = decMatrix::CreateRotationZ( DEG2RAD * 30.0f ) * mrot1;
	mrot3 = mrot1 * decMatrix::CreateRotationAxis( mrot1.TransformView(), DEG2RAD * 30.0f );
	ASSERT_TRUE( mrot3.IsEqualTo( mrot2 ) );
	
	dmrot1.SetRotation( DEG2RAD * 20.0, DEG2RAD * 50.0, DEG2RAD * 40.0 );
	dmrot2 = decDMatrix::CreateRotationZ( DEG2RAD * 30.0 ) * dmrot1;
	dmrot3 = dmrot1 * decDMatrix::CreateRotationAxis( dmrot1.TransformView(), DEG2RAD * 30.0 );
	ASSERT_TRUE( dmrot3.IsEqualTo( dmrot2 ) );
}
