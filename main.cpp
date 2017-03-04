#include "Hamming.hpp"
#include "CRC16.hpp"

using namespace std;

/*
1101011111
11010111110010
*/
main()
{
    Hamming<2000, 1000> h;
    CRC16<2000, 1000> c;

    //h.send( "entrada.txt", "canalH.txt" );
    h.receive( "canalH.txt", "salidaH.txt" );

    //c.send( "entrada.txt", "canalC.txt" );
    c.receive( "canalC.txt", "salidaC.txt" );
}
