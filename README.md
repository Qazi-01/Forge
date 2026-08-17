# Forge

Forge is a small programming language and toolchain written in C.

## v0.3.0 - Parser + Control Flow

Forge v0.3.0 introduces the parser, allowing Forge to turn its lexer token stream into structured syntax.

This release moves Forge beyond tokenization and into actual language parsing.

### Features

* Recursive-descent parser
* Expression parsing
* Operator precedence
* Unary operators
* Arithmetic expressions
* Comparison expressions
* Equality expressions
* Parenthesized expressions
* Variable declarations with `let`
* Expression statements
* Blocks
* `if` statements
* `else` branches
* `while` loops
* Function declarations
* Function parameters
* Function calls
* Function arguments
* `return` statements
* Nested control flow
* Nested blocks
* Parser error handling
* Parser source locations
* Syntax validation
* Non-zero exit codes for invalid programs

## Example

Forge source:

```forge
fn add(a, b)
{
    return a + b;
}

let x = 10;
let y = 20;
let result = add(x, y);

if (result > 20)
{
    result + 1;
}
else
{
    result - 1;
}
```

Parse it with:

```bash
./build/forge.exe parse examples/program.fg
```

Forge reports:

```text
Parse successful!
```

## Expressions

Forge supports arithmetic, comparison, equality, and unary expressions.

```forge
1 + 2 * 3;
(1 + 2) * 3;

x < 10;
x == 10;
x != 20;

-x;
!true;
```

Operator precedence is handled by the parser.

## Variables

Variables can be declared with `let`:

```forge
let x = 10;
let y = x + 20;
```

## Control Flow

Forge supports conditional statements:

```forge
if (x > 10)
{
    x + 1;
} 
else
{
    x - 1;
}
```

and loops:

```forge
while (x > 0) 
{
    x - 1;
}
```

Control-flow statements can be nested:

```forge
if (x > 10)
{
    while (x > 0)
    {
        if (x == 5)
        {
            return x;
        }

        x - 1;
    }
}
```

## Functions

Functions can define parameters and return values:

```forge
fn add(a, b)
{
    return a + b;
}

let result = add(10, 20);
```

Function calls support multiple arguments and nested expressions:

```forge
add(1 + 2, x * 5);
```

## Parser Diagnostics

Invalid syntax produces a parser error instead of silently accepting the program.

For example:

```forge
let x = 10
```

produces an error because the semicolon is missing.

Parser errors include the source location of the problem.

## CLI

Show available commands:

```bash
./build/forge.exe help
```

Show the Forge version:

```bash
./build/forge.exe version
```

Tokenize a source file:

```bash
./build/forge.exe tokenize examples/program.fg
```

Parse a source file:

```bash
./build/forge.exe parse examples/program.fg
```

Current version:

```text
Forge version 0.3.0
```

## Building

Forge currently uses CMake, Ninja, and GCC through MSYS2 UCRT64.

```bash
cmake -S . -B build
cmake --build build
```

Run Forge:

```bash
./build/forge.exe
```

## Project Status

Forge is under active development.

Current release:

**v0.3.0 - Parser + Control Flow**

Forge currently has a working lexer, CLI, diagnostics system, and recursive-descent parser capable of parsing expressions, declarations, control flow, functions, and nested program structures.

The next major milestone is constructing an Abstract Syntax Tree (AST) from the parsed source.

## Roadmap

* [x] v0.1.0 - Lexer Foundation
* [x] v0.2.0 - CLI + Diagnostics
* [x] v0.3.0 - Parser + Control Flow
* [ ] v0.4.0 - AST + Pretty Printer
* [ ] v0.5.0 - Interpreter
* [ ] v0.6.0 - Runtime + Standard Library
* [ ] v0.7.0 - Testing + Tooling
* [ ] v0.8.0 - Compiler / Code Generation
* [ ] v1.0.0 - Polish + Documentation + Demo

Forge is being built incrementally, with each release producing a working improvement over the previous version.