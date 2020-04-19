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
 * \brief 4x4 Component Matrix double precision.
 * Same as the decDMatrix class but with double precision.
 */
class decDMatrix{
public:
	/**
	 * \name Cells
	 * The first number indicates the row the second the column.
	 */
	double a11, a12, a13, a14;
	double a21, a22, a23, a24;
	double a31, a32, a33, a34;
	double a41, a42, a43, a44;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new matrix initialized to identity. */
	decDMatrix();
	
	/** \brief Create copy of a matrix. */
	decDMatrix( const decDMatrix &m );
	
	/** \brief Create copy of a float matrix. */
	decDMatrix( const decMatrix &m );
	/*@}*/
	
	
	
	/** \name Matrix Creation */
	/*@{*/
	/** \brief Create new identity matrix. */
	static decDMatrix CreateIdentity();
	
	/** \brief Create new translation matrix. */
	static decDMatrix CreateTranslation( double x, double y, double z );
	
	/** \brief Create new translation matrix. */
	static decDMatrix CreateTranslation( const decDVector &t );
	
	/** \brief Create new scaling matrix. */
	static decDMatrix CreateScale( double sx, double sy, double sz );
	
	/** \brief Create new scaling matrix. */
	static decDMatrix CreateScale( const decDVector &s );
	
	/** \brief Create new rotation around the x axis matrix. */
	static decDMatrix CreateRotationX( double arc );
	
	/** \brief Create new rotation around the y axis matrix. */
	static decDMatrix CreateRotationY( double arc );
	
	/** \brief Create new rotation around the z axis matrix. */
	static decDMatrix CreateRotationZ( double arc );
	
	/**
	 * \brief Create new rotation around the all three axes matrix.
	 * 
	 * This matrix is the same as first applying a z axis roation,
	 * then an x axis rotation and then an y axis rotation.
	 */
	static decDMatrix CreateRotation( double rx, double ry, double rz );
	
	/**
	 * \brief Create new rotation around the all three axes matrix.
	 * 
	 * This matrix is the same as first applying a z axis roation,
	 * then an x axis rotation and then an y axis rotation.
	 */
	static decDMatrix CreateRotation( const decDVector &r );
	
	/** \brief Create new rotation around an arbitrary axis matrix. */
	static decDMatrix CreateRotationAxis( const decDVector &axis, double angle );
	
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
	static decDMatrix CreateWorld( const decDVector &pos, const decDVector &view,
		const decDVector &up );
	
	/**
	 * \brief Create new world space matrix.
	 * 
	 * A world matrix is used to transform coordinates from the coordinate system of
	 * one object to another one.
	 * 
	 * \param pos Position of object
	 * \param orientation Orientation
	 */
	static decDMatrix CreateWorld( const decDVector &pos, const decQuaternion &orientation );
	
	/**
	 * \brief Create world space matrix.
	 * 
	 * A world matrix is used to transform coordinates from the object local
	 * coordinate system to the world coordinate system.
	 */
	static decDMatrix CreateWorld( const decDVector &pos, const decQuaternion &orientation,
		const decDVector &scale );
	
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
	static decDMatrix CreateCamera( const decDVector &pos, const decDVector &view,
		const decDVector &up );
	
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
	static decDMatrix CreateProjection( int width, int height, double fov, double fovRatio,
		double znear, double zfar );
	
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
	static decDMatrix CreateProjectionInfinite( int width, int height, double fov,
		double fovRatio, double znear );
	
	/**
	 * \brief Create orthogonal projection matrix.
	 * \param left Left cliping plane position
	 * \param right Right cliping plane position
	 * \param top Top cliping plane position
	 * \param bottom Bottom cliping plane position
	 * \param znear Distance to near cliping plane
	 * \param zfar Distance to far cliping plane
	 */
	static decDMatrix CreateOrtho( int left, int right, int top, int bottom, int znear, int zfar );
	
	/**
	 * \brief Create matrix to transform a projection matrix into cliping space.
	 * 
	 * In cliping space the view frustum turns into a cube.
	 */
	static decDMatrix CreateClipSpace();
	
	/**
	 * \brief Create bias matrix which multiplies all coordinates by 0.5 and offsets them by 0.5.
	 */
	static decDMatrix CreateBiasMatrix();
	
	/**
	 * \brief Create rotation-translation matrix.
	 * 
	 * This is the same as first doing a 3-axis rotation and then a translation.
	 * 
	 * \param rot Rotation vector
	 * \param pos Translation vector
	 */
	static decDMatrix CreateRT( const decDVector &rot, const decDVector &pos );
	
	/**
	 * \brief Create scale-translation matrix.
	 * 
	 * This is the same as first doing a scaling and then a translation.
	 */
	static decDMatrix CreateST( const decDVector &scale, const decDVector &translate );
	
	/**
	 * \brief Create scale-rotation-translation matrix.
	 * 
	 * This is the same as first doing a scaling, then a 3-axis rotation and then a translation.
	 * 
	 * \param scale Scaling vector
	 * \param rot Rotation vector
	 * \param pos Translation vector
	 */
	static decDMatrix CreateSRT( const decDVector &scale, const decDVector &rot,
		const decDVector &pos );
	
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
	static decDMatrix CreateSVUT( const decDVector &scale, const decDVector &view,
		const decDVector &up, const decDVector &pos );
	
	/**
	 * \brief Create view-up matrix.
	 * 
	 * This is the same as doing a world space matrix but with the zero vector as position.
	 * 
	 * \param view View vector
	 * \param up Up vector
	 */
	static decDMatrix CreateVU( const decDVector &view, const decDVector &up );
	
	/**
	 * \brief Create rotation matrix from a quaternion.
	 * \param q Rotation quaternion
	 */
	static decDMatrix CreateFromQuaternion( const decQuaternion &q );
	
	/**
	 * \brief Create rotation matrix from a quaternion.
	 */
	static decDMatrix CreateFromQuaternion( const decQuaternion &q, const decDVector &pivot );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Sets identity matrix. */
	void SetIdentity();
	
	/** \brief Sets translation matrix. */
	void SetTranslation( double x, double y, double z );
	
	/** \brief Sets translation matrix. */
	void SetTranslation( const decDVector &t );
	
	/** \brief Sets scaling matrix. */
	void SetScale( double sx, double sy, double sz );
	
	/** \brief Sets scaling matrix. */
	void SetScale( const decDVector &s );
	
	/** \brief Sets rotation around the x axis matrix. */
	void SetRotationX( double arc );
	
	/** \brief Sets rotation around the y axis matrix. */
	void SetRotationY( double arc );
	
	/** \brief Sets rotation around the z axis matrix. */
	void SetRotationZ( double arc );
	
	/**
	 * \brief Set rotation around the all three axes matrix.
	 * 
	 * This matrix is the same as first applying a z axis roation,
	 * then an x axis rotation and then an y axis rotation.
	 */
	void SetRotation( double rx, double ry, double rz );
	
	/**
	 * \brief Set rotation around the all three axes matrix.
	 * 
	 * This matrix is the same as first applying a z axis roation,
	 * then an x axis rotation and then an y axis rotation.
	 */
	void SetRotation( const decDVector &r );
	
	/** \brief Sets rotation around an arbitrary axis matrix. */
	void SetRotationAxis( const decDVector &axis, double angle );
	
	/**
	 * \brief Set world space matrix.
	 * 
	 * A world matrix is used to transform coordinates from the coordinate system of
	 * * one object to another one.
	 * 
	 * \param pos Position of object
	 * \param view View vector of object not necessary normalized
	 * \param up Up vector of object not necessary normalized
	 */
	void SetWorld( const decDVector &pos, const decDVector &view, const decDVector &up );
	
	/**
	 * \brief Set world space matrix.
	 * 
	 * A world matrix is used to transform coordinates from the coordinate system of
	 * one object to another one.
	 * 
	 * \param pos Position of object
	 * \param orientation Orientation
	 */
	void SetWorld( const decDVector &pos, const decQuaternion &orientation );
	
	/**
	 * \brief Set world space matrix.
	 * 
	 * A world matrix is used to transform coordinates from the object local
	 * coordinate system to the world coordinate system.
	 */
	void SetWorld( const decDVector &pos, const decQuaternion &orientation, const decDVector &scale );
	
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
	void SetCamera( const decDVector &pos, const decDVector &view, const decDVector &up );
	
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
	void SetProjection( int width, int height, double fov, double fovRatio, double znear, double zfar );
	
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
	void SetProjectionInfinite( int width, int height, double fov, double fovRatio, double znear );
	
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
	void SetRT( const decDVector &rot, const decDVector &pos );
	
	/**
	 * \brief Set scale-translation matrix.
	 * 
	 * This is the same as first doing a scaling and then a translation.
	 */
	void SetST( const decDVector &scale, const decDVector &translate );
	
	/**
	 * \brief Set scale-rotation-translation matrix.
	 * 
	 * This is the same as first doing a scaling, then a 3-axis rotation and then a translation.
	 * 
	 * \param scale Scaling vector
	 * \param rot Rotation vector
	 * \param pos Translation vector
	 */
	void SetSRT( const decDVector &scale, const decDVector &rot, const decDVector &pos );
	
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
	void SetSVUT( const decDVector &scale, const decDVector &view, const decDVector &up,
		const decDVector &pos );
	
	/**
	 * \brief Set view-up matrix.
	 * 
	 * This is the same as doing a world space matrix but with the zero vector as position.
	 * 
	 * \param view View vector
	 * \param up Up vector
	 */
	void SetVU( const decDVector &view, const decDVector &up );
	
	/**
	 * \brief Set rotation matrix from a quaternion.
	 * \param q Rotation quaternion
	 */
	void SetFromQuaternion( const decQuaternion &q );
	
	/** \brief Set rotation matrix from a quaternion. */
	void SetFromQuaternion( const decQuaternion &q, const decDVector &pivot );
	
	/** \brief Set matrix from a single precision matrix. */
	void Set( const decMatrix &matrix );
	
	/** \brief Calculates the determinant of the matrix. */
	double Determinant() const;
	
	/** \brief Inverse of this matrix if the inverse is possible. */
	decDMatrix Invert() const;
	
	/** \brief Transpose of this matrix. */
	decDMatrix Transpose() const;
	
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
	decDVector GetEulerAngles() const;
	
	/**
	 * \brief Convert matrix into a rotation quaternion.
	 * 
	 * This conversion is well defined in contrast to the rotation vector function.
	 */
	decQuaternion ToQuaternion() const;
	
	/**
	 * \brief Scaling of the matrix.
	 * 
	 * This works only if the matrix is the product of translations, rotations and scaling.
	 * The returned scaling is always positive.
	 */
	decDVector GetScale() const;
	
	/**
	 * \brief Position of the matrix.
	 * 
	 * This is the same as transforming the vector (0,0,0) by this matrix but faster.
	 */
	decDVector GetPosition() const;
	
	/**
	 * \brief View vector of this matrix.
	 * 
	 * This is the same as transforing the vector (0,0,1) by this matrix but faster.
	 */
	decDVector TransformView() const;
	
	/**
	 * \brief Right vector of this matrix.
	 * 
	 * This is the same as transforing the vector (1,0,0) by this matrix but faster.
	 */
	decDVector TransformRight() const;
	
	/**
	 * \brief Up vector of this matrix.
	 * 
	 * This is the same as transforing the vector (0,1,0) by this matrix but faster.
	 */
	decDVector TransformUp() const;
	
	/**
	 * \brief Transform normal by this matrix.
	 * 
	 * This is the same as transforming the normal by only the rotation part of it.
	 */
	decDVector TransformNormal( const decDVector &normal ) const;
	
	/** \brief Transforms a vector. */
	decDVector Transform( double x, double y, double z ) const;
	
	/** \brief Transforms a vector. */
	decDVector4 Transform( double x, double y, double z, double w ) const;
	
	/** \brief Transforms a vector. */
	void Transform( decDVector &result, double x, double y, double z ) const;
	
	/** \brief Transforms a vector. */
	void Transform( decDVector4 &result, double x, double y, double z, double w ) const;
	
	/**
	 * \brief Rotation part of the matrix.
	 * 
	 * The result is a  4x4 identity matrix with the upper left 3x3 copied from this matrix.
	 */
	decDMatrix GetRotationMatrix() const;
	
	/**
	 * \brief Matrix is cell wise equal to anither one in respect to a threshold.
	 */
	bool IsEqualTo( const decDMatrix &matrix, double threshold = DVECTOR_THRESHOLD ) const;
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
	decDMatrix QuickMultiply( const decDMatrix &matrix ) const;
	
	/** \brief Quick multiplies this matrix with a value k. */
	decDMatrix QuickMultiply( double k ) const;
	
	/** \brief Quick calculates the determinant of this matrix. */
	double QuickDeterminant() const;
	
	/** \brief Quick calculates the inverse of this matrix. */
	decDMatrix QuickInvert() const;
	
	/** \brief Quick cell wise adds another matrix to this matrix. */
	decDMatrix &QuickAddTo( const decDMatrix &m );
	
	/** \brief Converts to a single precision matrix with possible loss of precision. */
	decMatrix ToMatrix() const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set cells of this matrix to the values of another one. */
	decDMatrix &operator=( const decDMatrix &m );
	
	/** \brief Adds the cells of another matrix to this one. */
	decDMatrix &operator+=( const decDMatrix &m );
	
	/** \brief Subtracts the cells of another matrix from this vector. */
	decDMatrix &operator-=( const decDMatrix &m );
	
	/** \brief Multiplies the cells of this matrix with a value k. */
	decDMatrix &operator*=( double k );
	
	/**
	 * \brief Divide cells of this matrix by a value k.
	 * 
	 * If the value k is 0 an exception is thrown.
	 */
	decDMatrix &operator/=( double k );
	
	/** \brief Multiplies this matrix with another one. */
	decDMatrix &operator*=( const decDMatrix &m );
	
	/** \brief Retrieves a new matrix with the sum of this matrix with another one. */
	decDMatrix operator+( const decDMatrix &m ) const;
	
	/** \brief Retrieves a new matrix with the difference of this matrix to another one. */
	decDMatrix operator-( const decDMatrix &m ) const;
	
	/** \brief Retrieves a new matrix with this matrix scaled by k. */
	decDMatrix operator*( double k ) const;
	
	/** \brief Retrieves a new matrix with this matrix divided by k. If k is 0 an exception is thrown. */
	decDMatrix operator/( double k ) const;
	
	/** \brief Retrieves a new matrix with this matrix multiplied with another one. */
	decDMatrix operator*( const decDMatrix &m ) const;
	
	/** \brief Transforms vector by this matrix. */
	decDVector operator*( const decDVector &v ) const;
	
	/** \brief Transforms 4 component vector by this matrix. */
	decDVector4 operator*( const decDVector4 &v ) const;
	/*@}*/
};
