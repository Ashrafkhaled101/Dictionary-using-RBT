#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <ctype.h>
#include <conio.h>
#include <time.h>

#define MAX_LENGTH 30
typedef enum Color{
    RED,BLACK
}Color;

typedef struct Node{
        char data[MAX_LENGTH];
        Color color;
        struct Node *left,*right,*parent;
}Node;

Node *root = NULL;

// RED-BLACK-TREE
void insert(char* );
int search(char* );
Node* deleteNode(char* );
Node* findSuccessor(Node *);
Node *minl(Node *);
void fixViolationColor(Node *);
void fixViolation(Node *);
void rotateLeft(Node *);
void rotateRight(Node *);
void traversal(Node *);
void postorderTree(Node *);
void inorderTree(Node *);
int height(Node *);
void printGivenLevel(Node *, int);
void printLevelOrder(Node *);
void swap(Node *,Node *);
int isEmpty();
int size(Node *);

//interface
void banner();
void menu();
void start();

// file-system
int load();
int append(char *);
int isFILE();
int sizeFILE();
void updateFILE(Node *);

int main(){

    start();

    return 0;
}

int search(char* key){
    Node* temp = root;

    while (temp != NULL){

        if (strcmp(key , temp->data) > 0){
            temp = temp->right;

        }else if (strcmp(key , temp->data) < 0){
            temp = temp->left;

        }else{
            return 1;
        }

    }
    return 0;

}


void insert(char *data){

    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // creating new node ..

    Node *parent = NULL;
    Node *temp = NULL;

    Node *x = (Node*)malloc(sizeof(Node));
    x->color = RED;
    strcpy(x->data,data);
    x->left = NULL;
    x->right = NULL;

    temp = root;

    // if the element is already exist
    if(search(data) == 1){
        SetConsoleTextAttribute(hConsole,96);
        printf("WARRING : The Entered word already exists\n");
        SetConsoleTextAttribute(hConsole,15);

        return;
    }

    // Redif the tree is Empty ..
    if(root == NULL){
        root = x;
        root->color = BLACK;
    }

    // traverse throw the tree from the root to find the new node parent
    while ( temp != NULL){

        parent = temp;

        if ( strcmp(x->data , temp->data) < 0){
            temp = temp->left;
        }else {
            temp = temp->right;
        }
    }

    x->parent = parent;


    // insert the new node in it's position
    if ( parent == NULL){
        root = x;

    }else if( strcmp(x->data , parent->data) < 0){
        parent->left = x;

    }else {
        parent->right = x;
    }

    fixViolation(x);

}

void rotateLeft(Node *x){

    Node *x_right = x->right;
    x->right = x_right->left;

    if(x_right->left != NULL){
        x_right->left->parent = x;
    }

    x_right->parent = x->parent;

    if( x->parent == NULL){
        root = x_right;

    }else if( (x->parent->left!=NULL) && (strcmp(x->data , x->parent->left->data)==0)){
        x->parent->left = x_right;

    }else
        x->parent->right = x_right;

    x_right->left = x;
    x->parent = x_right;

}

void rotateRight(Node *x){

    Node *x_left = x->left;
    x->left = x_left->right;

    if ( x_left->right != NULL){
        x_left->right->parent = x;

    }

    x_left->parent = x->parent;

    if( x->parent == NULL){
        root = x_left;

    }else if((x->parent->left!=NULL)&& (strcmp(x->data , x->parent->left->data) == 0)){
        x->parent->left = x_left;

    }else
        x->parent->right = x_left;

    x_left->right = x;
    x->parent = x_left;

}

void fixViolation(Node *x){

    Node *x_uncle = NULL;

    while ((x->parent != NULL) && (x->parent->color == RED)){

        if ( (x->parent->parent->left != NULL) && (strcmp(x->parent->data , x->parent->parent->left->data) == 0)){

            if(x->parent->parent->right != NULL)
                x_uncle = x->parent->parent->right; // Right uncle

            if ((x_uncle != NULL) && (x_uncle->color == RED)){ // RED-UNCLE

                x->parent->color = BLACK;
                x_uncle->color = BLACK;
                x->parent->parent->color = RED;

                if(x->parent->parent != NULL)
                    x = x->parent->parent;

            }else{ // BLACK-UNCLE

                if ((x->parent->right != NULL) && (strcmp(x->data , x->parent->right->data) == 0)){
                    x = x->parent;
                    rotateLeft(x);
                }

                x->parent->color = BLACK;
                x->parent->parent->color = RED;
                rotateRight(x->parent->parent);

            }

        }else{

            if(x->parent->parent->left != NULL)

                x_uncle = x->parent->parent->left;

            if ((x_uncle != NULL) && (x_uncle->color == RED)){

                x->parent->color = BLACK;
                x_uncle->color = BLACK;
                x->parent->parent->color = RED;

                if(x->parent->parent != NULL)

                    x = x->parent->parent;

            }else{

                if ((x->parent->left != NULL) && (strcmp(x->data , x->parent->left->data) == 0)){
                    x = x->parent;
                    rotateRight(x);

                }

                x->parent->color = BLACK;
                x->parent->parent->color = RED;
                rotateLeft(x->parent->parent);

            }

        }

    }
    root->color = BLACK;


}

Node* deleteNode(char* key){
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    Node *x = NULL, *y = NULL, *z;

    z=root; // point to root

    // Tree had a single node (Root) .
    if((z->left == NULL ) &&(z->right == NULL) && (strcmp(z->data ,key) == 0)){

        root = NULL;
        SetConsoleTextAttribute(hConsole,96);
        printf("\nRB-TREE is Empty\n");
        SetConsoleTextAttribute(hConsole,15);

        return z;

    }

    // search for the target node to delete
    while((strcmp(z->data , key) != 0) && z != NULL){

        if(strcmp(key , z->data) < 0 )
            z=z->left;
        else
            z=z->right;

        if(z== NULL){
            SetConsoleTextAttribute(hConsole,64);
            printf("Error : NOT FOUND,There's no such word!!\n");
            SetConsoleTextAttribute(hConsole,15);

            return 0;
        }

    }

    // Find target position from his children
    if((z->left==NULL)||(z->right==NULL)){ // CASE (1) : has only 1 child (Left or right).
        y = z;

    }else{ // CASE (2) has more than 1 child (2 obviously xD). // Expected : either it's a leaf node has (2x Black Null(s)) or an inside node :3
        y = findSuccessor(z); // find the successor node
    }

    /*  Note :-
        Case 2: a has two children, a’s successor (c) is its right child:
        1. Remove a and put its successor (c) instead
        2. Make a’s left child (b) the successor’s (c) left child
    */

    /*
        Case 3: a has two children, a’s successor (d) is not its child:
        1. Put the successor’s (d) left child (e) instead of it
        2. Remove a and put its successor (d) instead of it, making a’s children (b, c) its new children
    */

    if (y->left != NULL){
        x = y->left;

    }else{
        if(y->right !=NULL)
            x = y->right;
    }

    if((x != NULL) && (y->parent != NULL))
        x->parent = y->parent;

    if ((y != NULL) && (x != NULL) && (y->parent == NULL)){
        root=x;

    }else if (y == y->parent->left){
        y->parent->left = x;

    }else{
        y->parent->right = x;

    }

    if (y != z){
        strcpy(z->data , y->data);
    }

    if ((y!=NULL) && (x!=NULL) && (y->color == BLACK)){
        fixViolationColor(x);
    }

    return y;

}

Node* findSuccessor(Node *x){

    Node *temp;
    // The successor always has no left child
    if (x->right){
        return minl(x->right);
    }

    temp = x->parent;

    while (temp && x == temp->right){
        x = temp;
        temp = temp->parent;
    }

    return temp;

}

Node *minl(Node *x){

    while (x->left){
        x = x->left;
    }

    return x;

}

void fixViolationColor(Node *x){

    while (x != root && x->color == BLACK){

        Node *w = NULL; // points to the sibling

        if ((x->parent->left != NULL) && (x == x->parent->left)){

            w = x->parent->right; // sibling

            /*
            CASE 1: a’s sibling (c) is red:
            1. Rotate left around a’s father (b)
            2. Switch colors between a’s father (b) and grandfather (c)
            3. Continue to the next case with the subtree rooted at b
            */

            if ((w!=NULL) && (w->color == RED)){

                w->color = BLACK;
                x->parent->color = RED;
                rotateLeft(x->parent);
                w = x->parent->right;

            }
           /*
            Case 2: a’s sibling (c) and its children (d, e) are black:
            1. Take one black from a and its sibling c and move it up (leaving a with one black and c – red)
            2. The problem is moved up – continue solving it
            */
            if ((w!=NULL) && (w->right!=NULL) && (w->left!=NULL) && (w->left->color == BLACK) && (w->right->color == BLACK)){

                w->color = RED;
                x = x->parent;

            /*
            Case 3: a’s sibling (c) is black, with left child (d) red and right child (e) black:
                1. Rotate right around a’s sibling (c)
                2. Switch colors between a’s new and old siblings (d, c)
                3. Continue to case 4
            */
            }else if((w!=NULL) && (w->right->color == BLACK)){

                w->left->color = BLACK;
                w->color = RED;
                rotateRight(w);
                w = x->parent->right;

            }
            /*
            Case 4: a’s sibling (c) is black, with right child (e) red:
                1. Rotate left around a’s father (b)
                2. Color a’s new grandfather (c) with a’s father’s (b) color
                3. Color a’s father (b) with a’s extra black, making a singly-colored
                4. Color a’s uncle (e) black (originally a’s right “nephew”)
            */
            if(w != NULL){

                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                rotateLeft(x->parent);
                x = root;

            }

        // same cases for the left sibling
        }else if(x->parent != NULL){

            w = x->parent->left;

            if ((w!=NULL) && (w->color == RED)){

                w->color = BLACK;
                x->parent->color = RED;
                rotateLeft(x->parent);
                if(x->parent!=NULL)
                    w = x->parent->left;

            }

            if ((w!=NULL) && (w->right!=NULL) && (w->left!=NULL) && (w->right->color == BLACK) && (w->left->color == BLACK)){
                x = x->parent;

            }else if((w!=NULL) && (w->right!=NULL) && (w->left!=NULL) && (w->left->color == 'b')){

                w->right->color = BLACK;
                w->color = RED;
                rotateRight(w);
                w = x->parent->left;

            }

            if(x->parent!=NULL){
                w->color = x->parent->color;
                x->parent->color = BLACK;

            }

            if(w->left!=NULL)
                w->left->color = BLACK;

            if(x->parent !=NULL)
                rotateLeft(x->parent);

            x = root;

        }

    }
    x->color = BLACK;

}

void traversal(Node* root){
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (root != NULL){
        SetConsoleTextAttribute(hConsole,4);
        printf("Root is [%s--%d]",root->data,root->color);

        SetConsoleTextAttribute(hConsole,10);
        printf("\nInorder tree traversal\n");
        SetConsoleTextAttribute(hConsole,15);
        inorderTree(root);

        SetConsoleTextAttribute(hConsole,10);
        printf("\nPostorder tree traversal\n");
        SetConsoleTextAttribute(hConsole,15);
        postorderTree(root);

        SetConsoleTextAttribute(hConsole,10);
        printf("\nLevelorder tree traversal\n");
        SetConsoleTextAttribute(hConsole,15);
        printLevelOrder(root);
    }
    SetConsoleTextAttribute(hConsole,4);
    printf("\nNumber of words is: %d word(s)\nThe Tree height is %d cm xD\n",size(root),height(root));
    SetConsoleTextAttribute(hConsole,15);


}

void inorderTree(Node* root){

    Node* temp = root;

    if (temp != NULL){
        inorderTree(temp->left);
        printf(" %s--%d ",temp->data,temp->color);
        inorderTree(temp->right);

    }

}

void postorderTree(Node* root){

    Node* temp = root;
    if (temp != NULL){

        postorderTree(temp->left);
        postorderTree(temp->right);
        printf(" %s--%d ",temp->data,temp->color);

    }


}


// Compute the "height" of the Tree
///the number of nodes along the longest path from the root node down to the farthest leaf node.*/
int height(Node* node){

    if (node==NULL)
        return 0;
    else{
        // compute the height of each subtree
        int lheight = height(node->left);
        int rheight = height(node->right);

        // use the larger one
        if (lheight > rheight)
            return(lheight+1);
        else
            return(rheight+1);
        }
}

// print level order traversal a tree
void printLevelOrder(Node* root){

    int h = height(root);
    int i;
    for (i=1; i<=h; i++)
        printGivenLevel(root, i);

}

// Print nodes at a given level
void printGivenLevel(Node* root, int level){
    if (root == NULL)
        return;
    if (level == 1)
        printf("%s ", root->data);
    else if (level > 1){
        printGivenLevel(root->left, level-1);
        printGivenLevel(root->right, level-1);

    }
}

// swapping 2xNodes's Colors

void swap(Node *n1,Node *n2){

    Color tp = BLACK;
    tp = n1->color;
    n1->color = n2->color;
    n2->color = tp;

    printf("\n> %d %d",n1->color,n2->color);

}

int append(char *data){

    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    FILE *f ;
    if((f = fopen("dictionary.txt","a")) == NULL){
        SetConsoleTextAttribute(hConsole,64);
        perror("ERROR");
        SetConsoleTextAttribute(hConsole,15);
        printf("\n");
        return 0;
    }

    if(fprintf(f,"%s\n",data)){
        SetConsoleTextAttribute(hConsole,160);
        printf("word has been appended successfully to the Dictionary\n");
        SetConsoleTextAttribute(hConsole,15);
    }else{

        SetConsoleTextAttribute(hConsole,64);
        perror("ERROR");
        SetConsoleTextAttribute(hConsole,15);
        printf("\n");
        return 0;

    }
    fclose(f);
    return 1;

}

int sizeFILE(){

    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    char word[MAX_LENGTH];
    int size=0;
    FILE *f ;
    if((f = fopen("dictionary.txt","r")) == NULL){
        SetConsoleTextAttribute(hConsole,64);
        perror("ERROR");
        SetConsoleTextAttribute(hConsole,15);
        printf("\n");
        return 0;
    }
    while(fscanf(f,"%s",word) != EOF){
        //printf("%s\n",word);
        size++;
    }
    fclose(f);

    return size;
}

int size(Node *node){

    if (node==NULL)
        return 0;
    else
        return(size(node->left) + 1 + size(node->right));
}

int load(){

    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    char word[MAX_LENGTH];
    FILE *f ;
    if((f = fopen("dictionary.txt","r")) == NULL){
        SetConsoleTextAttribute(hConsole,64);
        perror("ERROR");
        SetConsoleTextAttribute(hConsole,15);
        return 0;
    }
    int i=0;
    char c;
    while((c = fgetc(f)) != EOF){
        word[i++] = c;
        if(c == '\n'){
           word[i] = '\0';
           i=0;
           word[strcspn(word, "\r\n")] = '\0';
            insert(word);
        }
    }
    /*while(fscanf(f,"%s",word) != EOF){
        //printf("%s\n",word);
        insert(word);
    }
*/
    SetConsoleTextAttribute(hConsole,160);
    printf("File has been loaded successfully\n");
    SetConsoleTextAttribute(hConsole,15);
    fclose(f);
    return 1;
}


int isFILE(){

    FILE *f;
    if((f = fopen("dictionary.txt","r")) == NULL){
        return 0;
    }
    return 1;
    fclose(f);

}

void updateFILE(Node* root){

    Node* temp = root;
    if (temp != NULL){
        updateFILE(temp->left);
        FILE* f = fopen("dictionary.txt","a");
        fprintf(f,"%s\n",temp->data);
        updateFILE(temp->right);
    }

}

int isEmpty(){
    if(root == NULL)
        return 1;
    return 0;
}

void start(){
    menu();
}

void banner(){

    HANDLE hConsole;
    int k;
    k = 15;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,k);
    printf("\t\t\t\t\tDictionary Based On ");
    k = 4;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,k);
    printf("RED");

    k = 1;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,k);
    printf("-");

    k = 8;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,k);
    printf("BLACK ");

    k = 10;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,k);
    printf("TREE ");

    k = 6;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,k);
    printf(":)");
    printf("\n");
}

void menu(){

    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    char data[MAX_LENGTH];
    int input;
    while(1){
        banner();
        SetConsoleTextAttribute(hConsole,15);
        printf("\n\t\t\t\t\t\tChoose an Operation: \n\t\t\t\t1:Load Dictionary\t2:Insert new entry \t3:Delete\n\t\t\t\t4:Search\t\t5:Traversal\t\t6:Exit\n");
        scanf("%d",&input);



        switch(input){

        case 1:
            if(load()){
                printf("The Dictionary size is : %d word(s)\nThe Tree height is %d cm xD\n",size(root),height(root));
            }
            //printf("Loading");
            getchar();
            getchar();
            SetConsoleTextAttribute(hConsole,15);
            system("cls");
            break;
        case 2:
            printf("Please enter a new word: ");
            scanf("%s",data);
            if(isFILE() && !isEmpty()){
                append(data);
                insert(data);
                SetConsoleTextAttribute(hConsole,160);
                printf("The Dictionary size is : %d word(s)\nThe Tree height is %d cm xD\n",size(root),height(root));
                SetConsoleTextAttribute(hConsole,15);

            }else{
                SetConsoleTextAttribute(hConsole,96);
                printf("WARNING : There's no such file ,Please make sure to load the file first!\n");
                SetConsoleTextAttribute(hConsole,15);
            }
            //printf("inserting");
            getchar();
            getchar();
            SetConsoleTextAttribute(hConsole,15);
            system("cls");
            break;
        case 3:
            printf("Please enter the word that you want to be deleted: ");
            scanf("%s",data);
            if(isFILE() && !isEmpty()){
                if(deleteNode(data) != 0){
                    SetConsoleTextAttribute(hConsole,160);
                    printf(" [%s ] : Has been deleted successfully!!\n",data);
                    SetConsoleTextAttribute(hConsole,15);
                }

            }else{
                SetConsoleTextAttribute(hConsole,96);
                printf("WARNING : There's no such file ,Please make sure to load the file first!\n");
                SetConsoleTextAttribute(hConsole,15);
            }
            //printf("deleting");
            getchar();
            getchar();
            SetConsoleTextAttribute(hConsole,15);
            system("cls");
            break;
        case 4:
            printf("Please enter the word that you want to be search for: ");
            scanf("%s",data);
            if(isFILE() && !isEmpty()){
                if(search(data)){
                    SetConsoleTextAttribute(hConsole,160);
                    printf("Word Found!!\n");
                    SetConsoleTextAttribute(hConsole,15);
                }else{

                    SetConsoleTextAttribute(hConsole,64);
                    printf("Word NOT Found!!\n");
                    SetConsoleTextAttribute(hConsole,15);
                }
            }else{
                SetConsoleTextAttribute(hConsole,96);
                printf("WARNING : There's no such file ,Please make sure to load the file first!\n");
                SetConsoleTextAttribute(hConsole,15);
            }
            //printf("searching");
            getchar();
            getchar();
            SetConsoleTextAttribute(hConsole,15);
            system("cls");
            break;
        case 5:
            if(isFILE() && !isEmpty()){
                traversal(root);
            }else{
                SetConsoleTextAttribute(hConsole,96);
                printf("WARNING : There's no such file ,Please make sure to load the file first!\n");
                SetConsoleTextAttribute(hConsole,15);
            }
            //printf("traversal..");
            getchar();
            getchar();
            SetConsoleTextAttribute(hConsole,15);
            system("cls");
            break;
        case 6:
            SetConsoleTextAttribute(hConsole,160);
            printf("The program is Exiting successfully ,In 1 second.\n");
            Sleep(2000);
            SetConsoleTextAttribute(hConsole,15);
            system("cls");
            exit(0);
        default:
            SetConsoleTextAttribute(hConsole,64);
            printf("INVALID OPERATION!! .. please try again in 1 second\n");
            Sleep(2000);
            SetConsoleTextAttribute(hConsole,15);
            system("cls");

        }

    }

}


