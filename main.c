#include "comic.h"


int main(int argc, char* argv[])
{
    // exit if wrong number of arguments
    if ( argc != 3 )
	{
		fprintf(stderr, "Usage: comics <input-file> <output-file>\n");
		exit(1);
	}
    // open input and output files
    FILE* in;
	in = fopen(argv[1], "r");
    FILE* out;
	out = fopen(argv[2], "w");
    
    // command is string, assume max length 10
	char command[10];
    fscanf(in, "%s", command );

    // create comic list where books are loaded 
    struct Comic_List comic_list = init_list();

    // create purchases list
    struct Comic_List purchases = init_list();

    // iterate until no commands left
	while( !feof(in) )
    {
        // load
        if (strcmp(command,"load")==0)
        {
            // scan filename in (assuming max filename length is 99)
            char* filename = calloc(100,sizeof(char));
            fscanf(in,"%s",filename);
            // store previous count to compare
            int previous_count = comic_list.count;
            // append data in filename to comic_list
            comic_list=load(filename,comic_list);
            // output command and show how many comics were loaded
            fprintf(out,"Command: load %s\n\tNumber of comics: %d\n",filename,comic_list.count-previous_count);
            // free up string memory taken up by filename
            free(filename);
        }
        // display
        else if (strcmp(command,"display")==0)
        {
            // display method
            fprintf(out,"Command: display\n");
            display(out,comic_list);
        }
        // buy
        else if (strcmp(command,"buy")==0)
        {
            // scan in next number to index
            int index;
            fscanf(in,"%d",&index);
            fprintf(out,"Command: buy %d\n",index);
            // attempt to purchase book at index
            purchases=buy(out,index,comic_list,purchases);
        }
        // checkout
        else if (strcmp(command,"checkout")==0)
        {
            // display list of purchases
            fprintf(out,"Command: checkout\nComics in Purchase List\n");
            // call display() because similar output and more efficient
            display(out,purchases);
            // check out and calculate cost
            checkout(out,purchases);
        }
        // save
        else if (strcmp(command,"save")==0)
        {
            // scan in next string as filename 
            char* filename = calloc(100,sizeof(char));
            fscanf(in,"%s",filename);
            fprintf(out,"Command: save %s\n",filename);
            // save to file
            save(filename,comic_list);
            // free up string memory
            free(filename);
        }
        // clear
        else if (strcmp(command,"clear")==0)
        {
            // clear list
            fprintf(out,"Command: clear\n");
            comic_list=clear(comic_list);
        }
        // find
        else if (strcmp(command,"find")==0)
        {
            // scan in next number to index
            int index;
            fscanf(in,"%d",&index);
            fprintf(out,"Command: find %d\n",index);
            // find comic at index and display data
            find(out,index,comic_list);
        }
        // remove
        else if (strcmp(command,"remove")==0)
        {
            // scan in next number to index
            int index;
            fscanf(in,"%d",&index);
            fprintf(out,"Command: remove %d\n",index);
            // find comic, remove, and shift all other entries forward, return new count of comics in list
            comic_list.count = remove_comic(out,index,comic_list);
        }
        // scan in next command
        fscanf(in, "%s", command );
    }
    // clear comic list (will clear everything in purchases because it is all shallow copies)
    comic_list=clear(comic_list);
    // free memory in the comic lists
    free(comic_list.list);
    free(purchases.list);
    // close file connections
    fclose(in);
    fclose(out);
}

