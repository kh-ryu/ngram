#include "termfrequency.h"

TermFrequency::TermFrequency(const vector<Verse> &verse)  : verses(verse) {}

void TermFrequency::findTerms() {
    Trie* trie = new Trie();

    auto BookLengthList = getBookLengthList();
    cout << "Book Length" << endl;

    getWord(BookLengthList, verses, trie);
    cout << "word map" << endl;

    // 5 evenly distributed word/phrases
}

void TermFrequency::getWord(const vector<Triad> &BookLengthList, const vector<Verse> &verses, Trie* trie) {
    for (auto verse : verses) {
        cout << "Book: " << verse.book << endl;
        cout << "Chapter: " << verse.chapter << endl; 
        cout << "Verse: " << verse.verse << endl;
        vector<string> words = WordsandPhrases(verse);
        cout << "got words" << endl;
        for (auto word : words) {
            if (!trie->search(word)) { // if word does not exist in trie
                auto MeanVar = freqMeanVariance(word, BookLengthList, verses);
                auto mean = MeanVar.first;
                auto var = MeanVar.second;
                trie->insert(word, mean, var);
            }
        }
    }
}

pair<double, double> 
TermFrequency::freqMeanVariance (const string &word, const vector<Triad> &BookLengthList, const vector<Verse> &verses) {
    auto analyzer = Analyzer(verses);
    vector<double> frequencies;
    Triad start = {"TheFirstBookofMoses:CalledGenesis", 1, 1};
    Triad end = {"TheRevelationofSaintJohntheDivine", 22, 21};
    auto frequencyMean = analyzer.evaluateFrequency(word, start, end);
    double freqVar = 0.0;

    if (frequencyMean > 1.0) {
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

vector<string> TermFrequency::WordsandPhrases(const Verse &verse) {
    vector<string> contentWords = divideIntoWords(verse);
    vector<string> wordAndPhrases;
    string raw, word;

    // consider phrases with maximum 3 words
    for (auto it = contentWords.begin(); it != contentWords.end(); ++it) {
        raw = *it;
        word = process(raw);
        wordAndPhrases.push_back(*it);
        if (it+1 != contentWords.end()) {
            raw = *it + " " + *(it+1);
            word = process(raw);
            wordAndPhrases.push_back(word);
            if (it+2 != contentWords.end()) {
                raw = *it + " " + *(it+1) + " " + *(it+2);
                word = process(raw);
                wordAndPhrases.push_back(word);
            }
        }
    }
    return wordAndPhrases;
}

vector<string> TermFrequency::divideIntoWords(const Verse &verse) {
    string content = verse.content;
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

    transform(word.begin(), word.end(), word.begin(), ::tolower);

    for(int i = 0, len = word.size(); i < len; i++) {
        if (ispunct(word[i]) || word[i] < 0) {
            word.erase(i--, 1);
            len = word.size();
        }
    }

    return word;
}