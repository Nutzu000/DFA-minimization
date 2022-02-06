#include <iostream>
#include "AFD.h"
int main()
{
	AFD a;
	a.citireFisier("AFD.txt");
	if (a.verifica()) {
		a.minimizare();
		a.afisareInFisier("AFD minimizat.txt");
	}
	else {
		std::cout << "AFD-ul introdus nu este corect!";
	}
}