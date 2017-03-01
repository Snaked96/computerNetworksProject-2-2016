#include "Hamming.hpp"
#include "CRC16.hpp"

using namespace std;


main()
{
    Hamming<2000, 1000> h;
    CRC16<2000, 1000> c;

    c.send( "nombre.txt", "salida.txt" );
}
