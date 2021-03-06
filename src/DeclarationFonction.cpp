/*************************************************************************
PLD Comp
DeclarationFonction.cpp  -  Description
-------------------
début                : 23/02/2017
copyright            : (C)2015 par H4414
*************************************************************************/

//---------- Réalisation de la classe Declaration (fichier DeclarationFonction.cpp) --

//---------------------------------------------------------------- INCLUDE

//-------------------------------------------------------- Include système
#include <iostream>

using namespace std;

//------------------------------------------------------ Include personnel
#include "DeclarationFonction.h"

//---------------------------------------------------- Variables de classe

//----------------------------------------------------------- Types privés


//----------------------------------------------------------------- PUBLIC
//-------------------------------------------------------- Fonctions amies

//----------------------------------------------------- Méthodes publiques
void DeclarationFonction::print()
{
	cout<<"Debut de Declaration de Fonction"<<endl;
	cout<<"nom fonction : "<<nomFonction<<endl;
	cout<<"type de retour :"<<typeRetour<<endl;
	argsDef->print();

}

string DeclarationFonction::getNomFonction()
{
	return nomFonction;
}
//----- Constructeur
DeclarationFonction::DeclarationFonction(string nf, string tr, ArgsDef* ad):nomFonction(nf),typeRetour(tr),argsDef(ad)
{
		this->typeBrique = TBdecFonction;
}
//-----Bloc vide
//----- Fin constructeur

//----- Destructeur
DeclarationFonction::~DeclarationFonction()
{
	//cout << "Destructeur de DelarationFonction " << endl;
	delete this->argsDef;

}// Bloc vide
//----- Fin destructeur
