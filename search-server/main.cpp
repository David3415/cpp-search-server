

#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <cmath>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

string ReadLine() {
	string s;
	getline(cin, s);
	return s;
}

int ReadLineWithNumber() {
	int result = 0;
	cin >> result;
	ReadLine();
	return result;
}

vector<string> SplitIntoWords(const string& text) {
	vector<string> words;
	string word;
	for (const char c : text) {
		if (c == ' ') {
			if (!word.empty()) {
				words.push_back(word);
				word.clear();
			}
		}
		else {
			word += c;
		}
	}
	if (!word.empty()) {
		words.push_back(word);
	}

	return words;
}

struct Document {
	int id;
	double relevance;
};


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class SearchServer {
public:

	void SetStopWords(const string& text) {
		for (const string& word : SplitIntoWords(text)) {
			stop_words_.insert(word);
		}
	}

	void AddDocument(int document_id, const string& document) {
		const vector<string> words = SplitIntoWordsNoStop(document);
		document_count_++;
		double koef = 1.0 / words.size();
		for (const string& word : words) {
			word_to_document_freqs_[word][document_id] += koef;
		}


	}

	vector<Document> FindTopDocuments(const string& raw_query) const {
		const Query request = ParseQuery(raw_query);
		auto matched_documents = FindAllDocuments(request);

		sort(matched_documents.begin(), matched_documents.end(),
			[](const Document& lhs, const Document& rhs) {
				return lhs.relevance > rhs.relevance;
			});
		if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
			matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
		}
		return matched_documents;
	}

private:
	int document_count_ = 0;
	struct Query
	{
		set <string> minusWords;
		set <string> plusWords;// слова запроса без стоп слов
	};

	map<string, map<int, double>> word_to_document_freqs_;//  ПЕРЕДЕЛАННАЯ СТРУКТУРА, КОТОРУЮ ЗАПОЛНЯЕМ ПРИ ВВВОДЕ--------

	set<string> stop_words_;

	bool IsStopWord(const string& word) const {
		return stop_words_.count(word) > 0;
	}

	vector<string> SplitIntoWordsNoStop(const string& text) const {
		vector<string> words;
		for (const string& word : SplitIntoWords(text)) {
			if (!IsStopWord(word)) {
				words.push_back(word);
			}

		}
		return words;
	}

	Query ParseQuery(const string& text) const {
		Query request;
		for (const string& word : SplitIntoWordsNoStop(text)) {
			if (word[0] == '-')
				request.minusWords.insert(word.substr(1));
			else
				request.plusWords.insert(word);
		}
		return request;
	}

	/// / ЗДЕСЬ ИЩЕМ СЛОВАЗДЕСЬ ИЩЕМ СЛОВАЗДЕСЬ ИЩЕМ СЛОВАЗДЕСЬ ИЩЕМ СЛОВА!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  

	vector<Document> FindAllDocuments(const Query& query_words) const {
		vector<Document> matched_documents;
		map<int, double> document_to_relevance;


		//double invWordFreq = 0.0;
		for (const string& plusWord : query_words.plusWords)
		{
			if (word_to_document_freqs_.count(plusWord) == 0) {
				continue;
			}


		
		const double invWordFreq=log(document_count_ * 1.0 / word_to_document_freqs_.at(plusWord).size());
		if (word_to_document_freqs_.find(plusWord) != word_to_document_freqs_.end())


			for (const auto& [id, wordFr] : word_to_document_freqs_.at(plusWord))
			{
				document_to_relevance[id] += wordFr * invWordFreq;

			}

	}

	vector<int> delNum;
	for (const auto& minWord : query_words.minusWords)
	{
		if (word_to_document_freqs_.count(minWord) == 0) {
			continue;
		}
		for (const auto& [id, wordFr] : word_to_document_freqs_.at(minWord))
		{
			delNum.push_back(id);
		}
	}

	for (const auto& num : delNum) {
		document_to_relevance.erase(num);
	}

	for (const auto& document : document_to_relevance) {

		matched_documents.push_back({ document.first, document.second });
	}
	return matched_documents;
}
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%





SearchServer CreateSearchServer() {
	SearchServer search_server;
	search_server.SetStopWords(ReadLine());

	const int document_count = ReadLineWithNumber();
	for (int document_id = 0; document_id < document_count; document_id++) {
		search_server.AddDocument(document_id, ReadLine());
	}

	return search_server;
}

int main() {
	const SearchServer search_server = CreateSearchServer();


	const string query = ReadLine();
	for (const auto& s : search_server.FindTopDocuments(query)) {
		cout << "{ document_id = "s << s.id << ", "
			<< "relevance = "s << s.relevance << " }"s << endl;
	}
}
