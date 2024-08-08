#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/statvfs.h>
#endif

void espace_disque_restant(const char *path)
{
#ifdef _WIN32
    ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes, totalNumberOfFreeBytes;

    if (GetDiskFreeSpaceEx(path, &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes))
    {
        printf("L'espace disque restant sur le chemin %s est : %.2f Go\n",
               path, (double)totalNumberOfFreeBytes.QuadPart / (1024 * 1024 * 1024));
    }
    else
    {
        printf("Erreur lors de la récupération de l'espace disque restant pour le chemin %s\n", path);
    }
#else
    struct statvfs stat;

    if (statvfs(path, &stat) == 0)
    {
        unsigned long long free = stat.f_bsize * stat.f_bfree;
        printf("L'espace disque restant sur le chemin %s est : %.2f Go\n",
               path, (double)free / (1024 * 1024 * 1024));
    }
    else
    {
        perror("Erreur lors de la récupération de l'espace disque restant");
    }
#endif
}

void afficher_ram()
{
#ifdef _WIN32
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);

    if (GlobalMemoryStatusEx(&statex))
    {
        printf("RAM totale : %llu Go\n", statex.ullTotalPhys / (1024 * 1024 * 1024));
        printf("RAM disponible : %llu Go\n", statex.ullAvailPhys / (1024 * 1024 * 1024));
    }
    else
    {
        printf("Erreur lors de la récupération des informations sur la RAM\n");
    }
#else
    struct sysinfo info;

    if (sysinfo(&info) == 0)
    {
        printf("RAM totale : %lu Go\n", info.totalram * info.mem_unit / (1024 * 1024 * 1024));
        printf("RAM disponible : %lu Go\n", info.freeram * info.mem_unit / (1024 * 1024 * 1024));
    }
    else
    {
        perror("Erreur lors de la récupération des informations sur la RAM");
    }
#endif
}
void afficher_nombre_coeurs()
{
#ifdef _WIN32
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    printf("Nombre de cœurs du processeur : %u\n", sysInfo.dwNumberOfProcessors);
#else
#ifdef __linux__
    // Sous Linux
    int num_cpus = sysconf(_SC_NPROCESSORS_ONLN);
    printf("Nombre de cœurs du processeur : %d\n", num_cpus);
#else
    // Sous macOS
    int num_cpus;
    size_t len = sizeof(num_cpus);
    if (sysctlbyname("hw.ncpu", &num_cpus, &len, NULL, 0) == 0)
    {
        printf("Nombre de cœurs du processeur : %d\n", num_cpus);
    }
    else
    {
        perror("Erreur lors de la récupération du nombre de cœurs du processeur");
    }
#endif
#endif
}

int main()
{
    // Afficher l'espace disque restant pour le lecteur C (Windows) ou racine (Unix-like)
#ifdef _WIN32
    espace_disque_restant("C:\\");
#else
    espace_disque_restant("/");
#endif

    // Afficher la quantité de RAM totale et disponible
    afficher_ram();

    // Afficher le nombre de cœurs du processeur
    afficher_nombre_coeurs();

    return 0;
}