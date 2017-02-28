#include "Hamming.hpp"
#include "CRC16.hpp"

using namespace std;


main()
{
    Hamming<4000, 1000> h;
    string a( "hola" );

    h.send( "entrada.txt", "canalh.txt" );
    h.receive( "canalh.txt", "salidah.txt" );

}
