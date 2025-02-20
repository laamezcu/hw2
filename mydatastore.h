#ifndef MYDATASTORE_H
#define MYDATASTORE_H
#include <string>
#include <set>
#include <vector>
#include <map>
#include <iostream>
#include "datastore.h"
#include "util.h"
#include "product.h"
#include "user.h"

class MyDataStore : public DataStore {
public:
    MyDataStore();
    ~MyDataStore() override;
    void addProduct(Product* p) override;
    void addUser(User* u) override;
    std::vector<Product*> search(std::vector<std::string>& keywords, int type) override;
    void dump(std::ostream& ofile) override;
    void addCart(const std::string& username, int hit);
    void viewCart(const std::string& username);
    void buyCart(const std::string& username);
private:
    std::set<Product*> products;
    std::map<std::string, User*> users;
    std::map<std::string, std::vector<Product*>> uCarts;
    std::vector<Product*> hitResult;
};
#endif