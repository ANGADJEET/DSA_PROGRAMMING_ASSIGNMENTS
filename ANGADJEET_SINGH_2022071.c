#include "../include/common.h"
#include "pa2.h"

// don't remove these globals
static struct record *bst_root = NULL;
static struct record *avl_root = NULL;
static size_t num_bst_nodes = 0;
static size_t num_avl_nodes = 0;

// don't modify this function
struct record *get_bst_root()
{
  return bst_root;
}

// don't modify this function
struct record *get_avl_root()
{
  return avl_root;
}

// don't modify this function
// compare two uids2
// return -1 if uid1 < uid2
// return 0 if uid1 == uid2
// return 1 if uid1 > uid2
static int cmp_uid(char *uid1, char *uid2)
{
  int i;

  for (i = 0; i < MAX_LEN; i++)
  {
    if (uid1[i] > uid2[i])
    {
      return 1;
    }
    else if (uid1[i] < uid2[i])
    {
      return -1;
    }
  }
  return 0;
}

// don't modify this function
// compare two records
// we assume that uid is unique and
// there can't be two records with the
// same uid
// return -1 if record1 < record2
// return 0 if record1 == record2
// return 1 if record1 > record2
static int cmp_record(struct record *r1, struct record *r2)
{
  return cmp_uid(r1->uid, r2->uid);
}

// don't modify this function
static void *allocate_memory(size_t size)
{
  return __mymalloc(size);
}

// don't modify this function
static void free_memory(void *ptr)
{
  __myfree(ptr);
}

// ---------------- Add your code below this line ----------------- //

// Return the total number of records in the
// BST rooted at bst_root.
size_t get_num_bst_records()
{
  return num_bst_nodes;
}

// Return the total number of records in the
// AVL tree rooted at avl_root.
size_t get_num_avl_records()
{
  return num_avl_nodes;
}

struct record* insert_record_b(struct record* root, struct record r)
{
  if(root == NULL)
  {
    num_bst_nodes++;
    struct record * temp = (struct record *)allocate_memory(sizeof(struct record));
    *temp = r;
    temp->right = temp->left = NULL;
    temp->friends = NULL;
    return temp;
  }
  if(cmp_uid(r.uid,root->uid)<0)
  {
    root->left = insert_record_b(root->left, r);
  }
  else
  {
    root->right = insert_record_b(root->right,r);
  }
  return root;
}

// Update bst_root after an insertion.
void insert_record_bst(struct record r)
{
  bst_root = insert_record_b(bst_root , r);
}

void inorder(struct record* root)
{
    if (root != NULL)
    {
        inorder(root->left);
        puts(root->uid);
        inorder(root->right);
    }
}

struct record* find_min(struct record *bst_root)
{
  if(bst_root == NULL)
  {
    return NULL;
  }
  if(bst_root->left == NULL)
  {
    return bst_root;
  }
  else
  {
    return find_min(bst_root->left);
  }
}

struct record* allocate_node(struct record r) {
  struct record* newNode = allocate_memory(sizeof(struct record));
  *newNode = r;
  newNode->left = NULL;
  newNode->right = NULL;
  newNode -> friends = NULL;

  return newNode;
}
// struct record* find_min(struct record* root) {
//   if (root == NULL || root->left == NULL)
//     return root;

//   return find_min(root->left);
// }

// Function to get the balance of the node of an AVL tree
int get_balance(struct record* n) {
  int l_height = (n->left) ? n->left->height: -1;
  int r_height = (n->right) ? n->right->height: -1;
  return l_height - r_height;
}

// Function to set the height of the node of an AVL tree
void set_height(struct record* n) {
  int l_height = (n->left) ? n->left->height: -1;
  int r_height = (n->right) ? n->right->height: -1;
  n -> height = (l_height > r_height) ? l_height + 1: r_height + 1;
}

// Function to perform an LL rotation on the node of an AVL tree
struct record* LL(struct record* n) {
  struct record *new_root = n->left;
  n->left = new_root->right;
  new_root->right = n;
  set_height(n);
  set_height(new_root);
  return new_root;
}

// Function to perform an RL rotation on the node of an AVL tree
struct record* RL(struct record* n) {
  struct record *mid = n->right;
  struct record *new_root = mid->left;
  n->right = new_root->left;
  mid->left = new_root->right;
  new_root->left = n;
  new_root->right = mid;
  set_height(n);
  set_height(mid);
  set_height(new_root);
  return new_root;
}

// Function to perform an LR on the node of an AVL tree
struct record* LR(struct record* n) {
  struct record *mid = n->left;
  struct record *new_root = mid->right;
  n->left = new_root->right;
  mid->right = new_root->left;
  new_root->right = n;
  new_root->left = mid;
  set_height(mid);
  set_height(n);
  set_height(new_root);
  return new_root;
}

// Function to perform an RR on the node of an AVL tree
struct record* RR(struct record* n) {
  struct record *new_root = n->right;
  n->right = new_root->left;
  new_root->left = n;
  set_height(n);
  set_height(new_root);
  return new_root;
}

// Function to balance nodes and perform the required rotations on the node of an AVL tree
struct record* try_rotate(struct record* n) {
  int bal = get_balance(n);

  if (bal < -1) {
    bal = get_balance(n->right);

    if (bal > 0)
      n = RL(n);
   
    else
      n = RR(n);
  }

  else if (bal > 1) {
    bal = get_balance(n->left);
    if (bal < 0)
      n = LR(n);

    else
      n = LL(n);
  }
  return n;
}



// Function to insert node in AVL tree
struct record* insert_avl(struct record* root, struct record r) {
  if (root == NULL) {
    num_avl_nodes++;
    return allocate_node(r);
  }

  else {
    if (cmp_uid(r.uid, root->uid) >= 0) {
      root->right = insert_avl(root->right, r);
    }
    else {
      root->left = insert_avl(root->left, r);
    }

    set_height(root);
    root = try_rotate(root);
    return root;
  }
}



// Insert record r in the AVL tree rooted at avl_root.
void insert_record_avl(struct record r)
{
  avl_root = insert_avl(avl_root,r);
   return;
}

// Search the record corresponding to uid in the BST
// rooted at bst_root.
// If the record is not present, return a dummy record
// with -1 in the status field.
struct record search_record_bst(char uid[MAX_LEN])
{
    struct record *ptr = bst_root;

    while (ptr != NULL)
    {
        int cmp = cmp_uid(uid, ptr->uid);
        if (cmp == -1)
        {
            ptr = ptr->left;
        }
        else if (cmp == 1)
        {
            ptr = ptr->right;
        }
        else // record found.
        {
            struct record result = *ptr;
            return result;
        }
    }

    struct record dummy = { .status = -1 };
    return dummy;
}



struct record *corresponding_record_of(char uid[MAX_LEN])
{
  struct record *ptr = bst_root;

  while (ptr != NULL)
  {
    int cmp_result = cmp_uid(uid, ptr->uid);
    if (cmp_result == -1)
    {
      ptr = ptr->left;
    }
    else if (cmp_result == 1)
    {
      ptr = ptr->right;
    }
    else // record found
    {
      return ptr;
    }
  }
  // record not found
  return NULL;
}

// The friends field in "struct record" stores the
// head of the linked list of friends of a given user.
// Return the head of the linked list of friends
// of the user with uid uid in the BST rooted at bst_root.
// If the corresponding record doesn't exist, return NULL.
struct list_records *get_friends_list_bst(char uid[MAX_LEN])
{
  struct record *A = corresponding_record_of(uid);
  struct list_records *A_list = A->friends;
  return A_list;
}

struct record* next_node(struct record * bst_root)
{
  if(bst_root == NULL)
  {
    return NULL;
  }
  bst_root = bst_root -> right;
  while(bst_root->left != NULL)
  {
    bst_root = bst_root->left;
  }
  return bst_root;
}


// Delete the record (say n) corresponding to uid from the BST.
// Remove n from the lists of friends of other records
// and release the memory for the linked list nodes.
// Release memory for all the nodes in the list of friends of n.
// Return a copy of the value of the deleted record.
// If the record is not present, return a dummy record
// with -1 in the status field.
// struct record delete_record_bst(char uid[MAX_LEN])
// {
//   struct record *A = corresponding_record_of(uid);
 

//   if (A != NULL) {
//     bst_root = delete(uid, bst_root);

//     struct record *curr_ptr = bst_root;

//     while (curr_ptr != NULL) {
//       struct list_records *prev = NULL;
//       struct list_records *curr_list = curr_ptr->friends;

//       while (curr_list != NULL) {
//         if (cmp_record(curr_list->record, A) == 0) {
//           if (prev == NULL) {
//             curr_ptr->friends = curr_list->next;
//           } else {
//             prev->next = curr_list->next;
//           }

   
//             struct record* temp = curr_friend->record;
//             if(tail_ptr!=NULL)
//             {
//             tail_ptr -> next = curr_friend -> next;
//             }
//             else
//             {
//               current_record->friends = curr_friend->next;
//             }
//             free_memory(temp);
//           }
//           else
//           {
//             tail_ptr = curr_friend;
//           }
//           curr_friend = curr_friend->next;
//         }
//       }
//       current_record = current_record->right;
//     }
       
//     {
//       if(make_friends_bst(current_record->uid,actual_value->uid) == 1)
//       {
//         struct list_records* tail_ptr = NULL;
//         struct list_records* curr_friend = current_record->friends;
//         while(curr_friend)
//         {
//           if(cmp_record(actual_value,curr_friend->record) == 0)
//           {
         
//             struct record* temp = curr_friend->record;
//             if(tail_ptr!=NULL)
//             {
//             tail_ptr -> next = curr_friend -> next;
//             }
//             else
//             {
//               current_record->friends = curr_friend->next;
//             }
//             free_memory(temp);
//           }
//                     else
//           {
//             tail_ptr = curr_friend;
//           }
//           curr_friend = curr_friend->next;
//         }
//       }
//     }

//     }
//     struct list_records* ptr = actual_value->friends;
//     while(ptr)
//     {
//       struct list_records* destroy_node = ptr;
//       ptr->next;
//       free_memory(ptr);
//     }
//     bst_root = delete(uid,bst_root);
//     return *bst_root;
//   }
// }

// Make users with uids uid1 and uid2 in the BST rooted at bst_root
// friends of each other if they aren't already friends.
// The friends field in "struct record" stores the
// head of the linked list of friends of a given user.
// To make the user with record A a friend of the user with record B,
// add A to B's list of friends and add B to A's list of friends.
// Return 1 if uid1 and uid2 are already friends before this call.
// Return 0 if they become friends during this call.
int make_friends_bst(char uid1[MAX_LEN], char uid2[MAX_LEN])
{
  // Find the records for uid1 and uid2
  struct record *record1 = corresponding_record_of(uid1);
  struct record *record2 = corresponding_record_of(uid2);

  // Check if they are already friends
  struct list_records *friend_ptr = record1->friends;
  while (friend_ptr != NULL) {
    if (friend_ptr->record == record2) {
      // They are already friends, so return 1
      return 1;
    }
    friend_ptr = friend_ptr->next;
  }

  // They are not already friends, so make them friends
  struct list_records *new_friend1 = (struct list_records*) allocate_memory(sizeof(struct list_records));
  struct list_records *new_friend2 = (struct list_records*) allocate_memory(sizeof(struct list_records));
  //HAMDLING THE EDGECASE IF THE MEMORY IS NOT BEIMG ALLOCATED FROM THE HEAP
  if (new_friend1 == NULL) {
    free_memory(new_friend2);
  }
  if (new_friend2 == NULL) {
    free_memory(new_friend1);
  }

  //ADD THE FRIEND AT THE HEAD AND CHANGE THE FRIENDS POINTER TO HEAD
  new_friend1->record = record2;
  new_friend1->next = record1->friends;
  record1->friends = new_friend1;

  new_friend2->record = record1;
  new_friend2->next = record2->friends;
  record2->friends = new_friend2;

  // Return 0 to indicate that they are now friends
  return 0;
}




// struct record * delete_bst(struct record * root, char uid[MAX_LEN]){
//   if (root == NULL) {
//     return NULL;
//   }

//   if (cmp_uid(uid, root->uid) == -1) {
//     root->left = delete_bst(root->left, uid);
//   }
//   else if (cmp_uid(uid, root->uid) == 1) {
//     root->right = delete_bst(root->right, uid);
//   }
//   else {
//     if (root->left == NULL && root->right == NULL) {
//       //free(root);
//       root = NULL;
//     }
//     else if (root->left == NULL) {
//       struct record * temp = root;
//       root = root->right;
//       //free(temp);
//     }
//     else if (root->right == NULL) {
//       struct record * temp = root;
//       root = root->left;
//       //free(temp);
//     }
//     else {
//       struct record * temp = find_min(root->right);
//       temp->left = root->left;
//       temp->right = root->right;
//       root->right = delete_bst(root->right, temp->uid);
//     }
//   }
//   return root;
// }

struct record * delete_bst(struct record * root, char uid[MAX_LEN]){
  // Base case: If the root is null, return null
  if(root==NULL){
    return NULL;
  }

  // If the given uid is greater than the root's uid, delete it from the right subtree
  if(cmp_uid(uid,root->uid)==1){
    root->right=delete_bst(root->right,uid);
  }
  // If the given uid is less than the root's uid, delete it from the left subtree
  else if(cmp_uid(uid,root->uid)==-1){
    root->left=delete_bst(root->left,uid);
  }
  // If the given uid matches the root's uid, delete the root node
  else{
    // If the root has no left child, replace the root with its right child
    if(root->left==NULL){
      struct record * temp=root->right;
      return temp;
    }
    // If the root has no right child, replace the root with its left child
    if(root->right==NULL){
      struct record * temp=root->left;
      return temp;
    }
    // If the root has both left and right children, replace it with its in-order successor
    else
    {
      // Find the node with the minimum value in the right subtree (i.e., the in-order successor)
      struct record * successor=find_min(root->right);
      // Delete the in-order successor from its original location
      root = delete_bst(root,successor -> uid);

      // Replace the root node with the in-order successor
      successor->left=root->left;
      successor->right=root->right;
      return successor;
    }
  }
  return root;
}

// struct list_records* delete_record_from_list(struct list_records* head, char uid[MAX_LEN]) {
//     if (head == NULL) {
//         return NULL;
//     }
//     if (cmp_uid(head->record->uid, uid) == 0) {
//         struct list_records* temp = head->next;
//         free_memory(head);
//         return temp;
//     } else {
//         head->next = delete_record_from_list(head->next, uid);
//     }
//     return head;
// }
// void helper(struct list_records* current, char uid[MAX_LEN]) {
//   if (current == NULL) {
//     return;
//   }
//   if (current->next != NULL && cmp_uid(current->next->record->uid, uid) == 0) {
//     struct list_records* temp = current->next;
//     current->next = current->next->next;
//     free_memory(temp);
//     return;
//   }
//   helper(current->next, uid);
// }

// void delete_from_existing_friends(struct record* r1, char uid[MAX_LEN]) {
//   if (r1->friends == NULL) {
//     return;
//   }
//   if (cmp_uid(r1->friends->record->uid, uid) == 0) {
//     struct list_records* temp = r1->friends;
//     r1->friends = r1->friends->next;
//     free_memory(temp);
//     return;
//   }
//   helper(r1->friends, uid);
// }



// struct record delete_record_bst(char uid[MAX_LEN]) {
//   // Find the corresponding record of the given UID
//   struct record *record_to_delete = corresponding_record_of(uid);

//   // If the record doesn't exist, return a dummy record with status -1
//   if (record_to_delete == NULL) {
//     struct record dummy_record;
//     dummy_record.status = -1;
//     return dummy_record;
//   }

//   // Delete the record from the BST
//   bst_root = delete_bst(bst_root, record_to_delete->uid);

//   // Remove the record from the friends list of all its friends
//   while (record_to_delete->friends != NULL) {
//     struct record* friend_record = record_to_delete->friends->record;
//     delete_from_existing_friends(friend_record, record_to_delete->uid);
//     struct list_records* old_friend_node = record_to_delete->friends;
//     record_to_delete->friends = record_to_delete->friends->next;
//     free_memory(old_friend_node);
//   }

//   // Save the deleted record in a variable to return it later
//   struct record deleted_record = *record_to_delete;

//   // Free the memory used by the record
//   free_memory(record_to_delete);

//   // Decrement the number of nodes in the BST
//   num_bst_nodes--;

//   // Return the deleted record
//   return deleted_record;
// }


void helper(struct list_records* current, char uid[MAX_LEN]) {
  if (current == NULL) {
    return;
  }
  if (current->next != NULL && cmp_uid(current->next->record->uid, uid) == 0) {
    struct list_records* temp = current->next;
    current->next = current->next->next;
    free_memory(temp);
    return;
  }
  helper(current->next, uid);
}


void delete_from_existing_friends(struct record* r1, char uid[MAX_LEN]) {
  if (r1->friends == NULL) {
    return;
  }
  if (cmp_uid(r1->friends->record->uid, uid) == 0) {
    struct list_records* temp = r1->friends;
    r1->friends = r1->friends->next;
    free_memory(temp);
    return;
  }
  helper(r1->friends, uid);
}



struct record delete_record_bst(char uid[MAX_LEN]) {
  // Find the corresponding record of the given UID
  struct record *record_to_delete = corresponding_record_of(uid);

  // If the record doesn't exist, return a dummy record with status -1
  if (record_to_delete == NULL) {
    struct record dummy_record;
    dummy_record.status = -1;
    return dummy_record;
  }

  // Delete the record from the BST
  bst_root = delete_bst(bst_root, record_to_delete->uid);

  // Remove the record from the friends list of all its friends
  while (record_to_delete->friends != NULL) {
    struct record* friend_record = record_to_delete->friends->record;
    delete_from_existing_friends(friend_record, record_to_delete->uid);
    struct list_records* old_friend_node = record_to_delete->friends;
    record_to_delete->friends = record_to_delete->friends->next;
    free_memory(old_friend_node);
  }

  // Save the deleted record in a variable to return it later
  struct record deleted_record = *record_to_delete;

  // Free the memory used by the record
  free_memory(record_to_delete);

  // Decrement the number of nodes in the BST
  num_bst_nodes--;

  // Return the deleted record
  return deleted_record;
}





// Search the record corresponding to uid in the AVL tree
// rooted at avl_root.
// If the record is not present, return a dummy record
// with -1 in the status field.
struct record search_record_avl(char uid[MAX_LEN])
{
    struct record *ptr = avl_root;

    while (ptr != NULL)
    {
        int cmp = cmp_uid(uid, ptr->uid);
        if (cmp == -1)
        {
            ptr = ptr->left;
        }
        else if (cmp == 1)
        {
            ptr = ptr->right;
        }
        else // record found.
        {
            struct record result = *ptr;
            return result;
        }
    }

    struct record dummy = { .status = -1 };
    return dummy;
}

struct record *corresponding_record_of_avl(char uid[MAX_LEN])
{
  if (avl_root == NULL) {
    return NULL;
  }

  struct record *temp = avl_root;

  while (temp != NULL)
  {
    int cmp = cmp_uid(temp->uid, uid);
    if (cmp == 0)
    {
      return temp;
    }
    else if (cmp == 1)
    {
      temp = temp->left;
    }
    else if (cmp == -1)
    {
      temp = temp->right;
    }
  }
  return NULL;
}


// The friends field in "struct record" stores the
// head of the linked list of friends of a given user.
// Return the head of the linked list of friends
// of the user with uid uid in the AVL tree rooted at avl_root.
// If the corresponding record doesn't exist, return NULL.
struct list_records *get_friends_list_avl(char uid[MAX_LEN])
{
  struct record *A = corresponding_record_of_avl(uid);
  struct list_records *A_list = A->friends;
  return A_list;
}

void strcpys(char arr[], char str[])
{
  for(int i = 0; i < MAX_LEN; i++)
  {
    arr[i] = str[i];
  }
}

struct record * delete_avl(struct record * root, char uid[MAX_LEN]){
  if(root==NULL){
    return NULL;
  }

  if(cmp_uid(root->uid,uid) == 1)
  {
    root->left = delete_avl(root->left,uid);
  }
  else if(cmp_uid(root->uid,uid) == -1)
  {
    root->right = delete_avl(root->right,uid);
  }

  else
  {
    if(root->left == NULL)
    {
      struct record* ret = root->right;
      root->right=NULL;
      if (ret!=NULL){
        set_height(ret);
        ret=try_rotate(ret);
      }
      // free_memory(root);
      return ret;
    }
    
    else if(root->right == NULL)
    {
      struct record* ret = root->left;
      root->left=NULL;
      // free_memory(root);
      if (ret!=NULL){
        set_height(ret);
        ret=try_rotate(ret);
      }
      return ret;
    }

    else if(root->right != NULL && root -> left != NULL)
    {
      struct record *min_node = find_min(root->right);
      root->right = delete_avl(root->right, min_node->uid);
      // strcpys(root->uid,min_node -> uid);
      // strcpys(root->name, min_node->name);


      // root->height = min_node -> height;
      // root->friends = min_node->friends;
      // root->status = min_node->status;
      min_node->left=root->left;
      min_node->right=root->right;
      set_height(min_node);
      struct record* n = try_rotate(min_node);
      return n;
    }
  }
  set_height(root);
  struct record* n = try_rotate(root);
  return n;
}


void delete_from_existing_friends_avl(struct record** root, struct record* actual_record)
{
    if (*root == NULL) {
        return;
    }
    // Delete the record from the friends list of the root node
    struct list_records* ptr = (*root)->friends;
    struct list_records* prev_ptr = NULL;
    while (ptr != NULL) {
        if (cmp_record(ptr->record, actual_record) == 0) {
            // Found the actual_record in the friends list, delete it
            if (prev_ptr == NULL) {
                (*root)->friends = ptr->next;
            } else {
                prev_ptr->next = ptr->next;
            }
            free_memory(ptr);
            break;
        }
        prev_ptr = ptr;
        ptr = ptr->next;
    }
    // Traverse the left subtree
    delete_from_existing_friends_avl(&((*root)->left), actual_record);
    // Traverse the right subtree
    delete_from_existing_friends_avl(&((*root)->right), actual_record);
}


// Delete the record (say n) corresponding to uid from the AVL tree.
// Remove n from the lists of friends of other records
// and release the memory for the linked list nodes.
// Release memory for all the nodes in the list of friends of n.
// Return a copy of the value of the deleted record.
// If the record is not present, return a dummy record
// with -1 in the status field.

struct record delete_record_avl(char uid[MAX_LEN])
{
  // struct record* record = corresponding_record_of(uid);

    // Check if AVL tree is empty
    if (avl_root == NULL) {
       // printf("AVL tree is empty\n");
        struct record dummy;
        dummy.status = -1;
        return dummy;
    }

    // Check if record exists in AVL tree
    struct record* record = corresponding_record_of_avl(uid);
   // printf("After corresponding_record_of_avl\n");

    if (record == NULL) {
        //printf("Record not found in AVL tree\n");
        struct record dummy;
        dummy.status = -1;
        return dummy;
    }

    avl_root = delete_avl(avl_root,record->uid);
    while (record->friends != NULL) {
    struct record* friend_record = record->friends->record;
    delete_from_existing_friends(friend_record, record->uid);
    struct list_records* old_friend_node = record->friends;
    record->friends = record->friends->next;
    free_memory(old_friend_node);
  }
    set_height(avl_root);
    avl_root=try_rotate(avl_root);
  
  struct record copy = *record;
  free_memory(record);
  num_avl_nodes--;
  return copy;

}
 
// struct record delete_record_avl(char uid[MAX_LEN]) {
//    // printf("Before corresponding_record_of_avl\n");
    
//     // Check if AVL tree is empty
//     if (avl_root == NULL) {
//        // printf("AVL tree is empty\n");
//         struct record dummy;
//         dummy.status = -1;
//         return dummy;
//     }

//     // Check if record exists in AVL tree
//     struct record* req_rec = corresponding_record_of_avl(uid);
//    // printf("After corresponding_record_of_avl\n");

//     if (req_rec == NULL) {
//         //printf("Record not found in AVL tree\n");
//         struct record dummy;
//         dummy.status = -1;
//         return dummy;
//     }

//     struct record return_record;

//     // Save a copy of the value of the deleted record
//     return_record = search_record_avl(uid);



//     // Remove n from the lists of friends of other records
//     delete_from_existing_friends(avl_root, req_rec->uid);



//     // Release memory for all the nodes in the list of friends of n
//     struct list_records* curr = req_rec->friends;
//     struct list_records* temp;

//     while (curr != NULL) {
//         temp = curr;
//         curr = curr->next;
//         free_memory(temp);
//     }


//     // Remove the record from the AVL tree and release the memory for the node
//     avl_root = delete_avl(avl_root, uid);


//     while (record_to_delete->friends != NULL) {
//     struct record* friend_record = record_to_delete->friends->record;
//     delete_from_existing_friends(friend_record, record_to_delete->uid);
//     struct list_records* old_friend_node = record_to_delete->friends;
//     record_to_delete->friends = record_to_delete->friends->next;
//     free_memory(old_friend_node);
//   }

//     set_height(avl_root);
//     avl_root=try_rotate(avl_root);

//     //free_memory(req_rec);

//     // Update the number of nodes in the AVL tree
//     num_avl_nodes--;
//     free_memory(req_rec);

//     return return_record;
// }

void burn_tree(struct record *root)
{
  if (root == NULL)
  {
    return;
  }
  else
  {
    burn_tree(root->left);
    burn_tree(root->right);

    struct list_records *temp = root->friends;

    while (temp)
    {
      struct list_records *free_node = temp;
      temp = temp->next;
      free_memory(free_node);
    }
    free_memory(root);
    num_bst_nodes--;
  }
}

// Release memory for all BST nodes and their lists of friends.
// Make bst_root points to an empty tree.
void destroy_bst()
{
  burn_tree(bst_root);
  return;
}

void burn_tree_avl(struct record *root)
{
  if (root == NULL)
  {
    return;
  }
  else
  {
    burn_tree_avl(root->left);
    burn_tree_avl(root->right);

    struct list_records *temp = root->friends;

    while (temp)
    {
      struct list_records *free_node = temp;
      temp = temp->next;
      free_memory(free_node);
    }
    free_memory(root);
    num_avl_nodes--;
  }
}
// Release memory for all AVL nodes and their lists of friends.
// Make avl_root points to an empty tree.
void destroy_avl()
{
  burn_tree_avl(avl_root);
  return;
}


// Make users with uids uid1 and uid2 in the AVL tree rooted at avl_root
// friends of each other if they aren't already friends.
// The friends field in "struct record" stores the
// head of the linked list of friends of a given user.
// To make the user with record A a friend of the user with record B,
// add A to B's list of friends and add B to A's list of friends.
// Return 1 if uid1 and uid2 are already friends before this call.
// Return 0 if they become friends during this call.
int make_friends_avl(char uid1[MAX_LEN], char uid2[MAX_LEN])
{
    // Find the records for uid1 and uid2
  struct record *record1 = corresponding_record_of_avl(uid1);
  struct record *record2 = corresponding_record_of_avl(uid2);

  // Check if they are already friends
  struct list_records *friend_ptr = record1->friends;
  while (friend_ptr != NULL) {
    if (friend_ptr->record == record2) {
      // They are already friends, so return 1
      return 1;
    }
    friend_ptr = friend_ptr->next;
  }

  // They are not already friends, so make them friends
  struct list_records *new_friend1 = (struct list_records*) allocate_memory(sizeof(struct list_records));
  struct list_records *new_friend2 = (struct list_records*) allocate_memory(sizeof(struct list_records));

  if (new_friend1 == NULL) {
    free_memory(new_friend2);
  }
  if (new_friend2 == NULL) {
    free_memory(new_friend1);
  }

  new_friend1->record = record2;
  new_friend1->next = record1->friends;
  record1->friends = new_friend1;

  new_friend2->record = record1;
  new_friend2->next = record2->friends;
  record2->friends = new_friend2;

  // Return 0 to indicate that they are now friends
  return 0;
}
