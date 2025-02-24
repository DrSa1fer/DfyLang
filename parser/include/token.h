#pragma once

typedef struct token {
    ///Type of token
    int type;
    ///Data of token. If type is operator or keyword value NULL
    void *data;
} token_t;

#define TOKEN_TYPE_EOF                                                                                               (0)

//literal
/// REGEX: [_a-zA-Z][_a-zA-Z0-9]*
#define TOKEN_TYPE_LITERAL_IDENTIFIER                                                                                (1)
/// REGEX: ((([0-9]([_][0-9])?)*\.([0-9]([_][0-9])?)+)|([0-9]([_][0-9])?)+))
#define TOKEN_TYPE_LITERAL_NUMBER                                                                                    (2)
/// REGEX: (["'])(?:\\.|(?!\1).)*?\1
#define TOKEN_TYPE_LITERAL_STRING                                                                                    (3)

//declaration
/// TOKEN: var
#define TOKEN_TYPE_VAR                                                                                             (100)
/// TOKEN: val
#define TOKEN_TYPE_VAL                                                                                             (110)
/// TOKEN: type
#define TOKEN_TYPE_TYPE                                                                                            (120)
/// TOKEN: enum
#define TOKEN_TYPE_ENUM                                                                                            (130)
/// TOKEN: space
#define TOKEN_TYPE_SPACE                                                                                           (140)
/// TOKEN: func
#define TOKEN_TYPE_FUNC                                                                                            (150)

//branch
/// TOKEN: if
#define TOKEN_TYPE_IF                                                                                              (190)
/// TOKEN: elif
#define TOKEN_TYPE_ELIF                                                                                            (200)
/// TOKEN: else
#define TOKEN_TYPE_ELSE                                                                                            (210)
/// TOKEN: switch
#define TOKEN_TYPE_SWITCH                                                                                          (220)
/// TOKEN: case
#define TOKEN_TYPE_CASE                                                                                            (230)

//loop
/// TOKEN: for
#define TOKEN_TYPE_FOR                                                                                             (240)
/// TOKEN: while
#define TOKEN_TYPE_WHILE                                                                                           (250)
/// TOKEN: until
#define TOKEN_TYPE_UNTIL                                                                                           (260)

//jump
/// TOKEN: repeat
#define TOKEN_TYPE_REPEAT                                                                                          (270)
/// TOKEN: next
#define TOKEN_TYPE_NEXT                                                                                            (280)
/// TOKEN: last
#define TOKEN_TYPE_BACK                                                                                            (290)
/// TOKEN: stop
#define TOKEN_TYPE_STOP                                                                                            (300)
/// TOKEN: return

#define TOKEN_TYPE_RETURN                                                                                          (160)
/// TOKEN: eject
#define TOKEN_TYPE_EJECT                                                                                           (170)
/// TOKEN: yield
#define TOKEN_TYPE_YIELD                                                                                           (180)

//exception
/// TOKEN: try
#define TOKEN_TYPE_TRY                                                                                             (310)
/// TOKEN: catch
#define TOKEN_TYPE_CATCH                                                                                           (320)
/// TOKEN: finally
#define TOKEN_TYPE_FINALLY                                                                                         (330)
/// TOKEN: throw
#define TOKEN_TYPE_THROW                                                                                           (340)

//brace
/// TOKEN: (
#define TOKEN_TYPE_PARENTHESIS_LEFT                                                                                (400)
/// TOKEN: )
#define TOKEN_TYPE_PARENTHESIS_RIGHT                                                                               (410)
/// TOKEN: [
#define TOKEN_TYPE_SQUARE_BRACKET_LEFT                                                                             (420)
/// TOKEN: ]
#define TOKEN_TYPE_SQUARE_BRACKET_RIGHT                                                                            (430)
/// TOKEN: {
#define TOKEN_TYPE_CURLY_BRACKET_LEFT                                                                              (440)
/// TOKEN: }
#define TOKEN_TYPE_CURLY_BRACKET_RIGHT                                                                             (450)
/// TOKEN: <
#define TOKEN_TYPE_ANGEL_BRACKET_LEFT                                                                              (460)
/// TOKEN: >
#define TOKEN_TYPE_ANGEL_BRACKET_RIGHT                                                                             (470)

//arithmetic
/// TOKEN: +
#define TOKEN_TYPE_PLUS                                                                                            (480)
/// TOKEN: -
#define TOKEN_TYPE_MINUS                                                                                           (490)
/// TOKEN: *
#define TOKEN_TYPE_MULTIPLY                                                                                        (500)
/// TOKEN: /
#define TOKEN_TYPE_DIVIDE                                                                                          (510)
/// TOKEN: %
#define TOKEN_TYPE_MODULUS                                                                                         (550)
/// TOKEN: **
#define TOKEN_TYPE_POWER                                                                                           (520)
/// TOKEN: ++
#define TOKEN_TYPE_INCREMENT                                                                                       (530)
/// TOKEN: --
#define TOKEN_TYPE_DECREMENT                                                                                       (540)

//relation
/// TOKEN: <
#define TOKEN_TYPE_LESS                                                                    TOKEN_TYPE_ANGEL_BRACKET_LEFT
/// TOKEN: <=
#define TOKEN_TYPE_LESS_OR_EQUAL                                                                                   (700)
/// TOKEN: >
#define TOKEN_TYPE_GREATER                                                                TOKEN_TYPE_ANGEL_BRACKET_RIGHT
/// TOKEN: >=
#define TOKEN_TYPE_GREATER_OR_EQUAL                                                                                (720)

//equal
/// TOKEN: eq | ==
#define TOKEN_TYPE_EQ                                                                                              (620)
/// TOKEN: !=
#define TOKEN_TYPE_EQ_NOT                                                                                          (630)
/// TOKEN: is | ===
#define TOKEN_TYPE_IS                                                                                              (640)
/// TOKEN: !==
#define TOKEN_TYPE_IS_NOT                                                                                          (650)

//logical
/// TOKEN: or | |
#define TOKEN_TYPE_OR                                                                                              (660)
/// TOKEN: and | &
#define TOKEN_TYPE_AND                                                                                             (670)
/// TOKEN: not | !
#define TOKEN_TYPE_NOT                                                                                             (680)

//assign
/// TOKEN: =
#define TOKEN_TYPE_ASSIGN                                                                                          (730)
/// TOKEN: +=
#define TOKEN_TYPE_ASSIGN_PLUS                                                                                     (740)
/// TOKEN: -=
#define TOKEN_TYPE_ASSIGN_MINUS                                                                                    (750)
/// TOKEN: *=
#define TOKEN_TYPE_ASSIGN_MULTIPLY                                                                                 (760)
/// TOKEN: /=
#define TOKEN_TYPE_ASSIGN_DIVIDE                                                                                   (770)
/// TOKEN: %=
#define TOKEN_TYPE_ASSIGN_MODULUS                                                                                  (780)

//other
/// TOKEN: ,
#define TOKEN_TYPE_COMMA                                                                                           (560)
/// TOKEN: .
#define TOKEN_TYPE_DOT                                                                                             (570)
/// TOKEN: ..
#define TOKEN_TYPE_RANGE                                                                                           (580)
/// TOKEN: ...
#define TOKEN_TYPE_ELLIPSIS                                                                                        (590)
/// TOKEN: :
#define TOKEN_TYPE_COLON                                                                                           (600)
/// TOKEN: ;
#define TOKEN_TYPE_SEMICOLON                                                                                       (610)
/// TOKEN: @
#define TOKEN_TYPE_DOG                                                                                             (611)

// trivia
/// REGEX: #.*\\n
#define TOKEN_TYPE_TRIVIA_COMMENT                                                                                  (790)
/// TOKEN: ' '
#define TOKEN_TYPE_TRIVIA_SPACE                                                                                    (800)
/// TOKEN: \\t
#define TOKEN_TYPE_TRIVIA_TAB                                                                                      (810)
/// TOKEN: \\n
#define TOKEN_TYPE_TRIVIA_NEWLINE                                                                                  (820)