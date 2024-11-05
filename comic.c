#include "comic.h"

struct Comic_List init_list()
{
    // instantiate struct Comic_List
    struct Comic_List comic_list;
    // default size is 10
    comic_list.size = 10;
    // starts empty
    comic_list.count = 0;
    // alloc enough space for 10 comics
    comic_list.list = (struct Comic*) malloc(comic_list.size*sizeof(struct Comic));
    return comic_list;
}
struct Comic_List load(char* filename,struct Comic_List c_list)
{
    // open file
    FILE* data = fopen(filename,"r");
    // skip first line (header)
    fscanf(data, "%*[^\n]\n");
	while(!feof(data))
    {
        // make new struct Comic, alloc max size strings
        struct Comic c;
        c.date=calloc(100,sizeof(char));
        c.code=calloc(100,sizeof(char));
        c.publisher=calloc(100,sizeof(char));
        c.title=calloc(1000,sizeof(char));
        c.cost=calloc(10,sizeof(char));
        // scan data into strings
        fscanf(data,"%[^,],%[^,],%[^,],%[^,],%[^ \n]\n",c.date,c.code,c.publisher,c.title,c.cost);
        // realloc to 1 + string length to account for null terminator
        c.date = (char*)realloc(c.date,(strlen(c.date)+1)*sizeof(char*));
        c.code = (char*)realloc(c.code,(strlen(c.code)+1)*sizeof(char*));
        c.publisher = (char*)realloc(c.publisher,(strlen(c.publisher)+1)*sizeof(char*));
        c.title = (char*)realloc(c.title,(strlen(c.title)+1)*sizeof(char*));
        c.cost = (char*)realloc(c.cost,(strlen(c.cost)+1)*sizeof(char*));
        // add object to list, increase count
        c_list.list[c_list.count]=c;
        c_list.count+=1;
        // double list size and realloc if full
        if (c_list.count==c_list.size)
        {
            c_list.size *= 2;
            c_list.list = (struct Comic*)realloc(c_list.list,c_list.size*sizeof(struct Comic));
        }
    }
    // close connection to data
    fclose(data);
    // return updated list
    return c_list;
}
void display(FILE* out, struct Comic_List c_list)
{
    // if not empty
    if (c_list.count>0)
    {
        // iterate through list
        for (int i = 0; i<c_list.count; i++)
        {
            // print comic number header
            fprintf(out,"Comic Number: %d\n",i+1);
            // call find() because they both list data about the struct Comic
            find(out,i,c_list);
        }
    }
    else
    {
        // print statement saying list is empty
        fprintf(out,"List is currently empty.\n");
    }
    
}
struct Comic_List buy(FILE* out, int index, struct Comic_List c_list, struct Comic_List purchases)
{
    // if valid index
    if (0<=index && index<c_list.count)
    {
        // add item to purchases list, increase counter (shallow copies)
        purchases.list[purchases.count]=c_list.list[index];
        purchases.count+=1;
        // double purchase list capacity if at capacity
        if (purchases.count==purchases.size)
        {
            purchases.size*=2;
            purchases.list=(struct Comic*)realloc(purchases.list,purchases.size*sizeof(struct Comic));
        }
        fprintf(out,"Comic #%d added to purchase list\n",index);
    }
    else
    {
        // invalid index, do nothing
        fprintf(out,"Unable to buy comic #%d\n",index);
    }
    // return purchases list
    return purchases;
    
}

void checkout(FILE* out, struct Comic_List purchases)
{
    // calculate subtotal
    float subtotal=0.00;
    // iterate through purchases
    for (int i = 0; i<purchases.count; i++)
    {
        // find comic, shift ahead by 1 char (to get rid of $ symbol) and convert char sequence of cost to float
        subtotal+=atof(purchases.list[i].cost+1);
    }
    // print, rounded to 2 decimal points
    fprintf(out," Subtotal:  $%.2f\n      Tax:  $%.2f\n    Total:  $%.2f\n",subtotal,subtotal*0.05,subtotal*1.05);
}

void save(char* filename, struct Comic_List c_list)
{
    // open output file specified by filename
    FILE* output = fopen(filename,"w");
    // headers
    fprintf(output,"DATE,CODE,PUBLISHER,TITLE,PRICE\n");
    // iterate through comic list
    for (int i=0;i<c_list.count;i++)
    {
        // print csv data about the comic
        fprintf(output,"%s,%s,%s,%s,%s\n",c_list.list[i].date,c_list.list[i].code,c_list.list[i].publisher,c_list.list[i].title,c_list.list[i].cost);
    }
    // close connection to output file
    fclose(output);
}

struct Comic_List clear(struct Comic_List c_list)
{
    // iterate through list
    for (int i=0;i<c_list.count;i++)
    {
        // free comic at index
        free_comic(i,c_list);
    }
    // reset count to 0 and return
    c_list.count=0;
    return c_list;
}

void find(FILE* out, int index,struct Comic_List c_list) 
{
    // if valid index
    if ((0<=index) && (index<c_list.count))
    {
        // print all data about the Comic at the index
        fprintf(out,"\tDate: %s\n\tCode: %s\n\tPublisher: %s\n\tTitle: %s\n\tCost: %s\n",c_list.list[index].date,c_list.list[index].code,c_list.list[index].publisher,c_list.list[index].title,c_list.list[index].cost);
    }
    else
    {
        // invalid index
        fprintf(out,"There is no comic at index #%d in the list.\n",index);
    }
}

int remove_comic(FILE* out,int index,struct Comic_List c_list)
{
    // if valid index
    if ((0<=index) && (index<c_list.count))
    {
        // free comic at index
        free_comic(index,c_list);
        // iterate from index onwards to the end
        for (int i = index; i<(c_list.count); i++)
        {
            // shift all indices after towards front of the list
            c_list.list[i]=c_list.list[i+1];
        }
        // successful, return new count, decremented by 1
        fprintf(out,"Comic at index %d successfully removed\n",index);
        return c_list.count-1;
    }
    else
    {
        // invalid index, return 
        fprintf(out,"Comic at index %d was not removed\n",index);
        return c_list.count;
    }
}
void free_comic(int index, struct Comic_List c_list)
{
    // free all string data and null out all pointers
    free(c_list.list[index].date);
    free(c_list.list[index].code);
    free(c_list.list[index].publisher);
    free(c_list.list[index].title);
    free(c_list.list[index].cost);
    c_list.list[index].date=NULL;
    c_list.list[index].code=NULL;
    c_list.list[index].publisher=NULL;
    c_list.list[index].title=NULL;
    c_list.list[index].cost=NULL;
}