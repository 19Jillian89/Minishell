## 🐚 Minishell – 42 Project

## 1. Introduction
Minishell is a simplified Unix shell implemented as part of the 42 School curriculum.
The goal is to reproduce the core behavior of a shell, including parsing, variable expansion,
command execution, builtins, signals, and environment handling.

The project is designed with a clear separation between parsing and execution,
ensuring modularity and easier debugging.

```
2. Project Architecture
minishell/
├── parser/
├── execution/
├── builtin/
├── builder/
├── signal/
├── includes/
├── libft/
├── handle_input.c
└── main.c
```
---
 🛠️ Build
```
make
```

Run:
```
./minishell
```
---
## 3. Execution Flow

➊ Read user input  
➋ Tokenize input  
➌ Handle quote removal and variable expansion  
➍ Parse tokens  
➎ Build command structures  
➏ Execute commands  
➐ Update exit status and handle signals  

---
## 4. Execution / Builtin / Builder / Signal
## Responsibilities

This part of the project is responsible for command execution, assuming that parsing and variable expansion have already been completed correctly.

## Modules
`execution/`

• Process creation (fork)

• Pipe handling

• Redirections management

• Execution of external commands via execve

• Exit status propagation

`builtin/`

• Implementation of shell built-in commands

• Execution in the parent process when required (cd, exit, export, unset)

• Execution in the child process when part of a pipeline


`builder/`

• Construction of final data structures used for execution

• Linking commands, redirections, and file descriptors

`signal/`

• Signal handling based on the shell state.

• Differentiation between interactive mode and execution mode

## Execution Logic

• Builtins without pipes → executed in the parent process

• Builtins with pipes → executed in the child process

• External commands → fork + execve

• Pipes and redirections are set up before execution

## 5. Parser Phases
## Tokenization

• Split user input into tokens

• Identify operators (|, <, >, <<, >>)

• Preserve quote context

## Quote Handling

• Remove quotes

• Distinguish between single and double quotes

• Preserve literal content

## Expansion

• Expand `$VAR` and `$?` 

• No expansion inside single quotes

• No expansion on heredoc delimiters

## Parsing

• Build command structures

• Associate pipes and redirections

---

## 6. Expansion Handling
This section details the expansion behavior implemented during the parsing phase.

Expansion is handled during the parsing phase, immediately after tokenization.
Expansion is fully handled by the parser before any command execution occurs.

Implemented rules:

• Single quotes: no expansion  
• Double quotes: `$VAR` and `$?` expansion  
• Unquoted text: full expansion  
• Undefined variables → empty string  
• `$?` → last exit status  
• Heredoc delimiter → no expansion  

## 6. Work Distribution

• Parser: tokenization, quote handling, expansion, and command structure creation  

Fabio Vitharana
- Parsing layer implementation
- Tokenization and syntax analysis
- Quote handling and variable expansion
- Built-in command logic

• Execution / Builtin / Builder / Signal: process execution and runtime behavior  

Ilaria Nassi
- Execution layer implementation
- Process creation and management
- Pipes and redirections handling
- Signal handling and exit status management
