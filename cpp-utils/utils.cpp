#include <string>

std::string to_upper(std::string s) {
	for (int i = 0; i < s.size(); i++) {
		if (s[i] >= 'a' && s[i] <= 'z') {
			s[i] += 'A' - 'a';
		}
	}
	return s;
}