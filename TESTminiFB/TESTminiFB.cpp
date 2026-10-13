#include "pch.h"
#include "MiniFb.h"
#include "LoadText.h"
#include "Lexer.h"
#include "Parser.h"
#include "Sema.h"
#include "Compilateur.h"
#include "VM.h"
#include "Chrono.h"

std::vector<VarNode*> currentKnowVar;
std::map<std::string, int> currentVar;

bool CheckIfVarExist(VarNode* varNode)
{
    for (VarNode* node : currentKnowVar)
    {
        if (node->name == varNode->name)
            return true;
    }
    return false;
}

int OperationNode(Node* node)
{
    BinaryNode* bn = static_cast<BinaryNode*>(node);

    switch (bn->op)
    {
    case '=':
    {
        //get both variable

        if (dynamic_cast<VarNode*>(bn->childs[0]))
        {
            //check if var exist
            VarNode* varNode = dynamic_cast<VarNode*>(bn->childs[0]);
            if (CheckIfVarExist(varNode))
            {
                //exist
                //second operator
                if (dynamic_cast<IntNode*>(bn->childs[1]))
                {
                    IntNode* intNode = dynamic_cast<IntNode*>(bn->childs[1]);

                    currentVar[varNode->name] = intNode->value;
                }
                else if (dynamic_cast<BinaryNode*>(bn->childs[1]))
                {
                    currentVar[varNode->name] = OperationNode(bn->childs[1]);
                }
            }
            else
            {
                //error
            }
        }

        if (dynamic_cast<BinaryNode*>(bn->childs[0]))
        {
            int test = 0;
        }

        break;
    }
    case '+':
    {
        //get both operator and make the operation
        int value1;
        int value2;

        VarNode* varNode = dynamic_cast<VarNode*>(bn->childs[0]);
        if (varNode != nullptr)
        {
            if (CheckIfVarExist(varNode))
            {
                value1 = currentVar[varNode->name];
            }
        }
        
        IntNode* intNode = dynamic_cast<IntNode*>(bn->childs[1]);
        if (intNode != nullptr)
        {
            value2 = intNode->value;
        }

        return value1 + value2;

        break;
    }
    case '/':
    {
        int value1;
        int value2;

        VarNode* varNode = dynamic_cast<VarNode*>(bn->childs[0]);
        if (varNode != nullptr)
        {
            if (CheckIfVarExist(varNode))
            {
                value1 = currentVar[varNode->name];
            }
        }

        IntNode* intNode = dynamic_cast<IntNode*>(bn->childs[1]);
        if (intNode != nullptr)
        {
            value2 = intNode->value;
        }

        return value1 / value2;

        break;
    }
    case '*':
    {
        int value1;
        int value2;

        VarNode* varNode = dynamic_cast<VarNode*>(bn->childs[0]);
        if (varNode != nullptr)
        {
            if (CheckIfVarExist(varNode))
            {
                value1 = currentVar[varNode->name];
            }
        }

        IntNode* intNode = dynamic_cast<IntNode*>(bn->childs[1]);
        if (intNode != nullptr)
        {
            value2 = intNode->value;
        }

        return value1 * value2;

        break;
    }
    }

    return 0;
}

void TestNode(Node* node)
{
    //store global var and local where ???
    
    if (node != nullptr)
    {
        if (dynamic_cast<GlobalNode*>(node))
        {
            //read each child
            for (Node* child : node->childs)
            {
                TestNode(child);
            }
        }
        else if (dynamic_cast<FunctionNode*>(node))
        {
            //read each child
            for (Node* child : node->childs)//store param ?
            {
                TestNode(child);
            }
        }
        else if (dynamic_cast<BinaryNode*>(node))
        {
            OperationNode(node);
        }
        else if (dynamic_cast<VarNode*>(node))
        {
            VarNode* var = dynamic_cast<VarNode*>(node);
            currentKnowVar.push_back(var);
            currentVar.insert(std::make_pair(var->name, 0));
        }
        else if (dynamic_cast<ConditionNode*>(node))
        {
            for (Node* child : node->childs)
            {
                TestNode(child);
            }
        }
        //call function
    }
}

int main()
{
    LoadText text; 
    text.LoadTxt("../text/test_code2.tcl");
    std::vector<Token> tokens = Lexer::LexerCheck(text.text);

    Parser parser;
    Sema sema;
    Compilateur comp;
    VM vm;

    Node* node = parser.ParserCheck(tokens);

    //TestNode(node);

    sema.SemaCheck(node);

    std::unordered_map<int, std::vector<uint32_t>> result = comp.CompilateurCheck(node);

    //Test
    /*uint32_t test = result["init"][0];

    uint8_t op = test & 0xFF;
    uint8_t a = (test >> 8) & 0xFF;
    uint8_t b = (test >> 16) & 0xFF;
    uint8_t c = (test >> 24) & 0xFF;*/

    for (uint32_t val : result[0])
    {
        uint8_t op = val & 0xFF;

        int csc = 0;
    }

    Chrono chrono;
    chrono.Start();

    MiniFb mini;
    mini.Init(800, 600);
    vm.SetBuffer(mini.GetBuffer());
    vm.SetSize(800, 600);

    vm.CheckVM(result);

    float time = chrono.CurrentTime();

    cout << "Time taken to calcule with the vm " << time << " s" << endl;

    mini.Run();
}