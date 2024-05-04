/*
This program is a simple lexical analyzer (also known as a lexer or tokenizer) implemented in C.
It reads a source code file character by character, tokenizes it into a series of tokens, and writes these tokens to an output file.

Each token represents a meaningful component of the source code. These components can be an identifier, keyword, operator, or constant.

The lexer uses the getNextChar function to read the source code one character at a time.
It then uses this character to determine the type of the next token.

For instance, if the character is a letter, the lexer treats it as the start of an identifier.
It then reads subsequent characters until it finds a character that is not a letter or digit, marking the end of the identifier.

Similarly, if the character is a digit, the lexer treats it as the start of an integer constant.
It then reads subsequent digits until it finds a non-digit character, marking the end of the integer constant.

The lexer also handles whitespace and comments, skipping over them to find the next token.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_IDENTIFIER_LENGTH 10
#define MAX_INTEGER_LENGTH 8
#define MAX_STRING_LENGTH 256
#define BUFFER_SIZE 256

char buffer[BUFFER_SIZE];
int bufferIndex = 0;

typedef enum
{
    IDENTIFIER,
    INT_CONST,
    OPERATOR,
    LEFT_CURLY_BRACKET,
    RIGHT_CURLY_BRACKET,
    STRING_CONST,
    KEYWORD,
    END_OF_LINE,
    COMMA
} TokenType;

typedef struct
{
    TokenType type;
    char value[MAX_STRING_LENGTH];
} Token;

FILE *sourceFilePtr;
FILE *outputFilePtr;
int currentChar;
Token currentToken;

void openFiles(char *sourceFileName, char *outputFileName)
{
    sourceFilePtr = fopen(sourceFileName, "r");
    if (sourceFilePtr == NULL)
    {
        perror("Error opening source file");
        exit(EXIT_FAILURE);
    }

    outputFilePtr = fopen(outputFileName, "w");
    if (outputFilePtr == NULL)
    {
        perror("Error: Unable to open output file\n");
        exit(EXIT_FAILURE);
    }
}

void closeFiles()
{
    fclose(sourceFilePtr);
    fclose(outputFilePtr);
}

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

void getNextChar()
{
    do
    {
        currentChar = fgetc(sourceFilePtr);
    } while (currentChar == '\n');
}

void skipWhiteSpace()
{
    while (isspace(currentChar))
    {
        getNextChar();
    }
}

void skipComments()
{
    if (currentChar == '/')
    {
        getNextChar();
        if (currentChar == '*')
        {
            int commentEnd = 0;
            getNextChar();
            while (!feof(sourceFilePtr))
            {
                if (currentChar == '*')
                {
                    getNextChar();
                    if (currentChar == '/')
                    {
                        getNextChar();
                        commentEnd = 1;
                        break;
                    }
                }
                else
                {
                    getNextChar();
                }
            }

            if (!commentEnd)
            {
                fprintf(stderr, "Error: Block comment does not terminate before file end\n");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
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

void controlBracketNumber(int bracketCount)
{
    if (bracketCount > 0)
    {
        fprintf(stderr, "Error: Closing bracket expected.\n");
        exit(EXIT_FAILURE);
    }
    else if (bracketCount < 0)
    {
        fprintf(stderr, "Error: Opening bracket expected.\n");
        exit(EXIT_FAILURE);
    }
}

int bracketCount = 0;

Token getNextToken()
{
    Token token;
    int i = 0;

    skipWhiteSpace();
    skipComments();

    if (currentChar == EOF)
    {
        token.type = END_OF_LINE;
        strcpy(token.value, "EndOfLine");
        return token;
    }

    if (isalpha(currentChar))
    {
        while (isalnum(currentChar) || currentChar == '_')
        {
            token.value[i++] = currentChar;
            if (i > MAX_IDENTIFIER_LENGTH)
            {
                fprintf(stderr, "Error: Identifier exceeds maximum length\n");
                exit(EXIT_FAILURE);
            }
            getNextChar();
        }
        token.value[i] = '\0';
        if (strcmp(token.value, "int") == 0 || strcmp(token.value, "text") == 0 ||
            strcmp(token.value, "is") == 0 || strcmp(token.value, "loop") == 0 ||
            strcmp(token.value, "times") == 0 || strcmp(token.value, "read") == 0 ||
            strcmp(token.value, "write") == 0 || strcmp(token.value, "newLine") == 0)
        {
            token.type = KEYWORD;
        }
        else
        {
            token.type = IDENTIFIER;
        }
    }
    else if (isdigit(currentChar))
    {
        while (isdigit(currentChar))
        {
            token.value[i++] = currentChar;
            if (i >= MAX_INTEGER_LENGTH)
            {
                fprintf(stderr, "Error: Integer constant exceeds maximum length\n");
                exit(EXIT_FAILURE);
            }
            getNextChar();
        }
        token.value[i] = '\0';
        token.type = INT_CONST;
    }
    else if (currentChar == '"')
    {
        token.value[i++] = currentChar;
        getNextChar();
        while (currentChar != '"' && currentChar != EOF && i < MAX_STRING_LENGTH)
        {
            token.value[i++] = currentChar;
            getNextChar();
        }
        if (currentChar == EOF)
        {
            fprintf(stderr, "Error: String constant does not terminate before file end\n");
            exit(EXIT_FAILURE);
        }
        if (i >= MAX_STRING_LENGTH)
        {
            fprintf(stderr, "Error: String constant exceeds maximum length\n");
            exit(EXIT_FAILURE);
        }
        token.value[i++] = (char)currentChar;
        token.value[i] = '\0';
        token.type = STRING_CONST;
        getNextChar();
    }
    else if (currentChar == '{')
    {
        token.type = LEFT_CURLY_BRACKET;
        token.value[i++] = (char)currentChar;
        token.value[i] = '\0';
        bracketCount++;
        getNextChar();
    }
    else if (currentChar == '}')
    {
        token.type = RIGHT_CURLY_BRACKET;
        token.value[i++] = (char)currentChar;
        token.value[i] = '\0';
        bracketCount--;
        getNextChar();
    }
    else if (currentChar == '+' || currentChar == '-' || currentChar == '*' || currentChar == '/')
    {
        printf("Current char: %c\n", currentChar);
        token.type = OPERATOR;
        token.value[i++] = (char)currentChar;
        token.value[i] = '\0';
        getNextChar();
    }
    else if (currentChar == '.')
    {
        token.type = END_OF_LINE;
        token.value[i++] = (char)currentChar;
        token.value[i] = '\0';
        getNextChar();
    }
    else if (currentChar == ',')
    {
        token.type = COMMA;
        token.value[i++] = (char)currentChar;
        token.value[i] = '\0';
        getNextChar();
    }
    else
    {
        fprintf(stderr, "Error: Invalid character '%c'\n", currentChar);
        exit(EXIT_FAILURE);
    }

    return token;
}

int main(int argc, char *argv[])
{
    char *sourceFile;
    char *outputFile;

    int isFirstTokenMinus = 0;
    int elementCount = 0;

    if (argc != 3)
    {
        sourceFile = "../src/code.sta";
        outputFile = "../src/code.lex";
    }
    else
    {
        sourceFile = argv[1];
        outputFile = argv[2];
    }

    printf("Source file: %s\n", sourceFile);
    printf("Output file: %s\n", outputFile);
    openFiles(sourceFile, outputFile);

    getNextChar();

    while (!feof(sourceFilePtr))
    {
        if (elementCount == 0)
        {
            currentToken = getNextToken();

            if (currentToken.type == OPERATOR && currentToken.value[0] == '-')
            {
                isFirstTokenMinus = 1;
                elementCount = 1;
            }
            else
            {
                printTokenToSourceFile(currentToken);
                elementCount = 2;
            }
        }
        else if (elementCount == 1)
        {
            Token previousToken = currentToken;
            currentToken = getNextToken();
            if (currentToken.type == INT_CONST)
            {
                Token token;
                token.type = INT_CONST;
                token.value[0] = '0';
                token.value[1] = '\0';
                printTokenToSourceFile(token);
                elementCount = 2;
            }
            else
            {
                printTokenToSourceFile(previousToken);
                printTokenToSourceFile(currentToken);
                elementCount = 2;
            }
        }
        else
        {
            currentToken = getNextToken();
            printTokenToSourceFile(currentToken);
        }

        controlBracketNumber(bracketCount);
    }

    closeFiles();
    return 0;
}
