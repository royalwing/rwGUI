#pragma once

template<typename T>
class List
{
protected:
	size_t Count = 0;
	T* Data;
public:
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
		memcpy(Data, Other.Data, Count);
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
		memmove(Data, Other.Data, Count);
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
		memcpy(Data, Other.Data, Count);
		return *this;
	}


	List& operator=(const List&& Other)
	{
		Init(Other.Count);
		if (Count == 0) return *this;
		memmove(Data, Other.Data, Count);
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
		Insert(Count, inData);
	};

	void Append(const List& List)
	{
		if (List.Count < 1) return;
		T* NewData = new T[Count + List.Count];
		if(Data!=nullptr)
		{
			memcpy(NewData, Data, Count);
			delete[] Data;
		}
		memcpy(NewData + Count, List.Data, List.Count);
		Data = NewData;
	}

	size_t Size() const { return Count; };

	T* begin() const { return Data; };
	T* end() const { return Data + Count; };

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
};
