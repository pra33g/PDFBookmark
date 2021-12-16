#include "install.v2.c"
typedef intptr_t ssize_t;

ssize_t getlineud(char **lineptr, size_t *n, FILE *stream) {
    size_t pos;
    int c;

    if (lineptr == NULL || stream == NULL || n == NULL) {
        errno = SIGINT;
        return -1;
    }

    c = getc(stream);
    if (c == EOF) {
        return -1;
    }

    if (*lineptr == NULL) {
        *lineptr = malloc(128);
        if (*lineptr == NULL) {
            return -1;
        }
        *n = 128;
    }

    pos = 0;
    while(c != EOF) {
        if (pos + 1 >= *n) {
            size_t new_size = *n + (*n >> 2);
            if (new_size < 128) {
                new_size = 128;
            }
            char *new_ptr = realloc(*lineptr, new_size);
            if (new_ptr == NULL) {
                return -1;
            }
            *n = new_size;
            *lineptr = new_ptr;
        }

        ((unsigned char *)(*lineptr))[pos ++] = c;
        if (c == '\n') {
            break;
        }
        c = getc(stream);
    }

    (*lineptr)[pos] = '\0';
    return pos;
}
bool init();
char *prepress;
void freeGlobals(){
    if (CURRENT_DIR)    free(CURRENT_DIR);
    if (PATH_GS)    free(PATH_GS);
    if (DEFAULT_PROGRAM_DIR)    free(DEFAULT_PROGRAM_DIR);
    if (prepress) free (prepress);
}

//#pragma exit freeGlobals

typedef unsigned int u_int;

//report errors in bookmark tree
void reportErrors(int line){
	printf("bookmarks.file error: line %d\n\n",line);
	printf("Please check syntax is:\n");
	printf("[PageNo] [Title]\n\n");
	printf("And that there aren't empty regions.\n");
	printf("HINT: Put a new line at end if only 1 bookmark in bookmarks.file.\n");

}

void printHelp(short mode){
    system("cls");
        char help[]="10 Title\n\
11 Contents\n\
\t30 Chapter 1\n\
\t31 Chapter 2\n\
12 Acknowledgments\n\
\t32 Author\n\
\t33 Editor\n\
\t\t34 Editor Notes 1\n\
\t\t35 Editor Notes 2\n\
\t\t13 Editor notes 3\n\
33 Back cover\n\
\t50 level 1";
        
    FILE *f=fopen("bookmarksEXAMPLE.file","w");
    fprintf(f,"%s",help);
    fclose(f);
    if(mode == 1) {
char longhelp[]="What is PDFBookmark?\n\n\
		You can use PDFBookmark to create bookmarks quickly in a '.pdf' file.\n\n\
		In a file named 'bookmarks.file' type the bookmarks with 1 bookmark per line.\n\
		YOU MUST USE TABS to create levels (or child bookmarks).\n\n\
		The syntax of typing bookmarks in bookmarks.file is simple:\n\n\
		[PageNumber] [Title of bookmark]\n\n\
		Refer to the generated 'bookmarks[EXAMPLE].file' for more.\n\n\
How to use PDFBookmark?\n\n\
		Command line usage:\n\
		PDFBookmark.exe [input_filename (NO SPACES)] [output_filename (NO SPACES)] [(optional)offset]\n\n\
	* input_filename - The pdf file in which you want to add the bookmarks\n\
	\n\t* output_filename - The name you want to give to final pdf\n\
	\n\t* offset - optional argument, the bookmark location( aka page number) will be shifted by the offset. \n\
	\nFor example, if your 'bookmarks.file' looks like:\n\n\
		[FILE START]\n\
		20 Example\n\
		[END_FILE]\n\n\
	Calling:\n\
		PDFBookmark someInput.pdf someOutput.pdf\n\n\
	Will take 'someInput.pdf', create  bookmark titled 'Example' to page 20 and save it in 'someOutput.pdf'.\n\n\
	However calling with the third optional argument (offset):\n\n\
		PDFmark someInput.pdf someOutput.pdf -10\n\n\
	Will create a bookmark titled 'Example' to page 10 (as 20 - 10 = 10).\n";
    printf("%s", longhelp);
    
    } else if (mode == 0) {

        printf("PDFBookmark - A tool to create pdf bookmarks quickly using pdfmark\n\n");
        printf("\nCommand line usage:\nPDFBookmark.exe [INPUT.PDF] [OUTPUT.PDF] [(optional)+/- PAGE_OFFSET]\n\n");
        printf("\nBookmarks.file Format:\n[PageNumber] [Bookmark name]\n\n");
        printf("YOU MUST USE TABS to create tree structure/levels\n\n");
        printf("Please store bookmarks in a file called bookmarks.file");
    } else if (mode == 2){
        printf("\n\nCommand line usage requires you to specify input and output filenames!");
        printf("\n\nTry calling 'PDFBookmark -c -h' (short help) or 'PDFBookmark -c -help' (longer help)");
    }

    printf("\n\nExample file bookmarksEXAMPLE.file created. Please remember to create 'bookmarks.file' .\n");
    getchar();
}

//check bookmarks file for line errors and 
int* checkBookmarks(FILE **bookmarks, int *_n_lines){
	
	int n_lines_ = 0;
	
	char *line =NULL;
	size_t size = 0;
	while(getlineud(&line,&size,*bookmarks)>0){
		n_lines_++;
	}
	free(line);
	fseek(*bookmarks,0,SEEK_SET);

	const int n_lines = n_lines_;
	int *treeArr =  (int *)calloc(n_lines+2,sizeof(int));
	{
		char *line =NULL;
		size_t size = 0;
		int i = 0;
		int temp=getlineud(&line,&size,*bookmarks);
		bool exit_true=false;
		while(temp>0){
			if(temp==1){				//check for empty lines
				if(exit_true){
					printf(", %d",i+1);
				} else {
					printf("Please remove empty line at lines: [%d",i+1);
				}
				exit_true=true;
			}
			int count = 0,sub_index = 0;
			while(line[sub_index++]==9){
				count++;
			}
			treeArr[i++]=count;
			temp=getlineud(&line,&size,*bookmarks);
		}
		free(line);
		if (exit_true){
			printf("] in bookmarks.file.\n");
			return NULL;
		}
//error check more than 1 tab per level
		for (int i = 0; i < n_lines; i++){
			int tabdiff=treeArr[i+1]-treeArr[i];
			if(tabdiff>1){
				printf("Extra tabs at %d.\nUse only 1 tab per level.\n",i+2);
				return NULL;
			}
		}
	 
	}


	{
		for (int i = 0; i< n_lines; i++){ 
			int j=i+1;
			int count = 0;
			if(treeArr[j]-treeArr[i]==1){
				while (treeArr[j]-treeArr[i]>=1)
				{
					if(treeArr[j]-treeArr[i]==1){
						count++;
					}
					j++;
				}
			}
			treeArr[i]=count;
		}		
	}
	*_n_lines = n_lines;
	return treeArr;
}

bool checkInit(){
    if(!init()){
        printf("\n\nGS GPL wasn't found. Init failed.\n\n");
        return false;
    }
    else {
        printf("\n\nEverything ok.\n\n");
        system("timeout 3");
    }
    system("cls");   //turn on in final ****************
    return true;

}

bool checkFileByName(char *filename, char code) {
    FILE *check = fopen(filename, "r");
    if(code == 'o'){
        if(check){
            printf("\n%s exists!\nTo avoid truncating it, choose a different output filename.\n", filename);
            perror(filename);
            fclose(check);
            return false;
        }
        else{
            // fclose(check);
            return true;
        }
    }
    else {
        if(!check){
            printf("\n\n Any file named '%s' doesn't seem to exist in current directory.\n If you created it, make sure it is in the same directory as PDFBookmark.exe\n\n ", filename);
            // fclose(check);
            perror(filename);
            return false;
        }
        else{
            // file length check
            fseek(check, 0, SEEK_END);
            size_t len = ftell(check);
            //   
            if (len <= 0){
                printf("%s seems to be empty!\n\n ", filename);
                fclose(check);
                perror(filename);
                return false;
            }
            else {
                fclose(check);
                return true;
            }
        }
    }
}

void printMenu(char *input_name, char *output_name, int *offset){
    printf("\t\t\t\tPDFBookmark\n\n\t\t\t\t         -by vizier_33");
    printf("\n\n* A tool to make adding bookmarks in pdfs simpler");
    printf("\n\n* Call with -c flag for command line usage.");
    printf("\n\n1. Set input filename ( currently -> %s )", input_name);
    printf("\n\n2. Set output filename ( currently -> %s )", output_name);
    printf("\n\n3. Set offset ( optional: current -> %d )", *offset);
    printf("\n\n4. Begin adding bookmarks");
    printf("\n\n5. Help");
    printf("\n\n6. Long help");
    printf("\n\n0. Exit PDFbookmark");
    printf("\n\n\tChoice:");
}

bool writePostScriptFile(char *bookmarks_name, const int n_lines, int *treeArr, short page_offset){
    printf("\n\nWriting PS File\n");
    char settings_file[]="53++1Nq5.txt";

    prepress = (char *) malloc (strlen(settings_file));
    strcpy( prepress, settings_file);


    FILE *settings=fopen(settings_file,"w"),
        *bookmarks = fopen(bookmarks_name,"r");
    long sizelen;
    char *line=NULL;
    size_t size=0;
    fseek(bookmarks,0,SEEK_SET);
    printf("Executing...\n");
    char *title=NULL;
        for (int i = 0; i < n_lines; i++)
        {
            unsigned pgno;
            sizelen=getlineud(&line,&size,bookmarks);
            title=(char*)realloc(title,sizelen*sizeof(char));

            if(!title){
                fclose(settings);
                return false;
            }


            int temp=sscanf(line,"%d %[^\n]s",&pgno,title);

            if(treeArr[i]){
                if(temp==2){
                    fprintf(settings,"[/Count %d /Page %d /Title (%s) /OUT pdfmark\n",treeArr[i],pgno+page_offset, title);
                } else{
                    free(title);
	                free(line);
                    reportErrors(i+1);
                    fclose(settings);
                    return false;
                }
            }else{
                if(temp==2){
                    fprintf(settings,"[/Page %d /Title (%s) /OUT pdfmark\n",pgno+page_offset, title);
                }else{
                    free(title);
                    free(line);
                    reportErrors(i+1);
                    fclose(settings);
                    return false;
                }
            }
            
        }
	free(title);
	free(line);
    fclose(settings);
    return true;
}
 

void addBookmarks(char *output_name, char *input_name, char *bookmarks_name){
    char cmd[2048];
    PATH_GS[strlen(PATH_GS)-1] = 0;

    // printf("%s", prepress);

    sprintf(cmd,"\"%s\" -dBATCH -dNOPAUSE -sDEVICE=pdfwrite -sOutputFile=%s -dPDFSETTINGS=/prepress %s %s", PATH_GS, output_name, prepress, input_name);
    printf("%s",cmd);

    bool done = !system(cmd);
    if (done){
        printf("\nSuccessful!");
    } else {
        printf("\nThere was some error.");
    }    
    remove(prepress);
}


int main(int argc, char* argv[]) { // add thanks to gpl gs in menu
    printf("If program takes too long to start, delete the file named \"directory.dat\", restart the program and press 'x' to reset Ghostscript directory.");
    if(!checkInit()){
        return 10;
    }
    //printf("%s\n%s", PATH_TO_GS,CURRENT_DIR_PATH);

    // 1--* program variables
    int page_offset=0;
	
    char bookmarks_name[50] = "bookmarks.file",
         output_name[500] = "NOT_SET",
         input_name[500] = "NOT_SET",
         not_set[] = "NOT_SET",
         temp;
    bool add_bookmarks = false;
    int n_lines_;  //temp storage for const int n_lines
    int *treeArr;
    // 1--*
    if (argc == 1){
        char choice = 1;
        while (choice) {

            printMenu(input_name,output_name, &page_offset);

            scanf("%d%*c", &choice);
             bool exit_loop = false;
            switch(choice){
                case 4:
                    if    ( 
                        checkFileByName(input_name,'i') &&
                        checkFileByName(output_name,'o') &&
                        checkFileByName(bookmarks_name,'b')
                            ){
                        exit_loop = true;
                    } 
                    else {
                        getchar();
                    }

                break;
                case 1: 
                    printf("Set input pdf filename: ");
                    fgets(input_name,sizeof(input_name),stdin);
                    input_name[strlen(input_name)-1] = 0;
                    if(!(checkFileByName(input_name, 'i'))){
                        getchar();
                        strcpy(input_name,not_set);
                    }
                break;
                case 2:
                    printf("Set output pdf filename: ");
                    fgets(output_name,sizeof(input_name),stdin);
                    output_name[strlen(output_name)-1] = 0;
                    if(!(checkFileByName(output_name, 'o'))){
                        strcpy(output_name,not_set);
                        getchar();

                    }
                break;
                case 3:
                    //set offset
                    printf("\nEnter offset for bookmark pages: ");
                    scanf("%d%*c", &page_offset);
                break;
                case 5:
                    //print help
                    printHelp(0);
                break;
                case 6:
                    //print long help
                    printHelp(1);
                break;
                case 0:
                    printf("\n\nC ya");
                    getchar();
                    exit(10);
                break;
                default:
                    printf("\n\nInvalid option.");
                    getchar();
                break;
            }
            if( exit_loop ){
                break;
            }
            //getchar();
            system("cls");
        }
    }
    else if (argc > 1 && !(strcmpi(argv[1],"-c"))) {
        switch (argc){
        case 5:
            {
            page_offset=atoi(argv[4]);
            }
        case 4:
            {
            if(strlen(input_name)>500 || strlen(output_name)>500){
            printf("\nInput filename or Output filename must be less than 500 characters!");
            exit(1);
            }
            strcpy(input_name, argv[2]);
            strcpy(output_name , argv[3]);
        }
        break;
        case 3:
		if (!(strcasecmp(argv[2],"-h"))){
            printHelp(0);
            freeGlobals();
            exit(0);
        }
        else if(!(strcasecmp(argv[2],"-help"))){
            printHelp(1);
            freeGlobals();
            exit(0);
        }
        case 2:
            printHelp(2);
            freeGlobals();
            exit(0);
        break;
        default:break;
        }

    } 
    else {
        printf("\n\t\t\tPDFBookmark\n\n Unrecognized/ Inconsistent arguments to PDFBookmark.\n Terminal Error.");
        add_bookmarks = 0;
    }

    if( !(checkFileByName(input_name,'i') &&
        checkFileByName(output_name,'o') &&
        checkFileByName(bookmarks_name,'b'))){
        
        printf("\nThere was an error.");
        getchar();
        add_bookmarks = false;
    } 
    else {
        FILE *bookmarks = fopen(bookmarks_name,"r");
        //check bookmarks.file for errors free lines errors, tab errors and create bookmark tree
        //store number of lines 
        treeArr = checkBookmarks(&bookmarks,&n_lines_); //store bookmark tree
        if (treeArr){
            add_bookmarks = true;
        } else {
            add_bookmarks = false;
        }
        fclose(bookmarks);
    }
  
    // const int n_lines = n_lines_;
   
    if(add_bookmarks){
        bool writtenPS = writePostScriptFile(bookmarks_name, n_lines_, treeArr, page_offset);
        if (writtenPS){
            printf("\n\nWritten PS file\n\n");
            addBookmarks(output_name, input_name, bookmarks_name);
        }
        else {
            printf("\n\nError writing PS file. \n\nReport to pdfbmdev@example.com (It helps me make PDFBookmark better for a lot of people :)");
        }
        //addBookmarks();
        free(treeArr);
    }
    else {
        printf("\n\n Quit.");
    }
    printf("\n\nPlease report buggy behavior to pdfbmdev@example.com, I would highly appreciate it.");
    printf("\n\nThanks for using PDFBookmark, an open source project.");
    getchar();
    freeGlobals();
    return 0;
}
