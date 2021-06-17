/*
 * Hashmap.h
 *
 *  Created on: Jun 16, 2021
 *      Author: Ahmed Nasr
 */

#ifndef HASHMAP_H_
#define HASHMAP_H_

#include <string>

template<typename K, typename V>
class MapNode
{
	template<typename Y, typename Z>
	friend class mymap;

private:
    K key;
    V value;
    MapNode<K, V>* next;

    MapNode(const K &key, const V &value) : key{key}, value{value}, next{nullptr}
    {

    }

    ~MapNode()
    {
    	delete next;
    }
};

template<typename T>
class hash
{
	template<typename Y, typename Z>
	friend class mymap;

	static unsigned int hashFunction(const T &key, unsigned int bucketSize)
	{
		return static_cast<unsigned int>(key) % bucketSize;
	}
};

template<>
class hash<std::string>
{
	template<typename Y, typename Z>
	friend class mymap;

	static unsigned int hashFunction(const std::string &key, unsigned int bucketSize)
	{
		unsigned int hashcode = 0, base = 1;
		const unsigned int p = 37;

		for(int i = key.size() - 1; i >= 0; i--)
		{
			hashcode += (key[i] * base) % bucketSize;
			base = (base * p) % bucketSize;
		}

		return hashcode % bucketSize;
	}
};

template<typename K, typename V>
class mymap
{
private:
	unsigned int count;
	unsigned int numBuckets;
	MapNode<K, V>** buckets;
	bool checkLoadFactor;

	unsigned int getBucketIndex(const K &key) const
	{
		return hash<K>::hashFunction(key, numBuckets);
	}

public:
	mymap() : count{0}, numBuckets{5}, buckets{new MapNode<K, V>*[numBuckets]}, checkLoadFactor{true}
	{
		for(unsigned int i = 0; i < numBuckets; i++)
			buckets[i] = nullptr;
	}

	~mymap()
	{
		for(unsigned int i = 0; i < numBuckets; i++)
			delete buckets[i];

		delete[] buckets;
	}

	V &operator[](const K &key)
	{
		if(!isExist(key))
		{
			V temp;
			insert(key, temp);
		}

		V *value;

		for(MapNode<K, V>* head = buckets[getBucketIndex(key)]; head != nullptr; head = head->next)
			if(head->key == key)
			{
				value = &head->value;
				break;
			}

		return *value;
	}

	void insert(const K &key, const V &value)
	{
		unsigned int bucketIndex = getBucketIndex(key);
		MapNode<K, V>* head = buckets[bucketIndex];

		while(head != nullptr)
		{
			if(head->key == key)
			{
				head->value = value;
				return;
			}

			head = head->next;
		}

		MapNode<K, V>* node = new MapNode<K, V>(key, value);
		node->next = buckets[bucketIndex];
		buckets[bucketIndex] = node;

		count++;

		if(checkLoadFactor)
		{
			const double maxLoadFactor = 0.7;

			if(getLoadFactor() >= maxLoadFactor)
			{
				checkLoadFactor = false;
				rehash();
			}
		}
	}

	V remove(const K &key)
	{
		unsigned int bucketIndex = getBucketIndex(key);

		for(MapNode<K, V>* head = buckets[bucketIndex], *prev = nullptr; head != nullptr; prev = head, head = head->next)
		{
			if(head->key == key)
			{
				if(prev != nullptr)
					prev->next = head->next;
				else
					buckets[bucketIndex] = head->next;

				V currentValue = head->value;

				head->next = nullptr;
				delete head;

				count--;

				return currentValue;
			}
		}

		return 0;
	}

	V getValue(const K &key) const
	{
		for(MapNode<K, V>* head = buckets[getBucketIndex(key)]; head != nullptr; head = head->next)
			if(head->key == key)
				return head->value;

		try
		{
			return 0;
		}
		catch(...)
		{
			V temp;
			return temp;
		}
	}

	unsigned int size() const
	{
		return count;
	}

	double getLoadFactor() const
	{
		return static_cast<double>(count) / numBuckets;
	}

	void rehash()
	{
		//old version of buckets
		MapNode<K, V>** tempBuckets = buckets;
		unsigned int oldNumBuckets = numBuckets;

		//update bucket size
		numBuckets *= 2;
		buckets = new MapNode<K, V>*[numBuckets];

		//initialize new buckets to nullptr
		for(unsigned int i = 0; i < numBuckets; i++)
			buckets[i] = nullptr;

		//reset count
		count = 0;

		for(unsigned int i = 0; i < oldNumBuckets; i++)
		{
			//rehash all key-value pairs
			for(MapNode<K, V>* head = tempBuckets[i]; head != nullptr; head = head->next)
				insert(head->key, head->value);

			//delete linked list at oldBucket[i]
			delete tempBuckets[i];
		}

		//delete old buckets
		delete[] tempBuckets;

		checkLoadFactor = true;
	}

	bool isExist(const K &key)
	{
		for(MapNode<K, V>* head = buckets[getBucketIndex(key)]; head != nullptr; head = head->next)
			if(head->key == key)
				return true;

		return false;
	}
};

#endif /* HASHMAP_H_ */
