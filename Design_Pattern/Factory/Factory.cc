#include <iostream>

//产品总类
class Product
{
public:
    virtual void show() = 0;
};

//产品A
class ProductA : public Product
{
    void show() override
    {
        std::cout << "ProductA" << std::endl;
    }
};
//产品B
class ProductB : public Product
{
    void show() override
    {
        std::cout << "ProductB" << std::endl;
    }
};

//工厂类
class Factory
{
public:
    virtual Product* CreateProduct() = 0;

    void SomeOperation() {
    // Call the factory method to create a Product object.
    Product* product = this->CreateProduct();
    // Now, use the product.
    std::cout << "Creator: The same creator's code has just worked with ";
    product->show();
    delete product;
  }

};

//具体的产品A类，只生产产品A
class ConcretProductA : public Factory
{
    ProductA* CreateProduct() override
    {
        return new ProductA;
    }
};

//具体的产品B类，只生产产品B
class ConcretProductB : public Factory
{
    ProductB* CreateProduct() override
    {
        return new ProductB;
    }
};

int main()
{
    Factory* factoryA = new ConcretProductA;
    Factory* factoryB = new ConcretProductB;
    factoryA->SomeOperation();
    factoryB->SomeOperation();
    return 0;
}