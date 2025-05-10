#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NDSP_VERSION "1.0.1"

// Function prototypes
void print_help(void);
void deploy_project(const char *project, const char *dest);
void install_package_pkg(const char *repo_link, const char *pkgver);
void open_terminal(void);
void show_status(const char *project);
void list_deployments(void);
void rollback_project(const char *project);
char *get_pkg_name(const char *repo_link);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_help();
        return 1;
    }
    
    if (strcmp(argv[1], "deploy") == 0) {
        if (argc < 4) {
            printf("Usage: ndsp deploy <Project> <To>\n");
            return 1;
        }
        deploy_project(argv[2], argv[3]);
    }
    else if (strcmp(argv[1], "install") == 0) {
        // Command: ndsp install pkg <github_repo_link> <pkgver>
        if (argc < 5) {
            printf("Usage: ndsp install pkg <github_repo_link> <pkgver>\n");
            return 1;
        }
        if (strcmp(argv[2], "pkg") == 0) {
            install_package_pkg(argv[3], argv[4]);
        } else {
            printf("Unknown install command: %s\n", argv[2]);
            return 1;
        }
    }
    else if (strcmp(argv[1], "term") == 0) {
        open_terminal();
    }
    else if (strcmp(argv[1], "status") == 0) {
        if (argc < 3) {
            printf("Usage: ndsp status <Project>\n");
            return 1;
        }
        show_status(argv[2]);
    }
    else if (strcmp(argv[1], "list") == 0) {
        list_deployments();
    }
    else if (strcmp(argv[1], "rollback") == 0) {
        if (argc < 3) {
            printf("Usage: ndsp rollback <Project>\n");
            return 1;
        }
        rollback_project(argv[2]);
    }
    else if ( (strcmp(argv[1], "version") == 0) || (strcmp(argv[1], "--ver") == 0) ) {
        printf("NDS+ Version: %s\n", NDSP_VERSION);
    }
    else if (strcmp(argv[1], "help") == 0) {
        print_help();
    }
    else {
        printf("Unknown command: %s\n", argv[1]);
        print_help();
        return 1;
    }
    
    return 0;
}

//---------------------------------------------------------------------
// Prints help information for NDS+.
void print_help(void) {
    printf("NDS+ - Deployment & Package Management Tool (Version %s)\n", NDSP_VERSION);
    printf("Usage: ndsp <command> [options]\n");
    printf("Commands:\n");
    printf("  deploy <Project> <To>         : Deploy the specified project to destination\n");
    printf("  install pkg <repo_link> <ver>   : Install a package from a GitHub repo with the given version\n");
    printf("  term                          : Open NDS+ Terminal (ndspterm.exe) using bash\n");
    printf("  status <Project>              : Show status of the specified project\n");
    printf("  list                          : List deployed projects and packages\n");
    printf("  rollback <Project>            : Rollback deployment for the specified project\n");
    printf("  version | --ver               : Show NDS+ version\n");
    printf("  help                          : Show this help message\n");
}

//---------------------------------------------------------------------
// Simulates deployment by recursively copying the project directory to destination.
void deploy_project(const char *project, const char *dest) {
    printf("Deploying project '%s' to destination '%s'...\n", project, dest);
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "cp -r %s %s", project, dest);
    if (system(cmd) == 0) {
        printf("Project '%s' deployed successfully to '%s'.\n", project, dest);
    } else {
        printf("Failed to deploy project '%s'.\n", project);
    }
}

//---------------------------------------------------------------------
// Opens NDS+ Terminal by launching ndspterm.exe.
void open_terminal(void) {
    printf("Opening NDS+ Terminal (ndspterm.exe)...\n");
    system("ndspterm.exe");
}

//---------------------------------------------------------------------
// Simulates showing the status of a deployed project.
void show_status(const char *project) {
    printf("Status of project '%s': Running normally.\n", project);
}

//---------------------------------------------------------------------
// Simulates listing of deployed projects and packages.
void list_deployments(void) {
    printf("Listing deployed projects and packages:\n");
    printf("  - ProjectAlpha\n");
    printf("  - ProjectBeta\n");
}

//---------------------------------------------------------------------
// Simulates a rollback of a project's deployment.
void rollback_project(const char *project) {
    printf("Rolling back deployment for project '%s'...\n", project);
    printf("Rollback of project '%s' completed successfully.\n", project);
}

//---------------------------------------------------------------------
// Helper: Extracts a package name from the provided repository link.
// For example, if repo_link is "https://github.com/user/mypackage", returns "mypackage".
// The caller is responsible for freeing the returned string.
char *get_pkg_name(const char *repo_link) {
    const char *last_slash = strrchr(repo_link, '/');
    if (last_slash && *(last_slash + 1) != '\0') {
        return strdup(last_slash + 1);
    }
    return strdup(repo_link);
}

//---------------------------------------------------------------------
// Installs a package from a provided GitHub repository link and version.
// It downloads a ZIP file assumed to be named <pkgname>.<pkgver>.zip from the URL:
//   <repo_link>/releases/download/v<pkgver>/<pkgname>.<pkgver>.zip
// and extracts it into the directory pkg/<pkgname>.
void install_package_pkg(const char *repo_link, const char *pkgver) {
    char *pkg_name = get_pkg_name(repo_link);
    if (!pkg_name) {
        printf("Error extracting package name from repo link.\n");
        return;
    }
    printf("Installing package '%s' version %s from repo: %s\n", pkg_name, pkgver, repo_link);
    
    char download_url[512];
    snprintf(download_url, sizeof(download_url), "%s/releases/download/v%s/%s.%s.zip",
             repo_link, pkgver, pkg_name, pkgver);
    
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "wget -q %s -O %s.zip", download_url, pkg_name);
    if (system(cmd) != 0) {
        printf("Failed to download package from: %s\n", download_url);
        free(pkg_name);
        return;
    }
    
    snprintf(cmd, sizeof(cmd), "mkdir -p pkg/%s", pkg_name);
    system(cmd);
    
    snprintf(cmd, sizeof(cmd), "unzip -q %s.zip -d pkg/%s", pkg_name, pkg_name);
    if (system(cmd) != 0) {
        printf("Failed to extract package %s.zip\n", pkg_name);
        free(pkg_name);
        return;
    }
    
    printf("Package '%s' version %s installed successfully in pkg/%s.\n", pkg_name, pkgver, pkg_name);
    free(pkg_name);
}
