#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.c"

char * _strdup(const char * str) {
    char * aux = (char *)malloc(strlen(str) + 1);
    strcpy(aux, str);
    return aux;
}

char msg[100];

void err_msg(char* msg){
    printf("[ FAILED ] ");
    printf("%s\n",msg);
    exit(-1);
}

void ok_msg(char* msg){
    printf ("[ OK ] ");
    printf("%s\n",msg);
}

void info_msg(char* msg){
    printf ("[ INFO ] %s\n",msg);
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

TreeMap* initializeTree(){
    info_msg("inicializando el arbol...");
    TreeMap* tree=(TreeMap *)malloc(sizeof(TreeMap));
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
    return tree;
}

void create_test1(){
    TreeMap* t = createTreeMap(lower_than_int);

    if(t==NULL) {
        err_msg("createTreeMap debe retornar el mapa");
    }else 
        ok_msg("createTreeMap retorna un objeto");

    if(t->root!=NULL){
        err_msg("root debe ser NULL");
    } else
        ok_msg("root==NULL");
    

    int i=10,j=15;
    if(t->lower_than(&i,&j) !=1 ) {
        err_msg("la funcion lower_than no fue guardada de forma correcta");
        exit(-1);
    }else
        ok_msg("Funcion de comparcion inicializada correctamente");
}

void search_test1(TreeMap* tree){ //key se encuentra en raiz
    int key=5239;
    Palabra* pal = searchTreeMap(tree, &key);
    if(pal!=NULL && pal->id==5239)
        ok_msg("encuentra dato con clave 5239");
    else{
        err_msg("no encuentra dato con clave 5239");
    }
    
    if(tree->current != tree->root){
        err_msg("no actualiza current (5329)");
    }else
        ok_msg("current actualizado correctamente");

}

void search_test2(TreeMap* tree){ //key en root->right
    int key=8213;
    Palabra* pal = searchTreeMap(tree, &key);
    if(pal!=NULL && pal->id==8213)
        ok_msg("encuentra dato con clave 8213");
    else
        err_msg("no encuentra dato con clave 8213");
    

    if(tree->current != tree->root->right)
        err_msg("no actualiza current correctamente (8213)");
    else
        ok_msg("current actualizado correctamente");
}

void search_test3(TreeMap* tree){ //key en root->right->left
    int key=6980;
    Palabra* pal = searchTreeMap(tree, &key);
    if(pal!=NULL && pal->id==key){
        sprintf(msg, "encuentra dato con clave %d",key);
        ok_msg(msg);
    }else
        err_msg("no encuentra dato con clave 6980");
    

    if(tree->current != tree->root->right->left)
        err_msg("no actualiza current correctamente");
    else
        ok_msg("current actualizado correctamente");
    
}

void search_test4(TreeMap* tree){ //key no existe
    int key=7010;
    Palabra* pal = searchTreeMap(tree, &key);
    
    if(pal==NULL) {
        sprintf(msg, "retorna NULL: search(key=%d)",key);
        ok_msg(msg);
    }else{
        err_msg("retorna dato y clave no existe (7010)\n");
        exit(-1);    
    }
    
}

void insert_test1(TreeMap* tree){//dato repetido
    Palabra* p = creaPalabra(1273,_strdup("repetido"));
    insertTreeMap(tree, &p->id, p);

    if(tree->root->left->left != NULL ||
           tree->root->left->right != NULL){
        err_msg("se inserta dato repetido");
    }
    else
        ok_msg("no inserta dato repetido");

}

void insert_test2(){
    TreeMap * tree = initializeTree();

    Palabra* p = creaPalabra(900,_strdup("maicol"));
    info_msg("insertando dato con clave 900");
    insertTreeMap(tree, &p->id, p);
    

    if(tree->root->left->left == NULL ||
           tree->root->left->left->value!=p) {
        err_msg("dato insertado no se encuentra en root->left->left");
    }else
        ok_msg("dato insertado correctamente");
    
    if(tree->root->left->left != NULL &&
            tree->root->left->left->key!=&p->id) {
                err_msg("clave de dato no se guarda correctamente");
            }

    if(tree->root->left->left->parent != 
            tree->root->left){
                err_msg("no se inicializa el padre del nuevo nodo");
            }
    
    if(tree->current != tree->root->left->left){
        err_msg("no se actualiza el current");
     }
        ok_msg("current actualizado correctamente");


}

void erase_test1(){//nodo sin hijos
    TreeMap * tree = initializeTree();
    
    int key=1273;
    info_msg("eliminando dato con clave 1273 (nodo sin hijos)");
    eraseTreeMap(tree, &key);
    if(tree->root->left != NULL) {
        err_msg("el dato no se elimino correctamente: tree->root->left != NULL");
    }else
        ok_msg("dato eliminado correctamente");

}

void erase_test2(){//nodo con un hijo
    TreeMap * tree = initializeTree();
    
    int key=8213;
    info_msg("eliminando dato con clave 8213 (nodo con un hijo)");
    eraseTreeMap(tree, &key);

    if( * ((int*) tree->root->right->key) != 6980) {
        err_msg("el dato no se elimino correctamente root->right!=6980");
    }
    

    if(  tree->root->right->parent != tree->root){
        err_msg("falta actualizar el parent de nodo 6980");
    }else
        ok_msg("dato eliminado correctamente");
}

void erase_test3(){//nodo con dos hijos
    TreeMap * tree = initializeTree();
    
    int key=5239;
    info_msg("eliminando dato con clave 5239 (nodo con dos hijos)");
    eraseTreeMap(tree, &key);

    if( * ((int*) tree->root->key) != 6980){
        err_msg("el dato no se elimino correctamente root!=6980");
    }

    if( * ((int*) tree->root->right->key) != 8213){
        err_msg("el dato no se elimino correctamente root->right!=8213");
    }else
        ok_msg("dato eliminado correctamente");
    
}

void first_test1(TreeMap * tree){ 
    Palabra* aux=firstTreeMap(tree);
    if(!aux)
        err_msg("first retorna NULL");

    if(aux->id != 1273)
        err_msg("first no retorna nodo 1273");
    else
        ok_msg("first retorna nodo 1273");
}

void first_test2(){ //debes iterar
    TreeMap * tree = initializeTree();
    info_msg("agregando nodo con clave 100");
    Palabra* p=creaPalabra(100,"first_word");
    tree->root->left->left=createTreeNode(&p->id, p);
    tree->root->left->left->parent=tree->root->left;

    Palabra* aux=firstTreeMap(tree);
    if(aux->id != 100)
        err_msg("first no retorna nodo 100");
    else
        ok_msg("first retorna nodo 100");
}

void next_test1(){ //caso1: current tiene hijo derecho
    TreeMap * tree = initializeTree();
    info_msg("agregando nodo con clave 2000");
    Palabra* p=creaPalabra(2000,"next_word");
    tree->root->left->right=createTreeNode(&p->id, p);
    tree->root->left->right->parent=tree->root->left;

    info_msg("actualizando current -> nodo 1273");
    tree->current = tree->root->left;
    Palabra* aux=nextTreeMap(tree);

    if(aux==NULL) err_msg("next retorna NULL");

     if(aux->id != 2000)
        err_msg("next no retorna nodo 2000");
    else
        ok_msg("next retorna nodo 2000");   


}

void next_test2(){ //caso1: current tiene hijo derecho
    TreeMap * tree = initializeTree();
    info_msg("actualizando current -> root");
    tree->current = tree->root;
    Palabra* aux=nextTreeMap(tree);

     if(aux->id != 6980)
        err_msg("next no retorna nodo 6980");
    else
        ok_msg("next retorna nodo 6980");  
    
    aux=nextTreeMap(tree);
    
    if(aux->id != 8213)
        err_msg("next no retorna nodo 8213");
    else
        ok_msg("next retorna nodo 8213");  

    aux=nextTreeMap(tree);
    if(aux)
        err_msg("next != NULL");
    else
        ok_msg("next retorna NULL");  

}

void next_test3(){ //caso2: current sin hijo derecho
    TreeMap * tree = initializeTree();
    info_msg("agregando nodo con clave 2000");
    Palabra* p=creaPalabra(2000,"next_word");
    tree->root->left->right=createTreeNode(&p->id, p);
    tree->root->left->right->parent=tree->root->left;
    info_msg("actualizando current -> nodo 2000");
    tree->current=tree->root->left->right;

    Palabra* aux=nextTreeMap(tree);
    
    if(aux->id != 5239)
        err_msg("next no retorna nodo 5239\n");
    else
        ok_msg("next retorna nodo 5239");
}

void ub_test1(TreeMap* tree){ //el dato existe
    int j=6980;
    Palabra* aux=upperBound(tree, &j);

    if(aux==NULL) {
        err_msg("upperbound de 6980 retorna NULL");
    }

    if(aux->id != 6980){
        sprintf(msg,"upperbound de 6980 retorna %d",aux->id);
        err_msg(msg);
    }else{
        sprintf(msg,"upperbound de %d retorna %d",j,aux->id);
        ok_msg(msg);
    }

}

void ub_test2(TreeMap* tree){ 
    int j=6979;
    Palabra* aux=upperBound(tree, &j);

    if(aux==NULL) {
        sprintf(msg,"upperbound de %d retorna NULL",j);
        err_msg(msg);
    }

    if(aux->id != 6980){
        sprintf(msg,"upperbound de %d retorna %d",j,aux->id);
        err_msg(msg);
    }else{
        sprintf(msg,"upperbound de %d retorna %d",j,aux->id);
        ok_msg(msg);
    }
}

void ub_test3(TreeMap* tree){
    int j=6981;
    Palabra* aux=upperBound(tree, &j);

    if(aux==NULL) {
        sprintf(msg,"upperbound de %d retorna NULL",j);
        err_msg(msg);
    }

    if(aux->id != 8213){
        sprintf(msg,"upperbound de %d retorna %d\n",j,aux->id);
        err_msg(msg);
    }else{
        sprintf(msg,"upperbound de %d retorna %d",j,aux->id);
        ok_msg(msg);
    }
}

void ub_test4(TreeMap* tree){ 
    int j=8214;
    Palabra* aux=upperBound(tree, &j);

    if(aux!=NULL) {
        sprintf(msg,"upperbound de %d retorna NULL",j);
        err_msg(msg);
    }else
        ok_msg("upperbound de 8214 retona NULL");
    
}

int main( int argc, char *argv[] ) {
    TreeMap * tree = initializeTree();

    if(argc==1 || strcmp(argv[1],"create")==0){
        info_msg("testing createTreeMap...");
        create_test1(); //10 es menor que 15
    }
    if(argc==1 || strcmp(argv[1],"search")==0){
        info_msg("testing searchTreeMap...");
        search_test1(tree); //5239: auto
        search_test2(tree); //8213: rayo
        search_test3(tree); //6980: hoja
        search_test4(tree); //clave no existe
    }
    if(argc==1 || strcmp(argv[1],"insert")==0){
        info_msg("testing searchTreeMap...");
        insert_test1(tree);
        insert_test2(tree);
    }
    if(argc==1 || strcmp(argv[1],"erase")==0){
        info_msg("testing insertTreeMap...");
        erase_test1();
        erase_test2();
        erase_test3();
    }
    if(argc==1 || strcmp(argv[1],"first")==0){
        info_msg("testing firstTreeMap...");
        first_test1(tree); 
        first_test2(); 
    }
    if(argc==1 || strcmp(argv[1],"next")==0){
        info_msg("testing nextTreeMap...");
        next_test1(); 
        next_test2();
        next_test3();
    }
    if(argc==1 || strcmp(argv[1],"ub")==0){
        info_msg("testing upperBound...");
        ub_test1(tree);
        ub_test2(tree);
        ub_test3(tree);
        ub_test4(tree);
    }

    if(argc==1) printf("All tests passed!\n");

    return 0;
}