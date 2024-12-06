#include "task_manager.h"
using namespace std;

TaskManager::TaskManager() : task_counter(0) {}

void TaskManager::addTask(const string &title, const string &description, Priority priority)
{
    if (tasks.size() >= MAX_TASKS)
    {
        cout << "Error: Maximum number of tasks reached." << endl;
        return;
    }

    Task new_task;
    new_task.task_id = ++task_counter;
    new_task.title = title;
    new_task.description = description;
    new_task.priority = priority;
    new_task.is_completed = false;

    tasks.push_back(new_task);
    cout << "Task added successfully." << endl;
}

Task* TaskManager::findTask(int task_id)
{
    for (auto &task : tasks)
    {
        if (task.task_id == task_id)
        {
            return &task;
        }
    }
    return nullptr;
}

void TaskManager::deleteTask(int task_id)
{
    auto it = find_if(tasks.begin(), tasks.end(), [task_id](const Task &task) {
        return task.task_id == task_id;
    });

    if (it != tasks.end())
    {
        tasks.erase(it);
        cout << "Task " << task_id << " deleted successfully." << endl;
    }
    else
    {
        cout << "Error: Task not found." << endl;
    }
}

void TaskManager::updateTask(int task_id, const string &new_title, const string &new_description, Priority new_priority)
{
    Task *task = findTask(task_id);
    if (task == nullptr)
    {
        cout << "Error: Task not found." << endl;
        return;
    }

    task->title = new_title;
    task->description = new_description;
    task->priority = new_priority;
    cout << "Task updated successfully." << endl;
}

void TaskManager::markTaskCompleted(int task_id)
{
    Task *task = findTask(task_id);
    if (task == nullptr)
    {
        cout << "Error: Task not found." << endl;
        return;
    }

    task->is_completed = true;
    cout << "Task " << task_id << " marked as completed." << endl;
}

void TaskManager::displayTaskDetails(int task_id)
{
    Task *task = findTask(task_id);
    if (task == nullptr)
    {
        cout << "Error: Task not found." << endl;
        return;
    }

    cout << "Task ID: " << task->task_id << endl;
    cout << "Title: " << task->title << endl;
    cout << "Description: " << task->description << endl;
    cout << "Priority: " << formatPriority(task->priority) << endl;
    cout << "Status: " << formatStatus(task->is_completed) << endl;
}

string TaskManager::formatPriority(Priority priority)
{
    switch (priority)
    {
    case Priority::LOW:
        return "Low";
    case Priority::MEDIUM:
        return "Medium";
    case Priority::HIGH:
        return "High";
    default:
        return "Unknown";
    }
}

string TaskManager::formatStatus(bool is_completed)
{
    return is_completed ? "Completed" : "Incomplete";
}

void TaskManager::displayAllTasks()
{
    cout << "List of all tasks:" << endl;
    for (const auto &task : tasks)
    {
        cout << "Task ID: " << task.task_id << ", Title: " << task.title << ", Status: " << formatStatus(task.is_completed) << endl;
    }
}

void TaskManager::displayCompletedTasks()
{
    cout << "Completed tasks:" << endl;
    for (const auto &task : tasks)
    {
        if (task.is_completed)
        {
            cout << "Task ID: " << task.task_id << ", Title: " << task.title << endl;
        }
    }
}

void TaskManager::displayIncompleteTasks()
{
    cout << "Incomplete tasks:" << endl;
    bool found = false;
    for (const auto &task : tasks)
    {
        if (!task.is_completed)
        {
            cout << "Task ID: " << task.task_id << ", Title: " << task.title << endl;
            found = true;
        }
    }
    if (!found)
    {
        cout << "No incomplete tasks." << endl;
    }
}

void TaskManager::countTasksByStatus()
{
    int completed = 0, incomplete = 0;
    for (const auto &task : tasks)
    {
        if (task.is_completed)
            completed++;
        else
            incomplete++;
    }
    cout << "Completed tasks: " << completed << endl;
    cout << "Incomplete tasks: " << incomplete << endl;
}

void TaskManager::clearCompletedTasks()
{
    auto it = remove_if(tasks.begin(), tasks.end(), [](const Task &task) {
        return task.is_completed;
    });
    tasks.erase(it, tasks.end());
    cout << "Completed tasks have been cleared." << endl;
}

void TaskManager::sortTasksByPriority()
{
    sort(tasks.begin(), tasks.end(), [](const Task &a, const Task &b) {
        return static_cast<int>(a.priority) < static_cast<int>(b.priority);
    });
    cout << "Tasks sorted by priority." << endl;
}

void TaskManager::resetTasks()
{
    tasks.clear();
    task_counter = 0;
    cout << "All tasks have been reset." << endl;
}

void TaskManager::updateTaskStatus(int task_id, bool new_status)
{
    Task *task = findTask(task_id);
    if (task == nullptr)
    {
        cout << "Error: Task not found." << endl;
        return;
    }

    task->is_completed = new_status;
    cout << "Task " << task_id << " marked as " << (new_status ? "completed" : "incomplete") << "." << endl;
}

void TaskManager::displayTasksByPriority()
{
    cout << "Tasks grouped by priority:" << endl;
    for (int i = 0; i <= static_cast<int>(Priority::HIGH); ++i)
    {
        Priority p = static_cast<Priority>(i);
        cout << formatPriority(p) << ":" << endl;
        bool found = false;
        for (const auto &task : tasks)
        {
            if (task.priority == p)
            {
                cout << "  Task ID: " << task.task_id << ", Title: " << task.title << endl;
                found = true;
            }
        }
        if (!found)
        {
            cout << "  No tasks with this priority." << endl;
        }
    }
}

void TaskManager::searchTaskByTitle(const string &title)
{
    cout << "Searching tasks with title containing '" << title << "':" << endl;
    for (const auto &task : tasks)
    {
        if (task.title.find(title) != string::npos)
        {
            cout << "Task ID: " << task.task_id << ", Title: " << task.title << endl;
        }
    }
}

void TaskManager::searchTaskByDescription(const string &description)
{
    cout << "Searching tasks with description containing '" << description << "':" << endl;
    for (const auto &task : tasks)
    {
        if (task.description.find(description) != string::npos)
        {
            cout << "Task ID: " << task.task_id << ", Title: " << task.title << endl;
        }
    }
}

void TaskManager::displayTaskCount()
{
    cout << "Total number of tasks: " << tasks.size() << endl;
}

int TaskManager::getTaskCount() {
    return tasks.size();
}

Task* TaskManager::searchTaskById(int task_id) {
    for (auto& task : tasks) {
        if (task.task_id == task_id) {
            return &task;
        }
    }
    return nullptr;  // Task not found
}

void TaskManager::notifyHighPriorityTasks() {
    for (const auto& task : tasks) {
        if (task.priority == Priority::HIGH) {
            cout << "High-priority task: " << task.title << endl;
        }
    }
}

void TaskManager::countTasksByPriority() {
    int low_count = 0, medium_count = 0, high_count = 0;
    for (const auto& task : tasks) {
        switch (task.priority) {
            case Priority::LOW: low_count++; break;
            case Priority::MEDIUM: medium_count++; break;
            case Priority::HIGH: high_count++; break;
        }
    }
    std::cout << "Low priority tasks: " << low_count << std::endl;
    std::cout << "Medium priority tasks: " << medium_count << std::endl;
    std::cout << "High priority tasks: " << high_count << std::endl;
}

void TaskManager::sortTasksByTitle() {
    std::sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
        return a.title < b.title;
    });
    std::cout << "Tasks sorted by title." << std::endl;
}