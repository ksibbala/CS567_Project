#include "task_manager.h"
#include <deepstate/DeepState.hpp>
#include <sstream>
#include <iostream>

using namespace std;
using namespace deepstate;

TEST(TaskManagementTest, AddTask)
{
    TaskManager task_manager;

    std::string title = DeepState_CStrUpToLen(MAX_TITLE_LENGTH);
    std::string description = DeepState_CStrUpToLen(MAX_DESC_LENGTH);
    Priority priority = static_cast<Priority>(DeepState_IntInRange(0, 2));

    // Add task
    task_manager.addTask(title, description, priority);

    // Check that the task is added
    DeepState_Assert(task_manager.getTaskCount() == 1);
}

TEST(TaskManagementTest, FindTask)
{
    TaskManager task_manager;

    // Add some tasks
    task_manager.addTask("Shopping", "Plan a day for shopping", Priority::MEDIUM);
    task_manager.addTask("Cook", "Cook some good food", Priority::MEDIUM);

    int task_id = DeepState_IntInRange(1, 2);

    // Find the task using the symbolic task_id
    Task* found_task = task_manager.findTask(task_id);

    if (task_id == 1)
    {
        DeepState_Assert(found_task != nullptr);
        DeepState_Assert(found_task->task_id == 1);
    }
    else if (task_id == 2)
    {
        DeepState_Assert(found_task != nullptr);
        DeepState_Assert(found_task->task_id == 2);
    }

    int invalid_task_id = DeepState_IntInRange(3, 10);
    Task* invalid_task = task_manager.findTask(invalid_task_id);
    DeepState_Assert(invalid_task == nullptr);
}

TEST(TaskManagementTest, DeleteTask)
{
    TaskManager task_manager;

    // Add some tasks
    task_manager.addTask("Complete Project", "Finish Assurance Project", Priority::HIGH);
    task_manager.addTask("Vacation", "Plan vacation", Priority::MEDIUM);

    // Check that the task is added
    DeepState_Assert(task_manager.getTaskCount() == 2);

    int task_id = DeepState_IntInRange(1, 2);

    // Delete the task
    task_manager.deleteTask(task_id);

    // Check that the task count is reduced
    DeepState_Assert(task_manager.getTaskCount() == 1);
}

TEST(TaskManagementTest, UpdateTask)
{
    TaskManager task_manager;

    // Add a task
    task_manager.addTask("Test Task", "This is a test task", Priority::LOW);
    DeepState_Assert(task_manager.getTaskCount() == 1);

    int task_id = DeepState_IntInRange(1, 1);
    std::string new_title = DeepState_CStrUpToLen(MAX_TITLE_LENGTH);
    std::string new_description = DeepState_CStrUpToLen(MAX_DESC_LENGTH);
    Priority new_priority = static_cast<Priority>(DeepState_IntInRange(0, 2));

    // Update the task
    task_manager.updateTask(task_id, new_title, new_description, new_priority);
    Task* found_task = task_manager.findTask(task_id);

    // Verify the task was updated
    DeepState_Assert(task_manager.getTaskCount() == 1); 

    // Assert that the found task's details have been updated
    DeepState_Assert(found_task != nullptr);
    DeepState_Assert(found_task->task_id == task_id);

    // Verify that the title, description, and priority are updated
    DeepState_Assert(found_task->title == new_title);
    DeepState_Assert(found_task->description == new_description);
    DeepState_Assert(found_task->priority == new_priority);
}

TEST(TaskManagementTest, MarkTaskCompleted)
{
    TaskManager task_manager;

    // Add a task
    task_manager.addTask("Test Task", "Complete the test task", Priority::HIGH);
    DeepState_Assert(task_manager.getTaskCount() == 1);

    int task_id = 1;
    Task* found_task = task_manager.findTask(task_id);

    // Verify task is initially not completed
    DeepState_Assert(found_task != nullptr);
    DeepState_Assert(found_task->is_completed == false);

    // Mark the task as completed
    task_manager.markTaskCompleted(task_id);

    // Retrieve the task again
    found_task = task_manager.findTask(task_id);

    // Verify the task is now marked as completed
    DeepState_Assert(found_task != nullptr);
    DeepState_Assert(found_task->is_completed == true);
}

TEST(TaskManagementTest, DisplayTaskDetails)
{
    TaskManager task_manager;

    // Add a task
    task_manager.addTask("Test Task", "Complete the test task", Priority::HIGH);
    DeepState_Assert(task_manager.getTaskCount() == 1);

    int task_id = 1;
    Task* found_task = task_manager.findTask(task_id);

    // Verify that the task exists before displaying its details
    DeepState_Assert(found_task != nullptr);

    // Redirect std::cout to a stringstream to capture the output
    stringstream output_stream;
    streambuf* old_cout = cout.rdbuf(output_stream.rdbuf());

    // Call displayTaskDetails, which will print to the stringstream
    task_manager.displayTaskDetails(task_id);

    // Restore the original cout
    cout.rdbuf(old_cout);

    // Check the captured output
    string output = output_stream.str();

    // Verify that the correct task details were printed
    DeepState_Assert(output.find("Task ID: 1") != string::npos);
    DeepState_Assert(output.find("Title: Test Task") != string::npos);
    DeepState_Assert(output.find("Description: Complete the test task") != string::npos);
    DeepState_Assert(output.find("Priority: High") != string::npos);
    DeepState_Assert(output.find("Status: Incomplete") != string::npos);
}

TEST(TaskManagementTest, FormatPriority)
{
    TaskManager task_manager;

    // Test Low Priority
    string low_priority = task_manager.formatPriority(Priority::LOW);
    DeepState_Assert(low_priority == "Low");

    // Test Medium Priority
    string medium_priority = task_manager.formatPriority(Priority::MEDIUM);
    DeepState_Assert(medium_priority == "Medium");

    // Test High Priority
    string high_priority = task_manager.formatPriority(Priority::HIGH);
    DeepState_Assert(high_priority == "High");

    // Test Unknown Priority (this tests the default case)
    string unknown_priority = task_manager.formatPriority(static_cast<Priority>(999)); // Invalid priority
    DeepState_Assert(unknown_priority == "Unknown");
}

TEST(TaskManagementTest, FormatStatus)
{
    TaskManager task_manager;

    // Test for Incomplete status
    string incomplete_status = task_manager.formatStatus(false);
    DeepState_Assert(incomplete_status == "Incomplete");

    // Test for Completed status
    string completed_status = task_manager.formatStatus(true);
    DeepState_Assert(completed_status == "Completed");
}

TEST(TaskManagementTest, DisplayAllTasks)
{
    TaskManager task_manager;

    // Add some tasks
    task_manager.addTask("Task 1", "Test task 1", Priority::LOW);
    task_manager.addTask("Task 2", "Test task 2", Priority::MEDIUM);

    // Redirect output to capture the print statements
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    task_manager.displayAllTasks();

    // Check if tasks are displayed correctly
    std::string output = buffer.str();
    DeepState_Assert(output.find("Task ID:") != std::string::npos);
    DeepState_Assert(output.find("Title:") != std::string::npos);

    // Restore the original stdout
    std::cout.rdbuf(old);
}

TEST(TaskManagementTest, DisplayCompletedTasks)
{
    TaskManager task_manager;

    // Add tasks
    task_manager.addTask("Task 1", "Test task 1", Priority::LOW);
    task_manager.addTask("Task 2", "Test task 2", Priority::MEDIUM);
    task_manager.updateTaskStatus(1, true);

    // Redirect output to capture the print statements
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    task_manager.displayCompletedTasks();

    // Check if completed task is listed
    std::string output = buffer.str();
    DeepState_Assert(output.find("Task 1") != std::string::npos);
    DeepState_Assert(output.find("Task 2") == std::string::npos);

    // Restore the original stdout
    std::cout.rdbuf(old);
}

TEST(TaskManagementTest, DisplayIncompleteTasks)
{
    TaskManager task_manager;

    // Add tasks
    task_manager.addTask("Task 1", "Test task 1", Priority::LOW);
    task_manager.addTask("Task 2", "Test task 2", Priority::MEDIUM);

    // Mark task 1 as completed
    task_manager.updateTaskStatus(1, true);

    // Redirect output to capture the print statements
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    task_manager.displayIncompleteTasks();

    // Check if incomplete task is listed
    std::string output = buffer.str();
    DeepState_Assert(output.find("Task 2") != std::string::npos);
    DeepState_Assert(output.find("Task 1") == std::string::npos);

    // Restore the original stdout
    std::cout.rdbuf(old);
}

TEST(TaskManagementTest, CountTasksByStatus)
{
    TaskManager task_manager;

    // Add tasks
    task_manager.addTask("Task 1", "Test task 1", Priority::LOW);
    task_manager.addTask("Task 2", "Test task 2", Priority::MEDIUM);
    task_manager.updateTaskStatus(1, true);

    // Redirect output to capture the print statements
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    task_manager.countTasksByStatus();

    // Check if counts are printed correctly
    std::string output = buffer.str();
    DeepState_Assert(output.find("Completed tasks: 1") != std::string::npos); // One completed task
    DeepState_Assert(output.find("Incomplete tasks: 1") != std::string::npos); // One incomplete task

    // Restore the original stdout
    std::cout.rdbuf(old);
}

TEST(TaskManagementTest, ClearCompletedTasks)
{
    TaskManager task_manager;

    // Add tasks
    task_manager.addTask("Task 1", "Test task 1", Priority::LOW);
    task_manager.addTask("Task 2", "Test task 2", Priority::MEDIUM);
    task_manager.updateTaskStatus(1, true); // Mark task 1 as completed

    // Clear completed tasks
    task_manager.clearCompletedTasks();

    // Check if completed tasks were cleared
    DeepState_Assert(task_manager.getTaskCount() == 1); // Only 1 task should remain
    DeepState_Assert(task_manager.findTask(1) == nullptr); // Task 1 should be gone
}

TEST(TaskManagementTest, SortTasksByPriority)
{
    TaskManager task_manager;

    // Add tasks with different priorities
    task_manager.addTask("Task 1", "Test task 1", Priority::HIGH);
    task_manager.addTask("Task 2", "Test task 2", Priority::LOW);
    task_manager.addTask("Task 3", "Test task 3", Priority::MEDIUM);

    // Sort tasks by priority
    task_manager.sortTasksByPriority();

    // Check if tasks are sorted
    Task* first_task = task_manager.findTask(2); // LOW priority
    Task* second_task = task_manager.findTask(3); // MEDIUM priority
    Task* third_task = task_manager.findTask(1); // HIGH priority

    DeepState_Assert(first_task->priority == Priority::LOW);
    DeepState_Assert(second_task->priority == Priority::MEDIUM);
    DeepState_Assert(third_task->priority == Priority::HIGH);
}

TEST(TaskManagementTest, ResetTasks)
{
    TaskManager task_manager;

    // Add tasks
    task_manager.addTask("Task 1", "Test task 1", Priority::LOW);
    task_manager.addTask("Task 2", "Test task 2", Priority::MEDIUM);

    // Reset tasks
    task_manager.resetTasks();

    // Check if all tasks are cleared
    DeepState_Assert(task_manager.getTaskCount() == 0); // No tasks should remain
}

TEST(TaskManagementTest, DisplayTasksByPriority)
{
    TaskManager task_manager;

    // Add tasks with different priorities
    task_manager.addTask("Task 1", "Test task 1", Priority::LOW);
    task_manager.addTask("Task 2", "Test task 2", Priority::MEDIUM);
    task_manager.addTask("Task 3", "Test task 3", Priority::HIGH);

    // Redirect output to capture the print statements
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    task_manager.displayTasksByPriority();

    // Check if tasks are displayed by priority
    std::string output = buffer.str();
    DeepState_Assert(output.find("Low:") != std::string::npos);
    DeepState_Assert(output.find("Medium:") != std::string::npos);
    DeepState_Assert(output.find("High:") != std::string::npos);

    // Restore the original stdout
    std::cout.rdbuf(old);
}

TEST(TaskManagementTest, SearchTaskByTitle)
{
    TaskManager task_manager;

    // Add tasks
    task_manager.addTask("Test Task 1", "Description 1", Priority::LOW);
    task_manager.addTask("Another Task", "Description 2", Priority::MEDIUM);

    // Redirect output to capture the print statements
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    task_manager.searchTaskByTitle("Test");

    // Check if tasks are searched correctly
    std::string output = buffer.str();
    DeepState_Assert(output.find("Test Task 1") != std::string::npos);
    DeepState_Assert(output.find("Another Task") == std::string::npos);

    // Restore the original stdout
    std::cout.rdbuf(old);
}

TEST(TaskManagementTest, SearchTaskByDescription)
{
    TaskManager task_manager;

    // Add tasks with various descriptions
    task_manager.addTask("Task 1", "Test task description one", Priority::LOW);
    task_manager.addTask("Task 2", "Another task with description", Priority::MEDIUM);
    task_manager.addTask("Task 3", "Task description three", Priority::HIGH);

    // Redirect output to capture the print statements
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    // Search for tasks with "description"
    task_manager.searchTaskByDescription("description");

    // Check if tasks with the word "description" are found
    std::string output = buffer.str();
    DeepState_Assert(output.find("Task 1") != std::string::npos);
    DeepState_Assert(output.find("Task 2") != std::string::npos);
    DeepState_Assert(output.find("Task 3") != std::string::npos);

    // Search for tasks with "not found" description, expecting no results
    task_manager.searchTaskByDescription("not found");

    // Restore the original stdout
    std::cout.rdbuf(old);
}

TEST(TaskManagerTest, SearchTaskById) {
    TaskManager task_manager;

    // Add tasks to the manager
    task_manager.addTask("Task 1", "Description for Task 1", Priority::LOW);
    task_manager.addTask("Task 2", "Description for Task 2", Priority::MEDIUM);

    // Valid task_id
    int valid_task_id = 1;
    Task* found_task = task_manager.searchTaskById(valid_task_id);
    DeepState_Assert(found_task != nullptr);  // Task should be found
    DeepState_Assert(found_task->task_id == valid_task_id);  // Task ID should match

    // Invalid task_id
    int invalid_task_id = 999;
    found_task = task_manager.searchTaskById(invalid_task_id);
    DeepState_Assert(found_task == nullptr);  // Task should not be found
}


TEST(TaskManagerTest, NotifyHighPriorityTasks) {
    TaskManager task_manager;

    // Add tasks to the manager
    task_manager.addTask("Low Priority Task", "Description for Low Priority", Priority::LOW);
    task_manager.addTask("Medium Priority Task", "Description for Medium Priority", Priority::MEDIUM);
    task_manager.addTask("High Priority Task", "Description for High Priority", Priority::HIGH);
    task_manager.addTask("Another High Priority Task", "Description for Another High Priority", Priority::HIGH);

    // Capture the output of notifyHighPriorityTasks
    std::stringstream output;
    std::streambuf* original_buf = std::cout.rdbuf();  // Save the original buffer
    std::cout.rdbuf(output.rdbuf());  // Redirect std::cout to capture the output

    // Call the method to notify high-priority tasks
    task_manager.notifyHighPriorityTasks();

    // Check that high-priority tasks are notified
    DeepState_Assert(output.str().find("High-priority task: High Priority Task") != string::npos);
    DeepState_Assert(output.str().find("High-priority task: Another High Priority Task") != string::npos);
    DeepState_Assert(output.str().find("Low Priority Task") == string::npos);  // Ensure low priority tasks are not listed
    DeepState_Assert(output.str().find("Medium Priority Task") == string::npos);  // Ensure medium priority tasks are not listed

    // Restore original std::cout buffer
    std::cout.rdbuf(original_buf);
}
