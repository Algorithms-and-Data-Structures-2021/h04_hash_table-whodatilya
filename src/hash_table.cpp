#include "hash_table.hpp"
#include <stdexcept>
namespace itis
{

  int HashTable::hash(int key) const
  {
    return utils::hash(key, static_cast<int>(buckets_.size()));
  }

  HashTable::HashTable(int capacity, double load_factor) : load_factor_{load_factor}
  {
    if (capacity <= 0)
    {
      throw std::logic_error("hash table capacity must be greater than zero");
    }

    if (load_factor <= 0.0 || load_factor > 1.0)
    {
      throw std::logic_error("hash table load factor must be in range [0...1]");
    }
    buckets_.resize(capacity);
  }

  std::optional<std::string> HashTable::Search(int key) const
  {
    int index = hash(key);
    Bucket bucket = buckets_[index];

    for (const auto &pair : bucket)
    {
      if (pair.first == key)
      {
        return pair.second;
      }
    }
    return std::nullopt;
  }

  void HashTable::Put(int key, const std::string &value)
  {
    int index = hash(key);

    if (buckets_[index].empty())
    {
      num_keys_++;
      auto pair = std::pair<int, std::string>();
      pair.first = key;
      pair.second = value;
      buckets_[index].push_back(pair);

    }
    else
        {
      auto pair = std::pair<int, std::string>();
      pair.first = key;
      pair.second = value;
      buckets_[index].push_back(pair);
        }

    if (static_cast<double>(num_keys_) / buckets_.size() >= load_factor_)
    {
      utils::hash(key, buckets_.size());
      buckets_.resize(buckets_.capacity() * HashTable::kGrowthCoefficient);
    }
  }


  std::optional<std::string> HashTable::Remove(int key)
  {
    int index = hash(key);
    std::pair<int, std::string> toRemove;

    for (const auto &pair : buckets_[index])
    {

      if (pair.first == key)
      {
        toRemove = pair;
        buckets_[index].remove(pair);
        return toRemove.second;
      }
      return std::nullopt;
    }
  }

    bool HashTable::ContainsKey(int key) const
    {
      return Search(key).has_value();
    }

    bool HashTable::empty() const
    {
      return size() == 0;
    }

    int HashTable::size() const
    {
      return num_keys_;
    }

    int HashTable::capacity() const
    {
      return static_cast<int>(buckets_.size());
    }

    double HashTable::load_factor() const
    {
      return load_factor_;
    }

    std::unordered_set<int> HashTable::keys() const
    {
      std::unordered_set<int> keys(num_keys_);
      for (const auto &bucket : buckets_)
      {
        for (const auto &[key, _] : bucket)
        {
          keys.insert(key);
        }
      }
      return keys;
    }

    std::vector<std::string> HashTable::values() const
    {
      std::vector<std::string> values;
      for (const auto &bucket : buckets_)
      {
        for (const auto &[_, value] : bucket)
        {
          values.push_back(value);
        }
      }
      return values;
    }

  }
