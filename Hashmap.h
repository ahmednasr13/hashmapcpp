/*
 * Hashmap.h
 *
 *  Created on: Jun 16, 2021
 *      Author: Ahmed Nasr
 */

#ifndef HASHMAP_H_
#define HASHMAP_H_

#include <string>
#include <vector>

template<typename K, typename V>
class MapNode
{
	template<typename Y, typename Z>
	friend class mymaptemplate;
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

template<typename K>
class hash
{
	template<typename Y, typename Z>
	friend class mymaptemplate;
	template<typename Y, typename Z>
	friend class mymap;

	static unsigned int hashFunction(const K &key, unsigned int bucketSize)
	{
		return static_cast<unsigned int>(key) % bucketSize;
	}
};

template<>
class hash<std::string>
{
	template<typename Y, typename Z>
	friend class mymaptemplate;
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
class mymaptemplate
{
protected:
	unsigned int count;
	unsigned int numBuckets;
	MapNode<K, V>** buckets;
	bool checkLoadFactor;
	double maxLoadFactor;

	unsigned int getBucketIndex(const K &key) const
	{
		return hash<K>::hashFunction(key, numBuckets);
	}

public:
	mymaptemplate(unsigned int numbuckets, double maxLoadFactor) : count{0}, numBuckets{numbuckets}, buckets{new MapNode<K, V>*[numBuckets]}, checkLoadFactor{true}, maxLoadFactor{maxLoadFactor}
	{
		for(unsigned int i = 0; i < numBuckets; i++)
			buckets[i] = nullptr;
	}

	virtual ~mymaptemplate()
	{
		for(unsigned int i = 0; i < numBuckets; i++)
			delete buckets[i];

		delete[] buckets;
	}

	virtual V &operator[](const K &key) = 0;

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
			if(getLoadFactor() > maxLoadFactor)
			{
				checkLoadFactor = false;
				rehash();
			}
		}
	}

	virtual V remove(const K &key) = 0;

	virtual V getValue(const K &key) const = 0;

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

	std::vector<K> getAllKeys() const
	{
		std::vector<K> keys;

		for(unsigned int i = 0; i < numBuckets; i++)
			for(MapNode<K, V>* head = buckets[i]; head != nullptr; head = head->next)
				keys.push_back(head->key);

		return keys;
	}

	unsigned int getBucketsCount() const
	{
		return numBuckets;
	}

	void setMaxLoadFactor(double maxLoadFactor)
	{
		this->maxLoadFactor = maxLoadFactor;
	}
};

template<typename K, typename V>
class mymap : public mymaptemplate<K, V>
{
public:
	mymap(unsigned int numBuckets = 5, double maxLoadFactor = 0.7) : mymaptemplate<K, V>(numBuckets, maxLoadFactor)
	{

	}
	virtual ~mymap()
	{

	}

	virtual V &operator[](const K &key)
	{
		if(!mymaptemplate<K, V>::isExist(key))
			mymaptemplate<K, V>::insert(key, 0);

		V *value;

		for(MapNode<K, V>* head = mymaptemplate<K, V>::buckets[mymaptemplate<K, V>::getBucketIndex(key)]; head != nullptr; head = head->next)
			if(head->key == key)
			{
				value = &head->value;
				break;
			}

		//it's impossible to be not found as the element is already inserted, so it is impossible to dereference value variable without initializing it
		return *value;
	}

	virtual V remove(const K &key)
		{
			unsigned int bucketIndex = mymaptemplate<K, V>::getBucketIndex(key);

			for(MapNode<K, V>* head = mymaptemplate<K, V>::buckets[bucketIndex], *prev = nullptr; head != nullptr; prev = head, head = head->next)
			{
				if(head->key == key)
				{
					if(prev != nullptr)
						prev->next = head->next;
					else
						mymaptemplate<K, V>::buckets[bucketIndex] = head->next;

					V currentValue = head->value;

					head->next = nullptr;
					delete head;

					mymaptemplate<K, V>::count--;

					return currentValue;
				}
			}

			return 0;
		}

	virtual V getValue(const K &key) const
	{
		for(MapNode<K, V>* head = mymaptemplate<K, V>::buckets[mymaptemplate<K, V>::getBucketIndex(key)]; head != nullptr; head = head->next)
			if(head->key == key)
				return head->value;

		return 0;
	}

	std::pair<K, V> max_element() const
	{
		std::pair<K, V> max;

		if(mymaptemplate<K, V>::count == 0)
			return max;

		bool foundFirstElement = false;

		for(unsigned int i = 0; i < mymaptemplate<K, V>::numBuckets; i++)
			for(MapNode<K, V>* head = mymaptemplate<K, V>::buckets[i]; head != nullptr; head = head->next)
				if(!foundFirstElement || head->value > max.second)
				{
					max.first = head->key;
					max.second = head->value;

					foundFirstElement = true;
				}

		return max;
	}

	std::pair<K, V> min_element() const
	{
		std::pair<K, V> min;

		if(mymaptemplate<K, V>::count == 0)
			return min;

		bool foundFirstElement = false;

		for(unsigned int i = 0; i < mymaptemplate<K, V>::numBuckets; i++)
			for(MapNode<K, V>* head = mymaptemplate<K, V>::buckets[i]; head != nullptr; head = head->next)
				if(!foundFirstElement || head->value < min.second)
				{
					min.first = head->key;
					min.second = head->value;

					foundFirstElement = true;
				}

		return min;
	}
};

template<typename K>
class mymap<K, std::string> : public mymaptemplate<K, std::string>
{
public:
	mymap(unsigned int numbuckets = 5, double maxLoadFactor = 0.7) : mymaptemplate<K, std::string>(numbuckets, maxLoadFactor)
	{

	}
	virtual ~mymap()
	{

	}

	virtual std::string &operator[](const K &key)
	{
		if(!mymaptemplate<K, std::string>::isExist(key))
			mymaptemplate<K, std::string>::insert(key, "");

		std::string *value;

		for(MapNode<K, std::string>* head = mymaptemplate<K, std::string>::buckets[mymaptemplate<K, std::string>::getBucketIndex(key)]; head != nullptr; head = head->next)
			if(head->key == key)
			{
				value = &head->value;
				break;
			}

		//it's impossible to be not found as the element is already inserted, so it is impossible to dereference value variable without initializing it
		return *value;
	}

	virtual std::string remove(const K &key)
	{
		unsigned int bucketIndex = mymaptemplate<K, std::string>::getBucketIndex(key);

		for(MapNode<K, std::string>* head = mymaptemplate<K, std::string>::buckets[bucketIndex], *prev = nullptr; head != nullptr; prev = head, head = head->next)
		{
			if(head->key == key)
			{
				if(prev != nullptr)
					prev->next = head->next;
				else
					mymaptemplate<K, std::string>::buckets[bucketIndex] = head->next;

				std::string currentValue = head->value;

				head->next = nullptr;
				delete head;

				mymaptemplate<K, std::string>::count--;

				return currentValue;
			}
		}

		return "";
	}

	virtual std::string getValue(const K &key) const
	{
		for(MapNode<K, std::string>* head = mymaptemplate<K, std::string>::buckets[mymaptemplate<K, std::string>::getBucketIndex(key)]; head != nullptr; head = head->next)
			if(head->key == key)
				return head->value;

		return "";
	}
};

#endif /* HASHMAP_H_ */
