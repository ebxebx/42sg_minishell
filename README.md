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
- parsing operators such as pipes and redirections

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

---
## 🧠 Concepts we worked through

### 👶 Why `fork()` creates a child process

A shell has to stay alive even while it runs other commands. If the shell ran `ls` or `cat` directly inside itself, it would stop being a shell. So instead, it creates a **child process** with `fork()`. The child is used to run the command, while the parent shell stays alive, waits for the child to finish, stores the exit status, and shows the next prompt.

### 🔄 Why `execve()` replaces the child program

After `fork()`, the child is still just a copy of the shell. It has not become `ls`, `grep`, or `cat` yet. `execve()` is what replaces that copied shell program with the real executable the user asked for. If `execve()` succeeds, the old child code is gone and the process is now running the new program.

### 🔌 Why `dup2()` is the core of redirections and pipes

Redirections and pipes are really about **rewiring file descriptors**. `stdin`, `stdout`, and `stderr` are just file descriptors `0`, `1`, and `2`. `dup2()` lets us replace one of those descriptors so that input comes from a file instead of the keyboard, output goes to a file instead of the terminal, or one command's output becomes another command's input through a pipe.

### 🏠 Why some built-ins must run in the parent

Built-ins like `cd`, `export`, `unset`, and `exit` change the shell's own state. If they run in a child process, that change only affects the child, and disappears when the child exits. That is why these built-ins must run in the **parent shell process** instead of being handled like normal external commands.

### 🚨 Why signal handling must stay simple and safe

Signals are asynchronous, which means they can interrupt the program at almost any moment. Because of that, signal handlers should not try to manipulate complicated data structures. The safe pattern is to keep one simple global variable that stores the received signal number, and let the main shell logic respond to it later.

### 😵‍💫 Why shell parsing is harder than it first looks

At first glance, parsing a shell seems like splitting the input by spaces. But very quickly it becomes more complicated: quotes change meaning, `$VAR` expands in some contexts but not others, operators like `|`, `<`, `>`, `>>`, and `<<` affect structure, and tokens do not all behave the same way. Even a short command line can already contain hierarchy, context, and special rules.

---

## 🌳 About the AST

An **Abstract Syntax Tree (AST)** is a tree-shaped structure that represents the meaning of a command line, not just the order of its tokens.

A flat token list is useful at the start:

```text
echo "hello" | grep h > out.txt
```

might first become something like:

```text
[WORD echo] [WORD "hello"] [PIPE] [WORD grep] [WORD h] [REDIR_OUT] [WORD out.txt]
```

But the shell eventually needs more than a flat list. It needs to know:
- which command is on the left of a pipe
- which command is on the right of a pipe
- which redirection belongs to which command
- what should be executed first

That is where an AST becomes useful.

### A simple mental picture

For a pipeline like:

```bash
ls -l | grep .c
```

an AST might look like:

```text
        [PIPE]
       /      \
   [CMD ls] [CMD grep]
```

For a command with redirection like:

```bash
grep hello < input.txt > out.txt
```

you may represent the command and its attached redirections in a structured way so the executor knows exactly what belongs together.

### Why ASTs are useful in minishell

An AST is stronger than a plain linked list when syntax gets more structured, because it stores **relationships**, not just sequence.

That makes it easier for the executor to reason like this:
- if this node is a command, execute it
- if this node is a pipe, connect left and right children with a pipe
- if this node is a redirection, open the file and redirect the correct file descriptor before running the related command

In other words:
- a **linked list** is good for storing what comes next
- an **AST** is good for storing what belongs under what

That is why shell parsing starts to feel like real syntax analysis, not just string splitting.

---

## ⚙️ Execution flow notes

### Lexer / tokenizer
The lexer is the first stage that touches the raw input string. Its job is to split input like:

```bash
echo "hello $USER" | cat > out.txt
```

into meaningful tokens such as words, pipes, and redirections.

Quote handling is one of the trickiest parts:
- single quotes keep everything literal
- double quotes allow `$` expansion but still protect most special interpretation
- unquoted text follows normal shell splitting rules

### Expansion
Variable expansion turns things like `$USER` into their corresponding values from the environment. `$?` is a special case that expands to the exit status of the most recently executed foreground command or pipeline.

Expansion usually happens either during tokenization or in a dedicated pass between the lexer and parser.

### Pipes and redirections
Redirections are done by opening the target file and then using `dup2()` to attach it to the correct standard file descriptor.

Pipes extend the same idea between processes:
- `pipe(fd)` creates a read end and a write end
- the left command writes into the pipe
- the right command reads from the pipe

A very important practical detail is closing unused pipe ends. If a write end stays open somewhere by mistake, the reader may never receive EOF and the command can hang forever.

### Heredoc
Heredoc (`<<`) is its own special case. The shell must read lines until the delimiter is reached, then feed those collected lines as standard input to the command. It behaves differently from normal file redirection because it needs an extra read loop at execution time.

### Readline and the main loop
A shell is essentially a loop:

```c
while (1)
{
    input = readline("minishell$ ");
    parse(input);
    execute(...);
}
```

`readline()` displays the prompt, waits for input, and returns a heap-allocated string. That string must be freed after use. If the user presses `ctrl-D` on an empty line, `readline()` returns `NULL`, which usually means it is time to exit the shell.

---

## 🛠️ Instructions

1. To Compile
```bash
make
```
2. Run
```bash
./minishell
```

3. Clean object files
```bash
make clean
```

4. Remove all generated files
```bash
make fclean
```

5. Rebuild from scratch
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
- `cd` with valid and invalid paths
- `export` / `unset` edge cases
- hanging cases caused by forgotten pipe closures

---

## 📚 Resources

References used to understand the project topic:
- **Bash Reference Manual** https://www.gnu.org/s/bash/manual/bash.html
- **CS50x 2026 - Lecture 5 - Data Structures** https://www.youtube.com/watch?v=PmAI76OGE_E
- **Minishell Tester** https://github.com/LucasKuhn/minishell_tester
- https://m4nnb3ll.medium.com/minishell-building-a-mini-bash-a-42-project-b55a10598218
- https://www.mintlify.com/ibon-ira/Minishell

AI was used for:
- clarifying concepts such as `fork`, `execve`, `dup2`, pipes, signals, parsing and exit status
- helping break big topics into smaller understandable steps
- reviewing edge cases for built-ins like `export`, `unset`, `cd`, and `exit`

---
## 🌱 Final thought

As we go deeper into minishell, the magic behind Bash feels even more real. Something as familiar as daily terminal use turns out to be built on careful decisions about processes, file descriptors, signals, expansion, and parsing. With lots of sweat, frustration, and probably tidbits-stained fingers, we came away with a deeper appreciation for the engineers and computer scientists who built these systems before us.✨
