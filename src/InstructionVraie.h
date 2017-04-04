/*************************************************************************
PLD Comp
InstructionVraie.h  -  Description
-------------------
début                : 15/02/2017
copyright            : (C)2015 par FOLLEAS Jacques et SCHROTER Quentin
*************************************************************************/

//---------- Interface de la classe InstructionVraie (fichier InstructionVraie.h) ------
#if ! defined ( INSTRUCTIONVRAIE_H )
#define INSTRUCTIONVRAIE_H


//--------------------------------------------------- Interfaces utilisées

//------------------------------------------------------------------------

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------------
// Role de la classe InstructionVraie:
//

//------------------------------------------------------------------------

enum InstructionVraieClass{ instructionVraie = 0, declaration = 1, expression = 2, appelFonction = 3, expressionChar = 4 };

class InstructionVraie{
    public:
        virtual void print() = 0;
        InstructionVraie();
        ~InstructionVraie();
        virtual InstructionVraieClass typeClass();

    private:

};

#endif // if ! defined INSTRUCTIONVRAIE_H
