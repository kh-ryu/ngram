# ngram

This C++ project finds the frequency of specific word or phrase in the King James Version of the Bibile. 
The King James Version of the Bible includes 39 books of the Old Testament and the 27 books of the New Testament. 
Its eBook files in HTML, ePub, Kindle, or txt file can be found in Project Gutenberg website [Link](https://www.gutenberg.org/ebooks/10)

### Installation
    git clone https://github.com/kh-ryu/ngram.git
    cd ngram
    cmake .
    make
    ./ngram ./input/pg10.txt "startBook,startChapter,startVerse" "endBook,endChapter,endVerse" "word/phrase"

Example of execution line is as follows

    ./ngram ./input/pg10.txt "The First Book of Moses: Called Genesis,1,1" "Ezra,2,5" "god"

If you want to just test the file, you can use bash script.

    git clone https://github.com/kh-ryu/ngram.git
    cd ngram
    bash run_example.sh

### Manual
This receive 4 arguments.  

1. relative location of txt file
2. Start range of search in the format of "startBook,startChapter,startVerse"
3. End range of search in the format of "endBook,endChapter,endVerse"
4. Word or phrase that you want to search

If you need information about input argument, use `./ngram -h`.
You can have the Book Name, Chapter number, and verse number in [here](https://www.gutenberg.org/files/10/10-h/10-h.htm#The_First_Book_of_Samuel)

After computing the frequency of given word/phrase in given range, 
you will be asked whether you will continue to automatically search evenly/unevenly distributed words/phrases.

Lastly, after searching words, you can save frequency of those words per each Book/Chapter.
The data will be saved as csv file.
