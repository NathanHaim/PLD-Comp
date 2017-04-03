/*************************************************************************
PLD Comp
Affectation.h  -  Description
-------------------
début                : 15/02/2017
copyright            : (C)2015 par FOLLEAS Jacques et SCHROTER Quentin
*************************************************************************/

//---------- Interface de la classe AffectationDeclaratino (fichier AffectationDeclaratino.h) ------
#if ! defined ( AFFECTATION_H )
#define AFFECTATION_H


//--------------------------------------------------- Interfaces utilisées

#include "toto.h"
#include "Expression.h"

//------------------------------------------------------------------------

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------------
// Role de la classe AffectationDeclaratino:
//

//------------------------------------------------------------------------

class Affectation: public Expression {
    public:
        void print();
        Affectation(string nomVariable, string symbole, Expression* monExpression);
        ~Affectation();

    private:
    string nomVariable;
    string symbole;
    Expression* expression;

};

#endif // if ! defined AFFECTATION