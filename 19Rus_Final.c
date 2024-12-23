#include <stdio.h>
#include <string.h>

// Define a structure to hold Roman numeral and operation information
typedef struct {
    char roman1[50];
    char roman2[50];
    char operation;
} RomanOperation;

// Function to convert a single Roman numeral character to its decimal value
int romanToDecimal(char ch) {
    switch (ch) {
        case 'I': return 1;
        case 'V': return 5;
        case 'X': return 10;
        case 'L': return 50;
        case 'C': return 100;
        case 'D': return 500;
        case 'M': return 1000;
        default: return 0;
    }
}

// Function to convert a Roman numeral string to its decimal value
int convertRomanToDecimal(const char roman[]) {
    int total = 0;
    for (int i = 0; i < strlen(roman); i++) {
        if (romanToDecimal(roman[i]) == 0) {
            printf("Invalid Roman numeral: %s\n", roman);
            return -1; // Return -1 for invalid input
        }
        if (romanToDecimal(roman[i]) < romanToDecimal(roman[i + 1])) {
            total -= romanToDecimal(roman[i]);
        } else {
            total += romanToDecimal(roman[i]);
        }
    }
    return total;
}

// Function to convert a number to its word representation
void numberToWords(int num, char *words) {
    static const char *ones[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    static const char *teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    static const char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    static const char *thousands[] = {"", "Thousand", "Million"};

    if (num == 0) {
        strcpy(words, "Zero");
        return;
    } else if (num < 0) {
        strcpy(words, "Negative ");
        char positiveWords[500];
        numberToWords(-num, positiveWords);
        strcat(words, positiveWords);
        return;
    }

    char temp[500] = "", final_result[500] = "";
    int part = 0;

    while (num > 0) {
        char segment[100] = "";
        int n = num % 1000;

        if (n != 0) {
            if (n / 100 > 0) {
                strcat(segment, ones[n / 100]);
                strcat(segment, " Hundred ");
            }

            n %= 100;
            if (n >= 10 && n <= 19) {
                strcat(segment, teens[n - 10]);
                strcat(segment, " ");
            } else {
                if (n / 10 > 0) {
                    strcat(segment, tens[n / 10]);
                    strcat(segment, " ");
                }
                if (n % 10 > 0) {
                    strcat(segment, ones[n % 10]);
                    strcat(segment, " ");
                }
            }

            if (thousands[part][0] != '\0') {
                strcat(segment, thousands[part]);
                strcat(segment, " ");
            }
        }

        strcpy(temp, segment);
        strcat(temp, final_result);
        strcpy(final_result, temp);
        num /= 1000;
        part++;
    }

    strcpy(words, final_result);
}

// Function to perform arithmetic operations
int performArithmetic(int num1, int num2, char operation) {
    switch (operation) {
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        default:
            printf("Unsupported operation: %c\n", operation);
            return 0;
    }
}

// Function to open a file safely
FILE* openFile(const char *filename, const char *mode) {
    FILE *file = fopen(filename, mode);
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }
    return file;
}

// Function to process the file and calculate results
void processFile(FILE *inputFile, FILE *outputFile) {
    RomanOperation operation;
    int decimal1, decimal2, result;
    char resultWords[500];

    while (fscanf(inputFile, "%s %c %s", operation.roman1, &operation.operation, operation.roman2) != EOF) {
        decimal1 = convertRomanToDecimal(operation.roman1);
        decimal2 = convertRomanToDecimal(operation.roman2);

        if (decimal1 == -1 || decimal2 == -1) {
            fprintf(outputFile, "Invalid Roman numeral input.\n");
            continue;
        }

        result = performArithmetic(decimal1, decimal2, operation.operation);
        numberToWords(result, resultWords);

        fprintf(outputFile, "%d (%s) %c %d (%s) = %d (%s)\n",
                decimal1, operation.roman1, operation.operation, decimal2, operation.roman2, result, resultWords);
    }
}

// Main function
int main() {
    FILE *inputFile = openFile("input.txt", "r");
    FILE *outputFile = openFile("output.txt", "w");

    if (inputFile && outputFile) {
        processFile(inputFile, outputFile);
        fclose(inputFile);
        fclose(outputFile);
        printf("Conversion and calculations complete. Check output.txt\n");
    } else {
        printf("Error with file operations.\n");
    }

    return 0;
}
