#include <stdexcept>
#include "Sequence.h"
#include "DynamicArray.h"

template <typename T>
class ArraySequence: public Sequence<T> {
protected:
    DynamicArray<T>* items;
    int count;
    size_t capacity;

    virtual Sequence<T>* Instance() = 0;

    Sequence<T>* AppendInternal(const T& item){
        if (count + 1 == capacity){
            capacity = 2*capacity;
            items->Resize(capacity);
        }
        items->Set(count, item);
        count++;
        return this;    
    }

    Sequence<T>* PrependInternal(const T& item){
        if (count == capacity){
            capacity = 2 * capacity;
            items->Resize(capacity);
        }
        for (int i = count; i > 0; i--){
            items->Set(i, items->Get(i - 1));
        }
        items->Set(0, item);
        count++;
        return this;   
    }

    Sequence<T>* InsertAtInternal(const T& item, int index){
        if (index < 0 || index > count){
            throw std::invalid_argument("IndexOutOfRange");
        }
        if (count == capacity){
            capacity = 2 * capacity;
            items->Resize(capacity);
        }
        for (int i = count; i > index; i--){
            items->Set(i, items->Get(i - 1));
        }
        items->Set(index, item);
        count++;
        return this;
    }

    Sequence<T>* ConcatInternal(Sequence<T>* array){
        for (int i = 0; i < array->GetLength(); i++) {
            this->Append(array->Get(i));
        }
        return this;  
    }

public:
    ArraySequence(T* items, int count);
    ArraySequence();
    ArraySequence(const ArraySequence<T>& other);
    virtual ~ArraySequence();

    virtual T GetFirst() const override;
    virtual T GetLast() const override;
    virtual T Get(int index) const override;
    virtual ArraySequence<T>* GetSubsequence(int startIndex, int endIndex) const override;
    virtual int GetLength() const override;

    virtual Sequence<T>* Append(const T& item) override;
    virtual Sequence<T>* Prepend(const T& item) override;
    virtual Sequence<T>* InsertAt(const T& item, int index) override;
    virtual Sequence<T>* Concat(Sequence<T>* list) override;

    virtual const T& operator[](int index) const override;
    virtual T& operator[](int index) override;
    virtual bool operator==(const Sequence<T>& otherList) override;
    virtual bool operator!=(const Sequence<T>& otherList) override;
};

template <typename T>
const T& ArraySequence<T>::operator[](int index) const {
    if (index < 0 || index >= count){
        throw std::invalid_argument("IndexOutOfRange");
    }
    return items->Get(index);
}

template <typename T>
T& ArraySequence<T>::operator[](int index){
    if (index < 0 || index >= count){
        throw std::invalid_argument("IndexOutOfRange");
    }
    return items->Get(index);
}

template <typename T>
bool ArraySequence<T>::operator==(const Sequence<T>& otherList){
    if (this->count != otherList.GetLength()){ return false; }
    for (int i = 0; i < count; ++i){
        if ((*this)[i] != otherList[i]){ return false; }
    }
    return true;
}

template <typename T>
bool ArraySequence<T>::operator!=(const Sequence<T>& otherList){
    if (this->count != otherList.GetLength()){ return true; }
    for (int i = 0; i < count; ++i){
        if ((*this)[i] != otherList[i]){ return true; }
    }
    return false;
}

template <typename T>
ArraySequence<T>::ArraySequence(T* items, int count){
    if (count < 0){
        throw std::invalid_argument("IndexOutOfRange");
    }
    this->capacity = (count > 0) ? (2 * count) : 1;
    this->count = count;    
    this->items = new DynamicArray<T>(items, count);
}

template <typename T>
ArraySequence<T>::ArraySequence(){
    this->items = new DynamicArray(0);
    this->count = 0;
    this->capacity = 1;
}

template <typename T>
ArraySequence<T>::ArraySequence(const ArraySequence<T>& array){
    this->count = array.count;
    this->capacity = array.capacity;
    this->items = new DynamicArray<T>(*array.items);
}

template <typename T>
ArraySequence<T>::~ArraySequence(){
    delete items;
}

template <typename T>
T ArraySequence<T>::GetFirst() const {
    if (count == 0){
        throw std::invalid_argument("IndexOutOfRange");
    }
    return items->Get(0);
}

template <typename T>
T ArraySequence<T>::GetLast() const {
    if (count == 0){
        throw std::invalid_argument("IndexOutOfRange");
    }
    return items->Get(count - 1);
}

template <typename T>
T ArraySequence<T>::Get(int index) const {
    if (index < 0 || index >= count){
        throw std::invalid_argument("IndexOutOfRange");
    }
    return items->Get(index);
}

template <typename T>
int ArraySequence<T>::GetLength() const {
    return count;
}

template <typename T>
ArraySequence<T>* ArraySequence<T>::GetSubsequence(int startIndex, int endIndex) const {
    if (startIndex < 0 || endIndex >= count || endIndex < startIndex){
        throw std::invalid_argument("IndexOutOfRange");
    }
    int len = endIndex - startIndex;
    T* data = new T[len];
    for (int i = 0; i < len; ++i){
        data[i] = items->Get(startIndex + i);
    }
    MutabelArraySequence<T>* subSeq = new MutableArraySequence<T>(data, len);
    delete[] data;
    return subSeq;
}

template <typename T>
Sequence<T>* ArraySequence<T>::Append(const T& item){
    return Instance()->AppendInternal(item);
}

template <typename T>
Sequence<T>* ArraySequence<T>::Prepend(const T& item){
    return Instance()->PrependInternal(item);
}

template <typename T>
Sequence<T>* ArraySequence<T>::InsertAt(const T& item, int index){
    return Instance()->InsertAtInternal(item, index);
}

template <typename T>
Sequence<T>* ArraySequence<T>::Concat(Sequence<T>* array){
    return Instance()->ConcatInternal(array);
}