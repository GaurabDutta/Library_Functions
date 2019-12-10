/************************************************
* @file         Directory_Info.h
* @brief        Directory Information
* @details      This is a function to Get a Given Directory's Information
* @author       Mahesh
***********************************************/

#include <stdio.h> 
#include <dirent.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ftw.h>

#define _XOPEN_SOURCE 			500
#define SUCCESS					0
#define DIRECTORY_ERROR			-1

#define BUFFERSIZE				1024
#define COPYMORE				0644

void oops(char *, char *);
int copyDir(char *src, char *dest);
int copyFiles(char *src, char *dest);
int dostat(char *filename);

/************************************************
* @proc   		get_directory_info
* @brief   		Directory Information
* @detail  		detailed intro
***********************************************/

void get_directory_info (char *dir_path)
{
	char path [1024]; 
	struct dirent *directory_entry; 							/* Pointer for directory entry */ 
	struct stat *file_stat;
	file_stat = (struct stat *) malloc (sizeof (struct stat));

	DIR *dir = opendir(dir_path); 								// opendir() returns a pointer of DIR type. 
	memcpy (path, dir_path, sizeof (path));

	if (dir == NULL) 											// opendir returns NULL if couldn't open directory 
	{ 
		printf("\n****\tCould not open current directory\t****\n" ); 
		return; 
	} 

	while ((directory_entry = readdir(dir)) != NULL)			// for readdir() 
	{ 
		printf("directory_entry->d_type=%c\n",directory_entry->d_type);
		if (directory_entry->d_type == DT_DIR) 
        {
            if (strcmp(directory_entry->d_name, ".") == 0 || strcmp(directory_entry->d_name, "..") == 0)
                continue;
           
		   	printf("\nDirectory name: %s\n",directory_entry->d_name);
			snprintf(path, sizeof(path), "%s/%s", dir_path, directory_entry->d_name);
            get_directory_info(path);
        }
        else 
        {
			printf("\nFile name: %s\n",directory_entry->d_name);
            File_Info (directory_entry->d_name, dir_path, file_stat); 
        }
	}		

	closedir(dir);	 
	return; 
}

int Delete_Directory (char *dir_path)
{
	int ret = -1;
	char path [1024];
	struct dirent *directory_entry; 							/**< Pointer for directory entry */ 
	struct stat *file_stat;

	DIR *dir = opendir(dir_path); 								// opendir() returns a pointer of DIR type. 

	if (dir == NULL) 											// opendir returns NULL if couldn't open directory 
	{ 
		printf("\n****\tCould NOT OPEN the DIRECTORY\t****\n" ); 
		return DIRECTORY_ERROR; 
	}
	else 
	{
		while ((directory_entry = readdir(dir)) != NULL)			// for readdir() 
		{ 
			if (directory_entry->d_type == DT_DIR) 
			{
				if (strcmp(directory_entry->d_name, ".") == 0 || strcmp(directory_entry->d_name, "..") == 0)
					continue;
			   
				printf ("\nDirectory Name:-\t%s\n", directory_entry->d_name);
				snprintf(path, sizeof(path), "%s/%s", dir_path, directory_entry->d_name);
				printf ("path from directory 1 - %s\n", path);
				ret = Delete_Directory(path);
			} 
			else 
			{
				printf ("\nFile Name :-\t%s\n", directory_entry->d_name);
				printf ("path from directory 2 - %s\n", dir_path);
				ret = File_Delete (directory_entry->d_name, dir_path);
			}
		}
		
		closedir(dir);	
		
		if (!ret) 
			ret = rmdir (dir_path);
		
		return ret; 		
	} 
}

int Rename_Directory (char *dir_path, char *new_dir_path)
{
	int ret = -1;
	char new_dir [1024], *chr_ptr;
	struct dirent *directory_entry;
	
	memset (new_dir, '\0', sizeof (new_dir));
	
	DIR *dir = opendir(dir_path); 								// opendir() returns a pointer of DIR type. 

	if (dir == NULL) 											// opendir returns NULL if couldn't open directory 
	{ 
		printf("\n****\tCould NOT OPEN the DIRECTORY\t****\n" ); 
		return DIRECTORY_ERROR; 
	}
	
	else 
	{
		chr_ptr = strrchr (dir_path, '/');
		ret = (chr_ptr - dir_path) + 1;
		
		memcpy (new_dir, dir_path, ret);
		strcat (new_dir, new_dir_path);
		
		printf ("New Directory Path = %s\n", new_dir);
		
		if ((ret = rename(dir_path, new_dir)) == SUCCESS)
		{ 
			printf("\n*** Renamed Successfully ***\n"); 
			return ret;
		}
		else
		{
			printf("\n*** Unable to Rename the Directory ***\n"); 
			return ret;
		}
	}	
}

int Create_Directory (char *dir_path, char *dir_name)
{
	int ret = -1;
	char new_dir [1024];
	
	snprintf (new_dir, sizeof (new_dir), "%s/%s", dir_path, dir_name);
	printf ("New Directory name = %s\n", new_dir);
	
	if ((ret = mkdir (new_dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) == SUCCESS)
	{ 
		printf("\n*** Directory Created Successfully ***\n"); 
		return ret;
	}
	else
	{
		printf("\n*** Unable to Create the Directory ***\n"); 
		return ret;
	}
}

int Find_Directory (char *dir_path, char *dir_name)
{
	static int flag = 0;
	char path [1024]; 
	struct dirent *directory_entry; 							/* Pointer for directory entry */ 
	struct stat *file_stat;

	DIR *dir = opendir(dir_path); 								// opendir() returns a pointer of DIR type. 
	memcpy (path, dir_path, sizeof (path));

	if (dir == NULL) 											// opendir returns NULL if couldn't open directory 
	{ 
		printf("\n****\tGIVEN PATH DOES NOT EXIST\t****\n" ); 
		return DIRECTORY_ERROR; 
	} 

	while (((directory_entry = readdir(dir)) != NULL) && (flag == 0))			// for readdir() 
	{ 
		if (directory_entry->d_type == DT_DIR) 
        {
            if (strcmp(directory_entry->d_name, ".") == 0 || strcmp(directory_entry->d_name, "..") == 0)
                continue;
			
			if ((strcmp (directory_entry->d_name, dir_name)) == 0)
			{
				printf ("\nDIRECTORY IS FOUND in the path = %s\n", dir_path);
				
				memcpy (path, dir_path, sizeof (path));
				
				flag = 1;
				break;
			}
			snprintf(path, sizeof(path), "%s/%s", dir_path, directory_entry->d_name);
            Find_Directory(path, dir_name);
        } 
	}		

	closedir(dir);	 	
	memcpy (dir_path, path, sizeof (path));
	
	if (flag)
		return SUCCESS;
	else
		return DIRECTORY_ERROR;
}

int Copy_Directory(char *source, char *destination)
{
    int ret;
	char tempDest[1024];
    char tempSrc[1024];

	strcat(destination, "/");
    strcat(source, "/");

	memset(tempDest,'\0',sizeof(tempDest));
	memset(tempSrc,'\0',sizeof(tempSrc));

    strcpy(tempDest, destination);
    strcpy(tempSrc, source);

    struct dirent *directory_entry; 							/* Pointer for directory entry */ 
	struct stat *file_stat;
	file_stat = (struct stat *) malloc (sizeof (struct stat));

	DIR *dir = opendir(source);
	DIR *dir_dest = opendir(destination);
//	memcpy (tempSrc, source, sizeof (tempSrc));

	if((dir_dest = opendir(destination)) == NULL)
	{
		if((ret = mkdir (destination, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) == SUCCESS)
			printf("\n*** Directory Created Successfully ***\n");
		else
			printf("\n*** Unable to Create Directory ***\n");
	}

    if( dir == NULL )
    {
        printf("\n****\tCould not open current directory\t****\n" );
		fprintf(stderr, "cp1: cannot open %s for copying\n", source);
        return 0;
    }
	else
	{
	    while((directory_entry = readdir(dir)) != NULL)
	    {
	        if (strcmp(directory_entry->d_name, ".") == 0 || strcmp(directory_entry->d_name, "..") == 0)
				continue;
			if(dostat(directory_entry->d_name))  
	        {
	            strcat(tempDest, directory_entry->d_name);        
	            strcat(tempSrc, directory_entry->d_name);

				if (directory_entry->d_type == DT_DIR) 
				{
				    if (strcmp(directory_entry->d_name, ".") == 0 || strcmp(directory_entry->d_name, "..") == 0)
				        continue;
//				    snprintf(tempSrc, sizeof(tempSrc), "%s/%s", source, directory_entry->d_name);
//					snprintf(tempDest, sizeof(tempDest), "%s/%s", destination, directory_entry->d_name);
					Copy_Directory (tempSrc, tempDest);
				} 
				else
					copyFiles(tempSrc, tempDest);

				strcpy(tempDest, destination);
	            strcpy(tempSrc, source);            
	        }
	    }
	}
	closedir(dir);
    return 1;
}

int Move_Directory(char *source, char *destination)
{
    int ret;
	char tempDest[1024];
    char tempSrc[1024];

	strcat(destination, "/");
    strcat(source, "/");

	memset(tempDest,'\0',sizeof(tempDest));
	memset(tempSrc,'\0',sizeof(tempSrc));

    strcpy(tempDest, destination);
    strcpy(tempSrc, source);

    struct dirent *directory_entry; 							/* Pointer for directory entry */ 
	struct stat *file_stat;
	file_stat = (struct stat *) malloc (sizeof (struct stat));

	DIR *dir = opendir(source);
	DIR *dir_dest = opendir(destination);
//	memcpy (tempSrc, source, sizeof (tempSrc));

	if((dir_dest = opendir(destination)) == NULL)
	{
		if((ret = mkdir (destination, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) == SUCCESS)
			printf("\n*** Directory Created Successfully ***\n");
		else
			printf("\n*** Unable to Create Directory ***\n");
	}

    if( dir == NULL )
    {
        printf("\n****\tCould not open current directory\t****\n" );
		fprintf(stderr, "cp1: cannot open %s for copying\n", source);
        return 0;
    }
	else
	{
	    while((directory_entry = readdir(dir)) != NULL)
	    {
	        if (strcmp(directory_entry->d_name, ".") == 0 || strcmp(directory_entry->d_name, "..") == 0)
				continue;
			if(dostat(directory_entry->d_name))  
	        {
	            strcat(tempDest, directory_entry->d_name);        
	            strcat(tempSrc, directory_entry->d_name);

				if (directory_entry->d_type == DT_DIR) 
				{
				    if (strcmp(directory_entry->d_name, ".") == 0 || strcmp(directory_entry->d_name, "..") == 0)
				        continue;
//				    snprintf(tempSrc, sizeof(tempSrc), "%s/%s", source, directory_entry->d_name);
//					snprintf(tempDest, sizeof(tempDest), "%s/%s", destination, directory_entry->d_name);
					Copy_Directory (tempSrc, tempDest);
				} 
				else
					copyFiles(tempSrc, tempDest);

				strcpy(tempDest, destination);
	            strcpy(tempSrc, source);            
	        }
	    }
	}

	closedir(dir);
	Delete_Directory(source);
    return 1;
}

int dostat(char *filename)
{
    struct stat fileInfo;

    if(stat(filename, &fileInfo)>=0)
    if(S_ISREG(fileInfo.st_mode) || S_ISDIR(fileInfo.st_mode))
        return 1;
    else 
        return 0;
}

int copyFiles(char *source, char *destination)
{
    int in_fd, out_fd, n_chars;
    char buf[BUFFERSIZE];   
    
	/* open files */
    if( (in_fd=open(source, O_RDONLY)) == -1 )
    {
        oops("Cannot open ", source);
    }

    if((out_fd=creat(destination, COPYMORE)) == -1)
    {
        oops("Cannot create ", destination);
    } 
    /* copy files */
    while((n_chars = read(in_fd, buf, BUFFERSIZE)) > 0)
    {
        if(write(out_fd, buf, n_chars) != n_chars)
        {
            oops("Write error to ", destination);
        } 
        if( n_chars == -1 )
        {
            oops("Read error from ", source);
        }
    }   
    /* close files */
    if( close(in_fd) == -1 || close(out_fd) == -1 )
    {
        oops("Error closing files", "");
    }
    return 1;
}

void oops(char *s1, char *s2)
{
    fprintf(stderr, "Error: %s ", s1);
    perror(s2);
    exit(1);
}
