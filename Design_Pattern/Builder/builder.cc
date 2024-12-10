#include <iostream>
#include <string>
#include <vector>
#include <memory>

// 产品类，构建他的方式有很多种
class Product
{
public:
    std::vector<std::string> _parts;//放置的建造方案
    void ListParts()const{
        std::cout << "Product parts: ";
        for (size_t i = 0;i < _parts.size();i++){
            if(_parts[i] == _parts.back()){
                std::cout << _parts[i];
            }else{
                std::cout << _parts[i] << ", ";
            }
        }
        std::cout << "\n\n"; 
    }
};


// 建造者基类
// 他具有不同的建造方案，不同的建造方案可以建成不同的对象
class builder
{
public:
    virtual void ProducePartA() = 0;
    virtual void ProducePartB() = 0;
    virtual void ProducePartC() = 0;
};

//具体建造者
class ConcreteBuilder1 : public builder
{
public:
    ConcreteBuilder1()
    {
        Reset();
    }
    ~ConcreteBuilder1()
    {}

    void Reset(){
        _product= new Product();
    }

    void ProducePartA() override
    {
        _product->_parts.push_back("PartA1");
    }

    void ProducePartB() override
    {
        _product->_parts.push_back("PartB1");
    }

    void ProducePartC()     
    {
        _product->_parts.push_back("PartC1");
    }

    Product* GetProduct() {
        Product* result = _product;
        Reset();//强调，我将这个产品提交出去后，是需要重新再申请一个新的产品基类的
        return result;
    }
private:
    Product* _product;
};

class Director
{
public:
    void set_builder(builder* builder)
    {
        _builder = builder;
    }

    void BuildMinimalViableProduct(){
        _builder->ProducePartA();
    }
    
    void BuildFullFeaturedProduct(){
        _builder->ProducePartA();
        _builder->ProducePartB();
        _builder->ProducePartC();
    }

private:
    builder* _builder;
};

void ClientCode(Director& director)
{
    ConcreteBuilder1* builder = new ConcreteBuilder1();
    director.set_builder(builder);
    std::cout << "basic product:\n"; 
    director.BuildMinimalViableProduct();
    Product* p= builder->GetProduct();
    p->ListParts();
    delete p;

    std::cout << "Standard full featured product:\n"; 
    director.BuildFullFeaturedProduct();
    p= builder->GetProduct();
    p->ListParts();
    delete p;

    std::cout << "Custom product:\n";
    builder->ProducePartA();
    builder->ProducePartC();
    p=builder->GetProduct();
    p->ListParts();
    delete p;
    delete builder;
}




int main()
{
    Director* director= new Director();
    ClientCode(*director);
    delete director;
    return 0;   
    return 0;
}