#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// list of comics
struct Comic_List
{
    // actual list of objects
    struct Comic* list;
    // max capacity of list
    int size;
    // current count of comics
    int count;
};
// comic book object
struct Comic
{
    // date published
    char* date;
    // code
    char* code;
    // publisher of comic
    char* publisher;
    // title of comic
    char* title;
    // price of comic
    char *cost;
};
// initialize new struct Comic_List size 10
struct Comic_List init_list();
// load comics and append them to comic list, returns modified comic list
struct Comic_List load(char* filename,struct Comic_List c_list);
// print comics in list to output file
void display(FILE* out, struct Comic_List c_list);
// add comic at index from comic list to purchases, return purcjases list
struct Comic_List buy(FILE* out, int index, struct Comic_List c_list, struct Comic_List purchases);
// calculate total price of comics in purchases
void checkout(FILE* out,struct Comic_List purchases);
// save comics in comic list to file at filename
void save(char* filename, struct Comic_List c_list);
// clear all comics in comic list, return emptied comic list
struct Comic_List clear(struct Comic_List c_list);
// find comic at index in comic list and print its data
void find(FILE* out, int index,struct Comic_List c_list);
// remove comic at index from comic list and return new count of comics
int remove_comic(FILE* out,int index,struct Comic_List c_list);
// frees string data in comic at index of comic list
void free_comic(int index, struct Comic_List c_list);