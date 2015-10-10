#ifndef __AURA_ARITHMETIC_HPP__
#define __AURA_ARITHMETIC_HPP__

#include <DataTypes.hpp>

namespace Aura
{
	const AUR_FLOAT32 Epsilon = 1.0e-10f;
	const AUR_FLOAT32 HalfEpsilon = 1.0e-5f;
	const AUR_FLOAT32 Pi = 3.1415926535897932384626433832975f;
	const AUR_FLOAT32 PiOver360 = Pi / 360.0f;
	const AUR_FLOAT32 Float32Infinite =
		static_cast< AUR_FLOAT32 >( 0x7F800000 );

	AUR_BOOL IsZero( AUR_FLOAT32 p_Value );
	AUR_BOOL IsEqual( AUR_FLOAT32 p_Left, AUR_FLOAT32 p_Right );
	AUR_FLOAT32 Absolute( AUR_FLOAT32 p_Value );
	AUR_FLOAT32 SquareRoot( AUR_FLOAT32 p_Value );
	AUR_FLOAT32 InvSquareRoot( AUR_FLOAT32 p_Value );
	void SinCos( AUR_FLOAT32 p_Angle, AUR_FLOAT32 &p_Sine,
		AUR_FLOAT32 &p_Cosine );
}

#endif // __AURA_ARITHMETIC_HPP__

