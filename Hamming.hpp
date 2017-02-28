#ifndef _HAMMING_HPP_
#define _HAMMING_HPP_

    //Librerías
    #include "Protocol.hpp"

    template <const unsigned int MAX_CHAR_PER_MSG, const unsigned int TAM_TRAMA>
    class Hamming : public Protocol< MAX_CHAR_PER_MSG, TAM_TRAMA >
    {
        public:
            void     encode( string &data );
    };


    template <const unsigned int MAX_CHAR_PER_MSG, const unsigned int TAM_TRAMA>
    void    Hamming<MAX_CHAR_PER_MSG, TAM_TRAMA>::encode( string &data )
    {
        for( unsigned long int i=1 ; i <= data.size() ; i*=2 )
        {
            data.insert( i-1, "0" );
        }

        for( unsigned long int i=1 ; i <= data.size() ; i++ )
        {
            if( data[i-1] == '1' )
            {
                for( unsigned long int j = 1 ; j <= data.size() ; j*=2 )
                {
                    if( (bitset<11>(i) & bitset<11>(j)).any()  )
                    {
                        data[j-1] = ( data[j-1] == '1' ) ? '0' : '1';
                    }
                }
            }
        }
    }

#endif // _HAMMING_HPP_
