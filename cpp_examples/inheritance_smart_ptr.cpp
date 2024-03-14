#include <iostream>
#include <vector>

class Animal {
public:
    virtual void speak() const {
        std::cout << "This animal makes a sound." << std::endl;
    }
    // Base class destructor will always be called after the derived (even if it is overriden)
    virtual ~Animal() {
       std::cout << "Animal destructor called" << std::endl;
    }
};

class Cat : public Animal {
public:
    void speak() const override {
        std::cout << "Meow" << std::endl;
    }
    ~Cat() override {
        std::cout << "Cat destructor called" << std::endl;
    }
};

class Dog : public Animal {
public:
    void speak() const override {
        std::cout << "Woof" << std::endl;
    }    
    ~Dog() override {
        std::cout << "Dog destructor called" << std::endl;
    }
};

int main() {
    
    // 1. Slicing example
    std::cout << "\nSlicing test------" << std::endl;
    {
        std::vector<Animal> animals_sliced;
        Cat cat;
        Dog dog;
        
        animals_sliced.push_back(cat);
        animals_sliced.push_back(dog);

        // there might be some potential vector resizing happening here,
        // causing a Animal::~Animal() to be called

        for (const auto & animal : animals_sliced) {
            // slicing occurs here, only base class functions are called    
            animal.speak();
        }
    }

    // 2. Dynamic pointers test, override is successful, i.e. dynamic (late/runtime) binding can prevent slicing
    std::cout << "\nNew pointer test------" << std::endl;
    Animal * animals_ptr;
    {
        // Note 1: classical heap allocation, delete must be called outside the scope, it won't dealloc itself
        // Note 2: use new and delete as a pair, and use malloc and free as a pair. 
        // Interchanging leads to unexpected behaviour.
        animals_ptr = new Cat();
        animals_ptr->speak();
    }
    // Note: since there is no explicit virtual destructor defined in the base class, 
    // the virtual class's destructor isn't called, this is not okay.
    delete(animals_ptr);
    
    // 3. Now avoid slicing using shared pointers
    std::cout << "\nShared pointer test------" << std::endl;
    {
        std::vector<std::shared_ptr<Animal>> animals_shared_ptr; 
        std::shared_ptr<Cat> cat_shared_ptr = std::make_shared<Cat>(); 
        std::shared_ptr<Dog> dog_shared_ptr = std::make_shared<Dog>(); 

        animals_shared_ptr.push_back(cat_shared_ptr);
        animals_shared_ptr.push_back(dog_shared_ptr);
        
        for (size_t i = 0; i < animals_shared_ptr.size(); ++i) {
            animals_shared_ptr[i]->speak();
        }
    }

    // 4. Now avoid slicing using unique pointers
    std::cout << "\nUnique pointer test------" << std::endl;
    {
        std::vector<std::unique_ptr<Animal>> animals_unique_ptr; 
        std::unique_ptr<Cat> cat_unique_ptr = std::make_unique<Cat>(); 
        std::unique_ptr<Dog> dog_unique_ptr = std::make_unique<Dog>(); 

        // cannot do the below without moving ownership, i.e. std::move, 
        // but with shared pointer it was okay to do so without transferring ownership
        animals_unique_ptr.push_back(std::move(cat_unique_ptr));
        animals_unique_ptr.push_back(std::move(dog_unique_ptr));
        
        for (size_t i = 0; i < animals_unique_ptr.size(); ++i) {
            animals_unique_ptr[i]->speak();
        } 
    }

    
    std::cout << "\nProgram end" << std::endl;
    return 0;
}
