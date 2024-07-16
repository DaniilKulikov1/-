#include <iostream>
#include <cstring>

#define SIZE 50
#define NAMELENGTH 30

typedef char Name[NAMELENGTH];


template <class T>
class StaticArray {
public:
    StaticArray(int size = 0) {
        count = size;
        if (size > 0) {
            for (int i = 0; i < size; i++)
                array[i] = T();
        }
    }
    void add(T newval) {
        if (count < SIZE) {
            array[count++] = newval;
        }
    }
    void del(int index) {
        if (index < 0 || index >= count)
            return;
        count--;
        for (int i = index; i < count; i++) {
            array[i] = array[i + 1];
        }
    }
    T& operator [](int index) {
        if (index < 0 || index >= count)
            throw 0;
        return array[index];
    }

    int find(const T& val) {
        for (int i = 0; i < count; i++)
            if (array[i] == val)
                return i;
        return -1;
    }

    int size() {
        return count;
    }

private:
    T array[SIZE];
    int count;
};





class SocialNetwork {
public:
    struct Human {
        Human();
        Human(const Name _name, int _id);
        Human& operator=(const Human& other);
        Human(const Human& other);

        bool operator==(const Human& other);

        Name name;

        friend class SocialNetwork;

    private:
        int id;
    };

    SocialNetwork();
    void addnewMan(const Name name);
    void delMan(const Name name);
    void addFriendship(const Name name_1, const Name name_2);
    void showThreeHandshakes();

private:
    bool are_friends(const Human& m1, const Human& m2);
    bool man_exists_with_id(int man_id);
    Human findManByName(const Name name);

    int friend_matrix[SIZE][SIZE];
    StaticArray<Human> people;
    StaticArray<bool> busyids;
};


SocialNetwork::Human::Human() : id(-1)
{}

SocialNetwork::Human::Human(const Name _name, int _id) : id(_id) {
    strcpy_s(name, _name);
}
SocialNetwork::Human& SocialNetwork::Human::operator=(const Human& other) {
    strcpy_s(name, other.name);
    id = other.id;
    return *this;
}
SocialNetwork::Human::Human(const Human& other)
{
    strcpy_s(name, other.name);
    id = other.id;
}
bool SocialNetwork::Human::operator==(const Human& other) {
    return other.id == this->id;
}

// Реализация методов класса SocialNetwork
SocialNetwork::SocialNetwork() {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            friend_matrix[i][j] = 0;
    for (int i = 0; i < SIZE; i++) {
        busyids.add(false);
    }
}
void SocialNetwork::addnewMan(const Name name) {
    Human man = findManByName(name);
    if (man.id >= 0)
        return;
    int i = 0;
    for (; i < SIZE; i++)
        if (!busyids[i])
            break;
    busyids[i] = true;
    people.add(Human(name, i));
}

void SocialNetwork::delMan(const Name name) {
    Human man = findManByName(name);
    if (man.id < 0)
        return;
    busyids[man.id] = false;
    int index = people.find(man);
    for (int i = 0; i < people.size(); i++) {
        friend_matrix[i][man.id] = 0;
        friend_matrix[man.id][i] = 0;
    }
    people.del(index);
}

void SocialNetwork::addFriendship(const Name name_1, const Name name_2) {
    Human m1 = findManByName(name_1);
    Human m2 = findManByName(name_2);
    if (m1.id < 0 || m2.id < 0)
        return;
    friend_matrix[m1.id][m2.id] = 1;
    friend_matrix[m2.id][m1.id] = 1;
}

void SocialNetwork::showThreeHandshakes() {
    int wavesLevels[SIZE];
    for (int i = 0; i < SIZE; i++) {
        wavesLevels[i] = -1;
    }
    wavesLevels[0] = 0;

    StaticArray<int> queue;
    queue.add(0);

    while (queue.size()) {
        int currentManIndex = queue[0];
        queue.del(0);
        for (int i = 0; i < people.size(); i++)
            if (are_friends(people[currentManIndex], people[i]) &&
                wavesLevels[i] == -1)
            {
                wavesLevels[i] = wavesLevels[currentManIndex] + 1;
                queue.add(i);
            }
    }

    for (int i = 0; i < people.size(); i++) {
        for (int j = i + 1; j < people.size(); j++) {
            if (abs(wavesLevels[i] - wavesLevels[j]) <= 3) {
                std::cout << people[i].name << " " << people[j].name << std::endl;
            }
        }
    }
}


bool SocialNetwork::are_friends(const Human& m1, const Human& m2) {
    return friend_matrix[m1.id][m2.id] == 1;
}

bool SocialNetwork::man_exists_with_id(int man_id) {
    for (int i = 0; i < people.size(); i++)
        if (people[i].id == man_id)
            return true;
    return false;
}

SocialNetwork::Human SocialNetwork::findManByName(const Name name) {
    for (int i = 0; i < people.size(); i++)
        if (!strcmp(name, people[i].name))
            return people[i];
    return Human();
}





int main(char* ar[], int count)
{
    SocialNetwork sn;

    sn.addnewMan("Oleg");
    sn.addnewMan("Kostya");
    sn.addnewMan("Misha");
    sn.addnewMan("Vika");
    sn.addnewMan("Lesha");

    sn.delMan("Vika");
    sn.addnewMan("Anya");

    sn.addFriendship("Oleg", "Kostya");
    sn.addFriendship("Kostya", "Misha");
    sn.addFriendship("Misha", "Anya");
    sn.addFriendship("Anya", "Lesha");

    sn.showThreeHandshakes();

    return 0;
}
