#include "book.h"
#include "util.h"

Book::Book(const std::string& name, double price, int qty, const std::string& isbn, const std::string& author)
    : Product("book", name, price, qty), isbn_(isbn), author_(author) 
    {

    }

std::set<std::string> Book::keywords() const {
    std::set<std::string> keyWords = parseStringToWords(name_);
    std::set<std::string> authorKeys = parseStringToWords(author_);
    keyWords.insert(isbn_);
    keyWords.insert(authorKeys.begin(), authorKeys.end());
    return keyWords;
}

std::string Book::displayString() const {
    return name_ + "\nAuthor: " + author_ + " ISBN: " + isbn_ + "\n" + std::to_string(price_) +
    " " + std::to_string(qty_) + " left.";
}

void Book::dump(std::ostream& os) const {
    os << "book\n" << name_ << "\n" << price_ << "\n" << qty_ << "\n" << isbn_ << "\n" << author_ << std::endl;
}