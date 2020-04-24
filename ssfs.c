#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>



static const char *dirpath = "/home/bluejake3/Documents";

char key[90] = "9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO";
int ges = 10;

void encrypt(char* enc)
{
	//if(!strcmp(enc,".") || !strcmp(enc,"..")) return;
	
	for(int i=0;i<strlen(enc);i++)
	{
		for(int j=0;j<strlen(key);j++){
			if(enc[i]==key[j]){
				enc[i] = key[(j+ges)%strlen(key)];
				break;
			}
		}
	}
}

void decrypt(char* dec)
{
	//if(!strcmp(dec,".") || !strcmp(dec,"..")) return;
	for(int i=0;i<strlen(dec);i++)
	{
		for(int j=0;j<strlen(key);j++){
			if(dec[i]==key[j]){
				dec[i] = key[(j+strlen(key)-ges)%strlen(key)];
				break;
			}
		}
	}
}

int caesar (char *path, int flag) {
    DIR *dp;
    struct dirent *de;
    char f[1000];

    dp = opendir(path);
    if (dp == NULL){
      return -errno;
    }

    while ((de = readdir(dp)) != NULL) {

        struct stat st;
        memset(&st, 0, sizeof(st));
        strcpy(f, de->d_name);

        char fpath1[1000],fpath2[1000];
        sprintf(fpath1, "%s/%s", path, f);

        if (!strcmp(".", de->d_name) && !strcmp("..", de->d_name) && de->d_type == DT_DIR){
            char folder[1000];
            sprintf(folder, "%s/%s", path, f);

            if (flag == 1) {
                encrypt(f);
                caesar(folder, 1);
            }
            else {
                decrypt(f);
                caesar(folder, 0);
            }

            sprintf(fpath2, "%s/%s", path, f);

            int res = rename(fpath1, fpath2);
            if (res == -1){
              return -errno;
            }
        }

        if (de->d_type == DT_REG) {
        	char *ext = strrchr(f, '.');
          
          	if (ext == NULL) {
            	if (flag == 1){
              		encrypt(f);
            	}
            	else{
              		decrypt(f);
            	}
            	sprintf(fpath2, "%s/%s", path, f);
            }
            else {
                char noext[ext - f];
                strncpy(noext, f, ext - f);

                noext[ext - f] = '\0';
                if (flag == 1){
                  encrypt(noext);
                }
                else{
                  decrypt(noext);
                }

                sprintf(noext, "%s%s", noext, ext);
                sprintf(fpath2, "%s/%s", path, noext);
            }
            
            int res = rename(fpath1, fpath2);
            if (res == -1){
              	return -errno;
            }
        }
    }
    closedir(dp);
    return 0;
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
	int res;
    char fpath[1000];
    
	sprintf(fpath, "%s%s",dirpath,path);
	
	res = lstat(fpath, stbuf);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;
	
    char fpath[1000];
    
    if(strcmp(path,"/") == 0) {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
	else 
		sprintf(fpath, "%s%s",dirpath,path);
	
	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
        
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
			
		if (filler(buf, de->d_name, &st, 0))
			break;
	}

	closedir(dp);
	return 0;
}


static int xmp_mkdir(const char *path, mode_t mode)
{
	
    char fpath[1000];
    
	sprintf(fpath, "%s%s",dirpath,path);
	
	int res = mkdir(fpath, mode);
	if (res == -1)
		return -errno;
	
	return 0;
}

static int xmp_rename(const char *from, const char *to)
{
	int res;
    char new_from[1000];
    char new_to[1000];

    sprintf(new_from,"%s%s",dirpath,from);
    sprintf(new_to,"%s%s",dirpath,to);
    
    if (strncmp("/encv1_", to, 7) == 0) {
        caesar(new_from, 1); 
    }
    
    if (strncmp("/encv1_", from, 7) == 0) {
        caesar(new_from, 0);
    }
    
	res = rename(new_from, new_to);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
	int fd;
	int res;
    char fpath[1000];
    
    if(strcmp(path,"/") == 0) {
        path=dirpath;
        sprintf(fpath, "%s", path);
    }
    else
		sprintf(fpath, "%s%s",dirpath,path);
	
	(void) fi;
	
	fd = open(fpath, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static struct fuse_operations xmp_oper = 
{
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.mkdir		= xmp_mkdir,
	.rename		= xmp_rename,
	.read		= xmp_read,
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}
