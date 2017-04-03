#include <stdio.h>
#include <stdlib.h>
// Add your system includes here.
#include "ftree.h"
#include "hash.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/socket.h>


int setup(void) {
  int on = 1, status;
  struct sockaddr_in self;
  int listenfd;
  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }

  // Make sure we can reuse the port immediately after the
  // server terminates.
  status = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
                      (const char *) &on, sizeof(on));
  if(status == -1) {
    perror("setsockopt -- REUSEADDR");
  }

  self.sin_family = AF_INET;
  self.sin_addr.s_addr = INADDR_ANY;
  self.sin_port = htons(PORT);
  memset(&self.sin_zero, 0, sizeof(self.sin_zero));  // Initialize sin_zero to 0

  printf("Listening on %d\n", PORT);

  if (bind(listenfd, (struct sockaddr *)&self, sizeof(self)) == -1) {
    perror("bind"); // probably means port is in use
    exit(1);
  }

  if (listen(listenfd, 5) == -1) {
    perror("listen");
    exit(1);
  }
  return listenfd;
}

char* concat(const char *s1, const char *s2)
{
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char *result = malloc(len1+len2+1);
 
    memcpy(result, s1, len1);
    memcpy(result+len1, s2, len2+1);
    return result;
}

int indexfinder(const char* text, const char toFind){
    int index, length;
    index = 0;
    length = strlen(text);
    for (int i = 0; i<length; i ++){
        if (text[i] == toFind){
            index = i;
        }
    }
    return index;
}
/*
 * Returns the FTree rooted at the path fname.
 */

/*
struct TreeNode *generate_ftree(const char *fname) {
    // Your implementation here.
    struct TreeNode *root =(struct TreeNode *) malloc(sizeof(struct TreeNode));
    struct stat  *currentfile = malloc(sizeof(struct stat));
    struct dirent *dir_contents;
    DIR *directory;

//     // if no root exists;
//     if (lstat(fname, currentfile) < 0){
//         printf("lstat: the name '%s' is not a file or directory \n", fname);
//      return root;
//     }
//     // trim fname
//     char *filename = strrchr(fname, '/');
//     if (filename != NULL)
//     {
//             root->fname = strdup(filename + 1);

//     } else {
//             root->fname = strdup(fname);

    }
    root->permissions = currentfile->st_mode & 0777;
    root->hash = NULL;
    root->contents = NULL;
    // if current root is a directory
    if (S_ISDIR(currentfile->st_mode)){
        directory = opendir(fname);
        if (directory == NULL){
            fprintf(stderr, "Directory cannont be opened %s \n", root->fname);
            exit(1);
        }
        struct TreeNode *temp_root = malloc(sizeof(struct TreeNode));
        //if there is no files in directory
        //go through each file
        while ((dir_contents = readdir(directory)) != NULL){
            if (dir_contents->d_name[0] != '.'){ //exclude hidden files
                char *slash = "/";
                if (root->contents == NULL){
                    temp_root =  generate_ftree(concat(concat(fname,slash), dir_contents->d_name)); 
                    root->contents = temp_root;
                }
                else{
                    temp_root -> next = generate_ftree(concat(concat(fname,slash), dir_contents->d_name));
                    temp_root = temp_root->next; 
              }
            }            
        }
        temp_root -> next = NULL;
   } else if (S_ISREG(currentfile->st_mode) || S_ISLNK(currentfile->st_mode)){ // if current root is a link/reg
        FILE *opened_file;
        opened_file = fopen(fname, "rb");
        if (opened_file){
            root->hash = hash(opened_file);
            fclose(opened_file);
            }
        else{
            fprintf(stderr, "File cannont be read %s \n", root->fname);
            exit(1);
            }
        }
        return root;
   }
*/

/*
 * Prints the TreeNodes encountered on a preorder traversal of an FTree.
 */

/*
void print_ftree(struct TreeNode *root) {
    // Here's a trick for remembering what depth (in the tree) you're at
    // and printing 2 * that many spaces at the beginning of the line.
    static int depth = 0;
    // Your implementation here.
    void print_ftree_helper(struct TreeNode *root, int depth);
    print_ftree_helper(root, depth);
}

// void print_ftree_helper(struct TreeNode *root, int depth) {
//     printf("%*s", depth * 2, "");
//     depth += 1;
//     // if node is a directory
//     if (root->hash == NULL && root->fname != NULL){
//         printf("==== %s (%o) ==== \n", root->fname, root->permissions);
//         // loop through all the contents of the root directory
//      while ((root->contents) != NULL){  
//        print_ftree_helper(root->contents, depth);
//        root->contents = root->contents -> next;
//      }
//     // if node is a file/link
//     } else if (root->hash != NULL && root->fname != NULL){
//     printf("%s (%o)\n", root->fname, root->permissions);
//  }
//     return; 
// }
*/
/*
int copy_ftree(const char *src, const char *dest) {
    // if source or destinatoin does not exist
    struct stat *sourcefile = malloc(sizeof(struct stat));
    struct stat *destinationfile = malloc(sizeof(struct stat));
    DIR *directory;
    DIR *src_directory;
    struct dirent *dir_contents;
    //printf("INFO: source : %s   dest : %s \n", src, dest);    

    char *src_name = strrchr(src, '/');
    if( src_name == NULL ){

        src_name = strdup(src);

    } else {

        src_name += sizeof(char);
    }
    //printf("src_name is %s \n", src_name);

    const char *dest_file = concat(concat(dest, "/"), src_name);
    //printf("dest_file is %s \n", dest_file);
    FILE *f1 = fopen(src, "rb");
    FILE *f2 = NULL;
    //printf("INFO: src_name: %s  dest_file: %s \n", src_name, dest_file);
    if (lstat(src, sourcefile) < 0 || lstat(dest, destinationfile) < 0){
        exit(-1);
    }
    // if destination is not a directory, give error
    if (!(S_ISDIR(destinationfile -> st_mode))){
        perror("destination is not a directory.");
        exit(-1);
    } //if source is a file
    if (S_ISREG(sourcefile -> st_mode)){
   // printf("INFO: %s is regular file \n", src);
        bool writefile = true;
        directory = opendir(dest);
	if( directory == NULL ){
	    //printf("failed to open directory.\n");
	    perror("failed to open a directory.\n");
	    exit(-1);

	}
        while((dir_contents = readdir(directory)) != NULL){
	    //printf("currently comparing %s and %s \n", src_name, dir_contents->d_name);
            if (strcmp(dir_contents -> d_name, src_name) == 0){ // if they have the same name           
        	//printf("the two files have the same name \n");
        	struct stat *file2 = malloc(sizeof(struct stat));
                lstat(concat(concat(dest, "/"), dir_contents -> d_name), file2); //uses actual file path
                f2 = fopen(concat(concat(dest, "/"), dir_contents ->d_name), "rb");
                if (f1 && f2){ //succesfully open the files
		    //printf("source file size: %d file 2 size: %d \n", sourcefile->st_size, file2->st_size);
                    if (((sourcefile->st_size == file2->st_size) && (strcmp(hash(f1),hash(f2))== 0))){ // if they have same size and hash
                        writefile = false;
			//printf("file already exists \n");
			
                    }
                } else {
                    perror("Error when opening files of the same name \n");
                    exit(-1);
                }
            fclose(f2);
           }
        }
        if (writefile){
        
	    //printf("writing file\n");
	    f2 = fopen(dest_file, "wb");
        
	    if( f2 == NULL ) {

                perror("destination file is null \n");
		exit(-1);
            } else {

	    //printf("rewinding src file\n");
                rewind(f1);
            //void *buffer = malloc(100);
            //printf("code reached point where it starts to copy file data \n");
                
	    //printf("about to write %s to dest\n", src);
                int curr;
                while((curr = fgetc(f1)) != EOF){
                    //printf("going to write data \n");
                    fwrite(&curr, 1, 1, f2);
                    //printf("wrote some to file \n");

                } 

	    fclose(f1);
	    fclose(f2);

            }
	
        //printf("file was successfully copied \n");
        }
        // change permission of rewrittened file
        
        chmod(dest_file, sourcefile -> st_mode); //uses file path
    //if source is directory
    } else if (S_ISDIR(sourcefile->st_mode)){
        bool makedir = true;
        directory = opendir(dest);
        while((dir_contents = readdir(directory)) != NULL){ // check if directory already exists
            if (dir_contents -> d_name[0] != '.'){
		    //printf("TEST: contents: %s src_name: %s \n", dir_contents->d_name, src_name);
                if ((strcmp(dir_contents -> d_name, src_name) == 0) && (dir_contents -> d_type == DT_DIR)){ // if they have the same name and they are both directories
                    makedir = false;
                    //printf("don't need to make folder %s because %s already exists\n", src_name, dir_contents -> d_name);
                } else if (strcmp(dir_contents -> d_name, src_name) == 0){
                //printf("BAD: %s is the same as %s \n", dir_contents -> d_name, src_name);
                    perror("there is a file in destination that has the same name as source directory but is not a directory");
                    exit(-1);
		}
            }
        }
        closedir(directory);
        if (makedir){
            //printf("making directory %s \n", dest_file);
            mkdir(dest_file, sourcefile ->st_mode);
        }
        src_directory = opendir(src);
        // open source directory and read contents
        directory = opendir(dest_file); //need actual file path
        int parent_forks = 0;
        while ((dir_contents = readdir(src_directory)) != NULL ){ //read all contents in source directory
        //printf("currently on %s \n", dir_contents->d_name);
            if (dir_contents -> d_name[0] != '.'){
                if (dir_contents -> d_type ==  DT_REG){
                    //printf("recursive call on %s in src, regular file \n", dir_contents ->d_name);
                    copy_ftree(concat(concat(src,"/"),dir_contents -> d_name), dest_file ); //need actual file path for both arguements
                } else if (dir_contents -> d_type == DT_DIR){ // if sub directory in source directory is also a directory
                    //printf("recursive call on %s in src, directory \n", dir_contents -> d_name);
                    int result = fork();
                    if (result == -1){
                        perror ("Fork failed");
                        exit(-1);
                    } else if (result == 0){
                        int child_i = copy_ftree(concat(concat(src,"/"),dir_contents -> d_name), dest_file);

                        exit(child_i);
                    } else if (result > 0){
                        parent_forks ++;
                    }
                }
            }
        }

        int child_processes = 0;
	int errors = 0;
        for(int j = 0; j < parent_forks; j++){ //wait 3 times
            pid_t pid;
        int status;
        if ((pid = wait(&status) == -1)){ // if wait command fails
                perror("Wait failed");
		exit(-1);
            } else{
                if (WIFEXITED(status)){ //child terminated
                    child_processes += WEXITSTATUS(status);
		    if(WEXITSTATUS(status) < 0){

			errors++;

		    }
                    //printf("Child %d terminated with %d \n", pid, WEXITSTATUS(status));
                } else if(WIFSIGNALED(status)){ //child recieved signal to terminate
                    //printf("Child %d recieved signal %d to stop \n", pid, WTERMSIG(status));
                } else{
                    perror("undefined wait behaviour occurred");
		    exit(-1);
                }
            }
        }
	//printf("%s -- parent forks: %d sum of child exit codes: %d \n", src, parent_forks, child_processes);    
        if(errors > 0){
		return -(child_processes + 1);

	} else {
	    return child_processes + 1;
        }
    }

    return 1; //exit(0) makes entire program exit causing it to only be able to read one element in a directory
}
*/

int fcopy_client_helper(char *src_path, char *dest_path, char *host, int port, int sock){
	struct fileinfo info;
	FILE *f1;
	DIR *directory;
	struct dirent *dir_contents;
    struct stat *sourcefile = malloc(sizeof(struct stat));
    printf("source path: %s \n", src_path);

    if ((lstat(src_path, sourcefile)) == -1){
    	perror("source doest not exist");
    	exit(1);
    }

    // get absolute path
    char absolute_path[MAXPATH];
    // fill in struct filepath
    strcpy(info.path, concat(concat(dest_path, "/"), src_path));
    printf("absolute src path: %s \n", absolute_path);
    info.mode = sourcefile->st_mode;
    printf("mode: %d \n", info.mode);
    if( S_ISDIR(sourcefile -> st_mode) ){
	info.size = 0;
    } else {
        info.size = (size_t)sourcefile->st_size;
    }
    printf("size: %lu \n", info.size);
    printf("path, mode, size complete \n");
    
    if (S_ISREG(sourcefile -> st_mode)){
    	f1 = fopen(src_path, "rb");
    	strcpy(info.hash ,hash(f1));
	rewind(f1);
	}
    //write to the server
    printf("Client is sending info for %s to server\n", info.path);
    write(sock, &(info.path), MAXPATH * sizeof(char));
    write(sock, &(info.mode), sizeof(mode_t));
    write(sock, &(info.size), sizeof(size_t));
    write(sock, &(info.hash), HASH_SIZE * sizeof(char));
    printf("Client has sent file info for %s \n", info.path);
    // recieve messages match or mismatch
    int storage;
    printf("reading server message\n");

    read(sock, &storage, sizeof(int));
    printf("server message: %d \n", storage);

    if (storage == MISMATCH){ //mismatch
    	if (S_ISREG(sourcefile -> st_mode)){ //sends file data into server side
    		/*
		p = malloc(sizeof(char));
	        *p = NULL;

    		while (p != EOF){
    			p = fgetc(f1);
    		}
    		while (bytes_read > 0){
    			bytes_written = write(sock, f1, sizeof(p)); // reading one byte at a time
    			if (bytes_written <= 0){
    				perror("cannot write file data, transmit error");
    				exit(1);
    			}
    			bytes_read -= bytes_written;
    			p += bytes_written;

    		}
    		free(p);
		*/
		int p[32];
		printf("about to set p\n");
		printf("while loop starting\n");
		/*
		while(p != EOF){
		  printf("reading from file\n");
		  p = fgetc(f1);
		  write(sock, &p, sizeof(p));
		}
		*/
		clearerr(f1);
		while(!feof(f1)){

		  fread(&p, 1, sizeof(p), f1);
		  write(sock, &p, sizeof(p));

		}
		clearerr(f1);
	
		char end_of_file = EOF;
		write(sock, &end_of_file, sizeof(end_of_file));
	
		char carriage = '\r';
		write(sock, &carriage, sizeof(carriage));
		printf("done sending file\n");
        } else {

		printf("error finding file type\n");

	}
        fclose(f1);
    }	
    else if (storage == MATCH_ERROR){ //match error
    	perror("Files are incompatible");
    	exit(1);
    }
    else if (storage == TRANSMIT_ERROR){ //transmit error
    	perror("Transmit error on file %s");
    	exit(1);
    }
    //check if source is a directory
    if (S_ISDIR(sourcefile -> st_mode)){
	printf("opening src directory\n");
    	directory = opendir(src_path);
    	
		if( directory == NULL ){
	    	//printf("failed to open directory.\n");
	    	perror("failed to open a directory.\n");
	    	exit(-1);

		}
		while((dir_contents = readdir(directory)) != NULL){ // recursive calls on all directory contents
			// check if directory starts with "."
			if (dir_contents -> d_name[0] != '.'){
				printf("srcpath: %s destpath: %s \n", src_path, dest_path);
				fcopy_client_helper(concat(concat(src_path,"/"),dir_contents -> d_name), dest_path, host, port, sock);

			}
		}
		closedir(directory);
    }
    return 0;
}

int fcopy_client(char *src_path, char *dest_path, char *host, int port){
	int sock;
	int con;
	int returnvalue;
	struct sockaddr_in peer;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("error when calling socket");
		exit(1);
	}
	// filling in info for servaddr struct
	peer.sin_family = AF_INET;
	peer.sin_port = htons(port);

	// gets address for source 
	if (inet_pton(AF_INET, host ,&peer.sin_addr) == -1){
		perror("inet_pton does not work");
	}
	//
	if ((con = connect(sock, (struct sockaddr *)&peer, sizeof(peer))) == -1){
		perror("client cannot connect");
	}
	
	returnvalue = fcopy_client_helper(src_path, dest_path, host, port, sock);
	return returnvalue;
}

int find_network_nl(char *mesg, int mesg_size){

  for(int i = 0; i < mesg_size ; i++){

     if(mesg[i] == '\r'){

	return i;

     }
  }

  return -1;

}

void rewrite_file(int fd, FILE *overwrite) {
  
  bool over = false;
  char buf[512];
  char *after = buf; //tapehead in buffer
  size_t left = sizeof(buf); //space left in buffer
  int nbytes, where;
  printf("beginning to rewrite file on server\n");
  while(!over){
    //printf("it's not over");
    nbytes = read(fd, after, left);
    
    left -= nbytes;
    after += nbytes;
   
    if( left == 0 ){
	printf("reached end of buffer\n");
      	fwrite(buf, sizeof(char), sizeof(buf), overwrite);
	after = buf;
	left = sizeof(buf);

    }

    where = find_network_nl(buf, sizeof(buf));

    if( where >= 0 ){
      printf("network newline found\n");
      over = true;
      fwrite(buf, sizeof(char), sizeof( &(buf[where-1]) - buf), overwrite);

    }
  }
}

void fcopy_server(int port){

  int listenfd = setup();
  /*
  int *mismatch = &MISMATCH;
  int *match = &MATCH;
  int *matcherror = &MATCH_ERROR;
  int *transmit = &TRANSMIT_OK;
  int *trans_error = &TRANSMIT_ERROR;
  */
  int mesg;

  int fd; //file descriptor
  struct fileinfo current_info;
  struct stat *file_lstat = NULL;
  

  struct sockaddr_in peer; //the socket address into the server
  socklen_t socklen; //length of the socket
 
  socklen = sizeof(peer);

  if((fd = accept(listenfd, (struct sockaddr *)&peer, &socklen)) < 0 ){
 
    perror("error acceptiong");

  } else {

    while(1){

	//done reading struct to current_info
	
	read(fd, &(current_info.path), MAXPATH * sizeof(char));
        printf("recieved path: %s\n", current_info.path);
	read(fd, &(current_info.mode), sizeof(mode_t));
	printf("recieved mode: %d\n", current_info.mode);
	read(fd, &(current_info.size), sizeof(size_t));
	printf("recieved size: %lu\n", current_info.size);
	read(fd, &(current_info.hash), HASH_SIZE * sizeof(char));
        printf("recieved all info for %s \n", current_info.path);

	if(lstat(current_info.path, file_lstat) == -1){ //doesn't exist

	  printf("File doesn't exist on server\n");
	  if( current_info.size == 0){ //is directory

	    printf("File is a directory\n");
	    mkdir(current_info.path, current_info.mode);	
	    mesg = MATCH;
	    write(fd, &mesg, sizeof(int));

	  } else { //is file

	    printf("File is a file\n");
	    mesg = MISMATCH;
	    write(fd, &mesg, sizeof(int));
	    FILE *overwrite = fopen( current_info.path, "wb");

	    //rewriting file
	    printf("starting to write file on server\n");
	    rewrite_file(fd, overwrite);
	    fclose(overwrite);
	    mesg = TRANSMIT_OK;
	    write(fd, &mesg, sizeof(int));
	    printf("file written to server");

	  }

	} else if ( S_ISREG(current_info.mode) ){ //exists as file

	  FILE *check_hash = fopen(current_info.path, "rb" );
	  
	  char *local_hash = hash(check_hash);
	  fclose(check_hash);
	
	  if(strcmp(local_hash, current_info.hash) != 0){

	    //rewrite files
	    mesg = MISMATCH;
	    write(fd, &mesg, sizeof(int));
	    FILE *overwrite = fopen( current_info.path, "wb");
	    rewrite_file(fd, overwrite);
	    fclose(overwrite);
	    mesg = TRANSMIT_OK;
	    write(fd, &mesg, sizeof(int));

	  }	
      } else { //exists as folder

	chmod(current_info.path, current_info.mode);
	mesg = MATCH;
	write(fd, &mesg, sizeof(int));

      }

    }
  }
}



