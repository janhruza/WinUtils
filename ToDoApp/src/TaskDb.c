#include "..\inc\TaskDb.h"
#include <stdlib.h>

BOOL AddTaskToDb(PTaskDb db, PTaskItem newTask) {
    PTaskItem* newTasks = (PTaskItem*)realloc(db->tasks, sizeof(PTaskItem) * (db->nCount + 1));
    if (!newTasks) return FALSE;
    db->tasks = newTasks;

    // Jen přiřadíme pointer, nekopírujeme data (pokud už je newTask alokován)
    db->tasks[db->nCount] = newTask;
    db->nCount++;
    return TRUE;
}

void FreeDb(PTaskDb db) {
    if (db->tasks) {
        for (long i = 0; i < db->nCount; i++) {
            free(db->tasks[i]);
        }
        free(db->tasks);
        db->tasks = NULL;
    }
    db->nCount = 0;
}

BOOL SaveTasksToFile(wchar_t* lpFile, PTaskDb inDb) {
    HANDLE hFile = CreateFileW(lpFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) return FALSE;

    DWORD dwWritten;
    WriteFile(hFile, &inDb->nCount, sizeof(long), &dwWritten, NULL);

    for (int i = 0; i < inDb->nCount; i++) {
        WriteFile(hFile, inDb->tasks[i], sizeof(TaskItem), &dwWritten, NULL);
    }

    CloseHandle(hFile);
    return TRUE;
}

BOOL LoadTasksFromFile(wchar_t* lpFile, PTaskDb outDb) {
    HANDLE hFile = CreateFileW(lpFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) return FALSE;

    FreeDb(outDb); // Vyčistíme stávající, pokud existuje

    DWORD dwRead;
    ReadFile(hFile, &outDb->nCount, sizeof(long), &dwRead, NULL);

    outDb->tasks = (PTaskItem*)malloc(sizeof(PTaskItem) * outDb->nCount);
    for (int i = 0; i < outDb->nCount; i++) {
        outDb->tasks[i] = (PTaskItem)malloc(sizeof(TaskItem));
        ReadFile(hFile, outDb->tasks[i], sizeof(TaskItem), &dwRead, NULL);
    }

    CloseHandle(hFile);
    return TRUE;
}