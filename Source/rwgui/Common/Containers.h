#pragma once
#include "Threading.h"
#include "DebugHelpers.h"
#include <Common/Memory.h>

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
		rw::memcpy(Data, Other.Data, Count*sizeof(T));
	};

	List(const T* inData, size_t inSize)
	{
		Init(inSize);
		if (Count == 0) return;
		rw::memcpy(Data, inData, inSize * sizeof(T));
	};

	List(const List&& Other) noexcept
	{
		Init(Other.Count);
		if (Count == 0) return;
		for (size_t id = 0; id < Count; id++)
		{
			Data[id] = Other.Data[id];
		}
		rw::memset(Other.Data, 0, Count * sizeof(T));
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
		for(size_t id = 0;id<Count;id++)
		{
			Data[id] = Other.Data[id];
		}
		return *this;
	}


	List& operator=(const List&& Other)
	{
		Init(Other.Count);
		if (Count == 0) return *this;
		rw::memmove(Data, Other.Data, Count * sizeof(T));
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
		rw::memset(Data, 0, sizeof(T)*Count);
	};

	size_t Insert(const T& inData, size_t Position)
	{
		if (Position > Count) Position = Count;
		Count++;
		T* NewData = new T[Count];
		if(Data!=nullptr)
		{
			for(size_t i = 0;i<Position;i++)
			{
				NewData[i] = Data[i];
			}
			for(size_t i = Position;i<Count-1;i++)
			{
				NewData[i + 1] = Data[i];
			}
			//rw::memcpy(NewData, Data, Position*sizeof(T));
			//rw::memcpy(NewData + Position + 1, Data + Position, (Count - Position)*sizeof(T));
		}
		NewData[Position] = inData;
		//rw::memcpy(NewData + Position, &inData, sizeof(T)); // We should not copy everything as is because copy logic will not work in this case
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
			if(Position>0) rw::memcpy(NewData, Data, (Position) * sizeof(T));
			rw::memcpy(NewData + Position, Data + Position + 1, (Count - Position) * sizeof(T));
			delete[] Data;
		}
		Data = NewData;
		return true;
	};

	T& Add(const T& inData)
	{
		return Data[Insert(inData, Count)];
	};

	T& FindOrAdd(const T& inData)
	{
		for (size_t id = 0; id < Size(); id++)
		{
			if (Data[id] == inData)
				return Data[id];
		}
		return Add(inData);
	}

	bool AddUnique(const T& inData)
	{
		if(!Contains(inData))
		{
			Add(inData);
			return true;
		}
		return false;
	}

	void Append(const List& List)
	{
		if (List.Count < 1) return;
		T* NewData = new T[Count + List.Count];
		if(Data!=nullptr)
		{
			rw::memcpy(NewData, Data, Count * sizeof(T));
			delete[] Data;
		}
		rw::memcpy(NewData + Count, List.Data, List.Count * sizeof(T));
		Data = NewData;
		Count = Count + List.Count;
	}

	size_t Size() const { return Count; };

	T*  begin() const { return &Data[0]; };
	T* end() const { return &Data[Count]; };

	T& operator[](size_t Position) const { return Data[Position]; };

	List& operator=(const T inData[])
	{
		Init(sizeof(inData));
		rw::memcpy(Data, inData, sizeof(inData));
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

	bool Contains(const T& Other) const
	{
		return FindIndex(Other) > -1;
	}

	size_t FindIndex(const T& Other) const
	{
		for(size_t id = 0; id < Size();id++)
		{
			if (Data[id] == Other)
				return id;
		}
		return -1;
	};

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
		};
		~Entry()
		{
		}
	};

private:
	rw::threading::mutex mutex;
	size_t Count = 0;
	Entry* First = nullptr;
public:
	void Add(const T& Data)
	{
		bool bWasLocked = false;
		rw::threading::ScopeLock Lock(&mutex);
		if (First)
		{
			bWasLocked = true;
			First->mutex.Lock();
		}

		++Count;
		Entry* NewEntry = new Entry(this, Data);
		rw::threading::ScopeLock NewEntryLock(&NewEntry->mutex);
		if(First==nullptr)
		{
			First = NewEntry;
			NewEntry->prev = NewEntry;
			NewEntry->next = NewEntry;
		} else if(First->next==First)
		{
			NewEntry->prev = First;
			NewEntry->next = First;
			First->prev = NewEntry;
			First->next = NewEntry;
			if (First && bWasLocked) First->mutex.Release();
			First = NewEntry;
		} else
		{
			rw::threading::ScopeLock FirstPrevLock(&First->prev->mutex);
			NewEntry->prev = First->prev;
			NewEntry->prev->next = NewEntry;
			NewEntry->next = First;
			First->prev = NewEntry;
			if (First && bWasLocked) First->mutex.Release();
			First = NewEntry;
		}
	}


	void Remove(const T& Element)
	{
		mutex.Lock(); // Lock Container;
		if(First)
		{
			Entry* pEntry = First;
			do
			{
				rw::threading::ScopeLock Lock(&pEntry->mutex);
				Entry* next = pEntry->next;
				if(pEntry->Data==Element)
				{
					--Count;
					for(Iterator* Iterator : Iterators)
					{
						if(Iterator->CurrentEntry==pEntry)
						{
							Iterator->CurrentEntry = nullptr;
							Iterator->_next = next;
						}
					}

					if(pEntry->prev!= pEntry)
					{
						rw::threading::ScopeLock prevLock(&pEntry->prev->mutex);
						pEntry->prev->next = pEntry->next;
					}
					if(pEntry->next!= pEntry)
					{
						rw::threading::ScopeLock prevLock(&pEntry->prev->mutex);
						pEntry->next->prev = pEntry->prev;
					}
					if(First==pEntry)
					{
						if (pEntry->next)
							First = pEntry->next;
						else
							First = nullptr;
					}
					delete pEntry;
				}
				pEntry = next;
			} while (pEntry!=nullptr && pEntry != First);			
		}
		mutex.Release();
	}

	class Iterator
	{
	private:
		LinkedList* Container = nullptr;
		Entry* CurrentEntry = nullptr;
		Entry* _next = nullptr; // this only should be set on entry remove
		Iterator() {};
		Iterator(LinkedList* inContainer, Entry* pEntry)
			: Container(inContainer)
			, CurrentEntry(pEntry)
		{
			rw::threading::ScopeLock Lock(&Container->iterators_mutex);
			Container->Iterators.Add(this);
		}
		friend class LinkedList;
	public:
		Iterator(const Iterator& Other)
			: Container(Other.Container)
			, CurrentEntry(Other.CurrentEntry)
		{
			rw::threading::ScopeLock Lock(&Container->iterators_mutex);
			Container->Iterators.Add(this);
		};
		~Iterator()
		{
 			Container->Iterators.Remove(this);
		}
		T& Get() const { return CurrentEntry->Data; };
		operator T&() const { return Get(); };
		T& operator->() const { return Get(); };
		Iterator Next(bool bLoop = false)
		{
			rw::threading::ScopeLock Lock(&Container->mutex);
			Entry* Next = nullptr;
			if (CurrentEntry == nullptr)
			{
				if (_next == nullptr)
					return Iterator(Container, Container->First);
				else
					Next = _next;
			} else {
				Next = CurrentEntry->next;
			}
			if (Next == Container->First)
				if (!bLoop)
					return Iterator(Container, nullptr);
				else
					return Iterator(Container, Container->First);
			if (Next == CurrentEntry) return *this;
			return Iterator(Container, Next);
		};
		bool IsValid() const { return CurrentEntry != nullptr; };
		bool IsFirst() const { return Container && CurrentEntry ? Container->First == CurrentEntry : false; };
	};

	Iterator Itr()
	{
		rw::threading::ScopeLock Lock(&mutex);
		return Iterator(this, First);
	}
private:
	rw::threading::mutex iterators_mutex;
	List<class LinkedList<T>::Iterator*> Iterators;
};