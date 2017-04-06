#include <vector>
#include <iostream>
#include <cassert>
#include "veb.h"

template<unsigned int S>
class VEBTree : AbstractVEBTree<S>
{
private:
	VEBTree<(S >> 1)>* summary;
	VEBTree<(S >> 1)>* clusters[1ULL << (S >> 1)];
	unsigned long long min, max;

	inline unsigned long long high(unsigned long long x) const
	{
		return x >> (S >> 1);
	}

	inline unsigned long long low(unsigned long long x) const
	{
		return x & ((1ULL << (S >> 1)) - 1ULL);
	}

public:
	VEBTree() : summary(NULL), min(NO), max(NO) 
	{
		memset(clusters, 0, sizeof(clusters));
	}

	virtual ~VEBTree()
	{
		delete summary;
		for (unsigned long long i = 0; i < (1ULL << (S >> 1)); i++)
		{
			delete clusters[i];
		}
	}

	
	void add(unsigned long long x)
	{
		if (min == NO)
		{
			min = max = x;
			return;
		}
		if (min > x)
		{
			std::swap(min, x);
		}
		if (x > max)
		{
			max = x;
		}
		if (S != 1)
		{
			if (clusters[high(x)] == NULL)
			{
				clusters[high(x)] = new VEBTree<(S >> 1)>();
			}
			if (clusters[high(x)]->getMin() == NO)
			{
				if (summary == NULL)
				{
					summary = new VEBTree<(S >> 1)>();
				}
				summary->add(high(x));
			}
			clusters[high(x)]->add(low(x));
		}
	}

	
	void remove(unsigned long long x)
	{
		
		if (min == NO)
			return;
		if (min == x && min == max)
		{
			min = max = NO;
			return;
		}
		if (S != 1)
		{
			if (x == min && clusters[summary->getMin()] != NULL && clusters[summary->getMin()]->getMin() != NO)
			{
				x = clusters[summary->getMin()]->getMin() + ((summary->getMin()) << (S >> 1));
				min = x;
			}
			clusters[high(x)]->remove(low(x));
			if (clusters[high(x)]->getMin() == NO)
			{
				summary->remove(high(x));
			}
			if (max == x)
			{
				if (summary->getMin() != NO && clusters[summary->getMax()] != NULL && clusters[summary->getMax()]->getMax() != NO)
				{
					max = clusters[summary->getMax()]->getMax() + ((summary->getMax()) << (S >> 1));
				}
				else
				{
					max = min;
				}
			}
		}

	}
	
	
	unsigned long long next(unsigned long long x) const 
	{
		if (x < min)
			return min;
		if (min == NO || x >= max)
			return NO;
		if (S != 1)
		{
			if (clusters[high(x)] != NULL && clusters[high(x)]->getMax() > low(x))
			{
				return clusters[high(x)]->next(low(x)) + (high(x) << (S >> 1));
			}
			else if (summary != NULL)
			{
				unsigned long long high_next = summary->next(high(x));
				if (high_next != NO)
					return (high_next << (S >> 1)) + clusters[high_next]->getMin();
			}
		}
		return NO;
	}

	unsigned long long prev(unsigned long long x) const 
	{
		if (x > max)
			return max;
		if (min == NO || x <= min)
			return NO;
		if (S != 1)
		{
			if (clusters[high(x)] != NULL && (clusters[high(x)]->getMin()) < low(x))
			{
				return clusters[high(x)]->prev(low(x)) + (high(x) << (S >> 1));
			}
			else if (summary != NULL)
			{
				unsigned long long high_prev = summary->prev(high(x));
				if (high_prev != NO)
					return (high_prev << (S >> 1)) + clusters[high_prev]->getMax();
			}
		}
		return NO;
	}

	unsigned long long getMin() const
	{
		return min;
	}

	unsigned long long getMax() const 
	{
		return max;
	}
};

/*template<unsigned int S>
class VEBTree : AbstractVEBTree<S> 
{
private:
	std::vector<bool> f;

public:
	VEBTree() : f(1ULL << S) {	}

	void add(unsigned long long x)
	{
		f[x] = true;
	}

	void remove(unsigned long long x) 
	{
		f[x] = false;
	}

	unsigned long long next(unsigned long long x) const 
	{
		++x;
		while (x + 1 < (1ULL << S) && !f[x])
		{
			++x;
		}
		return (f[x] ? x : NO);
	}

	unsigned long long prev(unsigned long long x) const 
	{
		if (x == 0) return -1;
		while (x > 0 && !f[x])
		{
			--x;
		}
		return f[x] ? x : NO;
	}

	unsigned long long getMin() const
	{
		unsigned long long x = 0;
		while (x + 1 < (1ULL << S) && !f[x]) 
		{
			++x;
		}
		return f[x] ? x : NO;
	}

	unsigned long long getMax() const
	{
		unsigned long long x = (1ULL << S) - 1;
		while (x > 0 && !f[x]) 
		{
			--x;
		}
		return f[x] ? x : NO;
	}
};
*/


int main() 
{
	VEBTree<20> tree;
	tree.add(5);
	tree.add(11);
	tree.add(10);

	assert(tree.next(5) == 10);
	std::cout << tree.next(5) << std::endl;

	tree.remove(10);
	
	assert(tree.next(5) == 11);
	std::cout << tree.next(5) << std::endl;
	tree.remove(11);

	assert(tree.prev(12) == 5);
	std::cout << tree.prev(12) << std::endl;
}