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

## 4. Parser Phases
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

## 5. Expansion Handling
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
