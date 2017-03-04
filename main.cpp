#include "Hamming.hpp"
#include "CRC16.hpp"

using namespace std;

//0101001
//100010110011

main()
{
    Hamming<4000, 1000> h;
    string a( "hola" );
    string data = "0101001";

    //h.encode( data );
    data = "100010000011";
    if( h.decode( data ) )
        cout << endl << data << endl;
    else
        cout << endl << "Error" << endl;
}
