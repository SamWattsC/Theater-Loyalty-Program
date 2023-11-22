/* COP 3502C Assignment 5
This program is written by: Samuel Watts */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAXLEN 19

typedef struct customer {
    char name[MAXLEN+1];
    int points;
} customer;

typedef struct treenode {
    customer* cPtr;
    int size;
    struct treenode* left;
    struct treenode* right;
} treenode;

void swap1(customer *x, customer *y);
void updateSize (treenode * root, char * name);

//creates the customer node
customer * createCustomer(char * name, int points){
    customer * temp = malloc(sizeof(customer));
    strcpy(temp->name, name);
    temp->points = points;
    
    return temp;
}
//creates the tree node
treenode * createNode (customer * cust){
    treenode * temp = malloc(sizeof(treenode));
    temp->cPtr = cust;
    temp->right = NULL;
    temp->left = NULL;
    temp->size = 1;
    
    return temp;
}

int determineValueByName(char * name1, char * name2);

//moves through the tree and returns the name we are looking for
treenode * searchName (treenode * tree, char * name) {
    // Check if there are nodes in the tree.
    if (tree == NULL || determineValueByName(tree->cPtr->name, name) == 0){
        return tree;
    }
    // Search to the left
    if (determineValueByName(tree->cPtr->name, name) > 0)
        return searchName(tree->left, name);
    // Search to the right
    else
        return searchName(tree->right, name);
    
    return tree;
}
//moves through the tree to find the height
int searchHeight (treenode * root, char * name) {
    int count = 0;
    
    while(root != NULL){
        if (determineValueByName(name, root->cPtr->name) == 0) {
            break;
        }
        //if the name is further left
        else if (determineValueByName(name, root->cPtr->name) < 0) {
            count += 1;
            root = root->left;
        }
        //if the name is further right
        else {
            count++;
            root = root->right;
        }
    }
    return count;
}

int countSmaller(treenode * root, char * name) {
    //if the node is null we don't need to add it
    if(root == NULL){
        return 0;
    }
    //if the name is less than the node name, move to the left
    if (determineValueByName(name, root->cPtr->name) < 0) {
        return countSmaller(root->left, name);
    }
    //if the name is the same return the size of the left node
    else if(determineValueByName(name, root->cPtr->name) == 0){
        if (root->left != NULL){
            return root->left->size;
        }
        return 0;
    }
     
    else {
        if(root->left != NULL){
            return 1 + root->left->size + countSmaller(root->right, name);
        }
        else{
            return 1 + countSmaller(root->right, name);
        }
    }
}

//Needs to account for the suffling of people
treenode * findNameToDelete(treenode * tree, char * name){
    // Check if there are nodes in the tree.
    if (tree == NULL || determineValueByName(tree->cPtr->name, name) == 0){
        return tree;
    }
    //Node will be deleted under the current node, so -- reflects the size
    tree->size --;
    // Search to the left
    if (determineValueByName(tree->cPtr->name, name) > 0)
        return findNameToDelete(tree->left, name);
    // Search to the right
    else
        return findNameToDelete(tree->right, name);
    
    return tree;
}

int parentChildRelationship(treenode * parent, treenode * child){
    if (parent->left == child){
        return 1;
    }
    else if (parent->right == child){
        return 2;
    }
    else{
        return 0;
    }
}
//function for determining leaf
int isLeaf(treenode * root){
    if(root->left == NULL && root->right == NULL){
        return 1;
    }
    else{
        return 0;
    }
}
//function for determining if their is one left child
int oneChildLeft(treenode * root){
    if(root->left != NULL && root->right == NULL){
        return 1;
    }
    else{
        return 0;
    }
}
//function for determining if their is one right child
int oneChildRight(treenode * root){
    if(root->left == NULL && root->right != NULL){
        return 1;
    }
    else{
        return 0;
    }
}
//function for determining if there are 2 children
int twoChildren(treenode * root){
    if(root->left != NULL && root->right != NULL){
        return 1;
    }
    else{
        return 0;
    }
}

//pointer to the first name in the tree
treenode* firstName(treenode * root) {

  // Root stores the minimal value.
  if (root->right == NULL)
    return root;

  // The left subtree of the root stores the minimal value.
  else{
      root->size--;
      return firstName(root->right);
  }
}

treenode * findParentNode (treenode * tree, treenode * node){
    if (tree == NULL || tree == node){
        return NULL;
    }
    if (tree->left == node || tree->right == node)
        return tree;
    // Look for node's parent in the left side of the tree.
    if (determineValueByName(tree->cPtr->name, node->cPtr->name) > 0)
        return findParentNode(tree->left, node);
    // Look for node's parent in the right side of the tree.
    else if (determineValueByName(tree->cPtr->name, node->cPtr->name) < 0)
        return findParentNode(tree->right, node);
    
    return NULL;
}


treenode * addToTree(treenode * root, treenode * newNode){
    //if the tree is blank return the new customer, they are the only one in the tree
    if (root == NULL){
        return newNode;
    }
    //add to the size of the nodes as you move through them
    root->size ++;
    //if the customer name is "greater" than the node name move into the right of that node
    if (determineValueByName(root->cPtr->name, newNode->cPtr->name) < 0){
        root->right = addToTree(root->right, newNode);
    }
    //otherwise move to the left of the node
    else if(determineValueByName(root->cPtr->name, newNode->cPtr->name) > 0){
        root->left = addToTree(root->left, newNode);{
        }
    }
    return root;
}

void subPoints(treenode * tree, char * custName, int points){
    treenode * temp = searchName(tree, custName);
    
    if (temp != NULL){
        //if more points are subtracted than the customer has, make their points zero
        if (temp->cPtr->points < points){
            temp->cPtr->points = 0;
        }
        //otherwise remove the points normally
        else{
            temp->cPtr->points -= points;
        }
        printf("%s %d\n",temp->cPtr->name, temp->cPtr->points);
    }
    //if they don't exist
    else {
        printf("%s not found\n", custName);
    }
}

//recursive delete function
treenode * deleteNode(treenode * root, customer * cust, int freePermission, treenode * parentOfDelete){
    
    treenode * delnode = NULL;
    treenode * replacementNode;
    treenode * saveNode;
    customer * saveCust;

    /**
     If freePersmission is 0 that means this delete function is in the first iteration, and frees memory differently from the second iteration
     */
    
    //do if we are going through delete the first time
    if (freePermission == 0){
       // delnode = findNameToDelete(root, cust->name);
        delnode = searchName(root, cust->name);
        parentOfDelete = findParentNode(root, delnode);
        //subtracts points as it finds the name
        updateSize(root, delnode->cPtr->name);
    }
    //do if we are going through delete for a replacement node
    else{
        delnode = searchName(root, cust->name);
        parentOfDelete = findParentNode(root, delnode);
    }
    
    //if the node is a leaf
    if (isLeaf(delnode)) {
        
        if (parentOfDelete == NULL) {
            if (freePermission == 0){
                free(delnode->cPtr);
            }
            free(root);
            return NULL;
        }
        
        // deletes the node if it's a left child.
        if(determineValueByName(parentOfDelete->cPtr->name, delnode->cPtr->name) > 0){
            if (freePermission == 0){
                free(parentOfDelete->left->cPtr);
            }
            free(parentOfDelete->left);
            parentOfDelete->left = NULL;
        }
       
        else {
            //deletes node if it is a right child
            if (freePermission == 0){
                free(parentOfDelete->right->cPtr);
            }
            free(parentOfDelete->right);
            parentOfDelete->right = NULL;
        }
        
        return root;
    }
    
    //if there's one child on the left
    if (oneChildLeft(delnode) == 1) {
        //if the parent doesn't exist
        if (parentOfDelete == NULL) {
            saveNode = delnode->left;
            if (freePermission == 0){
                free(delnode->cPtr);
            }
            free(delnode);
            return saveNode;
        }
        
        //delete the node if it's a left child.
        if (determineValueByName(parentOfDelete->cPtr->name, delnode->cPtr->name) > 0) {
            saveNode = parentOfDelete->left;
            parentOfDelete->left = parentOfDelete->left->left;
            if (freePermission == 0){
                free(saveNode->cPtr);
            }
            free(saveNode);
          
        }
        
        //delete the node if it's a right child.
        else {
            saveNode = parentOfDelete->right;
            parentOfDelete->right = parentOfDelete->right->left;
            if (freePermission == 0){
                free(saveNode->cPtr);
            }
            free(saveNode);
        }
        return root;
    }
    
    //if the deleted node has one right child
    if (oneChildRight(delnode) == 1) {
        //if the parent doesn't exist
        if (parentOfDelete == NULL) {
            saveNode = delnode->right;
            if (freePermission == 0){
                free(delnode->cPtr);
            }
            free(delnode);
            return saveNode;
        }
        //delete node if its a left child
        if (determineValueByName(parentOfDelete->cPtr->name, delnode->cPtr->name) > 0) {
            saveNode = parentOfDelete->left;
            parentOfDelete->left = parentOfDelete->left->right;
            if (freePermission == 0){
                free(saveNode->cPtr);
            }
            free(saveNode);
        }
        
        // delete the node if it is a right child.
        else {
            saveNode = parentOfDelete->right;
            parentOfDelete->right = parentOfDelete->right->right;
            if (freePermission == 0){
                free(saveNode->cPtr);
            }
            free(saveNode);
        }
        return root;
    }
    
    //saves the node info that will replace the deleted customer
    replacementNode = firstName(delnode->left);
    //decrements size of current node that is getting replaced
    delnode->size --;
    //saves the customer info to be placed in the deleted node
    saveCust = replacementNode->cPtr;
    //deletes the node with the customer info
    deleteNode(root, saveCust, freePermission + 1, parentOfDelete);
    //free the customer info of the deletede
    free(delnode->cPtr);
    //assign the saved customer to the node of the previously deleted node
    delnode->cPtr = saveCust;
    
    return root;
}

//compares points first, then by name if they are the same points
int comparePoints(customer * cust1, customer * cust2){
    if(cust1->points > cust2->points){
        return 1;
    }
    else if (cust1->points < cust2->points){
        return 2;
    }
    else{
        if(determineValueByName(cust1->name, cust2->name) < 0){
            return 1;
        }
        else{
            return 2;
        }
    }
}
//frees all the memory for nodes and customers in the tree
void freeTree(treenode * root){
    if(root != NULL){
        freeTree(root->left);
        freeTree(root->right);
        free(root->cPtr);
        free(root);
    }
}



//returns 1 is the first number is greater, 2 if the second number is greater, 0 if they are the same
int determineValueByName(char * name1, char * name2){

    return strcmp(name1, name2);
}

void printCustArray(customer ** cust, int length){
    for (int i = 0; i < length; i++){
        printf("%s %d\n", cust[i]->name, cust[i]->points);
    }
}

//creates the array of customers from the binary search tree
int createCustomerArray(treenode * root, customer ** cust, int tracker){
    if(root == NULL)
         return tracker;
    
    cust[tracker] = root->cPtr;
    //increments the index
    tracker++;
    //recursively calls the function to search every part of the tree
    if(root->left != NULL)
         tracker = createCustomerArray(root->left, cust, tracker);
    if(root->right != NULL)
         tracker = createCustomerArray(root->right, cust, tracker);
    //recursively returns tracker as the index of the array
    return tracker;
}

//Sorting Takaes Place Here
int partition1(customer ** custArr, int low, int high){
    int i, lowpos;
    // pick a random partition element and swap it into index low.
    i = low + rand()%(high-low+1);
    swap1(custArr[low], custArr[i]);
    // store the index of the partition element.
    lowpos = low;
    // make low one more than the lowposition
    low++;
    // go until high and low cross
    while (low <= high) {
        //move low until we find a value to swap
        while (low <= high && comparePoints(custArr[low], custArr[lowpos]) == 1) low++;
        //move high until we find a value to swap
        while (high >= low && comparePoints(custArr[high], custArr[lowpos]) == 2) high--;
        // swap the values on the wrong side
        if (low < high)
            swap1(custArr[low], custArr[high]);
    }
    // swap the partition element into it's correct location
    swap1(custArr[lowpos], custArr[high]);
    return high;
}
 //quicksort function
customer ** quickSort1 (customer ** arr, int low, int high){
    if (low < high){
        int k = partition1(arr, low, high);
        quickSort1(arr, low, k-1);
        quickSort1(arr, k+1, high);
    }
    return arr;
}
 //swaps the two customers
void swap1(customer *x, customer *y){
    customer temp = *x;
    *x = *y;
    *y = temp;
}

void printTree(treenode * root){
    if (root != NULL){
        printTree(root->left);
        printf("%s %d\n", root->cPtr->name, root->size);
        printTree(root->right);
    }
}
void updateSize (treenode * root, char * name){
        if (root == NULL || determineValueByName(root->cPtr->name, name) == 0){
            return;
        }
        //Node will be deleted under the current node, so -- reflects the size
        root->size --;
        // Search to the left
        if (determineValueByName(root->cPtr->name, name) > 0)
            return updateSize(root->left, name);
        // Search to the right
        else
            return updateSize(root->right, name);
        
        return;
}

int main(int argc, const char * argv[]) {
    int numInputs = 0;
    char action[30];
    char name[MAXLEN+1];
    int points = 0;
    char * add = "add";
    char * sub = "sub";
    treenode * root = NULL;
    //counts the number of people being added and deleted from tree to properly fill array based on size
    int trackNumberOfPeople = 0;
    int arrCount = 0;
    int depth = 0;
    int printpermission = 0;
    
    scanf("%d", &numInputs);
    
    for (int i = 0; i < numInputs; i++){
        scanf("%s", action);
        scanf("%s", name);


        if(strcmp(action, add) == 0){
            scanf("%d", &points);
            treenode * temp = searchName(root, name);
            //if the person does not exist, create them and their node
            if (temp == NULL){
                customer * cust = createCustomer(name, points);
                treenode * newNode = createNode(cust);
                root = addToTree(root, newNode);
                printf("%s %d\n", newNode->cPtr->name, newNode->cPtr->points);
                trackNumberOfPeople++;
            }
            //if they already exist add points to their name
            else {
                temp->cPtr->points += points;
                printf("%s %d\n", temp->cPtr->name, temp->cPtr->points);
            }
        }
        //subtract points from the person
        else if(strcmp(action, sub) == 0){
            scanf("%d", &points);
            subPoints(root, name, points);
        }
        
        else if(strcmp(action, "search") == 0){
            treenode * search = searchName(root, name);
            if (search == NULL){
                printf("%s not found\n", name);
            }
            else{
                //find the height of that person
                depth = searchHeight(root, name);
                printf("%s %d %d\n", search->cPtr->name, search->cPtr->points, depth);
            }
        }
        else if(strcmp(action, "del") == 0){
            treenode * person = searchName(root, name);
            if(person != NULL){
                //if the person exists delete them
                printf("%s deleted\n", person->cPtr->name);
                root = deleteNode(root, person->cPtr, 0, NULL);
                trackNumberOfPeople--;
       
            }
            else {
                printf("%s not found\n", name);
            }
        }
        else if (strcmp(action, "count_smaller") == 0){
            //count the number of people that come before the given name
            int small = countSmaller(root, name);
            
                printf("%d\n", small);
        }
    }
    //allocate memory for the customer array
    customer ** custArr = malloc(trackNumberOfPeople * sizeof(customer *));
    //create the customer array
    createCustomerArray(root, custArr, arrCount);
    //sort the array by points, then alphabetically if they have the same points
    quickSort1(custArr, 0, trackNumberOfPeople-1);
    //print the array
    printCustArray(custArr, trackNumberOfPeople);
    //free the tree and customers
    freeTree(root);
    //free the array itself
    free(custArr);
    
    return 0;
}
