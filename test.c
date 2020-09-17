#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.c"

char * _strdup(const char * str) {
    char * aux = (char *)malloc(strlen(str) + 1);
    strcpy(aux, str);
    return aux;
}

typedef struct{
   int id;
   char* word;
}Palabra;

Palabra* creaPalabra(int id, char* word){
    Palabra* new = (Palabra*) malloc(sizeof(Palabra));
    new->id = id;
    new->word = _strdup(word);
}

// return 1 if key1<key2
int lower_than_int(void* key1, void* key2){
    int k1 = *((int*) (key1));
    int k2 = *((int*) (key2));
    return k1<k2;
}

void create_test1(){
    TreeMap* t = createTreeMap(lower_than_int);

    if(t->root!=NULL) printf("error: root debe ser NULL");
    int i=10,j=15;
    if(t->lower_than(&i,&j)) printf("%d es menor que %d\n",i,j);
}

void search_test1(TreeMap* tree){ //key se encuentra en raiz
    int key=5239;
    Palabra* pal = searchTreeMap(tree, &key);
    if(pal!=NULL) printf("%d: %s\n",pal->id,pal->word);
    if(tree->current != tree->root) printf("el current deberia quedar apuntando al nodo encontrado\n");
}

void search_test2(TreeMap* tree){ //key en root->right
    int key=8213;
    Palabra* pal = searchTreeMap(tree, &key);
    if(pal!=NULL) printf("%d: %s\n",pal->id,pal->word);
    if(tree->current != tree->root->right) printf("el current deberia quedar apuntando al nodo encontrado\n");
}

void search_test3(TreeMap* tree){ //key en root->right->left
    int key=6980;
    Palabra* pal = searchTreeMap(tree, &key);
    if(pal!=NULL) printf("%d: %s\n",pal->id,pal->word);
    if(tree->current != tree->root->right->left) printf("el current deberia quedar apuntando al nodo encontrado\n");
}

void search_test4(TreeMap* tree){ //key no existe
    int key=7010;
    Palabra* pal = searchTreeMap(tree, &key);
    if(pal==NULL) printf("clave no existe\n");
}

void insert_test1(TreeMap* tree){//dato repetido
    Palabra* p = creaPalabra(1273,_strdup("repetido"));
    insertTreeMap(tree, &p->id, p);

    if(tree->root->left->left != NULL ||
           tree->root->left->right != NULL) 
        printf("no se debe insertar datos repetidos\n");

}

void insert_test2(){
    int numeros[4] = { 5239, 8213, 6980, 1273};
    char words[4][15] = {"etc","even","evenly","ever"};
    
    TreeMap * tree = createTreeMap(lower_than_int);

    int i = 0;

    for (; i<4; i++) {
        Palabra* p=creaPalabra(numeros[i],words[i]);
        insertTreeMap(tree, &p->id, p);
    }

    Palabra* p = creaPalabra(900,_strdup("maicol"));
    insertTreeMap(tree, &p->id, p);

    if(tree->root->left->left == NULL ||
           tree->root->left->left->value!=p) 
        printf("el dato no se encuentra donde debiera\n");

    if(tree->root->left->left != NULL &&
            tree->root->left->left->key!=&p->id) 
        printf("la clave del nodo no corresponde\n");

    if(tree->root->left->left->parent != 
            tree->root->left)
        printf("recuerda inicializar el padre del nuevo nodo\n");
    
    if(tree->current != tree->root->left->left)
        printf("recuerda actualizar el current al nuevo nodo\n");


}

void erase_test1(){//nodo sin hijos
    int numeros[4] = { 5239, 8213, 6980, 1273};
    char words[4][15] = {"etc","even","evenly","ever"};
    
    TreeMap * tree = createTreeMap(lower_than_int);

    int i = 0;

    for (; i<4; i++) {
        Palabra* p=creaPalabra(numeros[i],words[i]);
        insertTreeMap(tree, &p->id, p);
    }
    
    int key=1273;
    eraseTreeMap(tree, &key);
    if(tree->root->left != NULL) 
        printf("el dato no se elimino correctamente (key:%d)\n",key);

}

void erase_test2(){//nodo con un hijo
    int numeros[4] = { 5239, 8213, 6980, 1273};
    char words[4][15] = {"etc","even","evenly","ever"};
    
    TreeMap * tree = createTreeMap(lower_than_int);

    int i = 0;

    for (; i<4; i++) {
        Palabra* p=creaPalabra(numeros[i],words[i]);
        insertTreeMap(tree, &p->id, p);
    }
    
    int key=8213;
    eraseTreeMap(tree, &key);

    if( * ((int*) tree->root->right->key) != 6980) 
        printf("el dato no se elimino correctamente (key:%d)\n",key);

    if(  tree->root->right->parent != tree->root)
        printf("falta actualizar el parent!\n");

}

void erase_test3(){//nodo con dos hijos
    int numeros[4] = { 5239, 8213, 6980, 1273};
    char words[4][15] = {"etc","even","evenly","ever"};
    
    TreeMap * tree = createTreeMap(lower_than_int);

    int i = 0;

    for (; i<4; i++) {
        Palabra* p=creaPalabra(numeros[i],words[i]);
        insertTreeMap(tree, &p->id, p);
    }
    
    int key=5239;
    eraseTreeMap(tree, &key);

    if( * ((int*) tree->root->key) != 6980) 
        printf("el dato no se elimino correctamente\n",key);

}

void first_test1(TreeMap * tree){ 
    Palabra* aux=firstTreeMap(tree);
    printf("%d: %s\n",aux->id, aux->word);
}

void first_test2(TreeMap * tree){ //debes iterar
    Palabra* p=creaPalabra(100,_strdup("first_word"));
    insertTreeMap(tree, &p->id, p);

    Palabra* aux=firstTreeMap(tree);
    printf("%d: %s\n",aux->id, aux->word);

    eraseTreeMap(tree, &p->id);
}

void next_test1(TreeMap * tree){ //caso1: current tiene hijo derecho
    Palabra* p=creaPalabra(2000,"next_word");
    tree->root->left->right=createTreeNode(&p->id, p);
    tree->root->left->right->parent=tree->root->left;

    Palabra* aux=firstTreeMap(tree);
    aux=nextTreeMap(tree);

    printf("%d: %s\n",aux->id, aux->word);
    eraseTreeMap(tree, &p->id);
}

void next_test2(TreeMap * tree){ //caso1: current tiene hijo derecho
    int j=5239;
    Palabra* aux=searchTreeMap(tree, &j);
    aux=nextTreeMap(tree);
    
    printf("%d: %s\n",aux->id, aux->word);
}

void next_test3(TreeMap * tree){ //caso2: current sin hijo derecho
    int j=6980;
    Palabra* aux=searchTreeMap(tree, &j);
    aux=nextTreeMap(tree);
    
    printf("%d: %s\n",aux->id, aux->word);
}

void next_test4(TreeMap * tree){ //caso2: current sin hijo derecho
    Palabra* p=creaPalabra(2000,_strdup("current_word"));
    tree->root->left->right=createTreeNode(&p->id, p);
    tree->root->left->right->parent=tree->root->left;
    tree->current=tree->root->left->right;
    Palabra* aux=nextTreeMap(tree);
    
    printf("%d: %s\n",aux->id, aux->word);
    eraseTreeMap(tree, &p->id);
}

void next_test5(TreeMap * tree){ //último dato
    int j=8213;
    Palabra* aux=searchTreeMap(tree, &j);
    aux=nextTreeMap(tree);

    if(aux!=NULL) printf("error: 8213 no tiene next\n");
}

void ub_test1(TreeMap* tree){ //el dato existe
    int j=6980;
    Palabra* aux=upperBound(tree, &j);

    if(aux!=NULL) printf("%d: %s\n",aux->id, aux->word);
    else printf("(null)\n");
}

void ub_test2(TreeMap* tree){ 
    int j=6979;
    Palabra* aux=upperBound(tree, &j);

    if(aux!=NULL) printf("%d: %s\n",aux->id, aux->word);
    else printf("(null)\n");
}

void ub_test3(TreeMap* tree){
    int j=6981;
    Palabra* aux=upperBound(tree, &j);

    if(aux!=NULL) printf("%d: %s\n",aux->id, aux->word);
    else printf("(null)\n");
}

void ub_test4(TreeMap* tree){ 
    int j=8214;
    Palabra* aux=upperBound(tree, &j);

    if(aux!=NULL) printf("%d: %s\n",aux->id, aux->word);
    else printf("(null)\n");
}

int main() {

    TreeMap * tree = (TreeMap *)malloc(sizeof(TreeMap));
    tree->lower_than = lower_than_int;
    Palabra* p=creaPalabra(5239,"auto");
    tree->root=createTreeNode(&p->id, p);
    p=creaPalabra(8213,"rayo");
    tree->root->right=createTreeNode(&p->id, p);
    tree->root->right->parent=tree->root;
    p=creaPalabra(6980,"hoja");
    tree->root->right->left=createTreeNode(&p->id, p);
    tree->root->right->left->parent=tree->root->right;
    p=creaPalabra(1273,"reto");
    tree->root->left=createTreeNode(&p->id, p);
    tree->root->left->parent=tree->root;

    printf("create_tests\n");
    create_test1(); //10 es menor que 15

    printf("search tests:\n");
    search_test1(tree); //5239: auto
    search_test2(tree); //8213: rayo
    search_test3(tree); //6980: hoja
    search_test4(tree); //clave no existe

    printf("insert tests:\n");
    insert_test1(tree);
    insert_test2(tree);

    printf("erase tests:\n");
    erase_test1();
    erase_test2();
    erase_test3();

    printf("first-next tests:\n"); //requiere insert y search
    first_test1(tree); //1273: reto
    first_test2(tree); //100: first_word
    next_test1(tree); 
    next_test2(tree);
    next_test3(tree);
    next_test4(tree);
    next_test5(tree);

    printf("ub_tests:\n");
    ub_test1(tree);
    ub_test2(tree);
    ub_test3(tree);
    ub_test4(tree);

    printf("fin\n");

    return 0;
}