#include <stdio.h>
#include <stdlib.h>
#include <ftw.h>
#include <unistd.h>
#include <linux/reboot.h>
#include <sys/reboot.h>
#include "File_Info.h"
#include "Directory_Info.h"
#include "Network_info.h"
#include "Memory_info.h"

int main ()
{
	int return_value = 0, choice = 0, config = 0;
	char file_name [1024];
	char file_name_2 [1024];
	char file_path [1024];
	char file_path_2 [1024];
	char ip[1024];
	char netmask[1024];
	char gateway[1024];
	char if_name[1024];

	struct stat *file_stat;
	struct sysinfo *mem_info;
	FILE *fp = NULL;
	
	file_stat = (struct stat *) malloc (sizeof (struct stat));
	mem_info = (struct sysinfo *) malloc (sizeof (struct sysinfo));
	
	memset (file_path, '\0', sizeof (file_path));
	memset (file_name, '\0', sizeof (file_name));
	memset (file_name_2, '\0', sizeof (file_name_2));
	
	getcwd (file_path, 1024);
	printf ("PWD = %s\n", file_path);
	
	printf ("\nSelect from the below options:-\n");
	printf ("1. Get the Direcotry Info\n");
	printf ("2. Delete File From the given Path\n");
	printf ("3. Rename a File in the given path\n");
	printf ("4. Find the File in the given Path\n");
	printf ("5. Copy a File's Data into a New File\n");
	printf ("6. Create a File\n");
	printf ("7. Delete Directory\n");
	printf ("8. Rename a Directory\n");
	printf ("9. Create a Directory\n");
	printf ("10. Find the Directory\n");
	printf ("11. Copy directory\n");
	printf ("12. Move directory\n");
	printf ("13. Get IP\n");
	printf ("14. Set IP\n");
	printf ("15. Configure IP\n");
	printf ("16. Get Memory Info\n");
	printf ("17. Reboot Device\n");
	
	printf ("\nEnter your Choice:- ");
	scanf ("%d", &choice);
	printf ("\n");

	switch (choice)
	{
		case 1:
				//Function to get the File and Directory Details
				memset (file_path, '\0', sizeof (file_path));
				printf ("Enter the Path of the Directory:- ");
				scanf ("%s", file_path);
				
				get_directory_info (file_path);	
		break;
				
		case 2:		
				// Deleting a File from the given path
				memset (file_path, '\0', sizeof (file_path));
				memset (file_name, '\0', sizeof (file_name));
				
				printf ("Enter the file name to Delete:- ");
				scanf ("%s", file_name);
				
				printf ("Enter the Path of the file:- ");
				scanf ("%s", file_path);
				
				return_value = File_Delete (file_name, file_path);
				printf ("return_value = %d\n", return_value);
		break;

		case 3:
				// Renaming a File from the given path
				memset (file_path, '\0', sizeof (file_path));
				memset (file_name, '\0', sizeof (file_name));
				
				printf ("\nEnter the file name to Rename:- ");
				scanf ("%s", file_name);
				
				printf ("\nEnter the New file name to Rename with:- ");
				scanf ("%s", file_name_2);
				
				printf ("Enter the Path of the file:-");
				scanf ("%s", file_path);
				
				return_value = Rename_File (file_name, file_path, file_name_2);
				printf ("return_value = %d\n", return_value);
		break;
		
		case 4:
				//Find a File in the Directory
				memset (file_path, '\0', sizeof (file_path));
				memset (file_name, '\0', sizeof (file_name));
				
				printf ("Enter the file name to Find:- ");
				scanf ("%s", file_name);
				
				printf ("Enter the Path of the file:- ");
				scanf ("%s", file_path);
				
				return_value = Find_File (file_name, file_path, file_stat);
				printf ("return_value = %d\n", return_value);
		break;
		
		case 5:
				//File Copy Operation
				memset (file_path, '\0', sizeof (file_path));
				memset (file_name, '\0', sizeof (file_name));
				memset (file_path_2, '\0', sizeof (file_path_2));
				memset (file_name_2, '\0', sizeof (file_name_2));
				
				printf ("Enter the Source File Name:- ");
				scanf ("%s", file_name);
				printf ("Enter the Source File Path:- ");
				scanf ("%s", file_path);
				
				printf ("Enter the Destination File Name:- ");
				scanf ("%s", file_name_2);
				printf ("Enter the Destination File Path:- ");
				scanf ("%s", file_path_2);
				
				return_value = Copy_File (file_name, file_path, file_name_2, file_path_2);
				printf ("Return Value = %d\n", return_value);		
		break;		
		
		case 6:
				//Create File Function
				memset (file_path, '\0', sizeof (file_path));
				memset (file_name, '\0', sizeof (file_name));
				memset (file_name_2, '\0', sizeof (file_name_2));
				
				printf ("Enter the File name:- ");
				scanf ("%s", file_name);
				
				printf ("Enter the Path:- ");
				scanf ("%s", file_path);
				
				printf ("Permission for the File (r/w/w+/a/a+):- ");
				scanf ("%s", file_name_2);
				
				fp = File_Create (file_name, file_path, file_name_2); 
				printf ("Return Value = %p\n", fp);
				
				if (fp)
				{
					fclose (fp);
				}
		break;
		
		case 7:
				//Delete Directory
				memset (file_path, '\0', sizeof (file_path));
				
				printf ("Enter the Directory Path:- "); 
				scanf ("%s", file_path);
				
				return_value = Delete_Directory (file_path);
				printf ("Return Value = %d\n", return_value);
		break;		

		case 8:
				//Rename A directory
				memset (file_path, '\0', sizeof (file_path));
				memset (file_path_2, '\0', sizeof (file_path_2));
				
				printf ("Enter the Source Directory Path:- ");
				scanf ("%s", file_path);
				printf ("Enter the New Directory Name:- ");
				scanf ("%s", file_path_2);
				
				return_value = Rename_Directory (file_path, file_path_2);
				printf ("Return Value = %d\n", return_value);
		break;
		
		case 9:
				//Create a New Directory
				memset (file_path, '\0', sizeof (file_path));
				memset (file_name, '\0', sizeof (file_name));
				
				printf ("Enter the Directory Path:- ");
				scanf ("%s", file_path);
				printf ("Enter the New Directory Name:- ");
				scanf ("%s", file_name);
				
				return_value = Create_Directory (file_path, file_name);
				printf ("Return Value = %d\n", return_value);
		break;	
		
		case 10:
				//Find the Directory
				memset (file_path, '\0', sizeof (file_path));
				memset (file_name, '\0', sizeof (file_name));
				
				printf ("Enter the Source Directory Path:- ");
				scanf ("%s", file_path);
				printf ("Enter the Directory Name:- ");
				scanf ("%s", file_name);
				
				return_value = Find_Directory (file_path, file_name);
				printf ("Return Value = %d\n", return_value);
		break;		

		case 11:
				//Copy Directory
				memset (file_path, '\0', sizeof (file_path));
				memset (file_path_2, '\0', sizeof (file_path_2));

				printf ("Enter Source directory path: ");
				scanf ("%s", file_path);
				printf ("Enter Destination path: ");
				scanf ("%s", file_path_2);

				return_value = Copy_Directory (file_path, file_path_2);
				printf ("Return Value = %d\n", return_value);
		break;

		case 12:
				//Move Directory
				memset (file_path, '\0', sizeof (file_path));
				memset (file_path_2, '\0', sizeof (file_path_2));

				printf ("Enter Source directory path: ");
				scanf ("%s", file_path);
				printf ("Enter Destination path: ");
				scanf ("%s", file_path_2);

				return_value = Move_Directory (file_path, file_path_2);
				printf ("Return Value = %d\n", return_value);
		break;

		case 13:
				//Get IP
				memset (if_name, '\0', sizeof (if_name));

				return_value = get_ip();
				printf ("Return Value = %d\n", return_value);
		break;

		case 14:
				//Set_IP
				memset (ip, '\0', sizeof (ip));
				memset (netmask, '\0', sizeof (netmask));
				memset (gateway, '\0', sizeof (gateway));

				printf("Enter IP: ");
				scanf("%s", ip);
				printf("Enter Netmask: ");
				scanf("%s", netmask);
				printf("Enter Gateway: ");
				scanf("%s", gateway);
				
				return_value = set_static_ip(ip,netmask,gateway);
//				return_value = set_static_ip(NULL,NULL,NULL);
				printf ("Return Value = %d\n", return_value);
		break;

		case 15:
				//Configure network
				return_value = configure_ip();
				printf ("Return Value = %d\n", return_value);
		break;

		case 16:
				//Memory Information
				return_value = memory_info(mem_info);
				printf ("\nReturn Value = %d\n", return_value);
		break;

		case 17:
				//Reboot Device
				printf("Rebooting Device\n");
//				reboot(RB_POWER_OFF);
				sync();
	  			reboot(LINUX_REBOOT_CMD_RESTART);
		break;		

		default:
				printf ("Enter proper Value\n");
		break;
	}
	
	return 0;
}
