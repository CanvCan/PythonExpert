<h1> Lexical Analyzer </h1>


**Openning source and output files.**

The `openFiles` method is responsible for opening two files:

1. **Source File:** This file is specified by the `sourceFileName` parameter and is opened in read mode ("r"). This means the program can only access the contents of the source file.
2. **Output File:** This file is specified by the `outputFileName` parameter and is opened in write mode ("w"). This means the program can write data to the output file. It will overwrite any existing content in the output file.

**Error Handling**

The `openFiles` method checks for errors during file opening. If either the source file or the output file cannot be opened, an error message is printed using `perror` and the program exits with an error code (`EXIT_FAILURE`). 

**Code Breakdown:**

```c
void openFiles(char *sourceFileName, char *outputFileName) {
  sourceFilePtr = fopen(sourceFileName, "r");
  if (sourceFilePtr == NULL) {
    perror("Error opening source file");
    exit(EXIT_FAILURE);
  }

  outputFilePtr = fopen(outputFileName, "w");
  if (outputFilePtr == NULL) {
    perror("Error: Unable to open output file\n");
    exit(EXIT_FAILURE);
  }
}
```




**Getting Characters From Source File**

The `getNextChar` method is responsible for reading a single character from the source file opened by the `openFiles` method.
It employs a loop to ensure it skips any leading newline characters before returning the next valid character.

**How it Works**

1) Loop: The code uses a `do-while` loop to repeatedly read characters until a character other than a newline is encountered.
2) `fgetc`: Inside the loop, `fgetc(sourceFilePtr)` is used to read a single character from the source file pointed to by `sourceFilePtr`. `fgetc` returns the character as an `int`, or `EOF (end-of-file)` if the end of the file is reached.
3) Skipping Newlines: The loop condition `currentChar == '\n'` checks if the read character (`currentChar`) is a newline. If it is, the loop continues, effectively discarding any leading newlines.
4) Returning the Character: Once a character other than a newline is read, the loop exits, and the `currentChar` variable containing the valid character is not modified. This character is then implicitly returned by the method.


**Code Breakdown:**

```c
void getNextChar() {
  do {
    currentChar = fgetc(sourceFilePtr);
  } while (currentChar == '\n');
}
```




**Skipping Whitespaces and Comments**

The `skipWhiteSpace method` is responsible for skipping over any whitespace characters (`isspace`) encountered in the source file. It accomplishes this using a `while` loop.

**How it Works**

1) Loop: The `while` loop continues to execute as long as the `currentChar` variable contains a whitespace character.
2) `isspace` : Inside the loop, `isspace(currentChar)` is used to check if the current character is a whitespace character (space, tab, newline, etc.).
3) Skipping Whitespace: If `currentChar` is whitespace, the `getNextChar` method is called to read the next character from the source file. This effectively discards the whitespace character.


**Code Breakdown:**

```c
void skipWhiteSpace() {
  while (isspace(currentChar)) {
    getNextChar();
  }
}
```

The `skipComments` method is designed to handle comments within the source file.

**How it Works**

1) The method starts by checking if the current character (`currentChar`) is a forward slash (`/`).
2) If `currentChar` is a forward slash, the `getNextChar` method is called to read the next character.
If the next character is an asterisk (`*`), it's a confirmed block comment.
The method enters a `while` loop that continues until the end of the block comment is found.
3) The code searches for an asterisk (`*`) followed by a forward slash (`/`) to mark the end of the block comment.
If the ending sequence (`* /`) is found, the loop breaks, and `getNextChar` is called to skip past the closing characters (the asterisk and slash). This ensures the next character processed is the one following the comment.
If the ending sequence is not yet encountered, `getNextChar` is called again to read the next character and continue iterating through the comment.
4) If the end of the file (`feof(sourceFilePtr)`) is reached before finding the closing sequence, an error message is printed indicating an unterminated block comment. The program exits with an error code (`EXIT_FAILURE`) to signal the problem.

**Code Breakdown:**

```c
void skipComments() {
  if (currentChar == '/') {
    getNextChar();
    if (currentChar == '*') { // Block comment
      int commentEnd = 0;
      getNextChar();
      while (!feof(sourceFilePtr)) {
        if (currentChar == '*') {
          getNextChar();
          if (currentChar == '/') {
            getNextChar();
            commentEnd = 1;
            break;
          }
        } else {
          getNextChar();
        }
      }

      if (!commentEnd) {
        fprintf(stderr, "Error: Block comment does not terminate before file end\n");
        exit(EXIT_FAILURE);
      }
    } else {
      Token token;
      printf("Current char: /");
      token.type = OPERATOR;
      token.value[0] = (char)'/';
      token.value[1] = '\0';
      printTokenToSourceFile(token);
      getNextChar();
    }
  }
}
```



**Determining the Token**

The `getNextToken` function is the core of the lexical analyzer. It reads the source code character by character and identifies the next meaningful element (token) in the code. This token could be a keyword, identifier, integer constant, string constant, operator, punctuation mark, or an indication of the end of the line.

**How it Works**

1) A `Token` struct is declared to hold the information about the identified token.
2) An integer variable is used as an index to keep track of the position within the `token.value` array where characters of the token are stored.
3) The code checks the value of `currentChar` to determine the type of token.
4) After identifying the token type and populating the `token.value` array appropriately, the `getNextToken` function returns the `Token` struct containing the identified token information.

**Code Breakdown:**

```c
Token getNextToken() {
    Token token;
    int i = 0;

    skipWhiteSpace();
    skipComments();

    // ... (rest of the code as explained above)

    return token;
}
```


**Printing Token to Source File**

The `printTokenToSourceFile` function is responsible for writing the details of a given `Token` struct to the output file opened by the `openFiles` method. This function helps create a human-readable representation of the identified tokens in the source code.

**How it Works**

1) The function takes a single argument token of type Token. This token struct contains the information about the token that needs to be printed to the output file.
2) The switch statement examines the token.type field of the provided token struct. Depending on the type:


**Code Breakdown:**

```c
void printTokenToSourceFile(Token token)
{
    switch (token.type)
    {
    case IDENTIFIER:
        fprintf(outputFilePtr, "Identifier: %s\n", token.value);
        break;
    case INT_CONST:
        fprintf(outputFilePtr, "Integer constant: %s\n", token.value);
        break;
    case OPERATOR:
        fprintf(outputFilePtr, "Operator: %s\n", token.value);
        break;
    case LEFT_CURLY_BRACKET:
        fprintf(outputFilePtr, "Left curly bracket: %s\n", token.value);
        break;
    case RIGHT_CURLY_BRACKET:
        fprintf(outputFilePtr, "Right curly bracket: %s\n", token.value);
        break;
    case STRING_CONST:
        fprintf(outputFilePtr, "String constant: %s\n", token.value);
        break;
    case KEYWORD:
        fprintf(outputFilePtr, "Keyword: %s\n", token.value);
        break;
    case END_OF_LINE:
        fprintf(outputFilePtr, "End of line: %s\n", token.value);
        break;
    case COMMA:
        fprintf(outputFilePtr, "Comma: %s\n", token.value);
        break;
    default:
        break;
    }
}
```

**Closing source and output files**

Finally `closeFiles` method closes `sourceFilePtr` and `outputFilePtr` with `fclose` method.

**Code Breakdown:**

```c
void closeFiles()
{
    fclose(sourceFilePtr);
    fclose(outputFilePtr);
}
```
