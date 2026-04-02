*This project has been created as part of the 42 curriculum by zchoo and ka-tan.*

# 🐚 Minishell

> _As beautiful as a shell_ ✨

Minishell is a small Bash-inspired shell written in C as part of the 42 curriculum.  
The goal of the project is to understand **how a shell really works under the hood**: reading user input, tokenizing and parsing commands, handling environment variables, managing processes, wiring file descriptors, executing programs, and reproducing essential shell behavior.

This project focuses on the mandatory part of the subject:
- interactive prompt with command history
- execution of external commands
- built-ins
- pipes and redirections
- environment variable expansion
- signal handling

It is basically a deep dive into **processes, file descriptors, signals, parsing, and execution flow** — all the things a shell hides from us every day 🤝

---

## 📖 Description

A shell is an interactive program that repeatedly:
1. displays a prompt
2. reads a command line
3. interprets it
4. executes it
5. returns to the prompt

This Minishell project recreates that cycle in C.

The shell is designed to:
- display a prompt while waiting for input
- keep a working history using `readline`
- search executables using `PATH`, relative paths, or absolute paths
- handle quotes correctly
- expand environment variables like `$USER` and `$?`
- support redirections (`<`, `>`, `<<`, `>>`)
- support pipelines (`|`)
- implement the required built-ins
- react properly to `ctrl-C`, `ctrl-D`, and `ctrl-\`

Rather than aiming to reproduce every Bash feature, the project focuses on the subset explicitly required by the subject.

---

## ✨ Features

### 🧩 Parsing & interpretation
- tokenization of user input
- handling of single quotes `'` and double quotes `"`
- environment variable expansion
- special expansion of `$?`
- command grouping into executable structures

### ⚙️ Execution
- execution of external commands with `fork()` + `execve()`
- command lookup through `PATH`
- pipeline handling with `pipe()`
- input/output redirections with `dup2()`
- heredoc support with `<<`

### 🧠 Built-ins
- `echo` with `-n`
- `cd`
- `pwd`
- `export`
- `unset`
- `env`
- `exit`

### 📡 Signals
Interactive behavior is implemented to match the subject requirements:
- `ctrl-C` shows a fresh prompt on a new line
- `ctrl-D` exits the shell
- `ctrl-\` does nothing in interactive mode

---

## 🏗️ Project overview

A simplified flow of the shell looks like this:

```text
readline() -> lexer/tokenizer -> expansion -> parser -> executor
                                              |
                                              -> built-ins / execve / pipes / redirections
```

### Main ideas behind the project
- **Lexer**: breaks raw input into meaningful tokens
- **Expander**: replaces variables such as `$HOME` and `$?`
- **Parser**: organizes tokens into command structures
- **Executor**: launches built-ins or external commands, manages pipes/redirections, and updates exit status

Some built-ins must run in the **parent process**, especially those that modify shell state:
- `cd`
- `export`
- `unset`
- `exit`

That is because changing directory or environment only inside a child process would disappear as soon as the child exits.

---

## 🛠️ Instructions

### 1. Clone the repository
```bash
git clone <your-repo-url>
cd minishell
```

### 2. Compile
```bash
make
```

### 3. Run
```bash
./minishell
```

### 4. Clean object files
```bash
make clean
```

### 5. Remove all generated files
```bash
make fclean
```

### 6. Rebuild from scratch
```bash
make re
```

---

## ▶️ Usage examples

### Run a simple command
```bash
minishell$ ls -la
```

### Use a pipe
```bash
minishell$ cat file.txt | grep hello
```

### Redirect output
```bash
minishell$ echo hello > out.txt
```

### Append output
```bash
minishell$ echo hello >> out.txt
```

### Redirect input
```bash
minishell$ wc -l < file.txt
```

### Use heredoc
```bash
minishell$ cat << EOF
hello
world
EOF
```

### Expand variables
```bash
minishell$ echo $HOME
minishell$ echo $?
```

### Use built-ins
```bash
minishell$ echo -n Hello 
minishell$ pwd
minishell$ cd <path..>
minishell$ export TEST=hello
minishell$ unset TEST
minishell$ env
minishell$ exit
```

---

## 🧪 Testing notes

Some useful comparisons while testing:

### Compare `env` output with Bash
Start your program:
```bash
./minishell
```
Inside minishell:
```bash
env | sort > my_env.txt
```
In Bash:
```bash
env | sort > bash_env.txt
diff -u bash_env.txt my_env.txt
```


### Useful things to test
- quoted vs unquoted variables
- invalid commands
- missing files in redirections
- multiple pipes
- built-ins inside and outside pipelines
- `ctrl-C`, `ctrl-D`, `ctrl-\`
- `$?` after success, failure, and signal interruption

---

## 📚 Resources

References used to understand the project topic:
- **Bash Reference Manual**
- **CS50x 2026 - Lecture 5 - Data Structures**
https://www.youtube.com/watch?v=PmAI76OGE_E

AI was used for:
- clarifying concepts such as `fork`, `execve`, `dup2`, pipes, signals, and exit status
- helping break big topics into smaller understandable steps
- reviewing edge cases for built-ins like `export`, `unset`, `cd`, and `exit`

---
## 🌱 Final thought

As we go deeper into minishell, the magic behind Bash feels even more real. Something as familiar as daily terminal use turns out to be built on careful decisions about processes, file descriptors, signals, expansion, and parsing. With lots of sweat, frustration, and probably tidbits-stained fingers, we came away with a deeper appreciation for the engineers and computer scientists who built these systems before us.✨
