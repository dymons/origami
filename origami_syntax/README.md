## Context-free grammar C++

```
program := function-definition

empty := ''

--- Description function
function-definition := (attribute-specifier-seq | empty) (decl-specifier-seq | empty) declarator (virt-specifier-seq | requires-clause) function-body

attribute-specifier-seq := (attribute-specifier-seq | empty) attribute-specifier

attribute-specifier := [[(attribute-using-prefixopt | empty) attribute-list]] 
                     | alignment-specifier

attribute-list := (attribute | attribute ...) (COMMA (attribute | attribute ...))*

--- Description identifier
identifier := identifier-nondigit 
            | identifier identifier-nondigit 
            | identifier digit

identifier-nondigit := nondigit
                     | universal-character-name

universal-character-name := \u hex-quad
                          | \U hex-quad hex-quad
                          
hex-quad := hexadecimal-digit hexadecimal-digit hexadecimal-digit hexadecimal-digit

hexadecimal-digit := digit | a | b | c | d | e | f | A | B | C | D | E | F

nondigit := a | b | c | d | e | f | g | h | i | j | k | l | m |
            n | o | p | q | r | s | t | u | v | w | x | y | z |
            A | B | C | D | E | F | G | H | I | J | K | L | M |
            N | O | P | Q | R | S | T | U | V | W | X | Y | Z | _

digit := 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
```

## Draft version of Context-free grammar C++
![Context-free grammar C++](../origami_doc/context_free_grammar_cpp.png "Context-free grammar C++")
