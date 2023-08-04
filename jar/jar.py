class Jar:
    def __init__(self, capacity=12):
        if capacity >= 0:
            self._capacity = [0] * capacity
        else:
            print("Invalid capacity")
            raise ValueError
        self._allocated = 0

    def __str__(self):
        cookie = "C"
        return (cookie * self._allocated)

    def deposit(self, n):
        for i in range(self.capacity):
            if not self._capacity[i]:
                id_free = i
                break
        n_free =  self.capacity - (id_free)
        if (n > n_free):
            raise ValueError("No more space")
        else:
            for i in range(n):
                self._capacity[id_free + i] = 1
                self._allocated += 1

    def withdraw(self, n):
        for i in range(self.capacity):
            if self._capacity[i] :
                id_free = i
                break
        n_free = self.capacity - (id_free + 1)
        if (n > n_free or n < 0) :
            raise ValueError("You can't withdraw that")
        else:
            for i in range(n):
                self._capacity[self.capacity - i - 1] = 0
                self._allocated -= 1

    @property
    def capacity(self):
        return len(self._capacity)
    @property
    def size(self):
        return self._allocated

def main():
    f = Jar(3)
    f.deposit(2)
    print(f)
    f.withdraw(1)
    print(f)


main()