/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "dictionary.h"

// Implement a node for the trie
typedef struct node
{
    bool is_word;
    struct node *children[27];
}
node;

// Root of the trie - TODO ALLOCATE MEMORY INSIDE OF FUNCTION
node *root;

// File stream for the dictionary
FILE *g_dict = NULL;

// Prototype for freeing the trie
void free_trie(node *t_ptr);


/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // TODO

    // Allocate transversal pointer to the root of the trie, assumes that load has been called to allocate the root
    node *trav = root;

    // Use a for loop to loop through the word; note: Must be a valid string ex. "DanMazo\0"
    // Not going to use strlen
    for(int i = 0; word[i] != '\0'; i++){
        // Make sure we get the lower case, if it is an apostrophe, it should return the apostrophe value
        char low = tolower(word[i]);
        // Get index that corresponds to the letter, a-z = 0-25, apostrophe is 26
        int index = (low == '\'') ? 26 : ((int) low % 97);

        // Now that we have got the index, lets traverse throught the nodes
        // Check for null, if it is, then the word does not exist
        if(trav->children[index] == NULL){
            return false;
        } else {
            trav = trav->children[index];
        }

        // Check if we are at the end of the string
        if (word[i + 1] == '\0'){
            // Check if the last letter returns true, as what i have specified
            // We are only going to check for BOOL in the last letter
            if(trav->is_word == true){
                return true;
            } else {
                return false;
            }
        }
    }

    // This only happens if somehow the first element of "word" is a null terminator - '\0'
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // TODO

    //Allocate root node for loading tries;
    root = malloc(sizeof(node));
    // If we cannot allocate memory
    if(root == NULL){
        return false;
    }

    //Open Dictionary file;
    FILE *dict = fopen(dictionary, "r");
    if(dict == NULL){
        // If we can't allocate memory for the dictionary, then we unload the root
        free(root);
        return false;
    }

    // Allocate for each word in dictionary
    char word[LENGTH];

    // Allocate a traversal pointer to root
    node *trav = root;

    // Make valgrind happy, There, initialized your fucking nodes >:( yes i Mad.
    trav->is_word = false;
    for(int i = 0; i < 27; i++){
        trav->children[i] = NULL;
    }

    // Read through the strings
    while(fscanf(dict, "%s", word) != EOF){
        // use a for loop to put every single char in a trie
        for(int i = 0; i < LENGTH && word[i] != '\0'; i++ ){

            // Get the index for the child where to put the node, note: 0-25 a-z, 26th index is for the apostrophe
            int index = (word[i] == '\'') ? 26 : ((int) word[i] % 97);

            // Check if index of the child of the traversal node pointer is NULL, if so allocate away, if not, make trav point to that node
            if(trav->children[index] == NULL){
                // Malloc a new node
                node *tmp_node = malloc(sizeof(node));
                if(tmp_node == NULL){
                    return false;
                }
                // Initially make the temporary node is_word boolean false by default before linking
                tmp_node->is_word = false;
                for(int i = 0; i < 27; i++){
                    tmp_node->children[i] = NULL;
                }

                trav->children[index] = tmp_node;
                trav = trav->children[index];
            } else {
                trav = trav->children[index];
            }

            // Check if we are at the last index of the word, set is_word to true
            if(i == 44 || word[i + 1] == '\0'){
                trav->is_word = true;
            }
        }
        trav = root;
    }

    // Set stream of dictionary to the global variable for size()
    g_dict = dict;
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // TODO
    char word[100];
    int c = 0;

    // Rewind stream to the beginning
    if(fseek(g_dict, 0, SEEK_SET) == 0){
        while(fscanf(g_dict, "%s", word) != EOF){
            // Increment for every word scanned in the dictionary
            c++;
        }
    } else {
        // There is an error at the stream - close the file
        fclose(g_dict);
        return 0;
    }
    // Close the dictionary
    fclose(g_dict);

    return c;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // TODO

    // Define a recursive function that closes the trie from bottom to top
    free_trie(root);

    // Fingers Crossed!
    return true;
}

/**
 * This will recursively free every node in the trie. we are going to attempt tail call recursion
 */

void free_trie(node *t_ptr){
    // There are 27 Children in each node in a trie, get to the root and work from there
    // Create a for loop, looping through the child node
    for(int i = 0; i < 27; i++){
        // If some node is not null, enter that node recursively
        if(t_ptr->children[i] != NULL){
            free_trie(t_ptr->children[i]);
        }
        // Hopefully, if the child node resolves, execution will be resumed here
    }
    // Will execute if all nodes of the pointer are null
    // Free the called pointer and end execution or return to the calling stack below it
    free(t_ptr);

}

