#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char ** argv){
    printf("Test %d\n", argc);
    int i;
    for(i = 0; i < argc; i++){
        printf("%d: %s\n", i, argv[i]);
    }
    struct timespec tp;
    if(clock_gettime(CLOCK_REALTIME, &tp) == 0){
        printf("clock_gettime %d %d\n", (int)tp.tv_sec, (int)tp.tv_nsec);
        printf("ctime %s", ctime(&tp.tv_sec));
        void* test = malloc(1024*1024);
        if(test > 0){
            printf("Malloc %p\n", test);
            free(test);
            printf("Free\n");

            DIR * dir = opendir("/home/");
            if (dir != NULL) {
                printf("opendir %p\n", dir);
                struct dirent * ent;
                while ((ent = readdir(dir)) != NULL) {
                    printf("%s\n", ent->d_name);
                }
                closedir(dir);

        	    pid_t pid = fork();
                if(pid > 0){
                    printf("Fork Parent %d = %d\n", getpid(), pid);
                    int status = 0;
                    if(waitpid(pid, &status, 0) >= 0){
                        printf("waitpid status %d\n", status);
                    }else{
                        perror("waitpid failed\n");
                    }
                }else if(pid == 0){
                    printf("Fork Child %d = %d\n", getpid(), pid);
                    int result = system("echo test");
                    if(result == 0){
                        printf("System Success");
                    } else {
                        printf("System Failed %d\n", result);
                    }
                    _exit(123);
                } else {
                    printf("Fork Failed %d = %d\n", getpid(), pid);
                }
            }else{
                perror("Opendir Failed\n");
            }
        } else {
            perror("Malloc Failed\n");
        }
    } else {
        perror("clock_gettime Failed\n");
    }
    return 234;
}
