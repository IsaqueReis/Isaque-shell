/*
   This file contains the function prototypes for a list data structure.
   by Autran 2018

   Implementation of prototypes
   by Isaque 2018
*/

//types definition
typedef void* list;
typedef void* list_item;
typedef void* key_item;

// listCreate
// ----------
// Create a list.
//
// Return a pointer to a list; or return NULL, in error case.

list list_create ();


// listDestroy
// -----------
// De-allocate the space memory used by l.
// 
// It is up to the application to release (free) the space 
// used by the items stored in l BEFORE destroying l.

void list_dealloc (list l);


// listGetI
// --------
// Get the p-th item of l (p > 0).
//
// Return the pointer to the p-th item of l; or return NULL, 
// if there is no p-th item in l.
//
// It is up to the application:
// - to guarantee that l points to the proper list;
// - to guarantee that p > 0.

list_item get (list l, int p);


// listIns
// -------
// Insert i in the position p of l, such that p > 0.
// If p > l->n, then i is inserted in the position l->n + 1.
//
// Return 0, if i was inserted in l at position p;
// or return > 0, in the case of error.
// 
// It is up to the application:
// - to guarantee that i and l points to the proper item and list;
// - to guarantee that p > 0.

int add (list_item i, int p, list l);


// listNOI
// -------
// Return the number of items in l.

int count (list l);


// listRmv
// -------
// Remove i from l.
//
// Return i or return NULL, if i is not found in l.
//
// It is up to the application:
// - to guarantee that i and l points to the proper item and list.

list_item  list_rmv (list_item i, list l);


// listSearch
// ----------
// Search an item in l whose key matches to the one in k.
//
// Return a pointer to the item that matches k; or return NULL, 
// whether l is empty or no item was found.
//
// cmp is a pointer to a user-defined comparison function in the user
// application. This function returns 0, if x == y, or return 
// a value != 0, otherwise.
//
// It is up to the application:
// - to guarantee that k and l points to the proper key and list;
// - to guarantee that p > 0.
//
// It is up to the application:
// - to guarantee that k and l points to the proper key and list.

list_item list_search (key_item k, list l, int (*cmp) (const list_item x, const list_item y));


// listSort
// --------
// Sort the content of l. 
//
// Return a new list such that their items are ordered according to cmp.
// The items in l maintain their original order (unsorted).
//
// cmp is a pointer to a user-defined comparison function in the user
// application. This function compares x against y; it returns a value
// less than, equal to, or greater than zero whether x < y, x == y or
// x > y, respectively.
// 
// It is up to the application to observe that 
// - l points to a list;
// - l is NOT empty;
// - x is a pointer to a pointer to (**) the item to be compared; so is y.
//   Therefore, cmp must de-reference these variables before comparing them.

list  list_sort (list l, int (*cmp) (const list_item x, const list_item y));