#include <iostream>
#include <ctime>
#include <string>

using namespace std;

class Patient
{
public:
    string name;
    string surname;
    string birthday;
    int bloodGroup;
    double bloodVolume;

    Patient(string name, string surname, string birthday, int bloodGroup, double bloodVolume)
    {
        this->name = name;
        this->surname = surname;
        this->birthday = birthday;
        this->bloodGroup = bloodGroup;
        this->bloodVolume = bloodVolume;
    }

    Patient()
    {

    }

    ~Patient()
    {
        cout << "Deleting " << this->name << endl;
    }
 };

class BloodDevice
{
public:
    int bloodGroup;
    bool vaccant;
    
    BloodDevice(int bloodGroup, bool vaccant)
    {
        this->bloodGroup = bloodGroup;
        this->vaccant = vaccant;
    }

    BloodDevice()
    {

    }
};

void printInfo(Patient* patient)
{
    cout << patient->name << endl;
    cout << patient->surname << endl;
    cout << patient->birthday << endl;
    cout << patient->bloodGroup << endl;
}

struct Node {
    Node* next;
    Node* previos;
    Patient* data;
};

class Queue {
public:
    Queue() {
        this->top = nullptr;
        this->last = nullptr;
    }

    ~Queue() {
        Node* iterator = this->top;
        while (iterator != nullptr) {
            delete iterator->data;
            iterator = iterator->next;
        }
    }

    bool isEmpty() {
        return this->top==nullptr;
    }

    void add(Patient* data) {
        Node* node = new Node;
        node->data = data;
        if (this->top == nullptr) {
            this->top = node;
            this->last = node;
            node->previos = nullptr;
            node->next = nullptr;
        }
        else {
            node->previos = this->last;
            node->next = nullptr;
            this->last->next = node;
            this->last = node;
        }
    }

    void print() {
        Node* iterator = this->top;
        while (iterator != nullptr) {
   //         cout << *iterator->data << endl;
            iterator = iterator->next;
        }
    };

    Patient* pop() {
        if (this->top != nullptr) {
            Node* toDelete = this->top;
            this->top = this->top->next;
            return toDelete->data;
        }
        return nullptr;
    }
    Node* getTop() {
        return this->top;
    }
    Node* getLast() {
        return this->last;
    }

private:
    Node* top;
    Node* last;
};

string generateRandomStr()
{
    const int len = 5;
    string tmp_s;
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    srand((unsigned)time(NULL) * rand());
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i)
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];

    return tmp_s;
}

Patient* generateRandom(int i)
{
    return new Patient(to_string(i), generateRandomStr(), generateRandomStr(), rand() % 4 + 1, rand() % 3);
}

void fillingPatient(Patient* patient) 
{
    cout << "Enter patient's name: "<< endl;
    cin >> patient->name;
    cout << "Enter patient's surname: " << endl;
    cin >> patient->surname;
    cout << "Enter patient's birthday: " << endl;
    cin >> patient->birthday;
    cout << "Enter patient's blood group: " << endl;
    cin >> patient->bloodGroup;
    cout << "Enter patient's blood volume: " << endl;
    cin >> patient->bloodVolume;
};

void transferToWaitingQueue(Patient* patient, Queue* queue)
{
    cout << "Addind patient to queue: " << patient->name << endl;
    queue->add(patient);
    printInfo(patient);
}

void patientPrepare(Patient* patient, BloodDevice* device)
{
    device->vaccant = false;
    cout << "Procedure with patient " << patient->name << " is running BL " << patient->bloodGroup << endl;
    cout << "Device is not vaccant" << endl;
}

bool bloodGroupMatcher(Patient* patient, BloodDevice* devices[])
{
    bool flag = false;
    for (int i = 0; i < 6; i++)
    {
        BloodDevice *temp = devices[i];
        if (temp->vaccant == true && temp->bloodGroup == patient->bloodGroup)
        {
            patientPrepare(patient, temp);
            flag = true;
            break;
        }
    }
    return flag;
}

void devicesExecuter(BloodDevice* devices[])
{
    for (int i = 0; i < 6; i++)
    {
        devices[i]->vaccant=true;
    }
}

void tranferToWardQueue(Queue* queueWaiting, Queue* queueWard, BloodDevice *devices[])
{
    Patient* temp = queueWaiting->pop();
    if (!bloodGroupMatcher(temp, devices))
    {
        cout << "Devices are executed because there were not enough space" << endl;
        devicesExecuter(devices);
        tranferToWardQueue(queueWaiting, queueWard, devices);
    }
    cout << "Transfering patient from waiting to ward:" << endl;
    queueWard->add(temp);
    printInfo(temp);
}

void changeBloodVolume(Patient* patient)
{
    double value;
    cout << "Enter the desired value" << endl;
    cin >> value;
    cout << "Previous value was : " << patient->bloodVolume << endl;
    patient->bloodVolume = value;
    cout << "The blood value has been changed" << endl;
}

int main()
{
    srand(time(NULL));
    Queue* toDevice = new Queue();
    Queue* toWard = new Queue();
    
    BloodDevice *devices[6];
    devices[0] = new BloodDevice(1, true);
    devices[1] = new BloodDevice(1, true);
    devices[2] = new BloodDevice(3, true);
    devices[3] = new BloodDevice(3, true);
    devices[4] = new BloodDevice(2, true);
    devices[5] = new BloodDevice(4, true);

    Patient* patients[16];

    for (int i = 0; i < 16; i++)
    {
        patients[i] = generateRandom(i);
    }

    int waitingCounter = 0;
    cout << "Tranfering patients to waiting queue" << endl;
    for (waitingCounter = 0; waitingCounter < 10; waitingCounter++)
    {
        transferToWaitingQueue(patients[waitingCounter], toWard);
    }


    cout << "Tranfering patients to ward: " << endl;
    for (int i = 0; i < 6; i++)
    {
        tranferToWardQueue(toWard, toDevice, devices);
        transferToWaitingQueue(patients[waitingCounter++], toWard);
    }
    
    changeBloodVolume(patients[5]);

}