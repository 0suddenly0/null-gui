#pragma once
#include <vector>

template <typename T>
class flags_list {
public:
	flags_list() { }
	flags_list(T flag) : list(std::vector<T>{ flag }) { }
	flags_list(std::vector<T> flags) : list(flags) { }
	flags_list(std::initializer_list<T> flags) : list(flags) { }
	~flags_list() { list.clear(); }

	int flag_id(T flag) {
		if (count(flag) == 0) return 0;
		return std::distance(list.begin(), std::find_if(list.begin(), list.end(), [=](T current_flag) { return (int)current_flag == (int)flag; }));
	}

	int count(T flag) { return std::count(list.begin(), list.end(), flag); }
	bool contains(T flag) { return count(flag) > 0; }
	bool empty() { return list.size() == 0; }

	void add(T flag) { if (count(flag) != 0) return; list.push_back(flag); }
	void add(std::vector<T> flags) { for (T flag : flags) add(flag); }

	void remove(T flag) { if (count(flag) == 0) return; remove(flag_id(flag)); }
	void remove(std::vector<T> flags) { for (T flag : flags) remove(flag); }
	void remove(int id) { list.erase(list.begin() + id); }

	operator std::vector<T>() { return list; }
	operator std::initializer_list<T>() { return list; }

	flags_list<T>& operator+(std::vector<T> src) { list.insert(src); return *this; }
	flags_list<T>& operator+(std::initializer_list<T> src) { list.insert(src); return *this; }
	flags_list<T>& operator+(const flags_list<T>& src) { list.insert(src.list); return *this; }

	flags_list<T>& operator=(std::vector<T> src) { list = src; return *this; }
	flags_list<T>& operator=(std::initializer_list<T> src) { list = src; return *this; }
	flags_list<T>& operator=(const flags_list<T>& src) { list = src.list; return *this; }

	flags_list<T>& operator+=(std::vector<T> src) { list.insert(src); return *this; }
	flags_list<T>& operator+=(std::initializer_list<T> src) { list.insert(src); return *this; }
	flags_list<T>& operator+=(const flags_list<T>& src) { list.insert(src.list); return *this; }

	std::vector<T> list;
};