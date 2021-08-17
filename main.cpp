#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
using namespace std;

template <class T>
        class ObjectPool {
        public:
            T* Allocate(){
                if(!free_data.empty()){
                    auto ojb_ptr=free_data.front();
                    allocated_data.insert(ojb_ptr);
                    free_data.pop_front();
                    return ojb_ptr;
                }
                T* ojb_ptr=new T;
                allocated_data.insert(ojb_ptr);
                return  ojb_ptr;
            };

            T* TryAllocate(){
                if(free_data.empty()){
                    return nullptr;
                }else{
                    auto ojb_ptr=free_data.front();
                    allocated_data.insert(ojb_ptr);
                    free_data.pop_front();
                    return ojb_ptr;
                }
            };


            void Deallocate(T* object){
                if(allocated_data.find(object)==allocated_data.end()){
                    throw invalid_argument("invalid_argument in allocated_data");
                }else{
                    free_data.push_back(object);
                   allocated_data.erase(object);
                }
            };


            ~ObjectPool(){
                for(auto item:allocated_data){
                    delete item;
                }
                for(auto item:free_data){
                    delete item;
                }
            };

        private:
            // Добавьте сюда поля
            set<T*> allocated_data;
            deque<T*> free_data;

        };

void TestObjectPool() {
    ObjectPool<string> pool;

    auto p1 = pool.Allocate();
    auto p2 = pool.Allocate();
    auto p3 = pool.Allocate();

    *p1 = "first";
    *p2 = "second";
    *p3 = "third";

    pool.Deallocate(p2);
    ASSERT_EQUAL(*pool.Allocate(), "second");

    pool.Deallocate(p3);
    pool.Deallocate(p1);
    ASSERT_EQUAL(*pool.Allocate(), "third");
    ASSERT_EQUAL(*pool.Allocate(), "first");

    pool.Deallocate(p1);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestObjectPool);
    return 0;
}
