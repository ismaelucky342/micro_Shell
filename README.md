# Microshell

This project is a microshell implementation inspired by the **Minishell** project from **42 Madrid** and developed as part of the **Systems** course at the **University of Technology and Digital Art (U-Tad)**. The goal is to deepen the understanding of key concepts in command interpreters and their efficient execution in Unix-based systems.

## Project Overview

The microshell emulates a minimal shell environment capable of interpreting and executing commands. It incorporates modern design patterns, such as an **Abstract Syntax Tree (AST)**, to parse and execute commands effectively.

### Key Features:

- **Command Tokenization**: Breaks down input into manageable tokens for processing.
- **Abstract Syntax Tree (AST)**: Provides an organized structure to handle complex command parsing and execution.
- **Support for Pipes (`|`) and Semicolons (`;`)**: Allows sequential and pipelined command execution.
- **Error Handling**: Outputs meaningful error messages for invalid commands or execution failures.
- **Custom Commands**: Implements basic shell commands, such as `cd`.

## Why Use an AST?

The AST is a critical component in shell implementations, offering several advantages:

1. **Structured Representation**: Organizes commands hierarchically for better readability and processing.
2. **Efficient Parsing**: Simplifies complex inputs, such as nested commands or pipelines, into manageable subtrees.
3. **Flexibility**: Facilitates future expansions, such as handling redirections or custom commands.
4. **Error Detection**: Enables syntactic validation during the parsing phase.

### Example:

For an input like:

```bash
ls -l | grep txt ; echo "Done
```

The AST would look like:

```css

                [;]
               /   \
          [|]         [echo]
         /   \        /    \
     [ls]  [grep]   "Done"
      -l     txt

```

This structure makes it easy to execute commands in the correct sequence and handle errors at the right level.

## Code Structure

The project is organized into:

- **`src/`**: Contains the main implementation code (`microshell.c`).
- **`includes/`**: Holds header files defining the project’s core functions and data structures (`microshell.h`).

### Components:

1. **Initialization**:
    - Prepares the shell environment and handles input/output configurations.
2. **Input and Tokenization**:
    - Splits user input into commands and arguments.
3. **Syntax Verification**:
    - Ensures that input follows valid shell syntax.
4. **AST Construction**:
    - Builds the AST based on the parsed input.
5. **Execution**:
    - Processes the AST to execute commands using system calls like `execve`.
6. **Error Handling**:
    - Detects and reports errors, such as invalid commands or system failures.
7. **Cleanup**:
    - Frees allocated memory and resets the environment to a stable state.

## Compilation

To compile the project, use the provided `Makefile`. Run the following commands:

```bash
make        # Compiles the project
make clean  # Removes object files
make fclean # Removes the executable and object files
make re     # Cleans and recompiles the project
```

## Usage

After compilation, run the microshell:

```bash
./microshell
```

Example commands:

```bash
cd ..
ls -l | grep .c ; echo "Finished"
```

## Future Enhancements

- **Redirection Support**: Implement input (`<`) and output (`>`).
- **Environment Variables**: Expand support for variable expansion (`$VAR`).
- **Heredoc (`<<`)**: Handle multi-line input.

## Acknowledgments

This project was inspired by:

- The **Minishell** project from **42 Madrid**.
- The **Systems** course at **U-Tad**, which emphasizes the importance of understanding low-level programming and Unix systems.

# Extras

This microshell not only demonstrates the foundational knowledge required to build a complete shell interpreter but also showcases the practical benefits of employing structured design with **Abstract Syntax Trees (ASTs)**. By combining low-level system programming skills with modern software design principles, this project illustrates how complex command sequences can be parsed, validated, and executed in an efficient and maintainable way.

Special thanks to U-Tad and 42 Madrid for making this project and many others like it possible, and for cultivating an environment where students can grow, learn, and create meaningful work.

-Enjoy the code

