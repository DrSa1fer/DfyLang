#pragma once

enum TokenType {
    TokenTypeEOF,                                    // Token: '\0'
    TokenTypeERR,                                    // Token: '\?'


    TokenTypeIdentifier,                             // Token:
    TokenTypeComment,                                // Token:
    TokenTypeString,                                 // Token:

    TokenTypeCharacter,                              // Regex: '.'
    TokenTypeDecimal,                                // Token:
    TokenTypeInteger,                                // Regex: [0-9][_0-9]*[0-9]

    TokenTypeOpenParen,                              // Token: '('
    TokenTypeCloseParen,                             // Token: ')'
    TokenTypeOpenCurly,                              // Token: '{'
    TokenTypeCloseCurly,                             // Token: '}'
    TokenTypeOpenSquare,                             // Token: '['
    TokenTypeCloseSquare,                            // Token: ']'
    TokenTypeOpenAngle,                              // Token: '<'
    TokenTypeCloseAngle,                             // Token: '>'

    TokenTypeSpace,                                  // Token: 'space'
    TokenTypeType,                                   // Token: 'type'
    TokenTypeEnum,                                   // Token: 'enum'
    TokenTypeFunc,                                   // Token: 'func'
    TokenTypeVar,                                    // Token: 'var'
    TokenTypeVal,                                    // Token: 'val'

    TokenTypeIf,                                     // Token: 'if'
    TokenTypeElif,                                   // Token: 'elif'
    TokenTypeElse,                                   // Token: 'else'
    TokenTypeSwitch,                                 // Token: 'switch'
    TokenTypeCase,                                   // Token: 'case'

    TokenTypeWhile,                                  // Token: 'while'
    TokenTypeUntil,                                  // Token: 'until'
    TokenTypeFor,                                    // Token: 'for'

    TokenTypeReturn,                                 // Token: 'return'
    TokenTypeEject,                                  // Token: 'eject'
    TokenTypeYield,                                  // Token: 'yield'
    TokenTypeAgain,                                  // Token: 'again'
    TokenTypeNext,                                   // Token: 'next'
    TokenTypeBack,                                   // Token: 'back'
    TokenTypeStop,                                   // Token: 'stop'

    TokenTypeTry,                                    // Token: 'try'
    TokenTypeCatch,                                  // Token: 'catch'
    TokenTypeFinally,                                // Token: 'finally'

    TokenTypeDo,                                     // Token: 'do'
    TokenTypeIn,                                     // Token: 'in'

    TokenTypePlus,                                   // Token: '+'
    TokenTypeMinus,                                  // Token: '-'
    TokenTypeMultiply,                               // Token: '*'
    TokenTypeDivide,                                 // Token: '/'
    TokenTypeModulo,                                 // Token: '%'
    TokenTypeExponent,                               // Token: '**'
    TokenTypeIncrement,                              // Token: '++'
    TokenTypeDecrement,                              // Token: '--'
    TokenTypeNot,                                    // Token: 'not' | '!'
    TokenTypeAnd,                                    // Token: 'and' | '&&'
    TokenTypeOr,                                     // Token: 'or'  | '||'
    TokenTypeBitNot,                                 // Token: '~'
    TokenTypeBitAnd,                                 // Token: '&'
    TokenTypeBitOr,                                  // Token: '|'
    TokenTypeIs,                                     // Token: 'is' | '==='
    TokenTypeNotIs,                                  // Token: '!=='
    TokenTypeEq,                                     // Token: 'eq' | '=='
    TokenTypeNotEq,                                  // Token: '!='
    TokenTypeLessThan = TokenTypeOpenAngle,         // Token: '<'
    TokenTypeGreaterThan = TokenTypeCloseAngle,     // Token: '>'
    TokenTypeLessThanOrEqual,                        // Token: '<='
    TokenTypeGreaterThanOrEqual,                     // Token: '>='

    TokenTypeAssign,                                 // Token: '='
    TokenTypeAddAssign,                              // Token: '+='
    TokenTypeSubAssign,                              // Token: '-='
    TokenTypeMulAssign,                              // Token: '*='
    TokenTypeDivAssign,                              // Token: '/='
    TokenTypeModAssign,                              // Token: '%='

    TokenTypeDot,                                    // Token: '.'
    TokenTypeRange,                                  // Token: '..'
    TokenTypeEllipsis,                               // Token: '...'
    TokenTypeComma,                                  // Token: ','
    TokenTypeColon,                                  // Token: ':'
    TokenTypeSemicolon,                              // Token: ';'
    TokenTypeDog,                                    // Token: '@'

    TokenTypeTabTrivia,                              // Token: '\t'
    TokenTypeSpaceTrivia,                            // Token: ' '
    TokenTypeNewLineTrivia,                          // Token: '\r\n' | '\r' | '\n'
};

union TokenData {
    char    *comment;

    char    *identifier;
    char    *string;
    char    character;
    int     boolean;
    long    integer;
    double  decimal;
};

typedef struct Token {
  ///Type of token
  enum TokenType type;
  ///Data of token. If type is operator or keyword value is TRASH
  union TokenData data;

  /// Start position of token
  unsigned long start;
  /// End position of token
  unsigned long end;
} Token_t;