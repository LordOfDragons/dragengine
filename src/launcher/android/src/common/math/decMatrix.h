/* 
 * Drag[en]gine Android Launcher
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




/**
 * @brief 4x4 Component Matrix.
 * Provides matrix calculation on a 4x4 matrix in row major order.
 * The coordinate system used by axis oriented creation functions
 * is a left handed one. The X axis points to the right, the Y axis
 * points up and the Z axis points into the screen. Rotations are
 * always in counter clockwise orientation. Rotation angles are
 * measured in radians.
 */
class decMatrix{
public:
	/**
	 * @name Cells
	 * The first number indicates the row the second the column.
	 */
	float a11, a12, a13, a14;
	float a21, a22, a23, a24;
	float a31, a32, a33, a34;
	float a41, a42, a43, a44;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new matrix initialized to identity. */
	decMatrix();
	/** Creates a copy of a matrix. */
	decMatrix( const decMatrix &m );
	/** Creates a copy of a double matrix. */
	decMatrix( const decDMatrix &m );
	/*@}*/
	
	/** @name Matrix Creation */
	/*@{*/
	/** Creates a new identity matrix. */
	static decMatrix CreateIdentity();
	/** Creates a new translation matrix. */
	static decMatrix CreateTranslation( float x, float y, float z );
	/** Creates a new translation matrix. */
	static decMatrix CreateTranslation( const decVector &t );
	/** Creates a new scaling matrix. */
	static decMatrix CreateScale( float sx, float sy, float sz );
	/** Creates a new scaling matrix. */
	static decMatrix CreateScale( const decVector &s );
	/** Creates a new rotation around the x axis matrix. */
	static decMatrix CreateRotationX( float arc );
	/** Creates a new rotation around the y axis matrix. */
	static decMatrix CreateRotationY( float arc );
	/** Creates a new rotation around the z axis matrix. */
	static decMatrix CreateRotationZ( float arc );
	/**
	 * Creates a new rotation around the all three axes matrix.
	 * This matrix is the same as first applying a z axis roation,
	 * then an x axis rotation and then an y axis rotation.
	 */
	static decMatrix CreateRotation( float rx, float ry, float rz );
	/**
	 * Creates a new rotation around the all three axes matrix.
	 * This matrix is the same as first applying a z axis roation,
	 * then an x axis rotation and then an y axis rotation.
	 */
	static decMatrix CreateRotation( const decVector &r );
	/** Creates a new rotation around an arbitrary axis matrix. */
	static decMatrix CreateRotationAxis( const decVector &axis, float angle );
	/**
	 * Creates a new world space matrix. A world matrix is used
	 * to transform coordinates from the coordinate system of
	 * one object to another one.
	 * @param pos Position of object
	 * @param view View vector of object not necessary normalized
	 * @param up Up vector of object not necessary normalized
	 */
	static decMatrix CreateWorld( const decVector &pos, const decVector &view, const decVector &up );
	/**
	 * Creates a new world space matrix. A world matrix is used
	 * to transform coordinates from the coordinate system of
	 * one object to another one.
	 * @param pos Position of object
	 * @param orientation Orientation
	 */
	static decMatrix CreateWorld( const decVector &pos, const decQuaternion &orientation );
	/**
	 * \brief Create world space matrix.
	 * \details A world matrix is used to transform coordinates from the object local
	 *          coordinate system to the world coordinate system.
	 */
	static decMatrix CreateWorld( const decVector &pos, const decQuaternion &orientation, const decVector &scale );
	/**
	 * Creates a new camera space matrix. A camera space matrix
	 * is different from a world space matrix in that it does
	 * transform positions from an world coordinate system into
	 * a coordinate system where the x axis points right, the
	 * y axis up and the z-axis straight into the screen. Best
	 * example to understand is a rotation. A world space matrix
	 * rotates a vector pointing straight down the z axis into
	 * the view vector used to create the matrix. A camera matrix
	 * though transforms a vector pointing into the view direction
	 * of the camera matrix into a vector pointing straight down
	 * the z axis. Hence the camera matrix is kinda the inverse
	 * of the world matrix what goes for the final transformation.
	 * @param pos Position of camera
	 * @param view View vector of camera not necessary normalized
	 * @param up Up vector of camera not necessary normalized
	 */
	static decMatrix CreateCamera( const decVector &pos, const decVector &view, const decVector &up );
	/**
	 * Creates a projection matrix based on a finite view frustum.
	 * The width and height are used to derive the aspect ratio from.
	 * @param width Width of the viewport window
	 * @param height Height of the viewport window
	 * @param fov Vertical field of view of the projection. Indicates
	 *            the radian angle from the left side of the view
	 *            frustum to the right side
	 * @param fovRatio Aspect ratio of the horizontal to the vertical
	 *                 field of view.
	 * @param znear Distance to near cliping plane
	 * @param zfar Distance to far cliping plane
	 */
	static decMatrix CreateProjection( int width, int height, float fov, float fovRatio, float znear, float zfar );
	/**
	 * Creates a projection matrix based on a infinite view frustum.
	 * The width and height are used to derive the aspect ratio from.
	 * @param width Width of the viewport window
	 * @param height Height of the viewport window
	 * @param fov Vertical field of view of the projection. Indicates
	 *            the radian angle from the left side of the view
	 *            frustum to the right side
	 * @param fovRatio Aspect ratio of the horizontal to the vertical
	 *                 field of view.
	 * @param znear Distance to near cliping plane
	 */
	static decMatrix CreateProjectionInfinite( int width, int height, float fov, float fovRatio, float znear );
	/**
	 * Creates an orthogonal projection matrix.
	 * @param left Left cliping plane position
	 * @param right Right cliping plane position
	 * @param top Top cliping plane position
	 * @param bottom Bottom cliping plane position
	 * @param znear Distance to near cliping plane
	 * @param zfar Distance to far cliping plane
	 */
	static decMatrix CreateOrtho( int left, int right, int top, int bottom, int znear, int zfar );
	/**
	 * Creates a matrix to transform a projection matrix into cliping space.
	 * In cliping space the view frustum turns into a cube.
	 */
	static decMatrix CreateClipSpace();
	/**
	 * Creates a bias matrix which multiplies all coordinates by 0.5 and
	 * offsets them by 0.5.
	 */
	static decMatrix CreateBiasMatrix();
	/**
	 * Creates a rotation-translation matrix. This is the same
	 * as first doing a 3-axis rotation and then a translation.
	 * @param rot Rotation vector
	 * @param pos Translation vector
	 */
	static decMatrix CreateRT( const decVector &rot, const decVector &pos );
	/**
	 * Creates a scale-rotation-translation matrix. This is the same
	 * as first doing a scaling, then a 3-axis rotation and then
	 * a translation.
	 * @param scale Scaling vector
	 * @param rot Rotation vector
	 * @param pos Translation vector
	 */
	static decMatrix CreateSRT( const decVector &scale, const decVector &rot, const decVector &pos );
	/**
	 * Creates a scale-view-up-translation matrix. This is the same
	 * as first doing a scaling, then a world space matrix.
	 * @param scale Scaling vector
	 * @param view View vector
	 * @param up Up vector
	 * @param pos Translation vector
	 */
	static decMatrix CreateSVUT( const decVector &scale, const decVector &view, const decVector &up, const decVector &pos );
	/**
	 * Creates a view-up matrix. This is the same as doing a world
	 * space matrix but with the zero vector as position.
	 * @param view View vector
	 * @param up Up vector
	 */
	static decMatrix CreateVU( const decVector &view, const decVector &up );
	/**
	 * Creates a rotation matrix from a quaternion.
	 * @param q Rotation quaternion
	 */
	static decMatrix CreateFromQuaternion( const decQuaternion &q );
	/**
	 * Creates a rotation matrix from a quaternion.
	 */
	static decMatrix CreateFromQuaternion( const decQuaternion &q, const decVector &pivot );
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Sets identity matrix. */
	void SetIdentity();
	/** Sets translation matrix. */
	void SetTranslation( float x, float y, float z );
	/** Sets translation matrix. */
	void SetTranslation( const decVector &t );
	/** Sets scaling matrix. */
	void SetScale( float sx, float sy, float sz );
	/** Sets scaling matrix. */
	void SetScale( const decVector &s );
	/** Sets rotation around the x axis matrix. */
	void SetRotationX( float arc );
	/** Sets rotation around the y axis matrix. */
	void SetRotationY( float arc );
	/** Sets rotation around the z axis matrix. */
	void SetRotationZ( float arc );
	/**
	 * Sets rotation around the all three axes matrix.
	 * This matrix is the same as first applying a z axis roation,
	 * then an x axis rotation and then an y axis rotation.
	 */
	void SetRotation( float rx, float ry, float rz );
	/**
	 * Sets rotation around the all three axes matrix.
	 * This matrix is the same as first applying a z axis roation,
	 * then an x axis rotation and then an y axis rotation.
	 */
	void SetRotation( const decVector &r );
	/** Sets rotation around an arbitrary axis matrix. */
	void SetRotationAxis( const decVector &axis, float angle );
	/**
	 * Sets world space matrix. A world matrix is used
	 * to transform coordinates from the coordinate system of
	 * one object to another one.
	 * @param pos Position of object
	 * @param view View vector of object not necessary normalized
	 * @param up Up vector of object not necessary normalized
	 */
	void SetWorld( const decVector &pos, const decVector &view, const decVector &up );
	/**
	 * Sets world space matrix. A world matrix is used
	 * to transform coordinates from the coordinate system of
	 * one object to another one.
	 * @param pos Position of object
	 * @param orientation Orientation
	 */
	void SetWorld( const decVector &pos, const decQuaternion &orientation );
	/**
	 * \brief Set world space matrix.
	 * \details A world matrix is used to transform coordinates from the object local
	 *          coordinate system to the world coordinate system.
	 */
	void SetWorld( const decVector &pos, const decQuaternion &orientation, const decVector &scale );
	/**
	 * Sets camera space matrix. A camera space matrix
	 * is different from a world space matrix in that it does
	 * transform positions from an world coordinate system into
	 * a coordinate system where the x axis points right, the
	 * y axis up and the z-axis straight into the screen. Best
	 * example to understand is a rotation. A world space matrix
	 * rotates a vector pointing straight down the z axis into
	 * the view vector used to create the matrix. A camera matrix
	 * though transforms a vector pointing into the view direction
	 * of the camera matrix into a vector pointing straight down
	 * the z axis. Hence the camera matrix is kinda the inverse
	 * of the world matrix what goes for the final transformation.
	 * @param pos Position of camera
	 * @param view View vector of camera not necessary normalized
	 * @param up Up vector of camera not necessary normalized
	 */
	void SetCamera( const decVector &pos, const decVector &view, const decVector &up );
	/**
	 * Sets projection matrix based on a finite view frustum.
	 * The width and height are used to derive the aspect ratio from.
	 * @param width Width of the viewport window
	 * @param height Height of the viewport window
	 * @param fov Vertical field of view of the projection. Indicates
	 *            the radian angle from the left side of the view
	 *            frustum to the right side
	 * @param fovRatio Aspect ratio of the horizontal to the vertical
	 *                 field of view.
	 * @param znear Distance to near cliping plane
	 * @param zfar Distance to far cliping plane
	 */
	void SetProjection( int width, int height, float fov, float fovRatio, float znear, float zfar );
	/**
	 * Sets projection matrix based on a infinite view frustum.
	 * The width and height are used to derive the aspect ratio from.
	 * @param width Width of the viewport window
	 * @param height Height of the viewport window
	 * @param fov Vertical field of view of the projection. Indicates
	 *            the radian angle from the left side of the view
	 *            frustum to the right side
	 * @param fovRatio Aspect ratio of the horizontal to the vertical
	 *                 field of view.
	 * @param znear Distance to near cliping plane
	 */
	void SetProjectionInfinite( int width, int height, float fov, float fovRatio, float znear );
	/**
	 * Sets orthogonal projection matrix.
	 * @param left Left cliping plane position
	 * @param right Right cliping plane position
	 * @param top Top cliping plane position
	 * @param bottom Bottom cliping plane position
	 * @param znear Distance to near cliping plane
	 * @param zfar Distance to far cliping plane
	 */
	void SetOrtho( int left, int right, int top, int bottom, int znear, int zfar );
	/**
	 * Sets matrix to transform a projection matrix into cliping space.
	 * In cliping space the view frustum turns into a cube.
	 */
	void SetClipSpace();
	/**
	 * Sets bias matrix which multiplies all coordinates by 0.5 and
	 * offsets them by 0.5.
	 */
	void SetBiasMatrix();
	/**
	 * Sets rotation-translation matrix. This is the same
	 * as first doing a 3-axis rotation and then a translation.
	 * @param rot Rotation vector
	 * @param pos Translation vector
	 */
	void SetRT( const decVector &rot, const decVector &pos );
	/**
	 * Sets scale-rotation-translation matrix. This is the same
	 * as first doing a scaling, then a 3-axis rotation and then
	 * a translation.
	 * @param scale Scaling vector
	 * @param rot Rotation vector
	 * @param pos Translation vector
	 */
	void SetSRT( const decVector &scale, const decVector &rot, const decVector &pos );
	/**
	 * Sets scale-view-up-translation matrix. This is the same
	 * as first doing a scaling, then a world space matrix.
	 * @param scale Scaling vector
	 * @param view View vector
	 * @param up Up vector
	 * @param pos Translation vector
	 */
	void SetSVUT( const decVector &scale, const decVector &view, const decVector &up, const decVector &pos );
	/**
	 * Sets view-up matrix. This is the same as doing a world
	 * space matrix but with the zero vector as position.
	 * @param view View vector
	 * @param up Up vector
	 */
	void SetVU( const decVector &view, const decVector &up );
	/**
	 * Sets rotation matrix from a quaternion.
	 * @param q Rotation quaternion
	 */
	void SetFromQuaternion( const decQuaternion &q );
	/**
	 * Sets rotation matrix from a quaternion.
	 */
	void SetFromQuaternion( const decQuaternion &q, const decVector &pivot );
	
	/** Calculates the determinant of the matrix. */
	float Determinant() const;
	/** Retrieves the inverse of this matrix if the inverse is possible. */
	decMatrix Invert() const;
	/**
	 * Calculates a set of euler angles producing this matrix if they
	 * are used as rotation vector for creating a rotation matrix.
	 * The calculated angles are not unique and different rotation
	 * vectors lead usually to the same matrix. The calculated rotation
	 * vector is garanteed to produce the matrix it has been calculated
	 * from. Furthermore there exist ambiguous situations where the
	 * calculated rotation vector is undefined. You should avoid using
	 * this function if you can.
	 */
	decVector GetEulerAngles() const;
	/**
	 * Converts the matrix into a rotation quaternion. This conversion
	 * is well defined in contrast to the rotation vector function.
	 */
	decQuaternion ToQuaternion() const;
	/**
	 * Retrieves the position of the matrix. This is the same as
	 * transforming the vector ( 0,0,0 ) by this matrix but faster.
	 */
	decVector GetPosition() const;
	/**
	 * Retrieves the scaling of the matrix. This works only if the matrix
	 * is the product of translations, rotations and scaling. The returned
	 * scaling is always positive.
	 */
	decVector GetScale() const;
	/**
	 * Retrieves the view vector of this matrix. This is the same as
	 * transforing the vector ( 0,0,1 ) by this matrix but faster.
	 */
	decVector TransformView() const;
	/**
	 * Retrieves the right vector of this matrix. This is the same as
	 * transforing the vector ( 1,0,0 ) by this matrix but faster.
	 */
	decVector TransformRight() const;
	/**
	 * Retrieves the up vector of this matrix. This is the same as
	 * transforing the vector ( 0,1,0 ) by this matrix but faster.
	 */
	decVector TransformUp() const;
	/**
	 * Transforms a normal by this matrix. This is the same as
	 * transforming the normal by only the rotation part of it.
	 */
	decVector TransformNormal( const decVector &normal ) const;
	/** Transforms a vector. */
	decVector Transform( float x, float y, float z ) const;
	decVector4 Transform( float x, float y, float z, float w ) const;
	void Transform( decVector &result, float x, float y, float z ) const;
	void Transform( decVector4 &result, float x, float y, float z, float w ) const;
	/**
	 * Retrieves the rotation part of the matrix. The result is a
	 * 4x4 identity matrix with the upper left 3x3 copied from
	 * this matrix.
	 */
	decMatrix GetRotationMatrix() const;
	/**
	 * Determines if this matrix is cell wise equal to anither one in respect to a threshold.
	 */
	bool IsEqualTo( const decMatrix &matrix, float threshold = 0.0001 ) const;
	/*@}*/
	
	/**
	 * @name Quick Calculations
	 * Quick calculations are a special set of functions designed to
	 * work only with matrices which are non-projective. This kind of
	 * matrices have only a rotation part and a translation part. The
	 * entire last row has the values 0,0,0,1 . Those functions are
	 * the same as the non-quick versions with the difference that
	 * calculations are stripped out due to the last row beeing 0,0,0,1 .
	 * As the name suggests those functions are faster than the normal
	 * ones for projective matrices due to them having less calculations.
	 * Only use those functions if you know for sure that your matrices
	 * are of the described kind.
	 */
	/*@{*/
	/** Quick multiplies this matrix with another one. */
	decMatrix QuickMultiply( const decMatrix &matrix ) const;
	/** Quick multiplies this matrix with a value k. */
	decMatrix QuickMultiply( float k ) const;
	/** Quick calculates the determinant of this matrix. */
	float QuickDeterminant() const;
	/** Quick calculates the inverse of this matrix. */
	decMatrix QuickInvert() const;
	/** Quick cell wise adds another matrix to this matrix. */
	decMatrix &QuickAddTo( const decMatrix &m );
	/*@}*/
	
	/** @name Operators */
	/*@{*/
	/** Sets the cells of this matrix to the values of another one. */
	decMatrix &operator=( const decMatrix &m );
	/** Adds the cells of another matrix to this one. */
	decMatrix &operator+=( const decMatrix &m );
	/** Subtracts the cells of another matrix from this vector. */
	decMatrix &operator-=( const decMatrix &m );
	/** Multiplies the cells of this matrix with a value k. */
	decMatrix &operator*=( float k );
	/**
	 * Divides the cells of this matrix by a value k. If the value k
	 * is 0 an exception is thrown.
	 */
	decMatrix &operator/=( float k );
	/** Multiplies this matrix with another one. */
	decMatrix &operator*=( const decMatrix &m );
	/** Retrieves a new matrix with the sum of this matrix with another one. */
	decMatrix operator+( const decMatrix &m ) const;
	/** Retrieves a new matrix with the difference of this matrix to another one. */
	decMatrix operator-( const decMatrix &m ) const;
	/** Retrieves a new matrix with this matrix scaled by k. */
	decMatrix operator*( float k ) const;
	/**
	 * Retrieves a new matrix with this matrix divided by k. If k is 0 an
	 * exception is thrown.
	 */
	decMatrix operator/( float k ) const;
	/** Retrieves a new matrix with this matrix multiplied with another one. */
	decMatrix operator*( const decMatrix &m ) const;
	/** Transforms vector by this matrix. */
	decVector operator*( const decVector &v ) const;
	/** Transforms 4 component vector by this matrix. */
	decVector4 operator*( const decVector4 &v ) const;
	/*@}*/
};
