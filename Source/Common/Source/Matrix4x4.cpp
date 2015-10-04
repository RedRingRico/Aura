#include <Matrix4x4.hpp>
#include <cstring>

namespace Aura
{
	Matrix4x4::Matrix4x4( )
	{
		this->Identity( );
	}

	Matrix4x4::~Matrix4x4( )
	{
	}

	Matrix4x4::Matrix4x4( const Matrix4x4 &p_Other )
	{
		memcpy( m_M, p_Other.m_M, sizeof( m_M ) );
	}

	Matrix4x4 &Matrix4x4::operator=( const Matrix4x4 &p_Other )
	{
		memcpy( m_M, p_Other.m_M, sizeof( m_M ) );

		return *this;
	}

	void Matrix4x4::Identity( )
	{
		m_M[ 0 ] = m_M[ 5 ] = m_M[ 10 ] = m_M[ 15 ] = 1.0f;

		m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = m_M[ 4 ] =
			m_M[ 6 ] = m_M[ 7 ] = m_M[ 8 ] = m_M[ 9 ] =
			m_M[ 11 ] = m_M[ 12 ] = m_M[ 13 ] = m_M[ 14 ] = 0.0f;
	}

	void Matrix4x4::AsArray( AUR_FLOAT32 *p_pMatrix ) const
	{
		memcpy( p_pMatrix, m_M, sizeof( m_M ) );
	}
}

