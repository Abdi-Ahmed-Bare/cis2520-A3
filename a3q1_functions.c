#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "a3q1_header.h"

// Define variables to store variable names and their values
Variable variables[10];  // Array to store up to 10 variables
int varCount = 0;        // Counter to keep track of how many variables are defined


// The createNode function allocates memory to the tree and creates a new node using the given data before returning the node.
Node* createNode(char *data){

	Node *newNode = (Node *)malloc(sizeof(Node));
    strcpy(newNode->data, data);
    newNode->left = newNode->right = NULL;
    return newNode;

}

// The parseExpression function parses the expression string passed in from command line, stores the information in a new node, and returns the root node of the tree.
Node* parseExpression(char *expr){

	Node *stack[100];  // Stack to hold nodes temporarily
    int top = -1;      // Stack pointer
    char token[10];    // Temporary string to hold current token

    for (int i = 0; expr[i] != '\0'; i++) {
        if (expr[i] == '(') {
            continue;  // Ignore opening parentheses
        } else if (expr[i] == ')') {
            Node *right = stack[top--];
            char op = expr[i-1]; // Operator just before ')'
            Node *left = stack[top--];
            Node *operatorNode = createNode(&op);
            operatorNode->left = left;
            operatorNode->right = right;
            stack[++top] = operatorNode; // Push the operator node back to the stack
        } else if (isdigit(expr[i]) || isalpha(expr[i])) {
            int j = 0;
            while (isdigit(expr[i]) || expr[i] == '.' || isalpha(expr[i])) {
                token[j++] = expr[i++];
            }
            token[j] = '\0';
            i--; // To counter the extra increment
            stack[++top] = createNode(token);
        }
    }

    return stack[top];  // The root of the expression tree

}

// The preOrder function prints tree nodes in preorder traversal.
void preorder(Node *root){
	if (root == NULL) return;
    printf("%s ", root->data);
    preorder(root->left);
    preorder(root->right);

}

// The inOrder function prints tree nodes in inorder traversal fully parenthesized.
void inorder(Node *root){
	if (root == NULL) return;

    if (root->left) {
        printf("(");
        inorder(root->left);
    }

    printf("%s", root->data);

    if (root->right) {
        inorder(root->right);
        printf(")");
    }

}

// The postOrder function prints tree nodes in postorder traversal.
void postorder(Node *root){
	if (root == NULL) return;
    postorder(root->left);
    postorder(root->right);
    printf("%s ", root->data);

}

// The promptVariables function prompts the user to assign values to each variable found in the expression tree. The values should be stored in the Variables struct.
void promptVariables(Node *root){
	if (root == NULL) return;
    
    // Check if the node is a variable
    if (isalpha(root->data[0])) {
        int found = 0;
        for (int i = 0; i < varCount; i++) {
            if (strcmp(root->data, variables[i].varName) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("Enter value for %s: ", root->data);
            scanf("%f", &variables[varCount].value);
            strcpy(variables[varCount].varName, root->data);
            varCount++;
        }
    }

    promptVariables(root->left);   // Left child
    promptVariables(root->right);  // Right child

}

// The calculate function calculates the expression and returns its result. Division by 0 and/or other error scenarios should be checked.
float calculate(Node *root){

	if (root == NULL) return 0.0;

    // If the node is a number, return its float value
    if (isdigit(root->data[0])) {
        return atof(root->data);
    }

    // If the node is a variable, retrieve its value
    if (isalpha(root->data[0])) {
        for (int i = 0; i < varCount; i++) {
            if (strcmp(root->data, variables[i].varName) == 0) {
                return variables[i].value;
            }
        }
        return 0.0;  // Return default value 0 if variable not found
    }

    // Handle operators: +, -, *, /
    float leftVal = calculate(root->left);
    float rightVal = calculate(root->right);

    if (strcmp(root->data, "+") == 0) return leftVal + rightVal;
    if (strcmp(root->data, "-") == 0) return leftVal - rightVal;
    if (strcmp(root->data, "*") == 0) return leftVal * rightVal;
    if (strcmp(root->data, "/") == 0) {
        if (rightVal == 0) {
            printf("Error: Division by zero\n");
            return 0;
        }
        return leftVal / rightVal;
    }

    return 0;  // Return default value if operator is unknown

}