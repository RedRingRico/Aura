#ifndef __AURA_REVERSERANGE_HPP__
#define __AURA_REVERSERANGE_HPP__

template < typename T >
struct reverse_range
{
private:
	T &x_;

public:
	reverse_range( T &p_X ) : x_( p_X ) { }
	auto begin( ) const -> decltype( this->x_.rbegin( ) )
	{
		return x_.rbegin( );
	}

	auto end( ) const -> decltype( this->x_.rend( ) )
	{
		return x_.rend( );
	}
};

template < typename T >
reverse_range< T > reverse_iterate( T &p_X )
{
	return reverse_range< T >( p_X );
}

#endif // __AURA_REVERSERANGE_HPP__

