#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

//make directory
void mkdir(char pathName[]){

    if(strcmp(pathName, "/") == 0){
        printf("MKDIR ERROR: no path provided\n");
        return;
    }
    char baseName[128];
    char dirName[128];

    struct NODE* parent = splitPath(pathName, baseName, dirName);
    if(parent == NULL){
        return;
    }

    //check whether the name already exists in the parent directory
    struct NODE* child = parent->childPtr;
    struct NODE* last = NULL;
    while(child != NULL){
        if(strcmp(child->name, baseName) == 0){
            printf("MKDIR ERROR: directory %s already exists\n", pathName);
            return;
        }
        last = child;
        child = child->siblingPtr;
    }

    struct NODE* newNode = (struct NODE*)malloc(sizeof(struct NODE));
    if(newNode == NULL){
        return;
    }
    strncpy(newNode->name, baseName, 63);
    newNode->name[63] = '\0';
    newNode->fileType = 'D';
    newNode->childPtr = NULL;
    newNode->siblingPtr = NULL;
    newNode->parentPtr = parent;

    if(last == NULL){
        parent->childPtr = newNode;
    }
    else{
        last->siblingPtr = newNode;
    }

    printf("MKDIR SUCCESS: node %s successfully created\n", pathName);
    return;
}

//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName){

    //root path
    if(strcmp(pathName, "/") == 0){
        strcpy(dirName, "/");
        strcpy(baseName, "");
        return root;
    }

    char* lastSlash = strrchr(pathName, '/');

    if(lastSlash == NULL){
        //just a name in the current directory
        strcpy(dirName, "");
        strcpy(baseName, pathName);
        return cwd;
    }

    if(lastSlash == pathName){
        //something like /a, so the parent is root
        strcpy(dirName, "");
        strcpy(baseName, pathName + 1);
        return root;
    }

    //general case, split at the last slash
    size_t len = (size_t)(lastSlash - pathName);
    strncpy(dirName, pathName, len);
    dirName[len] = '\0';
    strcpy(baseName, lastSlash + 1);

    //walk the directories in dirName
    struct NODE* current = (pathName[0] == '/') ? root : cwd;

    char copy[128];
    strncpy(copy, dirName, 127);
    copy[127] = '\0';

    char* token = strtok(copy, "/");
    while(token != NULL){
        struct NODE* child = current->childPtr;
        while(child != NULL){
            if(child->fileType == 'D' && strcmp(child->name, token) == 0){
                break;
            }
            child = child->siblingPtr;
        }
        if(child == NULL){
            printf("ERROR: directory %s does not exist\n", token);
            return NULL;
        }
        current = child;
        token = strtok(NULL, "/");
    }

    return current;
}
