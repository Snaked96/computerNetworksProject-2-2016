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

            void    send( string nf_in, string nf_out );    //Codifica el mensaje segun lo descrito en el protocolo
                                                            // y lo escribe en el archivo f_name

            void    receive( string nf_in, string nf_out );

            //Emisor
            void     toBin( string &data );                 //Convierte data en binario
            void     insertStuff( string &data );           //Relleno de bits
            void     insertFlags( string &data );           //Inserta  las Banderas
            void     insertHead( string &data, int ce, int t );//Inserta la cabecera

            virtual void     encode( string &data ){};

            //Receptor

            void    backToASCII( string &data );

    };

    template <const unsigned int MAX_CHAR_PER_MSG, const unsigned int TAM_TRAMA>
    void    Protocol<MAX_CHAR_PER_MSG, TAM_TRAMA>::send( string nf_in, string nf_out )
    {
        const unsigned int CHAR_PER_TRAMA = TAM_TRAMA/8;
        const unsigned int N_TRAMAS = MAX_CHAR_PER_MSG/CHAR_PER_TRAMA;
        string trama;
        string msg;
        char   msg_char[2500];
        ifstream f_in( nf_in.c_str() );
        ofstream f_out( nf_out.c_str() );

        f_in.getline( msg_char, MAX_CHAR_PER_MSG + 1 );
        f_in.close();

        msg = msg_char;

        for( int i = 0, pos = 0 ; i < N_TRAMAS ; i+=100,pos+=CHAR_PER_TRAMA )
        {
            trama = msg.substr( pos, ( pos + CHAR_PER_TRAMA < msg.size() ) ? CHAR_PER_TRAMA : msg.size() - pos );
            toBin( trama );
            insertHead( trama, i, (i < N_TRAMAS-1) ? 0 : 1 );
            encode( trama );
            insertStuff( trama );
            insertFlags( trama );
            f_out << trama << endl;
        }
        f_out.close();
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
        data.insert( 0, "01111110" ).append( "01111110" );
    }

    template <const unsigned int MAX_CHAR_PER_MSG, const unsigned int TAM_TRAMA>
    void    Protocol<MAX_CHAR_PER_MSG, TAM_TRAMA>::insertHead( string &data, int ce, int t )
    {
        data = "0"+bitset<4>(ce).to_string()+bitset<1>(t).to_string()+data;
    }

#endif // _PROTOCOL_HPP_
