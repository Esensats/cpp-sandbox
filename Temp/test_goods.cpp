#include <iostream>
#include "GoodsItem.h"

int main()
{
    using namespace std;

    GoodsItem consignment = GoodsItem("Coca-Cola 0.5l", 50000, 100);

    cout << "\nProduct: " << consignment.get_product_name() << endl;
    cout << "Consignment registration date: " << consignment.get_registration_date_string();
    cout << "Consignment price: " << consignment.get_consignment_price() << endl;
    cout << "Quantity: " << consignment.get_product_quantity() << endl;
    cout << "Price per single unit: " << consignment.calculate_product_price() << endl;

    cout << "\nAdding 20 products..." << endl;
    consignment.add_products(20);
    cout << "Quantity: " << consignment.get_product_quantity() << endl;
    cout << "Price per single unit: " << consignment.calculate_product_price() << endl;

    cout << "\nRemoving 70 products..." << endl;
    consignment.subtract_products(70);
    cout << "Quantity: " << consignment.get_product_quantity() << endl;
    cout << "Price per single unit: " << consignment.calculate_product_price() << endl;

    cout << "\nHalving consignment price..." << endl;
    consignment.set_consignment_price(consignment.get_consignment_price() / 2);
    cout << "Consignment price: " << consignment.get_consignment_price() << endl;
    cout << "Price per single unit: " << consignment.calculate_product_price() << '\n'
         << endl;

    return 0;
}
