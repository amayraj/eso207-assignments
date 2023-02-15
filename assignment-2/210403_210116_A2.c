#include <stdio.h>
#include <stdlib.h>

// Data structure (linked list) to store non-zero data, its row & column index,
// and pointer to the next non-zero row & column NODE
typedef struct node
{
    int data, R, C;
    struct node *next_col;
    struct node *next_row;
} NODE;

// Data structure to optimize O(m+n) space to O(m). This optimized-List (optm_rc_list) links
// all non-null pointing Row & Column Heads (& tails) and helps in getting rid of array of size 'n'.
// Contains NODE pointer to non-null row / column, 'rc' contains index to non-null row / column
typedef struct optnode
{
    NODE *row_col;
    int rc;
    struct optnode *next;
} optm_rc_list;

// Prototypes to functions.
NODE *createNode(int, int, int);
void link_rc_head_tail(NODE **, NODE **, NODE *, int, int);
void link_optm_rc_list(optm_rc_list **, NODE *, int, int);
void free_nodes(NODE *);
void free_optm_rc_list(optm_rc_list *);

int main()
{
    int n, a; // 'a' is for iterating 'n' whenever needed
    scanf("%d", &n);

    // NODE **Head_Tail[12];
    // for (a = 0; a < n; a++)
    //     Head_Tail[a] = calloc(n, sizeof(NODE *));

    // Repitition can be easily avoided by taking a NODE** array as above but that
    // will cause double pointer de-referencing, making debugging difficult: Head_Tail[12][n]->abc

    // Creating 'n' Row Head,Tail and 'n' Column Head,Tail pointers
    // for 3 matrices: two input matrices and 1 output matrix (total = 12 pointer to pointers)
    // This is O(m+n) space but finally will be O(m) space and rest will be freed
    NODE **RHead1 = calloc(n, sizeof(NODE *));
    NODE **RTail1 = calloc(n, sizeof(NODE *));
    NODE **CHead1 = calloc(n, sizeof(NODE *));
    NODE **CTail1 = calloc(n, sizeof(NODE *));
    NODE **RHead2 = calloc(n, sizeof(NODE *));
    NODE **RTail2 = calloc(n, sizeof(NODE *));
    NODE **CHead2 = calloc(n, sizeof(NODE *));
    NODE **CTail2 = calloc(n, sizeof(NODE *));
    NODE **RHead3 = calloc(n, sizeof(NODE *));
    NODE **RTail3 = calloc(n, sizeof(NODE *));
    NODE **CHead3 = calloc(n, sizeof(NODE *));
    NODE **CTail3 = calloc(n, sizeof(NODE *));

    // INPUT variables: "m i j val", flag1 & flag2 to denote null matrices
    int m, i, j, val, flag1 = 1, flag2 = 0;
    NODE *ptr;
    scanf("%d", &m);
    while (m == 1)
    {
        scanf("%d %d %d", &i, &j, &val);
        ptr = createNode(val, i, j);
        // Created node is linked to Row Head and then Column Head Linked-Lists
        link_rc_head_tail(RHead1, RTail1, ptr, i, 1);  // 1 to flag as row
        link_rc_head_tail(CHead1, CTail1, ptr, j, -1); // -1 to flag as column
        scanf("%d", &m);
    }
    // If m = 0 is scanned before m = 1 or 2, atleast 1 Input NULL Matrix exists.
    if (m == 0)
        flag1 = 0;
    else
    {
        while (m == 2)
        {
            scanf("%d %d %d", &i, &j, &val);
            ptr = createNode(val, i, j);
            // Created node is linked to Row Head and then Column Head Linked-Lists
            link_rc_head_tail(RHead2, RTail2, ptr, i, 1);  // 1 to flag as row
            link_rc_head_tail(CHead2, CTail2, ptr, j, -1); // -1 to flag as column
            scanf("%d", &m);
        }
    }

    // Reducing redundancy of creating Row_HeadM1, Row_TailM1, Col_HeadM1... and so on.
    // Index 0-3 (for matrix 1): Row_HeadM1, Row_TailM1, Col_HeadM1, Col_TailM1.
    // Index 4-7 (for matrix 2).
    // Index 8-11 (for 3rd ans. matrix).
    // This is to optimize O(m+n) input space to O(m). Refer optm_rc_list struct declaration.
    optm_rc_list **otpmList = calloc(12, sizeof(optm_rc_list *));

    // Iterating through ONLY NECESSARY Row & Column Heads
    // of both matrices that are NOT pointing to NULL Rows & Columns
    for (a = 0; a < n; a++)
    {
        if (RHead1[a] != NULL)
            link_optm_rc_list(otpmList, RHead1[a], a, 0);
        if (CHead1[a] != NULL)
            link_optm_rc_list(otpmList, CHead1[a], a, 2);
        if (RHead2[a] != NULL)
            link_optm_rc_list(otpmList, RHead2[a], a, 4);
        if (CHead2[a] != NULL)
            link_optm_rc_list(otpmList, CHead2[a], a, 6);
    }
    // Freeing all the input headers (in both matrices) of 'n' row_heads, 'n' col_heads,
    // 'n' row_tails & 'n' col_tails and thus optimizing O(m+n) to O(m) space.
    free(RHead1);
    free(RTail1);
    free(CHead1);
    free(CTail1);
    free(RHead2);
    free(RTail2);
    free(CHead2);
    free(CTail2);

    /***  MATRIX MULTIPLICATION  ***/

    NODE *r, *c;     // row & col node iterators
    int k, l;        // To flag row & col heads index
    int ans, i1, j1; // 'ans' temporarily stores multiplied data, i1 & j1 row & col index iterators

    optm_rc_list *r1 = otpmList[0]; // row_Head1 (of matrix 1).

    // Loops till all row_Heads of matrix 1 are traversed while multiplication
    while (r1 != NULL)
    {
        optm_rc_list *c2 = otpmList[6]; // col_Head2 (of matrix 2)

        // Loops till all col_Heads of matrix 2 are traversed while multiplication
        while (c2 != NULL)
        {
            r = r1->row_col; // 1st NODE of row_Head1
            c = c2->row_col; // 1st NODE of col_Head2
            k = r1->rc;      // row_index of row_Head1
            l = c2->rc;      // col_index of col_Head2
            ans = 0;         // resetted to 0 for every new row_Head1 & col_Head2

            // Loops till all NODES in ROW & COLUMN are traversed while multiplication
            while (r != NULL && c != NULL)
            {
                i1 = r->C; // Col index of 1st matrix NODE in row_Head1
                j1 = c->R; // Row index of 2nd matrix NODE in col_Head2

                // If indices are equal, data is multiplied and added to 'ans'
                if (i1 == j1)
                {
                    ans += (r->data) * (c->data);
                    r = r->next_col;
                    c = c->next_row;
                }
                // Else if Col index (of matrix 1) is higher, move to next NODE (next_row) in that column (in matrix 2)
                else if (i1 > j1)
                {
                    c = c->next_row;
                }
                // Else if Row index (of matrix 2) is higher, move to next NODE (next_col) in that row (in matrix 1)
                else
                {
                    r = r->next_col;
                }
            }
            // If ans. is not 0, ans. is added to 3rd SPARSE MATRIX as it is better
            // practice to store ans. and then print all together, than printing ans.
            // right-away as we receive it. So that it can be used later on whenever needed
            if (ans != 0)
            {
                // Matrix multiplication can also yield NULL Matrix. Flag2 is to signify ans. matrix is not null
                flag2 = 1;

                ptr = createNode(ans, k + 1, l + 1);
                // Created node is linked to Row Head and then Column Head Linked-Lists of 3rd answer matrix
                link_rc_head_tail(RHead3, RTail3, ptr, k + 1, 1);
                link_rc_head_tail(CHead3, CTail3, ptr, l + 1, -1);
            }
            // Iterate to next col_Head2 (of matrix 2) and multiply with CURRENT row of matrix 1
            c2 = c2->next;
        }
        // Iterate to next row_Head1 (of matrix 1) and multiply with ALL columns of matrix 2
        r1 = r1->next;
    }

    /***  PRINTING ANSWER  ***/

    // If matrix 1 or 2 is NULL || ans. matrix 3 is NULL
    if (flag1 == 0 || flag2 == 0)
        printf("NULL MATRIX!");

    // Else Optimize O(m+n) space to O(m) of ans. matrix and then print it
    else
    {
        // Iterating through ONLY NECESSARY Row & Column Heads
        // of ans. matrix 3 that are NOT pointing to NULL Rows & Columns
        for (a = 0; a < n; a++)
        {
            if (RHead3[a] != NULL)
                link_optm_rc_list(otpmList, RHead3[a], a, 8);
            if (CHead3[a] != NULL)
                link_optm_rc_list(otpmList, CHead3[a], a, 10);
        }
        // Freeing all the 3rd Matrix headers of 'n' row_heads, 'n' col_heads,
        // 'n' row_tails & 'n' col_tails and thus optimizing O(m+n) to O(m) space.
        free(RHead3);
        free(RTail3);
        free(CHead3);
        free(CTail3);

        // curr1: Optmized row_Heads of matrix 3
        optm_rc_list *curr1 = otpmList[8];
        NODE *curr2;
        // curr1 traverses through all row_Heads till the end
        while (curr1 != NULL)
        {
            // curr2: Current NODE of each row_Head of matrix 3
            curr2 = curr1->row_col;
            // curr2 traverses through each NODE in a row_Head printing NODE till the end of row
            while (curr2 != NULL)
            {
                printf("%d %d %d\n", curr2->R, curr2->C, curr2->data);
                curr2 = curr2->next_col; // Iterates to next NODE in a row
            }
            curr1 = curr1->next; // Iterates to next row_Head in optimized list
        }
    }

    /***  FREEING EVERY DYNAMICALLY ALLOCATED MEMORY  ***/

    // Freeing all the NODES in the 3 matrices.
    // Accessing via row_heads of optimized list is sufficient to clear all nodes.
    // Row Heads are at optmList[0], optmList[4] and optmList[8].
    for (a = 0; a <= 8; a = a + 4) // a = a + 4 to access: 0, 4, 8
    {
        optm_rc_list *curr_row_head = otpmList[a];
        while (curr_row_head != NULL)
        {
            free_nodes(curr_row_head->row_col);  // Frees all NODES in current row
            curr_row_head = curr_row_head->next; // Iterates to next row_Head in optimized list
        }
    }

    // Now all the 6 optmized Lists need to be freed: row_head & col_head of all 3 matrices.
    // a = a + 2 to access optmized list via Heads only. Heads are at optmList[0], optmList[3]...
    for (a = 0; a < 12; a = a + 2)
        free_optm_rc_list(otpmList[a]);

    // Now double pointer optmList finally needs to be freed as it was an array of 12 optmList pointers.
    free(otpmList);
    return 0;
}

// Function creates node with value, row & column index and return pointer to it
NODE *createNode(int val, int i, int j)
{
    NODE *temp = calloc(1, sizeof(NODE));
    temp->data = val;
    temp->R = i;
    temp->C = j;
    return temp;
}

// Function links new node to row and column heads. ARGUMENT takes:
// Head pointer to Row / Column pointers, Tail pointer to Row / Column pointers,
// Pointer to temporary node created, i / j pointer index of Head / Tail pointers,
// 'rc_flag' to denote if its for Row or Column (1 / -1)
void link_rc_head_tail(NODE **Head, NODE **Tail, NODE *ptr, int ij, int rc_flag)
{
    // If no previous node existed, Row / Column Head (at ij index) points to the new node
    if (Head[ij - 1] == NULL)
        Head[ij - 1] = ptr;

    // Else Head is left untouched, node is linked at the end and Tail is shifted
    else
    {
        // checks if pointer points to Row Tail or Column Tail pointers.
        // New node (ptr) is linked at the end of...
        if (rc_flag == 1)
            Tail[ij - 1]->next_col = ptr; // ...PREVIOUS ROW TAIL's NEXT COLUMN
        else
            Tail[ij - 1]->next_row = ptr; // ...PREVIOUS COLUMN TAIL's NEXT ROW
    }
    // Row / Column Tail (at ij index) now points to the end of created node (ptr)
    Tail[ij - 1] = ptr;
}

// Function links only the non-NULL ROW & COLUMN Heads (passed as 'rc_head')
// to the new list to optimize for O(m) space.
// 'a' denotes index of row / column head. 'idx' is index of optimised-List head
void link_optm_rc_list(optm_rc_list **optmList, NODE *rc_head, int a, int idx)
{
    // creating a optm_rc_list node for non-null row / column head
    optm_rc_list *temp = calloc(1, sizeof(optm_rc_list));
    temp->row_col = rc_head;
    temp->rc = a;
    // If new head (= optmList[idx]) is NULL, link node it to the head
    if (optmList[idx] == NULL)
        optmList[idx] = temp;
    // Else new tail is shifted
    else
    {
        optmList[idx + 1]->next = temp;
    }
    optmList[idx + 1] = temp;
}

// Function to free all NODES in a matrix
void free_nodes(NODE *row_head)
{
    if (row_head == NULL)
        return;
    // Recursively free the next NODE then the current NODE
    free_nodes(row_head->next_col);
    free(row_head);
}

// Function to free optimized row & column head lists
void free_optm_rc_list(optm_rc_list *head)
{
    if (head == NULL)
        return;
    // Recursively free the next optm_list_node then the current optm_list_node
    free_optm_rc_list(head->next);
    free(head);
}