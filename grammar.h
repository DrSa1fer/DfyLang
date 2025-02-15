#ifndef GRAMMAR_H
#define GRAMMAR_H

//TOKEN
#ifndef IGNORE_TOKEN_GRAMMAR
  //service
  #define TOKEN_ERR                                                                                                 (-1)
  #define TOKEN_EOF                                                                                                  (0)

  /// REGEX: [_a-zA-Z][_a-zA-Z0-9]*
  #define TOKEN_IDENTIFIER                                                                                           (1)

  //literal
  /// REGEX: ((([0-9]([_][0-9])?)*\.([0-9]([_][0-9])?)+)|([0-9]([_][0-9])?)+))
  #define TOKEN_NUMBER                                                                                               (2)
  /// REGEX: (["'])(?:\\.|(?!\1).)*?\1
  #define TOKEN_STRING                                                                                               (3)
  /// REGEX: (true|false)
  #define TOKEN_BOOLEAN                                                                                              (4)

  /// REGEX: #.*\\n
  #define TOKEN_COMMENT                                                                                             (-1)

  //declaration
  /// TOKEN: var
  #define TOKEN_VAR                                                                                                (100)
  /// TOKEN: val
  #define TOKEN_VAL                                                                                                (110)
  /// TOKEN: type
  #define TOKEN_TYPE                                                                                               (120)
  /// TOKEN: enum
  #define TOKEN_ENUM                                                                                               (130)
  /// TOKEN: space
  #define TOKEN_SPACE                                                                                              (140)
  /// TOKEN: func
  #define TOKEN_FUNC                                                                                               (150)

  //branch
  /// TOKEN: if
  #define TOKEN_IF                                                                                                 (190)
  /// TOKEN: elif
  #define TOKEN_ELIF                                                                                               (200)
  /// TOKEN: else
  #define TOKEN_ELSE                                                                                               (210)
  /// TOKEN: switch
  #define TOKEN_SWITCH                                                                                             (220)
  /// TOKEN: case
  #define TOKEN_CASE                                                                                               (230)

  //loop
  /// TOKEN: for
  #define TOKEN_FOR                                                                                                (240)
  /// TOKEN: while
  #define TOKEN_WHILE                                                                                              (250)
  /// TOKEN: until
  #define TOKEN_UNTIL                                                                                              (260)

  //jump
  /// TOKEN: repeat
  #define TOKEN_REPEAT                                                                                             (270)
  /// TOKEN: next
  #define TOKEN_NEXT                                                                                               (280)
  /// TOKEN: last
  #define TOKEN_BACK                                                                                               (290)
  /// TOKEN: stop
  #define TOKEN_STOP                                                                                               (300)
  /// TOKEN: return
  #define TOKEN_RETURN                                                                                             (160)
  /// TOKEN: eject
  #define TOKEN_EJECT                                                                                              (170)
  /// TOKEN: yield
  #define TOKEN_YIELD                                                                                              (180)

  //exception
  /// TOKEN: try
  #define TOKEN_TRY                                                                                                (310)
  /// TOKEN: catch
  #define TOKEN_CATCH                                                                                              (320)
  /// TOKEN: finally
  #define TOKEN_FINALLY                                                                                            (330)
  /// TOKEN: throw
  #define TOKEN_THROW                                                                                              (340)

  //brace
  /// TOKEN: (
  #define TOKEN_PARENTHESIS_LEFT                                                                                   (400)
  /// TOKEN: )
  #define TOKEN_PARENTHESIS_RIGHT                                                                                  (410)
  /// TOKEN: [
  #define TOKEN_SQUARE_BRACKET_LEFT                                                                                (420)
  /// TOKEN: ]
  #define TOKEN_SQUARE_BRACKET_RIGHT                                                                               (430)
  /// TOKEN: {
  #define TOKEN_CURLY_BRACKET_LEFT                                                                                 (440)
  /// TOKEN: }
  #define TOKEN_CURLY_BRACKET_RIGHT                                                                                (450)
  /// TOKEN: <
  #define TOKEN_ANGEL_BRACKET_LEFT                                                                                 (460)
  /// TOKEN: >
  #define TOKEN_ANGEL_BRACKET_RIGHT                                                                                (470)

  //arithmetic
  /// TOKEN: +
  #define TOKEN_PLUS                                                                                               (480)
  /// TOKEN: -
  #define TOKEN_MINUS                                                                                              (490)
  /// TOKEN: *
  #define TOKEN_MULTIPLY                                                                                           (500)
  /// TOKEN: /
  #define TOKEN_DIVIDE                                                                                             (510)
  /// TOKEN: %
  #define TOKEN_MODULUS                                                                                            (550)
  /// TOKEN: **
  #define TOKEN_POWER                                                                                              (520)
  /// TOKEN: ++
  #define TOKEN_INCREMENT                                                                                          (530)
  /// TOKEN: --
  #define TOKEN_DECREMENT                                                                                          (540)

  //relation
  /// TOKEN: <
  #define TOKEN_LESS                                                                            TOKEN_ANGEL_BRACKET_LEFT
  /// TOKEN: <=
  #define TOKEN_LESS_OR_EQUAL                                                                                      (700)
  /// TOKEN: >
  #define TOKEN_GREATER                                                                        TOKEN_ANGEL_BRACKET_RIGHT
  /// TOKEN: >=
  #define TOKEN_GREATER_OR_EQUAL                                                                                   (720)

  //equal
  /// TOKEN: eq | ==
  #define TOKEN_EQ                                                                                                 (620)
  /// TOKEN: !=
  #define TOKEN_EQ_NOT                                                                                             (630)
  /// TOKEN: is | ===
  #define TOKEN_IS                                                                                                 (640)
  /// TOKEN: !==
  #define TOKEN_IS_NOT                                                                                             (650)

  //logical
  /// TOKEN: or | |
  #define TOKEN_OR                                                                                                 (660)
  /// TOKEN: and | &
  #define TOKEN_AND                                                                                                (670)
  /// TOKEN: not | !
  #define TOKEN_NOT                                                                                                (680)

  //assign
  /// TOKEN: =
  #define TOKEN_ASSIGN                                                                                             (730)
  /// TOKEN: +=
  #define TOKEN_ASSIGN_PLUS                                                                                        (740)
  /// TOKEN: -=
  #define TOKEN_ASSIGN_MINUS                                                                                       (750)
  /// TOKEN: *=
  #define TOKEN_ASSIGN_MULTIPLY                                                                                    (760)
  /// TOKEN: /=
  #define TOKEN_ASSIGN_DIVIDE                                                                                      (770)
  /// TOKEN: %=
  #define TOKEN_ASSIGN_MODULUS                                                                                     (780)

  //other
  /// TOKEN: ,
  #define TOKEN_COMMA                                                                                              (560)                                                                                       (-1)
  /// TOKEN: .
  #define TOKEN_DOT                                                                                                (570)
  /// TOKEN: ..
  #define TOKEN_RANGE                                                                                              (580)
  /// TOKEN: ...
  #define TOKEN_ELLIPSIS                                                                                           (590)
  /// TOKEN: :
  #define TOKEN_COLON                                                                                              (600)
  /// TOKEN: ;
  #define TOKEN_SEMICOLON                                                                                          (610)
  /// TOKEN: @
  #define TOKEN_DOG                                                                                                (611)

  //space
  /// TOKEN: ' '
  #define TOKEN_WHITESPACE                                                                                         (800)
  /// TOKEN: \\t
  #define TOKEN_TAB                                                                                                (810)
  /// TOKEN: \\n
  #define TOKEN_NEWLINE                                                                                            (820)
#endif // IGNORE_TOKEN_GRAMMAR


//NODE
#ifndef IGNORE_NODE_GRAMMAR
  //service
  #define NODE_ERR                                                                                                  (-1)
  #define NODE_EOF                                                                                                   (0)

  #define NODE_IDENTIFIER                                                                                           (-1)
  //literal
  ///REGEX:
  #define NODE_NUMBER                                                                                               (-1)
  //REGEX
  #define NODE_STRING                                                                                               (-1)
  #define NODE_BOOLEAN                                                                                              (-1)

  //declaration
  #define NODE_TYPE                                                                                                 (-1)
  #define NODE_ENUM                                                                                                 (-1)
  #define NODE_FUNC                                                                                                 (-1)
  #define NODE_VAR                                                                                                  (-1)
  #define NODE_VAL                                                                                                  (-1)

  //branch
  #define NODE_IF
  #define NODE_ELIF
  #define NODE_ELSE
  #define NODE_SWITCH
  #define NODE_CASE

  //loop
  #define NODE_FOR
  #define NODE_WHILE
  #define NODE_UNTIL

  //jump
  #define NODE_NEXT
  #define NODE_BACK
  #define NODE_STOP
  #define NODE_REPEAT
  #define NODE_RETURN
  #define NODE_EJECT
  #define NODE_YIELD

  //brace
  ///NODE: { node* }
  #define NODE_COMPOUND

  //arithmetic
  #define NODE_ADDITION                                                                                             (-1)
  #define NODE_SUBTRACTION                                                                                          (-1)
  #define NODE_MULTIPLICATION                                                                                       (-1)
  #define NODE_DIVISION                                                                                             (-1)
  #define NODE_MODULO                                                                                               (-1)
  #define NODE_POWER                                                                                                (-1)
  #define NODE_INCREMENT                                                                                            (-1)
  #define NODE_DECREMENT                                                                                            (-1)

  //relation
  #define NODE_EQUAL                                                                                                (-1)
  #define NODE_NOT_EQUAL                                                                                            (-1)
  #define NODE_LESS                                                                                                 (-1)
  #define NODE_LESS_OR_EQUAL                                                                                        (-1)
  #define NODE_GREATER                                                                                              (-1)
  #define NODE_GREATER_OR_EQUAL                                                                                     (-1)

  //equality
  /// TOKEN: eq | ==
  #define NODE_EQ                                                                                                  (620)
  /// TOKEN: !=
  #define NODE_EQ_NOT                                                                                              (630)
  /// TOKEN: is | ===
  #define NODE_IS                                                                                                  (640)
  /// TOKEN: !==
  #define NODE_IS_NOT

  //logical
  #define NODE_OR                                                                                                   (-1)
  #define NODE_AND                                                                                                  (-1)
  #define NODE_NOT

  //statement


  //assign
  /// TOKEN: node = assign
  #define NODE_ASSIGN                                                                                              (232)
  /// TOKEN: node += assign
  #define NODE_ASSIGN_PLUS                                                                                         (233)
  /// TOKEN: node -= assign
  #define NODE_ASSIGN_MINUS                                                                                        (234)
  /// TOKEN: node *= assign
  #define NODE_ASSIGN_MULTIPLY                                                                                     (235)
  /// TOKEN: node /= assign
  #define NODE_ASSIGN_DIVIDE                                                                                       (236)
  /// TOKEN: node %= assign
  #define NODE_ASSIGN_MODULUS                                                                                      (237)

#endif // IGNORE_NODE_GRAMMAR
#endif //GRAMMAR_H