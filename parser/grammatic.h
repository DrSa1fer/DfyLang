#pragma once


#define TOKEN_ERR                                                                                                   (-1)
#define TOKEN_EOF                                                                                                    (0)

/// REGEX: [_a-zA-Z][_a-zA-Z0-9]*
#define TOKEN_IDENTIFIER                                                                                             (1)
/// REGEX: ((([0-9]([_][0-9])?)*\.([0-9]([_][0-9])?)+)|([0-9]([_][0-9])?)+))
#define TOKEN_NUMBER                                                                                                 (2)
/// REGEX: (["'])(?:\\.|(?!\1).)*?\1
#define TOKEN_STRING                                                                                                 (3)
/// REGEX: #.*\\n
#define TOKEN_COMMENT                                                                                               (-1)

/// TOKEN: var
#define TOKEN_VAR                                                                                                  (100)
/// TOKEN: val
#define TOKEN_VAL                                                                                                  (110)
/// TOKEN: type
#define TOKEN_TYPE                                                                                                 (120)
/// TOKEN: enum
#define TOKEN_ENUM                                                                                                 (130)
/// TOKEN: space
#define TOKEN_SPACE                                                                                                (140)
/// TOKEN: func
#define TOKEN_FUNC                                                                                                 (150)
/// TOKEN: return
#define TOKEN_RETURN                                                                                               (160)
/// TOKEN: eject
#define TOKEN_EJECT                                                                                                (170)
/// TOKEN: yield
#define TOKEN_YIELD                                                                                                (180)

//branch
/// TOKEN: if
#define TOKEN_IF                                                                                                   (190)
/// TOKEN: elif
#define TOKEN_ELIF                                                                                                 (200)
/// TOKEN: else
#define TOKEN_ELSE                                                                                                 (210)
/// TOKEN: switch
#define TOKEN_SWITCH                                                                                               (220)
/// TOKEN: case
#define TOKEN_CASE                                                                                                 (230)

//loop
/// TOKEN: for
#define TOKEN_FOR                                                                                                  (240)
/// TOKEN: while
#define TOKEN_WHILE                                                                                                (250)
/// TOKEN: until
#define TOKEN_UNTIL                                                                                                (260)
/// TOKEN: repeat
#define TOKEN_REPEAT                                                                                               (270)
/// TOKEN: next
#define TOKEN_NEXT                                                                                                 (280)
/// TOKEN: last
#define TOKEN_BACK                                                                                                 (290)
/// TOKEN: stop
#define TOKEN_STOP                                                                                                 (300)

/// TOKEN: try
#define TOKEN_TRY                                                                                                  (310)
/// TOKEN: catch
#define TOKEN_CATCH                                                                                                (320)
/// TOKEN: finally
#define TOKEN_FINALLY                                                                                              (330)
/// TOKEN: throw
#define TOKEN_THROW                                                                                                (340)

//brace
/// TOKEN: (
#define TOKEN_PARENTHESIS_LEFT                                                                                     (400)
/// TOKEN: )
#define TOKEN_PARENTHESIS_RIGHT                                                                                    (410)
/// TOKEN: [
#define TOKEN_SQUARE_BRACKET_LEFT                                                                                  (420)
/// TOKEN: ]
#define TOKEN_SQUARE_BRACKET_RIGHT                                                                                 (430)
/// TOKEN: {
#define TOKEN_CURLY_BRACKET_LEFT                                                                                   (440)
/// TOKEN: }
#define TOKEN_CURLY_BRACKET_RIGHT                                                                                  (450)
/// TOKEN: <
#define TOKEN_ANGEL_BRACKET_LEFT                                                                                   (460)
/// TOKEN: >
#define TOKEN_ANGEL_BRACKET_RIGHT                                                                                  (470)

//arithmetic
/// TOKEN: +
#define TOKEN_PLUS                                                                                                 (480)
/// TOKEN: -
#define TOKEN_MINUS                                                                                                (490)
/// TOKEN: *
#define TOKEN_MULTIPLY                                                                                             (500)
/// TOKEN: /
#define TOKEN_DIVIDE                                                                                               (510)
/// TOKEN: **
#define TOKEN_EXPONENT                                                                                             (520)
/// TOKEN: ++
#define TOKEN_INCREMENT                                                                                            (530)
/// TOKEN: --
#define TOKEN_DECREMENT                                                                                            (540)
/// TOKEN: %
#define TOKEN_MODULUS                                                                                              (550)
/// TOKEN: ,
#define TOKEN_COMMA                                                                                                (560)                                                                                       (-1)
/// TOKEN: .
#define TOKEN_DOT                                                                                                  (570)
/// TOKEN: ..
#define TOKEN_RANGE                                                                                                (580)
/// TOKEN: ...
#define TOKEN_ELLIPSIS                                                                                             (590)
/// TOKEN: :
#define TOKEN_COLON                                                                                                (600)
/// TOKEN: ;
#define TOKEN_SEMICOLON                                                                                            (610)

/// TOKEN: eq | ==
#define TOKEN_EQ                                                                                                   (620)
/// TOKEN: !=
#define TOKEN_EQ_NOT                                                                                               (630)
/// TOKEN: is | ===
#define TOKEN_IS                                                                                                   (640)
/// TOKEN: !==
#define TOKEN_IS_NOT                                                                                               (650)

//logical
/// TOKEN: or | |
#define TOKEN_OR                                                                                                   (660)
/// TOKEN: and | &
#define TOKEN_AND                                                                                                  (670)
/// TOKEN: not | !
#define TOKEN_NOT                                                                                                  (680)

//relational
/// TOKEN: <
#define TOKEN_LESS                                                                              TOKEN_ANGEL_BRACKET_LEFT
/// TOKEN: <=
#define TOKEN_LESS_OR_EQUAL                                                                                        (700)
/// TOKEN: >
#define TOKEN_GREATER                                                                          TOKEN_ANGEL_BRACKET_RIGHT
/// TOKEN: >=
#define TOKEN_GREATER_OR_EQUAL                                                                                     (720)

//assign
/// TOKEN: %=
#define TOKEN_ASSIGN                                                                                               (730)
/// TOKEN: +=
#define TOKEN_PLUS_ASSIGN                                                                                          (740)
/// TOKEN: -=
#define TOKEN_MINUS_ASSIGN                                                                                         (750)
/// TOKEN: *=
#define TOKEN_MULTIPLY_ASSIGN                                                                                      (760)
/// TOKEN: /=
#define TOKEN_DIVIDE_ASSIGN                                                                                        (770)
/// TOKEN: %=
#define TOKEN_MODULUS_ASSIGN                                                                                       (780)

//space
/// TOKEN: ' '
#define TOKEN_SPACE_CHAR                                                                                           (800)
/// TOKEN: '\\t'
#define TOKEN_TAB_CHAR                                                                                             (810)
/// TOKEN: '\\n'
#define TOKEN_NEWLINE_CHAR                                                                                         (820)










//NODE//

#define NODE_ERR ,
#define NODE_EOF ,

#define NODE_NUMBER ,
#define NODE_STRING ,

#define NODE_MINUS ,
#define NODE_PLUS ,
#define NODE_MULTIPLY ,
#define NODE_DIVIDE ,
#define NODE_MODULO ,
#define NODE_INCREMENT ,
#define NODE_DECREMENT ,

#define NODE_OR ,
#define NODE_AND ,
#define NODE_NOT ,

#define NODE_EQUAL ,
#define NODE_NOT_EQUAL ,
#define NODE_LESS ,
#define NODE_LESS_OR_EQUAL ,
#define NODE_GREATER ,
#define NODE_GREATER_OR_EQUAL ,

///NODE: { node* }
#define NODE_COMPOUND

//branch

#define NODE_IF
#define NODE_ELSE

#define NODE_SWITCH
#define NODE_CASE

//loop

#define NODE_WHILE
#define NODE_FOR
#define NODE_UNTIL

//jump

#define NODE_NEXT
#define NODE_BACK
#define NODE_STOP
#define NODE_REPEAT

#define NODE_RETURN
#define NODE_EJECT

//declaration

#define NODE_TYPE
#define NODE_ENUM
#define NODE_FUNC
#define NODE_VAR
#define NODE_VAL

/// TOKEN: node = node
#define NODE_ASSIGN                                                                                               (232)
/// TOKEN: node += node
#define NODE_PLUS_ASSIGN                                                                                          (233)
/// TOKEN: node -= node
#define NODE_MINUS_ASSIGN                                                                                         (234)
/// TOKEN: node *= node
#define NODE_MULTIPLY_ASSIGN                                                                                      (235)
/// TOKEN: node /= node
#define NODE_DIVIDE_ASSIGN                                                                                        (236)
/// TOKEN: node %= node
#define NODE_MODULUS_ASSIGN                                                                                       (237)

// NODE_FUNC_CALL,
// NODE_FUNC_DEC,
// NODE_VAR_DEC,
// NODE_VAL_DEC,
//
// NODE_VAR_ASSIGN,
//
// NODE_RETURN,
// NODE_YIELD,
// NODE_IF,
// NODE_WHILE,
// NODE_UNTIL,
// NODE_FOR,
// NODE_CONTINUE,
// NODE_NEXT,
// NODE_LAST,
// NODE_STOP,


//

//


//
