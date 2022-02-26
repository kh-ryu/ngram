#include "termfrequency.h"

TermFrequency::TermFrequency(const vector<Verse> &verse) {
    verses = verse;
    analyzer = Analyzer(verse);
}

void TermFrequency::findTerms() {
    Trie* allwords = new Trie();

    auto BookLengthList = getBookLengthList();
    cout << "Book Length" << endl;

    auto word_var = getWord(BookLengthList, allwords);
    cout << "word map" << endl;

    // 5 evenly distributed word/phrases
    for (auto it = word_var.begin(); it < word_var.begin() + 5; ++it){
        cout << it->first << " " << it->second << endl;
    }
    for (auto it = word_var.end() - 1; it > word_var.end() - 6; --it){
        cout << it->first << " " << it->second << endl;
    }
}

vector<pair<string, double>> TermFrequency::getWord
(const vector<Triad> &BookLengthList, Trie* trie) {
    vector<pair<string, double>> word_var;
    for (auto it = BookLengthList.begin(); it != BookLengthList.end(); ++it) {
        cout << "Book: " << get<0>(*it) << endl;
        cout << "Chapter: " << get<1>(*it) << endl; 
        cout << "Verse: " << get<2>(*it) << endl;

        string bookName = get<0>(*it);
        int endChapter = get<1>(*it);
        int endVerse = get<2>(*it);

        Triad start = {bookName, 1, 1};
        Triad end = {bookName, endChapter, endVerse};

        string content = process(analyzer.mergeVerses(start, end));

        vector<string> words = WordsandPhrases(content);
        vector<string> buffer;
        for (auto word : words) {
            if (!trie->search(word)) { // if word does not exist in trie
                buffer.push_back(word);
                trie->insert(word);
            }
        }
        auto var = freqVariance(buffer, BookLengthList);
        word_var.insert(word_var.end(), var.begin(), var.end());
    }

    // sort by size of variance
    sort(word_var.begin(), word_var.end(), cmp);
    // not interested in words that mean frequency is smaller than 5.0
    for (auto it = word_var.begin(); it != word_var.end(); ++it) {
        if (it->second < 0.0) {
            word_var.erase(it);
        }
        else break;
    }
    return word_var;
}

vector<pair<string, double>> TermFrequency::freqVariance(const vector<string> &buffer, const vector<Triad> &BookLengthList) {
    map<string, vector<double>> word_frequencies;

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
            auto iter = word_frequencies.find(str);
            if (iter == word_frequencies.end()) {  // cannot find str
                vector<double> temp;
                temp.push_back(frequency);
                word_frequencies[str] = temp;
            }
            else {
                auto temp = iter->second;
                temp.push_back(frequency);
                iter->second = temp;
            }
        }
    }
    if (word_frequencies.size() != buffer.size()){
        cout << "something missed during function freqVariance" << endl;
        exit(100);
    }

    int totalWords = (int)word_frequencies.size();
    int totalBooks = (int)BookLengthList.size();

    vector<pair<string, double>> word_var;
    for (auto it = word_frequencies.begin(); it != word_frequencies.end(); ++it){
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
        word_var.push_back({it->first, Var});
    }

    return word_var;
}

pair<double, double> 
TermFrequency::freqMeanVariance (const string &word, const vector<Triad> &BookLengthList, const vector<Verse> &verses) {
    
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

vector<string> TermFrequency::WordsandPhrases(const string &content) {
    vector<string> contentWords = divideIntoWords(content);
    vector<string> wordAndPhrases;
    string word;

    // consider phrases with maximum 2 words
    for (auto it = contentWords.begin(); it != contentWords.end(); ++it) {
        word = process(*it);
        wordAndPhrases.push_back(*it);
        if (it+1 != contentWords.end()) {
            word = process(*it + " " + *(it+1));
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
        auto processedWord = process(word);
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

string TermFrequency::process(const string &raw) {
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