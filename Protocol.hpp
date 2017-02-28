#ifndef _PROTOCOL_HPP_
#define _PROTOCOL_HPP_

    //Librerìas
    #include <iostream>
    #include <fstream>
    #include <bitset>
    #include <cmath>

    using namespace std;

    //Contantes

    template <const unsigned int MAX_CHAR_PER_MSG, const unsigned int TAM_TRAMA>
    class Protocol
    {
        public:
            static void     send( string msg, string f_name );     //Codifica el mensaje segun lo descrito en el protocolo
                                                                // y lo escribe en el archivo f_name
       // private:

            static void     toBin( string &data );               //Convierte data en binario
            static void     insertStuff( string &data );         //Relleno de bits
            static void     insertFlags( string &data );         //Inserta  las Banderas
            static void     insertHead( string &data, int ce, int t );//Inserta la cabecera

            static void     encode( string &data ){};
    };

    template <const unsigned int MAX_CHAR_PER_MSG, const unsigned int TAM_TRAMA>
    void    Protocol<MAX_CHAR_PER_MSG, TAM_TRAMA>::send( string msg, string f_name )
    {
        const unsigned int CHAR_PER_TRAMA = TAM_TRAMA/8;
        const unsigned int N_TRAMAS = MAX_CHAR_PER_MSG/CHAR_PER_TRAMA;
        string tramas[N_TRAMAS];

        for( int i = 0, pos = 0 ; i < N_TRAMAS ; i++,pos+=CHAR_PER_TRAMA )
        {
            tramas[i] = msg.substr( pos, CHAR_PER_TRAMA );
            toBin( tramas[i] );
            insertHead( tramas[i], i, (i < N_TRAMAS-1) ? 0 : 1 );
            encode( tramas[i] );
            insertStuff( tramas[i] );
            insertFlags( tramas[i] );
        }


    }

    template <const unsigned int MAX_CHAR_PER_MSG, const unsigned int TAM_TRAMA>
    void    Protocol<MAX_CHAR_PER_MSG, TAM_TRAMA>::toBin( string &data )
    {
        string bin("");

        for( int i=0 ; i < data.size() ; i++ )
        {
            bin += bitset<8>(data[i]).to_string();
        }

        data = bin;
    }

    template <const unsigned int MAX_CHAR_PER_MSG, const unsigned int TAM_TRAMA>
    void    Protocol<MAX_CHAR_PER_MSG, TAM_TRAMA>::insertStuff( string &data )
    {
        unsigned int n_unos(0);
        string data_aux("");

        for( int i=0 ; i < data.size() ; i++ )
        {
            if( data[i] == '1' )
            {
                if( n_unos == 5 )
                {
                    data_aux += "0";
                    n_unos = 1;
                }
                else
                {
                    n_unos++;
                }
            }
            else
            {
                n_unos = 0;
            }

            data_aux += data[i];
        }
        data = data_aux;
    }

    template <const unsigned int MAX_CHAR_PER_MSG, const unsigned int TAM_TRAMA>
    void    Protocol<MAX_CHAR_PER_MSG, TAM_TRAMA>::insertFlags( string &data )
    {
        data = "01111110" + data + "01111110";
    }

    template <const unsigned int MAX_CHAR_PER_MSG, const unsigned int TAM_TRAMA>
    void    Protocol<MAX_CHAR_PER_MSG, TAM_TRAMA>::insertHead( string &data, int ce, int t )
    {
        data = "0"+bitset<4>(ce).to_string()+bitset<1>(t).to_string()+data;
    }

#endif // _PROTOCOL_HPP_
