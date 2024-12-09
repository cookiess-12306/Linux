#include <iostream>

//抽象产品A系列的，他可以分为不同种类
//比如说给抽象产品A系列一个具体的产品系列是风格A的家具
//那他具体的产品都是基于这个风格去创建的家具，比如是风格A的椅子，或者风格A的桌子
//但他们都同属于风格A的家具系列
class Table
{
public:
    virtual ~Table(){};
    virtual void show() = 0;
};

class ModernTable : public Table
{
    void show() override
    {
        std::cout << "create ModernTable" << std::endl;
    }
};

class ChineseTable : public Table
{
    void show() override
    {
        std::cout << "create ChineseTable" << std::endl;
    }
};



class chair
{
public:
    virtual void show() = 0;
};

class Modernchair : public chair
{
    void show() override
    {
        std::cout << "create Modernchair" << std::endl;
    }
};

class Chinesechair : public chair
{
    void show() override
    {
        std::cout << "create Chinesechair" << std::endl;
    }
};



// 抽象工厂是控制不同工厂生产产品的工厂
// 工厂1生产的产品是(这里的X是任意风格的具体产品)X1系列的产品，生产出来的产品是同一个种类的
// 同理，工厂2生产的产品是X2系列的产品，生产出来的产品也是同一个种类的

//声明：系列是指一个家具风格系列，种类是指具体的类别是什么，比如说现代风格系列的桌子,桌子就是种类
class AbstractFactory
{
public:
    virtual Table* createTable() = 0;
    virtual chair* createChair() = 0; 
};

// 具体生产中式家具的工厂
// 生产出来的产品是相同系列的不同种类的产品
class ChineseFactory : public AbstractFactory
{
    Table* createTable() override
    {
        return new ChineseTable();
    }
    chair* createChair() override
    {
        return new Chinesechair();
    }
};

// 具体生产现代家具的工厂
class ModernFactory : public AbstractFactory
{
    Table* createTable() override
    {
        return new ModernTable();
    }
    chair* createChair() override
    {
        return new Modernchair();
    }
};


void test(AbstractFactory& abstractFactory)
{
    Table* table = abstractFactory.createTable();
    chair* chair = abstractFactory.createChair();
    table->show();
    chair->show();
}


int main()
{
    // 这里定义的具体工厂生产的是一个X1系列的产品，他生产同种类不同风格的工厂
    ChineseFactory* concreteFactory1 = new ChineseFactory();
    test(*concreteFactory1);
    delete concreteFactory1;
    ModernFactory* concreteFactory2 = new ModernFactory();
    test(*concreteFactory2);
    delete concreteFactory2;

    return 0;
}