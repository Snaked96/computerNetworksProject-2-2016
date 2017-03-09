#include "../libs/Hamming.hpp"
#include "../libs/CRC16.hpp"

using namespace std;

/*
1101011111
11010111110010
*/
main()
{
    Hamming<2000, 1000> h;
    CRC16<2000, 1000> c;

    //h.send( "../Entrada/entrada.txt", "../Canal/canalh.txt" );
    //h.receive( "../Canal/canalh.txt", "../Salida/salidah.txt" );

    //c.send( "../Entrada/entrada.txt", "../Canal/canalcrc.txt" );
    c.receive( "../Canal/canalcrc.txt", "../Salida/salidacrc.txt" );
}
