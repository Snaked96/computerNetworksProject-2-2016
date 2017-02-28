#include "Protocol.hpp"

using namespace std;

main()
{
    string nombre = "A";

    //Protocol v;

    Protocol<2000, 1000>::send(nombre, "hola");

    /*
    char cadena[128];
    ofstream f_out("nombre.txt");

    f_out << "Hola, mundo" << endl;
    f_out.close();

    ifstream f_in("nombre.txt");
    f_in.getline(cadena, 128);

    cout << cadena << endl;
    */
}
