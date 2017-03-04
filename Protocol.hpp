#ifndef _PROTOCOL_HPP_
#define _PROTOCOL_HPP_

    //Librerìas
    #include <iostream>
    #include <fstream>
    #include <bitset>
    #include <cmath>
    #include <set>

    using namespace std;

    //Contantes
    #define FLAG "01111110"

    template <const unsigned int MAX_CHAR_PER_MSG, const unsigned int TAM_TRAMA>
    class Protocol
    {
       public:

            set<int>    tramas_recibidas;

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
            bool    extractStuff( string &data );
            bool    extractFlags( string &data );
            int     extractHead( string &data );

            virtual int    decode( string &data ){};


    };

    template <const unsigned int MAX_CHAR_PER_MSG, const unsigned int TAM_TRAMA>
    void    Protocol<MAX_CHAR_PER_MSG, TAM_TRAMA>::send( string nf_in, string nf_out )
    {
        unsigned int CHAR_PER_TRAMA = TAM_TRAMA/8;
        unsigned int N_TRAMAS = MAX_CHAR_PER_MSG/CHAR_PER_TRAMA;
        string trama;
        string msg;
        char   msg_char[2500];
        ifstream f_in( nf_in.c_str() );
        ofstream f_out( nf_out.c_str() );

        f_in.getline( msg_char, MAX_CHAR_PER_MSG + 1 );
        f_in.close();

        msg = msg_char;

        for( int i = 0, pos = 0 ; i < N_TRAMAS && pos < msg.size()  ; i++,pos+=CHAR_PER_TRAMA )
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
    void    Protocol<MAX_CHAR_PER_MSG, TAM_TRAMA>::receive( string nf_in, string nf_out )
    {
        const unsigned int CHAR_PER_TRAMA = TAM_TRAMA/8;
        const unsigned int N_TRAMAS = MAX_CHAR_PER_MSG/CHAR_PER_TRAMA;
        string trama;
        string msg;
        char   msg_char[10000];
        ifstream f_in( nf_in.c_str() );
        ofstream f_out( nf_out.c_str() );
        int flag_hamming=0, flag_head;
        bool flag=-1, auxflags, auxstuff;

        for( int i = 0; i < N_TRAMAS ; i++ )
        {
            f_in.getline( msg_char, 9000 );
            trama = msg_char;
            if( trama.size() )
            {
                auxflags = extractFlags( trama );
                auxstuff = extractStuff( trama );
                flag_hamming = max( flag_hamming, decode( trama ));
                flag_head = extractHead( trama );
                //cout << endl << i << " flag_hamming = " << flag_hamming << "  flag_head = " <<  flag_head << " auxflags = " << auxflags << " auxstuff = " << auxstuff << endl;
                if( auxflags && auxstuff && flag_hamming && flag_head )
                {
                    backToASCII( trama );
                    f_out << trama;
                    if( flag_head == 2 )
                        i = N_TRAMAS;
                }
                else
                {
                    flag = 0;
                    i = N_TRAMAS;
                }
            }
            else if( i == 0 )
            {
                f_out << "No se recibio ningun mensaje.";
                i = N_TRAMAS;
            }
        }

        if( flag )
        {
            if( flag_hamming == 2 )
            {
                cout << endl << "Se ha detectado algun error.hola" << endl;
                f_out << endl << "Se ha detectado algun error." << endl;
            }
        }
        else
        {
            cout << endl << "Se ha detectado algun error.chao" << endl;
            f_out << endl << "Se ha detectado algun error." << endl;
        }

        f_in.close();
        f_out.close();
    }

    template <const unsigned int MAX_CHAR_PER_MSG, const unsigned int TAM_TRAMA>
    void    Protocol<MAX_CHAR_PER_MSG, TAM_TRAMA>::backToASCII( string &data )
    {
        string aux("");
        for( int pos = 0 ; pos < data.size()  ; pos+=8 )
            aux += (char) bitset<8>(data.substr( pos, ( pos + 8 < data.size() ) ? 8 : data.size() - pos )).to_ulong();
        data = aux;
    }

    template <const unsigned int MAX_CHAR_PER_MSG, const unsigned int TAM_TRAMA>
    bool    Protocol<MAX_CHAR_PER_MSG, TAM_TRAMA>::extractStuff( string &data )
    {
        unsigned int n_unos(0);
        string aux("");

        for( int i=0 ; i < data.size() ; i++ )
        {
            aux += data[i];
            if( data[i] == '1' )
            {
                if( n_unos == 4 )
                {
                    i++;
                    if( data[i] == '1' )
                        return ( 0 );
                    n_unos = 0;
                }
                else
                    n_unos++;
            }
            else
                n_unos = 0;
        }
        data = aux;
        return(1);
    }

    template <const unsigned int MAX_CHAR_PER_MSG, const unsigned int TAM_TRAMA>
    bool    Protocol<MAX_CHAR_PER_MSG, TAM_TRAMA>::extractFlags( string &data )
    {
        if( data.size() >= 22 && data.substr( 0, 8 ) == FLAG && data.substr( data.size()-8, data.size() ) == FLAG )
        {
            data.erase( 0, 8 );
            data.erase( data.size()-8, data.size() );
            return( 1 );
        }
        return( 0 );
    }

    template <const unsigned int MAX_CHAR_PER_MSG, const unsigned int TAM_TRAMA>
    int    Protocol<MAX_CHAR_PER_MSG, TAM_TRAMA>::extractHead( string &data )
    {
        int n_trama = bitset<5>( data.substr( 1, 4 ) ).to_ulong();

        if( !tramas_recibidas.count( n_trama ) )
        {
            tramas_recibidas.insert( n_trama );
            data.erase( 0, 6 );
            return( 1 + ( data[6] == '1' ) );
        }
        return(0);
    }

    template <const unsigned int MAX_CHAR_PER_MSG, const unsigned int TAM_TRAMA>
    void    Protocol<MAX_CHAR_PER_MSG, TAM_TRAMA>::toBin( string &data )
    {
        string bin("");

        for( int i=0 ; i < data.size() ; i++ )
            bin += bitset<8>(data[i]).to_string();

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
                    n_unos++;
            }
            else
                n_unos = 0;
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
