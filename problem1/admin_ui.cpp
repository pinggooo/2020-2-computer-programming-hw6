#include "admin_ui.h"

AdminUI::AdminUI(ShoppingDB &db, std::ostream& os): UI(db, os) { }

void AdminUI::add_product(std::string name, int price) {
    // TODO: For problem 1-1
    if (db.add_product(name, price)) {
        os << "ADMIN_UI: " << name << " is added to the database." << std::endl;
    } else {
        os << "ADMIN_UI: Invalid price." << std::endl;
    }
}

void AdminUI::edit_product(std::string name, int price) {
    // TODO: For problem 1-1
    Product* searched = db.search_product(name);
    if (searched == nullptr) {
        os << "ADMIN_UI: Invalid product name." << std::endl;
    } else if (price <= 0) {
        os << "ADMIN_UI: Invalid price." << std::endl;
    } else {
        db.edit_product(searched, price);
        os << "ADMIN_UI: " << name << " is modified from the database." << std::endl;
    }

}

void AdminUI::list_products() {
    // TODO: For problem 1-1
    os << "ADMIN_UI: Products: [";

    std::vector<Product*> productList = db.getProductList();
    for (auto iter = productList.begin(); iter != productList.end(); iter++) {
        if (iter == productList.end() - 1) {
            os << "(" << (*iter)->name << ", " << (*iter)->price << ")";
        } else {
            os << "(" << (*iter)->name << ", " << (*iter)->price << "), ";
        }
    }

    os << "]" << std::endl;
}
