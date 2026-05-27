#pragma once
#ifndef TASKDB_H
#define TASKDB_H

#include <windows.h>
#include "TaskItem.h"

typedef struct TaskDB_s {
    long int nCount;
    PTaskItem* tasks;
} TaskDB, * PTaskDb;

BOOL AddTaskToDb(PTaskDb db, PTaskItem newTask);
void FreeDb(PTaskDb db);

BOOL LoadTasksFromFile(wchar_t* lpFile, PTaskDb outDb);
BOOL SaveTasksToFile(wchar_t* lpFile, PTaskDb inDb);

#endif // !TASKDB_H