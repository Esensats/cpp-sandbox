#include <iostream>
#include <ctime>

class GoodsItem
{
private:
    std::string product_name;
    time_t registration_date;
    double consignment_price;
    int product_quantity;
    int invoice_number;

public:
    GoodsItem(
        const std::string &product_name,
        const double &consignment_price = 0,
        const int &product_quantity = 0)
    {
        const time_t now = time(0);
        srand(now);
        this->registration_date = now;
        this->invoice_number = rand() + 1;
        this->consignment_price = consignment_price;
        this->product_quantity = product_quantity;
        this->product_name = product_name;
    }

    std::string get_product_name() { return product_name; }
    void set_product_name(const std::string &new_name) { product_name = new_name; }

    time_t get_registration_date() { return registration_date; }
    std::string get_registration_date_string() { return ctime(&registration_date); }

    double get_consignment_price() { return consignment_price; }
    void set_consignment_price(const double &new_price) { consignment_price = new_price; }

    int get_product_quantity() { return product_quantity; }
    int add_products(const int &num)
    {
        product_quantity += num;
        return product_quantity;
    }
    int subtract_products(const int &num)
    {
        product_quantity -= num;
        return product_quantity;
    }

    double calculate_product_price()
    {
        return consignment_price / product_quantity;
    }
};