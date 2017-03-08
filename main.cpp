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

    h.send( "entrada.txt", "canalh.txt" );
    h.receive( "canalh.txt", "salidah.txt" );

    c.send( "entrada.txt", "canalcrc.txt" );
    c.receive( "canalcrc.txt", "salidacrc.txt" );
}
