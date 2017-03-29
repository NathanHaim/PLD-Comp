/*************************************************************************
PLD Comp
Retour_Fonction.h  -  Description
-------------------
début                : 15/02/2017
copyright            : (C)2015 par FOLLEAS Jacques et SCHROTER Quentin
*************************************************************************/

//---------- Interface de la classe Retour_Fonction (fichier Retour_Fonction.h) ------
#if ! defined ( RETOUR_FONCTION_H )
#define RETOUR_FONCTION_H


//--------------------------------------------------- Interfaces utilisées
# include "Expression.h"
//------------------------------------------------------------------------

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------------
// Role de la classe Expression:
//

//------------------------------------------------------------------------

class Retour_Fonction : public Expression{
    public:
        virtual void print();
        Retour_Fonction();
        ~Retour_Fonction();

    private:
    	Expression *exp;

};

#endif // if ! defined RETOUR_FONCTION_H