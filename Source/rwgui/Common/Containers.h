#pragma once


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
};

