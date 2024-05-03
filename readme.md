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

1) Check for Forward Slash: The method first verifies if currentChar is a forward slash (/).
2) The method calls getNextChar again to check for the asterisk (*).
3)  error handling asdfaf

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
