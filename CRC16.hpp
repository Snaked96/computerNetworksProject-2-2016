#ifndef _CRC16_HPP_
#define _CRC16_HPP_

    //Librerías
    #include "Protocol.hpp"

    #define POLY bitset<5>(string("10011"))
    //11000000000000101

    template <const unsigned int MAX_CHAR_PER_MSG, const unsigned int TAM_TRAMA>
    class CRC16 : public Protocol< MAX_CHAR_PER_MSG, TAM_TRAMA >
    {
        public:
            void        encode( string &data );
            bool        decode( string &data );
            string      remainder( const string &data );
    };


    template <const unsigned int MAX_CHAR_PER_MSG, const unsigned int TAM_TRAMA>
    void    CRC16<MAX_CHAR_PER_MSG, TAM_TRAMA>::encode( string &data )
    {
        data += remainder( data );
    }

    template <const unsigned int MAX_CHAR_PER_MSG, const unsigned int TAM_TRAMA>
    string    CRC16<MAX_CHAR_PER_MSG, TAM_TRAMA>::remainder( const string &data1 )
    {
        string data = data1;

        for( unsigned int i = 1; i < POLY.size() ; i++  )
            data += "0";

        for ( unsigned int i = 0; i < data.size()-POLY.size()+1 ; i++ )
            if( data[i] == '1' )
                data = data.substr( 0, i ) + ( bitset<5>( data.substr( i, POLY.size() ) ) ^ POLY ).to_string() + data.substr( i + POLY.size() );

        return( data.substr( data.size()- POLY.size() + 1 ) );
    }

    template <const unsigned int MAX_CHAR_PER_MSG, const unsigned int TAM_TRAMA>
    bool    CRC16<MAX_CHAR_PER_MSG, TAM_TRAMA>::decode( string &data )
    {
        string theweeknd = remainder(data);

        for( unsigned int i = 0 ; i < theweeknd.size() ; i++)
            if( theweeknd[i] == '1' )
                return( 0 );

        data.erase( data.size() - POLY.size() + 1 );
        return( 1 );
    }

#endif // _CRC16_HPP_
