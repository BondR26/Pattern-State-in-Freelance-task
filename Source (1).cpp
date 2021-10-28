#include<iostream>
#include<string>
#include<map>
#include<list>
#include<queue>

#include"priority.h"

class TaskState;
class TaskStateContext;
class Appointed_to_complete_state;
class Finished_state;








class TaskState
{
private:

    std::string name;

public:

    TaskState(const std::string& name): name(name) {}

    std::string GetName()
    {
        return name;
    }

    virtual void MakeTheTask(TaskStateContext* sc) = 0;
    virtual void Wait(TaskStateContext* sc) = 0;

};

class TaskStateContext
{
private:

    TaskState* taskstate;

public:

    TaskStateContext(TaskState * taskstate): taskstate(taskstate){}

    void MakeTheTask()
    {
        std::cout << "Making the task with "<< taskstate->GetName() << std::endl;
        taskstate->MakeTheTask(this);
    }

    void Wait()
    {
        std::cout << "Waiting and not doing the task " << taskstate->GetName()<<std::endl;
        taskstate->Wait(this);
    }


    void SetState(TaskState* ts)
    {
        std::cout << "Changing the state from " << taskstate->GetName() << " to " << ts->GetName() << std::endl;
        delete taskstate;
        taskstate = ts;
    }

    TaskState* GetState()
    {
        return this->taskstate;
    }

    ~TaskStateContext()
    {
        if(taskstate != NULL)
            delete taskstate;
    }

};

class Finished_state : public TaskState
{
public:

    Finished_state() :
        TaskState("Finished state") {}

    virtual void MakeTheTask(TaskStateContext* tsc) override
    {
        std::cout << "Task is already finished\n";
    }

    virtual void Wait(TaskStateContext* tsc) override
    {
        std::cout << "Nothing happens, the task is alreadt done\n";
    }

};

class Appointed_to_complete_state: public TaskState
{
public:

    Appointed_to_complete_state(): TaskState("Appointed task state") {}

    virtual void MakeTheTask(TaskStateContext* tsc) override;

    virtual void Wait(TaskStateContext* tsc) override;
};


void Appointed_to_complete_state::MakeTheTask(TaskStateContext* tsc)
{
    tsc->SetState(new Finished_state());
}

void Appointed_to_complete_state::Wait(TaskStateContext* tsc)
{
    std::cout << "You cant wait for too long\n";
}





class TaskNameAndState
{
private:

    std::string task_date;
    TaskStateContext* taskstatecontext;
    PriorityContext* prioritycontext;
public:

    TaskNameAndState(std::string task_date): task_date(task_date), taskstatecontext(new TaskStateContext(new Appointed_to_complete_state)),
        prioritycontext(new PriorityContext(new Important)) {}

    friend std::ostream& operator << (std::ostream& out, const TaskNameAndState& other)
    {
        out << other.task_date << " - " << " \nstate of completion "<<other.taskstatecontext->GetState()->GetName() << "\n Priority "<< other.prioritycontext->GetPriorityState()->GetName() << std::endl;
        return out;
    }

    friend bool operator < (const TaskNameAndState& a, const TaskNameAndState& b)
    {
        return (a.task_date < b.task_date);
    }

    bool operator == (std::string a)
    {
        return this->task_date == a;
    }

    bool operator == (const TaskNameAndState& a)
    {
        return this->task_date == a.task_date;
    }

    void MakeTheTask()
    {
        taskstatecontext->MakeTheTask();
    }

    void Wait()
    {
        taskstatecontext->Wait();
    }

    void MakeMoreImportant()
    {
        prioritycontext->MakeMoreImportant();
    }

    void MakeLessImportant()
    {
        prioritycontext->MakeLessImportant();
    }

    std::string SetTheDate(std::string& date)
    {
        this->task_date = date;
        return this->task_date;
    }
};


typedef std::map<std::string, TaskNameAndState> Mymap;

int main()
{   
    setlocale(LC_ALL, "");

    Mymap task_list;

    char choice;

    while (true)
    {
        system("CLS");

        std::cout << "Please enter what you want to do \n";
        std::cout << "1) создания списка дел,\t 2) установки приоритетов, \n3) установки дат выполнения, \y4) удаление и \n5) изменения дел \t6) show all tasks\n";
        std::cout << "Please enter choice ";
        std::cin >> choice;

        switch (choice)
        {
        case '1':
        {
            std::cin.ignore();
            std::string task_name, date;
            std::cout << "Please enter the name of the task ";
            std::getline(std::cin, task_name);
            std::cout << "Please enter date to which it must be completed ";
            std::getline(std::cin, date);

            task_list.insert(std::make_pair(task_name,TaskNameAndState(date)));
        }
        break;
        case '2':
        {
            std::cin.ignore();
            std::string task_name;
            std::cout << "Please enter task name to change it`s priority ";
            std::getline(std::cin, task_name);
            Mymap::iterator it = task_list.find(task_name);

            if (it != task_list.end())
            {
                int choice;
                std::cout << "Please enter 1 if you want to make the priority higher and 2 if you want to make it less ";
                std::cin >> choice;
                if (choice == 1)
                {
                    it->second.MakeMoreImportant();
                }
                else if (choice == 2)
                {
                    it->second.MakeLessImportant();
                }
                
                std::cin.ignore();
            }
            else
            {
                std::cout << "Such task wasnt found\n";
            }
        }
        break;
        case '3':
        {
            std::cin.ignore();
            std::string task_name;
            std::cout << "Please enter task name to change it`s date ";
            std::getline(std::cin, task_name);
            Mymap::iterator it = task_list.find(task_name);

            if (it != task_list.end())
            {
                std::cout << "Please enter new task date ";
                std::getline(std::cin, task_name);

                it->second.SetTheDate(task_name);

                std::cout << "The new date was set " << it->second << std::endl;
            }
            else
            {
                std::cout << "Such task wasnt found\n";
            }
        }
        break;
        case '4':
        {
            std::cin.ignore();
            std::string task_name;
            std::cout << "Please enter task name to erase ";
            std::getline(std::cin, task_name);
            Mymap::iterator it = task_list.find(task_name);

            if (it != task_list.end())
            {
                task_list.erase(it);
                std::cout << "The task was deleted\n";

            }
            else
            {
                std::cout << "Such task wasnt found\n";
            }
        }
        break;
        case '5':
        {
            std::cin.ignore();
            std::string task_name;
            std::cout << "Please enter task name to change it`s state ";
            std::getline(std::cin, task_name);
            Mymap::iterator it = task_list.find(task_name);

            if (it != task_list.end())
            {
                int choice;
                std::cout << "Please enter 1 if you want to completer the task and 2 if you want to wait a bit longer ";
                std::cin >> choice;
                if (choice == 1)
                {
                    it->second.MakeTheTask();
                }
                else if (choice == 2)
                {
                    it->second.Wait();
                }

            }
            else
            {
                std::cout << "Such task wasn`t found\n";
            }
        }
        break;
        case '6':
        {
            for (auto i : task_list)
            {
                std::cout << i.first << " " << i.second << std::endl;
            }
        }
        break;
        case '7':
        {
            exit(1);
        }
        break;
        }
        
        system("pause");
    }

}
