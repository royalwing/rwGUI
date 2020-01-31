#pragma once
#include "Threading.h"
#include "DebugHelpers.h"

template<typename T>
class List
{
protected:
	size_t Count = 0;
	T* Data = nullptr;;
public:

	typedef bool(*SortFunc)(T const & A, T const & B);

	List()
	{
		Data = nullptr;
		Count = 0;
	};

	List(size_t Size)
	{
		Init(Size);
	}

	List(const List& Other)
	{
		Init(Other.Count);
		if (Count == 0) return;
		memcpy(Data, Other.Data, Count*sizeof(T));
	};

	List(const T* inData, size_t inSize)
	{
		Init(inSize);
		if (Count == 0) return;
		memcpy(Data, inData, inSize * sizeof(T));
	};

	List(const List&& Other) noexcept
	{
		Init(Other.Count);
		if (Count == 0) return;
		memmove(Data, Other.Data, Count * sizeof(T));
	}

	~List()
	{
		Count = 0;
		if (Data!=nullptr)
			delete[] Data;
	};

	List& operator=(const List& Other)
	{
		Init(Other.Count);
		if (Count == 0) return *this;
		memcpy(Data, Other.Data, Count * sizeof(T));
		return *this;
	}


	List& operator=(const List&& Other)
	{
		Init(Other.Count);
		if (Count == 0) return *this;
		memmove(Data, Other.Data, Count * sizeof(T));
		return *this;
	}

	void Init(size_t Size)
	{
		if (Data!=nullptr)
			delete[] Data;
		Count = Size;
		if (Size == 0)
		{
			Data = nullptr;
			return;
		}
		Data = new T[Count];
		memset(Data, 0, sizeof(T)*Count);
	};

	size_t Insert(const T& inData, size_t Position)
	{
		if (Position > Count) Position = Count;
		T* NewData = new T[++Count];
		if(Data!=nullptr)
		{
			memcpy(NewData, Data, Position*sizeof(T));
			memcpy(NewData + Position + 1, Data + Position, (Count - Position)*sizeof(T));
		}
		memcpy(NewData + Position, &inData, sizeof(T));
		if(Data!=nullptr)
			delete[] Data;

		Data = NewData;
		return Position;
	};

	bool Remove(const T& Target)
	{
		bool bRemovedSomething = false;
		for(signed int pos = Size()-1;pos>-1;pos--)
		{
			if(Data[pos]==Target)
			{
				RemoveAt(pos);
				bRemovedSomething = true;
			}
		}
		return bRemovedSomething;
	}

	bool RemoveAt(size_t Position)
	{
		if (Position > Count-1) return false;
		if(Count==1)
		{
			delete[] Data;
			Count = 0;
			Data = nullptr;
			return true;
		}
		T* NewData = new T[--Count];
		if(Data!=nullptr)
		{
			if(Position>0) memcpy(NewData, Data, (Position) * sizeof(T));
			memcpy(NewData + Position, Data + Position + 1, (Count - Position) * sizeof(T));
			delete[] Data;
		}
		Data = NewData;
		return true;
	};

	void Add(const T& inData)
	{
		Insert(inData, Count);
	};

	void Append(const List& List)
	{
		if (List.Count < 1) return;
		T* NewData = new T[Count + List.Count];
		if(Data!=nullptr)
		{
			memcpy(NewData, Data, Count * sizeof(T));
			delete[] Data;
		}
		memcpy(NewData + Count, List.Data, List.Count * sizeof(T));
		Data = NewData;
	}

	size_t Size() const { return Count; };

	T* begin() const { return &Data[0]; };
	T* end() const { return &Data[Count]; };

	T& operator[](size_t Position) const { return Data[Position]; };

	List& operator=(const T inData[])
	{
		Init(sizeof(inData));
		memcpy(Data, inData, sizeof(inData));
		return *this;
	};

	operator T* const() const
	{
		return Data;
	}

	bool operator==(const T* Other)
	{
		return Data == Other;
	}

	bool IsValidIndex(signed int Index) const
	{
		return Index > -1 && Index < Size();
	}

	void Swap(size_t From, size_t To)
	{
		if (!IsValidIndex(From) && !IsValidIndex(To)) return;
		T* buffer = new T;
		*buffer = Data[To];
		Data[To] = Data[From];
		Data[From] = *buffer;
		delete buffer;
	}

	void Sort(SortFunc Predicate)
	{
		for(size_t pos = 0;pos<Size();pos++)
		{
			size_t startpos = pos;
			while(IsValidIndex(startpos + 1) && Predicate(Data[startpos], Data[startpos + 1]))
			{
				Swap(startpos, startpos + 1);
				startpos++;
			}
		}
	}

	typedef void(*ElementIteratingFunc)(const T&);;
};

template<typename T>
class LinkedList
{
public:
	class Entry
	{
	private:
		rw::threading::mutex mutex;
		T Data;
		Entry* next = nullptr;
		Entry* prev = nullptr;
		LinkedList* Container = nullptr;
		friend class LinkedList;
		Entry() = delete;
		Entry(const Entry&) = delete;
		Entry(LinkedList* inContainer, const T& inData, Entry* inPrev = nullptr, Entry* inNext = nullptr)
			: Data(inData), next(inNext), prev(inPrev), Container(inContainer)
		{
			rw::threading::ScopeLock LockContainer(&(Container->mutex));
			rw::threading::ScopeLock LockSelf(&mutex);

			++Container->Count;


			if (next == nullptr) next = this;
			if (prev == nullptr) prev = this;

			if (next != this)
				next->mutex.Lock();

			if (prev != this && prev!=next)
				prev->mutex.Lock();

			if (next) next->prev = this;
			if (prev) prev->next = this;

			if (next != this)
				next->mutex.Release();

			if (prev != this && prev!=next)
				prev->mutex.Release();
		};
		~Entry()
		{
			rw::threading::ScopeLock LockContainer(&(Container->mutex));
			--Container->Count;

			if (next != this)
				next->mutex.Lock();

			if (prev != this)
				prev->mutex.Lock();

			if (next) next->prev = prev;
			if (prev) prev->next = next;

			if (next != this)
				next->mutex.Release();

			if (prev != this)
				prev->mutex.Release();
		}
	};

private:
	rw::threading::mutex mutex;
	size_t Count = 0;
	Entry* First = nullptr;
public:
	void Push(const T& Data)
	{
		size_t inCount = 0;
		{
			rw::threading::ScopeLock Lock(&mutex);
			inCount = Count;
		}
		if(inCount==0)
		{
			First = new Entry(this, Data);
		} else {
			new Entry(this, Data, First->prev, First);
		}
	}

	T Pop()
	{
		rw::threading::ScopeLock Lock(&mutex);
		T Data = First->Data;
		if (First->next && First->next != First)
			First = First->next;
		else
			First = nullptr;
		delete First;
		return Data;
	}

	void Remove(const T& Element)
	{
		mutex.Lock(); // Lock Container;
		Entry* Itr = First;
		mutex.Release();
		do
		{
			Itr->mutex.Lock();
			Entry* NextItr = Itr->next;
			if(Itr->Data==Element)
			{
				Itr->mutex.Release();
				delete Itr;
			} else
			{
				Itr->mutex.Release();
			}
			Itr = NextItr;;
		} while (Itr != First);
	}

	class Iterator
	{
	private:
		LinkedList* Container = nullptr;
		Entry* CurrentEntry = nullptr;
		Iterator() {};
		Iterator(LinkedList* inContainer, Entry* pEntry)
			: Container(inContainer)
			, CurrentEntry(pEntry)
		{
			
		}
		friend class LinkedList;
	public:
		Iterator(const Iterator& Other)
			: Container(Other.Container)
			, CurrentEntry(Other.CurrentEntry)
		{

		};
		T& Get() const { return CurrentEntry->Data; };
		operator T&() const { return Get(); };
		T& operator->() const { return Get(); };
		Iterator Next()
		{
			if (CurrentEntry == nullptr) return Iterator(Container, Container->First);
			if (CurrentEntry->next == CurrentEntry) return *this;
			return Iterator(Container, CurrentEntry->next);
		};
		bool IsValid() const { return CurrentEntry != nullptr; };
		bool IsFirst() const { return Container && CurrentEntry ? Container->First == CurrentEntry : false; };
	};

	Iterator Itr()
	{
		return Iterator(this, First);
	}
};