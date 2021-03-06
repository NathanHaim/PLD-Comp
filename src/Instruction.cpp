/*************************************************************************
PLD Comp
Instruction.cpp  -  Description
-------------------
début                : 15/02/2017
copyright            : (C)2015 par FOLLEAS Jacques et SCHROTER Quentin
*************************************************************************/

//---------- Réalisation de la classe Instruction (fichier Instruction.cpp) --

//---------------------------------------------------------------- INCLUDE

//-------------------------------------------------------- Include système
#include <iostream>
#include <string>
#include <list>

using namespace std;

//------------------------------------------------------ Include personnel

#include "InstructionVraie.h"
#include "Expression.h"
#include "Instruction.h"
#include "Declaration.h"
#include "Return.h"
#include "Bloc.h"
#include "Cond.h"
#include "LoopStatement.h"

//---------------------------------------------------- Variables de classe

//----------------------------------------------------------- Types privés


//----------------------------------------------------------------- PUBLIC
//-------------------------------------------------------- Fonctions amies

//----------------------------------------------------- Méthodes publiques


void Instruction::checkContexte(Contexte* contexteCourant )
{


    if (instruction->getTypeInstruction() == Bloc::TypeInstruction::TIdeclaration )
    {
        contexteCourant->ajouterVariable( ((Declaration * )(instruction))->getNom() , "int64_t" );
        if(!contexteCourant->chercherVariable( ((Declaration * )(instruction))->getNom()))
            cout << "probleme avec declaration" <<endl;
    }


    if (instruction->getTypeInstruction() == Bloc::TypeInstruction::TIexpression )
    {
        list<string> listLValueUtilisees = ((Expression * )(instruction))->listeNomLValue();

        for(list<string>::iterator it = listLValueUtilisees.begin(); it != listLValueUtilisees.end(); ++it) 
        {
            if(!contexteCourant->chercherVariable( (*it)))
            {
                cout << "la variable  \" " << (*it) 
                    << " \" est utiliser mais n'est pas déclarée " << endl;
            }
        }
    }


    if (instruction->getTypeInstruction() == Bloc::TypeInstruction::TIretourFonction )
    {
        list<string> listLValueUtilisees = ((Return * )(instruction))->listeNomLValue();

        for(list<string>::iterator it = listLValueUtilisees.begin(); it != listLValueUtilisees.end(); ++it) 
        {
            if(!contexteCourant->chercherVariable( (*it)))
            {
                cout << "la variable  \" " << (*it) 
                    << " \" est utiliser dans un retour foncton mais n'est pas déclarée " << endl;
            }
        }
    }


    if (instruction->getTypeInstruction() == Bloc::TypeInstruction::TIbloc )
    {
         ((Bloc * )(instruction))->checkContexte(contexteCourant);

    }

    if (instruction->getTypeInstruction() == Bloc::TypeInstruction::TIcond )
    {
         ((Cond * )(instruction))->checkContexte(contexteCourant);

    }
    

    if (instruction->getTypeInstruction() == Bloc::TypeInstruction::TIloop )
    {
         ((LoopStatement * )(instruction))->checkContexte(contexteCourant);

    }
}

void Instruction::print()
{
    cout << "Instruction :" << endl;
    instruction->print();
}

//----- Constructeur
Instruction::Instruction(InstructionVraie* monInstruction): instruction(monInstruction)
{}// Bloc vide
//----- Fin constructeur

//----- Destructeur
Instruction::~Instruction()
{
    //cout << "Destructeur de Instruction " << endl;
    delete this->instruction;
}// Bloc vide
//----- Fin destructeur

InstructionVraie* Instruction::getInstructionVraie()
{
    return this->instruction;
}
