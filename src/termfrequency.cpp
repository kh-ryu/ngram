#include "termfrequency.h"

TermFrequency::TermFrequency(const vector<Verse> &verse) {
    verses = verse;
    analyzer = Analyzer(verse);
}

pair<vector<string>, vector<string>> TermFrequency::findTerms(const int &wordnum) {

    auto bookLengthList = getBookLengthList();
    auto wordRSDList = getWordWithRSDList(bookLengthList);

    vector<string> unevenWords;
    cout << wordnum << " unevenly distributed words/phrases" << endl;
    for (int i = 0; i < wordnum; i++) {
        auto it = max_element(wordRSDList.begin(), wordRSDList.end(), cmp);
        cout << "word: " << it->first << "    RSD: " << it->second << endl;
        unevenWords.push_back(it->first);
        wordRSDList.erase(it);
    }

    vector<string> evenWords;
    cout << wordnum << " evenly distributed words/phrases" << endl;
    for (int i = 0; i < wordnum; i++) {
        auto it = min_element(wordRSDList.begin(), wordRSDList.end(), cmp);
        cout << "word: " << it->first << "    RSD: " << it->second << endl;
        evenWords.push_back(it->first);
        wordRSDList.erase(it);
    }

    return {unevenWords, evenWords};
}

vector<pair<string, double>> TermFrequency::getWordWithRSDList
(const vector<Triad> &bookLengthList) {
    vector<pair<string, double>> wordRSDList;
	Trie allWords;
	
    for (auto it = bookLengthList.begin(); it != bookLengthList.end(); ++it) {
        cout << "Book: " << get<0>(*it) << endl;
        cout << "Chapter: " << get<1>(*it) << endl; 
        cout << "Verse: " << get<2>(*it) << endl;

        string bookName = get<0>(*it);
        int endChapter = get<1>(*it);
        int endVerse = get<2>(*it);

        Triad start = {bookName, 1, 1};
        Triad end = {bookName, endChapter, endVerse};

        string content = normalize(analyzer.mergeVerses(start, end));

        vector<string> words = getWordsAndPhrases(content);
        vector<string> newWordsInBook;
        for (auto word : words) {
            if (!allWords.search(word)) { // if word does not exist in trie
                newWordsInBook.push_back(word );
                allWords.insert(word);
            }
        }
        auto wordRSDListofBook = getFreqRSD(newWordsInBook, bookLengthList);
        wordRSDList.insert(wordRSDList.end(), wordRSDListofBook.begin(), wordRSDListofBook.end());
    }

    return wordRSDList;
}

vector<pair<string, double>> TermFrequency::getFreqRSD(const vector<string> &words, const vector<Triad> &BookLengthList) {
    map<string, vector<double>> wordFrequencies;

    for (auto it = BookLengthList.begin(); it != BookLengthList.end(); ++it){
        string bookName = get<0>(*it);
        int endChapter = get<1>(*it);
        int endVerse = get<2>(*it);

        Triad start = {bookName, 1, 1};
        Triad end = {bookName, endChapter, endVerse};

        auto freq = analyzer.evaluateFrequency(words, start, end);

        for (auto it : freq) {
            auto str = it.first;
            auto frequency = it.second;
            auto iter = wordFrequencies.find(str);
            if (iter == wordFrequencies.end()) {  // cannot find str
                vector<double> temp;
                temp.push_back(frequency);
                wordFrequencies.insert({str, temp});
            }
            else {
                auto temp = wordFrequencies.at(str);
                temp.push_back(frequency);
                wordFrequencies.at(str) = temp;
            }
        }
    }
    if (wordFrequencies.size() != words.size()){
        cout << "something missed during function TermFrequency::getFreqRSD" << endl;
        exit(100);
    }

    int totalWords = (int)wordFrequencies.size();
    int totalBooks = (int)BookLengthList.size();

    vector<pair<string, double>> wordRSDList;
    for (auto it = wordFrequencies.begin(); it != wordFrequencies.end(); ++it){
        double sum = 0.0;
        auto freqPerBook = it->second;
        for (auto j = freqPerBook.begin(); j != freqPerBook.end(); ++j)
            sum += (*j);
        auto Mean = sum / totalBooks;
        
        if (Mean > 1.0) {
            double Var = 0.0;
            for (auto j = freqPerBook.begin(); j != freqPerBook.end(); ++j) {
                Var += ((*j) - Mean)*((*j) - Mean);
            }
            Var = Var / totalBooks;
            double RSD = sqrt(Var) / Mean;
            wordRSDList.push_back({it->first, RSD});
        }
    }

    return wordRSDList;
}

vector<string> TermFrequency::getWordsAndPhrases(const string &content) {
    vector<string> contentWords = divideIntoWords(content);
    vector<string> wordAndPhrases;
    string word;

    // consider phrases with maximum 2 words
    for (auto it = contentWords.begin(); it != contentWords.end(); ++it) {
        word = normalize(*it);
        wordAndPhrases.push_back(*it);
        if (it != contentWords.end()-1) {
            word = normalize(*it + " " + *(it+1));
            wordAndPhrases.push_back(word);
        }
    }
    return wordAndPhrases;
}

vector<string> TermFrequency::divideIntoWords(const string &content) {
    string word;
    vector<string> verseInWords;

    stringstream ss(content);

    while(getline(ss, word, ' ')) {
        auto processedWord = remove_space(normalize(word));
        if (!processedWord.empty())
            verseInWords.push_back(processedWord);
    }

    return verseInWords;
}


vector<Triad> TermFrequency::getBookLengthList() {
    vector<Triad> BookLengthList;

    for (auto it = verses.begin(); it != verses.end(); ++it) {
        string Bookname = it->book;
        if ((it->book) != ((it+1)->book)) {
            Triad BookChapterVerse = {Bookname, it->chapter, it->verse};
            BookLengthList.push_back(BookChapterVerse);
        }
    }
    return BookLengthList;
}

string TermFrequency::normalize(const string &raw) {
    string word = raw;

    // uncapitalize
    transform(word.begin(), word.end(), word.begin(), ::tolower);

    for(int i = 0, len = word.size(); i < len; i++) {
        if ((word[i] < 'a' || word[i] > 'z') && word[i] != ' ') {
            word.erase(i--, 1);
            len = word.size();
        }
    }

    return word;
}

bool cmp(const pair<string,double> &a, const pair<string,double> &b) {
	return a.second < b.second;
}
