#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/** Binary Search Tree Node **/

typedef struct node
{
    int val;            // value stored in the node
    struct node *left;  // left child of the node
    struct node *right; // right child of the node
    int S;              // size of the subtree of the node (total number of elements in the subtree)
} TNd;

/** Functions used in the process **/

TNd *CreateNode(int);
int SubtreeSize(TNd *);
int checkNearlyBalanced(TNd *);
void Traversal(TNd *);
TNd *MakePerfectlyBalanced(int, int);
TNd *Insert(TNd *, TNd *);

/** Global Variables **/

TNd **Arr;      // Array which stores all the nodes of the subtree which needs to be balanced
double *Time_i; // Array storing time taken for ith insertion
int c;          // used for keeping track of the current index in the array

/** Creating a tree node **/

TNd *CreateNode(int x)
{
    TNd *temp = calloc(1, sizeof(TNd));

    // Initially subtree size is 0 and right and left child are NULL
    temp->val = x;
    return temp;
}

/** Function to calculate subtree size of a particular node and including that node **/

int SubtreeSize(TNd *v)
{
    if (v == NULL)
        return 0;
    else
        return (v->S + 1);
}

/** Function to check if a subtree rooted at node T is nearly balanced or not **/

int checkNearlyBalanced(TNd *T)
{
    int R = SubtreeSize(T->right); // size of right subtree
    int L = SubtreeSize(T->left);  // size of left subtree
    int size = L + R;

    /*  Condition for Nearly Balanced BST + checking that the size of atleast
        one of the subtrees is greater than 3 to avoid unnecessary balancings */
    if (((R > (3 * size) / 4) || (L > (3 * size) / 4)) && (R > 3 || L > 3))
        return 0;
    else
        return 1;
}

/** Function for Inorder traversal of a BST rooted at node T **/

void Traversal(TNd *T)
{
    if (T == NULL)
        return;

    else
    {
        if (T->left != NULL)
            Traversal(T->left);

        Arr[c++] = T; // Storing the nodes in the array. To be used in balancing the tree

        if (T->right != NULL)
            Traversal(T->right);
    }
}

/** Balancing the Binary Search Tree which has ceased to be nearly balanced.
    In perfectly balanced BST the difference between sizes of right subtree and left subtree is atmost 1 **/

TNd *MakePerfectlyBalanced(int s, int e)
{
    // Base case: The previous node was a leaf node
    if (s > e)
        return NULL;

    else
    {
        TNd *T;
        int mid = (s + e) / 2;
        T = Arr[mid];
        T->S = e - s;
        T->left = MakePerfectlyBalanced(s, mid - 1);
        T->right = MakePerfectlyBalanced(mid + 1, e);
        return T;
    }
}

/** Function used to insert nodes
    T is the current root node and v is the node to be inserted **/

TNd *Insert(TNd *T, TNd *v)
{
    TNd *p = T, *root = T, *prev = NULL;
    int check = 0, sz;

    while (p != NULL)
    {
        check++;

        if (!checkNearlyBalanced(p))
        {
            /*  If the tree ceases to be nearly balanced at node p, traverse
                through the subtree of p and store all the values in ascending order
                in the array Arr and then build a perfectly balanced BST from that array */
            c = 0;
            sz = SubtreeSize(p);
            Arr = malloc((sz) * sizeof(TNd *));

            Traversal(p);

            // returns the new root node for the subtree that has been rebuild
            p = MakePerfectlyBalanced(0, sz - 1);

            free(Arr);

            /*  If the rebuilding has been done at the root node, the root node will
                be changed later, otherwise we have to change the child of the parent of p */
            if (check != 1)
            {
                if (prev->val < p->val)
                    prev->right = p;
                else
                    prev->left = p;
            }
        }

        // Always assigning the root node to be returned to variable root
        if (check == 1)
            root = p;

        // now v will be inserted in the subtree of p so increasing the size of subtree of p
        p->S++;
        prev = p;

        // Now travering further in the tree
        /*  For our specific case, the value of v will always be greater than any
            other value inserted yet so we only need to go to the right subtree always */
        if (p->val < v->val)
            p = p->right;
        else
            p = p->left;
    }

    if (prev->val < v->val)
        prev->right = v;
    else
        prev->left = v;

    return root;
}

int main()
{
    clock_t start_t, end_t;
    double total_t, sum = 0.0;
    int i;

    Time_i = malloc(1e7 * sizeof(double));

    TNd *root = NULL;
    TNd *ins = NULL;

    for (i = 1; i <= 1e7; i++)
    {
        start_t = clock();

        ins = CreateNode(i);

        if (root == NULL)
        {
            root = ins;
        }
        else
        {
            root = Insert(root, ins);
        }

        end_t = clock();

        total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
        Time_i[i - 1] = total_t;
    }

    /*  Opening 2 files to print the required data:
        A) Total time for n insertions where n is a multiple of 50000
        B) Time taken for ith insertion where 1E5 <= i <= 1E6 */

    FILE *fpt1 = fopen("File1.csv", "w+");
    FILE *fpt2 = fopen("File2.csv", "w+");

    for (i = 1; i <= 1e7; i++)
    {
        sum = sum + Time_i[i - 1];

        if (i % 50000 == 0)
        {
            int log_i = 0;
            int tmp = i;
            while (tmp != 0)
            {
                tmp /= 2;
                log_i++;
            }
            fprintf(fpt1, "%d, %lf\n", i * log_i, sum);
        }

        if (i >= 1e5 && i <= 1e6)
            fprintf(fpt2, "%d, %lf\n", i, Time_i[i - 1]);
    }

    fclose(fpt1);
    fclose(fpt2);

    /* Freeing all the nodes and pointers used */

    free(Time_i);

    c = 0;
    Arr = malloc(1e7 * sizeof(TNd *));
    Traversal(root);

    for (i = 0; i < 1e7; i++)
        free(Arr[i]);

    free(Arr);

    return 0;
}