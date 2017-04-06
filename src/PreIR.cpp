/*************************************************************************
PLD Comp
PreIR.cpp  -  Description
-------------------
début                : 03/04/2017
copyright            : (C)2015 par Haim Nathan
*************************************************************************/


#include <iostream>
#include <vector>

using namespace std;

//------------------------------------------------------ Include personnel
#include "Programme.h"
#include "Expression.h"
#include "Brique.h"
#include "PreIR.h"
#include "Type.h"
#include "IR.h"
#include "Type.h"
#include "Outils.h"
#include "ExpressionBinaire.h"
#include "ExpressionChar.h"
#include "ExpressionEntier.h"
#include "ExpressionVariable.h"
#include "InstructionVraie.h"
#include "ListInstruction.h"
#include "Return.h"
#include "Instruction.h"

#include "Affectation.h"
#include "AffectationUnaire.h"

#include <fstream>
//---------------------------------------------------- Variables de classe

//----------------------------------------------------------- Types privés
typedef Expression::TypeExpression EnumExpression;
typedef IRInstr::Operation Operation;
typedef Outils::TypeSymbole Symboles;
typedef InstructionVraie::TypeInstruction TypeInstruction;

//----------------------------------------------------------------- PUBLIC
//-------------------------------------------------------- Fonctions amies

//----------------------------------------------------- Méthodes publiques
PreIR::PreIR()
{

}

PreIR::~PreIR()
{
	for (vector<CFG*>::iterator it = listCFG.begin(); it != listCFG.end(); it++)
	{
		delete *it;
	}
	listCFG.clear();
	// delete this->current_cfg;
	// delete this->current_bb;
}


void PreIR::analyseDefFonction(DefFonction* defFonction)
{
    CFG* cfg = new CFG(defFonction);
    listCFG.push_back(cfg);
    current_cfg = cfg;
    BasicBlock* bb = new BasicBlock(current_cfg, current_cfg->new_BB_name());
    current_bb = bb;
    cfg->add_bb(bb);
    analyseBloc(defFonction->getBloc());
}

void PreIR::launchPreIR(Programme* prog)
{
    vector<Brique*> listBrique = prog->getBriques()->getListBrique();
    typeClassBrique tp;

    for(vector<Brique*>::iterator it= listBrique.begin() ; it != listBrique.end() ; it++)
    {
        tp = (*it)->typeClass();
        switch(tp)
        {
            case typeClassBrique::defFonction : analyseDefFonction((DefFonction*)*it);break;
        }
    }
    launchASM();
}


void PreIR::analyseBloc(Bloc* b)
{
    ListInstruction* listInstr = b->getListInstruction();
    vector<Instruction*> instructions = listInstr->getInstructions();
    InstructionVraieClass ins;
    for(vector<Instruction*>::iterator it= instructions.begin() ; it != instructions.end() ; it++)
    {
        ins = (*it)->getInstructionVraie()->typeClass();
        switch(ins)
        {
            case InstructionVraieClass::declaration : analyseDeclaration((Declaration*)(*it)->getInstructionVraie());break;
            case InstructionVraieClass::appelFonction : analyseAppelFonction((AppelFonction*)(*it)->getInstructionVraie()) ; break;
            case InstructionVraieClass::affectation : analyseAffectation((Affectation*)(*it)->getInstructionVraie()) ; break;
            case InstructionVraieClass::returned : analyseReturn((Return*)(*it)->getInstructionVraie()) ; break;
        }
    }
}
 
void PreIR::analyseReturn(Return* returned)
{
    Expression* expr = returned->get_expression();
    InstructionVraieClass ins;
    string tmpVar;
    vector<string> params;
    ins = expr->typeClass();
    // cout << "LE INS EST : " << ins << endl;
    switch(ins)
    {
        case InstructionVraieClass::expressionEntier : 
                tmpVar = analyseExpressionEntier((ExpressionEntier*)expr);
                break;
        case InstructionVraieClass::expressionVariable :
                tmpVar = analyseExpressionVariable((ExpressionVariable*)expr);
                break;
        case InstructionVraieClass::lvalue :
                tmpVar = analyselvalue((LValue*)expr);
                params.push_back(tmpVar);
                break;
    }
    params.push_back(tmpVar);
    current_bb->add_IRInstr(IRInstr::Operation::ret,Type::int64, params);
}

void PreIR::analyseAffectation(Affectation* aff)
{
    string nomVar = aff->get_nom_variable();
    string symbole = aff->get_symbole();
    //TODO mettre un switch pour tester les differents symboles

    string tmpVar;
    vector<string> params;
    Expression* expr = aff->get_expression();
    tmpVar = expressionToIR(expr);
    params.push_back(tmpVar);
    params.push_back(nomVar);

    current_bb->add_IRInstr(IRInstr::Operation::copy,Type::int64, params);
    /*switch(expr->typeClass())
    {
        case InstructionVraieClass::expressionEntier :
                tmpVar = analyseExpressionEntier((ExpressionEntier*)expr); 
                params.push_back(tmpVar);
                params.push_back(nomVar);
                current_bb->add_IRInstr(IRInstr::Operation::copy,Type::int64, params);
                break;
        case InstructionVraieClass::expressionChar :
                tmpVar = analyseExpressionChar((ExpressionChar*)expr); 
                params.push_back(tmpVar);
                params.push_back(nomVar);
                current_bb->add_IRInstr(IRInstr::Operation::copy,Type::ch, params);
                break;
        case InstructionVraieClass::expressionVariable :
                tmpVar = analyseExpressionVariable((ExpressionVariable*)expr);
                params.push_back(tmpVar);
                params.push_back(nomVar);
                current_bb->add_IRInstr(IRInstr::Operation::copy,Type::int64, params);
                break;
        case InstructionVraieClass::expressionBinaire :
                tmpVar = analyseExpressionBinaire((ExpressionBinaire*)expr);
                params.push_back(tmpVar);
                params.push_back(nomVar);
                current_bb->add_IRInstr(IRInstr::Operation::copy,Type::int64, params);
                break;
        case InstructionVraieClass::lvalue :
                tmpVar = analyselvalue((LValue*)expr);
                params.push_back(tmpVar);
                params.push_back(nomVar);
                current_bb->add_IRInstr(IRInstr::Operation::copy,Type::int64, params);
                break;
    }*/
}


string PreIR::analyseExpressionBinaire(ExpressionBinaire* expr)
{
    Expression* exprGauche = expr->get_gauche();
    Expression* exprDroite = expr->get_droite();
    string var1;
    string var2;
    string tmp;
    vector<string> params;
    InstructionVraieClass e = exprGauche->typeClass();
    var1 = expressionToIR(exprGauche);
    var2 = expressionToIR(exprDroite);

    tmp = current_cfg->create_new_tempvar(Type::int64);
    params.push_back(tmp);
    params.push_back(var2);
    params.push_back(var1);
    Outils outils ;
    switch (outils.parse_symb(expr->get_symbole()))
    {
        case Symboles::plus:
        {
            current_bb->add_IRInstr(IRInstr::Operation::add,Type::int64, params);
            break;
        }
        case Symboles::moins:
        {
            current_bb->add_IRInstr(IRInstr::Operation::sub,Type::int64, params);
            break;
        }
    }
    return tmp;
}


void PreIR::analyseDeclaration(Declaration* dec)
{  
    string s = dec->getType();
    if(s == "int64")
    {
        current_cfg->add_to_symbol_table(dec->getNom(),Type::int64);
    }
    else if(s == "int32")
    {
        current_cfg->add_to_symbol_table(dec->getNom(),Type::int32); 
    }
    else if(s == "char")
    {
        current_cfg->add_to_symbol_table(dec->getNom(),Type::ch);
    }
}


void PreIR::launchASM()
{   
    ofstream outfile ("main.s",ofstream::binary);
    outfile<<".text"<<endl;
    outfile<<".global main"<<endl;
    outfile<<"main:"<<endl;
    for(vector<CFG*>::iterator it= listCFG.begin() ; it != listCFG.end() ; it++)
    {
        (*it)->gen_asm(outfile);
    }
    outfile.close();
}

void PreIR::analyseAppelFonction(AppelFonction* appelFonction)
{
    ArgsAppel* argsAppel = appelFonction->getArgsAppel();
    if(argsAppel)
    {
        std::vector<Expression*>listExp = argsAppel->getArgs();
        InstructionVraieClass ins;
        vector<string> listParam;
        listParam.push_back(appelFonction->getNomFonction());

        for(vector<Expression*>::iterator it= listExp.begin() ; it != listExp.end() ; it++)
        {
            string varStr;
            /*ins = (*it)->typeClass();
            
            switch(ins)
            {
                case InstructionVraieClass::expressionChar :
                            varStr = analyseExpressionChar((ExpressionChar*)(*it));
                            listParam.push_back(varStr);
                            break;
                case InstructionVraieClass::expressionVariable :
                            varStr = analyseExpressionVariable((ExpressionVariable*)(*it));
                            listParam.push_back(varStr);
                            break;
                case InstructionVraieClass::lvalue :
                            varStr = analyselvalue((LValue*)(*it));
                            listParam.push_back(varStr);
                            break;
            }*/
            varStr = expressionToIR(*it);
            listParam.push_back(varStr);
        }
        current_bb->add_IRInstr(IRInstr::Operation::call,Type::ch, listParam);
    //IRInstr* irInstr = new IRInstr(current_bb,IRInstr::Operation::call,Type::ch, listParam);    
    }
}

string PreIR::analyselvalue(LValue* expr)
{
    string var = expr->getNom();
    return var;
    //IRInstr* irInstr = new IRInstr(current_bb, Operation op, Type t, std::vector<std::string> params);
}

string PreIR::analyseExpressionChar(ExpressionChar* expressionChar)
{
    string tmpVar = current_cfg->create_new_tempvar(Type::ch);
    vector<string> params;
    params.push_back(tmpVar);
    params.push_back(to_string(expressionChar->getChar()));
    //cout << "le char est : " << to_string(expressionChar->getChar()) << endl;
    current_bb->add_IRInstr(IRInstr::Operation::ldconst,Type::ch, params);
    return tmpVar;
    //IRInstr* irInstr = new IRInstr(current_bb, Operation op, Type t, std::vector<std::string> params);
}


string PreIR::analyseExpressionVariable(ExpressionVariable* expressionVariable)
{
    string var = expressionVariable->get_nomVariable();
    return var;
    //IRInstr* irInstr = new IRInstr(current_bb, Operation op, Type t, std::vector<std::string> params);
}


string PreIR::analyseExpressionEntier(ExpressionEntier* expressionEntier)
{
    string tmpVar = current_cfg->create_new_tempvar(Type::int64);
    vector<string> params;
    params.push_back(tmpVar);
    params.push_back(to_string(expressionEntier->get_valeur()));
    current_bb->add_IRInstr(IRInstr::Operation::ldconst,Type::ch, params);
    return tmpVar;
    //IRInstr* irInstr = new IRInstr(current_bb, Operation op, Type t, std::vector<std::string> params);
}





void PreIR::analyseBloc2(Bloc* b)
{
    ListInstruction* listInstr = b->getListInstruction();
    vector<Instruction*> instructions = listInstr->getInstructions();
    InstructionVraieClass ins;
    for(vector<Instruction*>::iterator instr= instructions.begin() ; instr != instructions.end() ; instr++)
    {
        PreIR::instructionToIR(*instr);

    }

}

// string PreIR::instructionToIR(Instruction* instruction)
// {

// }
/// Return soit a dans le cas a = b + 1 soit tn 
void PreIR::instructionToIR(Instruction* instruction)
{
    InstructionVraie* instructionVraie=instruction->getInstructionVraie();
    TypeInstruction type = instructionVraie->getTypeInstruction();
    switch(type)
    {
        case(TypeInstruction::TIexpression):
        {
            
            Expression* expression = (Expression*)instructionVraie;
            PreIR::expressionToIR(expression);
            break;
        }

        case(TypeInstruction::TIbloc):
        {
            Bloc* bloc = (Bloc*)instructionVraie;
            ListInstruction* listInstruction = bloc->getListInstruction();
            std::vector<Instruction*> instructions = listInstruction-> getInstructions();
            for(std::vector<Instruction*>::iterator i =instructions.begin();i!=instructions.end();++i)
            {
                PreIR::instructionToIR(*i);

            }
            break;

        }
        
        case(TypeInstruction::TIretourFonction):
        {
            Return* retour = (Return*)instructionVraie;
            PreIR::expressionToIR(retour->get_expression());
            break;
        }
        case (TypeInstruction::TIloop):
        {
            break;
        }
        case (TypeInstruction::TIcond):
        {
            break;
        }
        case(TypeInstruction::TIdeclaration):
        {
            Declaration* dec = (Declaration*)instructionVraie;
            PreIR::analyseDeclaration(dec);
            break;
        }
        default:
        {
            break;
        }

    }

}
string PreIR::expressionToIR(Expression* expression)
{
    string result;
    string resultGauche;
    string resultDroite;
    vector<std::string> params;
    EnumExpression type = expression->getType();
    Outils outils ;
    switch(type)
    {
        case EnumExpression::Type_Unaire :
            break;
        case EnumExpression::Type_Binaire :
            
                result = analyseExpressionBinaire((ExpressionBinaire*)expression);
                return result;
                /*cout<<"expression est une expression binaire"<<endl;
                ExpressionBinaire* expressionBinaire = (ExpressionBinaire*)expression;
                resultGauche = expressionToIR(expressionBinaire->get_gauche());
                resultDroite = expressionToIR(expressionBinaire->get_droite());
                params.push_back(resultGauche);
                params.push_back(resultDroite);
                switch (outils.parse_symb(expressionBinaire->get_symbole()))
                {
                    case Symboles::plus:
                    {
                        cout<<"expression est une expression binaire avec un symbole +"<<endl;
                        current_bb->add_IRInstr(Operation::add,Type::int64, params);
                        break;
                    }
                    case Symboles::moins:
                    {
                        cout<<"expression est une expression binaire avec un symbole -"<<endl;
                        current_bb->add_IRInstr(Operation::sub,Type::int64, params);
                        break;
                    }
                    case Symboles::multi:
                    {
                        cout<<"expression est une expression binaire avec un symbole *"<<endl;
                        current_bb->add_IRInstr(Operation::mul,Type::int64, params);
                        break;
                    }
                    default:
                    {
                        throw "toujours pas fait ";
                    }
                break;*/
                    break;
                
            //break;   

        case EnumExpression::Type_Char :
            {
                result = analyseExpressionChar((ExpressionChar*)expression);
                return result;
                /*ExpressionChar* expressionChar = (ExpressionChar*)expression;
                cout << " il y a un char : " << expressionChar->getChar()<<endl;
                result = current_cfg->create_new_tempvar(Type::ch);
                params.push_back(result);
                params.push_back(to_string(expressionChar->getChar()));
                //cout << "le char est : " << to_string(expressionChar->getChar()) << endl;
                current_bb->add_IRInstr(IRInstr::Operation::ldconst,Type::ch, params);
                */
                break;
            }
        case EnumExpression::Type_Entier :
            {
                result = analyseExpressionEntier((ExpressionEntier*)expression); 
                break;
            }
        case EnumExpression::Type_Variable :
            {
                ExpressionVariable* expressionVariable = (ExpressionVariable*)expression;
                result = expressionVariable->get_nomVariable();
                // params.push_back(expressionVariable->get_nomVariable());
                // current_bb->add_IRInstr(Operation::ldconst,Type::ch, params);
                break;
            }
        case EnumExpression::Type_LValue :
                result = analyselvalue((LValue*)expression);
                return result;
        case EnumExpression::Type_Affectation :
            {
                Affectation* affectation = (Affectation*)expression;
                result = expressionToIR(affectation->get_expression());

                // move result to nom_variable
                params.push_back(result);
                params.push_back(affectation->get_nom_variable());
                //TODO mettre un switch pour tester les differents symboles
                switch(outils.parse_symb(affectation->get_symbole()))
                {
                    case Symboles::egal:
                        {
                            switch(expression->typeClass())
                            {
                                case InstructionVraieClass::expressionEntier :
                                        result = expressionToIR(expression); 
                                        params.push_back(result);
                                        params.push_back(affectation->get_nom_variable());
                                        current_bb->add_IRInstr(IRInstr::Operation::copy,Type::int64, params);
                                        break;
                                case InstructionVraieClass::expressionChar :
                                        result = expressionToIR(expression); 
                                        params.push_back(result);
                                        params.push_back(affectation->get_nom_variable());
                                        current_bb->add_IRInstr(IRInstr::Operation::copy,Type::ch, params);
                                        break;
                            }
                            break;
                        }
                    case Symboles::plusegal:
                        {
                            
                            break;
                        }
                    case Symboles::moinsegal:
                        {
                            break;
                        }
                    case Symboles::divegal:
                        {
                           break; 
                        }
                    case Symboles::mulegal:
                        {
                           break; 
                        }
                    default:
                        {
                            break;
                        }
                }
                break;
            }
        case EnumExpression::Type_AppelFonction :
        {
            break;
        }
        case EnumExpression::Type_Assignation :
        {
            break;
        }
        case EnumExpression::Type_ExpressionVariable :
        {
            break;
        }
        default:
        {
            break;
        }
    }
    return result;
}
