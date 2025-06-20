**Purpose**

This is the a big milestone from 42 School! Minishell aims at creating a shell from scratch mimicking several features of Bash. Compared to previous projects, this project is done in a group of 2 persons and requires a steep additional time investment to conclude it. Pure rigor to check all details (had a nearly 1000 lines of test in excel...) and deep diving into pipes, forks, files descriptors to understand their mechanics...

**Installation**

the library compiles with gcc or clang, using "cc" alias command.
In your terminal run "make" in the main directory, which compiles all files and creates a program named minishell
launch in your terminal "./minishell" with no arguments. Depending on your system, minishell prompt might be identical to the one used by Bash (user@hostname:current_path) but you're well using Minishell.

"make clean" removes object files created.
"make fclean" removes object files and the program.
"make re" does a "make fclean" followed by a "make" to regenerate all files.

**Dependencies**

Minishell uses readline to collect user inputs. You might need to install libreadline-dev in your distribution to compile it.

My own library "Libft" which was developed as part of the school to mimic C standard functions is shipped with this project. We are restricted by the school in the external functions we can use for each project, but allowed to use our own functions and library such as Libft.

*Tested on Linux Debian Trixie and Ubuntu*

**Additional information**

Minishell passed all evaluations despite being tortured by some evaluators :) The program is robust, leak free (memory and fds) at the exception of some readline leaks that appear sometimes.

Features developed for that Minishell:
- execute commands with absolute and relative paths
- keep history of commands
- handle pipes '|' like in bash
- handle redirections '<', '>', '>>', '<<' like in Bash
- handle quotes "" and '' like in Bash
- handle "&&" and "||" with parenthesis like in Bash
- handle environment variables and $?
- custom handling of ctrl-C, ctrl-D and ctrl-\ signals
- built-ins commands for echo, export, env, exit, pwd, unset. Apart from echo supporting -n argument, other built-ins do not have options. Exit can take a long long integer as an argument.

The sources files in /src are split into 5 sub-directories:
- **main** contains files related to intialize the minishell, prompt, environment and errors management. The main() is located in *minishell.c*.
- **execute** contains all functions related to execution of the command
- **parsing** will contain files related to the various stage of parsing the command
- **built-ins** the built-ins commands
- **utils** contains various functions used through the program (customed free(), string manipulations etc.)

*For C Projects, 42 School imposes a norm to follow strictly. Amongst many things, we are restricted to write no more than 25 lines per function, and 5 functions per file. I really do love norms, but just mentioning this to explain why sometimes we split functions and multiply files :)*
