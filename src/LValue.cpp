/*************************************************************************
PLD Comp
LValue.cpp  -  Description
-------------------
début                : 23/02/2017
copyright            : (C)2015 par Haim Nathan
*************************************************************************/

//---------- Réalisation de la classe LValue (fichier LValue.cpp) --

//---------------------------------------------------------------- INCLUDE

//-------------------------------------------------------- Include système
#include <iostream>

using namespace std;

//------------------------------------------------------ Include personnel
#include "LValue.h"

//---------------------------------------------------- Variables de classe

//----------------------------------------------------------- Types privés


//----------------------------------------------------------------- PUBLIC
//-------------------------------------------------------- Fonctions amies

//----------------------------------------------------- Méthodes publiques

std::list<string> LValue::listeNomLValue()
{
    list<string> list;
    list.push_back(nom);
    return list;
}

Expression::TypeExpression LValue::getType()
{
    return Expression::TypeExpression::Type_LValue;
}

//----- Constructeur
LValue::LValue(string no, bool isT, int nu):nom(no),isTab(isT), num(nu)
{
    this->type_expression = Expression::TypeExpression::Type_LValue;
}// Bloc vide
//----- Fin constructeur

//----- Destructeur
LValue::~LValue()
{}// Bloc vide
//----- Fin destructeur

void LValue::print()
{
	cout << "LValue : nom -> " << nom;
	if(isTab)
	{
		cout<<"[";
		if(this->num>0)
		{
			cout<<this->num;
		}
		cout<<"]";
	}
	cout<<endl;
}

InstructionVraieClass LValue::typeClass()
{
    return InstructionVraieClass::lvalue;
}


