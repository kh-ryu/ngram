#include "datawriter.h"

DataWriter::DataWriter(const string &givenhead, const vector<Verse> &givenverses, 
                        const pair<vector<string>, vector<string>> &givenwords) {
    head = givenhead;
    verses = givenverses;
    analyzer = Analyzer(givenverses);
    words = givenwords;
}

void DataWriter::saveData(int fast){
    auto unevenWords = words.first;
    auto evenWords = words.second;

    cout << "Saving unevenly distributed Words" << endl;
    writeFreqPerBook(unevenWords, "uneven");

    cout << "Saving evenly distributed words" << endl;
    writeFreqPerBook(evenWords, "even");

    if(fast == 2){
        cout << "Saving unevenly distributed Words(Frequency Per Chapter)" << endl;
        writeFreqPerChapter(unevenWords, "uneven");
        cout << "Saving evenly distributed Words(Frequency Per Chapter)" << endl;
        writeFreqPerChapter(evenWords, "even");
    }
}

void DataWriter::writeFreqPerBook(const vector<string> &words, string name){
    ofstream ofs("./data/" + head + "_" + name + "_FreqPerBook.csv");

    for (auto i = words.begin(); i != words.end(); ++i){
        ofs << *i << ",";
    }
    ofs << '\n';

    auto bookLengthList = TermFrequency(verses).getBookLengthList();

    for (auto it = bookLengthList.begin(); it != bookLengthList.end(); ++it) {
        string bookName = get<0>(*it);
        int endChapter = get<1>(*it);
        int endVerse = get<2>(*it);

        Triad start = {bookName, 1, 1};
        Triad end = {bookName, endChapter, endVerse};

        auto freqThisBook = analyzer.evaluateFrequency(words, start, end);

        for (auto i = words.begin(); i != words.end(); ++i){
            string word = *i;
            ofs << to_string(freqThisBook.at(word)) << ",";
        }
        ofs << '\n';
    }

    ofs.close();
}

void DataWriter::writeFreqPerChapter(const vector<string> &words, string name){
    ofstream ofs("./data/" + head + "_" + name + "_FreqPerChap.csv");

    for (auto i = words.begin(); i != words.end(); ++i){
        ofs << *i << ",";
    }
    ofs << '\n';

    for (int i = 0; i < verses.size(); ++i) {
        int Chapter = verses.at(i).chapter;

        if (Chapter != verses[i+1].chapter){
            string bookName = verses.at(i).book;
            int Verse = verses.at(i).verse;

            Triad start = {bookName, Chapter, 1};
            Triad end = {bookName, Chapter, Verse};

            auto AccumulatedFreq = analyzer.evaluateFrequency(words, start, end);

            for (auto i = words.begin(); i != words.end(); ++i){
                ofs << to_string(AccumulatedFreq.at(*i)) << ",";
            }
            ofs << '\n';
        }
    }

    ofs.close();
}