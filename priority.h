#pragma once

class PriorityContext;
class Not_Important;
class VeryImportant;


class PriorityState
{
    std::string priority_name;

public:

    PriorityState(const std::string& pn) : priority_name(pn) {}

    std::string GetName()
    {
        return priority_name;
    }

    virtual void MakeMoreImportant(PriorityContext* pc) = 0;
    virtual void MakeLessImportant(PriorityContext* pc) = 0;

};

class PriorityContext
{
private:

    PriorityState* priority_state;

public:

    PriorityContext(PriorityState* ps) : priority_state(ps) {}

    void MakeMoreImportant()
    {
        std::cout << "The priority " << priority_state->GetName();
        priority_state->MakeMoreImportant(this);
    }

    void MakeLessImportant()
    {
        std::cout << "The priority " << priority_state->GetName();
        priority_state->MakeLessImportant(this);
    }

    void SetPriorityState(PriorityState* ps)
    {
        std::cout << "The priority was change from " << priority_state->GetName() << " to " << ps->GetName();
        delete priority_state;
        priority_state = ps;
    }

    PriorityState* GetPriorityState()
    {
        return priority_state;
    }

    ~PriorityContext()
    {
        delete priority_state;
    }
};



class Important : public PriorityState
{
public:

    Important() : PriorityState("Important\n") {}

    virtual void MakeMoreImportant(PriorityContext* pc) override;

    virtual void MakeLessImportant(PriorityContext* pc) override;
};

class VeryImportant : public PriorityState
{
public:

    VeryImportant() : PriorityState("Very Important\n") {}

    virtual void MakeMoreImportant(PriorityContext* pc) override;

    virtual void MakeLessImportant(PriorityContext* pc) override;

};

class Not_Important : public PriorityState
{
public:
    Not_Important() : PriorityState("Not Important\n") {}

    virtual void MakeMoreImportant(PriorityContext* pc) override;

    virtual void MakeLessImportant(PriorityContext* pc) override;
};

void Important::MakeMoreImportant(PriorityContext* pc)
{
    std::cout << "Task updated to very important\n";
    pc->SetPriorityState(new VeryImportant());
}

void Important::MakeLessImportant(PriorityContext* pc)
{
    std::cout << "Task is update to not important\n";
    pc->SetPriorityState(new Not_Important());
}

void VeryImportant::MakeMoreImportant(PriorityContext* pc) 
{
    std::cout << "Task is already very important\n";
}

void VeryImportant::MakeLessImportant(PriorityContext* pc)
{
    std::cout << "Task updated to important\n";
    pc->SetPriorityState(new Important);
}

void Not_Important::MakeMoreImportant(PriorityContext* pc)
{
    std::cout << "Task updated to important\n";
    pc->SetPriorityState(new Important());
}

void Not_Important::MakeLessImportant(PriorityContext* pc)
{
    std::cout << "Task is already not important\n";

}