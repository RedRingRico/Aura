#include <Matrix4x4.hpp>
#include <Arithmetic.hpp>
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

	Matrix4x4 &Matrix4x4::RotateY( const AUR_FLOAT32 p_Y )
	{
		AUR_FLOAT32 Sine, Cosine;
		Aura::SinCos( p_Y, Sine, Cosine );

		m_M[ 0 ] = Cosine;
		m_M[ 1 ] = 0.0f;
		m_M[ 2 ] = Sine;
		m_M[ 3 ] = 0.0f;

		m_M[ 4 ] = 0.0f;
		m_M[ 5 ] = 1.0f;
		m_M[ 6 ] = 0.0f;
		m_M[ 7 ] = 0.0f;

		m_M[ 8 ] = -Sine;
		m_M[ 9 ] = 0.0f;
		m_M[ 10 ] = Cosine;
		m_M[ 11 ] = 0.0f;

		m_M[ 12 ] = 0.0f;
		m_M[ 13 ] = 0.0f;
		m_M[ 14 ] = 0.0f;
		m_M[ 15 ] = 1.0f;

		return *this;
	}

	void Matrix4x4::AsArray( AUR_FLOAT32 *p_pMatrix ) const
	{
		memcpy( p_pMatrix, m_M, sizeof( m_M ) );
	}

	Matrix4x4 Matrix4x4::operator*( const Matrix4x4 &p_Other ) const
	{
		Matrix4x4 Multiply;

		Multiply.m_M[ 0 ] =	m_M[ 0 ] * p_Other.m_M[ 0 ] +
							m_M[ 4 ] * p_Other.m_M[ 1 ] +
							m_M[ 8 ] * p_Other.m_M[ 2 ] +
							m_M[ 12 ] * p_Other.m_M[ 3 ];
		Multiply.m_M[ 1 ] =	m_M[ 1 ] * p_Other.m_M[ 0 ] +
							m_M[ 5 ] * p_Other.m_M[ 1 ] +
							m_M[ 9 ] * p_Other.m_M[ 2 ] +
							m_M[ 13 ] * p_Other.m_M[ 3 ];
		Multiply.m_M[ 2 ] =	m_M[ 2 ] * p_Other.m_M[ 0 ] +
							m_M[ 6 ] * p_Other.m_M[ 1 ] +
							m_M[ 10 ] * p_Other.m_M[ 2 ] +
							m_M[ 14 ] * p_Other.m_M[ 3 ];
		Multiply.m_M[ 3 ] = m_M[ 3 ] * p_Other.m_M[ 0 ] +
							m_M[ 7 ] * p_Other.m_M[ 1 ] +
							m_M[ 11 ] * p_Other.m_M[ 2 ] +
							m_M[ 15 ] * p_Other.m_M[ 3 ];

		Multiply.m_M[ 4 ] =	m_M[ 0 ] * p_Other.m_M[ 4 ] +
							m_M[ 4 ] * p_Other.m_M[ 5 ] +
							m_M[ 8 ] * p_Other.m_M[ 6 ] +
							m_M[ 12 ] * p_Other.m_M[ 7 ];
		Multiply.m_M[ 5 ] = m_M[ 1 ] * p_Other.m_M[ 4 ] +
							m_M[ 5 ] * p_Other.m_M[ 5 ] +
							m_M[ 9 ] * p_Other.m_M[ 6 ] +
							m_M[ 13 ] * p_Other.m_M[ 7 ];
		Multiply.m_M[ 6 ] = m_M[ 2 ] * p_Other.m_M[ 4 ] +
							m_M[ 6 ] * p_Other.m_M[ 5 ] +
							m_M[ 10 ] * p_Other.m_M[ 6 ] +
							m_M[ 14 ] * p_Other.m_M[ 7 ];
		Multiply.m_M[ 7 ] = m_M[ 3 ] * p_Other.m_M[ 4 ] +
							m_M[ 7 ] * p_Other.m_M[ 5 ] +
							m_M[ 11 ] * p_Other.m_M[ 6 ] +
							m_M[ 15 ] * p_Other.m_M[ 7 ];

		Multiply.m_M[ 8 ] =		m_M[ 0 ] * p_Other.m_M[ 8 ] +
								m_M[ 4 ] * p_Other.m_M[ 9 ] +
								m_M[ 8 ] * p_Other.m_M[ 10 ] +
								m_M[ 12 ] * p_Other.m_M[ 11 ];
		Multiply.m_M[ 9 ] = 	m_M[ 1 ] * p_Other.m_M[ 8 ] +
								m_M[ 5 ] * p_Other.m_M[ 9 ] +
								m_M[ 9 ] * p_Other.m_M[ 10 ] +
								m_M[ 13 ] * p_Other.m_M[ 11 ];
		Multiply.m_M[ 10 ] =	m_M[ 2 ] * p_Other.m_M[ 8 ] +
								m_M[ 6 ] * p_Other.m_M[ 9 ] +
								m_M[ 10 ] * p_Other.m_M[ 10 ] +
								m_M[ 14 ] * p_Other.m_M[ 11 ];
		Multiply.m_M[ 11 ] =	m_M[ 3 ] * p_Other.m_M[ 8 ] +
								m_M[ 7 ] * p_Other.m_M[ 9 ] +
								m_M[ 11 ] * p_Other.m_M[ 10 ] +
								m_M[ 15 ] * p_Other.m_M[ 11 ];

		Multiply.m_M[ 12 ] =	m_M[ 0 ] * p_Other.m_M[ 12 ] +
								m_M[ 4 ] * p_Other.m_M[ 13 ] +
								m_M[ 8 ] * p_Other.m_M[ 14 ] +
								m_M[ 12 ] * p_Other.m_M[ 15 ];
		Multiply.m_M[ 13 ] =	m_M[ 1 ] * p_Other.m_M[ 12 ] +
								m_M[ 5 ] * p_Other.m_M[ 13 ] +
								m_M[ 9 ] * p_Other.m_M[ 14 ] +
								m_M[ 13 ] * p_Other.m_M[ 15 ];
		Multiply.m_M[ 14 ] =	m_M[ 2 ] * p_Other.m_M[ 12 ] +
								m_M[ 6 ] * p_Other.m_M[ 13 ] +
								m_M[ 10 ] * p_Other.m_M[ 14 ] +
								m_M[ 14 ] * p_Other.m_M[ 15 ];
		Multiply.m_M[ 15 ] =	m_M[ 3 ] * p_Other.m_M[ 12 ] +
								m_M[ 7 ] * p_Other.m_M[ 13 ] +
								m_M[ 11 ] * p_Other.m_M[ 14 ] +
								m_M[ 15 ] * p_Other.m_M[ 15 ];

		return Multiply;
	}
}

