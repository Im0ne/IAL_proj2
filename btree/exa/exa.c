/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte funkci letter_count. Výstupní strom může být značně degradovaný (až na úroveň lineárního seznamu) 
 * a tedy implementujte index druhou funkci (bst_balance), která strom, na požadavek uživatele, vybalancuje.
 * Funkce jsou na sobě nezávislé a tedy automaticky NEVOLEJTE bst_balance v letter_count.
 * 
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v 
 * uložen ve stromu.
 * 
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 * 
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 * 
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/
void letter_count(bst_node_t **tree, char *input) {
    bst_init(tree);                                         // we initialize the tree
    int index = 0;                                          // we initialize the counter
    while(input[index] != '\0')
    {
        char currentChar = input[index];
        if(currentChar >= 'A' && currentChar <= 'Z')        // if the character is A-Z
        {
            currentChar += 32;                              // convert to lowercase
        }

        if(currentChar >= 'a' && currentChar <= 'z')        // if the character is a-z
        {
            int value;
            if(bst_search(*tree, currentChar, &value))      // if the character is already in the tree
            {
                bst_insert(tree, currentChar, value + 1);   // we increase the value
            }
            else
            {
                bst_insert(tree, currentChar, 1);           // we insert the character with value 1
            }
        }
        else if(currentChar == ' ')                         // if the character is space
        {
            int value;
            if(bst_search(*tree, currentChar, &value))      // if the character is already in the tree
            {
                bst_insert(tree, currentChar, value + 1);   // we increase the value
            }
            else
            {
                bst_insert(tree, currentChar, 1);           // we insert the character with value 1
            }
        }
        else                                                // if the character is not a-z or space
        {
            int value;
            if(bst_search(*tree, '_', &value))              // if the character is already in the tree
            {
                bst_insert(tree, '_', value + 1);           // we increase the value
            }
            else
            {
                bst_insert(tree, '_', 1);                   // we insert the character with value 1
            }
        }
        index++;
    }
}

// Function to count the number of nodes in a binary search tree
int countNodes(bst_node_t* root)
{
    if (root == NULL) return 0;                                     // if root is NULL, we return 0
   
    return 1 + countNodes(root->left) + countNodes(root->right);    // we recursively count nodes in left and right subtree
}

// Function to build a balanced binary search tree from a sorted array of nodes
bst_node_t* buildTreeFromSortedArray(bst_items_t *items, int start, int end)
{
    if (start > end) return NULL;                                   // if start is greater than end, we return NULL

    int mid = (start + end)/2;                                      // we get the middle element
    bst_node_t *root = items->nodes[mid];                           // we set the middle element as root
    
    root->left  = buildTreeFromSortedArray(items, start, mid-1);    // we recursively build left and right subtree
    root->right = buildTreeFromSortedArray(items, mid+1, end);      

    return root;    
}

/**
 * Vyvážení stromu.
 * 
 * Vyvážený binární vyhledávací strom je takový binární strom, kde hloubka podstromů libovolného uzlu se od sebe liší maximálně o jedna.
 * 
 * Předpokládejte, že strom je alespoň inicializován. K získání uzlů stromu využijte vhodnou verzi vámi naimplmentovaného průchodu stromem.
 * Následně můžete například vytvořit nový strom, kde pořadím vkládaných prvků zajistíte vyváženost.
 *  
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce. Není nutné, aby funkce fungovala *in situ* (in-place).
*/
void bst_balance(bst_node_t **tree) {

    bst_items_t items;    
    items.capacity = countNodes(*tree);
    items.nodes = malloc(sizeof(bst_node_t*) * items.capacity); // initialization and allocation 
    items.size = 0;

  
    bst_inorder(*tree, &items);                                 // getting nodes from tree by inorder traversal

    *tree = buildTreeFromSortedArray(&items, 0, items.size-1);  // building balanced tree from sorted array of nodes

    free(items.nodes);                                          // cleaning after ourselves
}