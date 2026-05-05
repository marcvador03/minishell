# minishell : A Bash-like Shell from Scratch

A **custom shell implementation** developed as a milestone project at [School 42](https://www.42.fr/). This project mimics several features of **Bash**, including command execution, pipes, redirections, environment variables, and built-in commands. It is designed to be **robust, leak-free (memory and file descriptors)**, and compliant with the strict **42 Norminette** coding standards.

---

## **Purpose**

This project is a deep dive into **process management, file descriptors, pipes, and forks** in Unix-like systems. It was developed in a team of two and required rigorous testing (including ~1000 lines of test cases in Excel) to ensure correctness and reliability.

---

## **Features**

- **Command Execution**:
  - Absolute and relative paths.
  - Command history (powered by `readline`).
- **Pipes (`|`)**: Chain commands like in Bash.
- **Redirections**:
  - Input/Output: `<`, `>`, `>>`, `<<` (heredoc).
- **Quotes Handling**: Support for `""` and `''`.
- **Logical Operators**: `&&`, `||`, and parenthesis for grouping.
- **Environment Variables**: Expand variables and `$?` (exit status).
- **Signal Handling**: Custom behavior for `Ctrl-C`, `Ctrl-D`, and `Ctrl-\`.
- **Built-in Commands**:
  - `echo` (with `-n` support)
  - `export`, `env`, `exit` (supports `long long` arguments)
  - `pwd`, `unset`

---

## **Technologies & Requirements**

- **Language**: C
- **Compiler**: `gcc` or `clang` (tested with `cc` alias).
- **Dependencies**:
  - `[libreadline-dev](https://tiswww.case.edu/php/chet/readline/rltop.html)`: For interactive input and command history.
  - **Libft**: Custom library (included in the repo) mimicking standard C functions, developed as part of the 42 curriculum.
- **OS**: Tested on **Linux Debian Trixie** and **Ubuntu**.

---

## **Installation & Usage**

### 1. **Clone the Repository**

```bash
git clone https://github.com/marcvador03/minishell.git
cd minishell
```

### 2. **Install Dependencies**

```bash
# Debian/Ubuntu
sudo apt-get install libreadline-dev
```

### 3. **Compile the Project**

```bash
make          # Compiles the program (output: ./minishell)
make clean    # Removes object files
make fclean   # Removes object files + executable
make re       # Full clean + recompile
```

### 4. **Run minishell**

```bash
./minishell
```

- The prompt will resemble Bash (`user@hostname:current_path$`), but you’re using **minishell**!

---

## **Project Structure**

```
minishell/
├── include/          # Header files
├── libft/            # Custom library (42's Libft)
├── src/              # Source files
│   ├── main/         # Initialization, prompt, environment, errors
│   │   └── minishell.c  # Entry point (main())
│   ├── execute/      # Command execution logic
│   ├── parsing/      # Command parsing (lexing, tokenizing, etc.)
│   ├── built-ins/    # Built-in commands (echo, export, etc.)
│   └── utils/        # Utility functions (memory, strings, etc.)
├── Makefile          # Build configuration
└── README.md         # Project documentation
```

---

## **42 Norminette Compliance**

- **25 lines max per function**.
- **5 functions max per file**.
- **No external libraries** (except `readline` and standard C libraries).
- **Strict error handling** (memory leaks, file descriptor leaks).

> *Note*: Some minor `readline` leaks may appear due to library limitations.

---

## **Testing**

- **Robustness**: Passed all evaluations at 42, including stress tests.
- **Edge Cases**: Handles complex command chains, nested quotes, and signal interruptions.
- **Leak Checks**: Verified with `valgrind` (memory and file descriptors).

---

## **Resources**

- [Bash Manual](https://www.gnu.org/software/bash/manual/)
- [Readline Library Documentation](https://tiswww.case.edu/php/chet/readline/rltop.html)
- [42 Norminette](https://github.com/42School/norminette)

---

## **License**

This project is open-source and available under the [MIT License](LICENSE).

---

## **Acknowledgments**

- Developed as part of the **42 School** curriculum.
- Special thanks to evaluators for their rigorous testing and feedback.
- Inspired by **Bash** and other Unix shells.
- AI was NOT used in any part of the development of this project, just to generate this README :)

