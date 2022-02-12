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


// TBinartyTree
//	
// This structure allow to keep data and identify it by any identifier that can be compared
// by "more or less" rule. So it can be integer or pointer in example.
//
// For the following keys there is datagram that represents how it will look like.
//		34, 58, 2, 89, 43, 122, 10, 54
//                                                                                   
//                                                                                   
//                       [34]                                                   
//                       /  \
//                      /    \
//                   [2]      [58]                                                    
//                     \      /  \
//                     [10]  /    \
//                         [43]    [89]                                              
//                            \      \
//                             [54]   \
//                                     [122]                                        
//                                                                                   
//
//	This kind of structure allows to search elements faster than simple list O(n) by
//  cutting off half of remaining elements on each stage of check O(log n).


template<typename KEY, typename VALUE>
class TBinaryTree
{
private:
	class TBinaryTreeNode
	{
	public:
		TBinaryTreeNode() {};

		TBinaryTreeNode(TBinaryTree* inTree, TBinaryTreeNode* inParent, KEY inKey, VALUE inValue)
			: Tree(inTree), Parent(inParent), Key(inKey), Value(inValue)
		{
			if (Parent)
			{
				if (Key > Parent->Key)
				{
					Parent->Right = this;
					LeftNeighbour = Parent;
					RightNeighbour = Parent->RightNeighbour;
					if (LeftNeighbour) LeftNeighbour->RightNeighbour = this;
					if(RightNeighbour) RightNeighbour->LeftNeighbour = this;
				} else if (Key < Parent->Key)
				{
					Parent->Left = this;
					LeftNeighbour = Parent->LeftNeighbour;
					RightNeighbour = Parent;
					if (LeftNeighbour) LeftNeighbour->RightNeighbour = this;
					if (RightNeighbour) RightNeighbour->LeftNeighbour = this;
				}
			}
			TBinaryTreeNode* Outer = Parent;
			TBinaryTreeNode* Target = this;
			while (Outer)
			{
				if (Outer->Right == Target)
				{
					Outer->Balance++;
				} else if (Outer->Left == Target)
				{
					Outer->Balance--;
				}
				Target = Outer;
				Outer = Outer->Parent;
			}
			Tree->Count++;


			Outer = Parent;
			while (Outer)
			{
				Tree->Balance(Outer);
				Outer = Outer->Parent;
			}
		};
		~TBinaryTreeNode()
		{
			if (Tree->RootNode == this)
				Tree->RootNode = nullptr;
			TBinaryTreeNode* Outer = Parent;
			TBinaryTreeNode* Target = this;
			while (Outer)
			{
				if (Outer->Right == Target)
				{
					Outer->Balance--;
				}
				else if (Outer->Left == Target)
				{
					Outer->Balance++;
				}
				Target = Outer;
				Outer = Outer->Parent;
			}

			if (Parent)
			{
				if (Parent->Left == this)
					Parent->Left = nullptr;
				if (Parent->Right == this)
					Parent->Right = nullptr;
			}
			if (RightNeighbour)
			{
				RightNeighbour->LeftNeighbour = LeftNeighbour;
			}
			if(LeftNeighbour)
			{
				LeftNeighbour->RightNeighbour = RightNeighbour;
			}

			Tree->Count--;

			Outer = Parent;
			while (Outer)
			{
				Tree->Balance(Outer);
				Outer = Outer->Parent;
			}
		};

		int GetNodeDepth()
		{
			int Depth = 0;
			TBinaryTreeNode* Outer = Parent;
			while (Outer)
			{
				Depth++;
				Outer = Outer->Parent;
			}
			return Depth;
		};

		TBinaryTree* Tree = nullptr;
		KEY Key;
		VALUE Value;

		TBinaryTreeNode* Parent = nullptr;
		TBinaryTreeNode* Left = nullptr;
		TBinaryTreeNode* Right = nullptr;

		TBinaryTreeNode* LeftNeighbour = nullptr;
		TBinaryTreeNode* RightNeighbour = nullptr;

		int Balance = 0;
	};
		
	TBinaryTreeNode* RootNode = nullptr;
	int Count = 0;

	void SwapNodes(TBinaryTreeNode* A, TBinaryTreeNode* B) const
	{
		if (A == nullptr || B == nullptr)
		{
			KEY key = A->Key;
			VALUE value = A->Value;
			TBinaryTreeNode* parent = A->Parent;
			TBinaryTreeNode* rn = A->RightNeighbour;
			TBinaryTreeNode* ln = A->LeftNeighbour;
			A->Key = B->Key;
			A->Value = B->Value;
			A->Parent = B->Parent;
			A->RightNeighbour = B->RightNeighbour;
			A->LeftNeighbour = B->LeftNeighbour;
			B->Key = key;
			B->Value = value;
			B->Parent = parent;
			B->RightNeighbour = rn;
			B->LeftNeighbour = ln;
		}
	};

	void Balance(TBinaryTreeNode* TargetNode)
	{
		// Rule # 1
		if (TargetNode && TargetNode->Left && TargetNode->Left->Right)
		{
			if (TargetNode->Balance == -2 && TargetNode->Left->Balance == 1)
			{
				TBinaryTreeNode *A = TargetNode, *B = TargetNode->Left, *C = TargetNode->Left->Right;
				if (RootNode == A)
					RootNode = C;

				if (A->Parent)
				{
					if (A->Parent->Left == A)
						A->Parent->Left = C;
					else if (A->Parent->Right == A)
						A->Parent->Right = C;
				}


				C->Parent = A->Parent;
				B->Parent = C;
				A->Parent = C;

				C->Left = B;
				C->Right = A;

				A->Left = nullptr;
				B->Right = nullptr;

				A->Balance = 0;
				B->Balance = 0;
				C->Balance = 0;

				return;
			}
		}
		// Rule #2
		if (TargetNode && TargetNode->Right && TargetNode->Right->Left)
		{
			if (TargetNode->Balance == 2 && TargetNode->Right->Balance == -1)
			{
				TBinaryTreeNode *A = TargetNode, *B = TargetNode->Right, *C = TargetNode->Right->Left;
				if (RootNode == A)
					RootNode = C;

				if (A->Parent)
				{
					if (A->Parent->Left == A)
						A->Parent->Left = C;
					else if (A->Parent->Right == A)
						A->Parent->Right = C;
				}

				

				C->Parent = A->Parent;
				B->Parent = C;
				A->Parent = C;

				C->Left = A;
				C->Right = B;

				A->Right = nullptr;
				B->Left = nullptr;

				A->Balance = 0;
				B->Balance = 0;
				C->Balance = 0;

				return;
			}
		}
		// Rule #3
		if (TargetNode && TargetNode->Right && TargetNode->Right->Right)
		{
			if (TargetNode->Balance == 2 && TargetNode->Right->Balance > 0)
			{
				TBinaryTreeNode *A = TargetNode, *B = TargetNode->Right, *C = TargetNode->Right->Right;
				if (RootNode == A)
					RootNode = B;

				if (A->Parent)
				{
					if (A->Parent->Left == A)
						A->Parent->Left = B;
					else if (A->Parent->Right == A)
						A->Parent->Right = B;
				}

				B->Left = A;
				B->Right = C;

				B->Parent = A->Parent;
				A->Parent = B;
				C->Parent = B;

				A->Right = nullptr;


				A->Balance = 0;
				B->Balance = 0;
				C->Balance = 0;

				return;
			}
		}
		// Rule #4
		if (TargetNode && TargetNode->Left && TargetNode->Left->Left)
		{
			if (TargetNode->Balance == -2 && TargetNode->Left->Balance < 0)
			{
				TBinaryTreeNode *A = TargetNode, *B = TargetNode->Left, *C = TargetNode->Left->Left;
				if (RootNode == A)
					RootNode = B;

				if (A->Parent)
				{
					if (A->Parent->Left == A)
						A->Parent->Left = B;
					else if (A->Parent->Right == A)
						A->Parent->Right = B;
				}


				B->Left = C;
				B->Right = A;

				B->Parent = A->Parent;
				A->Parent = B;
				C->Parent = B;

				A->Left = nullptr;

				A->Balance = 0;
				B->Balance = 0;
				C->Balance = 0;

				return;
			}
		}
		// Rule #5
		if (TargetNode && TargetNode->Right && TargetNode->Right->Left == nullptr && TargetNode->Left == nullptr)
		{
			TBinaryTreeNode *A = TargetNode, *B = TargetNode->Right;
			if (RootNode == A)
				RootNode = B;

			if (A->Parent)
			{
				if (A->Parent->Left == A)
					A->Parent->Left = B;
				else if (A->Parent->Right == A)
					A->Parent->Right = B;
			}

			B->Parent = A->Parent;
			A->Parent = B;

			B->Left = A;
			A->Right = nullptr;
			
			return;
		}
	};

public:

	~TBinaryTree()
	{
		Cleanup();
	};

	TBinaryTreeNode* Get(const KEY Key)
	{
		TBinaryTreeNode* TargetNode = RootNode;
		while (TargetNode != nullptr)
		{
			if (TargetNode->Key == Key)
			{
				return TargetNode;
			} else {
				if (TargetNode->Key > Key)
				{
					if (TargetNode->Left != nullptr)
					{
						TargetNode = TargetNode->Left;
						continue;
					}
					return nullptr;
				}
				if (TargetNode->Key < Key)
				{
					if (TargetNode->Right != nullptr)
					{
						TargetNode = TargetNode->Right;
						continue;
					}
					return nullptr;
				}
				break;
			}
		}
		return nullptr;
	};
	TBinaryTreeNode* Put(const KEY& Key, const VALUE& Value)
	{
		if (RootNode == nullptr)
		{
			RootNode = new TBinaryTreeNode(this, nullptr, Key, Value);
			return RootNode;
		} else {
			TBinaryTreeNode* TargetNode = RootNode;
			KEY TargetKey = Key;
			VALUE TargetValue = Value;

			while (TargetNode)
			{
				if (TargetNode->Key == TargetKey)
				{
					TargetNode->Value = TargetValue;
					break;
				}
				else if (TargetKey > TargetNode->Key) // right
                {
                    if (TargetNode->Right)
                    {
                        TargetNode = TargetNode->Right;
                        continue;
                    }
                    
                    return new TBinaryTreeNode(this, TargetNode, TargetKey, TargetValue);
                } else if(TargetKey < TargetNode->Key) // left
                {
                    if (TargetNode->Left)
                    {
                        TargetNode = TargetNode->Left;
                        continue;
                    }
                    return new TBinaryTreeNode(this, TargetNode, TargetKey, TargetValue);
                }
			}
		}
		return nullptr;
	};
	void Remove(const KEY& Key)
	{
		TBinaryTreeNode* TargetNode = RootNode;
		while (TargetNode != nullptr)
		{
			if (TargetNode->Key == Key)
			{
				if (TargetNode->Left == nullptr && TargetNode->Right == nullptr) // Node was leaf
					delete TargetNode;
				else if( // Node was branch with single subbranch
					(TargetNode->Left == nullptr && TargetNode->Right) ||
					(TargetNode->Right == nullptr && TargetNode->Left)
					)
				{
					TBinaryTreeNode* NotEmptyNode = (TargetNode->Right ? TargetNode->Right : TargetNode->Left);
					NotEmptyNode->Parent = TargetNode->Parent;
					delete TargetNode;
					if (NotEmptyNode->Parent)
					{
						if (NotEmptyNode->Parent->Left == TargetNode)
						{
							NotEmptyNode->Parent->Left = NotEmptyNode;
						}
						else if (NotEmptyNode->Parent->Right == TargetNode)
						{
							NotEmptyNode->Parent->Right = NotEmptyNode;
						}
					}
				} else if(TargetNode->Left && TargetNode->Right) // Node has both left and right side branches
				{

					TBinaryTreeNode* Right = TargetNode->Right;
					TBinaryTreeNode* Left = TargetNode->Left;

					TBinaryTreeNode* TargetParent = TargetNode->Parent;
					int TargetNodeBalance = TargetNode->Balance;

					if (RootNode == TargetNode)
						RootNode = Right;

					delete TargetNode;

					Right->Parent = TargetParent;
					if (Right->Parent)
					{
						if (Right->Parent->Left == TargetNode)
						{
							Right->Parent->Left = Right;
						}
						else if (Right->Parent->Right == TargetNode)
						{
							Right->Parent->Right = Right;
						}
					}


					while (Right->Left != nullptr)
						Right = Right->Left;

					Right->Left = Left;
					Left->Parent = Right;
					Right->Balance += (Left->Balance + TargetNodeBalance) - 1;

				}
				return;
			}
			else {
				if (TargetNode->Key > Key)
				{
					if (TargetNode->Left != nullptr)
					{
						TargetNode = TargetNode->Left;
						continue;
					}
					return;
				}
				if (TargetNode->Key < Key)
				{
					if (TargetNode->Right != nullptr)
					{
						TargetNode = TargetNode->Right;
						continue;
					}
					return;
				}
				break;
			}
		}
	};
	void Cleanup()
	{
		TBinaryTreeNode* TargetNode = RootNode;
		while (TargetNode)
		{
			if (TargetNode->Left)
			{
				TargetNode = TargetNode->Left;
				continue;
			}
			if (TargetNode->Right)
			{
				TargetNode = TargetNode->Right;
				continue;
			}
			TBinaryTreeNode* Parent = TargetNode->Parent;
			delete TargetNode;
			TargetNode = Parent;
		}
		RootNode = nullptr;
	}

	VALUE& operator[](const KEY& Key)
	{
		TBinaryTreeNode* Result = Get(Key);
		if (Result == nullptr)
			Result = Put(Key, VALUE());
		return Result->Value;
	}

	TBinaryTreeNode* GetLeftMost()
	{
		if (RootNode == nullptr)
			return nullptr;
		TBinaryTreeNode* LeftMost = RootNode;
		while (LeftMost->LeftNeighbour)
			LeftMost = LeftMost->LeftNeighbour;
		return LeftMost;
	}
};