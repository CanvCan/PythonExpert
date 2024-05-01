#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_IDENTIFIER_LENGTH 10
#define MAX_INTEGER_LENGTH 8
#define MAX_STRING_LENGTH 256

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
            getNextChar();
            while (1)
            {
                if (currentChar == '*')
                {
                    getNextChar();
                    if (currentChar == '/')
                    {
                        getNextChar();
                        break;
                    }
                }
                else
                {
                    getNextChar();
                }
            }
        }
    }
}

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
            if (i >= MAX_IDENTIFIER_LENGTH)
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
        getNextChar();
    }
    else if (currentChar == '}')
    {
        token.type = RIGHT_CURLY_BRACKET;
        token.value[i++] = (char)currentChar;
        token.value[i] = '\0';
        getNextChar();
    }
    else if (currentChar == '+' || currentChar == '-' || currentChar == '*' || currentChar == '/')
    {
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

void printToken(Token token)
{
    // fprintf(outputFilePtr, "Token: %u, Value: %s\n", token.type, token.value);

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

int main(int argc, char *argv[])
{
    char *sourceFile;
    char *outputFile;

    if (argc != 3)
    {
        sourceFile = "C:\\star files\\code.sta";
        outputFile = "C:\\star files\\code.lex";
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
        currentToken = getNextToken();
        // printf("Token: %u, Value: %s\n", currentToken.type, currentToken.value);
        printToken(currentToken);
    }

    closeFiles();
    return 0;
}