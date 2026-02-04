# Minishell

![Language](https://img.shields.io/badge/language-C-blue)
![Status](https://img.shields.io/badge/status-in%20progress-yellow)
![42](https://img.shields.io/badge/42-Common%20Core-black)

---

## Project Overview

The **Minishell** project from 42 School introduces:
- The fundamentals of **shell programming**  
- **Process creation**, **pipes**, and **redirections**  
- Handling **signals**, **environment variables**, and **built-in commands**

This project is all about recreating a simplified version of the Bash shell —  
learning how a real shell parses commands, executes processes, and manages the environment 🐚

---

## 📚 Learning Goals

- Master **process control** with `fork`, `execve`, and `wait`  
- Implement **I/O redirections** and **pipes**  
- Manage **signals** like `ctrl+C`, `ctrl+D`, and `ctrl+\`  
- Handle **environment variables** dynamically  
- Build a modular, robust, and readable C program

---

🧠 Design Choices

The project is structured with a clear separation between parsing and execution.

- Parsing is responsible for:
  - Tokenizing user input
  - Handling quotes
  - Expanding environment variables
  - Building clean command structures

- Execution only works with already parsed and expanded commands.

This design avoids mixing string manipulation logic with process control,
making the code easier to debug, extend, and reason about.

---

## ⚙️ Status

🧠 Currently working on command parsing and process management.  
🤝 **Project developed in collaboration with [Fabio Vitharana](https://github.com/FabzHub17)**.  
📅 **Repository will be updated as new features are implemented!**

---

## 🧩 Work Distribution

- **Fabio Vitharana** ![Role](https://img.shields.io/badge/role-parsing-green)  
  - Parsing layer implementation
  - Tokenization and syntax analysis
  - Quote handling and variable expansion
  - Built-in command logic

- **Ilaria Nassi** ![Role](https://img.shields.io/badge/role-execution-blue)  
  - Execution layer implementation
  - Process creation and management
  - Pipes and redirections handling
  - Signal handling and exit status management

---
## 🏗️ Architecture Overview

Below is a simplified view of how **Minishell** processes a command:

```text
┌──────────────────────────────────────────┐
│               User Input                 │
└──────────────────────────────────────────┘
                     │
                     ▼
┌──────────────────────────────────────────┐
│               Parsing Layer              │
│  • Tokenization                          │
│  • Syntax checking                       │
│  • Environment variable expansion         │
└──────────────────────────────────────────┘
                     │
                     ▼
┌──────────────────────────────────────────┐
│              Execution Layer             │
│  • Handle built-ins                      │
│  • Set up pipes and redirections         │
│  • Create processes with fork/execve     │
│  • Wait for children and manage status   │
└──────────────────────────────────────────┘
                     │
                     ▼
┌──────────────────────────────────────────┐
│              Shell Loop                  │
│  • Display prompt                        │
│  • Handle signals                        │
│  • Repeat until exit                     │
└──────────────────────────────────────────┘
```
---
🔍 Expansion Behavior

Environment variable expansion is handled during the parsing phase,
before command execution.

- Variables are expanded using `$VAR`
- `$?` expands to the last command exit status
- No expansion occurs inside single quotes
- Expansion is allowed inside double quotes
- Heredoc delimiters are not expanded

By resolving expansions early, the execution layer only deals with final strings.
---

## 👩‍💻 Authors

- **Ilaria Nassi [@19Jillian89](https://github.com/19Jillian89)** ![Role](https://img.shields.io/badge/role-execution-blue)  
- **Fabio Vitharana [@FabzHub17](https://github.com/FabzHub17)** ![Role](https://img.shields.io/badge/role-parsing-green)

---

## 📄 License

This project is for educational purposes only and is part of the 42 Common Core curriculum. [42 Common Core curriculum](https://www.42network.org/),

---

