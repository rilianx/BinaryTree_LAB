#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

//Ejemplo de uso de mapa

char * _strdup(const char * str) {
    char * aux = (char *)malloc(strlen(str) + 1);
    strcpy(aux, str);
    return aux;
}

int lower_than_string(void* key1, void* key2){
    char* k1=(char*) key1;
    char* k2=(char*) key2;
    if(strcmp(k1,k2)<0) return 1;
    return 0;
}


int main(){
    TreeMap* map = createTreeMap(lower_than_string);

    char words[9][5] = {"saco","cese","case","cosa","casa","cesa",
    "cose","seco","saca"};

    int i=0;
    for(;i<9; i++){
        insertTreeMap(map,_strdup(words[i]),_strdup(words[i]));
    }

    char* aux= firstTreeMap(map);
    while(aux!=NULL){
        printf("%s\n", aux);
        aux=nextTreeMap(map);
    }

    return 0;

}