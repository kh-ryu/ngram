#include "datawriter.h"

DataWriter::DataWriter(const string &givenhead, const vector<Verse> &givenverses, 
                        const pair<vector<string>, vector<string>> &givenwords) {
    head = givenhead;
    verses = givenverses;
    analyzer = Analyzer(givenverses);
    words = givenwords;
}

void DataWriter::saveData(){
    auto unevenWords = words.first;
    auto evenWords = words.second;

    writeFreqPerBook(unevenWords, "uneven");
    writeAccumulatedFreq(unevenWords, "uneven");
    writeFreqPerBook(evenWords, "even");
    writeAccumulatedFreq(evenWords, "even");
}

void DataWriter::writeFreqPerBook(const vector<string> &words, string name){
    ofstream ofs(head + "_" + name + "_FreqPerBook.csv");

    for (auto word : words){
        ofs << word << ",";
    }
    ofs << '\n';

    auto bookLengthList = TermFrequency(verses).getBookLengthList();

    for (auto book : bookLengthList) {
        string bookName = get<0>(book);
        int endChapter = get<1>(book);
        int endVerse = get<2>(book);

        Triad start = {bookName, 1, 1};
        Triad end = {bookName, endChapter, endVerse};

        auto freqThisBook = analyzer.evaluateFrequency(words, start, end);

        for (auto i = words.begin(); i != words.end(); ++i){
            ofs << to_string(freqThisBook[*i]) << ",";
        }
        ofs << '\n';
    }

    ofs.close();
}

void DataWriter::writeAccumulatedFreq(const vector<string> &words, string name){
    ofstream ofs(head + "_" + name + "_AccumulatedFreq.csv");

    for (auto word : words){
                ofs << word << ",";
    }
    ofs << '\n';

    Triad start = {verses.at(0).book, verses.at(0).chapter, verses.at(0).verse};
    int Chapter = 0;
    for (int i = 0; i < verses.size(); ++i) {
        string bookName = verses.at(i).book;
        int Verse = verses.at(i).verse;

        if (Chapter != verses.at(i).chapter){
            Chapter = verses.at(i).chapter;
            Triad end = {bookName, Chapter, Verse};

            auto AccumulatedFreq = analyzer.evaluateFrequency(words, start, end);

            for (auto i = words.begin(); i != words.end(); ++i){
                ofs << to_string(AccumulatedFreq[*i]) << ",";
            }
            ofs << '\n';
        }
    }

    ofs.close();
}