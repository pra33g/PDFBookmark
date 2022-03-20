
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <ctype.h>
typedef unsigned int u_int;
void c(){printf("RUN");}
void error(int line,FILE *f, char *close){
	printf("bookmarks.file error: line %d\n\n",line);
	printf("Please check syntax is:\n");
	printf("[PageNo] [Title]\n\n");
	printf("And that there aren't empty regions.\n");
	printf("HINT: Put a new line at end if only 1 bookmark in bookmarks.file.\n");
	fclose(f);
	remove(close);
	exit(7);
}
long linelen(FILE *f){
	char c=fgetc(f);
	long len=0;
	while(c!=10 && c!=-1){
		len++;
		c=fgetc(f);
	}
	
	// printf("%d\n",len);
	fseek(f,-(len+1),SEEK_CUR);
	return len;
}
void printarr(int* b, int size){
	printf("\n");

	for(int i = 0; i< size;i++){
		printf("%d ",b[i]);
	}
	printf("\n");

}
bool isnumber(char *string){
	int i=0;
	while (string[i]!=10 && string[i]!=0)
	{

		if(!isdigit(string[i])){
			return false;
		}
		i++;
	}
	return true;
}
int main(int argc, char* argv[]){ //use malloc on *output and bookmarks_size
	int page_offset=0;
	
	char bookmarks_name[50] = "bookmarks.file",
		 output_name[500] = "output.pdf",
		 input_name[500] = "INPUT_FILE_PDF";
	switch(argc){
	case 4:;
		page_offset=atoi(argv[3]);
	case 3:
	if(strlen(input_name)>500 || strlen(output_name)>500){
		printf("\nInput filename or Output filename must be less than 500 characters!");
		exit(1);
	}
	strcpy(input_name, argv[1]);
	strcpy(output_name , argv[2]);
	
	
	case 2:
	if (!(strcasecmp(argv[1],"-h"))){
		printf("PDFbookmark - A tool to create pdf bookmarks quickly using pdfmark\n\n");
		printf("\nUsage:\n./PDFbookmark [INPUT.PDF] [OUTPUT.PDF] [(optional)+/- PAGE_OFFSET]\n\n");
		printf("\nFormat:\n[PageNumber] [Bookmark name]\n\n");
		printf("Use tabs to create tree structure/levels\n\n");
		printf("Please store bookmarks in a file called bookmarks.file\n");

		printf("Example file bookmarksEXAMPLE.file created.\nYou can edit it to your needs.\n");


char help[]="10 Title\n\
11 Contents\n\
	30 Chapter 1\n\
	31 Chapter 2\n\
12 Acknowledgments\n\
	32 Author\n\
	33 Editor\n\
		34 Editor Notes 1\n\
		35 Editor Notes 2\n\
		13 Editor notes 3\n\
33 Back cover\n\
	50 level 1";
		
		FILE *f=fopen("bookmarksEXAMPLE.file","w");
		fprintf(f,"%s",help);
		fclose(f);
		exit(3);

	}
	
	if(!(strcasecmp(argv[1],"-help"))){
char help[]="10 Title\n\
11 Contents\n\
	30 Chapter 1\n\
	31 Chapter 2\n\
12 Acknowledgments\n\
	32 Author\n\
	33 Editor\n\
		34 Editor Notes 1\n\
		35 Editor Notes 2\n\
		13 Editor notes 3\n\
33 Back cover\n\
	50 level 1";

char longhelp[]="What is PDFbookmark?\n\n\
	You can use PDFbookmark to create bookmarks quickly in a '.pdf' file.\n\n\
	In a file named 'bookmarks.file' type the bookmarks with 1 bookmark per line.\n\
	Use tabs to create levels (or child bookmarks).\n\n\
	The syntax of typing bookmarks in bookmarks.file is simple:\n\n\
	[PageNumber] [Title of bookmark]\n\n\
	Refer to the generated 'bookmarks[EXAMPLE].file' for more.\n\n\
How to use PDFbookmark?\n\n\
	Usage:\n\
	./PDFbookmark [input_file] [output_filename] [(optional)offset]\n\n\
input_file - The pdf file in which you want to add the bookmarks\n\
output_filename - The name you want to give to final pdf\n\
offset - optional argument, the bookmark location will be shifted by the offset. \n\
For example, if your bookmarks.file looks like:\n\
	[FILE START]\n\
	20 Example\n\
	[END_FILE]\n\n\
	Calling:\n\
		./PDFbookmark someInput.pdf someOutput.pdf\n\
	Will take 'someInput.file', create  bookmark titled 'Example' to page 20 and save it in 'someOutput.file'.\n\n\
	However calling with the third optional argument (offset):\n\
		./PDFmark someInput.pdf someOutput.pdf -10\n\
	Will create a bookmark titled 'Example' to page 10 (as 20-10=10).\n";
		
		FILE *f=fopen("bookmarksEXAMPLE.file","w");
		fprintf(f,"%s",help);
		fprintf(stdout,"%s",longhelp);
		// system(longhelp);
		fclose(f);
		exit(4);
	}
	
	break;
	default:
	printf("Missing or inconsistent arguments.\nUse -h for short help and -help for long help.\n");
	exit(3);
	break;
	// //copy output file names & check for number of args and control offset
	// switch
	}

	FILE *bookmarks = fopen(bookmarks_name,"r"),
		 *input = fopen(input_name,"r"),
		 *output = fopen(output_name,"r");
	
	/* error checking*/
	if(!input){
		perror(input_name);
		exit(1);

	}
	if(!bookmarks){
		perror(bookmarks_name);
		exit(1);

	}
	if(output){
		printf("%s already exists!\nPlease choose different filename.\n",output_name);
		fclose(output);
		exit(1);
	}
	fclose(input);
	// check for empty bookmarks file
	fseek(bookmarks,0,SEEK_END);
	int bookmarks_size=ftell(bookmarks);
	if(!bookmarks_size){
		printf("nothing to read in bookmarks.file!\n");
		exit(6);
	}
	fseek(bookmarks,0,SEEK_SET);
	// empty lines in bookmarks.file
	/*error checking*/

	int n_lines_ = 0;
	
	char *line =NULL;
	size_t size = 0;
	while(getline(&line,&size,bookmarks)>0){
		n_lines_++;
	}
	free(line);
	fseek(bookmarks,0,SEEK_SET);

	const int n_lines = n_lines_;
	int *treeArr =  (int *)calloc(n_lines+2,sizeof(int));
	{
		char *line =NULL;
		size_t size = 0;
		int i = 0;
		int temp=getline(&line,&size,bookmarks);
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
			// printf("%d ",count);
			temp=getline(&line,&size,bookmarks);
		}
		free(line);
		if (exit_true){
			printf("] in bookmarks.file.\n");
			exit(4);
		}
//error check more than 1 tab per level
		for (int i = 0; i < n_lines; i++){
			int tabdiff=treeArr[i+1]-treeArr[i];
			if(tabdiff>1){
				printf("Extra tabs at %d.\nUse only 1 tab per level.\n",i+2);
				exit(5);
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

	//mudde pr aao
	char settings_file[]="53++1Nq5.txt";
	FILE *settings=fopen(settings_file,"w");
	long sizelen;
	line=NULL;
	size=0;
	fseek(bookmarks,0,SEEK_SET);
	printf("Executing...\n");
	char *title=NULL;
	for (int i = 0; i < n_lines; i++)
	{
		unsigned pgno;
		sizelen=getline(&line,&size,bookmarks);
		title=(char*)realloc(title,sizelen*sizeof(char));
		int temp=sscanf(line,"%d %[^\n]s",&pgno,title);

		if(treeArr[i]){
			if(temp==2){
				fprintf(settings,"[/Count %d /Page %d /Title (%s) /OUT pdfmark\n",treeArr[i],pgno+page_offset, title);
			} else{
				error(i+1,bookmarks,settings_file);
			}
		}else{
			if(temp==2){
				fprintf(settings,"[/Page %d /Title (%s) /OUT pdfmark\n",pgno+page_offset, title);
			}else{
				error(i+1,bookmarks,settings_file);
			}
		}
		
	}
	free(title);
	free(line);
	char cmd[2048];
	fclose(settings);
	sprintf(cmd,"gs -dBATCH -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOutputFile=%s -dPDFSETTINGS=/prepress %s %s",output_name,settings_file,input_name);
	system(cmd);
	remove(settings_file);
	free(treeArr);
	fclose(bookmarks);
	printf("SUCCESS!");
	return 0;
}
