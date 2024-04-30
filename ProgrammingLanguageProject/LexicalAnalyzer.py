import re
import colorama
from colorama import Fore

colorama.init(autoreset=True)

keywords = {"int", "text", "is", "loop", "times", "read", "write", "newLine"}
operators = {"+", "-", "*", "/"}
brackets = {"{", "}"}
left_bracket = "{"
right_bracket = "}"
delimiter = ","
end_of_line = "."


def classify_token(token):
    if token.lower() in keywords:
        return "Keyword(" + token.lower() + ")"
    elif token.isdigit():
        if len(token) > 8:
            return Fore.RED + "Error: Integer constant too long"
        else:
            return "IntConst(" + token + ")"
    elif re.match(r"^[a-zA-Z][a-zA-Z0-9_]*$", token):
        if len(token) > 10:
            return Fore.RED + "Error: Identifier too long"
        else:
            return "Identifier(" + token + ")"
    elif token == ",":
        return "Delimiter(,)"
    elif token == ".":
        return "EndOfLine"
    elif token in operators:
        return "Operator(" + token + ")"
    elif token == "{":
        return "LeftCurlyBracket"
    elif token == "}":
        return "RightCurlyBracket"
    else:
        return Fore.RED + "InvalidToken(" + token + ")"


def tokenize_code(code):
    tokens = []
    current_token = ''
    in_string = False
    in_comment = False
    has_left_bracket = False

    i = 0
    while i < len(code):
        char = code[i]
        if in_comment:
            if char == '/' and i > 0 and code[i - 1] == '*':
                in_comment = False
                i += 1
            else:
                i += 1
        elif in_string:
            if char == '"' and (i == 0 or code[i - 1] != '\\'):
                tokens.append('String(' + current_token + char + ')')
                current_token = ''
                in_string = False
                i += 1
            else:
                current_token += char
                i += 1
        elif char.isspace():
            if current_token:
                tokens.append(classify_token(current_token))
                current_token = ''
            i += 1
        elif char == '/' and i < len(code) - 1 and code[i + 1] == '*':
            in_comment = True
            i += 2
        elif char == '"':
            if current_token:
                tokens.append(classify_token(current_token))
                current_token = ''
            in_string = True
            current_token += char
            i += 1
        elif char == '.':
            if i == len(code) - 2 or in_string or in_comment:
                if current_token:
                    tokens.append(classify_token(current_token))
                    current_token = ''
                tokens.append("EndOfLine")
                i += 1
            else:
                tokens.append(Fore.RED + "Error: '.' should be at the end of line or inside a string/comment")
                i += 1
        elif char == '{':
            if not has_left_bracket:
                has_left_bracket = True
                if current_token:
                    tokens.append(classify_token(current_token))
                    current_token = ''
                tokens.append("LeftCurlyBracket")
                i += 1
            else:
                tokens.append(Fore.RED + "Error: Left bracket already exists")
                i += 1
        elif char == '}':
            if has_left_bracket:
                has_left_bracket = False
                if current_token:
                    tokens.append(classify_token(current_token))
                    current_token = ''
                tokens.append("RightCurlyBracket")
                i += 1
            else:
                tokens.append(Fore.RED + "Error: Right bracket without left bracket")
                i += 1
        elif char == delimiter:
            if current_token:
                tokens.append(classify_token(current_token))
                current_token = ''
            else:
                tokens.append(Fore.RED + "Error: Delimiter should be preceded by identifier")
                i += 1
                continue
            tokens.append("Delimiter(,)")
            i += 1
            if i < len(code) and not code[i].isspace() and not (in_string or in_comment):
                tokens.append(Fore.RED + "Error: Delimiter should be followed by identifier")
        elif char in operators:
            if current_token:
                tokens.append(classify_token(current_token))
                current_token = ''
            tokens.append("Operator(" + char + ")")
            i += 1
        else:
            current_token += char
            i += 1

    if current_token:
        tokens.append(classify_token(current_token))

    if has_left_bracket:
        tokens.append(Fore.RED + "Error: Left bracket without right bracket")

    return tokens


def main():
    with open("code.sta", "r") as file:
        lines = file.readlines()

    with open("code.lex", "w") as output_file:
        for line in lines:
            tokens = tokenize_code(line)
            for token in tokens:
                print(token)
                output_file.write(token + "\n")


main()
