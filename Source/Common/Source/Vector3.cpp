#include <Vector3.hpp>
#include <Arithmetic.hpp>
#include <iostream>

namespace Aura
{
	Vector3::Vector3( ) :
		m_X( 0.0f ),
		m_Y( 0.0f ),
		m_Z( 0.0f )
	{
	}

	Vector3::~Vector3( )
	{
	}

	Vector3::Vector3( const Vector3 &p_Other ) :
		m_X( p_Other.m_X ),
		m_Y( p_Other.m_Y ),
		m_Z( p_Other.m_Z )
	{
	}

	Vector3::Vector3( const AUR_FLOAT32 p_X, const AUR_FLOAT32 p_Y,
		const AUR_FLOAT32 p_Z ) :
		m_X( p_X ),
		m_Y( p_Y ),
		m_Z( p_Z )
	{
	}

	Vector3 &Vector3::operator=( const Vector3 &p_Other )
	{
		m_X = p_Other.m_X;
		m_Y = p_Other.m_Y;
		m_Z = p_Other.m_Z;

		return *this;
	}

	void Vector3::Set( const AUR_FLOAT32 p_X, const AUR_FLOAT32 p_Y,
		const AUR_FLOAT32 p_Z )
	{
		m_X = p_X;
		m_Y = p_Y;
		m_Z = p_Z;
	}

	AUR_FLOAT32 Vector3::GetX( ) const
	{
		return m_X;
	}

	AUR_FLOAT32 Vector3::GetY( ) const
	{
		return m_Y;
	}

	AUR_FLOAT32 Vector3::GetZ( ) const
	{
		return m_Z;
	}

	void Vector3::SetX( const AUR_FLOAT32 p_X )
	{
		m_X = p_X;
	}

	void Vector3::SetY( const AUR_FLOAT32 p_Y )
	{
		m_Y = p_Y;
	}

	void Vector3::SetZ( const AUR_FLOAT32 p_Z )
	{
		m_Z = p_Z;
	}

	AUR_FLOAT32 Vector3::Magnitude( ) const
	{
		return SquareRoot( m_X * m_X + m_Y * m_Y + m_Z * m_Z );
	}

	AUR_FLOAT32 Vector3::MagnitudeSq( ) const
	{
		return ( m_X * m_X + m_Y * m_Y + m_Z * m_Z );
	}

	AUR_FLOAT32 Vector3::Distance( const Vector3 &p_Other ) const
	{
		AUR_FLOAT32 X = m_X - p_Other.m_X;
		AUR_FLOAT32 Y = m_Y - p_Other.m_Y;
		AUR_FLOAT32 Z = m_Z - p_Other.m_Z;

		return SquareRoot( X*X + Y*Y + Z*Z );
	}

	AUR_FLOAT32 Vector3::DistanceSq( const Vector3 &p_Other ) const
	{
		AUR_FLOAT32 X = m_X - p_Other.m_X;
		AUR_FLOAT32 Y = m_Y - p_Other.m_Y;
		AUR_FLOAT32 Z = m_Z - p_Other.m_Z;

		return ( X*X + Y*Y + Z*Z );
	}

	void Vector3::Normalise( )
	{
		AUR_FLOAT32 MagSq = this->MagnitudeSq( );

		if( Aura::IsZero( MagSq ) )
		{
			this->Zero( );
		}
		else
		{
			AUR_FLOAT32 Factor = InvSquareRoot( MagSq );

			m_X *= Factor;
			m_Y *= Factor;
			m_Z *= Factor;
		}
	}

	AUR_BOOL Vector3::IsZero( ) const
	{
		if( Aura::IsZero( m_X + m_Y + m_Z ) )
		{
			return AUR_TRUE;
		}

		return AUR_FALSE;
	}

	void Vector3::Zero( )
	{
		m_X = m_Y = m_Z = 0.0f;
	}

	void Vector3::Clean( )
	{
		if( Aura::IsZero( m_X ) )
		{
			m_X = 0.0f;
		}

		if( Aura::IsZero( m_Y ) )
		{
			m_Y = 0.0f;
		}

		if( Aura::IsZero( m_Z ) )
		{
			m_Z = 0.0f;
		}
	}

	AUR_FLOAT32 Vector3::Dot( const Vector3 &p_Other ) const
	{
		return ( m_X * p_Other.m_X + m_Y * p_Other.m_Y + m_Z * p_Other.m_Z );
	}

	Vector3 Vector3::Cross( const Vector3 &p_Other ) const
	{
		Vector3 Cross;

		Cross.m_X = ( m_Y * p_Other.m_Z ) - ( m_Z * p_Other.m_Y );
		Cross.m_Y = ( m_Z * p_Other.m_X ) - ( m_X * p_Other.m_Z );
		Cross.m_Z = ( m_X * p_Other.m_Y ) - ( m_Y * p_Other.m_X );

		return Cross;
	}

	void Vector3::Print( const std::string &p_Name ) const
	{
		std::cout << "Vector [" << p_Name << "]: < " << m_X << ", " << m_Y <<
			", " << m_Z << " >" << std::endl;
	}

	Vector3 &Vector3::operator-( )
	{
		m_X = -m_X;
		m_Y = -m_Y;
		m_Z = -m_Z;

		return *this;
	}

	Vector3 Vector3::operator-( const Vector3 &p_Other ) const
	{
		return Vector3( m_X - p_Other.m_X, m_Y - p_Other.m_Y,
			m_Z - p_Other.m_Z );
	}

	Vector3 &Vector3::operator+=( const Vector3 &p_Other )
	{
		m_X += p_Other.m_X;
		m_Y += p_Other.m_Y;
		m_Z += p_Other.m_Z;

		return *this;
	}
}

