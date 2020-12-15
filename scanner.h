#ifndef SCANNER_H
#define SCANNER_H

#include <locale.h>

#define CONSUMED 0

#define MAX_ID_SIZE 100

#define RESERVED_SIZE 8

#define SYMBOLS_SIZE 9

enum states{DONE, START, INCOMMMENT, INNUM, INID, INASSIGN, INSYMBOL};
extern char tokenName[][20];

enum tokens{PLUS=0, MINUS=1, MULTIPLY=2, DIVIDE=3, EQUAL=4, LESSTHAN=5,
            OPENBRACKET=6, CLOSEBRACKET=7, SEMICOLON=8,
            IF=SYMBOLS_SIZE+0, THEN=SYMBOLS_SIZE+1, ELSE=SYMBOLS_SIZE+2,
            END=SYMBOLS_SIZE+3, REPEAT=SYMBOLS_SIZE+4, UNTIL=SYMBOLS_SIZE+5,
            READ=SYMBOLS_SIZE+6, WRITE=SYMBOLS_SIZE+7,
            ASSIGN, OTHER, NUM, ID};


struct evaluation
{
  bool tokenEvaluated;
  tokens tokenType;
  char* tokenValue;
};

class scanner
{
private:
  states state;
  tokens resulting_token;
  char token_value[MAX_ID_SIZE];
  int token_value_index;
  char nextChar;
  bool reservedCandidate;
  states evaluate_current_state();
  void consume_character(bool dontIgnore);

public:
  evaluation insert_next_char(char next);
  states get_state(void);
  scanner();
};

#endif // SCANNER_H
