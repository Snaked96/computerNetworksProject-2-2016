#ifndef _CRC16_HPP_
#define _CRC16_HPP_

    //Librer�as
    #include "Protocol.hpp"

    #define POLY bitset<5>(string("11000000000000101"))
    //10011

    template <const size_t MAX_CHAR_PER_MSG, const size_t TAM_TRAMA>
    class CRC16 : public Protocol< MAX_CHAR_PER_MSG, TAM_TRAMA >
    {
        public:
            void        encode( string &data );
            int         decode( string &data );
            string      remainder( const string &data );
    };


    template <const size_t MAX_CHAR_PER_MSG, const size_t TAM_TRAMA>
    void    CRC16<MAX_CHAR_PER_MSG, TAM_TRAMA>::encode( string &data )
    {
        data += remainder( data );
    }

    template <const size_t MAX_CHAR_PER_MSG, const size_t TAM_TRAMA>
    string    CRC16<MAX_CHAR_PER_MSG, TAM_TRAMA>::remainder( const string &data1 )
    {
        string data = data1;

        for( size_t i = 1; i < POLY.size() ; i++  )
            data += "0";

        for ( size_t i = 0; i < data.size()-POLY.size()+1 ; i++ )
            if( data[i] == '1' )
                data = data.substr( 0, i ) + ( bitset<5>( data.substr( i, POLY.size() ) ) ^ POLY ).to_string() + data.substr( i + POLY.size() );

        return( data.substr( data.size()- POLY.size() + 1 ) );
    }

    template <const size_t MAX_CHAR_PER_MSG, const size_t TAM_TRAMA>
    int    CRC16<MAX_CHAR_PER_MSG, TAM_TRAMA>::decode( string &data )
    {
        string theweeknd = remainder(data);

        for( size_t i = 0 ; i < theweeknd.size() ; i++)
            if( theweeknd[i] == '1' )
                return( 0 );

        data.erase( data.size() - POLY.size() + 1 );
        return( 1 );
    }

#endif // _CRC16_HPP_
