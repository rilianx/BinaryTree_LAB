#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap-answer.c"

char * _strdup(const char * str) {
    char * aux = (char *)malloc(strlen(str) + 1);
    strcpy(aux, str);
    return aux;
}

char msg[100];

void err_msg(char* msg){
    fprintf(stderr,"[\033[1;31m FAILED \033[0m] ");
    fprintf(stderr,"%s\n",msg);
    exit(-1);
}

void ok_msg(char* msg){
    printf ("[\033[32;1m OK \033[0m] ");
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
    printf("[INFO] inicializando el arbol..\n");
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
        exit(-1);
    }else 
        ok_msg("createTreeMap retorna un objeto");

    if(t->root!=NULL){
        fprintf(stderr,"[FAIL] root debe ser NULL\n");
        exit(-1);
    } else
        ok_msg("root==NULL");
    

    int i=10,j=15;
    if(t->lower_than(&i,&j) !=1 ) {
        fprintf(stderr,"[FAIL] la funcion lower_than no fue guardada de forma correcta\n");
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
        fprintf(stderr,"[FAIL] no encuentra dato con clave 5239\n");
        exit(-1);
    }
    
    if(tree->current != tree->root){
        fprintf(stderr,"[FAIL] no actualiza current (5329)\n");
        exit(-1);
    }else
        ok_msg("current actualizado correctamente");

}

void search_test2(TreeMap* tree){ //key en root->right
    int key=8213;
    Palabra* pal = searchTreeMap(tree, &key);
    if(pal!=NULL && pal->id==8213)
        ok_msg("encuentra dato con clave 8213");
    else{
        fprintf(stderr,"[FAIL] no encuentra dato con clave 8213\n");
        exit(-1);
    }

    if(tree->current != tree->root->right){
        fprintf(stderr,"[FAIL] no actualiza current correctamente (8213)\n");
        exit(-1);
    }else
        ok_msg("current actualizado correctamente");
}

void search_test3(TreeMap* tree){ //key en root->right->left
    int key=6980;
    Palabra* pal = searchTreeMap(tree, &key);
    if(pal!=NULL && pal->id==key){
        sprintf(msg, "encuentra dato con clave %d",key);
        ok_msg(msg);
    }else{
        fprintf(stderr,"[FAIL] no encuentra dato con clave %d\n",key);
        exit(-1);
    }

    if(tree->current != tree->root->right->left){
        fprintf(stderr,"[FAIL] no actualiza current correctamente (%d)\n",key);
        exit(-1);
    }else
        ok_msg("current actualizado correctamente");
    
}

void search_test4(TreeMap* tree){ //key no existe
    int key=7010;
    Palabra* pal = searchTreeMap(tree, &key);
    
    if(pal==NULL) {
        sprintf(msg, "retorna NULL: search(key=%d)",key);
        ok_msg(msg);
    }else{
        fprintf(stderr,"[FAIL] retorna dato y clave no existe (%d)\n",key);
        exit(-1);    
    }
    
}

void insert_test1(TreeMap* tree){//dato repetido
    Palabra* p = creaPalabra(1273,_strdup("repetido"));
    insertTreeMap(tree, &p->id, p);

    if(tree->root->left->left != NULL ||
           tree->root->left->right != NULL){
        fprintf(stderr,"[FAIL] se inserta dato repetido\n");
        exit(-1);
    }
    else
        ok_msg("no inserta dato repetido");

}

void insert_test2(){
    TreeMap * tree = initializeTree();

    Palabra* p = creaPalabra(900,_strdup("maicol"));
    printf("[INFO] insertando dato con clave 900\n");
    insertTreeMap(tree, &p->id, p);
    

    if(tree->root->left->left == NULL ||
           tree->root->left->left->value!=p) {
        fprintf(stderr,"[FAIL] dato insertado no se encuentra en root->left->left \n");
        exit(-1);
    }else
        ok_msg("dato insertado correctamente");
    
    if(tree->root->left->left != NULL &&
            tree->root->left->left->key!=&p->id) {
                fprintf(stderr,"[FAIL] clave de dato no se guarda correctamente\n");
                exit(-1);
            }

    if(tree->root->left->left->parent != 
            tree->root->left){
                fprintf(stderr,"[FAIL] no se inicializa el padre del nuevo nodo\n");
                exit(-1);
            }
    
    if(tree->current != tree->root->left->left){
        fprintf(stderr,"[FAIL] no se actualiza el current\n");
                exit(-1);
     }
        ok_msg("current actualizado correctamente");


}

void erase_test1(){//nodo sin hijos
    TreeMap * tree = initializeTree();
    
    int key=1273;
    printf("[INFO] eliminando dato con clave 1273 (nodo sin hijos)\n");
    eraseTreeMap(tree, &key);
    if(tree->root->left != NULL) {
        fprintf(stderr,"[FAIL] el dato no se elimino correctamente: tree->root->left != NULL\n");
        exit(-1);
    }else
        ok_msg("dato eliminado correctamente");

}

void erase_test2(){//nodo con un hijo
    TreeMap * tree = initializeTree();
    
    int key=8213;
    printf("[INFO] eliminando dato con clave 8213 (nodo con un hijo)\n");
    eraseTreeMap(tree, &key);

    if( * ((int*) tree->root->right->key) != 6980) {
        fprintf(stderr,"[FAIL] el dato no se elimino correctamente root->right!=6980\n");
        exit(-1);
    }
    

    if(  tree->root->right->parent != tree->root){
        fprintf(stderr,"[FAIL] falta actualizar el parent de nodo 6980\n");
    }else
        ok_msg("dato eliminado correctamente");
}

void erase_test3(){//nodo con dos hijos
    TreeMap * tree = initializeTree();
    
    int key=5239;
    printf("[INFO] eliminando dato con clave 5239 (nodo con dos hijos)\n");
    eraseTreeMap(tree, &key);

    if( * ((int*) tree->root->key) != 6980){
        fprintf(stderr,"[FAIL] el dato no se elimino correctamente root!=6980\n");
        exit(-1);
    }

    if( * ((int*) tree->root->right->key) != 8213){
        fprintf(stderr,"[FAIL] el dato no se elimino correctamente root->right!=8213\n");
        exit(-1);
    }else
        ok_msg("dato eliminado correctamente");
    
}

void first_test1(TreeMap * tree){ 
    Palabra* aux=firstTreeMap(tree);
    if(!aux)
        err_msg("[FAIL] first retorna NULL\n");

    if(aux->id != 1273)
        err_msg("[FAIL] first no retorna nodo 1273\n");
    else
        ok_msg("first retorna nodo 1273");
}

void first_test2(){ //debes iterar
    TreeMap * tree = initializeTree();
    printf("[INFO] agregando nodo con clave 100\n");
    Palabra* p=creaPalabra(100,"first_word");
    tree->root->left->left=createTreeNode(&p->id, p);
    tree->root->left->left->parent=tree->root->left;

    Palabra* aux=firstTreeMap(tree);
    if(aux->id != 100)
        err_msg("[FAIL] first no retorna nodo 100\n");
    else
        ok_msg("first retorna nodo 100");
}

void next_test1(){ //caso1: current tiene hijo derecho
    TreeMap * tree = initializeTree();
    printf("[INFO] agregando nodo con clave 2000\n");
    Palabra* p=creaPalabra(2000,"next_word");
    tree->root->left->right=createTreeNode(&p->id, p);
    tree->root->left->right->parent=tree->root->left;

    printf("[INFO] actualizando current -> nodo 1273\n");
    tree->current = tree->root->left;
    Palabra* aux=nextTreeMap(tree);

    if(aux==NULL) err_msg("[FAIL] next retorna NULL\n");

     if(aux->id != 2000)
        err_msg("[FAIL] next no retorna nodo 2000");
    else
        ok_msg("next retorna nodo 2000");   


}

void next_test2(){ //caso1: current tiene hijo derecho
    TreeMap * tree = initializeTree();
    printf("[INFO] actualizando current -> root \n");
    tree->current = tree->root;
    Palabra* aux=nextTreeMap(tree);

     if(aux->id != 6980)
        err_msg("[FAIL] next no retorna nodo 6980\n");
    else
        ok_msg("next retorna nodo 6980");  
    
    aux=nextTreeMap(tree);
    
    if(aux->id != 8213)
        err_msg("[FAIL] next no retorna nodo 8213\n");
    else
        ok_msg("next retorna nodo 8213");  

    aux=nextTreeMap(tree);
    if(aux)
        err_msg("[FAIL] next != NULL\n");
    else
        ok_msg("next retorna NULL");  

}

void next_test3(){ //caso2: current sin hijo derecho
    TreeMap * tree = initializeTree();
    printf("[INFO] agregando nodo con clave 2000\n");
    Palabra* p=creaPalabra(2000,"next_word");
    tree->root->left->right=createTreeNode(&p->id, p);
    tree->root->left->right->parent=tree->root->left;
    printf("[INFO] actualizando current -> nodo 2000\n");
    tree->current=tree->root->left->right;

    Palabra* aux=nextTreeMap(tree);
    
    if(aux->id != 5239)
        err_msg("[FAIL] next no retorna nodo 5239\n");
    else
        ok_msg("next retorna nodo 5239");
}

void ub_test1(TreeMap* tree){ //el dato existe
    int j=6980;
    Palabra* aux=upperBound(tree, &j);

    if(aux==NULL) {
        fprintf(stderr,"[FAIL] upperbound de %d retorna NULL",j);
        exit(-1);
    }

    if(aux->id != 6980){
        fprintf(stderr,"[FAIL] upperbound de %d retorna %d\n",j,aux->id);
        exit(-1);
    }else{
        sprintf(msg,"upperbound de %d retorna %d",j,aux->id);
        ok_msg(msg);
    }

}

void ub_test2(TreeMap* tree){ 
    int j=6979;
    Palabra* aux=upperBound(tree, &j);

    if(aux==NULL) {
        fprintf(stderr,"[FAIL] upperbound de %d retorna NULL",j);
        exit(-1);
    }

    if(aux->id != 6980){
        fprintf(stderr,"[FAIL] upperbound de %d retorna %d\n",j,aux->id);
        exit(-1);
    }else{
        sprintf(msg,"upperbound de %d retorna %d",j,aux->id);
        ok_msg(msg);
    }
}

void ub_test3(TreeMap* tree){
    int j=6981;
    Palabra* aux=upperBound(tree, &j);

    if(aux==NULL) {
        fprintf(stderr,"[FAIL] upperbound de %d retorna NULL\n",j);
        exit(-1);
    }

    if(aux->id != 8213){
        fprintf(stderr,"[FAIL] upperbound de %d retorna %d\n",j,aux->id);
        exit(-1);
    }else{
        sprintf(msg,"upperbound de %d retorna %d",j,aux->id);
        ok_msg(msg);
    }
}

void ub_test4(TreeMap* tree){ 
    int j=8214;
    Palabra* aux=upperBound(tree, &j);

    if(aux!=NULL) {
        fprintf(stderr,"[FAIL] upperbound de %d retorna NULL\n",j);
        exit(-1);
    }else
        ok_msg("upperbound de 8214 retona NULL");
    
}

int main( int argc, char *argv[] ) {
    TreeMap * tree = initializeTree();

    if(argc==1 || strcmp(argv[1],"create")==0){
        create_test1(); //10 es menor que 15
    }
    if(argc==1 || strcmp(argv[1],"search")==0){
        search_test1(tree); //5239: auto
        search_test2(tree); //8213: rayo
        search_test3(tree); //6980: hoja
        search_test4(tree); //clave no existe
    }
    if(argc==1 || strcmp(argv[1],"insert")==0){
        insert_test1(tree);
        insert_test2(tree);
    }
    if(argc==1 || strcmp(argv[1],"erase")==0){
        erase_test1();
        erase_test2();
        erase_test3();
    }
    if(argc==1 || strcmp(argv[1],"first")==0){
        first_test1(tree); 
        first_test2(); 
    }
    if(argc==1 || strcmp(argv[1],"next")==0){
        next_test1(); 
        next_test2();
        next_test3();
    }
    if(argc==1 || strcmp(argv[1],"ub")==0){
        ub_test1(tree);
        ub_test2(tree);
        ub_test3(tree);
        ub_test4(tree);
    }

    printf("All tests passed!\n");

    return 0;
}