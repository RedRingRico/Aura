#ifndef __AURA_MATRIX4X4_HPP__
#define __AURA_MATRIX4X4_HPP__

#include <DataTypes.hpp>

namespace Aura
{
	class Matrix4x4
	{
	public:
		Matrix4x4( );
		~Matrix4x4( );
		Matrix4x4( const Matrix4x4 &p_Other );
		Matrix4x4 &operator=( const Matrix4x4 &p_Other );

		void Identity( );

		Matrix4x4 &RotateY( const AUR_FLOAT32 p_Y );

		void AsArray( AUR_FLOAT32 *p_pMatrix ) const;

		Matrix4x4 operator*( const Matrix4x4 &p_Other ) const;

		// Row-column manipulator
		inline AUR_FLOAT32 &operator( )( const AUR_MEMSIZE p_Row,
			const AUR_MEMSIZE p_Column )
			{ return m_M[ p_Row + ( p_Column * 4 ) ]; }

		// Row-column accessor
		inline AUR_FLOAT32 operator( )( const AUR_MEMSIZE p_Row,
			AUR_MEMSIZE p_Column ) const
			{ return m_M[ p_Row + ( p_Column * 4 ) ]; }

	private:
		// Column-major
		// 0 4  8 12
		// 1 5  9 13
		// 2 6 10 14
		// 3 7 11 15
		AUR_FLOAT32 m_M[ 16 ];
	};
}

#endif // __AURA_MATRIX4X4_HPP__

