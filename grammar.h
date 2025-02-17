#ifndef GRAMMAR_H
#define GRAMMAR_H

//TOKEN
#ifndef IGNORE_TOKEN_TYPE_GRAMMAR
  //service
  #define TOKEN_TYPE_EOF                                                                                                  (0)

  /// REGEX: [_a-zA-Z][_a-zA-Z0-9]*
  #define TOKEN_TYPE_IDENTIFIER                                                                                           (1)

  //literal
  /// REGEX: ((([0-9]([_][0-9])?)*\.([0-9]([_][0-9])?)+)|([0-9]([_][0-9])?)+))
  #define TOKEN_TYPE_NUMBER                                                                                               (2)
  /// REGEX: (["'])(?:\\.|(?!\1).)*?\1
  #define TOKEN_TYPE_STRING                                                                                               (3)
  /// REGEX: (true|false)
  #define TOKEN_TYPE_BOOLEAN                                                                                              (4)

  /// REGEX: #.*\\n
  #define TOKEN_TYPE_COMMENT                                                                                              (5)

  //declaration
  /// TOKEN: var
  #define TOKEN_TYPE_VAR                                                                                                (100)
  /// TOKEN: val
  #define TOKEN_TYPE_VAL                                                                                                (110)
  /// TOKEN: type
  #define TOKEN_TYPE_TYPE                                                                                               (120)
  /// TOKEN: enum
  #define TOKEN_TYPE_ENUM                                                                                               (130)
  /// TOKEN: space
  #define TOKEN_TYPE_NAMESPACE                                                                                              (140)
  /// TOKEN: func
  #define TOKEN_TYPE_FUNC                                                                                               (150)

  //branch
  /// TOKEN: if
  #define TOKEN_TYPE_IF                                                                                                 (190)
  /// TOKEN: elif
  #define TOKEN_TYPE_ELIF                                                                                               (200)
  /// TOKEN: else
  #define TOKEN_TYPE_ELSE                                                                                               (210)
  /// TOKEN: switch
  #define TOKEN_TYPE_SWITCH                                                                                             (220)
  /// TOKEN: case
  #define TOKEN_TYPE_CASE                                                                                               (230)

  //loop
  /// TOKEN: for
  #define TOKEN_TYPE_FOR                                                                                                (240)
  /// TOKEN: while
  #define TOKEN_TYPE_WHILE                                                                                              (250)
  /// TOKEN: until
  #define TOKEN_TYPE_UNTIL                                                                                              (260)

  //jump
  /// TOKEN: repeat
  #define TOKEN_TYPE_REPEAT                                                                                             (270)
  /// TOKEN: next
  #define TOKEN_TYPE_NEXT                                                                                               (280)
  /// TOKEN: last
  #define TOKEN_TYPE_BACK                                                                                               (290)
  /// TOKEN: stop
  #define TOKEN_TYPE_STOP                                                                                               (300)
  /// TOKEN: return
  #define TOKEN_TYPE_RETURN                                                                                             (160)
  /// TOKEN: eject
  #define TOKEN_TYPE_EJECT                                                                                              (170)
  /// TOKEN: yield
  #define TOKEN_TYPE_YIELD                                                                                              (180)

  //exception
  /// TOKEN: try
  #define TOKEN_TYPE_TRY                                                                                                (310)
  /// TOKEN: catch
  #define TOKEN_TYPE_CATCH                                                                                              (320)
  /// TOKEN: finally
  #define TOKEN_TYPE_FINALLY                                                                                            (330)
  /// TOKEN: throw
  #define TOKEN_TYPE_THROW                                                                                              (340)

  //brace
  /// TOKEN: (
  #define TOKEN_TYPE_PARENTHESIS_LEFT                                                                                   (400)
  /// TOKEN: )
  #define TOKEN_TYPE_PARENTHESIS_RIGHT                                                                                  (410)
  /// TOKEN: [
  #define TOKEN_TYPE_SQUARE_BRACKET_LEFT                                                                                (420)
  /// TOKEN: ]
  #define TOKEN_TYPE_SQUARE_BRACKET_RIGHT                                                                               (430)
  /// TOKEN: {
  #define TOKEN_TYPE_CURLY_BRACKET_LEFT                                                                                 (440)
  /// TOKEN: }
  #define TOKEN_TYPE_CURLY_BRACKET_RIGHT                                                                                (450)
  /// TOKEN: <
  #define TOKEN_TYPE_ANGEL_BRACKET_LEFT                                                                                 (460)
  /// TOKEN: >
  #define TOKEN_TYPE_ANGEL_BRACKET_RIGHT                                                                                (470)

  //arithmetic
  /// TOKEN: +
  #define TOKEN_TYPE_PLUS                                                                                               (480)
  /// TOKEN: -
  #define TOKEN_TYPE_MINUS                                                                                              (490)
  /// TOKEN: *
  #define TOKEN_TYPE_MULTIPLY                                                                                           (500)
  /// TOKEN: /
  #define TOKEN_TYPE_DIVIDE                                                                                             (510)
  /// TOKEN: %
  #define TOKEN_TYPE_MODULUS                                                                                            (550)
  /// TOKEN: **
  #define TOKEN_TYPE_POWER                                                                                              (520)
  /// TOKEN: ++
  #define TOKEN_TYPE_INCREMENT                                                                                          (530)
  /// TOKEN: --
  #define TOKEN_TYPE_DECREMENT                                                                                          (540)

  //relation
  /// TOKEN: <
  #define TOKEN_TYPE_LESS                                                                            TOKEN_TYPE_ANGEL_BRACKET_LEFT
  /// TOKEN: <=
  #define TOKEN_TYPE_LESS_OR_EQUAL                                                                                      (700)
  /// TOKEN: >
  #define TOKEN_TYPE_GREATER                                                                        TOKEN_TYPE_ANGEL_BRACKET_RIGHT
  /// TOKEN: >=
  #define TOKEN_TYPE_GREATER_OR_EQUAL                                                                                   (720)

  //equal
  /// TOKEN: eq | ==
  #define TOKEN_TYPE_EQ                                                                                                 (620)
  /// TOKEN: !=
  #define TOKEN_TYPE_EQ_NOT                                                                                             (630)
  /// TOKEN: is | ===
  #define TOKEN_TYPE_IS                                                                                                 (640)
  /// TOKEN: !==
  #define TOKEN_TYPE_IS_NOT                                                                                             (650)

  //logical
  /// TOKEN: or | |
  #define TOKEN_TYPE_OR                                                                                                 (660)
  /// TOKEN: and | &
  #define TOKEN_TYPE_AND                                                                                                (670)
  /// TOKEN: not | !
  #define TOKEN_TYPE_NOT                                                                                                (680)

  //assign
  /// TOKEN: =
  #define TOKEN_TYPE_ASSIGN                                                                                             (730)
  /// TOKEN: +=
  #define TOKEN_TYPE_ASSIGN_PLUS                                                                                        (740)
  /// TOKEN: -=
  #define TOKEN_TYPE_ASSIGN_MINUS                                                                                       (750)
  /// TOKEN: *=
  #define TOKEN_TYPE_ASSIGN_MULTIPLY                                                                                    (760)
  /// TOKEN: /=
  #define TOKEN_TYPE_ASSIGN_DIVIDE                                                                                      (770)
  /// TOKEN: %=
  #define TOKEN_TYPE_ASSIGN_MODULUS                                                                                     (780)

  //other
  /// TOKEN: ,
  #define TOKEN_TYPE_COMMA                                                                                              (560)
  /// TOKEN: .
  #define TOKEN_TYPE_DOT                                                                                                (570)
  /// TOKEN: ..
  #define TOKEN_TYPE_RANGE                                                                                              (580)
  /// TOKEN: ...
  #define TOKEN_TYPE_ELLIPSIS                                                                                           (590)
  /// TOKEN: :
  #define TOKEN_TYPE_COLON                                                                                              (600)
  /// TOKEN: ;
  #define TOKEN_TYPE_SEMICOLON                                                                                          (610)
  /// TOKEN: @
  #define TOKEN_TYPE_DOG                                                                                                (611)

  //space
  /// TOKEN: ' '
  #define TOKEN_TYPE_SPACE                                                                                         (800)
  /// TOKEN: \\t
  #define TOKEN_TYPE_TAB                                                                                                (810)
  /// TOKEN: \\n
  #define TOKEN_TYPE_NEWLINE                                                                                            (820)
#endif // IGNORE_TOKEN_TYPE_GRAMMAR


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