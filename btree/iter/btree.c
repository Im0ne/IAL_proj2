/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h 
 * a připravených koster funkcí implementujte binární vyhledávací 
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
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
  *tree = NULL;                                  // we set tree to NULL
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 * 
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  while(tree)                  // while tree contains nodes
  {
    if(key == tree->key)       // if key is equal to tree->key
    {
      *value = tree->value;    // we found the key, so we return true and value
      return true;
    }

    else if(key < tree->key)   // if key is less than tree->key
    {
      tree = tree->left;       // we go to left subtree
    }

    else if(key > tree->key)   // if key is greater than tree->key
    {
      tree = tree->right;      // we go to right subtree
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
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  bst_node_t *node = malloc(sizeof(bst_node_t));  // we allocate memory for node
  
  if(!node) return;                               // allotion check                                        
  
  node->key = key;                                // we set key and value
  node->value = value;                            
  node->left = NULL;                              // nodes init
  node->right = NULL;                             

  if(!*tree)                                      // if tree is empty
  {
    *tree = node;                                 // we root to node
    return;
  }

  else                                            // if tree is not empty
  {
    bst_node_t *current = *tree;                  // we create current node and set it to tree

    while(current)                                // while current contains nodes
    {
      if(key == current->key)
      {
        current->value = value;                   // if key is equal to current->key, we replace value
        free(node);                               // we free node
        return;
      }
      
      else if(key < current->key)                 // if key is less than current->key
      {
        
        if(current->left)                         // if left subtree of current is empty
        {
          current = current->left;                // we set left subtree of current to node
        }
        
        else                                      // if left subtree of current is not empty
        {
          current->left = node;                   // we go to left subtree
          return;
        }
      }
      
      else if(key > current->key)                 // if key is greater than current->key
      {
        
        if(current->right)                        // if right subtree of current is empty
        {
          current = current->right;               // we set right subtree of current to node
        }
        
        else                                      // if right subtree of current is not empty
        {
          current->right = node;                  // we go to right subtree
          return;
        }
      }
    }
  }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  bst_node_t *current = *tree;                        
  bst_node_t *parent = target;      
  
  if(!current) return;                // if tree is empty, we return
  
  while(current->right){
    parent = current;                 // we set parent to current
    current = current->right;         // we go to right subtree
  }

  if(current->left)
  {
    target->key = current->key;       // we replace key and value of target with key and value of rightmost node
    target->value = current->value;
    
    parent->right = current->left;    // we set parent->right to current->left
    free(current);                    // we free current
  }
  
  else
  {
    target->key = current->key;       // we replace key and value of target with key and value of rightmost node
    target->value = current->value;
    
    if(target!=parent)                // if target is not parent
    {
      parent->right = NULL;           // we set parent->right to NULL
    }
    
    else 
    {
      parent->left = NULL;            // we set parent->left to NULL
    }
    
    free(current);                    // we free current
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
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
  bst_node_t *current = *tree;                        
  bst_node_t *parent = NULL;      
  
  if(!current) return;                                      // if tree is empty, we return
  
  while(current)                                            // while current contains nodes
  {
    if(key == current->key)
    {
      if(current->left && current->right)
      {
        bst_replace_by_rightmost(current, &current->left);  // if current has both subtrees, we replace it with rightmost node in left subtree
      }
      else if(!current->left && !current->right)
      {
        if(current == *tree)
        {
          free(current);                                    // if current is root and has no subtrees, we free current
          return;
        }
        if(current == parent->left)
        {
          parent->left = NULL;                              // if current is left child of parent, we set parent->left to NULL
        }
        else if(current == parent->right)
        {
          parent->right = NULL;                             // if current is right child of parent, we set parent->right to NULL
        }
        free(current);
        break;
      }
      else
      {
        if(current == *tree)
        {
          if(current->left)
          {
            *tree = current->left;                          // if current is root and has only left subtree, we set tree to left subtree
          }
          else if(current->right)
          {
            *tree = current->right;                         // if current is root and has only right subtree, we set tree to right subtree
          }
        }
        else if(current == parent->left)
        {
          if(current->left)
          {
            parent->left = current->left;                   // if current is left child of parent and has only left subtree, we set parent->left to left subtree
          }
          else if(current->right)
          {
            parent->left = current->right;                  // if current is left child of parent and has only right subtree, we set parent->left to right subtree
          }
        }
        else if(current == parent->right)
        {
          if(current->left)
          {
            parent->right = current->left;                  // if current is right child of parent and has only left subtree, we set parent->right to left subtree
          }
          else if(current->right)
          {
            parent->right = current->right;                 // if current is right child of parent and has only right subtree, we set parent->right to right subtree
          }
        }
        free(current);
        break;
      }
    }
    else if(key < current->key)
    {
      if(current->left)
      {
        parent = current;                                   // we set parent to current
        current = current->left;                            // we go to left subtree
      }
      else
      {
        return;                                             // if left subtree of current is empty, we return
      }
    }
    else if(key > current->key)
    {
      if(current->right)
      {
        parent = current;                                   // we set parent to current
        current = current->right;                           // we go to right subtree
      }
      else
      {
        return;                                             // if right subtree of current is empty, we return
      }
    }
  }
}

/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
  stack_bst_t stack;
  stack_bst_init(&stack);                         // we initialize stack

  stack_bst_push(&stack, *tree);                  // we push tree to stack

  while(!stack_bst_empty(&stack)){                // until stack is empty
    bst_node_t *current = stack_bst_pop(&stack);  // assigning top of stack to current
    
    if(!current) continue;                        // if current is NULL, we continue

    stack_bst_push(&stack, current->left);        // we push left and right subtree of current to stack
    stack_bst_push(&stack, current->right);       

    free(current);
  }
  
  *tree = NULL;
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items) {
  bst_node_t *current = tree;                       

  while(current)                                    // while current contains nodes
  {
    bst_add_node_to_items(current, items);          // we add current to items
    stack_bst_push(to_visit, current);              // we push current to stack
    current = current->left;                        // we go to left subtree
  }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
  stack_bst_t stack;
  stack_bst_init(&stack);                                 // we initialize stack

  bst_leftmost_preorder(tree, &stack, items);             // going to most left in left subtree

  while(!stack_bst_empty(&stack))                         // until stack is empty
  {
    bst_node_t *current = stack_bst_pop(&stack);          // assigning top of stack to current

    bst_leftmost_preorder(current->right, &stack, items); // we go to most left in right subtree
  }
}

/*
 * Pomocná funkce pro iterativní inorder.
 * 
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  bst_node_t *current = tree;                       

  while(current)                                    // while current contains nodes
  {
    stack_bst_push(to_visit, current);              // we push current to stack
    current = current->left;                        // we go to left subtree
  }

}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {  
  stack_bst_t stack;
  stack_bst_init(&stack);                                 // we initialize stack
  
  bst_leftmost_inorder(tree, &stack);                     // going to most left in left subtree

  bst_node_t *current = tree;

  while(!stack_bst_empty(&stack))                         // until stack is empty
  {
    current = stack_bst_pop(&stack);                      // assigning top of stack to current
    bst_add_node_to_items(current, items);                // we add current to items
    bst_leftmost_inorder(current->right, &stack);         // we go to most left in right subtree
  }
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
  bst_node_t *current = tree;
  
  while(current)                                    // while current contains nodes
  {
    stack_bst_push(to_visit, current);              // we push current to stack
    stack_bool_push(first_visit, true);             // we push true to stack
    current = current->left;                        // we go to left subtree
  }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
  stack_bst_t stack;
  stack_bst_init(&stack);                                             // we initialize stack

  stack_bool_t first_visit;
  stack_bool_init(&first_visit);                                      // we initialize stack

  bst_leftmost_postorder(tree, &stack, &first_visit);                 // going to most left in left subtree

  while(!stack_bst_empty(&stack))                                     // until stack is empty
  {
    bst_node_t *current = stack_bst_pop(&stack);                      // assigning top of stack to current
    bool first = stack_bool_pop(&first_visit);                        // assigning top of stack to first

    if(first)                                                         // if it's first
    {
      stack_bst_push(&stack, current);                                // we push current to stack
      stack_bool_push(&first_visit, false);                           // we push false to stack
      bst_leftmost_postorder(current->right, &stack, &first_visit);   // we go to most left in right subtree
    }

    else
    {
      bst_add_node_to_items(current, items);                          // we add current to items
    }
  }
}
