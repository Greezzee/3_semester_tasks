#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <sstream>

bool IsPunct(char c) {
	return std::ispunct(c) || std::isspace(c) || std::isdigit(c);
}

bool CompareFreq(std::pair<std::string, size_t> a, std::pair<std::string, size_t> b) {
	return a.second > b.second;
}

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "Russian");

	if (argc != 3) {
		std::cout << "Please, enter filename and words count\n";
		return -1;
	}

	std::fstream file(argv[1]);
	if (file.fail()) {
		std::cout << "Can't open " << argv[1] << "\n";
		return -1;
	}

	size_t word_count = std::stoi(argv[2]);

	std::string text((std::istreambuf_iterator<char>(file)),
		             (std::istreambuf_iterator<char>()));

	std::replace_if(text.begin(), text.end(), IsPunct, ' ');
	std::transform(text.begin(), text.end(), text.begin(), tolower);

	std::map<std::string, size_t> words_freq;

	std::istringstream ss(text);
	std::string word;
	while (ss >> word) {
		words_freq[word]++;
	}

	std::vector<std::pair<std::string, size_t>> top_freq_words;

	for (std::pair<std::string, size_t> i : words_freq) {
		top_freq_words.push_back(i);
	}

	std::sort(top_freq_words.begin(), top_freq_words.end(), CompareFreq);

	for (size_t i = 0; i < std::min(word_count, top_freq_words.size()); i++) {
		std::cout << i + 1 << ": " << top_freq_words[i].first << " - " << top_freq_words[i].second << "\n";
	}

	return 0;
}