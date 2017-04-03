/*************************************************************************
PLD COMP
Programme.cpp  -  Description
-------------------
début                : 20/03/2017
copyright            : (C)2017 par H4114
*************************************************************************/

//---------- Réalisation de la classe Programme (fichier Programme.cpp) --

//---------------------------------------------------------------- INCLUDE

//-------------------------------------------------------- Include système
#include <iostream>

using namespace std;

//------------------------------------------------------ Include personnel
#include "const.h"
#include "Programme.h"

//---------------------------------------------------- Variables de classe

//----------------------------------------------------------- Types privés


//----------------------------------------------------------------- PUBLIC
//-------------------------------------------------------- Fonctions amies

//----------------------------------------------------- Méthodes publiques
void Programme::print()
{
    cout << "Debut du programme" << endl;

    nbTab++;
    this->brique->print();
    for(vector<Contexte*>::iterator it = listeDeContexte.begin(); it != listeDeContexte.end(); it++)
    {
        (*it)->print();
    }
    nbTab--;
    cout << "Fin du programme" << endl;
}

//----- Constructeur
Programme::Programme():Contexte("Programme")
{}// Bloc vide
//----- Fin constructeur

//----- Constructeur
Programme::Programme(vector <Contexte*> listeDeContexte,Briques* brique):Contexte("Programme")
{
    this->listeDeContexte = listeDeContexte;
    this->brique = brique;
    // Ajout du Parent //
    for (std::vector<Contexte*>::iterator it = listeDeContexte.begin() ; it != listeDeContexte.end(); ++it)
    {
        (*it)->ajouterParent(this); 
    }
}
//----- Fin constructeur

//----- Destructeur
Programme::~Programme()
{
    // cout << "destructeur de Programme" << endl;
     delete this->brique ;
    // for(vector<Contexte*>::iterator it = listeDeContexte.begin(); it != listeDeContexte.end(); it++)
    // {
    //     delete (*it);
    // }
}// Bloc vide
//----- Fin destructeur


vector <Contexte*> Programme::getListeDeContexte()
{
    return this->listeDeContexte;
}

Briques* Programme::getBriques()
{
    return this->brique;
}