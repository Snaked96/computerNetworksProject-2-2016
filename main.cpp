#include "Hamming.hpp"
#include "CRC16.hpp"

using namespace std;


main()
{
    Hamming<2000, 1000> h;
    string a = "0110101";

    h.encode( a );

    cout << a << endl;

    //h.send( "nombre.txt", "salida.txt" );

}
