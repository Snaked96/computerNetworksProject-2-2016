#ifndef _CRC16_HPP_
#define _CRC16_HPP_

    //Librerías
    #include "Protocol.hpp"

    template <const unsigned int MAX_CHAR_PER_MSG, const unsigned int TAM_TRAMA>
    class CRC16 : public Protocol< MAX_CHAR_PER_MSG, TAM_TRAMA >
    {
        void     encode( string &data );
    };


    template <const unsigned int MAX_CHAR_PER_MSG, const unsigned int TAM_TRAMA>
    void    CRC16<MAX_CHAR_PER_MSG, TAM_TRAMA>::encode( string &data )
    {

    }

#endif // _CRC16_HPP_
