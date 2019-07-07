#include<OpenGL/gl.h>
#include <math.h>
#include <GLUT/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define max(x, y) (((x) > (y)) ? (x) : (y))

int sb;

/* Data Structure for each node in the tree */
struct node
{
    int key;
    struct node *left;
    struct node *right;
    int height;
};

// Global Variables - Start
struct node* root = NULL;           /* Initialize Root Node to NULL */
struct node* current=NULL;          /* Initialize Current Node for highlighting(GreenColor)*/

int result=0,page=0;                       /* Store the current number to be inputted*/

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

// Global Variables - End

// Function Prototypes
void display(void);
void drawFirstPage(void);
void printCredits(void);
void printAbout(void);
void color();

/* A utility function to get height of the tree*/
int height(struct node *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}

/* Function that allocates a new node with the given key. */
struct node* newNode(int key)
{
    struct node* node = (struct node*)
    malloc(sizeof(struct node));
    node->key   = key;
    node->left   = NULL;
    node->right  = NULL;
    node->height = 1;  /* New node is initially added at leaf*/
    current=node;
    return(node);
}

/* A utility function to right rotate subtree rooted with y*/
struct node *rightRotate(struct node *y)
{
    struct node *x = y->left;
    struct node *T2 = x->right;
    
    // Perform rotation
    x->right = y;
    y->left = T2;
    
    // Update heights
    y->height = max(height(y->left), height(y->right))+1;
    x->height = max(height(x->left), height(x->right))+1;
    
    // Return new root
    return x;
}

/* A utility function to left rotate subtree rooted with x */
struct node *leftRotate(struct node *x)
{
    struct node *y = x->right;
    struct node *T2 = y->left;
    
    // Perform rotation
    y->left = x;
    x->right = T2;
    
    //  Update heights
    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;
    
    // Return new root
    return y;
}

/* Function to Get Balance factor of node N */
int getBalance(struct node *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
    
}

/*void sleepx(unsigned int mseconds)
 {
 clock_t goal = mseconds + clock();
 while (goal > clock());
 }*/

/* Function to Insert a node into the tree and perform balancing*/
struct node* insert(struct node* node, int key)
{
    /* 1.  Perform the normal BST rotation */
    if (node == NULL)
        return(newNode(key));
    
    sleep(4);
    
    if (key < node->key)
        node->left  = insert(node->left, key);
    else
        node->right = insert(node->right, key);
    
    display();      /* Display Tree after normal BST Insertion */
    
    sleep(1);
    
    /* 2. Update height of this ancestor node */
    node->height = max(height(node->left), height(node->right)) + 1;
    
    
    /* 3. Get the balance factor of this ancestor node to check whether
     this node became unbalanced */
    int bal = getBalance(node);

    if(bal>1)
        printf("\n it is not balanced");
    
    else
        printf("\n it is balanced");
    
    /* Left Left Rotation case */
    if (bal > 1 && key <= node->left->key)
        return rightRotate(node);

    
    
    /* Right Right Rotation case */
    if (bal < -1 && key >= node->right->key)
        return leftRotate(node);
    
    /* Left Right Rotation case */
    if (bal > 1 && key >= node->left->key)
    {
        node->left =  leftRotate(node->left);
        display();
        sleep(1);
        return rightRotate(node);
    }
    
    /* Right Left Rotation case */
    if (bal < -1 && key <= node->right->key)
    {
        node->right = rightRotate(node->right);
        
        display();
        sleep(1);
        return leftRotate(node);
    }
    
    /* return the (unchanged) node pointer */
    return node;
}

/* Given a non-empty binary search tree, return the
 node with minimum key value found in that tree.
 Note that the entire tree does not need to be
 searched. */
struct node* minValueNode(struct node* node)
{
    struct node* current = node;
    
    /* loop down to find the leftmost leaf */
    while (current->left != NULL)
        current = current->left;
    
    return current;
}


// Recursive function to delete a node with given key
// from subtree with given root. It returns root of
// the modified subtree.
struct node* deleteNode(struct node* root, int key)
{
    // STEP 1: PERFORM STANDARD BST DELETE
    
    if (root == NULL)
        return root;
    
    // If the key to be deleted is smaller than the
    // root's key, then it lies in left subtree
    if ( key < root->key )
        root->left = deleteNode(root->left, key);
    
    // If the key to be deleted is greater than the
    // root's key, then it lies in right subtree
    else if( key > root->key )
        root->right = deleteNode(root->right, key);
    
    // if key is same as root's key, then This is
    // the node to be deleted
    else
    {
        // node with only one child or no child
        if( (root->left == NULL) || (root->right == NULL) )
        {
            struct node *temp = root->left ? root->left :
            root->right;
            
            // No child case
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else // One child case
                *root = *temp; // Copy the contents of
            // the non-empty child
            free(temp);
        }
        else
        {
            // node with two children: Get the inorder
            // successor (smallest in the right subtree)
            struct node* temp = minValueNode(root->right);
            
            // Copy the inorder successor's data to this node
            root->key = temp->key;
            
            // Delete the inorder successor
            root->right = deleteNode(root->right, temp->key);
        }
    }
    
    // If the tree had only one node then return
    if (root == NULL)
        return root;
    
    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
    root->height = 1 + max(height(root->left),
                           height(root->right));
    
    // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to
    // check whether this node became unbalanced)
    int balance = getBalance(root);
    
    // If this node becomes unbalanced, then there are 4 cases
    
    // Left Left Case
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);
    
    // Left Right Case
    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left =  leftRotate(root->left);
        return rightRotate(root);
    }
    
    // Right Right Case
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);
    
    // Right Left Case
    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    
    return root;
}

/* Function to draw a line to connect nodes */
void draw_line(float x1,float y1,float x2, float y2)
{
    glBegin(GL_LINES);
    glVertex2f(x1,y1);
    glVertex2f(x2,y2);
    glEnd();
}
/* Function to display the text */
void draw_text(char* text,float x, float y)
{
    int i;
    glRasterPos3f(x-0.5,y-0.5,1.5);
    for (i = 0; text[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
}

/* A recursive function to draw a binary tree */
void drawNode(struct node* t_root,float x1,float y1,int level)
{
    char buff[5];
    
    if (t_root==NULL)
        return;
    
    float radius = 1.5;
    float left_angle = 245;
    float right_angle = 115;
    float branch_length = 12 - level*2.5;
    float angle_change = 15;
    
    /* Draw the current node */
    if(t_root==current)
        glColor3f(0.0,1.0,0.0);           /* If the node to be drawn is the currently inputted node
                                        Set color of node to green until its placed in its position */
    else
        glColor3f(1.0,0.0,0.0);     /* else set color of node to red */
    
    glPushMatrix();
    glTranslated(x1,y1,0);              /* Draw the node*/
    glutSolidSphere(radius,50,50);
    glPopMatrix();
    
    sprintf(buff,"%d",t_root->key); //atoi
    glColor3f(1.0,1.0,1.0);
    draw_text(buff,x1,y1);
    /* Display value of the node*/
    
    if(t_root->left)    /*Checks if parent has a left child node to be drawn*/
    {
        /* Draw the left child node */
        float angle = left_angle - level*angle_change;
        double radian = angle*3.14/180;
        //float m = (double)tan((double)radian);
        float x2 = x1 + branch_length * sin((double) radian);
        float y2 = y1 + branch_length * cos((double) radian);
        drawNode(t_root->left,x2,y2,level+1);
        glColor3f(0.0,0.0,0.0);
        draw_line(x1,y1,x2,y2);
    }
    if(t_root->right) /*Checks if parent has a Right child node to be drawn*/
    {
        /* Draw the Right child node */
        float angle = right_angle + level*angle_change;
        float radian = angle*3.14/180;
        //float m = (double)tan((double)radian);
        float x2 = x1 + branch_length * sin((double) radian);
        float y2 = y1 + branch_length * cos((double) radian);
        drawNode(t_root->right,x2,y2,level+1);
        
        glColor3f(0.0,0.0,0.0);
        draw_line(x1,y1,x2,y2);
    }
    
}

void display()
{
    if(page==0)
    {
        glClearColor (1,1,1,0);
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        glTranslatef(0,10,-30);
        glColor3f(0.0,0.0,0.0);
        
        
        drawFirstPage();
        glutSwapBuffers();
    }
    if(page==1)
    {
        glClearColor (1.5,1.0,1.0,1.0);
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        glTranslatef(0,10,-30);
        glColor3f(1,1,1);
        
        drawNode(root,0,0,0);
        
        glutSwapBuffers();
    }
    if(page==2)
    {
        glClearColor (1,1,1,1.0);
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        glTranslatef(0,10,-30);
        glColor3f(1,1,1);
        
        printAbout();
        glutSwapBuffers();
    }
    
    if(page==3)
    {
        glClearColor (1,1,1,1.0);
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        glTranslatef(0,10,-30);
        glColor3f(1,1,1);
        
        printCredits();
        glutSwapBuffers();
    }
    
}
void printAbout()
{
    int i;
    char buffer[200]="What is an AVL tree?. ";
    glColor3f(0.0,0.0,0.0);
    glRasterPos3f(-11,-0.5,1.5);
    for (i = 0; buffer[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, buffer[i]);
    
    strcpy(buffer,"1. AVL tree is a self-balancing binary search tree.");
    glRasterPos3f(-18,-3.5,1.5);
    for (i = 0; buffer[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,buffer[i]);
    
    
    strcpy(buffer,"2. The heights of the two child subtrees of any node differ by at most one.");
    glRasterPos3f(-18,-5.5,1.5);
    for (i = 0; buffer[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,buffer[i]);
    
    strcpy(buffer,"3. If at any time they differ by more than one,rebalancing is done to the tree. ");
    glRasterPos3f(-18,-7.5,1.5);
    for (i = 0; buffer[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,buffer[i]);
    
    strcpy(buffer,"4. Lookup,insertion & deletion all take O(log n)time in both the average & worst cases, where ’n’ is the no. of nodes in the tree prior to operation.");
    glRasterPos3f(-18,-9.5,1.5);
    for (i = 0; buffer[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,buffer[i]);
    
    strcpy(buffer,"5. Insertions and deletions may require the tree to be rebalanced by one or more tree rotations.There are 4 roations in AVL Trees");
    glRasterPos3f(-18,-11.5,1.5);
    for (i = 0; buffer[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,buffer[i]);
    
    strcpy(buffer,"6. There are 4 roations in AVL Trees, they are as follows:-");
    glRasterPos3f(-18,-13.5,1.5);
    for (i = 0; buffer[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,buffer[i]);
    
    strcpy(buffer,"i) Left Rotation (Double left Rotation) ");
    glRasterPos3f(-17,-15.5,1.5);
    for (i = 0; buffer[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,buffer[i]);
    
    strcpy(buffer,"ii) Right Rotation (Double Right Rotation)");
    glRasterPos3f(-17,-17.5,1.5);
    for (i = 0; buffer[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,buffer[i]);
    
    strcpy(buffer,"iii) Right-Left Rotations (RL Rotations)");
    glRasterPos3f(-17,-19.5,1.5);
    for (i = 0; buffer[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,buffer[i]);
    
    strcpy(buffer,"iv) Left-Right Rotations (LR Rotations)");
    glRasterPos3f(-17,-21.5,1.5);
    for (i = 0; buffer[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,buffer[i]);
}
   void color(id)
    {
    switch (id)
    {
        case 2:printf("\nleft rotation is done when there is unabalance in tree i.e., if height is more than 1,left rotation is done on left side in order to balance the tree where the height is between -1 to 1\n\n");
               break;
        case 3:printf("right rotation is done when there is unabalance in tree i.e., if height is more than 1,right rotation is done on left side in order to balance the tree where the height is between -1 to 1\n\n");
               break;
        case 4:printf(" left right rotation is a rotation that must be performed when attempting to balance a tree which has a right subtree, that is left heavy.\n\n");
            break;
        case 5:printf(" right left rotation is a rotation that must be performed when attempting to balance a tree which has a left subtree, that is right heavy. this is a mirror operation of left right totation.\n\n");
            break;
    }
    glutPostRedisplay();
    }


void printCredits()
{
    int i;
    char buffer[200]="JSS ACADEMY OF TECHNICAL EDUCATION";
    glClearColor(0,0,0,1);
    glColor3f(0.1,0.0,0.0);
    
    glRasterPos3f(-10,-0.5,1.0);
    for (i = 0; buffer[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, buffer[i]);
    
    strcpy(buffer,"DEPARTMENT OF COMPUTER SCIENCE AND ENGINEERING");
    glRasterPos3f(-10,-2.5,1.5);
    for (i = 0; buffer[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,buffer[i]);
    
    
    strcpy(buffer,"COMPUTER GRAPHICS MINI PROJECT");
    glRasterPos3f(-7,-4.5,2.0);
    for (i = 0; buffer[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,buffer[i]);

    
    strcpy(buffer,"'**************************************************************************************'");
    glRasterPos3f(-11,-6.5,2.5);
    for (i = 0; buffer[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,buffer[i]);
    strcpy(buffer,"AVL TRESS");
    glRasterPos3f(-3,-7.0,2.2);
    for (i = 0; buffer[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,buffer[i]);
    strcpy(buffer,"'**************************************************************************************'");
    glRasterPos3f(-11,-8.0,2.5);
    for (i = 0; buffer[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,buffer[i]);
    
    strcpy(buffer,"SUBMITTED BY,");
    glRasterPos3f(-4,-9,1.5);
    for (i = 0; buffer[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,buffer[i]);
    
    strcpy(buffer,"SHWETA M DEVANAGAONKAR              1JS15CS098");
    glRasterPos3f(-10,-11,1.5);
    for (i = 0; buffer[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,buffer[i]);
    strcpy(buffer,"VINEETA V GARUR                                   1JS15CS121");
    glRasterPos3f(-10,-13,1.5);
    for (i = 0; buffer[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,buffer[i]);
    strcpy(buffer,"UNDER THE GUIDANCE OF,");
    glRasterPos3f(-5,-15,1.5);
    for (i = 0; buffer[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,buffer[i]);
    strcpy(buffer,"Mr. MAHESH KUMAR M R");
    glRasterPos3f(-10,-17,1.5);
    for (i = 0; buffer[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,buffer[i]);
    strcpy(buffer,"MS. SHANTHALA K V");
    glRasterPos3f(2,-17,1.5);
    for (i = 0; buffer[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,buffer[i]);
    
    
    
}
void drawFirstPage()
{
    int i;
    char buffer[50]="AVL TREE CONSTRUCTION ";
    glClearColor(1,0,0,0);
    //glColor3f(1.0,1.0,1.0);
    
    glRasterPos3f(-8,-1.5,1.5);
    for (i = 0; buffer[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, buffer[i]);
    
    strcpy(buffer,"1. START");
    glRasterPos3f(-5,-6.5,1.5);
    for (i = 0; buffer[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, buffer[i]);
    
    strcpy(buffer,"2. ABOUT");
    glRasterPos3f(-5,-9.5,1.5);
    for (i = 0; buffer[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, buffer[i]);
    
    strcpy(buffer,"3. CREDITS");
    glRasterPos3f(-5,-12.5,1.5);
    for (i = 0; buffer[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, buffer[i]);
    
    strcpy(buffer,"4. EXIT");
    glRasterPos3f(-5,-15.5,1.5);
    for (i = 0; buffer[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, buffer[i]);
}
void tm(int id)
{
    switch(id)
    {
        case 1: exit(0);
            break;
        default:color(id);
            break;
    }
}

    
    
    void reshape (int w, int h)
{
    glViewport (0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective (60, (GLfloat)w / (GLfloat)h, 0.1, 100.0);
    glMatrixMode (GL_MODELVIEW);
}

void keyboard(unsigned char key,int mousex,int mousey)
{
    if(page==0)
    {
        switch(key)
        {
            case '1':
                page=1;
                display();
                break;
                
            case '2':
                page=2;
                display();
                break;
                
            case '3':
                page=3;
                display();
                break;
                
            case '4':
                exit(0);
        }
    }
    else if(page==1)
    {
        switch(key)
        {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                result=result*10+(key-'0');
                break;
            case 'z':
                root=insert(root,result);
                sleep(0.5);
                display();
                result=0;
                break;
            case 'd':
                root=deleteNode(root,result);
                sleep(1);
                display();
                result=0;
                break;
            case 'x':
                exit(0);
        }
    }
    else if(page==2)
    {
        page=0;
        display();
    }
    else if(page==3)
    {
        page=0;
        display();
    }
}

int main (int argc, char **argv)
{
    
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize (640, 480);
    glutInitWindowPosition (0, 0);
    glutCreateWindow ("AVL tree : A self balancing B-Tree");
    glutDisplayFunc (display);
    glutReshapeFunc (reshape);
    glutKeyboardFunc (keyboard);
    sb=glutCreateMenu(color);
    glutAddMenuEntry("left rotate",2);
    glutAddMenuEntry("right rotate",3);
    glutAddMenuEntry("left-right rotate",4);
    glutAddMenuEntry("right-left rotate",5);
    glutCreateMenu(tm);
    glutAddMenuEntry("quit",1);
    glutAddSubMenu("rotations",sb);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glEnable(GL_DEPTH_TEST);
    
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    glutMainLoop ();
    
    
    return 0;
}

