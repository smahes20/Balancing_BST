#include "bst.h"
#include <stdio.h>
#include <stdlib.h>

struct bst_node {
    int val;
    int sizeleft;
    int sizeright;
    struct bst_node *left;
    struct bst_node *right;
    int work;

};
typedef struct bst_node NODE;


struct bst {
    NODE *root;
};

BST * bst_create(){
  BST * t = malloc(sizeof(struct bst));
  t->root = NULL;
  return t;
}


static void free_r(NODE *r){
    if(r==NULL) return;
    free_r(r->left);
    free_r(r->right);
    free(r);
}
void bst_free(BST * t){
    free_r(t->root);
    free(t);
}
//================================================================

//functionst hat check if the BST or the root node if empty or not 
// Returns 1 if empty tree
static int is_empty_tree(BST *t) {
	return t==NULL;
}

// Returns 1 if tree is not empty
static int is_not_empty_tree(BST *t) {
	return is_empty_tree(t)==0;
}

// Returns 1 if empty subtree
static int is_empty_subtree(NODE *r) {
	return r==NULL;
}

// Returns 1 if subtree is not empty
static int is_not_empty_subtree(NODE *r) {
	return is_empty_subtree(r)==0;
}
//===============================================================

static int size(NODE *r){
    
    if(r==NULL) return 0;
    return size(r->left) + size(r->right) + 1;
}
int bst_size(BST * t){
    
    return size(t->root);
}

static int height(NODE *r){
    int l_h, r_h;

    if(r==NULL) return -1;
    l_h = height(r->left);
    r_h = height(r->right);
    return 1 + (l_h > r_h ? l_h : r_h);

}
int bst_height(BST * t){
    return height(t->root);

}

//bst to array function 
int *copy_elements(int *array, int newsize, int oldsize){
    
    int i =0;
    int *newarray = (int*)malloc(sizeof(int)* newsize);
    for(i = 0; i < oldsize; i++){
        newarray[i] = array[i];
    }
    return newarray;
}

//make a helper function 
void bst_to_array_helper(NODE *root, int *count, int *top, int **array){
    
    int newtop;
    
    if (root == NULL){
        return;
    }
    else{
    
        bst_to_array_helper(root->left, count, top, array);
        if (*count >= *top){
    
            newtop = *top * 2; 
            int *newarray = *array;
            *array = copy_elements(*array, *count, newtop);
            *top = newtop;
            free(newarray);
        }
    
        (*array)[*count] = root->val;
        (*count)++;
        bst_to_array_helper(root->right, count, top, array);
    }
}
    
    /* allocates an integer array, populates it with the
    elements of t (in-order) and returns the array as an
    int pointer */
int * bst_to_array(BST * t){
    
    int count = 0;
    int top = 10;
    int i = 0;
    
    int *array = (int*) malloc(sizeof(int) * top);
    bst_to_array_helper(t->root, &count, &top, &array);
    printf("count: %d\n", count);
    printf("top/capacity: %d\n", top);
    
    for (i = 0;i < count;i++){
        printf("(%d) ", array[i]);
    }
    printf("\n");
    return array;
}

int bst_sb_work(BST *t)
{
    return t->root->sizeleft + t->root->sizeright + 1;
}

///SIZE BALANCING 

int max(int x, int y)
{
    return (x > y) ? x : y;          
}

int min(int x, int y)
{
    return (x < y) ? x : y;          
}
/* 
 * Write the (recursive) helper function from_arr, used by
 * bst_from_sorted_arr(...). The function must return a sub-tree that is
 * perfectly balanced, given a sorted array of elements a.
 */
 /////////////////////////////////////////////////
 static NODE * from_arr(int *a, int n){
    int m;
    NODE *root;
    
        if(n <= 0)
        {
            return NULL;
        }
        m = n/2;
        root = malloc(sizeof(NODE));
        root->val = a[m];
        if (n % 2 == 0)
        {
            root->sizeleft = n - m;
        }
        else
        {
            root->sizeleft = n - m - 1;
        }
        root->sizeright = n - m - 1;
        root->left = from_arr(a, m);
        root->right = from_arr(&(a[m+1]), n-(m+1));
        return root;
    
    }
    
BST * bst_from_sorted_arr(int *a, int n){
    
    BST * t = bst_create();
    
    t->root = from_arr(a, n);
    
    return t;
}


static NODE * createNode(int value)
{
    NODE *newNode = (NODE*)malloc(sizeof(NODE));
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->val = value;
    newNode->sizeleft = 0;
    newNode->sizeright = 0;
    return newNode;
}

int isBalance(NODE *root)
{
   int leftEq = max(root->sizeleft, root->sizeright);
   int rightEq = 2 * min(root->sizeleft, root->sizeright) + 1;

   if (leftEq <= rightEq)
   {
       return 1;
   }
   else
   {
       return 0;
   }
}
static NODE * insert_helper(NODE *r, int x, int *work)
{
    // Base case: Tree is empty
    if (r == NULL)
    {
        return(createNode(x));
    }

    // Recursive case: The value we are searching for is smaller than the 
    // current node.
    if (x < r->val)
    {
        r->left = insert_helper(r->left, x, work);
        r->sizeleft = r->sizeleft + 1;
    }
    // Recursive case: The value we are searching for is greater than the
    // current node.
    else if ( x > r->val)
    {
        r->right = insert_helper(r->right, x, work);
        r->sizeright = r->sizeright + 1;
    }
    //the value is equal to the current node
    else 
    {
        return r;
    }

    if (!isBalance(r))
    {
        int size = r->sizeleft + r->sizeright + 1;
        BST *t = bst_create();
        t->root = r;
        int * arr = bst_to_array(t);
        t = bst_from_sorted_arr(arr, size);
        *work = *work + bst_sb_work(t);
        printf("the work done is: %d\n", *work);
        r = t->root;
    }
    return r;
}
static NODE * insert(NODE *r, int x)
{
    int work = 0;
    return insert_helper(r, x, &work);
}


// how about an iterative version?
static NODE *insert_i(NODE *r, int x){

  return NULL;

}


void bst_insert(BST * t, int x){
    if(bst_contains(t, x) == 0){
        t->root = insert(t->root, x);
    }
}

int bst_contains(BST * t, int x){
    NODE *p = t->root;

    while(p != NULL){

        if(p->val == x)
            return 1;
        if(x < p->val){
            p = p->left;
        }
        else
            p = p->right;
    }
    return 0;  
}

static int min_h(NODE *r){
  if(r==NULL)
    return -1; // should never happen!
  while(r->left != NULL)
      r = r->left;
  return r->val;
}

static int max_h(NODE *r){
  if(r==NULL)
    return -1; // should never happen!
  while(r->right != NULL)
      r = r->right;
  return r->val;
}

static NODE * remove_r_helper(NODE *r, int x, int *success, int *work){
    NODE   *tmp;
    int sanity;
    
      if(r==NULL){
        *success = 0;
        return NULL;
      }
      if(r->val == x){
        *success = 1;
    
        if(r->left == NULL){
            tmp = r->right;
            r->sizeleft = r->sizeleft - 1;
            free(r);
            return tmp;
        }
        if(r->right == NULL){
            tmp = r->left;
            r->sizeright = r->sizeright - 1;
            free(r);
            return tmp;
        }
        // if we get here, r has two children
        r->val = min_h(r->right);
        r->right = remove_r_helper(r->right, r->val, &sanity, work);
        if(!sanity)
            printf("ERROR:  remove() failed to delete promoted value?\n");
        return r;
      }
      if(x < r->val){
        r->left = remove_r_helper(r->left, x, success, work);
        r->sizeleft = r->sizeleft - 1;
      }
      else {
        r->right = remove_r_helper(r->right, x, success, work);
        r->sizeright = r->sizeright - 1;
      }
      if (!isBalance(r))
      {
          int size = r->sizeleft + r->sizeright + 1;
          BST *t = bst_create();
          t->root = r;
          int * arr = bst_to_array(t);
          t = bst_from_sorted_arr(arr, size);
          *work = *work + bst_sb_work(t);
          printf("the work done in remove: %d\n", *work);
          r = t->root;
      }
      return r;
}
static NODE *remove_r(NODE *r, int x, int *success){
    int work = 0;
    return remove_r_helper(r, x, success, &work);

}


int bst_remove(BST * t, int x){
    int success;
    t->root = remove_r(t->root, x, &success);
    return success;
}


int bst_min(BST * t){
    return min_h(t->root);
}

int bst_max(BST * t){
    return max_h(t->root);
}

static void indent(int m){
    int i;
    for(i=0; i<m; i++)
        printf("-");
}

static void inorder(NODE *r){
  if(r==NULL) return;
  inorder(r->left);
  printf("[%d]\n", r->val);
  inorder(r->right);

}
void bst_inorder(BST * t){

  printf("========BEGIN INORDER============\n");
  inorder(t->root);
  printf("=========END INORDER============\n");

}

static void preorder(NODE *r, int margin){
  if(r==NULL) {
    indent(margin);
    printf("NULL \n");
  } else {
    indent(margin);
    printf("%d\n", r->val);
    preorder(r->left, margin+3);
    preorder(r->right, margin+3);
  }
}

void bst_preorder(BST * t){

  printf("========BEGIN PREORDER============\n");
  preorder(t->root, 0);
  printf("=========END PREORDER============\n");

}

/* 
 * Complete the (recursive) helper function for the post-order traversal.
 * Remember: the indentation needs to be proportional to the height of the node!
 */
static void postorder(NODE *r, int margin){
    /* FILL IN FUNCTION */
    if(r==NULL) {
        indent(margin);
        printf("NULL \n");
    } 
    else {
        indent(margin);
        postorder(r->left, margin+3);
        postorder(r->right, margin+3);
        printf("%d\n", r->val);
    }
    
}

// indentation is proportional to depth of node being printed
//   depth is #hops from root.
void bst_postorder(BST * t){

  printf("========BEGIN POSTORDER============\n");
  postorder(t->root, 0);
  printf("=========END POSTORDER============\n");

}
//==================================================================

//===============================================================
// Recursive helper for bst_get_ith
static void  bst_get_ith_helper(NODE *root, int i, int *result) {
	if (is_empty_subtree(root)){
        return;
    } 
	
	if (size(root->left) + 1  == i) {
		*result = root->val;
		return;
	}
	else if (size(root->left) > i) {
		bst_get_ith_helper(root->left, i, result);
	}
	else if (size(root->left) < i) {
		bst_get_ith_helper(root->right, i-(size(root->left)+1), result);
    }
}

/* returns the ith smallest element in t.  i ranges
   from 1..n where n is the number of elements in
   the tree.

   If i is outside this range, an error message is printed
   to stderr and the return value is arbitrary (you may return
   whatever you like, but it has no meaning.

   Runtime:  O(h) where h is the tree height
*/

int bst_get_ith(BST *t, int x) {
    
    if (is_empty_tree(t) || is_empty_subtree(t->root) || x<1 || x>size(t->root)) {
		fprintf(stderr,"Tree is empty or x is out of bounds x: %d  tree size: %d\n", x, size(t->root));
		return -1;
    }
    int result = -1;
    bst_get_ith_helper(t->root, x, &result);
	return result;
}

//=====================================================================

//====================================================================
// Recursive helper for bst_get_nearest
static void bst_get_nearest_helper(NODE *r, int x, int *closest) {
	
	NODE *b = r->left;
	NODE *c = r->right;
    
    if (is_empty_subtree(r)) return;

	// Compare previous closest elem with current elem
	if (abs(x- r->val) < abs(x- *closest)){
        *closest = r->val;
    } 
	
	// Closest elem MUST be current
	if (r->val==x) {
		*closest = x;
		return;
    }
    // Closest elem is current OR in left subtree
	if (x < r->val){
        bst_get_nearest_helper(b, x, closest);
    }
    // Closest elem is current OR in right subtree
    else{
        bst_get_nearest_helper(c, x, closest);
    }
}
/* returns the value in the tree closest to x -- in other
   words, some y in the tree where |x-y| is minimum.

   If the tree is empty, a message is sent to stderr and 
   the return value is of your choosing.

   Runtime:  O(h) where h is the tree height.
*/

// Returns the elem with closest proximity to x
int bst_get_nearest(BST *t, int x) {

    if (is_empty_tree(t) || is_empty_subtree(t->root)) {
		fprintf(stderr, "Tree is empty!\n", size(t->root));
		return -1;
	}
	
	int closest = t->root->val;
	bst_get_nearest_helper(t->root, x, &closest);
	return closest; 
}
   
// //=========================================================

//==========================================================
void bst_num_leq_helper(NODE *root, int x, int *counter)
{
    if (root == NULL){
        return;
    }
    if (root->val > x){
        bst_num_leq_helper(root->left, x, counter);
    }
    else{
        (*counter)++;
        bst_num_leq_helper(root->left, x, counter);
        bst_num_leq_helper(root->right, x, counter);

    }
}

/* returns the number of elements in t which are less
   than or equal to x.

   Runtime:  O(h) where h is the tree height

*/

// Returns the number of elem less than or equal to x
int bst_num_leq(BST *t, int x)
{

    int counter = 0;
    bst_num_leq_helper(t->root, x, &counter);

    return counter;
}

//===========================================================

//===========================================================
void bst_num_geq_helper(NODE *root, int x, int *counter){
    if (root == NULL){
        return;
    }

    if(x < (root->val)){
        (*counter)++;
        bst_num_geq_helper(root->left, x, counter);
        bst_num_geq_helper(root->right, x, counter);
    }
    else{
        bst_num_geq_helper(root->right, x, counter);
    }
}

/* returns the number of elements in t which are greater
   than or equal to x.

   Runtime:  O(h) where h is the tree height

*/

int bst_num_geq(BST *t, int x)
{
    int counter = 0;
    bst_num_geq_helper(t->root, x, &counter);

    return counter;
}

//================================================================

//====================================================================
void bst_num_range_helper(NODE *root, int min, int max, int *counter){
    if (root == NULL){
        return; 
    }
    if (root->val >= min && root->val <= max){
        (*counter)++;
    }
    if (root->val > min){
        
        bst_num_range_helper(root->left, min, max, counter);
        
    }
    if (root->val < max){
        
        bst_num_range_helper(root->right, min, max, counter);
    }
    
}
/* returns the number of elements in t which are between min
       and max (inclusive).

   Runtime:  O(h) where h is the tree height

*/

//returns the number of elements in t which are in between min and max
int bst_num_range(BST *t, int min, int max){
    
    int counter = 0;
    bst_num_range_helper(t->root, min, max, &counter);
    return counter;
}
//========================================================

