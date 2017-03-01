#ifndef _HAMMING_HPP_
#define _HAMMING_HPP_

    //Librerías
    #include "Protocol.hpp"

    template <const unsigned int MAX_CHAR_PER_MSG, const unsigned int TAM_TRAMA>
    class Hamming : public Protocol< MAX_CHAR_PER_MSG, TAM_TRAMA >
    {
        void     encode( string &data );
    };


    template <const unsigned int MAX_CHAR_PER_MSG, const unsigned int TAM_TRAMA>
    void    Hamming<MAX_CHAR_PER_MSG, TAM_TRAMA>::encode( string &data )
    {

    }

#endif // _HAMMING_HPP_
