# Try-catch Recursive Descent Parser

A Recursive Descent Parser for a try-catch grammar.

#### Usage:

- Compile with:
```
g++ parser.cpp -o try-catch-parser
```
- Run with:
```
./try-catch-parser
./try-catch-parser < <input_file>
```
- Example input (space separated tokens):
```
try instr catch instr
```
- Example output:
```
Input Accepted: the type of the try-catch expression is:
 { Either  { Type 1 }   { Type 3 }  } 
```