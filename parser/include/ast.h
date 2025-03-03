#pragma once

#include <stdlib.h>

struct ASTNodeTypeDec;
struct ASTNodeEnumDec;
struct ASTNodeFuncDec;
struct ASTNodeVarDec;
struct ASTNodeValDec;

struct ASTNodeWhileStmt;
struct ASTNodeUntilStmt;
struct ASTNodeForStmt;

struct ASTNodeReturnStmt;
struct ASTNodeRepeatStmt;
struct ASTNodeEjectStmt;
struct ASTNodeYieldStmt;
struct ASTNodeNextStmt;
struct ASTNodeBackStmt;
struct ASTNodeStopStmt;

struct ASTNodeIfStmt;
struct ASTNodeElifStmt;
struct ASTNodeElseStmt;
struct ASTNodeSwitchStmt;
struct ASTNodeCaseStmt;

enum ASTNodeType {
    ASTNodeTypeEOF,                                    // Rule: '\0'
    ASTNodeTypeERR,                                    // Rule: '\?'

    ASTNodeTypeComment,                                // Rule:

    ASTNodeTypeIdentifier,                             // Rule: IDENTIFIER
    ASTNodeTypeCharacter,                              // Rule: CHARACTER
    ASTNodeTypeNumber,                                 // Rule: NUMBER
    ASTNodeTypeString,                                 // Rule: STRING
    ASTNodeTypeArray,                                  // Rule: '[' NODE (',' NODE)* ']' | '.' NUMBER

    ASTNodeTypeScopeStmt,     // Rule: '{' NODE* '}' | ';'
    ASTNodeTypeConditionExpr, // Rule: NODE

    ASTNodeTypeAttributeDecl,     // Rule: 'attr' IDENTIFIER  SCOPE
    ASTNodeTypeSpaceDecl, // Rule: 'space' STRING  SCOPE
    ASTNodeTypeTypeDecl,  // Rule: 'type' IDENTIFIER (':' IDENTIFIER)?  SCOPE
    ASTNodeTypeEnumDecl,  // Rule: 'enum' IDENTIFIER  SCOPE
    ASTNodeTypeFuncDecl,  // Rule: 'func' IDENTIFIER '(' IDENTIFIER (':' IDENTIFIER)? (',' IDENTIFIER (':' IDENTIFIER)?)*  ')' ('>' IDENTIFIER)?  SCOPE
    ASTNodeTypeVarDecl,   // Rule: 'var' IDENTIFIER (':' IDENTIFIER)?
    ASTNodeTypeValDecl,   // Rule: 'val' IDENTIFIER (':' IDENTIFIER)?

    ASTNodeTypeAttributeStmt,     // Rule: '@' IDENTIFIER

    ASTNodeTypeIfStmt,     // Rule: 'if' CONDITION  SCOPE ELIF* ELSE?
    ASTNodeTypeElifStmt,   // Rule: 'elif' CONDITION  SCOPE
    ASTNodeTypeElseStmt,   // Rule: 'else'  SCOPE
    ASTNodeTypeSwitchStmt, // Rule: 'switch' NODE  SCOPE
    ASTNodeTypeCaseStmt,   // Rule: 'case'  SCOPE

    ASTNodeTypeWhileStmt, // Rule: 'while' CONDITION  SCOPE
    ASTNodeTypeUntilStmt, // Rule: 'until' CONDITION  SCOPE
    ASTNodeTypeForStmt,    // Rule: 'for' IDENTIFIER 'in' NODE  SCOPE

    ASTNodeTypeReturnStmt, // Rule: 'return' '(' NODE ')'
    ASTNodeTypeEjectStmt,  // Rule: 'eject'  '(' NODE ')'
    ASTNodeTypeYieldStmt,  // Rule: 'yield'  '(' NODE ')'
    ASTNodeTypeDoStmt,     // Rule: 'do'     '(' NODE ')'

    ASTNodeTypeAgainStmt,  // Rule: 'again'
    ASTNodeTypeNextStmt,   // Rule: 'next'
    ASTNodeTypeBackStmt,   // Rule: 'back'
    ASTNodeTypeStopStmt,   // Rule: 'stop'

    ASTNodeTypeTryStmt,      // Rule: 'try'      SCOPE
    ASTNodeTypeCatchStmt,    // Rule: 'catch'    SCOPE //todo
    ASTNodeTypeFinallyStmt,  // Rule: 'finally'  SCOPE

    ASTNodeTypeAdditionExpr,         // Rule: '+'
    ASTNodeTypeSubtractionExpr,      // Rule: '-'
    ASTNodeTypeMultiplicationExpr,   // Rule: '*'
    ASTNodeTypeDivisionExpr,         // Rule: '/'
    ASTNodeTypeModulusExpr,          // Rule: '%'
    ASTNodeTypeExponentiationExpr,   // Rule: '**'
    ASTNodeTypeIncrementExpr,        // Rule: '++'
    ASTNodeTypeDecrementExpr,        // Rule: '--'
    ASTNodeTypeNotExpr,              // Rule: 'not' | '!'
    ASTNodeTypeAndExpr,              // Rule: 'and' | '&&'
    ASTNodeTypeOrExpr,               // Rule: 'or'  | '||'
    ASTNodeTypeBitNotExpr,           // Rule: '~'
    ASTNodeTypeBitAndExpr,           // Rule: '&'
    ASTNodeTypeBitOrExpr,            // Rule: '|'
    ASTNodeTypeIsExpr,               // Rule: 'is' | '==='
    ASTNodeTypeNotIsExpr,            // Rule: '!=='
    ASTNodeTypeEqExpr,               // Rule: 'eq' | '=='
    ASTNodeTypeNotEqExpr,            // Rule: '!='
    ASTNodeTypeLessThanExpr,         // Rule: '<'
    ASTNodeTypeGreaterThanExpr,      // Rule: '>'
    ASTNodeTypeLessThanOrEqualExpr,  // Rule: '<='
    ASTNodeTypeGreaterThanOrEqualExpr, // Rule: '>='

    ASTNodeTypeAssignExpr,             // Rule: '='
    ASTNodeTypeAddAssignExpr,          // Rule: '+='
    ASTNodeTypeSubAssignExpr,          // Rule: '-='
    ASTNodeTypeMulAssignExpr,          // Rule: '*='
    ASTNodeTypeDivAssignExpr,          // Rule: '/='
    ASTNodeTypeModAssignExpr,          // Rule: '%='

    ASTNodeTypeObjAccessExpr,     // Rule: PRIMARY '.'  PRIMARY
    ASTNodeTypePtrAccessExpr,     // Rule: PRIMARY '->' PRIMARY

    ASTNodeTypeRangeExpr,         // Rule: NUMBER '..' NUMBER
    ASTNodeTypeEllipsis,          // Rule: '...'
    ASTNodeTypeComma,             // Rule: ','
    ASTNodeTypeColon,             // Rule: ':'

    ASTNodeTypeFuncCall,       // Rule: PRIMARY '('  ')' ('do' (FUNC_DECL | PRIMARY))?


    ASTNodeTypeExprExpr,    // Rule: '(' NODE (',' NODE)* ')'
    ASTNodeTypeSubExprExpr, // Rule: '(' NODE ')'

    ASTNodeTypeTabTrivia,        // Rule: '\t'
    ASTNodeTypeSpaceTrivia,      // Rule: ' '
    ASTNodeTypeNewLineTrivia,    // Rule: '\n' | '\t'
};

struct ASTNode {
    /// Type of node
    enum ASTNodeType type;
    /// Data of node
    union {
        union {
            char    *identifier_lit;
            char    *comment_lit;
            char    *string_lit;

            char    character_lit;
            int     integer_lit;
            double  decimal_lit;
        };

        struct {
            struct ASTNode *operand0;
            struct ASTNode *operand1;
            struct ASTNode *operand2;
        };

        union {
            struct ASTNodeTypeDec       *type_dec;
            struct ASTNodeEnumDec       *enum_dec;
            struct ASTNodeFuncDec       *func_dec;
            struct ASTNodeVarDec        *var_dec;
            struct ASTNodeValDec        *val_dec;
        };

        union {
            struct ASTNodeWhileStmt     *while_stmt;
            struct ASTNodeUntilStmt     *until_stmt;
            struct ASTNodeForStmt       *for_stmt;

            struct ASTNodeReturnStmt    *return_stmt;
            struct ASTNodeRepeatStmt    *repeat_stmt;
            struct ASTNodeEjectStmt     *eject_stmt;
            struct ASTNodeYieldStmt     *yield_stmt;
            struct ASTNodeNextStmt      *next_stmt;
            struct ASTNodeBackStmt      *back_stmt;
            struct ASTNodeStopStmt      *stop_stmt;

            struct ASTNodeIfStmt        *if_stmt;
            struct ASTNodeElifStmt      *elif_stmt;
            struct ASTNodeElseStmt      *else_stmt;
            struct ASTNodeSwitchStmt    *switch_stmt;
            struct ASTNodeCaseStmt      *case_stmt;
        };
    };

    /// Start position of node
    unsigned long position;
    /// Length of node
    unsigned long length;
};

enum SymbolType {
    SymbolTypeSpace,
    SymbolTypeType,
    SymbolTypeEnum,
    SymbolTypeFunc,
    SymbolTypeVar,
    SymbolTypeVal,
};
struct Scope {


};

typedef struct Scope Scope_t;
typedef struct ASTNode AST_t;
typedef struct ASTNode ASTNode_t;

struct ASTNodeScopeStmt {
    struct {
        char *name;
        enum SymbolType type;
    } *symbols;
    size_t symbols_count;
};

struct ASTNodeFuncDec {
    char* name;
    char* return_type;
    struct Scope *scope;

    struct ASTNode *args;
    struct ASTNode *body;

};

struct ASTNodeTypeDec {
    char* name;
    char* parent_type;
    struct Scope *scope;

    struct ASTNode *body;
};

struct ASTNodeVarDec {
    char* name;
    char* type;
    unsigned long alignment;
    struct ASTNode* value;
};
struct ASTNodeValDec {
    char* name;
    char* type;
    struct ASTNode* value;
};





struct node_file {
    struct ASTNode *nodes;
};

struct node_proj {
    struct ASTNode *files;
};

struct node_param {
    char* name;
    char* type;
};




//service
#define NODE_SRV_EOF                                                                                                   (0)
#define NODE_SRV_FILE                                                                                                 (-1)
#define NODE_SRV_PROJ                                                                                                 (-1)

//literal
/// NODE: IDENTIFIER
#define NODE_LIT_IDENTIFIER                                                                                           (-1)
/// NODE: NUMBER
#define NODE_LIT_NUMBER                                                                                               (-1)
/// NODE: STRING
#define NODE_LIT_STRING                                                                                               (-1)

//declaration
///NODE: '{' NODE* '}' | ';'
#define NODE_BODY                                                                                                (-1)
/// NODE: type IDENTIFIER (':' IDENTIFIER)? BODY
#define NODE_DEC_TYPE                                                                                                 (-1)
/// NODE: enum IDENTIFIER BODY
#define NODE_DEC_ENUM                                                                                                 (-1)
///NODE: func ID FUNC_ARGS ('>' ID)? BODY
#define NODE_DEC_FUNC                                                                                                 (-1)
///NODE: var ID (':' ID)?
#define NODE_DEC_VAR                                                                                             (99)
///NODE: val ID (':' ID)?
#define NODE_DEC_VAL                                                                                            (100)

//branch
#define NODE_STM_IF
#define NODE_STM_ELIF
#define NODE_STM_ELSE
#define NODE_STM_SWITCH
#define NODE_STM_CASE

//loop
#define NODE_STM_FOR
#define NODE_STM_WHILE
#define NODE_STM_UNTIL

//jump
#define NODE_JMP_NEXT
#define NODE_JMP_BACK
#define NODE_JMP_STOP
#define NODE_JMP_REPEAT
#define NODE_JMP_RETURN
#define NODE_JMP_EJECT
#define NODE_JMP_YIELD

//arithmetic
#define NODE_BIN_ADDITION                                                                                             (-1)
#define NODE_BIN_SUBTRACTION                                                                                          (-1)
#define NODE_BIN_MULTIPLICATION                                                                                       (-1)
#define NODE_BIN_DIVISION                                                                                             (-1)
#define NODE_BIN_MODULO                                                                                               (-1)
#define NODE_BIN_EXPONENTIATION                                                                                       (-1)
#define NODE_UNY_INCREMENT                                                                                            (-1)
#define NODE_UNY_DECREMENT                                                                                            (-1)

//relation
#define NODE_BIN_EQUAL                                                                                                (-1)
#define NODE_BIN_EQUAL_NOT                                                                                            (-1)
#define NODE_BIN_LESS                                                                                                 (-1)
#define NODE_BIN_LESS_OR_EQUAL                                                                                        (-1)
#define NODE_BIN_GREATER                                                                                              (-1)
#define NODE_BIN_GREATER_OR_EQUAL                                                                                     (-1)

//equality
/// TOKEN: eq | ==
#define NODE_BIN_EQ                                                                                                  (620)
/// TOKEN: !=
#define NODE_BIN_EQ_NOT                                                                                              (630)
/// TOKEN: is | ===
#define NODE_BIN_IS                                                                                                  (640)
/// TOKEN: !==
#define NODE_BIN_IS_NOT

//logical
#define NODE_BIN_OR                                                                                                   (-1)
#define NODE_BIN_AND                                                                                                  (-1)
#define NODE_UNY_NOT

//assign
/// TOKEN: node = assign
#define NODE_BIN_ASSIGN                                                                                              (232)
/// TOKEN: node += assign
#define NODE_BIN_ASSIGN_PLUS                                                                                         (233)
/// TOKEN: node -= assign
#define NODE_BIN_ASSIGN_MINUS                                                                                        (234)
/// TOKEN: node *= assign
#define NODE_BIN_ASSIGN_MULTIPLY                                                                                     (235)
/// TOKEN: node /= assign
#define NODE_BIN_ASSIGN_DIVIDE                                                                                       (236)
/// TOKEN: node %= assign
#define NODE_BIN_ASSIGN_MODULUS                                                                                      (237)