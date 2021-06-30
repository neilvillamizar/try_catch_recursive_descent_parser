/*
Recursive Descent Parser for the try-catch grammar:

S   → I $                   {   S.type <- I.type  }

I   → try I0 catch I1 F R   {
                                F.inherited <- Either I0.type I1.type
                                R.inherited <- F.type
                                I.type <- R.type
                            }
    | instr R               {
                                R.inherited <- instr.type
                                I.type <- R.type
                            }

F   → finally I             {   F.type <- I.type    }

    | λ                     {   F.type <- F.inherited    }

R   → ; I                   {   R.type <- I.type  }

    | λ                     {   R.type <- R.inherited    }

*/
#include <iostream>
#include <vector>
#include <string>

using namespace std;

vector<string> TOK;  // tokens
int LA;              // Look Ahead

// Reject the input
void reject() {
    cout << "Parsing error: input sequence rejected." << endl;
    exit(0);
}

// representing the rule: S → I $
struct S_rule {

    string type;
    void parse();

};

// representing the rule starting with I
struct I_rule {

    string type, inherited;
    void parse();

};

// representing the rule starting with F
struct F_rule {

    string type, inherited;
    void parse();

};

// representing the rule starting with R
struct R_rule {

    string type, inherited;
    void parse();

};

// method to reduce with the R rule
void R_rule::parse() {
    // if ';' is found 
    // read ';' and continue with I rule
    if (TOK[LA] == ";") {
        LA++;
        I_rule I;
        I.parse();
        type = I.type;
        return;
    }
    //  Check if R -> λ
    if ( TOK[LA] == "$" || TOK[LA] == "catch" || TOK[LA] == "finally") {
        type = inherited;
        return;
    }
       
    reject();
}

// method to reduce with the F rule
void F_rule::parse() {

    // A finally is found. Read finally and continue with I rule
    if (TOK[LA] == "finally") {
        LA++;
        I_rule I;
        I.parse();
        type = I.type;
        return;
    }
    
    // Check if F -> λ
    if (TOK[LA] == "$" || TOK[LA] == ";" || TOK[LA] == "catch") {
        type = inherited;
        return;
    }
    
    reject();
}

// method to reduce with the I rule
void I_rule::parse() {

    // A try is found
    // corresponds to 
    // I → try I0 catch I1 F R
    if (TOK[LA] == "try") {
        
        LA++;
        I_rule I0;
        I0.parse();

        if (TOK[LA] != "catch")
            reject();

        LA++;
        I_rule I1;
        I1.parse();

        F_rule F;
        F.inherited = " { Either " + I0.type + " " + I1.type + " } ";
        F.parse();

        R_rule R;
        R.inherited = F.type;
        R.parse();
        type = R.type;

        return;
    }

    // A instr is found
    // corresponds to I → instr R 
    if (TOK[LA] == "instr") {
        LA++;
        R_rule R;
        R.inherited = " { Type " + to_string(LA - 1) + " } ";
        R.parse();
        type = R.type;
        return;
    }
    
    reject();
}

// method to reduce with the S rule
void S_rule::parse() {
    
    // A try catch expression starts with try or instr tokens
    if (TOK[LA] == "try" || TOK[LA] == "instr") {
        
        I_rule I;
        I.parse();
        type = I.type;
        
        // Check end of sequence
        if (TOK[LA] != "$" || TOK.size() - 1 > LA)
            reject();

        return;
    }
    
    reject();
}

int main() {

    string input_str = "", aux, token, space = " ", endOfInput = "$";
    size_t pos = 0;

    cout << "Enter the sequence of tokens to be parsed (space separated):" << endl;

    while(getline(cin, aux)){
        input_str += aux;
        input_str += " ";
    }

    while ((pos = input_str.find(space)) != string::npos) {
        token = input_str.substr(0, pos);
        input_str.erase(0, pos+1);
        if(token == space || !token.size())
            continue;
        TOK.push_back(token);
    }

    TOK.push_back(endOfInput);

    LA = 0;

    S_rule parser;
    parser.parse();
    cout << "Input Accepted: the type of the try-catch expression is: \n";
    cout << parser.type << endl;

    return 0;
}