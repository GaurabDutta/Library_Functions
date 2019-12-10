#include <stdio.h>
#include <sys/stat.h> 
#include <sys/syscall.h> 
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h> 

#define _GNU_SOURCE
#define SUCCESS											 0
#define FILE_ERROR										-1
#define ALL_RENAME_OPERATION_FAILED						 1
#define SOME_RENAME_FAILED								 2
#define NOTHING_RENAMED									 4
#define UNANTICIPATED_ERROR								 64
//#define __NR_copy_file_range 							326

void File_Info (char file_name [], char file_path [], struct stat *file_info);
int File_Delete (char *file_name, char *file_path);
int Rename_File (char *file_name, char *file_path, char *new_file_name);
int Find_File (char *file_name, char *file_path, struct stat *file_info);
int Copy_File (char *source_file, char *source_path, char *destination_file, char *destination_path);
FILE *File_Create (char *file_name, char *file_path, char *permission);

void File_Info (char file_name [], char file_path [], struct stat *file_info)
{
	int ret = -1;
	char file[1024];
	
//	file_info = (struct stat *) malloc (sizeof (struct stat));
	
	memset (file, '\0', sizeof(file));
	snprintf (file, sizeof (file), "%s/%s", file_path, file_name);
		
	if ((ret = stat (file, file_info)) == 0)
	{
		printf ("File st_uid\t:- %d \n",file_info->st_uid);
		printf ("File st_blksize\t:- %ld \n",file_info->st_blksize);
		printf ("File st_gid\t:- %d \n",file_info->st_gid);
		printf ("File st_blocks\t:- %ld \n",file_info->st_blocks);
		printf ("File st_size\t:- %ld \n",file_info->st_size);
		printf ("File st_nlink\t:- %u \n",(unsigned int)file_info->st_nlink);
		printf ("File Permissions for User\n");
		printf ((file_info->st_mode & S_IRUSR)? "r":"-");
		printf ((file_info->st_mode & S_IWUSR)? "w":"-");
		printf ((file_info->st_mode & S_IXUSR)? "x":"-");
		printf ("\nFile Permissions for Group\n");
		printf ((file_info->st_mode & S_IRGRP)? "r":"-");
		printf ((file_info->st_mode & S_IWGRP)? "w":"-");
		printf ((file_info->st_mode & S_IXGRP)? "x":"-");
		printf ("\nFile Permissions for Other\n");
		printf ((file_info->st_mode & S_IROTH)? "r":"-");
		printf ((file_info->st_mode & S_IWOTH)? "w":"-");
		printf ((file_info->st_mode & S_IXOTH)? "x":"-");
		printf ("\n");
	}
	else 
		printf ("\n*** File NOT FOUND ***\n");
		
	return;
}

int File_Delete (char *file_name, char *file_path)
{ 
	int ret = -1;
	char file [1024]; 
	struct stat file_info;
	
	snprintf (file, sizeof (file), "%s/%s", file_path, file_name);
	printf ("File path = %s\n", file);
	
	if ((ret = stat (file, &file_info)) == 0)
	{
		printf ("File Found\n");
		
		if (remove(file) == SUCCESS)
		{ 
			printf("\nDeleted successfully\n"); 
			return SUCCESS;
		}
		
		else
		{
			printf("\nUnable to Delete the file\n"); 
			return FILE_ERROR;
		} 
	}
	
	else
	{
		printf ("File NOT FOUND\n");
		return FILE_ERROR;	
	}
} 

int Rename_File (char *file_name, char *file_path, char *new_file_name)
{
	int ret = -1;
	char file_1 [1024], file_2 [1024]; 
	struct stat *file_info;
	
	file_info = (struct stat *) malloc (sizeof (struct stat));
	ret = Find_File (file_name, file_path, file_info);
	
	if (ret == SUCCESS)
	{
		printf ("\nFile FOUND in the path - %s\n", file_path);
		
		snprintf (file_1, sizeof (file_1), "%s/%s", file_path, file_name);
		printf ("File path = %s\n", file_1);
		
		if ((ret = stat (file_1, file_info)) != 0)
		{
			printf ("File NOT FOUND\n");
			return FILE_ERROR;
		}	
		
		snprintf (file_2, sizeof (file_2), "%s/%s", file_path, new_file_name);
		
		if ((ret = rename(file_1, file_2)) == SUCCESS)
		{ 
			printf("\n*** Renamed Successfully ***\n"); 
			return ret;
		}
		else
		{
			printf("\n*** Unable to Rename the file ***\n"); 	
			return ret;
		} 
	}
	else
	{
		printf ("\n*** Source File NOT FOUND in the given PATH ***\n");
		return FILE_ERROR;
	}
}

int Find_File (char *file_name, char *file_path, struct stat *file_info)
{
	int ret = 0;
	static int flag = 0;
	char path [1024]; 
	char file [1024];
	struct dirent *directory_entry; 							// Pointer for directory entry 

	DIR *dir = opendir(file_path); 								// opendir() returns a pointer of DIR type. 
	
	memset (path, '\0', sizeof (path));
	memset (file, '\0', sizeof (file));
	
	memcpy (path, file_path, sizeof (path));

	if (dir == NULL) 											// opendir returns NULL if couldn't open directory 
	{ 
		printf("\n****\tCould not open current directory\t****\n" ); 
		return FILE_ERROR; 
	} 

	while (((directory_entry = readdir(dir)) != NULL) && (flag != 1))			// for readdir() 
	{ 
		if (directory_entry->d_type == DT_DIR) 
        {
            if (strcmp(directory_entry->d_name, ".") == 0 || strcmp(directory_entry->d_name, "..") == 0)
                continue;
           
			printf ("\nDirectory Name\t:- %s\n", directory_entry->d_name);
			
			snprintf(path, sizeof(path), "%s/%s", file_path, directory_entry->d_name);
            Find_File(file_name, path, file_info);
        } 
        
        else 
        {
			snprintf(file, sizeof(file), "%s/%s", file_path, file_name);
			
			if ((ret = stat (file, file_info)) == 0)
			{
				printf ("\nFile FOUND :-\t%s\n", file_name);
				printf ("File Path :-\t%s\n", file_path);
				File_Info (file_name, file_path, file_info); 
				memcpy (path, file_path, sizeof (path));
				
				flag = 1;
				break;
			}
        }	
	}		

	closedir(dir);	
	memcpy (file_path, path, sizeof (path));
	
	if (flag)
		return SUCCESS;
	else
		return FILE_ERROR;
}

static loff_t copy_file_range(int fd_in, loff_t *off_in, int fd_out, loff_t *off_out, size_t len, unsigned int flags) 
{
    return syscall(__NR_copy_file_range, fd_in, off_in, fd_out, off_out, len, flags); 
}

int Copy_File (char *source_file, char *source_path, char *destination_file, char *destination_path)
{
	int ret = -1;
	int fd_in, fd_out;
	struct stat *file_info;
	loff_t file_size;
	char destin_path [1024];
	char file [1024];
	
	file_info = (struct stat *) malloc (sizeof (struct stat));
	
	ret = Find_File (source_file, source_path, file_info);		//Checking the Availability of the Source File
	
	if (ret == SUCCESS)
	{
		printf ("\nFile FOUND in the path - %s\n", source_path);
		
		memset (file, '\0', sizeof (file));
		snprintf (file, sizeof (file), "%s/%s", source_path, source_file);
		
		fd_in = open (file, O_RDONLY);
		printf ("fd_in = %d\n", fd_in);
		
		if (fd_in == -1)
		{
			printf ("File %s COULD NOT BE OPENED for Reading\n", source_file);
			return FILE_ERROR;
		}
		else
		{
			file_size = file_info->st_size;					//Copying the Source file size
			
			snprintf (file, sizeof (destin_path), "%s/%s", destination_path, destination_file);
			printf ("Destination file = %s\n", file);
			
			fd_out = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if (fd_out == -1)
			{
				printf ("Destination file %s COULD NOT BE CREATED\n", destination_file);
				return FILE_ERROR;
			}
			else
			{
				while (file_size > 0)
				{
					ret = copy_file_range(fd_in, NULL, fd_out, NULL, file_size, 0);		//copying from Source to Destination
					if (ret == -1)
					{
						printf ("\n*** Copy Function Failed ***\n");
						return FILE_ERROR;
					}
					
					file_size = file_size - ret;
				}
			}
			
			close(fd_in);
			close(fd_out);
			
			return SUCCESS;
		}
	}
	else
	{
		printf ("\n*** Source File NOT FOUND in the given PATH ***\n");
		return FILE_ERROR;
	}	
}

FILE *File_Create (char *file_name, char *file_path, char *permission)
{
	char file [1024];
	FILE *fp1 = NULL;
	
	snprintf (file, sizeof (file), "%s/%s", file_path, file_name);
	printf ("file = %s\n", file);
	
	fp1 = fopen (file, permission);
	
	if (fp1 != NULL)
	{
		printf ("\n*** File Got Created ***\n");
		return fp1;
	}
	else
	{
		printf ("\n*** File NOT Created ***\n");
		
		return NULL;
	}
}