#include "termfrequency.h"

TermFrequency::TermFrequency(const vector<Verse> &verse) {
    verses = verse;
    analyzer = Analyzer(verse);
}

void TermFrequency::findTerms() {

    auto bookLengthList = getBookLengthList();
    cout << "Book Length" << endl;

    auto wordVarList = getWordWithVarList(bookLengthList);
    cout << "word map" << endl;

    // 5 evenly distributed word/phrases
    for (auto it = wordVarList.begin(); it < wordVarList.begin() + 5; ++it){
        cout << it->first << " " << it->second << endl;
    }
    for (auto it = wordVarList.end() - 1; it > wordVarList.end() - 6; --it){
        cout << it->first << " " << it->second << endl;
    }
}

vector<pair<string, double>> TermFrequency::getWordWithVarList
(const vector<Triad> &bookLengthList) {
    vector<pair<string, double>> wordVarList;
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
                newWordsInBook.push_back(word);
                allWords.insert(word);
            }
        }
        auto var = getFreqRSD(newWordsInBook, bookLengthList);
        wordVarList.insert(wordVarList.end(), var.begin(), var.end());
    }

    // sort by size of variance
    sort(wordVarList.begin(), wordVarList.end(), cmp);
    // not interested in words that mean frequency is smaller than 5.0
    for (auto it = wordVarList.begin(); it != wordVarList.end(); ++it) {
        if (it->second < 0.0) {
            wordVarList.erase(it);
        }
        else break;
    }
    return wordVarList;
}

vector<pair<string, double>> TermFrequency::getFreqRSD(const vector<string> &buffer, const vector<Triad> &BookLengthList) {
    map<string, vector<double>> wordFrequencies;

    for (auto it = BookLengthList.begin(); it != BookLengthList.end(); ++it){
        string bookName = get<0>(*it);
        int endChapter = get<1>(*it);
        int endVerse = get<2>(*it);

        Triad start = {bookName, 1, 1};
        Triad end = {bookName, endChapter, endVerse};

        auto freq = analyzer.evaluateFrequency(buffer, start, end);

        for (auto it : freq) {
            auto str = it.first;
            auto frequency = it.second;
            auto iter = wordFrequencies.find(str);
            if (iter == wordFrequencies.end()) {  // cannot find str
                vector<double> temp;
                temp.push_back(frequency);
                wordFrequencies[str] = temp;
            }
            else {
                auto temp = iter->second;
                temp.push_back(frequency);
                iter->second = temp;
            }
        }
    }
    if (wordFrequencies.size() != buffer.size()){
        cout << "something missed during function freqVariance" << endl;
        exit(100);
    }

    int totalWords = (int)wordFrequencies.size();
    int totalBooks = (int)BookLengthList.size();

    vector<pair<string, double>> wordVarList;
    for (auto it = wordFrequencies.begin(); it != wordFrequencies.end(); ++it){
        double sum = 0.0;
        auto freqPerBook = it->second;
        for (auto j = freqPerBook.begin(); j != freqPerBook.end(); ++j)
            sum += (*j);
        auto Mean = sum / totalBooks;
        
        double Var = 0.0;
        if (Mean < 5.0) {
            Var = -5.0;
        } else {
            for (auto j = freqPerBook.begin(); j != freqPerBook.end(); ++j) {
                Var += ((*j) - Mean)*((*j) - Mean);
            }
            Var = Var / totalBooks;
        }
        double RSD = sqrt(Var) / Mean;
        wordVarList.push_back({it->first, RSD});
    }

    return wordVarList;
}

pair<double, double> 
TermFrequency::getFreqMeanVariance (const string &word, const vector<Triad> &BookLengthList, const vector<Verse> &verses) {
    
    vector<double> frequencies;
    Triad start = {"TheFirstBookofMoses:CalledGenesis", 1, 1};
    Triad end = {"TheRevelationofSaintJohntheDivine", 22, 21};
    auto frequencyMean = analyzer.evaluateFrequency(word, start, end);
    double freqVar = 0.0;

    if (frequencyMean > 5.0) {
        cout << word << endl;
        for (auto book : BookLengthList) {
            string bookName = get<0>(book);
            int endChapter = get<1>(book);
            int endVerse = get<2>(book);

            start = {bookName, 1, 1};
            end = {bookName, endChapter, endVerse};

            double freq = analyzer.evaluateFrequency(word, start, end);
            frequencies.push_back(freq);
        }
        vector<double> diff(frequencies.size());
        transform(frequencies.begin(), frequencies.end(), diff.begin(), [frequencyMean](double x) {return x - frequencyMean;});
        auto sq_sum = inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
        freqVar = sq_sum/frequencies.size();
    }
    else {
        frequencyMean = 0.0;
        freqVar = 0.0;
    }
    return {frequencyMean, freqVar};
}

vector<string> TermFrequency::getWordsAndPhrases(const string &content) {
    vector<string> contentWords = divideIntoWords(content);
    vector<string> wordAndPhrases;
    string word;

    // consider phrases with maximum 2 words
    for (auto it = contentWords.begin(); it != contentWords.end(); ++it) {
        word = normalize(*it);
        wordAndPhrases.push_back(*it);
        if (it+1 != contentWords.end()) {
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

    while(ss >> word) {
        auto processedWord = normalize(word);
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
