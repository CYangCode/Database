grammar MyElang;

options {
  language = C;
  //backtrack = true;
  //k=1;
  memoize  = true;
  output   = AST;
}

start_rule
  :
  (select_expression)* EOF
  ;

select_expression
  :
  'SELECT'
  (
    'DISTINCT'
    | 'UNIQUE'
    | 'ALL'
  )?
  (
    (
      ASTERISK
      | displayed_column (COMMA displayed_column)*
    )
    'FROM' table_spec (where_clause)? (LPAREN select_expression RPAREN)? (order_by_clause)?
  )
  ;

displayed_column
  :
  (
    column_name
    // | aggregate_function
    | (LPAREN select_expression RPAREN)?
  )
  (alias)?
  ; 

column_name
  :
  ID
  | '`' ID '`'
  ;
//�ݲ�ʵ��

aggregate_function: ;
//����

alias
  :
  ('AS') column_name
  ;

table_spec
  :
  (schema_name DOT)? table_name
  ;

schema_name
  :
  ID
  ;

table_name
  :
  ID
  ;
//where�������

where_clause
  :
  'WHERE' sql_condition
  ;
//sql������

sql_condition
  :
  condition_or
  | LPAREN condition_or RPAREN
  ;

//������

condition_or
  :
  (condition_and) ('OR' condition_and)*
  ;
//������

condition_and
  :
  (condition_not) ('AND' condition_not)*
  ;

//������

condition_not
  :
  'NOT' condition_expr
  | condition_expr
  ;
//����

condition_expr
  :
  condition_comparison
  | condition_between
  | condition_is_empty
  ;
//�˴��ڴ���С�ںź�Ӧ��һ�����ʽ����ʵ��

condition_comparison
  :
  column_name
  (
    EQ
    | NOT_EQ
    | GTH
    | GEQ
    | LTH
    | LEQ
  )
  (NUMBER
  |'"'ID'"')
  ;

condition_between
  :
  column_name ('NOT')? 'BETWEEN' NUMBER 'AND' NUMBER
  ;

condition_is_empty
  :
  column_name 'IS' ('NOT')? 'EMPTY'
  ;

order_by_clause
  :
  'ORDER' 'BY' (column_name
  //    | position
  //    | column_alias
  )
  (
    'ASC'
    | 'DESC'
  )?
  ;

ASTERISK
  :
  '*'
  ;

COMMA
  :
  ','
  ;

ID
  :
  'A'..'Z'
  (
    'A'..'Z'
    | '0'..'9'
    | '_'
    | '$'
    | '#'
  )*
  ;

DOT
  :
  '.'
  ;

EQ
  :
  '='
  ;

NOT_EQ
  :
  '<>'
  | '!='
  | '^='
  ;

GTH
  :
  '>'
  ;

GEQ
  :
  '>='
  ;

LTH
  :
  '<'
  ;

LEQ
  :
  '<='
  ;

NUMBER
  :
  ( PLUS | MINUS )?
  (
    (('0'..'9' ('0'..'9')*) DOT ('0'..'9' ('0'..'9')*)) => ('0'..'9' ('0'..'9')*) DOT ('0'..'9' ('0'..'9')*)
    | DOT ('0'..'9' ('0'..'9')*)
    | ('0'..'9' ('0'..'9')*)
  )
  (
    'E'
    (
      PLUS
      | MINUS
    )?
    '0'..'9' ('0'..'9')*
  )?
  ;

PLUS
  :
  '+'
  ;
  

MINUS
  :
  '-'
  ;

LPAREN
  :
  '('
  ;

RPAREN
  :
  ')'
  ;
