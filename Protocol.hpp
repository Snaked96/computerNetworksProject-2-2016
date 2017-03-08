#ifndef _PROTOCOL_HPP_
#define _PROTOCOL_HPP_

    //Librer�as
    #include <iostream>
    #include <fstream>
    #include <bitset>
    #include <cmath>
    #include <vector>

    using namespace std;

    //Contantes
    #define FLAG "01111110"

    template <const size_t MAX_CHAR_PER_MSG, const size_t TAM_TRAMA>
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
            bool    extractStuff( string &data );
            bool    extractFlags( string &data );
            int     extractHead( string &data, vector<string> &tramas, size_t &n_trama );
            bool    contiguas( const vector<string> &tramas );

            virtual int    decode( string &data ){};


    };

    template <const size_t MAX_CHAR_PER_MSG, const size_t TAM_TRAMA>
    void    Protocol<MAX_CHAR_PER_MSG, TAM_TRAMA>::send( string nf_in, string nf_out )
    {
        size_t CHAR_PER_TRAMA = TAM_TRAMA/8;
        size_t N_TRAMAS = MAX_CHAR_PER_MSG/CHAR_PER_TRAMA;
        string trama;
        string msg;
        char   msg_char[2500];

        try
        {
            ifstream f_in( nf_in.c_str() );
            ofstream f_out( nf_out.c_str() );
            f_in.get( msg_char, MAX_CHAR_PER_MSG );
            f_in.close();



            msg = msg_char;

            for( int i = 0, pos = 0 ; i < N_TRAMAS && pos < msg.size()  ; i++,pos+=CHAR_PER_TRAMA )
            {
                trama = msg.substr( pos, ( pos + CHAR_PER_TRAMA < msg.size() ) ? CHAR_PER_TRAMA : msg.size() - pos );
                toBin( trama );

                insertHead( trama, i, (i == N_TRAMAS-1 || trama.size() < 125*8) ? 1 : 0 );
                encode( trama );
                insertStuff( trama );
                insertFlags( trama );
                f_out << trama << endl;
            }
            f_out.close();
        }
        catch( char const* error )
        {
            cout << endl << error << endl;
        }
    }

    template <const size_t MAX_CHAR_PER_MSG, const size_t TAM_TRAMA>
    void    Protocol<MAX_CHAR_PER_MSG, TAM_TRAMA>::receive( string nf_in, string nf_out )
    {
        const size_t CHAR_PER_TRAMA = TAM_TRAMA/8;
        const size_t N_TRAMAS = MAX_CHAR_PER_MSG/CHAR_PER_TRAMA;
        string aux_trama;
        string msg;
        char   msg_char[10000];
        ifstream f_in( nf_in.c_str() );
        ofstream f_out( nf_out.c_str() );
        size_t flag_hamming=0, flag_head, n_trama, k;
        bool flag=-1, auxflags, auxstuff;
        vector<string> tramas(N_TRAMAS, "");

        try
        {
            for( size_t i = 0; i < N_TRAMAS ; i++ )
            {
                f_in.getline( msg_char, 9000 );
                aux_trama = msg_char;
                if( aux_trama.size() )
                {
                    auxflags = extractFlags( aux_trama );
                    auxstuff = extractStuff( aux_trama );
                    flag_hamming = max( flag_hamming, (size_t)decode( aux_trama ) );
                    flag_head = extractHead( aux_trama, tramas, n_trama );
                    //cout << endl << i << " flag_hamming = " << flag_hamming << "  flag_head = " <<  flag_head << " auxflags = " << auxflags << " auxstuff = " << auxstuff << endl;
                    if( auxflags && auxstuff && flag_hamming && flag_head )
                    {
                        backToASCII( aux_trama );
                        //f_out << trama;
                        tramas[ n_trama ] = aux_trama;
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

            if( flag && contiguas( tramas ) )
            {
                for( size_t i = 0; i < tramas.size() ; i++ )
                    f_out << tramas[i];
                if( flag_hamming == 2 )
                {
                    cout << endl << "Se ha detectado algun error y se corrigio." << endl;
                    f_out << endl << "Se ha detectado algun error y se corrigio." << endl;
                }
            }
            else
            {
                cout << endl << "Se ha detectado algun error." << endl;
                f_out << endl << "Se ha detectado algun error." << endl;
            }

            f_in.close();
            f_out.close();
        }
        catch( char const* error )
        {
            cout << endl << error << endl;
        }
    }

    template <const size_t MAX_CHAR_PER_MSG, const size_t TAM_TRAMA>
    bool    Protocol<MAX_CHAR_PER_MSG, TAM_TRAMA>::contiguas( const vector<string> &a )
    {
        size_t k;
        for( k = 0 ; k < a.size() && a[k] != "" ; k++ );
        for( ; k < a.size() && a[k] == "" ; k++ );
        if( k < a.size() )
            throw "Tramas perdidas.";
        return( k == a.size() );
    }

    template <const size_t MAX_CHAR_PER_MSG, const size_t TAM_TRAMA>
    void    Protocol<MAX_CHAR_PER_MSG, TAM_TRAMA>::backToASCII( string &data )
    {
        string aux("");
        for( int pos = 0 ; pos < data.size()  ; pos+=8 )
            aux += (char) bitset<8>(data.substr( pos, ( pos + 8 < data.size() ) ? 8 : data.size() - pos )).to_ulong();
        data = aux;
    }

    template <const size_t MAX_CHAR_PER_MSG, const size_t TAM_TRAMA>
    bool    Protocol<MAX_CHAR_PER_MSG, TAM_TRAMA>::extractStuff( string &data )
    {
        size_t n_unos(0);
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
                    {
                        throw "Se encontro la bandera en medio de una trama";
                        return ( 0 );
                    }
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

    template <const size_t MAX_CHAR_PER_MSG, const size_t TAM_TRAMA>
    bool    Protocol<MAX_CHAR_PER_MSG, TAM_TRAMA>::extractFlags( string &data )
    {
        if( data.size() >= 22 && data.substr( 0, 8 ) == FLAG && data.substr( data.size()-8, data.size() ) == FLAG )
        {
            data.erase( 0, 8 );
            data.erase( data.size()-8, data.size() );
            return( 1 );
        }
        throw "No se encontraron las banderas al princiopio o al final de una trama.";
        return( 0 );
    }

    template <const size_t MAX_CHAR_PER_MSG, const size_t TAM_TRAMA>
    int    Protocol<MAX_CHAR_PER_MSG, TAM_TRAMA>::extractHead( string &data, vector<string> &tramas, size_t &n_trama )
    {
        n_trama = bitset<5>( data.substr( 1, 4 ) ).to_ulong();
        if( tramas[ n_trama ] == "" )
        {
            data.erase( 0, 6 );
            return( 1 + ( data[6] == '1' ) );
        }
        throw "Tramas repetidas.";
        return(0);
    }

    template <const size_t MAX_CHAR_PER_MSG, const size_t TAM_TRAMA>
    void    Protocol<MAX_CHAR_PER_MSG, TAM_TRAMA>::toBin( string &data )
    {
        string bin("");

        for( int i=0 ; i < data.size() ; i++ )
        {
            if( data[i] < 32 || data[i] > 126 )
                throw "Caracter invalido.";
            bin += bitset<8>(data[i]).to_string();
        }
        data = bin;
    }

    template <const size_t MAX_CHAR_PER_MSG, const size_t TAM_TRAMA>
    void    Protocol<MAX_CHAR_PER_MSG, TAM_TRAMA>::insertStuff( string &data )
    {
        size_t n_unos(0);
        string data_aux("");

        for( int i=0 ; i < data.size() ; i++ )
        {
            data_aux += data[i];
            if( data[i] == '1' )
            {
                if( n_unos == 4 )
                {
                    data_aux += "0";
                    n_unos = 1;
                }
                else
                    n_unos++;
            }
            else
                n_unos = 0;
        }
        data = data_aux;
    }

    template <const size_t MAX_CHAR_PER_MSG, const size_t TAM_TRAMA>
    void    Protocol<MAX_CHAR_PER_MSG, TAM_TRAMA>::insertFlags( string &data )
    {
        data.insert( 0, "01111110" ).append( "01111110" );
    }

    template <const size_t MAX_CHAR_PER_MSG, const size_t TAM_TRAMA>
    void    Protocol<MAX_CHAR_PER_MSG, TAM_TRAMA>::insertHead( string &data, int ce, int t )
    {
        data = "0"+bitset<4>(ce).to_string()+bitset<1>(t).to_string()+data;
    }

#endif // _PROTOCOL_HPP_
