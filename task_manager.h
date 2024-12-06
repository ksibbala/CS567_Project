#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

constexpr int MAX_TASKS = 100;
constexpr int MAX_TITLE_LENGTH = 100;
constexpr int MAX_DESC_LENGTH = 500;

// Enum for task priority
enum class Priority {
    LOW,
    MEDIUM,
    HIGH
};

// Structure to represent a task
struct Task
{
    int task_id;
    std::string title;
    std::string description;
    Priority priority;
    bool is_completed;
};

// Class to represent the Task Management System
class TaskManager
{
public:
    TaskManager();

    // Task management functions
    void addTask(const std::string &title, const std::string &description, Priority priority);
    Task* findTask(int task_id);
    Task* searchTaskById(int task_id);
    void deleteTask(int task_id);
    void updateTask(int task_id, const std::string &new_title, const std::string &new_description, Priority new_priority);
    void markTaskCompleted(int task_id);
    void displayTaskDetails(int task_id);
    void displayAllTasks();
    void displayCompletedTasks();
    void displayIncompleteTasks();
    void displayTasksByPriority();
    void displayTaskCount();
    int getTaskCount();
    void notifyHighPriorityTasks();
    void countTasksByPriority();

    // Additional management utilities
    void countTasksByStatus();
    void clearCompletedTasks();
    void resetTasks();
    void updateTaskStatus(int task_id, bool new_status);
    void sortTasksByTitle();

    // Search functions
    void searchTaskByTitle(const std::string &title);
    void searchTaskByDescription(const std::string &description);

    // Sorting function
    void sortTasksByPriority();

    // helper functions
    std::string formatPriority(Priority priority);
    std::string formatStatus(bool is_completed);

private:
    std::vector<Task> tasks;
    int task_counter; 
};

#endif