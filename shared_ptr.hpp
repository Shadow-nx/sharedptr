#include <atomic>
#include <iostream>

using namespace std;
class Storage{
    public:
        atomic_uint count;
    public:
        Storage(){
            count=0;
        }
        auto decrease() -> void{
            count--;
        }
        auto increase() -> void{
            count++;
        }
};

template <typename T>
class Shared_Ptr {
private:
    T* value;
    Storage* storage;
public:
    Shared_Ptr(){
        value = nullptr;
        storage = nullptr;
    }
    Shared_Ptr(T data){
        value = new T(data);
        storage = new Storage();
        storage->increase();
    }
    Shared_Ptr(const Shared_Ptr<T>& r){
        value = r.value;
        storage = r.storage;
        storage -> increase();
    }
    
    auto operator = (Shared_Ptr<T>& r) -> Shared_Ptr<T>&{
        if (value != nullptr) {
		    storage->decrease();
    		if (storage != nullptr) {
			    delete value;
	            value = nullptr;
	            delete storage;
	            storage = nullptr;
		    }
	    }
	    value = r.value;
	    storage = &r.storage;
	    storage->increase();
	    return *this;
    }
    auto use_count() const -> size_t {
	    if (storage != nullptr)
		    return size_t(storage->count);
	    else return 0;
    };
    operator bool() const{
        if (value != nullptr) 
            return true;
	    return false;
    }
    auto operator->() const -> T* {
	    return value;
    }
    auto operator*() const -> T& {
    	if (value != nullptr) 
	        return *value;
    }
    auto get() -> T*{
        if(value != nullptr)
            return value;
        return 0;
    }
    auto reset() -> void{
        if(storage != nullptr){
            storage -> decrease();
            if(storage -> count == 0){
                delete value;
                value = nullptr;
                delete storage;
                storage = nullptr;
            }
            value = nullptr;
            storage = nullptr;
        }
    }
    auto reset(T* ptr) -> void{
        if(storage != nullptr){
            storage -> decrease();
            if(storage -> count == 0){
                delete value;
                value = nullptr;
                delete storage;
                storage = nullptr;
            }
        }
        value = ptr;
        storage = new Storage();
        storage -> increase();
    }
    auto swap(Shared_Ptr& r) -> void{
        std::swap(storage, r.storage);
	    std::swap(value, r.value);
    }
    ~Shared_Ptr(){
        if(storage != nullptr){
            if(storage->count == 0){
                delete value;
                delete storage;
            }
        }
        value = nullptr;
        storage = nullptr;
    }
};
