// dalii :)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>


#define MAX_WORDS 20
#define MAX_LINE 1024
//#define global_path "/home/sarina/Desktop/global-neogit"
char cwd_main_file[] = "/home/sarina/Desktop/test";

// todo creat config ********************************************************
int create_configs() {
    char username[50], email[50];
    // open global config
    FILE *global_config = fopen("/home/sarina/Desktop/global-neogit/config", "r");
    if (global_config == NULL) {
        perror("can't open global config");
        return 1;
    }
    // open local config
    char cwd[500], cwd_copy[500];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd in creat config has problem");
        return 1;
    }
    strcpy(cwd_copy, cwd);
//    fprintf(stdout, "cwd: %s", cwd);
    FILE *local_config = fopen(strcat(cwd_copy, "/.neogit/config"), "w");
    if (local_config == NULL) {
        perror("can't open local config");
        return 1;
    }
    // copy config data
    if (fscanf(global_config, "username: %s\nemail: %s", username, email) == 2) {
        fprintf(local_config, "username: %s\nemail: %s\n", username, email);
    } else {
        perror("error reading data from source file");
    }
    fclose(global_config);

    //TODO
    fprintf(local_config, "final ID: %d\n", 0);
    fprintf(local_config, "current commit_ID: %d\n", 0);
    fprintf(local_config, "branch: %s\n", "master");
    fprintf(local_config, "HEAD: %d\n", 0);

    fclose(local_config);

    // creat alias's file
    strcpy(cwd_copy, cwd);
//    fprintf(stdout, "cwd: %s", cwd);
    FILE *file = fopen(strcat(cwd_copy, "/.neogit/alias"), "w");
    fclose(file);
    // create stage folder
    strcpy(cwd_copy, cwd);
    if (mkdir(strcat(cwd_copy, "/.neogit/stage"), 0755) != 0) return 1;
    strcpy(cwd_copy, cwd);
    FILE *file2 = fopen(strcat(cwd_copy, "/.neogit/stage_data"), "w");
    fclose(file2);

    // create unstage folder
    strcpy(cwd_copy, cwd);
    if (mkdir(strcat(cwd_copy, "/.neogit/unstage"), 0755) != 0) return 1;
    strcpy(cwd_copy, cwd);
    FILE *file3 = fopen(strcat(cwd_copy, "/.neogit/unstage_data"), "w");
    fclose(file3);

    // create branches folder
    strcpy(cwd_copy, cwd);
    if (mkdir(strcat(cwd_copy, "/.neogit/branches"), 0755) != 0) return 1;
    strcpy(cwd_copy, cwd);
    FILE *file4 = fopen(strcat(cwd_copy, "/.neogit/branches/all_commit_data"), "w");
    fclose(file4);

    // dir and config for master
    strcpy(cwd_copy, cwd);
    if (mkdir(strcat(cwd_copy, "/.neogit/branches/master"), 0755) != 0) return 1;
    strcpy(cwd_copy, cwd);
    FILE *file5 = fopen(strcat(cwd_copy, "/.neogit/branches/config_master"), "w");
    fprintf(file5, "0");
    fclose(file5);

    strcpy(cwd_copy, cwd);
    FILE *file6 = fopen(strcat(cwd_copy, "/.neogit/branches/tag"), "w");
    fclose(file6);

    // shortcut
    strcpy(cwd_copy, cwd);
    FILE *file7 = fopen(strcat(cwd_copy, "/.neogit/branches/shortcut"), "w");
    fclose(file7);

    // shortcut
    strcpy(cwd_copy, cwd);
    FILE *file8 = fopen(strcat(cwd_copy, "/.neogit/hook"), "w");
    fclose(file8);

    // shortcut
    strcpy(cwd_copy, cwd);
    FILE *file9 = fopen(strcat(cwd_copy, "/.neogit/hook_all"), "w");
    fclose(file9);

//    file = fopen(".neogit/tracks", "w");
//    fclose(file);

    return 0;
}

// todo init ********************************************************
int init_function(int argc, char *const argv[]) {
    char cwd[500];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd has problem");
        return 1;
    }
    char tmp_cwd[500];
    bool exists = false;
    struct dirent *entry;
    do {
        // find .neogit
        DIR *dir = opendir(".");
        if (dir == NULL) {
            perror("Error opening current directory");
            return 1;
        }
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".neogit") == 0)
                exists = true;
        }
        closedir(dir);

        // update current working directory
        if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL) return 1;

        // change cwd to parent
        if (strcmp(tmp_cwd, "/") != 0) {
            if (chdir("..") != 0) return 1;
        }

    } while (strcmp(tmp_cwd, "/") != 0);
    // return to the initial cwd
    if (chdir(cwd) != 0) return 1;


    if (!exists) {
        mkdir(".neogit", 0755);
        perror("neeogit init successfully");
        return create_configs();
    } else {
        perror("neogit repository has already initialized");
    }
    return 0;
}

int valid_alias(char command[]) {

    char function[100], function2[100];
    sscanf(command, "%*s %s %s", function, function2);
    char all_commands[] = "init config add reset status commit log branch checkout";
    if (strstr(all_commands, function) != NULL) {
        return 1;
    }
    return 0;
}

// todo ********************************************************
int config_alias_function(int argc, char *const argv[]) {
    if (argc < 4) {
        perror("invalid command");
        return 1;
    }
    char alias_name[50], command[50];
    char cwd[500], cwd_copy[500], buffer[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd has problem");
        return 1;
    }
    strcpy(cwd_copy, cwd);
    FILE *local_alias = fopen(strcat(cwd_copy, "/.neogit/alias"), "a");
    if (local_alias == NULL) {
        perror("can't open local alias");
        return 1;
    }
    if (strcmp(argv[2], "-global") == 0) {
        if (valid_alias(argv[4]) == 0) {
            perror("invalid command");
            return 1;
        }
        if (argc < 5) {
            perror("invalid command");
            return 1;
        }
        FILE *global_alias = fopen("/home/sarina/Desktop/global-neogit/alias", "r+");
        sscanf(argv[3], "alias.%s", alias_name);
        strcpy(command, argv[4]);
        fprintf(global_alias, "%s %s\n", alias_name, command);
        fprintf(local_alias, "%s %s\n", alias_name, command);
        fclose(global_alias);
    } else if (strstr(argv[2], "alias") != NULL) {
        if (valid_alias(argv[3]) == 0) {
            perror("invalid command");
            return 1;
        }
        sscanf(argv[2], "alias.%s", alias_name);
        strcpy(command, argv[3]);
        fprintf(local_alias, "%s %s\n", alias_name, command);
    } else {
        perror("invalid command");
        return 1;
    }
    perror("alias added successfully");
    fclose(local_alias);
    return 0;
}


// todo ********************************************************
int config_function(int argc, char *const argv[]) {
    if (argc < 4) {
        perror("invalid command");
        return 1;
    }
    if (strstr(argv[3], "alias") != NULL || strstr(argv[2], "alias") != NULL) {
        return config_alias_function(argc, argv);
    }
//     open local config
    char cwd[500], cwd_copy[500];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd in creat config has problem");
        return 1;
    }
    strcpy(cwd_copy, cwd);
    FILE *local_config = fopen(strcat(cwd, "/.neogit/config"), "r");
    if (local_config == NULL) {
        perror("can't open local config");
        return 1;
    }
    // scan data from old local config
    char username[50], email[50], branch[50];
    char last_id, current_id;
    fscanf(local_config, "username: %s\nemail: %s\nfinal ID: %c\ncurrent commit_ID: %c\nbranch: %s", username,
           email, &last_id, &current_id, branch);
    fclose(local_config);
    // change global config
    if (strcmp(argv[2], "-global") == 0) {
        if (argc != 5) {
            perror("invalid command");
            return 1;
        }
        // open global config
        FILE *global_config = fopen("/home/sarina/Desktop/global-neogit/config", "r");
        if (global_config == NULL) {
            perror("can't open global config");
            return 1;
        }


        // open staged_data
        char stage_data_path[200];
        snprintf(stage_data_path, sizeof(stage_data_path), "%s/%s", cwd_copy, ".neogit/stage_data");
        FILE *stage_data = fopen(stage_data_path, "r");

        // scan data from old global config
        char username_G[50], email_G[50];
        fscanf(global_config, "username: %s\nemail: %s", username_G, email_G);
        fclose(global_config);
        FILE *local_config3 = fopen(cwd, "w");
        FILE *global_config2 = fopen("/home/sarina/Desktop/global-neogit/config", "w");
        if (strcmp(argv[3], "user.name") == 0) {
            fprintf(local_config3, "username: %s\nemail: %s\nfinal ID: %c\ncurrent commit_ID: %c\nbranch: %s",
                    argv[4], email, last_id, current_id, branch);
            fprintf(global_config2, "username: %s\nemail: %s", argv[4], email_G);
            perror("global name changed successfully");
        } else if (strcmp(argv[3], "user.email") == 0) {
            fprintf(local_config3, "username: %s\nemail: %s\nfinal ID: %c\ncurrent commit_ID: %c\nbranch: %s",
                    username, argv[4], last_id, current_id, branch);
            fprintf(global_config2, "username: %s\nemail: %s", username_G, argv[4]);
            perror("global email changed successfully");
        } else {
            perror("invalid input, type user or email");
            return 1;
        }
        fclose(local_config3);
        fclose(global_config2);
        return 0;
        // change local config
    } else if (strstr(argv[2], "user") != NULL) {
        if (argc != 4) {
            perror("rerer");
            perror("invalid command");
            return 1;
        }
        FILE *local_config2 = fopen(cwd, "w");
        if (strcmp(argv[2], "user.name") == 0) {
            fprintf(local_config2, "username: %s\nemail: %s\nfinal ID: %c\ncurrent commit_ID: %c\nbranch: %s",
                    argv[3],
                    email, last_id, current_id, branch);
            perror("local name changed successfully");
        } else if (strcmp(argv[2], "user.email") == 0) {
            fprintf(local_config2, "username: %s\nemail: %s\nfinal ID: %c\ncurrent commit_ID: %c\nbranch: %s",
                    username,
                    argv[3], last_id, current_id, branch);
            perror("local email changed successfully");
        } else {
            perror("invalid input, type user or email");
            return 1;
        }
        fclose(local_config2);
        return 0;
    } else {
        fprintf(stdout, "argv[3]: %s", argv[3]);
        perror("invalid command");
        return 1;
    }
}

//// todo ********************************************************
//void print_command(int argc, char *argv[]) {
//    for (int i = 0; i < argc; i++) {
//        fprintf(stdout, "%s ", argv[i]);
//    }
//    fprintf(stdout, "\n");
//}

// todo ********************************************************
int check_local_alias(int *argc, char *argv[]) {
    // open local config
    char cwd[500], cwd_copy[500];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd has problem");
        return 1;
    }
//  strcpy(cwd_copy,cwd);
    FILE *local_alias = fopen(strcat(cwd, "/.neogit/alias"), "r");
    if (local_alias == NULL) {
        perror("can't open local alias");
        return 1;
    }
    char buffer[MAX_LINE];
    char str[50];
    int yes = 0;
    if (argv[2] != NULL) {
        yes = 1;
        strcpy(str, argv[2]);
    }
    char *token;
    int count = 0;
    while (fgets(buffer, sizeof(buffer), local_alias) != NULL) {
        if (strncmp(buffer, argv[1], strlen(argv[1])) == 0) {
            token = strtok(buffer + strlen(argv[1]) + 1, " \t\n");
            while (token != NULL && count < MAX_WORDS) {
                argv[count] = strdup(token);
                token = strtok(NULL, " \t\n");
                count++;
            }
            fclose(local_alias);
            if (yes == 1) {
                argv[count] = strdup(str);
                count++;
            }
            *argc = count;
            return 1;
        }
    }
    return 0;
}

// todo ********************************************************
int check_global_alias(int *argc, char *argv[]) {
    char cwd[500];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd has problem");
        return 1;
    }
    FILE *global_alias = fopen(strcat("/home/sarina/Desktop/global-neogit", "/alias"), "r");
    if (global_alias == NULL) {
        perror("can't open global alias");
        return 1;
    }
    char buffer[MAX_LINE];
    char str[50];
    int yes = 0;
    if (argv[2] != NULL) {
        yes = 1;
        strcpy(str, argv[2]);
    }
    char *token;
    int count = 0;
    while (fgets(buffer, sizeof(buffer), global_alias) != NULL) {
        if (strncmp(buffer, argv[1], strlen(argv[1])) == 0) {
            token = strtok(buffer + strlen(argv[1]) + 1, " \t\n");
            while (token != NULL && count < MAX_WORDS) {
                argv[count] = strdup(token);
                token = strtok(NULL, " \t\n");
                count++;
            }
            fclose(global_alias);
            if (yes == 1) {
                argv[count] = strdup(str);
                count++;
            }
            *argc = count;
            return 1;
        }
    }
    return 0;
}

// todo ********************************************************
int compare_tow_files(FILE *file1, FILE *file2) {
    char c1 = fgetc(file1);
    char c2 = fgetc(file2);
    while (c1 != EOF || c2 != EOF) {
        if (c1 != c2) {
            return 0;
        }
        c1 = fgetc(file1);
        c2 = fgetc(file2);
    }
    return 1;
}

// todo ********************************************************
int find_file_in_stage(char file_name[]) {
    char file_2[200];
    snprintf(file_2, sizeof(file_2), "%s/%s", cwd_main_file, ".neogit/stage");
    DIR *dir = opendir(file_2);
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {

        if (entry->d_type == DT_REG) {
            if (strcmp(entry->d_name, file_name) == 0) {
                return 1;
            }
        }

    }
    return 0;
}

int counter = 0;

// todo ********************************************************
void add_dir(int argc, char *argv[], int path) {
    char cwd[500];
    int yes = 0;
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd in creat config has problem");
        return;
    }
    DIR *dir = opendir(argv[path]);
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char file_path[PATH_MAX];
            snprintf(file_path, sizeof(file_path), "%s/%s", argv[path], entry->d_name);
            // file_path is the path of the file in directory
            // TODO B MODE
            if (find_file_in_stage(entry->d_name) == 1) {
                FILE *f1 = fopen(file_path, "r");
                char file_2[200];
                snprintf(file_2, sizeof(file_2), "%s/%s/%s", cwd, ".neogit/stage", entry->d_name);
                FILE *f2 = fopen(file_2, "r");
                if (compare_tow_files(f1, f2) == 1) {
                    continue;
                }
                fclose(f1);
                fclose(f2);
            }
            yes = 1;

            char system_command[PATH_MAX];
            snprintf(system_command, sizeof(system_command), "cp %s %s/%s", file_path, cwd, ".neogit/stage");
            system(system_command);

            char stage_data_path[200];
            snprintf(stage_data_path, sizeof(stage_data_path), "%s/%s", cwd, ".neogit/stage_data");
            FILE *stage_data = fopen(stage_data_path, "a+");
            fprintf(stage_data, "%s %s/%s\n", entry->d_name, argv[path], entry->d_name);

        } else if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 &&
                   strcmp(entry->d_name, "..") && strcmp(entry->d_name, ".neogit") != 0) {
            perror("45450");
            char dicdic_path[PATH_MAX];
            snprintf(dicdic_path, sizeof(dicdic_path), "%s/%s", argv[path], entry->d_name);
            strcpy(argv[10], dicdic_path);
            add_dir(argc, argv, 10);
        }
    }
    closedir(dir);
    if (yes) {
        printf("files of directory were staged successfully\n");
    } else
        printf("files of directory have not changed\n");
}

// todo ********************************************************
void add_file(int argc, char *argv[], int path) {
    char cwd[500], cwd_copy[500];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd in creat config has problem");
        return;
    }
    strcpy(cwd_copy, cwd);
    FILE *file = fopen(argv[path], "r");
    char system_command[PATH_MAX], file_name[100];
    int i = strlen(argv[path]), j = 0;
    while (argv[path][i] != '/') {
        j++;
        i--;
    }
    memmove(file_name, argv[path] + i + 1, j * sizeof(char));
    if (find_file_in_stage(file_name) == 1) {
        FILE *f1 = fopen(argv[path], "r");
        char file_path[200];
        snprintf(file_path, sizeof(file_path), "%s/%s/%s", cwd_copy, ".neogit/stage", file_name);
        FILE *f2 = fopen(file_path, "r");
        if (compare_tow_files(f1, f2) == 1) {
            fclose(f1);
            fclose(f2);
            fprintf(stdout, "file has not changed\n");
            return;
        }
        fclose(f1);
        fclose(f2);
    }
    printf("source: %s\n", argv[path]);
    snprintf(system_command, sizeof(system_command), "cp %s %s/%s", argv[path], cwd_copy, ".neogit/stage");
    system(system_command);
    // put name and path of the file in stage data
    char stage_data_path[200];
    snprintf(stage_data_path, sizeof(stage_data_path), "%s/%s", cwd_copy, ".neogit/stage_data");
    FILE *stage_data = fopen(stage_data_path, "a+");
    fprintf(stage_data, "%s %s\n", file_name, argv[path]);
    fclose(stage_data);

    printf("file was staged successfully\n");
    fclose(file);
}

int cnt = 0;
char cwd_copy_add[100];
int depth;

// todo ********************************************************
int add_n(char cwd[]) {

    if (cnt == 0)
        strcpy(cwd_copy_add, cwd);
    printf("cwd_copy: %s\n", cwd_copy_add);
    DIR *dir = opendir(cwd);
    struct dirent *entry;
    cnt++;
    while ((entry = readdir(dir)) != NULL) {

        // find path of object in dir
        char path[PATH_MAX];
        snprintf(path, sizeof(path), "%s/%s", cwd, entry->d_name);
        // open staged_data
        char stage_data_path[PATH_MAX];
        snprintf(stage_data_path, sizeof(stage_data_path), "%s/%s", cwd_copy_add, ".neogit/stage_data");
//
//        printf("pth: %s\n", stage_data_path);
//        printf("cwd_copy: %s\n", cwd_copy);

        FILE *stage_data = fopen(stage_data_path, "r");
        perror("diling dooloong");

//        printf("name of object= %s\n", entry->d_name);
        // check if it is a file
        if (entry->d_type == DT_REG && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            int match = 0;
            char buffer[MAX_LINE];
            char name_of_staged_file[50], path_of_staged_file[50];

            while (fgets(buffer, sizeof(buffer), stage_data) != NULL) {
                sscanf(buffer, "%s %s", name_of_staged_file, path_of_staged_file);
                if (strcmp(path_of_staged_file, path) == 0) {
                    match = 1;
                    printf("*%s is staged\n", entry->d_name);
                    break;
                }
            }
            if (!match) {
                printf("%s is not staged\n", entry->d_name);
            }
            fclose(stage_data);
            // check if it is a directory of file
        } else if ((entry->d_type == DT_DIR) && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 &&
                   strcmp(entry->d_name, ".neogit") != 0) {
            DIR *dirdir = opendir(path);
            struct dirent *entrydir;
            int match = 0;
            int all = 0;
            int x = 0;
            while ((entrydir = readdir(dirdir)) != NULL) {
                if (entrydir->d_type == DT_REG) {
                    all++;
                    // find path of file
                    char file_path[PATH_MAX];
                    snprintf(file_path, sizeof(file_path), "%s/%s", path, entrydir->d_name);
                    char line[MAX_LINE];
                    char name_of_staged_file[50], path_of_staged_file[PATH_MAX];
                    // read stage data to match
                    while (fgets(line, sizeof(line), stage_data) != NULL) {
                        sscanf(line, "%s %s", name_of_staged_file, path_of_staged_file);
                        if (strcmp(path_of_staged_file, file_path) == 0) {
                            match = 1;
                            x++;
                            printf("*%s is staged\n", entrydir->d_name);
                            continue;
                        }
                    }
                    if (all == x) {
                        printf("*all files of %s directory\n", entry->d_name);
                    }
                }
                if (entrydir->d_type == DT_DIR && strcmp(entrydir->d_name, ".") != 0 &&
                    strcmp(entrydir->d_name, "..") != 0 && strcmp(entrydir->d_name, ".neogit") != 0) {
                    char dicdic_path[PATH_MAX];
                    snprintf(dicdic_path, sizeof(dicdic_path), "%s/%s/%s", cwd, entry->d_name, entrydir->d_name);
                    add_n(dicdic_path);
                }
            }
            if (!match) {
                printf("there is no file in stage to unstage from %s directory\n", entry->d_name);
            }
            fclose(stage_data);
        }
    }
    closedir(dir);
    return 0;

}


// todo ********************************************************
int add_function(int argc, char *argv[]) {
    char cwd[500];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd in creat config has problem");
        return 1;
    }
    if (strcmp(argv[2], "-n") == 0) {
        // TODO OOOOOOOOOOOOOOO
        depth = atoi(argv[3]);
        add_n(cwd);
        return 0;
    }
    int start = 2;
    if (strcmp(argv[2], "-f") == 0) {
        start = 3;
    }
    for (int i = start; i < argc; i++) {
        DIR *dir = opendir(argv[i]);
        if (dir != NULL) {
            add_dir(argc, argv, i);
            continue;
        }
        FILE *file = fopen(argv[i], "r");
        if (file != NULL) {
            add_file(argc, argv, i);
            continue;
        }
        printf("there is no file or directory with %s path", argv[i]);
    }
    return 1;

}

// todo ********************************************************
void reset_file(int argc, char *argv[], int path) {
    char cwd[500], cwd_copy[500];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd in creat config has problem");
        return;
    }
    strcpy(cwd_copy, cwd);
    // open staged_data
    char stage_data_path[PATH_MAX];
    snprintf(stage_data_path, sizeof(stage_data_path), "%s/%s", cwd_copy, ".neogit/stage_data");
    FILE *stage_data = fopen(stage_data_path, "r");
    // open copy of staged data
    char c_stage_data_path[PATH_MAX];
    snprintf(c_stage_data_path, sizeof(stage_data_path), "%s/%s", cwd_copy, ".neogit/stage_data_copy");
    FILE *c_stage_data = fopen(c_stage_data_path, "w");
    // open unstaged_data
    char unstage_data_path[PATH_MAX];
    snprintf(unstage_data_path, sizeof(unstage_data_path), "%s/%s", cwd_copy, ".neogit/unstage_data");
    FILE *unstage_data = fopen(unstage_data_path, "a");

    int match = 0;
    char buffer[1000];
    char name_of_staged_file[50], path_of_staged_file[50];


    while (fgets(buffer, sizeof(buffer), stage_data) != NULL) {
        sscanf(buffer, "%s %s", name_of_staged_file, path_of_staged_file);
        if (strcmp(path_of_staged_file, argv[path]) == 0) {
            match = 1;
            fprintf(unstage_data, "%s %s\n", name_of_staged_file, path_of_staged_file);
            // cat the file from stage to unstage
            char system_command[PATH_MAX];
            snprintf(system_command, sizeof(system_command), "cp %s/%s/%s %s/%s", cwd_copy, ".neogit/stage",
                     name_of_staged_file, cwd_copy, ".neogit/unstage");
            system(system_command);

            char remove_path[PATH_MAX];
            snprintf(remove_path, sizeof(remove_path), "%s/%s/%s", cwd_copy, ".neogit/stage", name_of_staged_file);
            remove(remove_path);

        } else {
            fprintf(c_stage_data, "%s %s\n", name_of_staged_file, path_of_staged_file);
        }
    }
    if (!match) {
        printf("there is no file to unstage from stage");
    } else {
        remove(stage_data_path);
        rename(c_stage_data_path, stage_data_path);
        printf("file was unstaged");
    }
    fclose(stage_data);
    fclose(unstage_data);
    fclose(c_stage_data);
}


// todo ********************************************************
void reset_directory(int argc, char *argv[], int path) {
    char cwd[500], cwd_copy[500];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd in creat config has problem");
        return;
    }
    strcpy(cwd_copy, cwd);
    // open staged_data
    char stage_data_path[200];
    snprintf(stage_data_path, sizeof(stage_data_path), "%s/%s", cwd_copy, ".neogit/stage_data");
    FILE *stage_data = fopen(stage_data_path, "r");

    // open copy of staged data
    char c_stage_data_path[200];
    snprintf(c_stage_data_path, sizeof(c_stage_data_path), "%s/%s", cwd_copy, ".neogit/stage_data_copy");
    FILE *c_stage_data = fopen(c_stage_data_path, "w");
    // open unstaged_data
    char unstage_data_path[200];
    snprintf(unstage_data_path, sizeof(unstage_data_path), "%s/%s", cwd_copy, ".neogit/unstage_data");
    FILE *unstage_data = fopen(unstage_data_path, "a");

    strcpy(cwd_copy, cwd);
    DIR *dir = opendir(argv[path]);
    struct dirent *entry;
    int match = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            // find path of file
            char file_path[PATH_MAX];
            snprintf(file_path, sizeof(file_path), "%s/%s", argv[path], entry->d_name);
            char line[MAX_LINE];
            char name_of_staged_file[50], path_of_staged_file[PATH_MAX];
            // read stage data to match
            while (fgets(line, sizeof(line), stage_data) != NULL) {
                sscanf(line, "%s %s", name_of_staged_file, path_of_staged_file);
                if (strcmp(path_of_staged_file, file_path) == 0) {
                    match = 1;
                    fprintf(unstage_data, "%s %s\n", name_of_staged_file, path_of_staged_file);

                    char system_command[PATH_MAX];
                    snprintf(system_command, sizeof(system_command), "cp %s/%s/%s %s/%s", cwd_copy, ".neogit/stage",
                             entry->d_name, cwd_copy, ".neogit/unstage");
//                    printf("system command; %s\n", system_command);
                    system(system_command);

                    char remove_path[PATH_MAX];
                    snprintf(remove_path, sizeof(remove_path), "%s/%s/%s", cwd_copy, ".neogit/stage",
                             name_of_staged_file);
                    remove(remove_path);

                } else {
                    fprintf(c_stage_data, "%s %s\n", name_of_staged_file, path_of_staged_file);
                }
            }
        } else if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char dicdic_path[PATH_MAX];
            snprintf(dicdic_path, sizeof(dicdic_path), "%s/%s", argv[path], entry->d_name);
            strcpy(argv[10], dicdic_path);
            reset_directory(argc, argv, 10);
        }
    }
    if (!match) {
        printf("there is no file in dorectory to unstage from stage\n");
    } else {
        // TODO OOOOOOOO
        printf("//stage_data_path: %s\n", stage_data_path);
        remove(stage_data_path);
        printf("//stage_data_path: %s\n", c_stage_data_path);
        printf("//stage_data_path: %s\n", stage_data_path);
        rename(c_stage_data_path, stage_data_path);
        printf("files of directory were unstaged successfully\n");
    }
    fclose(stage_data);
    fclose(unstage_data);
    fclose(c_stage_data);
    closedir(dir);
}

void reset_undo() {
    char cwd[500], cwd_copy[500];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd in creat config has problem");
        return;
    }
    strcpy(cwd_copy, cwd);

    // open staged_data
    char stage_data_path[200];
    snprintf(stage_data_path, sizeof(stage_data_path), "%s/%s", cwd_copy, ".neogit/stage_data");
    FILE *stage_data = fopen(stage_data_path, "a");
    // open unstaged_data
    char unstage_data_path[200];
    snprintf(unstage_data_path, sizeof(unstage_data_path), "%s/%s", cwd_copy, ".neogit/unstage_data");
    FILE *unstage_data = fopen(unstage_data_path, "r");


    char line[MAX_LINE];
    char name_of_unstaged_file[50], path_of_unstaged_file[PATH_MAX];

//    if (fgets(line, sizeof(line), unstage_data)){
//
//        printf("there aren't any unstaged file");
//        fseek(unstage_data,0,SEEK_SET);
//    }

    // read stage data to match
    while (fgets(line, sizeof(line), unstage_data) != NULL) {
        sscanf(line, "%s %s", name_of_unstaged_file, path_of_unstaged_file);
        fprintf(stage_data, "%s %s\n", name_of_unstaged_file, path_of_unstaged_file);

        char system_command[PATH_MAX];
        snprintf(system_command, sizeof(system_command), "cp %s/%s/%s %s/%s", cwd_copy, ".neogit/unstage",
                 name_of_unstaged_file, cwd_copy, ".neogit/stage");
//                    printf("system command; %s\n", system_command);
        system(system_command);

        char remove_path[PATH_MAX];
        snprintf(remove_path, sizeof(remove_path), "%s/%s/%s", cwd_copy, ".neogit/unstage",
                 name_of_unstaged_file);
        remove(remove_path);
    }
    fclose(stage_data);
    fclose(unstage_data);

    FILE *file = fopen(unstage_data_path, "w");
    fclose(file);

    printf("unstaged files were staged successfully\n");

}


// todo ********************************************************
int reset_function(int argc, char *argv[]) {
    if (strcmp(argv[2], "-undo") == 0) {
        reset_undo();
        return 0;
    }


    int start = 2;
    if (strcmp(argv[2], "-f") == 0) {
        start = 3;
    }

    for (int i = start; i < argc; i++) {
        DIR *dir = opendir(argv[2]);
        if (dir != NULL) {
            reset_directory(argc, argv, 2);
            continue;
        }
        FILE *file = fopen(argv[2], "r");
        if (file != NULL) {
            reset_file(argc, argv, 2);
            perror("123");
            continue;
        }
        printf("there is no file or directory with this path");
    }

    return 1;
}

// todo ********************************************************
int set_shortcut_function(int argc, char *argv[]) {
    if (argc < 6) {
        printf("invalid command\n");
        return 0;
    }

    char cwd[500], cwd_copy[500];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd in creat config has problem\n");
        return 1;
    }
    strcpy(cwd_copy, cwd);

    // open shortcut
    char shortcut_path[200];
    snprintf(shortcut_path, sizeof(shortcut_path), "%s/%s", cwd_copy, ".neogit/branches/shortcut");
    FILE *shortcut = fopen(shortcut_path, "a");

    fprintf(shortcut, "%s /%s/\n", argv[5], argv[3]);
    printf("shortcut added successfully\n");
    fclose(shortcut);
    return 0;
}

// todo ********************************************************
int replace_shortcut_function(int argc, char *argv[]) {
    if (argc < 6) {
        printf("invalid command\n");
        return 0;
    }

    char cwd[500], cwd_copy[500];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd in creat config has problem");
        return 1;
    }
    strcpy(cwd_copy, cwd);

    // open shortcut
    char shortcut_path[200];
    snprintf(shortcut_path, sizeof(shortcut_path), "%s/%s", cwd_copy, ".neogit/branches/shortcut");
    FILE *shortcut = fopen(shortcut_path, "r+");

    // open shortcut
    char shortcut_copy_path[200];
    snprintf(shortcut_copy_path, sizeof(shortcut_copy_path), "%s/%s", cwd_copy, ".neogit/branches/shortcut_copy");
    FILE *shortcut_copy = fopen(shortcut_copy_path, "w");

    int match = 0;
    char buffer[1000];
    char name_of_shortcut[50], massage[50];
    while (fgets(buffer, sizeof(buffer), shortcut) != NULL) {
        sscanf(buffer, "%s /%[^/]s/", name_of_shortcut, massage);
        if (strcmp(name_of_shortcut, argv[5]) == 0) {
            strcpy(massage, argv[3]);
            match = 1;
        }
        fprintf(shortcut_copy, "%s /%s/\n", name_of_shortcut, massage);
    }
    if (match == 0) {
        printf("invalid shortcut-name\n");
        return 0;
    }
    fclose(shortcut);
    fclose(shortcut_copy);

    remove(shortcut_path);
    rename(shortcut_copy_path, shortcut_path);
    printf("massage replaced successfully\n");
    return 0;
}

// todo ********************************************************
int remove_shortcut_function(int argc, char *argv[]) {
    if (argc < 4) {
        printf("invalid command\n");
        return 0;
    }

    char cwd[500], cwd_copy[500];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd in creat config has problem");
        return 1;
    }
    strcpy(cwd_copy, cwd);

    // open shortcut
    char shortcut_path[200];
    snprintf(shortcut_path, sizeof(shortcut_path), "%s/%s", cwd_copy, ".neogit/branches/shortcut");
    FILE *shortcut = fopen(shortcut_path, "r+");

    // open shortcut
    char shortcut_copy_path[200];
    snprintf(shortcut_copy_path, sizeof(shortcut_copy_path), "%s/%s", cwd_copy, ".neogit/branches/shortcut_copy");
    FILE *shortcut_copy = fopen(shortcut_copy_path, "w");

    int match = 0;
    char buffer[1000];
    char name_of_shortcut[50], massage[50];
    while (fgets(buffer, sizeof(buffer), shortcut) != NULL) {
        sscanf(buffer, "%s /%[^/]s/", name_of_shortcut, massage);
        if (strcmp(name_of_shortcut, argv[3]) == 0) {
            match = 1;
            continue;
        }
        fprintf(shortcut_copy, "%s /%s/\n", name_of_shortcut, massage);
    }
    if (match == 0) {
        printf("invalid shortcut-name\n");
        return 0;
    }
    fclose(shortcut);
    fclose(shortcut_copy);

    remove(shortcut_path);
    rename(shortcut_copy_path, shortcut_path);
    printf("massage removed successfully\n");
    return 0;
}


// todo ********************************************************
int check_shortcut(int argc, char *argv[]) {
    char cwd[500], cwd_copy[500];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd in creat config has problem");
        return 1;
    }
    strcpy(cwd_copy, cwd);

    // open shortcut
    char shortcut_path[200];
    snprintf(shortcut_path, sizeof(shortcut_path), "%s/%s", cwd_copy, ".neogit/branches/shortcut");
    FILE *shortcut = fopen(shortcut_path, "r");

    char buffer[1000];
    char name_of_shortcut[50], massage[50];
    while (fgets(buffer, sizeof(buffer), shortcut) != NULL) {
        sscanf(buffer, "%s /%[^/]s/", name_of_shortcut, massage);
        if (strcmp(name_of_shortcut, argv[3]) == 0) {
            strcpy(argv[3], massage);
            return 1;
        }
    }
    fclose(shortcut);
    printf("invalid shortcut-name\n");
    return 0;

}

// todo ********************************************************
void find_branch_author(char cwd[], char branch_name[], char author_name[]) {
    char config_path[200];
    int id;
    snprintf(config_path, sizeof(config_path), "%s/%s", cwd, ".neogit/config");
    FILE *config = fopen(config_path, "r+");
    char username[50], email[50];
    char last_id, current_id;
    fscanf(config, "username: %s\nemail: %s\nfinal ID: %c\ncurrent commit_ID: %c\nbranch: %s", author_name,
           email, &last_id, &current_id, branch_name);
    fclose(config);
}

int find_current_id(char cwd[]) {
    char config_path[200];
    int id;

    snprintf(config_path, sizeof(config_path), "%s/%s", cwd, ".neogit/config");
    FILE *config = fopen(config_path, "r+");
    char username[50], email[50], branch_name[1000];
    char last_id, current_id;
    fscanf(config, "username: %s\nemail: %s\nfinal ID: %c\ncurrent commit_ID: %c\nbranch: %s", username,
           email, &last_id, &current_id, branch_name);
    fclose(config);
    id = current_id - '0';
    return id;
}


int find_final_id(char cwd[]) {
    char config_path[200];
    int id;

    snprintf(config_path, sizeof(config_path), "%s/%s", cwd, ".neogit/config");
    FILE *config = fopen(config_path, "r+");
    char username[50], email[50], branch_name[1000];
    char last_id, current_id;
    fscanf(config, "username: %s\nemail: %s\nfinal ID: %c\ncurrent commit_ID: %c\nbranch: %s", username,
           email, &last_id, &current_id, branch_name);
    fclose(config);
    id = last_id - '0';
    return id;
}

// todo ********************************************************
int find_id_and_plus(char cwd[]) {
    char config_path[200];
    int id;

    snprintf(config_path, sizeof(config_path), "%s/%s", cwd, ".neogit/config");
    FILE *config = fopen(config_path, "r+");
    char username[50], email[50], branch_name[1000];
    char last_id, current_id;
    fscanf(config, "username: %s\nemail: %s\nfinal ID: %c\ncurrent commit_ID: %c\nbranch: %s", username,
           email, &last_id, &current_id, branch_name);
    fclose(config);

    id = last_id - '0';
    id++;
    int final = id;

    FILE *config2 = fopen(config_path, "w");
    fprintf(config2, "username: %s\nemail: %s\nfinal ID: %d\ncurrent commit_ID: %d\nbranch: %s",
            username, email, final, id, branch_name);
    fclose(config2);
    return id;
}

// todo ********************************************************
int change_number_of_commit_in_branch(char cwd[], char branch_name[]) {
    // open config of branch in branches folder
    char branch_config_path[200];
    snprintf(branch_config_path, sizeof(branch_config_path), "%s/%s/config_%s", cwd, ".neogit/branches",
             branch_name);
    FILE *branch_config = fopen(branch_config_path, "r");

    // find number of commit of that branch
    int number_of_commit_branch;
    fscanf(branch_config, "%d\n1", &number_of_commit_branch);
    printf("***number of commits in commit: %d\n", number_of_commit_branch);
    number_of_commit_branch++;
    printf("***number of commits in commit: %d\n", number_of_commit_branch);
    fclose(branch_config);

    FILE *branch_config2 = fopen(branch_config_path, "w");
    fprintf(branch_config2, "%d\n1", number_of_commit_branch);
    fclose(branch_config2);
    printf("***number of commits in commit: %d\n", number_of_commit_branch);
    return number_of_commit_branch;

}


// todo ********************************************************
int find_number_of_last_commit_in_branch(char cwd[], char branch_name[]) {
    // open config of branch in branches folder
    char branch_config_path[200];
    snprintf(branch_config_path, sizeof(branch_config_path), "%s/%s/config_%s", cwd, ".neogit/branches",
             branch_name);
    FILE *branch_config = fopen(branch_config_path, "r");

    // find number of commit of that branch
    int number_of_commit_branch;
    fscanf(branch_config, "%d", &number_of_commit_branch);
    fclose(branch_config);
    printf("number of comits in branch: %d\n", number_of_commit_branch);
    return number_of_commit_branch;

}

// todo ********************************************************
int check_if_stage_is_empty() {
    char file_2[PATH_MAX];
    snprintf(file_2, sizeof(file_2), "%s/%s", cwd_main_file, ".neogit/stage");
    DIR *dir = opendir(file_2);
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            return 1;
        }
    }
    return 0;
}


int if_first_or_not() {

    char config_path[200];
    int id;
    snprintf(config_path, sizeof(config_path), "%s/%s", cwd_main_file, ".neogit/config");
    FILE *config = fopen(config_path, "r");
    char username[50], email[50], branch_name[1000];
    char last_id, current_id;
    fscanf(config, "username: %s\nemail: %s\nfinal ID: %c\ncurrent commit_ID: %c\nbranch: %s", username,
           email, &last_id, &current_id, branch_name);

    if (strcmp(branch_name, "master") == 0)
        return 1;
    // open config of branch in branches folder
    char branch_config_path[200];
    snprintf(branch_config_path, sizeof(branch_config_path), "%s/%s/config_%s", cwd_main_file, ".neogit/branches",
             branch_name);
    FILE *branch_config = fopen(branch_config_path, "r");
    perror("[][]");

    int stat;
    fscanf(branch_config, "%*d\n%d", &stat);

    if (stat == 0) {
        return 0;
    }
    return 1;
}

void change_stat_from_0_to_1() {
    char config_path[200];
    int id;
    snprintf(config_path, sizeof(config_path), "%s/%s", cwd_main_file, ".neogit/config");
    FILE *config = fopen(config_path, "r");
    char username[50], email[50], branch_name[1000];
    char last_id, current_id;
    fscanf(config, "username: %s\nemail: %s\nlast commit_ID: %c\ncurrent commit_ID: %c\nbranch: %s", username,
           email, &last_id, &current_id, branch_name);
    fclose(config);
    // open config of branch in branches folder
    char branch_config_path[200];
    snprintf(branch_config_path, sizeof(branch_config_path), "%s/%s/config_%s", cwd_main_file, ".neogit/branches",
             branch_name);
    FILE *branch_config = fopen(branch_config_path, "r");

    int num;
    fscanf(branch_config, "%d\n%*d", &num);
    printf("num in stat change: %d", num);
    fclose(branch_config);

    FILE *branch_config2 = fopen(branch_config_path, "w");

    fprintf(branch_config2, "%d\n1", num);
    fclose(branch_config2);
}

// todo ********************************************************
int find_number_of_last_id_in_branch(char branch_name[]) {
    // open all_commit_data
    char all_data_path[200];
    snprintf(all_data_path, sizeof(all_data_path), "%s/%s", cwd_main_file, ".neogit/branches/all_commit_data");
    FILE *all_commit_data = fopen(all_data_path, "r");

    int count = 0;
    char buffer[100][1000];
    while (fgets(buffer[count], sizeof(buffer), all_commit_data) != NULL) {
        count++;
    }

    for (int i = count - 1; i >= 0; i--) {
        char name[100];
        int id;
        sscanf(buffer[i], "%*d %s %d %*s %*s %*s /%*[^/]s/ /%*[^/\n]s/\n", name, &id);
        if (strcmp(name, branch_name) == 0) {
            fclose(all_commit_data);
            return id;
        }
    }
    fclose(all_commit_data);
    return 0;
}

// todo ********************************************************
int check_head() {
    char config_path[200];
    int id;
    snprintf(config_path, sizeof(config_path), "%s/%s", cwd_main_file, ".neogit/config");
    FILE *config = fopen(config_path, "r+");
    char username[50], email[50], branch_name[1000];
    char last_id, current_id;
    fscanf(config, "username: %s\nemail: %s\nfinal ID: %c\ncurrent commit_ID: %c\nbranch: %s", username,
           email, &last_id, &current_id, branch_name);
    fclose(config);

    int current = atoi(&current_id);
    int head = find_number_of_last_id_in_branch(branch_name);

    if (current == head)
        return 1;
    return 0;

}

// todo ********************************************************
int commit_function(int argc, char *argv[]) {

    if (argc > 4) {
        printf("put massage in the double quote\n");
        return 0;
    }
    if (argc == 3) {
        printf("write a massage\n");
        return 0;
    }
    if (argc < 3) {
        printf("invalid command\n");
        return 0;
    }
    if (strlen(argv[3]) > 72) {
        printf("write a shorter massage\n");
        return 0;
    }
    if (strcmp(argv[2], "-s") == 0) {
        if (check_shortcut(argc, argv) == 0) {
            return 0;
        }
    }

    if (if_first_or_not() == 0) {

        change_stat_from_0_to_1();
    } else {

        if (!check_head()) {
            printf("you have to checkout on HEAD\n");
            return 0;
        }
    }

    // check if there is a file in stage folder
    if (!check_if_stage_is_empty()) {
        printf("you have to add a file first\n");
        return 0;
    }

    char cwd[500], cwd_copy[500];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd in creat config has problem");
        return 0;
    }
    strcpy(cwd_copy, cwd);

    // scan and change the commit ID and scan branch name and name of author
    char branch_name[100], author_name[100];
    find_branch_author(cwd, branch_name, author_name);
    int id = find_id_and_plus(cwd);


    // find time of commit
    time_t rawtime;
    struct tm *time_info;
    time(&rawtime);
    time_info = localtime(&rawtime);
    char time_string[500];
    strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", time_info);

    // open config of branch in branches folder
    // find number of commit of that branch
    int number_of_commit_in_branch = change_number_of_commit_in_branch(cwd, branch_name);
    perror("-----");
    // open commit folder in branch
    char commit_in_branch_path[PATH_MAX];
    snprintf(commit_in_branch_path, sizeof(commit_in_branch_path), "%s/%s/%d", ".neogit/branches", branch_name,
             number_of_commit_in_branch);
    mkdir(commit_in_branch_path, 0775);

    char stage_path[PATH_MAX];
    snprintf(stage_path, sizeof(stage_path), "%s/%s", cwd, ".neogit/stage");
    DIR *stage_dir = opendir(stage_path);
    // cat file
    struct dirent *entry;
    while ((entry = readdir(stage_dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            // cat file from stage to current branch and current commit_num folder
            char system_command[PATH_MAX];
            snprintf(system_command, sizeof(system_command), "cp %s/%s/%s %s/%s/%s/%d", cwd, ".neogit/stage",
                     entry->d_name, cwd_copy, ".neogit/branches", branch_name, number_of_commit_in_branch);
            system(system_command);

            char remove_path[PATH_MAX];
            snprintf(remove_path, sizeof(remove_path), "%s/%s/%s", cwd, ".neogit/stage", entry->d_name);
            remove(remove_path);
        }
    }
    closedir(stage_dir);

    // open staged_data
    char stage_data_path[200];
    snprintf(stage_data_path, sizeof(stage_data_path), "%s/%s", cwd_copy, ".neogit/stage_data");
    FILE *stage_data = fopen(stage_data_path, "r");

    // open all_commit_data
    char all_data_path[200];
    snprintf(all_data_path, sizeof(all_data_path), "%s/%s", cwd_copy, ".neogit/branches/all_commit_data");
    FILE *all_commit_data = fopen(all_data_path, "a");

    // cat data
    char line[MAX_LINE];
    char name_of_staged_file[50], path_of_staged_file[PATH_MAX];
    // read stage data
    while (fgets(line, sizeof(line), stage_data) != NULL) {
        sscanf(line, "%s %[^\n]s\n", name_of_staged_file, path_of_staged_file);
        fprintf(all_commit_data, "%d %s %d %s %s %s /%s/ $%s\n", id, branch_name, number_of_commit_in_branch,
                name_of_staged_file,
                path_of_staged_file, author_name,
                time_string, argv[3]);
    }
    fclose(stage_data);
    fclose(all_commit_data);


    // delete data of stage
    FILE *file = fopen(stage_data_path, "w");
    fclose(file);
    printf("staged files commited successfully\n");
    printf("id: %d\ntime: %s\nmassage: %s\n", id, time_string, argv[3]);
    return 0;
}


time_t string_to_time(char time_string[]) {
    time_t time_pointer;
    struct tm temp = {0}; // Initialize struct tm to zeros to avoid garbage values

    if (sscanf(time_string, "%d/%d/%d %d:%d:%d",
               &temp.tm_year, &temp.tm_mon, &temp.tm_mday,
               &temp.tm_hour, &temp.tm_min, &temp.tm_sec) != 6) {
        return -1;
    }

    // Adjust values to match struct tm requirements
    temp.tm_year -= 1900; // years since 1900
    temp.tm_mon--;       // months in the range 0 to 11

    return mktime(&temp);
}


typedef struct {
    int number_of_commit_in_branch, id;
    char name_of_commited_file[50], path_of_commited_file[PATH_MAX], branch_name[50], author_name[50];
    char time[100];
    char massage[100];
} data;

// todo ********************************************************
int log_function(int argc, char *argv[]) {

    char cwd[500];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd in creat config has problem");
        return 0;
    }
    // open all_commit_data
    char all_data_path[200];
    snprintf(all_data_path, sizeof(all_data_path), "%s/%s", cwd, ".neogit/branches/all_commit_data");
    FILE *all_commit_data = fopen(all_data_path, "r");


    data line[100];
    char buffer[1000];
    int count = 0;
    while (fgets(buffer, sizeof(buffer), all_commit_data) != NULL) {
//        printf("buffer: %s", buffer);
        sscanf(buffer, "%d %s %d %s %s %s /%[^/]s/ $%[^\n]\n", &line[count].id, line[count].branch_name,
               &line[count].number_of_commit_in_branch,
               line[count].name_of_commited_file, line[count].path_of_commited_file, line[count].author_name,
               line[count].time,
               line[count].massage);

        char *ret = strrchr(buffer, '$');
        for (int i = 0; i < strlen(ret); ++i) {
            line[count].massage[i] = ret[i + 1];
        }

        count++;
    }
    int start = count - 1, finish = 0;

////////////////////////////////////////////////
    if (argc > 2 && strcmp(argv[2], "-branch") == 0) {
        int match = 0;
        for (int i = count - 1; i >= 0; i--) {
            if (strcmp(line[i].branch_name, argv[3]) == 0) {
                printf("-----\nname of file: %s\ncommit ID: %d\nbranch name: %s\ntime: %s\nmassage: %s\n",
                       line[i].name_of_commited_file, line[i].id, line[i].branch_name, line[i].time, line[i].massage);
                match = 1;
            }
        }

        if (!match) {
            printf("there is no branch with that name\n");
        }
        return 0;
    }

////////////////////////////////////////////////
    if (argc > 2 && strcmp(argv[2], "-search") == 0) {
        if (argc == 4) {
            int match = 0;
            for (int i = count - 1; i >= 0; i--) {
                if (strstr(line[i].massage, argv[3]) != NULL) {
                    printf("-----\nname of file: %s\ncommit ID: %d\nbranch name: %s\ntime: %s\nmassage: %s\n",
                           line[i].name_of_commited_file, line[i].id, line[i].branch_name, line[i].time,
                           line[i].massage);
                    match = 1;
                }
            }
            if (!match) {
                printf("there is no massage with that word\n");
            }
            return 0;

        } else if (argc > 4) {
            int x = argc;
            char check[10];
            int y = 0;
            for (int j = 3; j <= argc - 1; ++j) {
                int match = 0;
                for (int i = count - 1; i >= 0; i--) {
                    if (strstr(line[i].massage, argv[j]) != NULL) {
                        if (check[i] == 1)
                            continue;
                        printf("-----\nname of file: %s\ncommit ID: %d\nbranch name: %s\ntime: %s\nmassage: %s\n",
                               line[i].name_of_commited_file, line[i].id, line[i].branch_name, line[i].time,
                               line[i].massage);
                        x--;
                        match = 1;
                        check[i] = 1;
                    }

                }
                if (!match) {
                    printf("there is no massage with that %s\n", argv[j]);
                }
            }
            return 0;
        }
////////////////////////////////////////////////
        if (argc > 2 && strcmp(argv[2], "-author") == 0) {
            int match = 0;
            for (int i = count - 1; i >= 0; i--) {
                if (strstr(line[count].author_name, argv[3]) != NULL) {
                    printf("-----\nname of file: %s\ncommit ID: %d\nbranch name: %s\ntime: %s\nmassage: %s\n",
                           line[i].name_of_commited_file, line[i].id, line[i].branch_name, line[i].time,
                           line[i].massage);
                    match = 1;
                }
            }
            if (!match) {
                printf("there is no massage with that word\n");
            }
            return 0;
        }
////////////////////////////////////////////////
        if (argc > 2 && strcmp(argv[2], "-since") == 0) {
            int x = 0;
            time_t time_input = string_to_time(argv[3]);
            for (int i = 0; i < count; ++i) {
                time_t time_data = string_to_time(line[i].time);
                if (time_data > time_input) {
                    x = i;
                }
            }
            finish = count - x;
            start = count - 1;
        }
////////////////////////////////////////////////
        if (argc > 2 && strcmp(argv[2], "-before") == 0) {
            int x = 0;
            time_t time_input = string_to_time(argv[3]);
            for (int i = 0; i < count; ++i) {
                time_t time_data = string_to_time(line[i].time);
                if (time_data > time_input) {
                    x = i - 1;
                }
            }
            finish = 0;
            start = count - x - 1;
        }
////////////////////////////////////////////////

        if (argc > 2 && strcmp(argv[2], "-n") == 0) {
            int number = atoi(argv[3]);
            finish = count - number;
            start = count - 1;
        }
////////////////////////////////////////////////
//    printf("start: %d   finish: %d\n",start,finish);
        for (int i = start; i >= finish; i--) {
            printf("-----\nname of file: %s\ncommit ID: %d\nbranch name: %s\ntime: %s\nmassage: %s\n",
                   line[i].name_of_commited_file, line[i].id, line[i].branch_name, line[i].time, line[i].massage);
        }
//    printf("----------------------------------------------------------------\n");

        printf("====\nnumber of all commits: %d\n", count);
        return 0;
    }
}

struct situation {
    char X;
    char Y;
    char name[100];
};
struct situation File[100];
int number_of_changed_files;

// todo ********************************************************
int D_function(char cwd[]) {
    char names[15][100], paths[15][PATH_MAX];
    int flag = 0;
    int i = 0;
    DIR *dir = opendir(cwd);
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            strcpy(names[i], entry->d_name);
            i++;
        }
    }
    closedir(dir);
    // open all_commit_data
    char all_data_path[200];
    snprintf(all_data_path, sizeof(all_data_path), "%s/%s", cwd_main_file, ".neogit/branches/all_commit_data");
    FILE *all_commit_data = fopen(all_data_path, "r");

    char buffer[1000];
    char path_commited_file[PATH_MAX], name_of_commited_file[100];
    int count = 0;
    while (fgets(buffer, sizeof(buffer), all_commit_data) != NULL) {

        sscanf(buffer, "%*d %*s %*d %s %s %*s /%*[^/]s/ /%*[^/\n]s/\n", name_of_commited_file, path_commited_file);
        int q = strlen(path_commited_file), w = 0;
        while (path_commited_file[q] != '/') {
            w++;
            q--;
        }
        path_commited_file[q] = '\0';
        int match = -1;
        for (int j = 0; j < i; ++j) {
            if (strcmp(path_commited_file, cwd) == 0 && strcmp(name_of_commited_file, names[j]) == 0) {
                match = 1;
                break;
            } else if (strcmp(path_commited_file, cwd) == 0 && strcmp(name_of_commited_file, names[j]) != 0) {
                match = 0;
            }
        }

        if (match == 0) {
            strcpy(File[number_of_changed_files].name, name_of_commited_file);
            File[number_of_changed_files].Y = 'D';
            flag = 1;
            if (find_file_in_stage(name_of_commited_file) == 1) {
                File[number_of_changed_files].X = '+';
            } else
                File[number_of_changed_files].X = '-';
            number_of_changed_files++;

        }

    }
    if (flag == 1)
        return 1;
    return 0;
}

// todo ********************************************************
int find_exact_data_in_all_commit(char name_of_file[], char data_string[]) {
    // open all_commit_data
    char all_data_path[200];
    snprintf(all_data_path, sizeof(all_data_path), "%s/%s", cwd_main_file, ".neogit/branches/all_commit_data");
    FILE *all_commit_data = fopen(all_data_path, "r");

    int count = 0;
    char buffer[100][1000];
    while (fgets(buffer[count], sizeof(buffer), all_commit_data) != NULL) {
        count++;
    }

    for (int i = count - 1; i >= 0; i--) {
        char name[100];
        sscanf(buffer[i], "%*d %*s %*d %s %*s %*s /%*[^/]s/ /%*[^/\n]s/\n", name);
        if (strcmp(name, name_of_file) == 0) {
            strcpy(data_string, buffer[i]);
            fclose(all_commit_data);
            return 1;
        }
    }
    fclose(all_commit_data);
    return 0;
}

// todo ********************************************************
int M_A_unction(char cwd[]) {
    DIR *dir = opendir(cwd);
    struct dirent *entry;
    int flag = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {

            char data_string[1000];
            if (find_exact_data_in_all_commit(entry->d_name, data_string) == 1) {
                // open file in commit
                int number_of_commit_in_branch;
                char branch_name[100], path_of_commited_file[PATH_MAX], name_of_commited_file[100];
                sscanf(data_string, "%*d %s %d %s %s %*s /%*[^/]s/ /%*[^/\n]s/\n", branch_name,
                       &number_of_commit_in_branch,
                       name_of_commited_file,
                       path_of_commited_file);

                char file_in_commit_path[PATH_MAX];
                snprintf(file_in_commit_path, sizeof(file_in_commit_path), "%s/%s/%s/%d/%s", cwd_main_file,
                         ".neogit/branches/",
                         branch_name, number_of_commit_in_branch,
                         name_of_commited_file);
                FILE *file_in_commit = fopen(file_in_commit_path, "r");

                // open file in folder
                char file_in_folder_path[PATH_MAX];
                snprintf(file_in_folder_path, sizeof(file_in_folder_path), "%s/%s", cwd, entry->d_name);
                FILE *file_in_folder = fopen(file_in_folder_path, "r");

                if (compare_tow_files(file_in_commit, file_in_folder) == 0) {
                    strcpy(File[number_of_changed_files].name, entry->d_name);
                    File[number_of_changed_files].Y = 'M';
                    flag = 1;
                    if (find_file_in_stage(entry->d_name) == 1) {
                        File[number_of_changed_files].X = '+';
                    } else {
                        File[number_of_changed_files].X = '-';
                    }
                    number_of_changed_files++;

                }
            } else {
                strcpy(File[number_of_changed_files].name, entry->d_name);
                File[number_of_changed_files].Y = 'A';
                flag = 1;
                if (find_file_in_stage(entry->d_name) == 1) {
                    File[number_of_changed_files].X = '+';
                } else {
                    File[number_of_changed_files].X = '-';
                }
                number_of_changed_files++;

            }

        }
    }
    if (flag == 1)
        return 1;
    return 0;
}

int file_permission(char file1_path[], char file2_path[]) {

    struct stat file1_stat, file2_stat;

    if (stat(file1_path, &file1_stat) == 0 && stat(file2_path, &file2_stat) == 0) {
        if (file1_stat.st_mode == file2_stat.st_mode) {
            return 0;
        } else {
            return 1;
        }
    } else {
        perror("Error getting file stats");
    }
}


int T_function(char cwd[]) {

    DIR *dir = opendir(cwd);
    struct dirent *entry;
    int flag = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {

            char data_string[1000];
            if (find_exact_data_in_all_commit(entry->d_name, data_string) == 1) {
                // open file in commit
                int number_of_commit_in_branch;
                char branch_name[100], path_of_commited_file[PATH_MAX], name_of_commited_file[100];
                sscanf(data_string, "%*d %s %d %s %s %*s /%*[^/]s/ /%*[^/\n]s/\n", branch_name,
                       &number_of_commit_in_branch,
                       name_of_commited_file,
                       path_of_commited_file);

                char file_in_commit_path[PATH_MAX];
                snprintf(file_in_commit_path, sizeof(file_in_commit_path), "%s/%s/%s/%d/%s", cwd_main_file,
                         ".neogit/branches/",
                         branch_name, number_of_commit_in_branch,
                         name_of_commited_file);
                FILE *file_in_commit = fopen(file_in_commit_path, "r");

                // open file in folder
                char file_in_folder_path[PATH_MAX];
                snprintf(file_in_folder_path, sizeof(file_in_folder_path), "%s/%s", cwd, entry->d_name);
                FILE *file_in_folder = fopen(file_in_folder_path, "r");

                if (file_permission(file_in_folder_path, file_in_commit_path) == 1) {
                    strcpy(File[number_of_changed_files].name, entry->d_name);
                    File[number_of_changed_files].Y = 'T';
                    flag = 1;
                    if (find_file_in_stage(entry->d_name) == 1) {
                        File[number_of_changed_files].X = '+';
                    } else
                        File[number_of_changed_files].X = '-';
                    number_of_changed_files++;
                }


            }
        }
    }

    return 0;
}

// todo ********************************************************
int status_function(char cwd[]) {

    D_function(cwd);
    M_A_unction(cwd);
    T_function(cwd);
    DIR *dir = opendir(cwd);
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 &&
            strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".neogit") != 0) {
            char dicdic_path[PATH_MAX];
            snprintf(dicdic_path, sizeof(dicdic_path), "%s/%s", cwd, entry->d_name);
            status_function(dicdic_path);
        }
    }
    return 0;
}

// todo ********************************************************
int print_status_result() {
    for (int i = 0; i < number_of_changed_files; ++i) {
        printf("%s %c%c\n----\n", File[i].name, File[i].X, File[i].Y);
    }
    return 0;
}

// todo ********************************************************
void change_branch_name(char cwd[], char branch_name[]) {
    char config_path[200];
    int id;
    snprintf(config_path, sizeof(config_path), "%s/%s", cwd, ".neogit/config");
    FILE *config = fopen(config_path, "r+");
    // scan data from old local config
    char username[50], email[50], branch[50];
    char last_id, current_id;
    fscanf(config, "username: %s\nemail: %s\nfinal ID: %c\ncurrent commit_ID: %c\nbranch: %s", username,
           email, &last_id, &current_id, branch);
    fclose(config);
    // put data
    FILE *config2 = fopen(config_path, "w");
    fprintf(config2, "username: %s\nemail: %s\nfinal ID: %c\ncurrent commit_ID: %c\nbranch: %s",
            username,
            email, last_id, current_id, branch_name);
    fclose(config2);

}

// todo ********************************************************
int branch_function(int argc, char *argv[], char cwd[]) {
    // check if we had ths branch already
    char branches_path[PATH_MAX];
    snprintf(branches_path, sizeof(branches_path), "%s/%s", cwd, ".neogit/branches");
    DIR *branches = opendir(branches_path);

    if (argc == 2) {
        struct dirent *entry;
        while ((entry = readdir(branches)) != NULL) {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 &&
                strcmp(entry->d_name, "..") != 0) {
                printf("%s\n----\n", entry->d_name);
            }
        }
        return 0;
    }

    if (argc == 3) {
        struct dirent *entry;
        while ((entry = readdir(branches)) != NULL) {
            if (strcmp(entry->d_name, argv[2]) == 0) {
                printf("this name has already been used\n");
                return 0;
            }
        }
        char pre_branch_name[100], str[100];
        find_branch_author(cwd, pre_branch_name, str);
        int number_of_last_commit = find_number_of_last_commit_in_branch(cwd, pre_branch_name);
        change_branch_name(cwd, argv[2]);
//        int number_of_first_commit_ID = find_number_of_last_id_in_branch(pre_branch_name);

        // dir and config for master
        char branch_in_commit_path[PATH_MAX];
        snprintf(branch_in_commit_path, sizeof(branch_in_commit_path), "%s/%s/%s", cwd, ".neogit/branches",
                 argv[2]);
        if (mkdir(branch_in_commit_path, 0755) != 0) return 1;
        char branch_config_in_commit_path[PATH_MAX];
        snprintf(branch_config_in_commit_path, sizeof(branch_config_in_commit_path), "%s/%s/config_%s", cwd,
                 ".neogit/branches", argv[2]);
        FILE *file5 = fopen(branch_config_in_commit_path, "w");
        fprintf(file5, "%d\n0", number_of_last_commit);
        printf("number of last commit in creat branch: %d\n", number_of_last_commit);

        fclose(file5);

        char pre_branch_path[PATH_MAX];
        snprintf(pre_branch_path, sizeof(pre_branch_path), "%s/%s/%s", cwd, ".neogit/branches", pre_branch_name);
        DIR *dir = opendir(pre_branch_path);
        struct dirent *pre_entry;

        while ((pre_entry = readdir(dir)) != NULL) {
            if (pre_entry->d_type == DT_DIR && strcmp(pre_entry->d_name, ".") != 0 &&
                strcmp(pre_entry->d_name, "..") != 0) {
                char system_command[PATH_MAX];
                // todo correct system output
                snprintf(system_command, sizeof(system_command), "rsync %s/%s %s/%s/%s", pre_branch_path,
                         pre_entry->d_name, cwd, ".neogit/branches", argv[2]);
                system(system_command);
            }

        }
    }
    printf("branch added successfully\n");

    closedir(branches);
    return 0;
}

// todo ********************************************************
int
find_last_commited_files_in_exact_branch(char name_of_branch[], int number_of_last_commit,
                                         char data_string[][1000]) {
    // open all_commit_data
    char all_data_path[200];
    snprintf(all_data_path, sizeof(all_data_path), "%s/%s", cwd_main_file, ".neogit/branches/all_commit_data");
    FILE *all_commit_data = fopen(all_data_path, "r");
    int count = 0;
    char buffer[100][1000];
    while (fgets(buffer[count], sizeof(buffer), all_commit_data) != NULL) {
        count++;
    }
    int x = 0;
    for (int i = count - 1; i >= 0; i--) {
        char name[100];
        int num = 0;
        sscanf(buffer[i], "%*d %s %d %*s %*s %*s /%*[^/]s/ /%*[^/\n]s/\n", name, &num);
        if (strcmp(name, name_of_branch) == 0 && num == number_of_last_commit) {
            strcpy(data_string[x], buffer[i]);
            x++;
        }
    }

    fclose(all_commit_data);
    printf("x: %d", x);
    return x;
}

//todo ********************************************************
int delete_files_appended_after(char cwd[], int last_branch_id, char branch_name[]) {
    DIR *dir = opendir(cwd);
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            int match = 0;
            char unknown_file_path[PATH_MAX];
            snprintf(unknown_file_path, sizeof(unknown_file_path), "%s/%s", cwd, entry->d_name);

            // open all_commit_data
            char all_data_path[200];
            snprintf(all_data_path, sizeof(all_data_path), "%s/%s", cwd_main_file,
                     ".neogit/branches/all_commit_data");
            FILE *all_commit_data = fopen(all_data_path, "r");

            int count = 0;
            char buffer[100][1000];
            char b_name[100][100], path[100][PATH_MAX];
            int id;
            while (fgets(buffer[count], sizeof(buffer), all_commit_data) != NULL) {
                sscanf(buffer[count], "%d %s %*d %*s %s %*s /%*[^/]s/ /%*[^/\n]s/\n", &id, b_name[count],
                       path[count]);
                if (id > last_branch_id)
                    break;
                count++;
            }
            if (strcmp(branch_name, "master") == 0) {
                for (int i = count - 1; i >= 0; i--) {
                    if (strcmp(path[i], unknown_file_path) == 0 && strcmp(b_name[i], "master") == 0) {
                        match = 1;
                        break;
                    }
                }
                perror("[][]");
            } else {
                for (int i = count - 1; i >= 0; i--) {
                    if (strcmp(path[i], unknown_file_path) == 0) {
                        match = 1;
                        break;
                    }
                }
            }
            fclose(all_commit_data);
            if (!match) {
                remove(unknown_file_path);
            }

        } else if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 &&
                   strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".neogit") != 0) {
            char dicdic_path[PATH_MAX];
            snprintf(dicdic_path, sizeof(dicdic_path), "%s/%s", cwd, entry->d_name);
            delete_files_appended_after(dicdic_path, last_branch_id, branch_name);
        }
    }
    return 0;
}

int find_exact_data_in_exact_branch(char name_of_file[], char branch_name[], char data_string[]) {
    // open all_commit_data
    char all_data_path[200];
    snprintf(all_data_path, sizeof(all_data_path), "%s/%s", cwd_main_file, ".neogit/branches/all_commit_data");
    FILE *all_commit_data = fopen(all_data_path, "r");

    int count = 0;
    char buffer[100][1000];
    while (fgets(buffer[count], sizeof(buffer), all_commit_data) != NULL) {
        count++;
    }

    for (int i = count - 1; i >= 0; i--) {
        char name[100], b_name[100];
        sscanf(buffer[i], "%*d %s %*d %s %*s %*s /%*[^/]s/ /%*[^/\n]s/\n", b_name, name);
        if (strcmp(name, name_of_file) == 0 && strcmp(b_name, branch_name) == 0) {
            strcpy(data_string, buffer[i]);
            fclose(all_commit_data);
            return 1;
        }
    }
    fclose(all_commit_data);
    return 0;
}


int check_possiblity_for_checkout(char cwd[], char branch_name[]) {
    if (M_A_unction(cwd) == 0) {
        DIR *dir = opendir(cwd);
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 &&
                strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".neogit") != 0) {
                char dicdic_path[PATH_MAX];
                snprintf(dicdic_path, sizeof(dicdic_path), "%s/%s", cwd, entry->d_name);
                check_possiblity_for_checkout(dicdic_path, branch_name);
            }
        }
        return 0;
    }
    return 1;
}

void put_current_into_head() {
    char config_path[200];
    int id;
    snprintf(config_path, sizeof(config_path), "%s/%s", cwd_main_file, ".neogit/config");
    FILE *config = fopen(config_path, "r");
    char username[50], email[50], branch_name[1000];
    char last_id, current_id;
    fscanf(config, "username: %s\nemail: %s\nfinal ID: %c\ncurrent commit_ID: %c\nbranch: %s", username,
           email, &last_id, &current_id, branch_name);
    fclose(config);

    int head = find_number_of_last_id_in_branch(branch_name);

    FILE *config2 = fopen(config_path, "w");
    fprintf(config2, "username: %s\nemail: %s\nfinal ID: %c\ncurrent commit_ID: %d\nbranch: %s", username,
            email, last_id, head, branch_name);
    fclose(config2);

}


// todo ********************************************************
int checkout_branch(int argc, char *argv[], char cwd[]) {
    // change branch name in config
    change_branch_name(cwd, argv[2]);

    put_current_into_head();

    char pre_branch_name[100], str[100];
    find_branch_author(cwd, pre_branch_name, str);

    if (check_possiblity_for_checkout(cwd, pre_branch_name) == 1) {
        printf("you have to commit first\n");
        return 0;
    }

    char data_string[100][1000];
    int number_of_last_commit_in_branch = find_number_of_last_commit_in_branch(cwd, argv[2]);
    printf("number of last commit: %d", number_of_last_commit_in_branch);
    int number_of_file = find_last_commited_files_in_exact_branch(argv[2], number_of_last_commit_in_branch,
                                                                  data_string);

    // change files which didn't delete
    for (int i = 0; i < number_of_file; ++i) {
        char path_in_folder[PATH_MAX], name[100];
        // scan data
        sscanf(data_string[i], "%*d %*s %*d %s %s %*s /%*[^/]s/ /%*[^/\n]s/\n", name, path_in_folder);

        // remove file
        remove(path_in_folder);

        // creat path in branch
        char path_in_branch[PATH_MAX];
        snprintf(path_in_branch, sizeof(path_in_branch), "%s/%s/%s/%d/%s", cwd, ".neogit/branches", argv[2],
                 number_of_last_commit_in_branch, name);

        // add file
        char command[PATH_MAX];
        snprintf(command, sizeof(command), "cp %s %s", path_in_branch, path_in_folder);
        system(command);
    }
    // check if there is a file that appended after this commit
    int last_id_in_branch = find_number_of_last_id_in_branch(argv[2]);
    delete_files_appended_after(cwd, last_id_in_branch, argv[2]);
    // recover deleted file after this commit
//    recover_deleted_file_after_this_commit(cwd,last_id_in_branch,argv[2]);


    return 0;
}

// todo ********************************************************
int find_commits_with_exact_id(int id, char data_string[][1000]) {
    // open all_commit_data
    char all_data_path[200];
    snprintf(all_data_path, sizeof(all_data_path), "%s/%s", cwd_main_file, ".neogit/branches/all_commit_data");
    FILE *all_commit_data = fopen(all_data_path, "r");
    int count = 0;
    char buffer[100][1000];
    while (fgets(buffer[count], sizeof(buffer), all_commit_data) != NULL) {
        count++;
    }
    int x = 0;
    for (int i = count - 1; i >= 0; i--) {
        int id_buf;
        sscanf(buffer[i], "%d %*s %*d %*s %*s %*s /%*[^/]s/ /%*[^/\n]s/\n", &id_buf);
        if (id == id_buf) {
            strcpy(data_string[x], buffer[i]);
            x++;
        }
    }

    fclose(all_commit_data);
    return x;
}

// todo ********************************************************
void change_cID_lID(char *argv[], char cwd[]) {
    char config_path[200];
    snprintf(config_path, sizeof(config_path), "%s/%s", cwd, ".neogit/config");
    FILE *config = fopen(config_path, "r+");
    char username[50], email[50], branch_name[1000];
    char last_id, current_id;
    fscanf(config, "username: %s\nemail: %s\nfinal ID: %c\ncurrent commit_ID: %c\nbranch: %s", username,
           email, &last_id, &current_id, branch_name);
    fclose(config);

    FILE *config2 = fopen(config_path, "w");
    int current_id_int = atoi(argv[2]);


    fprintf(config2, "username: %s\nemail: %s\nfinal ID: %c\ncurrent commit_ID: %d\nbranch: %s",
            username, email, last_id, current_id_int, branch_name);
    fclose(config2);

}

//todo ********************************************************
int delete_files_appended_after_for_id(char cwd[], int last_id) {
    printf("last id in delete: %d\n", last_id);
    DIR *dir = opendir(cwd);
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            int match = 0;
            char unknown_file_path[PATH_MAX];
            snprintf(unknown_file_path, sizeof(unknown_file_path), "%s/%s", cwd, entry->d_name);

            // open all_commit_data
            char all_data_path[200];
            snprintf(all_data_path, sizeof(all_data_path), "%s/%s", cwd_main_file,
                     ".neogit/branches/all_commit_data");
            FILE *all_commit_data = fopen(all_data_path, "r");

            int count = 0;
            char buffer[100][1000];
            char b_name[100][100], path[100][PATH_MAX];
            int id;
            while (fgets(buffer[count], sizeof(buffer), all_commit_data) != NULL) {
                sscanf(buffer[count], "%d %s %*d %*s %s %*s /%*[^/]s/ /%*[^/\n]s/\n", &id, b_name[count],
                       path[count]);
                if (id > last_id)
                    break;
                count++;
            }

            for (int i = count - 1; i >= 0; i--) {
                if (strcmp(path[i], unknown_file_path) == 0) {
                    match = 1;
                    break;
                }
            }

            fclose(all_commit_data);
            if (!match) {
                remove(unknown_file_path);
            }

        } else if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 &&
                   strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".neogit") != 0) {
            char dicdic_path[PATH_MAX];
            snprintf(dicdic_path, sizeof(dicdic_path), "%s/%s", cwd, entry->d_name);
            delete_files_appended_after_for_id(dicdic_path, last_id);
        }
    }
    return 0;
}

// todo ********************************************************
int checkout_head(int argc, char *argv[], char cwd[]) {
    char config_path[200];
    int id;
    snprintf(config_path, sizeof(config_path), "%s/%s", cwd_main_file, ".neogit/config");
    FILE *config = fopen(config_path, "r");
    char username[50], email[50], branch_name[1000];
    char last_id, current_id;
    fscanf(config, "username: %s\nemail: %s\nfinal ID: %c\ncurrent commit_ID: %c\nbranch: %s", username,
           email, &last_id, &current_id, branch_name);
    fclose(config);

    // change branch name in config
    change_branch_name(cwd, branch_name);

    put_current_into_head();

    char pre_branch_name[100], str[100];
    find_branch_author(cwd, pre_branch_name, str);

    if (check_possiblity_for_checkout(cwd, pre_branch_name) == 1) {
        printf("you have to commit first\n");
        return 0;
    }

    char data_string[100][1000];
    int number_of_last_commit_in_branch = find_number_of_last_commit_in_branch(cwd, branch_name);
    printf("number of last commit: %d", number_of_last_commit_in_branch);
    int number_of_file = find_last_commited_files_in_exact_branch(branch_name, number_of_last_commit_in_branch,
                                                                  data_string);
    // change files which didn't delete
    for (int i = 0; i < number_of_file; ++i) {
        char path_in_folder[PATH_MAX], name[100];
        // scan data
        sscanf(data_string[i], "%*d %*s %*d %s %s %*s /%*[^/]s/ /%*[^/\n]s/\n", name, path_in_folder);

        // remove file
        remove(path_in_folder);

        // creat path in branch
        char path_in_branch[PATH_MAX];
        snprintf(path_in_branch, sizeof(path_in_branch), "%s/%s/%s/%d/%s", cwd, ".neogit/branches", branch_name,
                 number_of_last_commit_in_branch, name);

        // add file
        char command[PATH_MAX];
        snprintf(command, sizeof(command), "cp %s %s", path_in_branch, path_in_folder);
        system(command);
    }
    // check if there is a file that appended after this commit
    int last_id_in_branch = find_number_of_last_id_in_branch(branch_name);
    delete_files_appended_after(cwd, last_id_in_branch, branch_name);

    return 0;
}

// todo ********************************************************
int checkout_id(int argc, char *argv[], char cwd[], int id) {

    char pre_branch_name[100], str[100];
    find_branch_author(cwd, pre_branch_name, str);

    if (check_possiblity_for_checkout(cwd, pre_branch_name) == 1) {
        printf("you have to commit first\n");
        return 0;
    }

    // change commit id char to int

    // find files with that commit id
    char data_string[100][1000];
    int number_of_files = find_commits_with_exact_id(id, data_string);

    // change files which didn't delete
    for (int i = 0; i < number_of_files; ++i) {
        char path_in_folder[PATH_MAX], name[100], branch_name[100];
        int commit_num_in_branch;

        // scan data
        sscanf(data_string[i], "%*d %s %d %s %s %*s /%*[^/]s/ /%*[^/\n]s/\n", branch_name, &commit_num_in_branch,
               name,
               path_in_folder);

        // remove file
        remove(path_in_folder);

        // creat path in branch
        char path_in_branch[PATH_MAX];
        snprintf(path_in_branch, sizeof(path_in_branch), "%s/%s/%s/%d/%s", cwd, ".neogit/branches", branch_name,
                 commit_num_in_branch, name);

        // add file
        char command[PATH_MAX];
        snprintf(command, sizeof(command), "cp %s %s", path_in_branch, path_in_folder);
        system(command);
    }
    // check if there is a file that appended after this commit
    delete_files_appended_after_for_id(cwd, id);

    // change current id in config
    change_cID_lID(argv, cwd);
    return 0;
}

// todo ********************************************************
int checkout_function(int argc, char *argv[], char cwd[]) {
    if (strcmp(argv[2], "HEAD") == 0) {
        checkout_head(argc, argv, cwd);
        return 1;
    }
    if (strlen(argv[2]) > 2) {
        checkout_branch(argc, argv, cwd);
    } else {
        int id = atoi(argv[2]);
        checkout_id(argc, argv, cwd, id);
    }
    return 0;
}

// todo ********************************************************
void write_and_copy_new_info_in_all_commit(char cwd[], int last_id) {
    printf("**last id: %d\n", last_id);
    // open all_commit_data
    char all_data_path[200];
    snprintf(all_data_path, sizeof(all_data_path), "%s/%s", cwd_main_file, ".neogit/branches/all_commit_data");
    FILE *all_commit_data = fopen(all_data_path, "a+");

    int count = 0;
    char buffer[100][1000], branch_name[100][100];
    char name[100][100], path[100][PATH_MAX], massage[100][100], author[100][50];
    int id, commit_num;
    while (fgets(buffer[count], sizeof(buffer), all_commit_data) != NULL) {
        printf("buffer: %s\n", buffer[count]);
        sscanf(buffer[count], "%d %s %d %s %s %s /%*[^/]s/ /%[^\n]s/\n", &id, branch_name[count], &commit_num,
               name[count],
               path[count], author[count], massage[count]);
        printf("id: %d\n", id);
        if (id != last_id)
            continue;
        printf("count: %d\n", count);

        printf("branch: %s\n", branch_name[count]);

        count++;
    }

    // new time
    time_t rawtime;
    struct tm *time_info;
    time(&rawtime);
    time_info = localtime(&rawtime);
    char time_string[500];
    strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", time_info);

    // new num of commit in branch
    int number_of_last_commit = find_number_of_last_commit_in_branch(cwd, branch_name[0]);

    // ++ num of commit
    change_number_of_commit_in_branch(cwd, branch_name[0]);

    // new id
    int new_id = find_id_and_plus(cwd);

    for (int i = 0; i < count; ++i) {
        fprintf(all_commit_data, "%d %s %d %s %s %s /%s/ /%s/\n", new_id, branch_name[i],
                number_of_last_commit + 1, name[i], path[i], author[i], time_string, massage[0]);
    }

    // copy file and rename
    char new_commit_path[PATH_MAX];
    snprintf(new_commit_path, sizeof(new_commit_path), "%s/%s/%s/%d", cwd, ".neogit/branches", branch_name[0],
             number_of_last_commit + 1);
    mkdir(new_commit_path, 0755);

    char pre_commit_path[PATH_MAX];
    snprintf(pre_commit_path, sizeof(pre_commit_path), "%s/%s/%s/%d", cwd, ".neogit/branches", branch_name[0],
             commit_num);
    printf("pre commit path: %s\n", pre_commit_path);
    DIR *dir = opendir(pre_commit_path);
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char source[PATH_MAX];
            snprintf(source, sizeof(source), "%s/%s/%s/%d/%s", cwd, ".neogit/branches", branch_name[0], commit_num,
                     entry->d_name);

            char command[PATH_MAX];
            snprintf(command, sizeof(command), "cp %s %s", source, new_commit_path);
            system(command);
        }
    }
    closedir(dir);
}

int revert(int argc, char *argv[], char cwd[], int id) {

    char pre_branch_name[100], str[100];
    find_branch_author(cwd, pre_branch_name, str);

    if (check_possiblity_for_checkout(cwd, pre_branch_name) == 1) {
        printf("you have to commit first\n");
        return 0;
    }
    // change commit id char to int

    // find files with that commit id
    char data_string[100][1000];
    int number_of_files = find_commits_with_exact_id(id, data_string);

    // change files which didn't delete
    for (int i = 0; i < number_of_files; ++i) {
        char path_in_folder[PATH_MAX], name[100], branch_name[100];
        int commit_num_in_branch;

        // scan data
        sscanf(data_string[i], "%*d %s %d %s %s %*s /%*[^/]s/ /%*[^/\n]s/\n", branch_name, &commit_num_in_branch,
               name,
               path_in_folder);

        // remove file
        remove(path_in_folder);

        // creat path in branch
        char path_in_branch[PATH_MAX];
        snprintf(path_in_branch, sizeof(path_in_branch), "%s/%s/%s/%d/%s", cwd, ".neogit/branches", branch_name,
                 commit_num_in_branch, name);

        // add file
        char command[PATH_MAX];
        snprintf(command, sizeof(command), "cp %s %s", path_in_branch, path_in_folder);
        system(command);
    }

    // check if there is a file that appended after this commit
    delete_files_appended_after_for_id(cwd, id);

    write_and_copy_new_info_in_all_commit(cwd, id);

    return 0;
}

// todo ////////////////////////////////////////////////
void write_and_copy_new_info_in_all_commit_m(char cwd[], int last_id, char *argv[]) {
    // open all_commit_data
    char all_data_path[200];
    snprintf(all_data_path, sizeof(all_data_path), "%s/%s", cwd_main_file, ".neogit/branches/all_commit_data");
    FILE *all_commit_data = fopen(all_data_path, "a+");


    int count = 0;
    char buffer[100][1000], branch_name[100][100];
    char name[100][100], path[100][PATH_MAX], massage[100], author[100][50];
    int id, commit_num;
    while (fgets(buffer[count], sizeof(buffer), all_commit_data) != NULL) {
        printf("buffer: %s\n", buffer[count]);
        sscanf(buffer[count], "%d %s %d %s %s %s /%*[^/]s/ /%*[^\n]s/\n", &id, branch_name[count], &commit_num,
               name[count],
               path[count], author[count]);
        printf("id: %d\n", id);
        if (id != last_id)
            continue;
        printf("count: %d\n", count);

        printf("branch: %s\n", branch_name[count]);

        count++;
    }

    // new time
    time_t rawtime;
    struct tm *time_info;
    time(&rawtime);
    time_info = localtime(&rawtime);
    char time_string[500];
    strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", time_info);

    // new num of commit in branch
    int number_of_last_commit = find_number_of_last_commit_in_branch(cwd, branch_name[0]);

    // ++ num of commit
    change_number_of_commit_in_branch(cwd, branch_name[0]);

    // new id
    int new_id = find_id_and_plus(cwd);

    strcpy(massage, argv[3]);


    for (int i = 0; i < count; ++i) {
        fprintf(all_commit_data, "%d %s %d %s %s %s /%s/ /%s/\n", new_id, branch_name[i],
                number_of_last_commit + 1, name[i], path[i], author[i], time_string, massage);
    }

    // copy file and rename
    char new_commit_path[PATH_MAX];
    snprintf(new_commit_path, sizeof(new_commit_path), "%s/%s/%s/%d", cwd, ".neogit/branches", branch_name[0],
             number_of_last_commit + 1);
    mkdir(new_commit_path, 0755);
    perror("][][");

    char pre_commit_path[PATH_MAX];
    snprintf(pre_commit_path, sizeof(pre_commit_path), "%s/%s/%s/%d", cwd, ".neogit/branches", branch_name[0],
             commit_num);
    printf("pre commit path: %s\n", pre_commit_path);
    DIR *dir = opendir(pre_commit_path);
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char source[PATH_MAX];
            snprintf(source, sizeof(source), "%s/%s/%s/%d/%s", cwd, ".neogit/branches", branch_name[0], commit_num,
                     entry->d_name);

            char command[PATH_MAX];
            snprintf(command, sizeof(command), "cp %s %s", source, new_commit_path);
            system(command);
        }
    }
    closedir(dir);
}

int revert_m(int argc, char *argv[], char cwd[], int id) {

    char pre_branch_name[100], str[100];
    find_branch_author(cwd, pre_branch_name, str);

    if (check_possiblity_for_checkout(cwd, pre_branch_name) == 1) {
        printf("you have to commit first\n");
        return 0;
    }
    // change commit id char to int

    // find files with that commit id
    char data_string[100][1000];
    int number_of_files = find_commits_with_exact_id(id, data_string);

    // change files which didn't delete
    for (int i = 0; i < number_of_files; ++i) {
        char path_in_folder[PATH_MAX], name[100], branch_name[100];
        int commit_num_in_branch;

        // scan data
        sscanf(data_string[i], "%*d %s %d %s %s %*s /%*[^/]s/ /%*[^/\n]s/\n", branch_name, &commit_num_in_branch,
               name,
               path_in_folder);

        // remove file
        remove(path_in_folder);

        // creat path in branch
        char path_in_branch[PATH_MAX];
        snprintf(path_in_branch, sizeof(path_in_branch), "%s/%s/%s/%d/%s", cwd, ".neogit/branches", branch_name,
                 commit_num_in_branch, name);

        // add file
        char command[PATH_MAX];
        snprintf(command, sizeof(command), "cp %s %s", path_in_branch, path_in_folder);
        system(command);
    }

    // check if there is a file that appended after this commit
    delete_files_appended_after_for_id(cwd, id);

    write_and_copy_new_info_in_all_commit_m(cwd, id, argv);

    return 0;
}

// todo ********************************************************
int revert_function(int argc, char *argv[], char cwd[]) {
    if (argc == 3) {
        int id = atoi(argv[2]);
        return revert(argc, argv, cwd, id);

    } else if (strcmp(argv[2], "-m") == 0) {
        int id = atoi(argv[4]);
        return revert_m(argc, argv, cwd, id);

    } else if (strcmp(argv[2], "-n") == 0) {
        if (argc == 4) {
            int id = atoi(argv[2]);
            return checkout_id(argc, argv, cwd, id);
        } else if (argc == 3) {
            int id = find_final_id(cwd);
            return checkout_id(argc, argv, cwd, id);
        }

    } else if (strstr(argv[argc - 1], "HEAD") != NULL) {
        int num;
        sscanf(argv[argc - 1], "HEAD-%d", &num);
        int final = find_final_id(cwd);

        if (strcmp(argv[2], "-m") == 0) {
            return revert_m(argc, argv, cwd, final - num);
        } else {
            return revert(argc, argv, cwd, final - num);
        }

    }
    printf("invalid command");
    return 0;
}

// todo ********************************************************
int tag_show(int argc, char *argv[], char cwd[]) {
    // open tag
    char tag_path[200];
    snprintf(tag_path, sizeof(tag_path), "%s/%s", cwd_main_file, ".neogit/branches/tag");
    FILE *tag = fopen(tag_path, "r");

    char line[MAX_LINE];
    int match = 0, id;
    char author_name[MAX_LINE], date[MAX_LINE], massage[MAX_LINE];
    while (fgets(line, sizeof(line), tag)) {
        char name[MAX_LINE];
        sscanf(line, "%s %*[^\0]s", name);
        if (strcmp(argv[3], name) == 0) {
            int num = sscanf(line, "%s +%d+ *%[^*]* (%[^)]) /%[^*+/]/\n", name, &id, author_name, date, massage);
            if (num == 5) {
                printf("Tag %s\n", name);
                printf("Commit %d\n", id);
                printf("Author %s\n", author_name);
                printf("Date: %s\n", date);
                printf("Message: %s\n", massage);
                return 0;
            }
            printf("Tag %s\n", name);
            printf("Commit %d\n", id);
            printf("Author %s\n", author_name);
            printf("Date: %s\n", date);
//            printf("Message: %s\n",massage);
            return 0;
        }
    }
    return 0;
}


// todo ********************************************************
void swap(char a[], char b[]) {
    char temp[MAX_LINE];
    strcpy(temp, a);
    strcpy(a, b);
    strcpy(b, temp);
}

int tag(int argc, char *argv[], char cwd[]) {
    // open tag
    char tag_path[200];
    snprintf(tag_path, sizeof(tag_path), "%s/%s", cwd_main_file, ".neogit/branches/tag");
    FILE *tag = fopen(tag_path, "r");

    int count = 0;
    char line[MAX_LINE];
    char names[100][MAX_LINE];

    while (fgets(line, sizeof(line), tag) != NULL) {
        sscanf(line, "%s %*[^\0]s", names[count]);
        count++;
    }

    fclose(tag);

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(names[j], names[j + 1]) > 0) {
                swap(names[j], names[j + 1]);
            }
        }
    }


    for (int i = 0; i < count; i++) {
        printf("%s\n", names[i]);
    }
    return 0;
}

// todo ********************************************************
int tag_a(int argc, char *argv[], char cwd[]) {
    printf("argc= %d\n", argc);
    // open tag
    char tag_path[200];
    snprintf(tag_path, sizeof(tag_path), "%s/%s", cwd_main_file, ".neogit/branches/tag");
    FILE *tag = fopen(tag_path, "r");

    char copy_path[200];
    snprintf(copy_path, sizeof(copy_path), "%s/%s", cwd_main_file, ".neogit/branches/tag_copy");
    FILE *copy = fopen(copy_path, "w");

    if (tag == NULL || copy == NULL) {
        perror("Error opening file");
        return 0;
    }

    int id = find_current_id(cwd);

    char str[MAX_LINE], author_name[MAX_LINE];
    find_branch_author(cwd, str, author_name);

    // new time
    time_t rawtime;
    struct tm *time_info;
    time(&rawtime);
    time_info = localtime(&rawtime);
    char time_string[500];
    strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", time_info);


    char line[MAX_LINE];
    int match = 0;
    while (fgets(line, sizeof(line), tag)) {
        char name[MAX_LINE];
        sscanf(line, "%s %*[^\0]", name);

        if (strcmp(name, argv[3]) == 0) {

            if (strcmp(argv[argc - 1], "-f") == 0) {
                match = 1;
                if (argc == 9) {

                    fprintf(copy, "%s +%s+ *%s* (%s) /%s/\n", argv[3], argv[7], author_name, time_string, argv[5]);

                } else if (argc == 7) {

                    if (strcmp(argv[4], "-m") == 0) {
                        fprintf(copy, "%s +%d+ *%s* (%s) /%s/\n", argv[3], id, author_name, time_string, argv[5]);
                    } else if (strcmp(argv[4], "-c") == 0) {
                        fprintf(copy, "%s +%s+ *%s* (%s)\n", argv[3], argv[5], author_name, time_string);
                    }

                } else if (argc == 5) {

                    fprintf(copy, "%s +%d+ *%s* (%s)\n", argv[3], id, author_name, time_string);

                } else {

                    printf("invalid command");

                }

            } else {
                printf("this tag has been used already");
                return 0;
            }
        } else {
            fprintf(copy, "%s", line);
        }

    }

    if (!match) {
        if (argc >= 8) {

            fprintf(copy, "%s +%s+ *%s* (%s) /%s/\n", argv[3], argv[7], author_name, time_string, argv[5]);


        } else if (argc >= 6) {

            if (strcmp(argv[4], "-m") == 0) {
                fprintf(copy, "%s +%d+ *%s* (%s) /%s/\n", argv[3], id, author_name, time_string, argv[5]);
            } else if (strcmp(argv[4], "-c") == 0) {
                fprintf(copy, "%s +%s+ *%s* (%s)\n", argv[3], argv[5], author_name, time_string);
            }

        } else if (argc >= 4) {

            fprintf(copy, "%s +%d+ *%s* (%s)\n", argv[3], id, author_name, time_string);

        } else {

            printf("invalid command\n");

        }
    }

    fclose(copy);
    fclose(tag);

    remove(tag_path);
    rename(copy_path, tag_path);
    return 0;
}

int tag_function(int argc, char *argv[], char cwd[]) {
    if (argc == 2) {
        return tag(argc, argv, cwd);
    } else if (strcmp(argv[2], "-a") == 0) {
        return tag_a(argc, argv, cwd);
    } else if (strcmp(argv[2], "show") == 0) {
        tag_show(argc, argv, cwd);
    } else
        printf("invalid command\n");
    return 0;

}


// todo ********************************************************
int grep_file(int argc, char *argv[], char cwd[]) {
    FILE *file = fopen(argv[3], "r");
    char line[MAX_LINE];
    int count = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        count++;
        if (strstr(line, argv[5]) != NULL) {
            if (strcmp(argv[argc - 1], "-n") == 0)
                printf("line %d\n", count);
            char *position = strstr(line, argv[5]);
            int x = position - line;

            for (int i = 0; i < x; ++i) {
                printf("%c", line[i]);
            }
            printf("\033[1;32m");
            printf("%s", argv[5]);
            printf("\033[0m");

            printf("%s", line + x + strlen(argv[5]));
            printf("----------------------------------------------------------------\n");
        }
    }
    return 0;
}

// todo ********************************************************
int grep_commit(int argc, char *argv[], char cwd[]) {

    int last_id = atoi(argv[7]);

    char data_string[100][1000];
    int number_of_files = find_commits_with_exact_id(last_id, data_string);

    for (int j = 0; j < number_of_files; j++) {
        char path[PATH_MAX];
        sscanf(data_string[j], "%*d %*s %*d %*s %s %*s /%*[^/]s/ /%*[^/\n]s/\n", path);

        FILE *file = fopen(path, "r");
        char line[MAX_LINE];
        int count = 0;
        while (fgets(line, sizeof(line), file) != NULL) {
            count++;
            if (strstr(line, argv[5]) != NULL) {
                if (strcmp(argv[argc - 1], "-n") == 0)
                    printf("line %d\n", count);
                char *position = strstr(line, argv[5]);
                int x = position - line;

                for (int i = 0; i < x; ++i) {
                    printf("%c", line[i]);
                }
                printf("\033[1;32m");
                printf("%s", argv[5]);
                printf("\033[0m");

                printf("%s", line + x + strlen(argv[5]));
                printf("----------------------------------------------------------------\n");
            }
        }
    }
    return 0;
}


// todo ********************************************************
int grep_function(int argc, char *argv[], char cwd[]) {
    if (argc <= 7 && strcmp(argv[4], "-p") == 0) {
        return grep_file(argc, argv, cwd);
    } else if (argc <= 9 && strcmp(argv[6], "-c") == 0) {
        return grep_commit(argc, argv, cwd);
    }
    printf("invalid input\n");
    return 0;
}


const char *getFilename(const char *path) {
    const char *filename = strrchr(path, '/');
    if (filename == NULL) {
        filename = path; // If no '/' found, use the full path as the filename
    } else {
        filename++; // Move past the last '/'
    }
    return filename;
}

void print_diff_word(int line_num, char *word1, char *word2) {
    if (strcmp(word1, "\n") != 0 && strcmp(word2, "\n") != 0) {
        printf("words that are different: <<%s>> and <<%s>>\n", word1, word2);
    }
}

int isWhitespaceLine(const char *line) {
    while (*line != '\0') {
        if (!isspace((unsigned char) *line)) {
            return 0; // line contains non-whitespace characters
        }
        line++;
    }
    return 1; // line contains only white spaces
}


int diff_with_line(int argc, char *argv[]) {
    FILE *file1 = fopen(argv[3], "r");
    FILE *file2 = fopen(argv[4], "r");
    char line1[1000], line2[1000];
    int line_num = 0;

    const char *filename1 = getFilename(argv[3]);
    const char *filename2 = getFilename(argv[4]);

    if (file1 == NULL) {
        printf("File couldn't be opened: %s", argv[3]);
        return 0;
    }
    if (file2 == NULL) {
        printf("File couldn't be opened: %s", argv[4]);
        return 0;
    }
    if (strcmp(argv[5], "--line1") != 0 || strcmp(argv[7], "--line2") != 0) {
        printf("invalid notation for diff with line!\n");
        return 0;
    }

    int end1, end2, start1, start2;
    sscanf(argv[6], "%d-%d", &start1, &end1);
    sscanf(argv[8], "%d-%d", &start2, &end2);


    if (start1 < 0 || end1 < start1 || start2 < 0 || end2 < start2) {
        printf("invalid line range provided.\n");
        return 0;
    }

    for (int i = 0; i < start1; i++) {
        if (fgets(line1, sizeof(line1), file1) == NULL) {
            printf("Invalid starting line for %s: %d\n", filename1, start1);
            fclose(file1);
            fclose(file2);
            return 0;
        }
    }

    // Skip lines before the specified range
    for (int i = 0; i < start2; i++) {
        if (fgets(line2, sizeof(line2), file2) == NULL) {
            printf("Invalid starting line for %s: %d\n", filename2, start2);
            fclose(file1);
            fclose(file2);
            return 0;
        }
    }

    while ((fgets(line1, sizeof(line1), file1)) && (fgets(line2, sizeof(line2), file2)) && line_num <= end1) {
        if (line_num >= start1 && line_num <= end1 && line_num >= start2 && line_num <= end2 &&
            strcmp(line1, line2) != 0 && !isWhitespaceLine(line1) && !isWhitespaceLine(line2)) {

            char *line1_with = strtok(line1, "\n");
            char *line2_with = strtok(line2, "\n");

            printf("----------------------------------------------------------------\n");

            printf("%d th different line:\n", line_num);
            printf("\033[1;32m");
            printf("%s: %s\n", filename1, line1_with);
            printf("\033[1;34m");
            printf("%s: %s\n", filename2, line2_with);
            printf("\033[0m");

            int i = 0, j = 0;

            long int len1 = strlen(line1), len2 = strlen(line2);

            while (i < len1 && j < len2) {

                while (i < len1 && line1[i] == ' ') i++;
                while (j < len2 && line2[j] == ' ') j++;

                if (i == len1 || j == len2) break;

                int start1 = i, start2 = j;

                while (i < len1 && line1[i] != ' ') i++;
                while (j < len2 && line2[j] != ' ') j++;

                if (i - start1 != j - start2 || strncmp(&line1[start1], &line2[start2], i - start1) != 0) {
                    line1[i] = '\0';
                    line2[j] = '\0';
                    print_diff_word(line_num, &line1[start1], &line2[start2]);
                }
            }
        }

        line_num++;
    }

    fclose(file1);
    fclose(file2);
    return 0;
}


int diff_without_line(int argc, char *argv[]) {
    FILE *file1 = fopen(argv[3], "r");
    FILE *file2 = fopen(argv[4], "r");
    char line1[1000], line2[1000];
    int line_num = 0;

    const char *filename1 = getFilename(argv[3]);
    const char *filename2 = getFilename(argv[4]);

    if (file1 == NULL) {
        printf("File couldn't be opened: %s", argv[3]);
        return 0;
    }
    if (file2 == NULL) {
        printf("File couldn't be opened: %s", argv[4]);
        return 0;
    }

    while ((fgets(line1, sizeof(line1), file1)) && (fgets(line2, sizeof(line2), file2))) {

        if (strcmp(line1, line2) != 0 && !isWhitespaceLine(line1) && !isWhitespaceLine(line2)) {

            char *line1_with = strtok(line1, "\n");
            char *line2_with = strtok(line2, "\n");

            printf("----------------------------------------------------------------\n");


            printf("Line %d:\n", line_num + 1);
            printf("\033[1;32m");
            printf("%s: %s\n", filename1, line1_with);
            printf("\033[1;34m");
            printf("%s: %s\n", filename2, line2_with);
            printf("\033[0m");

            int i = 0, j = 0;

            long int len1 = strlen(line1), len2 = strlen(line2);

            while (i < len1 && j < len2) {

                while (i < len1 && line1[i] == ' ') i++;
                while (j < len2 && line2[j] == ' ') j++;

                if (i == len1 || j == len2) break;

                int start1 = i, start2 = j;

                while (i < len1 && line1[i] != ' ') i++;
                while (j < len2 && line2[j] != ' ') j++;

                if (i - start1 != j - start2 || strncmp(&line1[start1], &line2[start2], i - start1) != 0) {
                    line1[i] = '\0';
                    line2[j] = '\0';
                    print_diff_word(line_num, &line1[start1], &line2[start2]);
                }
            }
        }

        line_num++;
    }

    if (fgets(line1, sizeof(line1), file1)) {
        printf("----------------------------------------------------------------\n");
        printf("%s: has more lines:\n%s", filename1, line1);
        while (fgets(line1, sizeof(line1), file1)) {
            printf("%s", line1);
        }
    } else if (fgets(line2, sizeof(line2), file2)) {
        printf("----------------------------------------------------------------\n");
        printf("%s: has more lines:\n%s", filename2, line2);
        while (fgets(line2, sizeof(line2), file2)) {
            printf("%s", line2);
        }
    }

    fclose(file1);
    fclose(file2);
    return 0;
}


// daliiiiiiiii
int diff(int argc, char *argv[]) {
    if (strcmp(argv[2], "-f") == 0) {
        if (argc == 5) {
            return diff_without_line(argc, argv);
        } else if (argc == 9) {
            return diff_with_line(argc, argv);
        }
    } else if (strcmp(argv[2], "-c") == 0) {

    }
    printf("invalid command");
    return 0;


}


// todo ********************************************************
int merge(int argc, char *argv[], char cwd[]) {
    // find files in branch1
    int final_id_1 = find_number_of_last_id_in_branch(argv[3]);
    char files_1[100][1000];
    int number_of_files_1 = find_commits_with_exact_id(final_id_1, files_1);

    // find files in branch2
    int final_id_2 = find_number_of_last_id_in_branch(argv[4]);
    char files_2[100][1000];
    int number_of_files_2 = find_commits_with_exact_id(final_id_2, files_2);

    int min;
    if (number_of_files_2 > number_of_files_1)
        min = number_of_files_1;
    else
        min = number_of_files_2;

    int one = 0, two = 0;
    int count = 0;
    char data[100][PATH_MAX];
    while (min > 0) {

        char path1[PATH_MAX], path2[PATH_MAX];
        char name1[MAX_WORDS], name2[MAX_WORDS];

        sscanf(files_1[one],
               "%*d %*s %*d %s %s %*s /%*[^/]s/ /%*[^\n]s/\n", name1, path1);
        sscanf(files_2[two],
               "%*d %*s %*d %s %s %*s /%*[^/]s/ /%*[^\n]s/\n", name2, path2);

        if (strcmp(path1, path2) == 0) {
            char argv_c[100][100];
            strcpy(argv_c[3], path1);
            strcpy(argv_c[4], path2);
            diff_without_line(argc, argv_c);
            continue;
        }

        find_exact_data_in_exact_branch(name1, argv[3], data[count]);
        find_exact_data_in_exact_branch(name2, argv[4], data[count + 1]);

        min--, one++, two++;
        count++;
    }
    if (number_of_files_2 > number_of_files_1) {
        for (int i = 0; i < number_of_files_2 - number_of_files_1; ++i) {
            char path2[PATH_MAX], name2[MAX_WORDS];
            sscanf(files_2[two], "%*d %*s %*d %s %s %*s /%*[^/]s/ /%*[^\n]s/\n", name2, path2);

            find_exact_data_in_exact_branch(name2, argv[4], data[count]);

            count++;
        }
    }
    if (number_of_files_2 < number_of_files_1) {
        for (int i = 0; i < number_of_files_1 - number_of_files_2; ++i) {
            char path1[PATH_MAX], name1[MAX_WORDS];
            sscanf(files_2[two], "%*d %*s %*d %s %s %*s /%*[^/]s/ /%*[^\n]s/\n", name1, path1);

            find_exact_data_in_exact_branch(name1, argv[3], data[count]);

            count++;
        }
    }

    // open all_commit_data
    char all_data_path[200];
    snprintf(all_data_path, sizeof(all_data_path), "%s/%s", cwd_main_file, ".neogit/branches/all_commit_data");
    FILE *all_commit_data = fopen(all_data_path, "a+");


    char name[100][100], path[100][PATH_MAX], massage[100][100], author[100][50];
    for (int i = 0; i < count; ++i) {
        printf("data: %s\n", data[i]);
        sscanf(data[i], "%*d %*s %*d %s %s %s /%*[^/]s/ /%[^\n]s/\n", name[i],
               path[i], author[i], massage[i]);
    }

    fclose(all_commit_data);
    // new num of commit in branch
    int number_of_last_commit2 = find_number_of_last_commit_in_branch(cwd, argv[4]);

    // new id
    int new_id = find_id_and_plus(cwd);

    // new time
    time_t rawtime;
    struct tm *time_info;
    time(&rawtime);
    time_info = localtime(&rawtime);
    char time_string[500];
    strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", time_info);


    for (int i = 0; i < count; ++i) {
        fprintf(all_commit_data, "%d %s %d %s %s %s /%s/ /%s/\n", new_id, argv[4],
                number_of_last_commit2 + 1, name[i], path[i], author[i], time_string, massage[0]);
    }


    // copy file and rename
    char new_commit_path[PATH_MAX];
    snprintf(new_commit_path, sizeof(new_commit_path), "%s/%s/%s/%d", cwd, ".neogit/branches", argv[4],
             number_of_last_commit2 + 1);
    mkdir(new_commit_path, 0755);


    // files in branch 2
    char pre_commit_path2[PATH_MAX];
    snprintf(pre_commit_path2, sizeof(pre_commit_path2), "%s/%s/%s/%d", cwd, ".neogit/branches", argv[4],
             number_of_last_commit2);
    DIR *dir2 = opendir(pre_commit_path2);
    struct dirent *entry2;
    while ((entry2 = readdir(dir2)) != NULL) {
        if (entry2->d_type == DT_REG) {
            char source[PATH_MAX];
            snprintf(source, sizeof(source), "%s/%s/%s/%d/%s", cwd, ".neogit/branches", argv[4],
                     number_of_last_commit2,
                     entry2->d_name);

            char command[PATH_MAX];
            snprintf(command, sizeof(command), "cp %s %s", source, new_commit_path);
            system(command);
        }
    }
    closedir(dir2);


    int number_of_last_commit1 = find_number_of_last_commit_in_branch(cwd, argv[3]);

    // files in branch 1
    char pre_commit_path1[PATH_MAX];
    snprintf(pre_commit_path1, sizeof(pre_commit_path1), "%s/%s/%s/%d", cwd, ".neogit/branches", argv[3],
             number_of_last_commit1);
    DIR *dir1 = opendir(pre_commit_path1);
    struct dirent *entry1;
    while ((entry1 = readdir(dir2)) != NULL) {
        if (entry1->d_type == DT_REG) {
            char source[PATH_MAX];
            snprintf(source, sizeof(source), "%s/%s/%s/%d/%s", cwd, ".neogit/branches", argv[3],
                     number_of_last_commit1,
                     entry1->d_name);

            char command[PATH_MAX];
            snprintf(command, sizeof(command), "cp %s %s", source, new_commit_path);
            system(command);
        }
    }
    closedir(dir1);

    change_branch_name(cwd, argv[4]);
    change_number_of_commit_in_branch(cwd, argv[4]);

    return 0;

}


struct state {
    char name[100];
    char todo_check[100];
    char eof_blank_space[100];
    char format_check[100];
    char balance_braces[100];
    char static_error_check[100];
    char file_size_check[100];
    char character_limit[100];
};
struct state hook[100];
int number_of_hook_file;
int fail;

void hook_todo_u(char cwd[]) {
    // open file
    char path[PATH_MAX];
    snprintf(path, sizeof(path), "%s/%s", cwd, ".neogit/hook");
    FILE *file = fopen(path, "r");

    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    fseek(file, -1, SEEK_END);


    fseek(file, 0, SEEK_SET);

    while (fgetc(file) == ' ')
        fseek(file, -2, SEEK_CUR);

    fputc('\0', file);

    fclose(file);

}

void hook_todo(char file_name[], char dir_path[]) {

    // open file
    char path[PATH_MAX];
    snprintf(path, sizeof(path), "%s/%s", dir_path, file_name);
    FILE *file = fopen(path, "r");

    if (strstr(file_name, ".cpp") != NULL || strstr(file_name, ".c") != NULL) {

        char line[MAX_LINE];
        int match = 0;
        while (fgets(line, sizeof(line), file) != NULL) {
            if (strstr(line, "// TODO") != NULL || strstr(line, "//TODO") != NULL) {
                match = 1;
                fail = 1;
                strcpy(hook[number_of_hook_file].todo_check, "FAILED");

                printf("\033[1;31m");
                printf("hook id: todo-check -> %s\n", hook[number_of_hook_file].todo_check);
                printf("\033[0m");

                break;
            }
        }
        if (!match) {
            strcpy(hook[number_of_hook_file].todo_check, "PASSED");

            printf("\033[1;32m");
            printf("hook id: todo-check -> %s\n", hook[number_of_hook_file].todo_check);
            printf("\033[0m");
        }

    } else if (strstr(file_name, ".txt") != NULL) {

        char line[MAX_LINE];
        int match = 0;
        while (fgets(line, sizeof(line), file) != NULL) {
            if (strstr(line, "TODO") != NULL) {
                match = 1;
                strcpy(hook[number_of_hook_file].todo_check, "FAILED");

                printf("\033[1;31m");
                printf("hook id: todo-check -> %s\n", hook[number_of_hook_file].todo_check);
                printf("\033[0m");

                break;
            }
        }
        if (!match) {
            strcpy(hook[number_of_hook_file].todo_check, "PASSED");

            printf("\033[1;32m");
            printf("hook id: todo-check -> %s\n", hook[number_of_hook_file].todo_check);
            printf("\033[0m");
        }

    } else {
        strcpy(hook[number_of_hook_file].todo_check, "SKIPPED");
    }
}

void hook_eof_blank_space(char file_name[], char dir_path[]) {

    // open file
    char path[PATH_MAX];
    snprintf(path, sizeof(path), "%s/%s", dir_path, file_name);
    FILE *file = fopen(path, "r");

    if (strstr(file_name, ".cpp") != NULL || strstr(file_name, ".c") != NULL || strstr(file_name, ".txt") != NULL) {

        fseek(file, -1, SEEK_END);

        int last_char = fgetc(file);

        if (last_char == ' ' || last_char == '\t' || last_char == '\n' || last_char == '\r') {
            strcpy(hook[number_of_hook_file].eof_blank_space, "FAILED");

            printf("\033[1;31m");
            printf("hook id: eof-blank-space -> %s\n", hook[number_of_hook_file].eof_blank_space);
            printf("\033[0m");

            fail = 1;

        } else {
            strcpy(hook[number_of_hook_file].eof_blank_space, "PASSED");

            printf("\033[1;32m");
            printf("hook id: eof-blank-space -> %s\n", hook[number_of_hook_file].eof_blank_space);
            printf("\033[0m");
        }

    } else {
        strcpy(hook[number_of_hook_file].eof_blank_space, "SKIPPED");
    }

}

void hook_format_check(char file_name[], char dir_path[]) {
    if (strstr(file_name, ".cpp") != NULL || strstr(file_name, ".c") != NULL || strstr(file_name, ".txt") != NULL ||
        strstr(file_name, ".mp3") != NULL || strstr(file_name, ".mp4") != NULL ||
        strstr(file_name, ".wav") != NULL) {
        strcpy(hook[number_of_hook_file].format_check, "PASSED");

        printf("\033[1;32m");
        printf("hook id: format-check -> %s\n", hook[number_of_hook_file].format_check);
        printf("\033[0m");

    } else {
        fail = 1;
        strcpy(hook[number_of_hook_file].format_check, "FAILED");

        printf("\033[1;31m");
        printf("hook id: format-check -> %s\n", hook[number_of_hook_file].format_check);
        printf("\033[0m");

    }
}

void hook_balance_braces(char file_name[], char dir_path[]) {
    if (strstr(file_name, ".cpp") != NULL || strstr(file_name, ".c") != NULL || strstr(file_name, ".txt") != NULL) {

        // open file
        char path[PATH_MAX];
        snprintf(path, sizeof(path), "%s/%s", dir_path, file_name);
        FILE *file = fopen(path, "r");

        char ch;
        int open_parentheses = 0, closed_parentheses = 0;
        int open_brackets = 0, closed_brackets = 0;
        int open_crochet = 0, closed_crochet = 0;

        if (file == NULL) {
            printf("file not found or unable to open.\n");
            return;
        }

        while ((ch = fgetc(file)) != EOF) {
            if (ch == '(') {
                open_parentheses++;
            } else if (ch == ')') {
                closed_parentheses++;
            } else if (ch == '[') {
                open_brackets++;
            } else if (ch == ']') {
                closed_brackets++;
            } else if (ch == '{') {
                open_crochet++;
            } else if (ch == '}') {
                closed_crochet++;
            }
        }

        fclose(file);

        if (open_parentheses == closed_parentheses && open_crochet == closed_crochet &&
            open_brackets == closed_brackets) {
            strcpy(hook[number_of_hook_file].balance_braces, "PASSED");

            printf("\033[1;32m");
            printf("hook id: balance-braces -> %s\n", hook[number_of_hook_file].balance_braces);
            printf("\033[0m");

        } else {
            fail = 1;
            strcpy(hook[number_of_hook_file].balance_braces, "FAILED");

            printf("\033[1;31m");
            printf("hook id: balance-braces -> %s\n", hook[number_of_hook_file].balance_braces);
            printf("\033[0m");
        }
    } else
        strcpy(hook[number_of_hook_file].balance_braces, "SKIPPED");
}

void hook_static_error_check(char file_name[], char dir_path[]) {
    if (strstr(file_name, ".cpp") != NULL || strstr(file_name, ".c") != NULL) {

        char command[PATH_MAX];
        snprintf(command, sizeof(command), "gcc %s -o kar_kon", file_name);

        if (system(command) == 0) {
            strcpy(hook[number_of_hook_file].static_error_check, "PASSED");

            printf("\033[1;32m");
            printf("hook id: static-error-check -> %s\n", hook[number_of_hook_file].static_error_check);
            printf("\033[0m");

        } else {
            fail = 1;
            strcpy(hook[number_of_hook_file].static_error_check, "FAILED");

            printf("\033[1;31m");
            printf("hook id: static-error-check -> %s\n", hook[number_of_hook_file].static_error_check);
            printf("\033[0m");
        }

    } else {
        strcpy(hook[number_of_hook_file].static_error_check, "SKIPPED");
    }
}

void hook_file_size_check(char file_name[], char dir_path[]) {
    // open file
    char path[PATH_MAX];
    snprintf(path, sizeof(path), "%s/%s", dir_path, file_name);
    FILE *file = fopen(path, "rb");

    fseek(file, 0, SEEK_END);
    long size = ftell(file);

    double size_B = (double) size / (1024 * 1024);

    if (size_B > 5.0) {
        fail = 1;
        strcpy(hook[number_of_hook_file].file_size_check, "FAILED");

        printf("\033[1;31m");
        printf("hook id: file-size-check -> %s\n", hook[number_of_hook_file].file_size_check);
        printf("\033[0m");

    } else {
        strcpy(hook[number_of_hook_file].file_size_check, "PASSED");

        printf("\033[1;32m");
        printf("hook id: file-size-check -> %s\n", hook[number_of_hook_file].file_size_check);
        printf("\033[0m");
    }
}

void hook_character_limit(char file_name[], char dir_path[]) {
    if (strstr(file_name, ".cpp") != NULL || strstr(file_name, ".c") != NULL || strstr(file_name, ".txt") != NULL) {
        // open file
        char path[PATH_MAX];
        snprintf(path, sizeof(path), "%s/%s", dir_path, file_name);
        FILE *file = fopen(path, "r");

        char ch;
        int count = 0;
        while ((ch = fgetc(file)) != EOF)
            count++;

        if (count > 200000) {
            strcpy(hook[number_of_hook_file].character_limit, "FAILED");

            printf("\033[1;31m");
            printf("hook id: character-limit -> %s\n", hook[number_of_hook_file].character_limit);
            printf("\033[0m");


            fail = 1;
        } else {
            strcpy(hook[number_of_hook_file].character_limit, "PASSED");

            printf("\033[1;32m");
            printf("hook id: character-limit -> %s\n", hook[number_of_hook_file].character_limit);
            printf("\033[0m");

        }
    } else
        strcpy(hook[number_of_hook_file].character_limit, "SKIPPED");

}

int pre_commit(int argc, char *argv[], char cwd[]) {
    // open file
    char path[PATH_MAX];
    snprintf(path, sizeof(path), "%s/%s", cwd, ".neogit/hook");
    FILE *file = fopen(path, "r");

    char line[MAX_LINE], all[MAX_LINE];
    while (fgets(line, sizeof(line), file) != NULL) {
        strcat(all, line);
    }

    char dir_path[200];
    snprintf(dir_path, sizeof(dir_path), "%s/%s", cwd_main_file, ".neogit/stage");
    DIR *dir = opendir(dir_path);
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {

        if (entry->d_type == DT_REG) {
            strcpy(hook[number_of_hook_file].name, entry->d_name);
            printf("File Name: %s\n", hook[number_of_hook_file].name);

            if (strstr(all, "todo-check") != NULL) {
                hook_todo(entry->d_name, dir_path);
            }

            if (strstr(all, "eof-blank-space") != NULL) {
                hook_eof_blank_space(entry->d_name, dir_path);
            }

            if (strstr(all, "format-check") != NULL) {
                hook_format_check(entry->d_name, dir_path);
            }

            if (strstr(all, "balance-braces") != NULL) {
                hook_balance_braces(entry->d_name, dir_path);
            }

            if (strstr(all, "static-error-check") != NULL) {
                hook_static_error_check(entry->d_name, dir_path);
            }

            if (strstr(all, "file-size-check") != NULL) {
                hook_file_size_check(entry->d_name, dir_path);
            }

            if (strstr(all, "character-limit") != NULL) {
                hook_character_limit(entry->d_name, dir_path);
            }

            number_of_hook_file++;
        }
    }
    if (fail == 1) {
        printf("Are you sure you want to commit?\n");
        // TODO ask and answer
    }

}

void hook_all_list(char cwd[]) {
    // open file
    char path[PATH_MAX];
    snprintf(path, sizeof(path), "%s/%s", cwd, ".neogit/hook_all");
    FILE *file = fopen(path, "r");

    char line[MAX_LINE], name[MAX_LINE];
    int match = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        sscanf(line, "%[^\n]s\n", name);
        printf("%s\n", name);
    }
}

void hook_list(char cwd[]) {
    // open file
    char path[PATH_MAX];
    snprintf(path, sizeof(path), "%s/%s", cwd, ".neogit/hook");
    FILE *file = fopen(path, "r");

    char line[MAX_LINE], name[MAX_LINE];
    int match = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        sscanf(line, "%[^\n]s\n", name);
        printf("%s\n", name);
    }
}

void hook_add(char cwd[], char hook_id[]) {
    // open file
    char path[PATH_MAX];
    snprintf(path, sizeof(path), "%s/%s", cwd, ".neogit/hook");
    FILE *file = fopen(path, "a");

    fprintf(file, "%s\n", hook_id);
}

void hook_remove(char cwd[], char hook_id[]) {
    // open file
    char path[PATH_MAX];
    snprintf(path, sizeof(path), "%s/%s", cwd, ".neogit/hook");
    FILE *file = fopen(path, "r");

    char path_c[PATH_MAX];
    snprintf(path_c, sizeof(path), "%s/%s", cwd, ".neogit/hook_c");
    FILE *file_c = fopen(path, "w");


    char line[MAX_LINE], name[MAX_LINE];
    int match = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line, hook_id) != NULL) {
            continue;
        }
        fprintf(file_c, "%s", line);
    }

    fclose(file_c);
    fclose(file);

    remove(path);
    rename(path_c, path);
}

int pre_commit_menu(int argc, char *argv[], char cwd[]) {
    if (argc == 2) {
        pre_commit(argc, argv, cwd);
    } else if (argc == 4) {
        if (strcmp(argv[argc - 1], "list") == 0) {
            hook_all_list(cwd);
        } else if (strcmp(argv[argc - 1], "hooks") == 0) {
            hook_list(cwd);
        }
    } else if (strcmp(argv[3], "hook") == 0) {
        if (strcmp(argv[2], "add") == 0) {
            hook_add(cwd, argv[4]);
        } else if (strcmp(argv[2], "remove") == 0) {
            hook_remove(cwd, argv[4]);
        }
    } else {
        printf("invalid command\n");
    }
    return 0;
}


// todo main ********************************************************
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stdout, "please enter a valid command");
        return 1;
    }
    char cwd[500];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd in creat config has problem");
        return 0;
    }
//    strcpy(cwd_main_file, cwd);
//    find_number_of_commit_in_branch(cwd);
//    if (check_local_alias(argc, argv) == 1)
//        check_global_alias(argc, argv);
    while (1) {
        if (strcmp(argv[1], "init") == 0) {
            return init_function(argc, argv);
        } else if (strcmp(argv[1], "config") == 0) {
            return config_function(argc, argv);
        } else if (strcmp(argv[1], "add") == 0) {
            return add_function(argc, argv);
        } else if (strcmp(argv[1], "rest") == 0) {
            return reset_function(argc, argv);
        } else if (strcmp(argv[1], "commit") == 0) {
            return commit_function(argc, argv);
        } else if (strcmp(argv[1], "set") == 0) {
            return set_shortcut_function(argc, argv);
        } else if (strcmp(argv[1], "replace") == 0) {
            return replace_shortcut_function(argc, argv);
        } else if (strcmp(argv[1], "remove") == 0) {
            return remove_shortcut_function(argc, argv);
        } else if (strcmp(argv[1], "log") == 0) {
            return log_function(argc, argv);
        } else if (strcmp(argv[1], "checkout") == 0) {
            return checkout_function(argc, argv, cwd);
        } else if (strcmp(argv[1], "branch") == 0) {
            return branch_function(argc, argv, cwd);
        } else if (strcmp(argv[1], "revert") == 0) {
            return revert_function(argc, argv, cwd);
        } else if (strcmp(argv[1], "tag") == 0) {
            return tag_function(argc, argv, cwd);
        } else if (strcmp(argv[1], "merge") == 0) {
            return merge(argc, argv, cwd);
        } else if (strcmp(argv[1], "grep") == 0) {
            return grep_function(argc, argv, cwd);
        } else if (strcmp(argv[1], "pre-commit") == 0) {
            return pre_commit_menu(argc, argv, cwd);
        } else if (strcmp(argv[1], "diff") == 0) {
            return diff(argc, argv);
        } else if (strcmp(argv[1], "status") == 0) {
            status_function(cwd);
            return print_status_result();
        } else if (check_local_alias(&argc, argv) == 1 || check_global_alias(&argc, argv) == 1) {
            continue;
        } else {
            // todo ooooooooooooooooooooooooooooooooooo
            printf("invalid command!\n");
        }

        break;
    }


    return 0;
}