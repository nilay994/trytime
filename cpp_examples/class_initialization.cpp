/** 
 * @brief Checks how classes/params are initialized in CPP
 */

#include <iostream>
#include <memory>

class base_class {
    public:
    base_class(std::shared_ptr<int> val) : m_baseclass_param(val)  {
        std::cout << "baseclass init complete: " << *m_baseclass_param << std::endl;
    }

    // protected members cannot be accessed outside the class, but allowed to be accessed
    // from inherited classes
    protected:
        std::shared_ptr<int> m_baseclass_param;
};

class derived_class final : public base_class {
    public:
    derived_class(std::shared_ptr<int> val) : base_class(val), m_derived_class_param(val), m_derived_class_param_2(10) {
        std::cout << "derived_class init: " << *m_derived_class_param << std::endl;
        std::cout << "derived_class init 2: " << m_derived_class_param_2 << std::endl;
    }

    protected:

        // would the below be a new class object, unknown to C++, it would expect a 
        // default constructor to be defined for the object.
        // And when you don't initialize such custom class objects in the constructor of
        // this class, then it would throw a compiler error.
        std::shared_ptr<int> m_derived_class_param; // replicates m_Topic
        int m_derived_class_param_2; // replicates m_Topic
};

void fn(std::string& val)
{
    val = "50";
}

int main()
{
    auto val = std::make_shared<int> ();
    *val = 5;
    derived_class obj(val);

    std::string st = "hello";
    fn(st);
    std::cout << "st: " << st << std::endl;
}
