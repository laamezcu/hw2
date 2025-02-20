#include "mydatastore.h"
#include <iostream>
#include <fstream>

MyDataStore::MyDataStore() {

}

MyDataStore::~MyDataStore() {
    for (std::set<Product*>::iterator pit = products.begin(); pit != products.end(); ++pit) {
        delete *pit;
    }
    for (std::map<std::string, User*>::iterator uit = users.begin(); uit != users.end(); ++uit) {
        delete uit->second;
    }
}

void MyDataStore::addProduct(Product* p) {
    products.insert(p);
}

void MyDataStore::addUser(User* u) {
    std::string uname = convToLower(u->getName());
    users[uname] = u;
}

std::vector<Product*> MyDataStore::search(std::vector<std::string>& keywords, int type) {
    std::set<Product*> result;
    if (keywords.empty()) return hitResult;
    std::set<std::string> keyTerm;
    std::set<Product*> match;
    std::set<Product*> keyProducts;
    for (size_t i = 0; i < keywords.size(); i++) {
        keyTerm.insert(convToLower(keywords[i]));
    }
    for (size_t j = 0; j < keywords.size(); j++) {
        const std::string& keyword = convToLower(keywords[j]);
        for (std::set<Product*>::iterator it = products.begin(); it != products.end(); ++it) {
            std::set<std::string> productKeys = (*it)->keywords();
            Product* p = *it;
            if (!type) {
                std::set<std::string> intersect = setIntersection(keyTerm, productKeys);
                if (intersect.size() == keyTerm.size()) {
                    result.insert(p);
                }
            }
            else {
                if (productKeys.find(keyword) != productKeys.end()) {
                    keyProducts.insert(p);
                }
            }
        }
        if (type == 1) {
            if (j == 0) {
                result = keyProducts;
            }
        }
    }
    if (type == 1) {
        result = setUnion(result, keyProducts);
    }
    hitResult.clear();
    for (std::set<Product*>::iterator it = result.begin(); it != result.end(); ++it) {
        hitResult.push_back(*it);
    }
    return hitResult;
}

void MyDataStore::dump(std::ostream& ofile) {
    ofile << "<products>" << std::endl;
    for (std::set<Product*>::iterator it = products.begin(); it != products.end(); ++it) {
        (*it)->dump(ofile);
    }
    ofile << "</products>" << std::endl;
    ofile << "<users>" << std::endl;
    for (std::map<std::string, User*>::iterator it = users.begin(); it != users.end(); ++it) {
        it->second->dump(ofile);
    }
    ofile << "</users>" << std::endl;
}

void MyDataStore::addCart(const std::string& username, int hit) {
    std::string uname = convToLower(username);
    if (users.find(uname) == users.end() || hit < 0 || hit >= (int)hitResult.size()) {
        std::cout << "Invalid request" << std::endl;
        return;
    }
    uCarts[uname].push_back(hitResult[hit]);
}

void MyDataStore::viewCart(const std::string& username) {
    std::string uname = convToLower(username);
    if (users.find(uname) == users.end()) {
        std::cout << "Invalid username" << std::endl;
        return;
    }
    const std::vector<Product*>& uCart = uCarts[uname];
    if (uCart.empty()) {
        std::cout << "Empty" << std::endl;
        return;
    }
    for (size_t i = 0; i < uCart.size(); i++) {
        std::cout << "Item " << i + 1 << ":" << std::endl;
        std::cout << uCart[i]->displayString() << std::endl;
        std::cout << std::endl;
    }
}

void MyDataStore::buyCart(const std::string& username) {
    std::string uname = convToLower(username);
    if (users.find(uname) == users.end()) {
        std::cout << "Invalid username" << std::endl;
        return;
    }
    User* user = users[uname];
    std::vector<Product*>& uCart = uCarts[uname];
    std::vector<Product*> newCart;
    for (size_t i = 0; i < uCart.size(); i++) {
        Product* p = uCart[i];
        if (p->getQty() > 0 && user->getBalance() >= p->getPrice()) {
            user->deductAmount(p->getPrice());
            p->subtractQty(1);
        }
        else {
            newCart.push_back(p);
        }
    }
    uCarts[uname] = newCart;
}