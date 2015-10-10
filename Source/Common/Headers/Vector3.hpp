#ifndef __AURA_VECTOR3_HPP__
#define __AURA_VECTOR3_HPP__

#include <DataTypes.hpp>
#include <string>

namespace Aura
{
	class Vector3
	{
	public:
		Vector3( );
		~Vector3( );
		Vector3( const Vector3 &p_Other );
		Vector3( const AUR_FLOAT32 p_X, const AUR_FLOAT32 p_Y,
			const AUR_FLOAT32 p_Z );
		Vector3 &operator=( const Vector3 &p_Other );

		void Set( const AUR_FLOAT32 p_X, const AUR_FLOAT32 p_Y,
			const AUR_FLOAT32 p_Z );

		AUR_FLOAT32 GetX( ) const;
		AUR_FLOAT32 GetY( ) const;
		AUR_FLOAT32 GetZ( ) const;

		void SetX( const AUR_FLOAT32 p_X );
		void SetY( const AUR_FLOAT32 p_Y );
		void SetZ( const AUR_FLOAT32 p_Z );

		AUR_FLOAT32 Magnitude( ) const;
		AUR_FLOAT32 MagnitudeSq( ) const;

		AUR_FLOAT32 Distance( const Vector3 &p_other ) const;
		AUR_FLOAT32 DistanceSq( const Vector3 &p_other ) const;

		void Normalise( );

		AUR_BOOL IsZero( ) const;
		void Zero( );
		void Clean( );

		AUR_FLOAT32 Dot( const Vector3 &p_Other ) const;
		Vector3 Cross( const Vector3 &p_Other ) const;

		void Print( const std::string &p_Name ) const;

		Vector3 &operator-( );

		Vector3 operator-( const Vector3 &p_Other ) const;

		Vector3 &operator+=( const Vector3 &p_Other );
		Vector3 &operator*=( const Vector3 &p_Other );
		Vector3 &operator*=( const AUR_FLOAT32 p_Scalar );

	private:
		AUR_FLOAT32 m_X;
		AUR_FLOAT32 m_Y;
		AUR_FLOAT32 m_Z;
	};
}

#endif // __AURA_VECTOR3_HPP__

