#include <Arithmetic.hpp>
#include <cmath>

namespace Aura
{
	AUR_BOOL IsZero( AUR_FLOAT32 p_Value )
	{
		return ( fabsf( p_Value ) < Epsilon );
	}

	AUR_BOOL IsEqual( AUR_FLOAT32 p_Left, AUR_FLOAT32 p_Right )
	{
		return IsZero( p_Left - p_Right );
	}

	AUR_BOOL Absolute( AUR_FLOAT32 p_Value )
	{
		return fabsf( p_Value );
	}

	AUR_FLOAT32 SquareRoot( AUR_FLOAT32 p_Value )
	{
		return sqrtf( p_Value );
	}

	AUR_FLOAT32 InvSquareRoot( AUR_FLOAT32 p_Value )
	{
		return ( 1.0f / sqrtf( p_Value ) );
	}

	void SinCos( AUR_FLOAT32 p_Angle, AUR_FLOAT32 &p_Sine,
		AUR_FLOAT32 &p_Cosine )
	{
		p_Sine = sinf( p_Angle );
		p_Cosine = cosf( p_Angle );
	}
}

