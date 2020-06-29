/* 
 * Drag[en]gine Game Engine
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
 * \brief 4x4 Component Matrix.
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
	 * \name Cells
	 * The first number indicates the row the second the column.
	 */
	float a11, a12, a13, a14;
	float a21, a22, a23, a24;
	float a31, a32, a33, a34;
	float a41, a42, a43, a44;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new matrix initialized to identity. */
	decMatrix();
	
	/** \brief Create copy of a matrix. */
	decMatrix( const decMatrix &m );
	
	/** \brief Create copy of a double matrix. */
	decMatrix( const decDMatrix &m );
	/*@}*/
	
	
	
	/** \name Matrix Creation */
	/*@{*/
	/** \brief Create new identity matrix. */
	static decMatrix CreateIdentity();
	
	/** \brief Create new translation matrix. */
	static decMatrix CreateTranslation( float x, float y, float z );
	
	/** \brief Create new translation matrix. */
	static decMatrix CreateTranslation( const decVector &t );
	
	/** \brief Create new scaling matrix. */
	static decMatrix CreateScale( float sx, float sy, float sz );
	
	/** \brief Create new scaling matrix. */
	static decMatrix CreateScale( const decVector &s );
	
	/** \brief Create new rotation around the x axis matrix. */
	static decMatrix CreateRotationX( float arc );
	
	/** \brief Create new rotation around the y axis matrix. */
	static decMatrix CreateRotationY( float arc );
	
	/** \brief Create new rotation around the z axis matrix. */
	static decMatrix CreateRotationZ( float arc );
	
	/**
	 * \brief Create new rotation around the all three axes matrix.
	 * 
	 * This matrix is the same as first applying a z axis roation,
	 * then an x axis rotation and then an y axis rotation.
	 */
	static decMatrix CreateRotation( float rx, float ry, float rz );
	
	/**
	 * \brief Create new rotation around the all three axes matrix.
	 * 
	 * This matrix is the same as first applying a z axis roation,
	 * then an x axis rotation and then an y axis rotation.
	 */
	static decMatrix CreateRotation( const decVector &r );
	
	/** \brief Create new rotation around an arbitrary axis matrix. */
	static decMatrix CreateRotationAxis( const decVector &axis, float angle );
	
	/**
	 * \brief Create new world space matrix.
	 * 
	 * A world matrix is used to transform coordinates from the coordinate system of
	 * one object to another one.
	 * 
	 * \param pos Position of object
	 * \param view View vector of object not necessary normalized
	 * \param up Up vector of object not necessary normalized
	 */
	static decMatrix CreateWorld( const decVector &pos, const decVector &view,
		const decVector &up );
	
	/**
	 * \brief Create new world space matrix.
	 * 
	 * A world matrix is used to transform coordinates from the coordinate system of
	 * one object to another one.
	 * 
	 * \param pos Position of object
	 * \param orientation Orientation
	 */
	static decMatrix CreateWorld( const decVector &pos, const decQuaternion &orientation );
	
	/**
	 * \brief Create world space matrix.
	 * 
	 * A world matrix is used to transform coordinates from the object local
	 * coordinate system to the world coordinate system.
	 */
	static decMatrix CreateWorld( const decVector &pos, const decQuaternion &orientation,
		const decVector &scale );
	
	/**
	 * \brief Create new camera space matrix.
	 * 
	 * A camera space matrix is different from a world space matrix in that it does
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
	 * 
	 * \param pos Position of camera
	 * \param view View vector of camera not necessary normalized
	 * \param up Up vector of camera not necessary normalized
	 */
	static decMatrix CreateCamera( const decVector &pos, const decVector &view, const decVector &up );
	
	/**
	 * \brief Create projection matrix based on a finite view frustum.
	 * 
	 * The width and height are used to derive the aspect ratio from.
	 * 
	 * \param width Width of the viewport window
	 * \param height Height of the viewport window
	 * \param fov Vertical field of view of the projection. Indicates the radian angle from
	 *            the left side of the view frustum to the right side
	 * \param fovRatio Aspect ratio of the horizontal to the vertical field of view.
	 * \param znear Distance to near cliping plane
	 * \param zfar Distance to far cliping plane
	 */
	static decMatrix CreateProjection( int width, int height, float fov, float fovRatio,
		float znear, float zfar );
	
	/**
	 * \brief Create projection matrix based on a infinite view frustum.
	 * 
	 * The width and height are used to derive the aspect ratio from.
	 * 
	 * \param width Width of the viewport window
	 * \param height Height of the viewport window
	 * \param fov Vertical field of view of the projection. Indicates the radian angle from
	 *            the left side of the view frustum to the right side
	 * \param fovRatio Aspect ratio of the horizontal to the vertical field of view.
	 * \param znear Distance to near cliping plane
	 */
	static decMatrix CreateProjectionInfinite( int width, int height, float fov,
		float fovRatio, float znear );
	
	/**
	 * \brief Create orthogonal projection matrix.
	 * \param left Left cliping plane position
	 * \param right Right cliping plane position
	 * \param top Top cliping plane position
	 * \param bottom Bottom cliping plane position
	 * \param znear Distance to near cliping plane
	 * \param zfar Distance to far cliping plane
	 */
	static decMatrix CreateOrtho( int left, int right, int top, int bottom, int znear, int zfar );
	
	/**
	 * \brief Create matrix to transform a projection matrix into cliping space.
	 * 
	 * In cliping space the view frustum turns into a cube.
	 */
	static decMatrix CreateClipSpace();
	
	/**
	 * \brief Create bias matrix which multiplies all coordinates by 0.5 and offsets them by 0.5.
	 */
	static decMatrix CreateBiasMatrix();
	
	/**
	 * \brief Create rotation-translation matrix.
	 * 
	 * This is the same as first doing a 3-axis rotation and then a translation.
	 * 
	 * \param rot Rotation vector
	 * \param pos Translation vector
	 */
	static decMatrix CreateRT( const decVector &rot, const decVector &pos );
	
	/**
	 * \brief Create scale-rotation-translation matrix.
	 * 
	 * This is the same as first doing a scaling, then a 3-axis rotation and then a translation.
	 * 
	 * \param scale Scaling vector
	 * \param rot Rotation vector
	 * \param pos Translation vector
	 */
	static decMatrix CreateSRT( const decVector &scale, const decVector &rot, const decVector &pos );
	
	/**
	 * \brief Create scale-view-up-translation matrix.
	 * 
	 * This is the same as first doing a scaling, then a world space matrix.
	 * 
	 * \param scale Scaling vector
	 * \param view View vector
	 * \param up Up vector
	 * \param pos Translation vector
	 */
	static decMatrix CreateSVUT( const decVector &scale, const decVector &view,
		const decVector &up, const decVector &pos );
	
	/**
	 * \brief Create view-up matrix.
	 * 
	 * This is the same as doing a world space matrix but with the zero vector as position.
	 * 
	 * \param view View vector
	 * \param up Up vector
	 */
	static decMatrix CreateVU( const decVector &view, const decVector &up );
	
	/**
	 * \brief Create rotation matrix from a quaternion.
	 * \param q Rotation quaternion
	 */
	static decMatrix CreateFromQuaternion( const decQuaternion &q );
	
	/** \brief Create rotation matrix from a quaternion. */
	static decMatrix CreateFromQuaternion( const decQuaternion &q, const decVector &pivot );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Sets identity matrix. */
	void SetIdentity();
	
	/** \brief Sets translation matrix. */
	void SetTranslation( float x, float y, float z );
	
	/** \brief Sets translation matrix. */
	void SetTranslation( const decVector &t );
	
	/** \brief Sets scaling matrix. */
	void SetScale( float sx, float sy, float sz );
	
	/** \brief Sets scaling matrix. */
	void SetScale( const decVector &s );
	
	/** \brief Sets rotation around the x axis matrix. */
	void SetRotationX( float arc );
	
	/** \brief Sets rotation around the y axis matrix. */
	void SetRotationY( float arc );
	
	/** \brief Sets rotation around the z axis matrix. */
	void SetRotationZ( float arc );
	
	/**
	 * \brief Set rotation around the all three axes matrix.
	 * 
	 * This matrix is the same as first applying a z axis roation,
	 * then an x axis rotation and then an y axis rotation.
	 */
	void SetRotation( float rx, float ry, float rz );
	
	/**
	 * \brief Set rotation around the all three axes matrix.
	 * 
	 * This matrix is the same as first applying a z axis roation,
	 * then an x axis rotation and then an y axis rotation.
	 */
	void SetRotation( const decVector &r );
	
	/** \brief Sets rotation around an arbitrary axis matrix. */
	void SetRotationAxis( const decVector &axis, float angle );
	
	/**
	 * \brief Set world space matrix.
	 * 
	 * A world matrix is used to transform coordinates from the coordinate system of
	 * one object to another one.
	 * 
	 * \param pos Position of object
	 * \param view View vector of object not necessary normalized
	 * \param up Up vector of object not necessary normalized
	 */
	void SetWorld( const decVector &pos, const decVector &view, const decVector &up );
	
	/**
	 * \brief Set world space matrix.
	 * 
	 * A world matrix is used to transform coordinates from the coordinate system of
	 * one object to another one.
	 * 
	 * \param pos Position of object
	 * \param orientation Orientation
	 */
	void SetWorld( const decVector &pos, const decQuaternion &orientation );
	
	/**
	 * \brief Set world space matrix.
	 * 
	 * A world matrix is used to transform coordinates from the object local
	 * coordinate system to the world coordinate system.
	 */
	void SetWorld( const decVector &pos, const decQuaternion &orientation, const decVector &scale );
	
	/**
	 * \brief Set camera space matrix.
	 * 
	 * A camera space matrix is different from a world space matrix in that it does
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
	 * 
	 * \param pos Position of camera
	 * \param view View vector of camera not necessary normalized
	 * \param up Up vector of camera not necessary normalized
	 */
	void SetCamera( const decVector &pos, const decVector &view, const decVector &up );
	
	/**
	 * \brief Set projection matrix based on a finite view frustum.
	 * 
	 * The width and height are used to derive the aspect ratio from.
	 * 
	 * \param width Width of the viewport window
	 * \param height Height of the viewport window
	 * \param fov Vertical field of view of the projection. Indicates the radian angle from
	 *            the left side of the view frustum to the right side
	 * \param fovRatio Aspect ratio of the horizontal to the vertical field of view.
	 * \param znear Distance to near cliping plane
	 * \param zfar Distance to far cliping plane
	 */
	void SetProjection( int width, int height, float fov, float fovRatio, float znear, float zfar );
	
	/**
	 * \brief Set projection matrix based on a infinite view frustum.
	 * 
	 * The width and height are used to derive the aspect ratio from.
	 * 
	 * \param width Width of the viewport window
	 * \param height Height of the viewport window
	 * \param fov Vertical field of view of the projection. Indicates the radian angle from
	 *            the left side of the view frustum to the right side
	 * \param fovRatio Aspect ratio of the horizontal to the vertical field of view.
	 * \param znear Distance to near cliping plane
	 */
	void SetProjectionInfinite( int width, int height, float fov, float fovRatio, float znear );
	
	/**
	 * \brief Set orthogonal projection matrix.
	 * \param left Left cliping plane position
	 * \param right Right cliping plane position
	 * \param top Top cliping plane position
	 * \param bottom Bottom cliping plane position
	 * \param znear Distance to near cliping plane
	 * \param zfar Distance to far cliping plane
	 */
	void SetOrtho( int left, int right, int top, int bottom, int znear, int zfar );
	
	/**
	 * \brief Set matrix to transform a projection matrix into cliping space.
	 * 
	 * In cliping space the view frustum turns into a cube.
	 */
	void SetClipSpace();
	
	/**
	 * \brief Set bias matrix which multiplies all coordinates by 0.5 and offsets them by 0.5.
	 */
	void SetBiasMatrix();
	
	/**
	 * \brief Set rotation-translation matrix.
	 * 
	 * This is the same as first doing a 3-axis rotation and then a translation.
	 * 
	 * \param rot Rotation vector
	 * \param pos Translation vector
	 */
	void SetRT( const decVector &rot, const decVector &pos );
	
	/**
	 * \brief Set scale-rotation-translation matrix.
	 * 
	 * This is the same as first doing a scaling, then a 3-axis rotation and then a translation.
	 * 
	 * \param scale Scaling vector
	 * \param rot Rotation vector
	 * \param pos Translation vector
	 */
	void SetSRT( const decVector &scale, const decVector &rot, const decVector &pos );
	
	/**
	 * \brief Set scale-view-up-translation matrix.
	 * 
	 * This is the same as first doing a scaling, then a world space matrix.
	 * 
	 * \param scale Scaling vector
	 * \param view View vector
	 * \param up Up vector
	 * \param pos Translation vector
	 */
	void SetSVUT( const decVector &scale, const decVector &view, const decVector &up,
		const decVector &pos );
	
	/**
	 * \brief Set view-up matrix.
	 * 
	 * This is the same as doing a world space matrix but with the zero vector as position.
	 * 
	 * \param view View vector
	 * \param up Up vector
	 */
	void SetVU( const decVector &view, const decVector &up );
	
	/**
	 * \brief Set rotation matrix from a quaternion.
	 * \param q Rotation quaternion
	 */
	void SetFromQuaternion( const decQuaternion &q );
	
	/** \brief Set rotation matrix from a quaternion. */
	void SetFromQuaternion( const decQuaternion &q, const decVector &pivot );
	
	/** \brief Calculates the determinant of the matrix. */
	float Determinant() const;
	
	/** \brief Inverse of this matrix if the inverse is possible. */
	decMatrix Invert() const;
	
	/**
	 * \brief Calculate set of euler angles producing this matrix if they are used as
	 *        rotation vector for creating a rotation matrix.
	 * 
	 * The calculated angles are not unique and different rotation
	 * vectors lead usually to the same matrix. The calculated rotation
	 * vector is garanteed to produce the matrix it has been calculated
	 * from. Furthermore there exist ambigious situations where the
	 * calculated rotation vector is undefined. You should avoid using
	 * this function if you can.
	 */
	decVector GetEulerAngles() const;
	
	/**
	 * \brief Convert matrix into a rotation quaternion.
	 * 
	 * This conversion is well defined in contrast to the rotation vector function.
	 */
	decQuaternion ToQuaternion() const;
	
	/**
	 * \brief Position of the matrix.
	 * 
	 * This is the same as transforming the vector ( 0,0,0 ) by this matrix but faster.
	 */
	decVector GetPosition() const;
	
	/**
	 * \brief Scaling of the matrix.
	 * 
	 * This works only if the matrix is the product of translations, rotations and scaling.
	 * The returned scaling is always positive.
	 */
	decVector GetScale() const;
	
	/**
	 * \brief View vector of this matrix.
	 * 
	 * This is the same as transforing the vector (0,0,1) by this matrix but faster.
	 */
	decVector TransformView() const;
	
	/**
	 * \brief Right vector of this matrix.
	 * 
	 * This is the same as transforing the vector (1,0,0) by this matrix but faster.
	 */
	decVector TransformRight() const;
	
	/**
	 * \brief Up vector of this matrix.
	 * 
	 * This is the same as transforing the vector (0,1,0) by this matrix but faster.
	 */
	decVector TransformUp() const;
	
	/**
	 * \brief Transforms a normal by this matrix.
	 * 
	 * This is the same as transforming the normal by only the rotation part of it.
	 */
	decVector TransformNormal( const decVector &normal ) const;
	
	/** \brief Transforma vector. */
	decVector Transform( float x, float y, float z ) const;
	
	/** \brief Transforma vector. */
	decVector4 Transform( float x, float y, float z, float w ) const;
	
	/** \brief Transforma vector. */
	void Transform( decVector &result, float x, float y, float z ) const;
	
	/** \brief Transforma vector. */
	void Transform( decVector4 &result, float x, float y, float z, float w ) const;
	
	/**
	 * \brief Rotation part of the matrix.
	 * 
	 * The result is a 4x4 identity matrix with the upper left 3x3 copied from this matrix.
	 */
	decMatrix GetRotationMatrix() const;
	
	/**
	 * \brief Normalize matrix.
	 * 
	 * Modifies matrix with scaling removed. Only normalized matrices are safe to be used
	 * with the GetEulerAngles() and ToQuaternion() calls since scaling interferes with
	 * the calculation.
	 * 
	 * This calls the same as this code
	 * \code{.cpp}
	 * SetWorld(GetPosition(), TransformView(), TransformUp())
	 * \endcode
	 * 
	 * but is safe against 0-scaling. In such a case a 0-rotation matrix is set.
	 */
	void Normalize();
	
	/**
	 * \brief Normalize matrix.
	 * 
	 * Returns matrix with scaling removed. Only normalized matrices are safe to be used
	 * with the GetEulerAngles() and ToQuaternion() calls since scaling interferes with
	 * the calculation.
	 * 
	 * This calls the same as this code
	 * \code{.cpp}
	 * decMatrix::CreateWorld(m.GetPosition(), m.TransformView(), m.TransformUp())
	 * \endcode
	 * 
	 * but is safe against 0-scaling. In such a case a 0-rotation matrix is created.
	 */
	decMatrix Normalized() const;
	
	/** \brief Matrix is cell wise equal to anither one in respect to a threshold. */
	bool IsEqualTo( const decMatrix &matrix, float threshold = 0.0001 ) const;
	/*@}*/
	
	
	
	/**
	 * \name Quick Calculations
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
	/** \brief Quick multiplies this matrix with another one. */
	decMatrix QuickMultiply( const decMatrix &matrix ) const;
	
	/** \brief Quick multiplies this matrix with a value k. */
	decMatrix QuickMultiply( float k ) const;
	
	/** \brief Quick calculates the determinant of this matrix. */
	float QuickDeterminant() const;
	
	/** \brief Quick calculates the inverse of this matrix. */
	decMatrix QuickInvert() const;
	
	/** \brief Quick cell wise adds another matrix to this matrix. */
	decMatrix &QuickAddTo( const decMatrix &m );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set cells of this matrix to the values of another one. */
	decMatrix &operator=( const decMatrix &m );
	
	/** \brief Adds the cells of another matrix to this one. */
	decMatrix &operator+=( const decMatrix &m );
	
	/** \brief Subtracts the cells of another matrix from this vector. */
	decMatrix &operator-=( const decMatrix &m );
	
	/** \brief Multiplies the cells of this matrix with a value k. */
	decMatrix &operator*=( float k );
	
	/**
	 * \brief Divide cells of this matrix by a value k.
	 * 
	 * If the value k is 0 an exception is thrown.
	 */
	decMatrix &operator/=( float k );
	
	/** \brief Multiplies this matrix with another one. */
	decMatrix &operator*=( const decMatrix &m );
	
	/** \brief Retrieves a new matrix with the sum of this matrix with another one. */
	decMatrix operator+( const decMatrix &m ) const;
	
	/** \brief Retrieves a new matrix with the difference of this matrix to another one. */
	decMatrix operator-( const decMatrix &m ) const;
	
	/** \brief Retrieves a new matrix with this matrix scaled by k. */
	decMatrix operator*( float k ) const;
	
	/**
	 * \brief Matrix with this matrix divided by k.
	 * 
	 * If k is 0 an exception is thrown.
	 */
	decMatrix operator/( float k ) const;
	
	/** \brief Retrieves a new matrix with this matrix multiplied with another one. */
	decMatrix operator*( const decMatrix &m ) const;
	
	/** \brief Transforms vector by this matrix. */
	decVector operator*( const decVector &v ) const;
	
	/** \brief Transforms 4 component vector by this matrix. */
	decVector4 operator*( const decVector4 &v ) const;
	/*@}*/
};
