/*
 * Binární vyhledávací strom — rekurzivní varianta
 *
 * S využitím datových typů ze souboru btree.h a připravených koster funkcí
 * implementujte binární vyhledávací strom pomocí rekurze.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci. 
 */
void bst_init(bst_node_t **tree) {
  *tree = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 * 
 * Funkci implementujte rekurzivně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  if(tree)                                          // if tree contains nodes
  { 
    if(key == tree->key)                              // if key is equal to tree->key
    {
      *value = tree->value;                           // we found the key, so we return true and value
      return true;
    }

    else if(key < tree->key)                               // if key is less than tree->key
    {
      return bst_search(tree->left, key, value);      // we recursively search in left subtree
    }

    else if(key > tree->key)                               // if key is greater than tree->key
    {
      return bst_search(tree->right, key, value);     // we recursively search in right subtree
    }
  }
  return false;
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší. 
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  if(*tree == NULL) {                           // if tree is empty
    *tree = malloc(sizeof(bst_node_t));         // we allocate memory for new node
    
    (*tree)->key = key;                         // we set key and value
    (*tree)->value = value;
    
    (*tree)->left = NULL;                       // we set left and right subtree to NULL
    (*tree)->right = NULL;   
  }
  
  else if(key == (*tree)->key) {                
    (*tree)->value = value;                     // if key is equal to tree->key, we replace value
  }
  
  else if(key < (*tree)->key) {
    bst_insert(&(*tree)->left, key, value);     // if key is less than tree->key, we recursively insert in left subtree
  }
  
  else if(key > (*tree)->key) {
    bst_insert(&(*tree)->right, key, value);    // if key is greater than tree->key, we recursively insert in right subtree
  }
  return;
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazeny klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využitá při implementaci funkce bst_delete.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  if((*tree)->right) {                          // if right subtree isnt NULL
    if((*tree)->right->right)                   // if right subtree of right subtree isnt NULL
    {
      bst_replace_by_rightmost(target, &(*tree)->right);    // we recursively search for rightmost node
    }
    else
    {
      target->key = (*tree)->right->key;       // we replace key and value of target with key and value of rightmost node
      target->value = (*tree)->right->value;
      
      bst_node_t *tmp = (*tree)->right;        // we free the rightmost node
      (*tree)->right = (*tree)->right->left;   // we set right subtree of tree to left subtree of rightmost node if it exists
      free(tmp);
      
      return;
    }
  }
  
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 * 
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 * 
 * Funkci implementujte rekurzivně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
  if(!*tree) 
  {
    return;                                             // if tree is empty, we return
  }
  
  if(key < (*tree)->key) 
  {
    bst_delete(&(*tree)->left, key);                    // if key is less than tree->key, we recursively delete in left subtree
    return;
  }
  
  if(key > (*tree)->key) 
  {
    bst_delete(&(*tree)->right, key);                   // if key is greater than tree->key, we recursively delete in right subtree
    return;
  }
  
  if(!(*tree)->left) 
  {
    bst_node_t *tmp = *tree;                            // if left subtree is empty, we replace tree with right subtree and free left subtree
    *tree = (*tree)->right;
    free(tmp);
    return;
  }
  
  if(!(*tree)->right) 
  {
    bst_node_t *tmp = *tree;                            // if right subtree is empty, we replace tree with left subtree and free right subtree
    *tree = (*tree)->left;
    free(tmp);
    return;
  }
  
  bst_replace_by_rightmost(*tree, &(*tree)->left);      // if both subtrees arent empty, we replace tree with rightmost node of left subtree
}

/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
  if(*tree)                       // if tree contains nodes
  {
  bst_dispose(&(*tree)->right);   // recursively dispose right and left subtree
  bst_dispose(&(*tree)->left);    
  free(*tree);
  *tree = NULL;
  }
  
  return;
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
  if(tree)                              // if tree contains nodes
  {
  bst_add_node_to_items(tree, items);   // we're going from root to left subtree to right subtree
  bst_preorder(tree->left, items);      
  bst_preorder(tree->right, items);     
  }
  
  return;
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
  if(tree)                              // if tree contains nodes
  {   
  bst_inorder(tree->left, items);       // we're going from left subtree to root to right subtree
  bst_add_node_to_items(tree, items);   
  bst_inorder(tree->right, items);      
  }
  
  return;
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
  if(tree)                              // if tree contains nodes 
  {
  bst_postorder(tree->left, items);     //  we're going from left subtree to right subtree to root
  bst_postorder(tree->right, items);    
  bst_add_node_to_items(tree, items);   
  }
  
  return;
}