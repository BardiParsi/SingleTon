#include <iostream>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>
#include <cstdint>
#include <vector>
#include <cassert>

using std::cout;
using std::endl;

//Singleton Pattern Lazy Initialization
//Requires C++20 to compile this program
//The code contains 3 asserts to detect the potential bugs. You may remove them for your project.

const size_t CPUThreadNum = std::thread::hardware_concurrency(); //Numbers of CPU Threads in your device
std::mutex coutMtx; //Mutex lock for the std::cout for the console output

class SingleTon{
private:
static std::atomic<SingleTon*> ptrObj; //The class main object to do the heavy work
static std::once_flag initFlag;
static std::mutex mtx;
std::atomic<int32_t> x; //some task examples used in pretending an expensive task.
SingleTon() = default;  //Constructor is inside the Private section for safe initialization
static void cleanup() {
        SingleTon* old = ptrObj.load(std::memory_order_acquire); // delete the ptr manually
        delete old;
    }
public:
static SingleTon* getInstance(){
    //making sure is the object is not initialized before starting the process
    SingleTon* temp = ptrObj.load(std::memory_order_acquire);
    if (temp == nullptr) {
        std::lock_guard<std::mutex> lock(mtx);
        temp = ptrObj.load(std::memory_order_relaxed);
        if (temp == nullptr) {
            std::call_once(initFlag, [&](){
                SingleTon* newInstance = new SingleTon();
                ptrObj.store(newInstance, std::memory_order_release);
            });
            temp = ptrObj.load(std::memory_order_acquire);
        }
    }
    return temp; //May return nullptr
}

void expensiveTask(const int32_t& x){
    //Check the error condition
    if (x <= 0)
    {
        throw std::invalid_argument("Error Invalid Argument!");
    }
    //pretending an expensive task is being done
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    this -> x = x;
    {
        std::lock_guard<std::mutex> lock(coutMtx);
        cout << "The expensive task is completed! the Value of X is: " << x << endl;
        cout << "The thread ID is: " << std::this_thread::get_id() << endl;
    }
    assert(x > 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

static void destroyInstance() {
        cleanup(); // Explicitly call cleanup method
        ptrObj.store(nullptr, std::memory_order_release); // Reset pointer to nullptr
    }

~SingleTon() = default;
};

//pre initialization of the variables of SingleTone's object.
std::atomic<SingleTon*> SingleTon::ptrObj = nullptr;
std::mutex SingleTon::mtx;
std::once_flag SingleTon::initFlag;

int main() {
    assert(CPUThreadNum > 0);
    std::vector<std::jthread> vec; // vector of threads based on the performance of the user device
    try{
        for (size_t i = 0; i < CPUThreadNum; i++) {
        int32_t x = i + 1;
        SingleTon* singleObj = SingleTon::getInstance();
        assert(singleObj != nullptr);
        try
        {
            vec.emplace_back([singleObj, x]() mutable {singleObj->expensiveTask(std::ref(x));});
        }
        catch(const std::invalid_argument& e)
        {
            std::cerr << "\nThe value of X is not coresponded with CPU Thread numbers. " << e.what() << '\n';
        }
        }
        for(auto& thread : vec){
        thread.join();
        }
        // Cleanup at the end of the program
        SingleTon::destroyInstance();
        cout << "\nXXXXXXX THE END OF THE PROGRAM XXXXXXX" << endl;
    }
    catch(const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        // Ensure proper cleanup in case of exception
        SingleTon::destroyInstance();
    }
    
    std::system("pause"); //Is used for VSCODE To pause the console. You may eliminate this.

    return 0;
}
