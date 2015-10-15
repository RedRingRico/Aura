#ifndef __AURA_CAMERA_HPP__
#define __AURA_CAMERA_HPP__

#include <DataTypes.hpp>
#include <Matrix4x4.hpp>
#include <Vector3.hpp>

namespace Aura
{
	enum PROJECTIONMODE
	{
		PROJECTIONMODE_SCREEN = 0,
		PROJECTIONMODE_ORTHOGRAPHIC,
		PROJECTIONMODE_PERSPECTIVE
	};

	class Camera
	{
	public:
		Camera( );
		~Camera( );
		Camera( const Camera &p_Other );
		Camera &operator=( const Camera &p_Other );

		void SetPosition( const AUR_FLOAT32 p_X, const AUR_FLOAT32 p_Y,
			const AUR_FLOAT32 p_Z );
		void SetPosition( const Vector3 &p_Position );
		void GetPosition( Vector3 &p_Position ) const;

		void SetDimensions( const AUR_FLOAT32 p_Width,
			const AUR_FLOAT32 p_Height );
		void GetDimensions( AUR_FLOAT32 &p_Width,
			AUR_FLOAT32 &p_Height ) const;

		void SetLookPoint( const AUR_FLOAT32 p_X, const AUR_FLOAT32 p_Y,
			const AUR_FLOAT32 p_Z );
		void SetLookPoint( const Vector3 &p_LookPoint );
		void GetLookPoint( Vector3 &p_LookPOint ) const;

		void SetWorldUp( const AUR_FLOAT32 p_X, const AUR_FLOAT32 p_Y,
			const AUR_FLOAT32 p_Z );
		void SetWorldUp( const Vector3 &p_WorldUp );
		void GetWorldUp( Vector3 &p_WorldUp ) const;

		void SetClippingPlanes( const AUR_FLOAT32 p_Near,
			const AUR_FLOAT32 p_Far );
		void GetClippingPlanes( AUR_FLOAT32 &p_Near,
			AUR_FLOAT32 &p_Far ) const;

		void SetProjectionMode( const PROJECTIONMODE p_ProjectionMode );
		PROJECTIONMODE GetProjectionMode( ) const;

		void SetAspectRatio( const AUR_FLOAT32 p_AspectRatio );
		AUR_FLOAT32 GetAspectRatio( ) const;

		void SetFieldOfView( const AUR_FLOAT32 p_FieldOfView );
		AUR_FLOAT32 GetFieldOfView( ) const;

		void GetProjectionMatrix( Matrix4x4 &p_Projection ) const;
		void GetViewMatrix( Matrix4x4 &p_View ) const;

		AUR_UINT32 CalculateProjectionMatrix( );
		AUR_UINT32 CalculateViewMatrix( );

	private:
		Matrix4x4		m_View;
		Matrix4x4		m_Projection;
		Vector3			m_Position;
		Vector3			m_LookPoint;
		Vector3			m_WorldUp;

		AUR_FLOAT32		m_Near;
		AUR_FLOAT32		m_Far;
		AUR_FLOAT32		m_AspectRatio;
		AUR_FLOAT32		m_FieldOfView;
		AUR_FLOAT32		m_Width;
		AUR_FLOAT32		m_Height;

		PROJECTIONMODE	m_ProjectionMode;
	};
}

#endif // __AURA_CAMERA_HPP__

