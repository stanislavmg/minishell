# Minishell

Simple implementation of a bash-like command interpreter 

---

## Features

- **Command History**
- **Support for Quotes and Special Characters**
- **I/O Redirections**: Supports I/O redirection and here documents `<`, `>`, `>>`, `<<`
- **Grouped Redirection Support**: Allows grouped redirections, enabling the execution of commands like `(ls ; who) > out`
- **Environment Variables**
- **Signal Management**: `ctrl-C`, `ctrl-D`, and `ctrl-\`
- **Special Variables**: Supports special variables, like `$?`
- **Compound Commands**: Allows subshell execution with parentheses `()` for compound commands
- **Command Lists**: Supports logical operators such as `&&`,  `||` and `;`  for chaining commands
- **Wildcard**: Supports wildcard in the current working directory
- **Built-in commands**:  `echo`, `cd`, `pwd`, `export`, `unset`, `env`, and `exit`

---

## Installation and running

Follow these steps to set up and run the minishell:

```bash
git clone https://github.com/stanislavmg/minishell.git
cd minishell
make configure && make all
./minishell
```
