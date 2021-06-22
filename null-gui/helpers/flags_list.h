#pragma once
#include <vector>

template <typename T>
class flags_list {
public:
	flags_list() { }
	flags_list(T flag) : list(std::vector<T>{ flag }) { }
	flags_list(std::vector<T> flags) : list(flags) { }
	~flags_list() { list.clear(); }

	int count(T flag) { return std::count(list.begin(), list.end(), flag); }
	bool contains(T flag) { return count(flag) > 0; }
	bool empty() { return list.size() == 0; }

	int flag_id(T flag) {
		if (count(flag) == 0) return 0;
		return std::distance(list.begin(), std::find_if(list.begin(), list.end(), [=](T current_flag) { return (int)current_flag == (int)flag; }));
	}

	void add(T flag) {
		if (count(flag) != 0) return;
		list.push_back(flag);
	}

	void add(std::vector<T> flags) {
		for (T flag : flags)
			add(flag);
	}

	void remove(T flag) {
		if (count(flag) == 0) return;
		remove(flag_id(flag));
	}

	void remove(std::vector<T> flags) {
		for (T flag : flags)
			remove(flag);
	}

private:
	void remove(int id) { list.erase(list.begin() + id); }

	std::vector<T> list;
};