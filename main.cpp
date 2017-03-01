#include "Protocol.hpp"

using namespace std;

#define hola Protocol<2000, 1000>::send

main()
{
    hola( "nombre.txt", "salida.txt" );
    cout << (int)'a' << endl;
}
