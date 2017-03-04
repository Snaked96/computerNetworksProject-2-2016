#ifndef _HAMMING_HPP_
#define _HAMMING_HPP_

    //Librerías
    #include "Protocol.hpp"

    template <const unsigned int MAX_CHAR_PER_MSG, const unsigned int TAM_TRAMA>
    class Hamming : public Protocol< MAX_CHAR_PER_MSG, TAM_TRAMA >
    {
        public:
            void     encode( string &data );
            int     decode( string &data );
    };


    template <const unsigned int MAX_CHAR_PER_MSG, const unsigned int TAM_TRAMA>
    void    Hamming<MAX_CHAR_PER_MSG, TAM_TRAMA>::encode( string &data )
    {
        int n_unos=0;

        for( unsigned long int i=1 ; i <= data.size() ; i*=2 )
            data.insert( i-1, "0" );

        for( unsigned long int i=1 ; i <= data.size() ; i++ )
            if( data[i-1] == '1' )
                for( unsigned long int j = 1 ; j <= data.size() ; j*=2 )
                    if( (bitset<11>(i) & bitset<11>(j)).any()  )
                        data[j-1] = ( data[j-1] == '1' ) ? '0' : '1';

         for( unsigned long int i=0 ; i <= data.size() ; i++ )
            if( data[i] == '1' )
                n_unos++;

         if( n_unos % 2 )
             data += "1";
         else
            data += "0";
    }

    template <const unsigned int MAX_CHAR_PER_MSG, const unsigned int TAM_TRAMA>
    int    Hamming<MAX_CHAR_PER_MSG, TAM_TRAMA>::decode( string &data1 )
    {
        string data = data1;
        bitset<20> a, b;
        int n_unos=0, pos_error, bit_ext, flag;

        bit_ext = ( data[data.size()-1] == '1' )? 1 : 0;
        data.erase( data.size()-1, 1 );

        for( unsigned long int i=1, j=0 ; i <= data.size() ; i*=2,j++ )
            a[j] = (data[i-1] == '1') ? 1 : 0;

        for( unsigned long int i=1 ; i <= data.size() ; i++ )
            if( data[i-1] == '1' )
            {
                n_unos++;
                for( unsigned long int j = 1 ; j <= data.size() ; j*=2 )
                    if( (bitset<11>(i) & bitset<11>(j)).any()  )
                        data[j-1] = ( data[j-1] == '1' ) ? '0' : '1';
            }

        for( unsigned long int i=1, j=0 ; i <= data.size() ; i*=2,j++ )
            b[j] = (data[i-1] == '1') ? 1 : 0;

        a ^= b;
        bit_ext = bit_ext == n_unos % 2;
        flag = 1;

        if( a.any() && !bit_ext )
        {
            pos_error = a.to_ulong() - 1;
            data[pos_error] = ( data[pos_error] == '0' ) ? '1' : '0';
            flag = 2;
        }
        else if( a.any() )
            flag = 0;

        for( unsigned long int i=1, j=0 ; i-j-1 < data.size() ; i*=2,j++ )
            data.erase( i-j-1, 1 );

        data1 = data;

        return( flag );
    }

#endif // _HAMMING_HPP_
