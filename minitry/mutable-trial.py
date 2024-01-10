# demo of sneaky python mutable...

class trial:
    def __init__(self, var):
        self.var = var
        self.initvar = var

    def increment(self):
        self.var[0] = self.var[0] + 1

    def printvar(self):
        print("--------------")
        print("Var: ")
        print(self.var)
        print("--------------")

    def printinitvar(self):
        print("--------------")
        print("Init var: ")
        print(self.initvar)
        print("--------------")

def main():
    trial_obj = trial([-1])
    # lists are mutable, expect initvar to change even outside constructor..
    # if trial_obj = trial(-1), then initvar should maintain its sanity..
    for i in range(10):
        trial_obj.increment()
        trial_obj.printvar()
        trial_obj.printinitvar()

if __name__ == "__main__":
    main()
